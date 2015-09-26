.. index::
    single: Forms
    single: Components; Form

Le composant Formulaire
=======================

    Le composant Form vous permet de créer facilement, traiter et réutiliser
    des formulaires HTML.

Le composant Form est un outil pour vous aider à résoudre le problème
de permettre aux utilisateurs finaux d'intéragir et modifier des données
dans votre application. Et c'est via les formulaires HTML que le
composant se concentre pour traiter des données soumises par l'utilisateur
pour et depuis votre application, que la donnée provienne d'un formulaire
posté normalement ou d'une API.

Installation
------------

Vous pouvez installer le composant de deux manières différentes :

* :doc:`L'installation via Composer </components/using_components>` (``symfony/form`` on `Packagist`_);
* Utilisez le repository Git officiel (https://github.com/symfony/Form).

Configuration
-------------

.. tip::

    Si vous travaillez avec le framework Symfony full stack, le composant Form est
    déjà configuré pour vous. Dans ce cas, passez directement à la section
    :ref:`component-form-intro-create-simple-form`.

Dans Symfony2, les formulaires sont représentés par des objets et ceux-ci
sont construits en utilisant une *form factory* (usine à formulaire). Il est simple de
construire une form factory::

    use Symfony\Component\Form\Forms;

    $formFactory = Forms::createFormFactory();

Cette factory peut déjà être utilisée pour créer des formulaires basiques,
mais il lui manque des fonctionnalités très importantes :

* **Manipulation de la requête :** Support de la manipulation de l'objet Request
  et de téléchargement de fichiers;
* **Protection CSRF :** Support de la protection contre les attaques
  Cross-Site-Request-Forgery (CSRF);
* **Templating :** Intégration avec une couche templating vous permettant
  de réutiliser des fragments HTML lors de l'affichage du formulaire;
* **Traduction :** Support des traductions des messages d'erreur, champs
  labelisés et autre chaînes de caractères;
* **Validation :** Intégration avec la bibliothèque de validation pour
  générer les messages d'erreurs pour les données soumises.

Le composant Form de Symfony2 est basé sur d'autres bibliothèques pour
résoudre ces problèmes. La majorité du temps vous utiliserez Twig ainsi que
les composants :doc:`HttpFoundation </components/http_foundation/introduction>`,
Traduction et Validation. Ceci dit, vous pouvez remplacer chacune de ces
bibliothèques par celle(s) de votre choix.

Les sections suivantes vous expliquent comment brancher ces bibliothèques avec la
form factory.

.. tip::

    Pour un exemple fonctionnel, consultez https://github.com/bschussek/standalone-forms

Manipulation de la Requête
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    La méthode ``handleRequest()`` a été ajoutée dans la version 2.3 de Symfony.

Pour traiter les données d'un formulaire, vous aurez besoin de la méthode
:method:`Symfony\\Component\\Form\\Form::handleRequest`::

    $form->handleRequest();

En coulisses, cette méthode utilise un objet de type
:class:`Symfony\\Component\\Form\\NativeRequestHandler` pour lire les données
provenants des variables super-globales PHP (i.e. ``$_POST``ou ``$_GET``) basées sur
la méthode HTTP configurée sur le formulaire (POST par défaut).

.. sidebar:: Intégration avec le composant HttpFoundation

    Si vous utilisez le composant HttpFoundation, vous devriez alors
    ajouter la :class:`Symfony\\Component\\Form\\Extension\\HttpFoundation\\HttpFoundationExtension`
    à votre form factory::

        use Symfony\Component\Form\Forms;
        use Symfony\Component\Form\Extension\HttpFoundation\HttpFoundationExtension;

        $formFactory = Forms::createFormFactoryBuilder()
            ->addExtension(new HttpFoundationExtension())
            ->getFormFactory();

    Maintenant, lorsque vous traitez un formulaire, vous pouvez passer l'objet
    :class:`Symfony\\Component\\HttpFoundation\\Request` à la méthode
    :method:`Symfony\\Component\\Form\\Form::handleRequest`::

        $form->handleRequest($request);

    .. note::

        Pour plus d'informations concernant le composant HttpFoundation ou
        comment l'installer, consultez :doc:`/components/http_foundation/introduction`.

Protection CRSF
~~~~~~~~~~~~~~~

La protection contre les attaques CSRF est incluse dans le composant formulaire,
mais vous devez explicitement l'activer ou la remplacer par une solution
personnalisée::

    use Symfony\Component\Form\Forms;
    use Symfony\Component\Form\Extension\Csrf\CsrfExtension;
    use Symfony\Component\Form\Extension\Csrf\CsrfProvider\SessionCsrfProvider;
    use Symfony\Component\HttpFoundation\Session\Session;

    // générer le secret CSRF depuis quelque part
    $csrfSecret = '<generated token>';

    // créer un objet session depuis le composant HttpFoundation
    $session = new Session();

    $csrfProvider = new SessionCsrfProvider($session, $csrfSecret);

    $formFactory = Forms::createFormFactoryBuilder()
        // ...
        ->addExtension(new CsrfExtension($csrfProvider))
        ->getFormFactory();

Pour sécuriser votre application contre les attaques CSRF, vous devez définir
un sercret CSRF. Générer une chaîne de caractères aléatoires avec au moins
32 caractères, insérez-les dans le morceau de code ci-dessus et assurez vous
que personne d'autre que votre server web ne peut accéder à ce secret.

En interne, cette extension ajoutera automatiquement un champ caché à chacun
des formulaires (appelé ``__token`` par défaut) dont la valeur est automatiquement
générée et validée lors du binding (liaison) du formulaire.

.. tip::

    Si vous n'utilisez pas le composant HttpFoundation, vous pouvez utiliser
    :class:`Symfony\\Component\\Form\\Extension\\Csrf\\CsrfProvider\\DefaultCsrfProvider`
    à la place, qui se repose sur la manipulation de session native de PHP::

        use Symfony\Component\Form\Extension\Csrf\CsrfProvider\DefaultCsrfProvider;

        $csrfProvider = new DefaultCsrfProvider($csrfSecret);

Le Templating avec Twig
~~~~~~~~~~~~~~~~~~~~~~~

Si vous utilisez le composant formulaire pour traiter des formulaires HTML,
vous aurez besoin d'une solution pour afficher facilement les champs de votre
formulaire HTML (compléter avec les valeurs des champs, les erreurs et les libellés). Si vous
utilisez `Twig`_ comme moteur de rendu, le composant Form offre une intégration
riche.

Pour utiliser cette intégration, vous aurez besoin de ``TwigBridge``, fournissant
une intégration entre Twig et quelques composants Symfony2. Si vous utilisez Composer,
vous pouvez installer la dernière version 2.4 en ajoutant la ligne ``require``
suivante dans votre fichier ``composer.json``:

.. code-block:: json

    {
        "require": {
            "symfony/twig-bridge": "2.4.*"
        }
    }

L'intégration de TwigBridge vous fournit un certain nombre de
:doc:`fonctions Twig </reference/forms/twig_reference>` vous aidant à présenter
chacun des widgets HTML, libellés et erreurs pour chaque champs (ainsi quelques autres
petites choses). Pour configurer cette intégration, vous aurez besoin de
bootstrapper ou accéder à Twig et ajouter la classe
:class:`Symfony\\Bridge\\Twig\\Extension\\FormExtension`::

    use Symfony\Component\Form\Forms;
    use Symfony\Bridge\Twig\Extension\FormExtension;
    use Symfony\Bridge\Twig\Form\TwigRenderer;
    use Symfony\Bridge\Twig\Form\TwigRendererEngine;

    // le fichier Twig contenant toutes les balises pour afficher les formulaires
    // ce fichier vient avoir le TwigBridge
    $defaultFormTheme = 'form_div_layout.html.twig';

    $vendorDir = realpath(__DIR__ . '/../vendor');
    // le chemin vers TwigBridge pour que Twig puisse localiser
    // le fichier form_div_layout.html.twig
    $vendorTwigBridgeDir =
        $vendorDir . '/symfony/twig-bridge/Symfony/Bridge/Twig';
    // le chemin vers les autres templates
    $viewsDir = realpath(__DIR__ . '/../views');

    $twig = new Twig_Environment(new Twig_Loader_Filesystem(array(
        $viewsDir,
        $vendorTwigBridgeDir . '/Resources/views/Form',
    )));
    $formEngine = new TwigRendererEngine(array($defaultFormTheme));
    $formEngine->setEnvironment($twig);
    // ajoutez à Twig la FormExtension
    $twig->addExtension(
        new FormExtension(new TwigRenderer($formEngine, $csrfProvider))
    );

    // créez votre form factory comme d'habitude
    $formFactory = Forms::createFormFactoryBuilder()
        // ...
        ->getFormFactory();

Votre `Configuration Twig`_ peut varier, mais le but
est toujours d'ajouter l'extension :class:`Symfony\\Bridge\\Twig\\Extension\\FormExtension`
à Twig, ce qui vous donne accès au fonctions twig pour afficher les formulaires.
Pour faire cela, il vous faut premièrement créer un
:class:`Symfony\\Bridge\\Twig\\Form\\TwigRendererEngine`, où vous définissez vos
:ref:`form themes <cookbook-form-customization-form-themes>` (i.e. resources/fichiers définissant
votre balisage de formulaire HTML).

Pour plus de détails concernant l'affichage de formulaires, consultez :doc:`/cookbook/form/form_customization`.

.. note::

    Si vous utilisez une intégration avec Twig, lisez ":ref:`component-form-intro-install-translation`"
    ci-dessous pour les détails nécessaires aux filtres de traduction.

.. _component-form-intro-install-translation:

Traduction
~~~~~~~~~~

Si vous utilisez une intégration avec Twig avec l'un des fichiers
de thème de formulaire par défaut (par exemple ``form_div_layout.html.twig``),
il y a deux filtres Twig (``trans``et ``transChoice``) qu'il faut
utiliser pour la traduction des libellés, erreurs, texte en option et
autres chaînes de caractères d'un formulaire.

Pour ajouter ces filtres Twig, vous pouvez soit utiliser ceux fournis
par défaut dans la classe :class:`Symfony\\Bridge\\Twig\\Extension\\TranslationExtension`
qui est intégrée avec le composant Traduction de Symfony, ou ajouter
ces deux filtres vous-même, via une extension Twig.

Pour utiliser l'intégration fournie par défaut, assurez-vous que votre projet
dispose des composants de Symfony Traduction et doc:`Config </components/config/introduction>`
installés. Si vous utilisez Composer, vous pouvez récupérer la dernière
version 2.4 de ces composants en ajoutant les lignes suivantes à votre
fichier ``composer.json`` :

.. code-block:: json

    {
        "require": {
            "symfony/translation": "2.4.*",
            "symfony/config": "2.4.*"
        }
    }

Ensuite, ajoutez la classe :class:`Symfony\\Bridge\\Twig\\Extension\\TranslationExtension`
à votre instance de ``Twig_Environment``::

    use Symfony\Component\Form\Forms;
    use Symfony\Component\Translation\Translator;
    use Symfony\Component\Translation\Loader\XliffFileLoader;
    use Symfony\Bridge\Twig\Extension\TranslationExtension;

    // instancier un objet de la classe Translator
    $translator = new Translator('en');
    // charger, en quelque sorte, des traductions dans ce translator
    $translator->addLoader('xlf', new XliffFileLoader());
    $translator->addResource(
        'xlf',
        __DIR__.'/path/to/translations/messages.en.xlf',
        'en'
    );

    // ajoutez le TranslationExtension (nous donnant les filtres trans et transChoice)
    $twig->addExtension(new TranslationExtension($translator));

    $formFactory = Forms::createFormFactoryBuilder()
        // ...
        ->getFormFactory();

En fonction de la manière dont vos traductions sont chargées, vous pouvez maintenant
ajouter des clés, comme des libellés de champs, et leurs traductions dans vos
fichiers de traductions.

Pour plus de détails sur les traductions, consulter :doc:`/book/translation`.

Validation
~~~~~~~~~~

Le composant Form vient avec une petite (mais optionnelle) intégration du
composant Validation de Symfony. Si vous utilisez une solution différente
pour la validation, pas de problème ! Prenez simplement les données
soumises/liées de votre formulaire (qui sont contenues dans un tableau
ou un objet) et passez les à votre propre système de validation.

Pour utiliser l'intégration avec le composant Validation, premièrement
assurez-vous qu'il est installé dans votre application. Si vous utilisez
Composer et que vous souhaitez installer la dernière version 2.4, ajoutez
ceci à votre ``composer.json`` :

.. code-block:: json

    {
        "require": {
            "symfony/validator": "2.3.*"
        }
    }

Si vous n'êtes pas familiez avec le composant Validation de Symfony, lisez-en
plus à son propos : :doc:`/book/validation`. Le composant Form vient avec la
classe :class:`Symfony\\Component\\Form\\Extension\\Validator\\ValidatorExtension`,
qui applique automatiquement la validation lorsque vos données sont liées.
Ces erreurs sont ensuite mappées au bon champs et affichées.

Votre intégration avec le composant Validation ressemblera à ceci::

    use Symfony\Component\Form\Forms;
    use Symfony\Component\Form\Extension\Validator\ValidatorExtension;
    use Symfony\Component\Validator\Validation;

    $vendorDir = realpath(__DIR__ . '/../vendor');
    $vendorFormDir = $vendorDir . '/symfony/form/Symfony/Component/Form';
    $vendorValidatorDir =
        $vendorDir . '/symfony/validator/Symfony/Component/Validator';

    // créez le validator - les détails varieront
    $validator = Validation::createValidator();

    // il y a des traductions fournies pour les messages d'erreurs du coeur du composant
    $translator->addResource(
        'xlf',
        $vendorFormDir . '/Resources/translations/validators.en.xlf',
        'en',
        'validators'
    );
    $translator->addResource(
        'xlf',
        $vendorValidatorDir . '/Resources/translations/validators.en.xlf',
        'en',
        'validators'
    );

    $formFactory = Forms::createFormFactoryBuilder()
        // ...
        ->addExtension(new ValidatorExtension($validator))
        ->getFormFactory();

Pour en apprendre plus, allez directement à la session :ref:`component-form-intro-validation`.

Accéder à la Form Factory
~~~~~~~~~~~~~~~~~~~~~~~~~

Votre application n'a besoin que d'une form factory (usine de formulaire),
et cette unique factory d'objet devrait être utilisée pour créer tous les
objets Form dans votre application. Cela signifie que vous devriez la
créer à un endroit central, au moment où votre application est bootstrappée
puis y accéder lorsque vous souhaitez construire un formulaire.

.. note::

    Dans ce document, la form factory est toujours dans une variable locale
    appelée ``$formFactory``. Le but ici est que vous aurez probablement
    besoin de créer cet objet de façon plus "globale" de manière à ce que
    vous puissiez y accéder depuis n'importe où.

C'est à vous de déterminer la manière dont vous accéderez à cette form factory.
Si utilisez un :term:`Conteneur de services`, vous devriez
dont ajouter cette form factory à votre conteneur et le récupérer lorsque vous
en aurez besoin. Si votre application utilise des variables globales ou statiques
(pas une bonne idée en général), vous pouvez alors garder l'objet dans une classe
statique ou une solution similaire.

Sans prêter attention à comment vous avez conçu votre application,
souvenez-vous simplement que vous devriez n'avoir qu'une seule form factory
et que vous aurez besoin d'y accéder partout dans votre application.

.. _component-form-intro-create-simple-form:

Création d'un formulaire simple
-------------------------------

.. tip::

    Si vous utilisez le framework Symfony2, alors la form factory est disponible
    automatiquement comme service et est appelé ``form.factory``. Aussi, la classe de
    contrôleur de base par défaut possède la méthode
    :method:`Symfony\\Bundle\\FrameworkBundle\\Controller::createFormBuilder`,
    qui est un raccourci pour récupérer la form factory et appelle ``createBuilder``
    dessus.

La création d'un formulaire est faite via un objet
:class:`Symfony\\Component\\Form\\FormBuilder`, où vous construisez et configurez
les champs du formulaire. Le form builder est créé depuis la form factory.

.. configuration-block::

    .. code-block:: php-standalone

        $form = $formFactory->createBuilder()
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

        echo $twig->render('new.html.twig', array(
            'form' => $form->createView(),
        ));

    .. code-block:: php-symfony

        // src/Acme/TaskBundle/Controller/DefaultController.php
        namespace Acme\TaskBundle\Controller;

        use Symfony\Bundle\FrameworkBundle\Controller\Controller;
        use Symfony\Component\HttpFoundation\Request;

        class DefaultController extends Controller
        {
            public function newAction(Request $request)
            {
                // createFormBuilder est un raccourci pour récupérer le "form factory"
                // et appellera ensuite la méthode "createBuilder()"
                $form = $this->createFormBuilder()
                    ->add('task', 'text')
                    ->add('dueDate', 'date')
                    ->getForm();

                return $this->render('AcmeTaskBundle:Default:new.html.twig', array(
                    'form' => $form->createView(),
                ));
            }
        }

Comme vous pouvez le voir, créer un formulaire est comme écrire une recette :
vous appelez la méthode ``add`` pour chacun des champs que vous souhaitez créer. Le
premier argument de la méthode ``add`` est le nom de votre champ, et le second
est le "type" du champ. Le composant Form est fourni avec beaucoup de
:doc:`types par défaut </reference/forms/types>`.

Maintenant que vous avez construit votre formulaire, apprenez comment
:ref:`l'afficher <component-form-intro-rendering-form>` et effectuer
:ref:`le traitement lors de la soumission de formulaire <component-form-intro-handling-submission>`.

Réglage des valeurs par défaut
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous avez besoin que votre formulaire soit chargé avec quelques valeurs
par défaut (ou que vous construisez un formulaire d'édition), passez
simplement les valeurs par défaut lorsque vous créez votre form builder :

.. configuration-block::

    .. code-block:: php-standalone

        $defaults = array(
            'dueDate' => new \DateTime('tomorrow'),
        );

        $form = $formFactory->createBuilder('form', $defaults)
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

    .. code-block:: php-symfony

        $defaults = array(
            'dueDate' => new \DateTime('tomorrow'),
        );

        $form = $this->createFormBuilder($defaults)
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

.. tip::

    Dans cet exemple, les données par défaut sont dans un tableau. Plus
    tard, lorsque vous utiliserez l'option :ref:`data_class <book-forms-data-class>`
    pour lier les données directement à des objets, vos données par défaut
    seront une instance de cet objet.

.. _component-form-intro-rendering-form:

L'affichage du formulaire
~~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant que le formulaire a été créé, l'étape suivante est de l'afficher.
C'est fait en passant l'objet spécial form "view" à votre template (notez
le ``$form->createView()`` dans le contrôleur ci-dessus) et en utilisant
une suite de fonctions helper du formulaire :

.. code-block:: html+jinja

    <form action="#" method="post" {{ form_enctype(form) }}>
        {{ form_widget(form) }}

        <input type="submit" />
    </form>

.. image:: /images/book/form-simple.png
    :align: center

Et voilà ! En écrivant ``form_widget(form)``, chaque champ du
formulaire est affiché avec son label et le message d'erreur (s'il y
en a un). C'est très facile, mais (pas encore) très flexible. Généralement,
vous voudrez afficher chaque champs de votre formulaire individuellement
ainsi vous pourrez contrôler le look de votre formulaire. Vous apprendrez
comment le faire dans la session ":ref:`form-rendering-template`".

.. _component-form-intro-handling-submission:

Traitement lors de la soumission de formulaire
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour effectuer un traitement lors de la soumission de formulaire, utilisez
la méthode :method:`Symfony\\Component\\Form\\Form::handleRequest` :

.. configuration-block::

    .. code-block:: php-standalone

        use Symfony\Component\HttpFoundation\Request;
        use Symfony\Component\HttpFoundation\RedirectResponse;

        $form = $formFactory->createBuilder()
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

        $request = Request::createFromGlobals();

        $form->handleRequest($request);

        if ($form->isValid()) {
            $data = $form->getData();

            // ... exécutez une action, comme enregistrer des
            // données dans une base de données

            $response = new RedirectResponse('/task/success');
            $response->prepare($request);

            return $response->send();
        }

        // ...

    .. code-block:: php-symfony

        // ...

        public function newAction(Request $request)
        {
            $form = $this->createFormBuilder()
                ->add('task', 'text')
                ->add('dueDate', 'date')
                ->getForm();

            $form->handleRequest($request);

            if ($form->isValid()) {
                $data = $form->getData();

                // ... exécutez une action, comme enregistrer des
                // données dans une base de données

                return $this->redirect($this->generateUrl('task_success'));
            }

            // ...
        }

Ceci définit le "workflow" commun, contenant 3 possibilités différentes :

1) Sur la requête en GET initiale (i.e. lorsque l'utilisateur "surfe" sur
   votre page), il y a construction du formulaire et affichage de celui-ci;

Si la requête est en POST, traitez les données soumises (via ``handleRequest()``).
Puis :

2) si le formulaire est invalide, affichez à nouveau le formulaire (qui contiendra
maintenant les erreurs);
3) si le formulaire est valide, effectuez les actions nécessaires et redirigez
l'utilisateur.

Heureusement, vous n'avez pas besoin de décider si oui ou non un formulaire
a été soumis. Passez simplement la requête courante à la méthode ``handleRequest()``.
Puis, le composant formulaire fera tout ce qui est nécessaire à votre place.

.. _component-form-intro-validation:

Validation de formulaire
~~~~~~~~~~~~~~~~~~~~~~~~

La façon la plus simple pour ajouter de la validation à votre formulaire
est de le faire via l'option ``constraints`` lors de la construction de
chaque champ :

.. configuration-block::

    .. code-block:: php-standalone

        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Type;

        $form = $formFactory->createBuilder()
            ->add('task', 'text', array(
                'constraints' => new NotBlank(),
            ))
            ->add('dueDate', 'date', array(
                'constraints' => array(
                    new NotBlank(),
                    new Type('\DateTime'),
                )
            ))
            ->getForm();

    .. code-block:: php-symfony

        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Type;

        $form = $this->createFormBuilder()
            ->add('task', 'text', array(
                'constraints' => new NotBlank(),
            ))
            ->add('dueDate', 'date', array(
                'constraints' => array(
                    new NotBlank(),
                    new Type('\DateTime'),
                )
            ))
            ->getForm();

Lorsque le formulaire est bindé (lié), ces contraintes de validation seront
automatiquement appliquées et les erreurs seront affichées près du champ
concerné par l'erreur.

.. note::

    Pour une liste de toutes les contraintes de validation, consultez
    :doc:`/reference/constraints`.

.. _Packagist: https://packagist.org/packages/symfony/form
.. _Twig:      http://twig.sensiolabs.org
.. _`Configuration Twig`: http://twig.sensiolabs.org/doc/intro.html
