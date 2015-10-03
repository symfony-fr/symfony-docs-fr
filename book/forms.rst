.. index::
   single: Forms

Formulaires
===========

Intéragir avec les formulaires HTML est l'une des tâches les plus habituelles
- et stimulantes - pour un développeur web. Symfony2 intègre un composant « Form »
qui rend cette intéraction très facile. Dans ce chapitre, vous allez construire un
formulaire complexe depuis la base, tout en apprenant au fur et à mesure les
caractéristiques les plus importantes de la bibliothèque des formulaires.

.. note::

   Le composant formulaire de Symfony est une bibliothèque autonome qui peut être
   utilisée en dehors des projets Symfony2. Pour plus d'informations, voir le
   `Composant Formulaire Symfony2`_ sur Github.

.. index::
   single: Forms; Create a simple form

Créer un formulaire simple
--------------------------

Supposez que vous construisiez une application « todo list » (en français : « liste
de choses à faire ») simple qui doit afficher des « tâches ». Parce que vos
utilisateurs devront éditer et créer des tâches, vous allez avoir besoin de
construire des formulaires. Mais avant de commencer, concentrez-vous
d'abord sur la classe générique ``Task`` qui représente et stocke les données
pour une tâche::

    // src/Acme/TaskBundle/Entity/Task.php
    namespace Acme\TaskBundle\Entity;

    class Task
    {
        protected $task;

        protected $dueDate;

        public function getTask()
        {
            return $this->task;
        }
        public function setTask($task)
        {
            $this->task = $task;
        }

        public function getDueDate()
        {
            return $this->dueDate;
        }
        public function setDueDate(\DateTime $dueDate = null)
        {
            $this->dueDate = $dueDate;
        }
    }

.. note::

   Si vous codez en même temps que vous lisez cet exemple, créez en premier
   le bundle ``AcmeTaskBundle`` en exécutant la commande suivante (et en
   acceptant toutes les options par défaut):

   .. code-block:: bash

        $ php app/console generate:bundle --namespace=Acme/TaskBundle

Cette classe est un « bon vieil objet PHP » parce que, jusqu'ici, elle n'a rien
à voir avec Symfony ou un quelconque autre bibliothèque. C'est tout simplement
un objet PHP normal qui répond directement à un besoin dans *votre*
application (c-a-d le besoin de représenter une tâche dans votre application).
Bien sûr, à la fin de ce chapitre, vous serez capable de soumettre des données
à une instance de ``Task`` (via un formulaire HTML), de valider ses données,
et de les persister dans la base de données.

.. index::
   single: Forms; Create a form in a controller

Construire le Formulaire
~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant que vous avez créé une classe ``Task``, la prochaine étape est
de créer et d'afficher le formulaire HTML. Dans Symfony2, cela se fait
en construisant un objet formulaire qui est ensuite affiché dans un template.
Pour l'instant, tout ceci peut être effectué dans un contrôleur::

    // src/Acme/TaskBundle/Controller/DefaultController.php
    namespace Acme\TaskBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Acme\TaskBundle\Entity\Task;
    use Symfony\Component\HttpFoundation\Request;

    class DefaultController extends Controller
    {
        public function newAction(Request $request)
        {
            // crée une tâche et lui donne quelques données par défaut pour cet exemple
            $task = new Task();
            $task->setTask('Write a blog post');
            $task->setDueDate(new \DateTime('tomorrow'));

            $form = $this->createFormBuilder($task)
                ->add('task', 'text')
                ->add('dueDate', 'date')
                ->add('save', 'submit')
                ->getForm();

            return $this->render('AcmeTaskBundle:Default:new.html.twig', array(
                'form' => $form->createView(),
            ));
        }
    }

.. tip::

   Cet exemple vous montre comment construire votre formulaire directement
   depuis le contrôleur. Plus tard, dans la section
   « :ref:`book-form-creating-form-classes` », vous apprendrez à construire
   votre formulaire dans une classe autonome, ce qui est recommandé, car vos formulaires
   deviendront ainsi réutilisables.

Créer un formulaire nécessite relativement peu de code, car les objets formulaires
de Symfony2 sont construits avec un « constructeur de formulaire » (« form builder »).
Le principe du constructeur de formulaire est de vous permettre d'écrire des
« conteneurs » de formulaire simples, et de le laisser prendre en charge
la plus grosse partie du travail qu'est la construction du formulaire.

Dans cet exemple, vous avez ajouté deux champs à votre formulaire - ``task`` et
``dueDate`` - correspondants aux propriétés ``task`` et ``dueDate`` de votre
classe ``Task``. Vous avez aussi assigné à chaque champ un « type » (par exemple :
``text``, ``date``), qui, entre autres, détermine quelle(s) balise(s) HTML
est affichée pour ce champ. Enfin, vous avez ajouté un bouton « submit » pour
envoyer le formulaire sur le serveur.

.. versionadded:: 2.3
    Le support des boutons « submit » est une nouveauté de Symfony 2.3. Avant cela,
    vous deviez ajouter vous même les boutons au formulaire HTML.

Symfony2 est livré avec de nombreux types prédéfinis qui seront présentés
rapidement plus tard (voir :ref:`book-forms-type-reference`).

.. index::
  single: Forms; Basic template rendering

Affichage du Formulaire
~~~~~~~~~~~~~~~~~~~~~~~

Maintenant que le formulaire a été créé, la prochaine étape est de l'afficher.
Ceci est fait en passant un objet spécial « vue de formulaire » à votre template
(notez le ``$form->createView()`` dans le contrôleur ci-dessus) et en utilisant
un ensemble de fonctions d'aide (helpers) pour les formulaires :

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}

        {{ form(form) }}

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Default/new.html.php -->

        <?php echo $view['form']->form($form) ?>

.. image:: /images/book/form-simple.png
    :align: center

.. note::

    Cet exemple suppose que vous soumettez le formulaire grâce à une requête
    « POST », et à la même URL que celle qui l'a affiché. Vous apprendrez plus
    tard comment changer la méthode de la requête et l'URL cible du formulaire.

C'est tout ! En affichant ``form(form)``, chaque champ du formulaire
est affiché, avec un label et un message d'erreur (si erreur il y a). La fonction
``form`` affiche tout dans une balise HTML ``form``. Aussi
facile que cela soit, ce n'est pas (encore) très flexible. Habituellement,
vous voudrez afficher chaque champ du formulaire individuellement afin de
pouvoir contrôler ce à quoi le formulaire ressemble. Vous apprendrez comment
faire cela dans la section «:ref:`form-rendering-template`».

Avant de continuer, notez comment le champ ``task`` affiché possède la
valeur de la propriété ``task`` de l'objet ``$task`` (c-a-d « Write a blog
post »). C'est le premier travail d'un formulaire : de prendre les données
d'un objet et de les traduire dans un format adapté pour être affiché dans
un formulaire HTML.

.. tip::

   Le système de formulaire est assez intelligent pour accéder la valeur de la
   propriété protégée ``task`` via les méthodes ``getTask()`` et ``setTask()``
   de la classe ``Task``. A moins qu'une propriété soit publique, elle *doit*
   avoir une méthode « getter » et un « setter » afin que le composant formulaire
   puisse récupérer et assigner des données à cette propriété. Pour une propriété
   booléenne, vous pouvez utiliser une méthode « isser » ou « hasser » (par exemple :
   ``isPublished()`` ou ``hasReminder()``) à la place d'un getter
   (par exemple : ``getPublished()`` ou ``getReminder()``).

.. index::
  single: Forms; Handling form submissions

.. _book-form-handling-form-submissions:

Gérer la Soumission des Formulaires
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le second travail d'un formulaire est de transmettre les données soumises par
l'utilisateur aux propriétés d'un objet. Pour réaliser cela, les données
soumises par l'utilisateur doivent être liées au formulaire. Ajoutez la
fonctionnalité suivante à votre contrôleur::

    // ...
    use Symfony\Component\HttpFoundation\Request;

    public function newAction(Request $request)
    {
        // just setup a fresh $task object (remove the dummy data)
        $task = new Task();

        $form = $this->createFormBuilder($task)
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->add('save', 'submit')
            ->getForm();

        $form->handleRequest($request);

        if ($form->isValid()) {
            // fait quelque chose comme sauvegarder la tâche dans la bdd

            return $this->redirect($this->generateUrl('task_success'));
        }

        // ...
    }

.. versionadded:: 2.3
    La méthode :method:`Symfony\\Component\\Form\\FormInterface::handleRequest` a été
    ajoutée dans Symfony 2.3. Avant cela, la ``$request`` était passée à la méthode
    ``submit``, une stratégie qui est dépréciée et qui sera supprimée dans Symfony 3.0.
    Pour plus de détails sur cette méthode, lisez :ref:`cookbook-form-submit-request`.

Ce contrôleur suit un pattern commun dans la manière de gérer les formulaires,
et a trois scénarios possibles :

#. Lors du chargement initial de la page dans votre navigateur, le formulaire
   est simplement créé et affiché. :method:`Symfony\\Component\\Form\\FormInterface::handleRequest`
   détermine que le formulaire n'a pas été soumis et ne fait rien.
   :method:`Symfony\\Component\\Form\\FormInterface::isValid` retourne ``false`` si le
   formulaire n'a pas été soumis.

#. Lorsque l'utilisateur soumet le formulaire, :method:`Symfony\\Component\\Form\\FormInterface::handleRequest`
   s'en rend compte et écrit immédiatement les données soumises dans les
   propriétés ``task`` et ``dueDate`` de l'objet ``$task``. Ensuite, cet objet
   est validé. S'il est invalide (la validation est abordée dans la section
   suivante), :method:`Symfony\\Component\\Form\\FormInterface::isValid` retourne
   encore ``false`` et le formulaire est affiché de nouveau avec toutes ses
   erreurs de validation.

   .. note::

       Vous pouvez utiliser la méthode :method:`Symfony\\Component\\Form\\FormInterface::isSubmitted`
       pour vérifier si un formulaire a été soumis ou non, indépendemment du fait
       que les données soumises soient valides ou non.

#. Lorsque l'utilisateur soumet le formulaire avec des données valides, les
   données soumises sont toujours écrites dans l'objet, mais cette fois,
   :method:`Symfony\\Component\\Form\\FormInterface::isValid` retourne ``true``.
   Vous avez alors la possibilité d'effectuer certaines actions qui utilisent
   l'objet ``$task`` (comme le persister en base de données) avant de rediriger
   l'utilisateur vers une autre page (par exemple une page « merci » ou 
   « ça a marché ! »).


.. note::

       Rediriger un utilisateur après une soumission de formulaire réussie empêche
       l'utilisateur de pouvoir rafraichir la page et de resoumettre les données.

.. index::
   single: Forms; Multiple Submit Buttons

.. _book-form-submitting-multiple-buttons:

Gérer la Soumission des Formulaires ayant Plusieurs Boutons
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    Le support des boutons dans les formulaires est une nouveauté de Symfony 2.3.

Lorsque votre formulaire contient plus d'un bouton submit, vous devrez vérifier
quel bouton a été utilisé pour adapter les actions à effectuer dans votre
contrôleur. Ajoutons un second bouton avec l'intitulé « Enregistrer et nouveau »
(save and add) à notre formulaire::

    $form = $this->createFormBuilder($task)
        ->add('task', 'text')
        ->add('dueDate', 'date')
        ->add('save', 'submit')
        ->add('saveAndAdd', 'submit')
        ->getForm();

Dans votre contrôleur, utilisez la méthode
:method:`Symfony\\Component\\Form\\ClickableInterface::isClicked` du bouton pour
vérifier si un clic sur le bouton « Enregistrer et nouveau » a été fait::

    if ($form->isValid()) {
        // fait quelque chose comme sauvegarder la tâche dans la bdd

        $nextAction = $form->get('saveAndAdd')->isClicked()
            ? 'task_new'
            : 'task_success';

        return $this->redirect($this->generateUrl($nextAction));
    }

.. index::
   single: Forms; Validation

Validation de formulaire
------------------------

Dans la section précédente, vous avez appris comment un formulaire peut être
soumis avec des données valides ou non. Dans Symfony2, la validation
est appliquée à l'objet sous-jacent (par exemple : ``Task``). En d'autres termes,
la question n'est pas de savoir si le « formulaire » est valide, mais plutôt de
savoir si l'objet ``$task`` est valide ou non après que le formulaire lui a
appliqué les données. Appeler ``$form->isValid()`` est un raccourci qui demande
à l'objet ``$task`` si oui ou non il possède des données valides.

La validation est effectuée en ajoutant un ensemble de règles (appelées contraintes)
à une classe. Pour voir cela en action, ajoutez des contraintes de validation
afin que le champ ``task`` ne puisse pas être vide et que le champ ``dueDate`` ne
puisse pas être vide et qu'il doive être un objet \DateTime valide.

.. configuration-block::

    .. code-block:: yaml

        # Acme/TaskBundle/Resources/config/validation.yml
        Acme\TaskBundle\Entity\Task:
            properties:
                task:
                    - NotBlank: ~
                dueDate:
                    - NotBlank: ~
                    - Type: \DateTime

    .. code-block:: php-annotations

        // Acme/TaskBundle/Entity/Task.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Task
        {
            /**
             * @Assert\NotBlank()
             */
            public $task;

            /**
             * @Assert\NotBlank()
             * @Assert\Type("\DateTime")
             */
            protected $dueDate;
        }

    .. code-block:: xml

        <!-- Acme/TaskBundle/Resources/config/validation.xml -->
        <?xml version="1.0" charset="UTF-8"?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">
            <class name="Acme\TaskBundle\Entity\Task">
                <property name="task">
                    <constraint name="NotBlank" />
                </property>
                <property name="dueDate">
                    <constraint name="NotBlank" />
                    <constraint name="Type">\DateTime</constraint>
                </property>
            </class>
        <constraint-mapping>

    .. code-block:: php

        // Acme/TaskBundle/Entity/Task.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Type;

        class Task
        {
            // ...

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('task', new NotBlank());

                $metadata->addPropertyConstraint('dueDate', new NotBlank());
                $metadata->addPropertyConstraint(
                    'dueDate',
                    new Type('\DateTime')
                );
            }
        }

C'est tout ! Si vous soumettez le formulaire à nouveau avec des données non valides,
vous allez voir les erreurs correspondantes affichées avec le formulaire.

.. _book-forms-html5-validation-disable:

.. sidebar:: Validation HTML5

   Grâce à HTML5, de nombreux navigateurs peuvent nativement forcer certaines
   contraintes de validation côté client. La validation la plus commune est
   activée en affichant un attribut ``required`` sur les champs qui sont requis.
   Pour les navigateurs qui supportent HTML5, cela entrainera l'affichage
   d'un message natif du navigateur si l'utilisateur essaye de soumettre le
   formulaire avec ce champ vide.

   Les formulaires générés profitent pleinement de cette nouvelle fonctionnalité
   en ajoutant des attributs HTML qui déclenchent la validation. La validation
   côté client, néanmoins, peut être désactivée en ajoutant l'attribut ``novalidate``
   à la balise ``form`` ou ``formnovalidate`` à la balise ``submit``. Ceci est
   particulièrement utile quand vous voulez tester vos contraintes de validation
   côté serveur, mais que vous en êtes empêché par le formulaire de votre
   navigateur, par exemple, quand vous soumettez des champs vides.

La validation est une fonctionnalité très puissante de Symfony2 et possède son
:doc:`propre chapitre</book/validation>`.

.. index::
   single: Forms; Validation groups

.. _book-forms-validation-groups:

Les Groupes de Validation
~~~~~~~~~~~~~~~~~~~~~~~~~

.. tip::

    Si vous n'utilisez pas :ref:`les groupes de validation <book-validation-validation-groups>`,
    alors vous pouvez sauter cette section.

Si votre objet profite des avantages des :ref:`groupes de validation <book-validation-validation-groups>`,
vous aurez besoin de spécifier quel(s) groupe(s) de validation votre formulaire doit utiliser::

    $form = $this->createFormBuilder($users, array(
        'validation_groups' => array('registration'),
    ))->add(...);

.. versionadded:: 2.7
    La méthode ``configureOptions(OptionsResolver $resolver)`` a été ajoutée dans Symfony 2.7.
    Avant cela, la méthode était appelée ``setDefaultOptions(OptionsResolverInterface $resolver)``.
    Changez aussi ``use Symfony\Component\OptionsResolver\OptionsResolverInterface`` en
    ``use Symfony\Component\OptionsResolver\OptionsResolver``.

Si vous créez :ref:`des classes de formulaire<book-form-creating-form-classes>` (une
bonne pratique), alors vous devrez ajouter ce qui suit à la méthode ``configureOptions()``::

    use Symfony\Component\OptionsResolver\OptionsResolver;

    public function configureOptions(OptionsResolver $resolver)
    {
        $resolver->setDefaults(array(
            'validation_groups' => array('registration'),
        ));
    }

Dans les deux cas, le groupe de validation ``registration`` *uniquement*
sera utilisé pour valider l'objet sous-jacent.

.. index::
   single: Forms; Disabling validation

Désactiver la validation
~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    La possibilité de définir ``validation_groups`` à false a été ajoutée dans Symfony 2.3,
    bien que la définir comme un tableau vide revient au même dans les versions
    précédentes.

Parfois, il peut être utile de supprimer complètement la validation d'un formulaire.
Pour cela, vous pouvez éviter l'appel à la méthode :method:`Symfony\\Component\\Form\\FormInterface::isValid`
dans votre contrôleur. Si cela n'est pas possible, vous pouvez également définir
l'option ``validation_groups`` à ``false`` ou en tant que tableau vide::

    use Symfony\Component\OptionsResolver\OptionsResolver;

    public function configureOptions(OptionsResolver $resolver)
    {
        $resolver->setDefaults(array(
            'validation_groups' => false,
        ));
    }

Notez que lorsque vous faites ceci, le formulaire exécutera toujours
des vérifications de bases, comme par exemple véirifier si un fichier est
trop gros pour être uploadé, ou si des champs qui n'existent pas sont soumis.
Si vous voulez supprimer complètement la validation, supprimez l'appel à la
méthode :method:`Symfony\\Component\\Form\\FormInterface::isValid` dans votre
contrôleur.

.. index::
   single: Forms; Validation groups based on submitted data

Groupes basés sur les données soumises
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous avez besoin de plus de logique pour déterminer les groupes de validation
(c'est-à-dire en vous basant sur les données), vous pouvez définir l'option ``validation_groups``
comme un tableau callback::

    use Symfony\Component\OptionsResolver\OptionsResolver;

    public function configureOptions(OptionsResolver $resolver)
    {
        $resolver->setDefaults(array(
            'validation_groups' => array(
                'Acme\AcmeBundle\Entity\Client',
                'determineValidationGroups',
            ),
        ));
    }

Cela appellera la méthode statique  ``determineValidationGroups()`` de la classe
``Client`` après que le formulaire est soumis, mais avant que la validation ne soit faite.
L'objet Form est passé comme argument à cette méthode (regardez l'exemple suivant).
Vous pouvez aussi définir une logique entière en utilisant une Closure::

    use Symfony\Component\Form\FormInterface;
    use Symfony\Component\OptionsResolver\OptionsResolver;

    public function configureOptions(OptionsResolver $resolver)
    {
        $resolver->setDefaults(array(
            'validation_groups' => function(FormInterface $form) {
                $data = $form->getData();
                if (Entity\Client::TYPE_PERSON == $data->getType()) {
                    return array('person');
                } else {
                    return array('company');
                }
            },
        ));
    }

.. index::
   single: Forms; Validation groups based on clicked button

Groupes basés sur les clics de boutons
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    Le support des boutons dans les formulaires est une nouveauté de Symfony 2.3.

Lorsque votre formulaire contient plusieurs boutons submit, vous pouvez changer
de groupe de validation selon le bouton qui a été utilisé pour soumettre le formulaire.
Par exemple, considérez un formulaire d'assistance qui vous permet d'aller à une
étape suivante, ou de revenir à une étape précédente. Supposez également que lorsque
vous retournez à l'étape précédente, les données du formulaires doivent être
enregistrées mais pas validées.

Premièrement, vous avez besoin d'ajouter les deux boutons au formulaire::

    $form = $this->createFormBuilder($task)
        // ...
        ->add('nextStep', 'submit')
        ->add('previousStep', 'submit')
        ->getForm();

Ensuite, configurez le bouton qui renvoie à l'étape précédente pour qu'il
exécute des groupes de validation spécifiques. Dans cet exemple, vous voulez
qu'il supprime la validation, donc vous devez définir ses options ``validation_groups``
à false::

    $form = $this->createFormBuilder($task)
        // ...
        ->add('previousStep', 'submit', array(
            'validation_groups' => false,
        ))
        ->getForm();

Maintenant, le formulaire ne vérifiera pas les contraintes de validation. Mais
il effectuera toujours des vérifications d'intégrité de base, comme vérifier
si un fichier est trop gros pour être uploadé ou si vous tentez de soumettre
un texte dans un champ censé contenir un nombre.

.. index::
   single: Forms; Built-in field types

.. _book-forms-type-reference:

Types de champ intégrés
-----------------------

Symfony est fourni par défaut avec un grand nombre de types de champ qui couvrent
la plupart des champs de formulaire existants et des types de données que vous
pourrez rencontrer :

.. include:: /reference/forms/types/map.rst.inc

Vous pouvez aussi créer vos propres types de champ personnalisés. Ce sujet
est couvert par l'article du cookbook « :doc:`/cookbook/form/create_custom_field_type` ».

.. index::
   single: Forms; Field type options

Options des types de champ
~~~~~~~~~~~~~~~~~~~~~~~~~~

Chaque type de champ possède un nombre d'options qui peuvent être utilisées
pour le configurer. Par exemple, le champ ``dueDate`` est affiché par défaut
comme 3 champs select. Cependant, le :doc:`champ date</reference/forms/types/date>`
peut être configuré pour être affiché en tant qu'un unique champ texte (où
l'utilisateur doit entrer la date « manuellement » comme une chaîne de caractères)::

    ->add('dueDate', 'date', array('widget' => 'single_text'))

.. image:: /images/book/form-simple2.png
    :align: center

Chaque type de champ a un certain nombre d'options différentes qui peuvent lui être
passées. Beaucoup d'entre elles sont spécifiques à chacun d'entres-eux et vous pouvez trouver
ces détails dans la documentation de chaque type.

.. sidebar:: L'option ``required``

    La plus commune des options est l'option ``required``, qui peut être appliquée à
    tous les champs. Par défaut, cette dernière est définie à ``true``,
    signifiant que les navigateurs supportant HTML5 vont appliquer la validation
    côté client si le champ est laissé vide. Si vous ne souhaitez pas ce
    comportement, vous pouvez soit définir l'option ``required`` de vos champs
    à ``false``, ou soit :ref:`désactiver la validation HTML5<book-forms-html5-validation-disable>`.

    Notez aussi que définir l'option ``required`` à ``true`` **ne** créera
    **aucune** validation côté serveur. En d'autres termes, si un utilisateur
    soumet une valeur vide pour le champ (soit avec un vieux navigateur ou via
    un service web, par exemple), cette valeur sera acceptée comme valide à moins
    que vous n'utilisiez la contrainte de validation de Symfony ``NotBlank`` ou
    ``NotNull``.

    En d'autres termes, l'option ``required`` est « cool », mais une réelle validation
    côté serveur devrait *toujours* être mise en place.

.. sidebar:: L'option ``label``

    Le label d'un champ de formulaire peut être défini grâce à l'option ``label``,
    qui s'applique à n'importe quel champ::
	
        ->add('dueDate', 'date', array(
            'widget' => 'single_text',	
            'label'  => 'Due Date',	
        ))

    Le label d'un champ peut aussi être défini dans le template lorsque vous affichez
    le formulaire, voir ci-dessous.

.. index::
   single: Forms; Field type guessing

.. _book-forms-field-guessing:

Prédiction de Type de Champ
---------------------------

Maintenant que vous avez ajouté les métadonnées de validation à la classe
``Task``, Symfony en sait déjà un peu plus à propos de vos champs. Si vous
l'autorisez, Symfony peut « prédire » le type de vos champs et les mettre
en place pour vous. Dans cet exemple, Symfony peut deviner depuis les règles
de validation que le champ ``task`` est un champ ``texte`` classique et que
``dueDate`` est un champ ``date``::

    public function newAction()
    {
        $task = new Task();

        $form = $this->createFormBuilder($task)
            ->add('task')
            ->add('dueDate', null, array('widget' => 'single_text'))
            ->getForm();
    }

La « prédiction » est activée lorsque vous omettez le second argument de
la méthode ``add()`` (ou si vous lui passez ``null``). Si vous passez un
tableau d'options en tant que troisième argument (comme pour ``dueDate``
ci-dessus), ces options sont appliquées au champ prédit.

.. caution::

    Si votre formulaire utilise un groupe de validation spécifique, le
    prédicteur de type de champ continuera toujours à considérer *toutes*
    les contraintes de validation lorsqu'il essaie de deviner les types
    de champ (incluant les contraintes qui ne font pas partie du ou des groupes
    qui sont utilisés).

.. index::
   single: Forms; Field type guessing

Prédiction des options de type de champ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En plus de pouvoir prédire le « type » d'un champ, Symfony peut aussi essayer
de deviner les valeurs correctes d'un certain nombre d'options de champ.

.. tip::
    Lorsque ces options sont définies, le champ sera affiché avec des attributs HTML
    spécifiques qui permettent la validation du champ côté client. Cependant, cela
    ne génère pas l'équivalent côté serveur (ex ``Assert\Length``). Et puisque
    vous aurez besoin d'ajouter la validation côté serveur manuellement, ces options
    peuvent être déduites de cette information.

* ``required`` : L'option ``required`` peut être devinée grâce aux règles de
  validation (est-ce que le champ est ``NotBlank`` ou ``NotNull``?) ou
  grâce aux métadonnées de Doctrine (est-ce que le champ est ``nullable``?).
  Ceci est très utile car votre validation côté client va automatiquement
  correspondre à vos règles de validation.

* ``max_length`` : Si le champ est de type texte, alors l'option ``max_length``
  peut être devinée grâce aux contraintes de validation (si ``Length`` 
  ou ``Range`` sont utilisées) ou grâce aux métadonnées 
  de Doctrine (via la longueur du champ).


.. note::

  Ces options de champ sont *uniquement* devinées si vous utilisez Symfony pour
  prédire le type des champs (c-a-d en omettant ou en passant ``null`` en tant
  que deuxième argument de la méthode ``add()``).

Si vous voulez changer une des valeurs prédites, vous pouvez la surcharger en
passant l'option au tableau des options de champ ::

    ->add('task', null, array('max_length' => 4))

.. index::
   single: Forms; Rendering in a template

.. _form-rendering-template:

Afficher un Formulaire dans un Template
---------------------------------------

Jusqu'içi, vous avez vu comment un formulaire entier peut être affiché avec
seulement une ligne de code. Bien sûr, vous aurez probablement besoin de
bien plus de flexibilité :

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}
        {{ form_start(form) }}
            {{ form_errors(form) }}

            {{ form_row(form.task) }}
            {{ form_row(form.dueDate) }}

            <input type="submit" />
        {{ form_end(form) }}

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Default/newAction.html.php -->
        <?php echo $view['form']->start($form) ?>
            <?php echo $view['form']->errors($form) ?>

            <?php echo $view['form']->row($form['task']) ?>
            <?php echo $view['form']->row($form['dueDate']) ?>

            <input type="submit" />
        <?php echo $view['form']->end($form) ?>

Jetons un coup d'oeil à chaque partie :

* ``form_start(form)`` - Affiche la balise d'ouverture form.

* ``form_errors(form)`` - Affiche les erreurs globales du formulaire
  (les erreurs spécifiques à chaque champ seront affichées à côté des champs);

* ``form_row(form.dueDate)`` - Affiche le label, les erreurs éventuelles et
  le widget HTML  d'un champ donné (ex ``dueDate``) dans un élément ``div``
  (par défaut);

* ``form_end()`` - Affiche la balise de fermeture du formulaire ainsi que tous les champs
  qui n'ont pas encore été affichés. C'est utile pour afficher les champs cachés et pour
  profiter de la :ref:`protection CSRF<forms-csrf>` automatique.

La plus grande partie du travail est effectuée par la fonction d'aide ``form_row``,
qui rend le label, les erreurs et le widget HTML de chaque champ dans une balise
``div`` par défaut. Dans la section :ref:`form-theming`, vous apprendrez comment
le rendu de ``form_row`` peut être personnalisé à différents niveaux.

.. tip::

    Vous pouvez accéder à la donnée courante de votre formulaire via ``form.vars.value``:

    .. configuration-block::

        .. code-block:: jinja

            {{ form.vars.value.task }}

        .. code-block:: html+php

            <?php echo $view['form']->get('value')->getTask() ?>

.. index::
   single: Forms; Rendering each field by hand

Afficher chaque Champ à la Main
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La fonction ``form_row`` est géniale, car, grâce à elle, vous pouvez
afficher très rapidement chaque champ de votre formulaire (et les balises
utilisées pour ce champ peuvent être personnalisées aussi). Mais comme la
vie n'est pas toujours simple, vous pouvez aussi afficher chaque champ
entièrement à la main. Le résultat décrit ci-dessous revient au même
que lorsque vous avez utilisé la fonction ``form_row`` :

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_start(form) }}
            {{ form_errors(form) }}

            <div>
                {{ form_label(form.task) }}
                {{ form_errors(form.task) }}
                {{ form_widget(form.task) }}
            </div>

            <div>
                {{ form_label(form.dueDate) }}
                {{ form_errors(form.dueDate) }}
                {{ form_widget(form.dueDate) }}
            </div>

        <input type="submit" />

        {{ form_end(form) }}

    .. code-block:: html+php

        <?php echo $view['form']->start($form) ?>

            <?php echo $view['form']->errors($form) ?>

            <div>
                <?php echo $view['form']->label($form['task']) ?>
                <?php echo $view['form']->errors($form['task']) ?>
                <?php echo $view['form']->widget($form['task']) ?>
            </div>

            <div>
                <?php echo $view['form']->label($form['dueDate']) ?>
                <?php echo $view['form']->errors($form['dueDate']) ?>
                <?php echo $view['form']->widget($form['dueDate']) ?>
            </div>

            <input type="submit" />

        <?php echo $view['form']->end($form) ?>

Si le label autogénéré pour un champ n'est pas tout à fait correct, vous
pouvez le spécifier explicitement :

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_label(form.task, 'Task Description') }}

    .. code-block:: html+php

        <?php echo $view['form']->label($form['task'], 'Task Description') ?>

Certains types de champ ont des options d'affichage supplémentaires
qui peuvent être passées au widget. Ces options sont documentées avec chaque
type, mais l'option ``attr`` est commune à tous les type et vous permet de modifier
les attributs d'un élément de formulaire. Ce qui suit ajouterait la classe
``task_field`` au champ texte affiché :

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_widget(form.task, {'attr': {'class': 'task_field'}}) }}

    .. code-block:: html+php

        <?php echo $view['form']->widget($form['task'], array(
            'attr' => array('class' => 'task_field'),
        )) ?>

Si vous avez besoin d'afficher des champs du formulaire « à la main » alors vous
pouvez utiliser individuellement les valeurs des champs comme ``id``, ``name`` et ``label``.
Par exemple, pour obtenir l'``id`` :

.. configuration-block::

    .. code-block:: html+jinja

        {{ form.task.vars.id }}

    .. code-block:: html+php

        <?php echo $form['task']->get('id') ?>

Pour obtenir la valeur utilisée pour l'attribut name du champ de formulaire, 
vous devez utiliser la valeur ``full_name`` :

.. configuration-block::

    .. code-block:: html+jinja

        {{ form.task.vars.full_name }}

    .. code-block:: html+php

        <?php echo $form['task']->get('full_name') ?>

Fonctions de Référence des Templates Twig
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous utilisez Twig, un :doc:`manuel de référence</reference/forms/twig_reference>`
complet des fonctions d'affichage de formulaire est mis à votre disposition.
Lisez-le afin de tout connaître sur les fonctions d'aide disponibles
ainsi que les options qui peuvent être utilisées pour chacune d'elles.

.. index::
   single: Forms; Changing the action and method

.. _book-forms-changing-action-and-method:

Changer les attributs Action et Method d'un formulaire
------------------------------------------------------

Jusqu'ici, la fonction ``form_start()`` a été utilisée pour afficher la
balise d'ouverture du formulaire et nous avons supposé que chaque formulaire
était soumis à la même URL avec une requête de type POST. Dans certains cas, vous
aurez besoin de changer ces paramètres. Vous pouvez le faire de différentes manières.
Si vous construisez votre formulaire dans un contrôleur, vous pouvez utiliser
``setAction()`` et ``setMethod()``::

    $form = $this->createFormBuilder($task)
        ->setAction($this->generateUrl('target_route'))
        ->setMethod('GET')
        ->add('task', 'text')
        ->add('dueDate', 'date')
        ->getForm();

.. note::

    Cet exemple suppose que vous avez créé une route appelée ``target_route``
    qui pointe vers le contrôleur qui va traiter le formulaire.

Dans :ref:`book-form-creating-form-classes`, vous apprendrez comment déporter
le code qui construit votre formulaire dans des classes séparées. Lorsque
vous utilisez une classe de formulaire externe dans votre contrôleur, vous
pouvez passer les attributs ``action`` et ``method`` comme options de votre
formulaire::

    $form = $this->createForm(new TaskType(), $task, array(
        'action' => $this->generateUrl('target_route'),
        'method' => 'GET',
    ));

Enfin, vous pouvez surcharger ces attributs dans un template en les passant aux
fonctions ``form()`` ou ``form_start()`` :

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}
        {{ form(form, {'action': path('target_route'), 'method': 'GET'}) }}

        {{ form_start(form, {'action': path('target_route'), 'method': 'GET'}) }}

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Default/newAction.html.php -->
        <?php echo $view['form']->form($form, array(
            'action' => $view['router']->generate('target_route'),
            'method' => 'GET',
        )) ?>

        <?php echo $view['form']->start($form, array(
            'action' => $view['router']->generate('target_route'),
            'method' => 'GET',
        )) ?>

.. note::

    Si la méthode n'est pas GET ou POST, mais PUT, PATCH ou DELETE, Symfony2
    insèrera un nouveau champ caché avec le nom ``_method`` qui stockera cette
    méthode. Le formulaire sera soumis dans une requête POST classique mais
    le routeur de Symfony2 sera capable de détecter le paramètre ``_method``
    et interprètera la requête comme une requête PUT, PATCH ou DELETE. Lisez
    le chapitre du Cookbook « :doc:`/cookbook/routing/method_parameters` »
    pour obtenir plus d'informations à ce sujet.

.. index::
   single: Forms; Creating form classes

.. _book-form-creating-form-classes:

Créer des Classes de Formulaire
-------------------------------

Comme vous l'avez vu, un formulaire peut être créé et utilisé directement dans
un contrôleur. Cependant, une meilleure pratique est de construire le formulaire
dans une classe PHP séparée et autonome, qui peut ainsi être réutilisée n'importe
où dans votre application. Créez une nouvelle classe qui va héberger la logique
de construction du formulaire « task »::

    // src/Acme/TaskBundle/Form/Type/TaskType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;

    class TaskType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('task');
            $builder->add('dueDate', null, array('widget' => 'single_text'));
        }

        public function getName()
        {
            return 'task';
        }
    }

Cette nouvelle classe contient toutes les directives nécessaires à la création
du formulaire « task » (notez que la méthode ``getName()`` doit retourner un
identifiant unique pour ce « type » de formulaire). Il peut être utilisé pour
construire rapidement un objet formulaire dans le contrôleur::

    // src/Acme/TaskBundle/Controller/DefaultController.php

    // ajoutez cette nouvelle déclaration « use » en haut de la classe
    use Acme\TaskBundle\Form\Type\TaskType;

    public function newAction()
    {
        $task = // ...
        $form = $this->createForm(new TaskType(), $task);

        // ...
    }

Placer la logique du formulaire dans sa propre classe signifie que le formulaire
peut être réutilisé facilement ailleurs dans votre projet. C'est la meilleure
manière de créer des formulaires, mais le choix final vous revient.

.. _book-forms-data-class:

.. sidebar:: Définir la ``data_class``

    Chaque formulaire a besoin de savoir le nom de la classe qui détient les
    données sous-jacentes (par exemple : ``Acme\TaskBundle\Entity\Task``).
    Généralement, cette information est devinée grâce à l'objet passé en
    second argument de la méthode ``createForm`` (c-a-d ``$task``).
    Plus tard, quand vous commencerez à imbriquer des formulaires les uns avec
    les autres, cela ne sera plus suffisant. Donc, bien que facultatif,
    c'est généralement une bonne idée de spécifier explicitement l'option
    ``data_class`` en ajoutant ce qui suit à votre classe formulaire::

        use Symfony\Component\OptionsResolver\OptionsResolver;

        public function configureOptions(OptionsResolver $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'Acme\TaskBundle\Entity\Task',
            ));
        }


.. tip::

    Lorsque vous associez un formulaire à un objet, tous les champs sont mappés.
    Chaque champ du formulaire qui n'existe pas dans l'objet associé entrainera
    la levée d'une exception.
  
    Dans le cas où vous avez besoin de champs supplémentaires dans le formulaire
    (par exemple une checkbox « Acceptez-vous les conditions d'utilisation ») qui
    ne doit pas être mappée à l'objet sous-jacent, vous devez définir l'option
    ``mapped`` à ``false``::

        use Symfony\Component\Form\FormBuilderInterface;

        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('task');
            $builder->add('dueDate', null, array('mapped' => false));
        }

    De plus, s'il y a des champs dans le formulaire qui ne sont pas inclus dans
    les données soumises, ces champs seront définis à ``null``.

    La donnée du champ est accessible dans un contrôleur de cette manière::

        $form->get('dueDate')->getData();

Définir vos formulaire en tant que services
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Définir votre formulaire en tant que service est une bonne pratique, et le
rend très facilement utilisable dans votre application.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/TaskBundle/Resources/config/services.yml
        services:
            acme_demo.form.type.task:
                class: Acme\TaskBundle\Form\Type\TaskType
                tags:
                    - { name: form.type, alias: task }

    .. code-block:: xml

        <!-- src/Acme/TaskBundle/Resources/config/services.xml -->
        <service id="acme_demo.form.type.task"
            class="Acme\TaskBundle\Form\Type\TaskType">
            <tag name="form.type" alias="task" />
        </service>

    .. code-block:: php

        // src/Acme/TaskBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container
            ->register(
                'acme_demo.form.type.task',
                'Acme\TaskBundle\Form\Type\TaskType'
            )
            ->addTag('form.type', array(
                'alias' => 'task',
            ))
        ;

Et c'est tout! Maintenant, vous pouvez utiliser votre type de formulaire directement dans un
contrôleur::

    // src/Acme/TaskBundle/Controller/DefaultController.php
    // ...

    public function newAction()
    {
        $task = ...;
        $form = $this->createForm('task', $task);

        // ...
    }

ou même l'utiliser au sein d'un autre type de formulaire::

    // src/Acme/TaskBundle/Form/Type/ListType.php
    // ...

    class ListType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            // ...

            $builder->add('someTask', 'task');
        }
    }

Lisez :ref:`form-cookbook-form-field-service` pour plus d'informations.



.. index::
   pair: Forms; Doctrine

Formulaires et Doctrine
-----------------------

Le but d'un formulaire est de traduire les données d'un objet (par exemple :
``Task``) en un formulaire HTML et puis de transcrire en retour les données
soumises par l'utilisateur à l'objet original. En tant que tel, le fait de
persister l'objet ``Task`` dans la base de données n'a rien à voir avec
les formulaires. Mais, si vous avez configuré la classe ``Task`` de
telle sorte qu'elle soit persistée via Doctrine (c-a-d que vous avez ajouté des
:ref:`métadonnées de correspondance<book-doctrine-adding-mapping>`),
alors sa persistance peut être effectuée après la soumission d'un formulaire
lorsque ce dernier est valide::

    if ($form->isValid()) {
        $em = $this->getDoctrine()->getManager();
        $em->persist($task);
        $em->flush();

        return $this->redirect($this->generateUrl('task_success'));
    }

Si, pour une quelconque raison, vous n'avez pas accès à votre objet ``$task``
d'origine, vous pouvez le récupérer depuis le formulaire::

    $task = $form->getData();

Pour plus d'informations, voir le :doc:`chapitre sur l'ORM Doctrine</book/doctrine>`.

Le plus important est de comprendre que lorsque le formulaire est lié (« bindé »),
les données soumises sont transférées à l'objet sous-jacent immédiatement. Si
vous souhaitez persister ces données, vous avez simplement besoin de persister
l'objet lui-même (qui contient déjà les données soumises).

.. index::
   single: Forms; Embedded forms

Formulaires imbriqués
---------------------

Souvent, vous allez vouloir construire un formulaire qui inclura des champs
de beaucoup d'objets différents. Par exemple, un formulaire de souscription
pourrait contenir des données appartenant à un objet ``User`` ainsi qu'à
plusieurs objets ``Address``. Heureusement, gérer cela est facile et naturel
avec le composant formulaire.

Imbriquer un objet unique
~~~~~~~~~~~~~~~~~~~~~~~~~

Supposez que chaque ``Task`` appartienne à un simple objet ``Category``.
Commencez, bien sûr, par créer l'objet ``Category``::

    // src/Acme/TaskBundle/Entity/Category.php
    namespace Acme\TaskBundle\Entity;

    use Symfony\Component\Validator\Constraints as Assert;

    class Category
    {
        /**
         * @Assert\NotBlank()
         */
        public $name;
    }

Ensuite, ajoutez une nouvelle propriété ``category`` à la classe ``Task``::

    // ...

    class Task
    {
        // ...

        /**
         * @Assert\Type(type="Acme\TaskBundle\Entity\Category")
         */
        protected $category;

        // ...

        public function getCategory()
        {
            return $this->category;
        }

        public function setCategory(Category $category = null)
        {
            $this->category = $category;
        }
    }

Maintenant que votre application a été mise à jour pour refléter nos nouvelles
conditions requises, créez une classe formulaire afin que l'objet ``Category``
puisse être modifié par l'utilisateur::

    // src/Acme/TaskBundle/Form/Type/CategoryType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolver;

    class CategoryType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('name');
        }

        public function configureOptions(OptionsResolver $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'Acme\TaskBundle\Entity\Category',
            ));
        }

        public function getName()
        {
            return 'category';
        }
    }

Le but final est d'autoriser la ``Category`` d'une ``Task`` à être modifiée
directement dans le formulaire de la tâche. Pour accomplir cela,
ajoutez un champ ``category`` à l'objet ``TaskType`` dont le type est une
instance de la nouvelle classe ``CategoryType`` :

.. code-block:: php

    use Symfony\Component\Form\FormBuilderInterface;

    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        // ...

        $builder->add('category', new CategoryType());
    }

Les champs de ``CategoryType`` peuvent maintenant être affichés à côté de ceux
de la classe ``TaskType``. Pour activer la validation sur CategoryType, ajoutez
l'option ``cascade_validation`` à ``TaskType``::

    public function configureOptions(OptionsResolver $resolver)
    {
        $resolver->setDefaults(array(
            'data_class' => 'Acme\TaskBundle\Entity\Task',
            'cascade_validation' => true,
        ));
    }

Affichez les champs de ``Category`` de la même manière
que les champs de la classe ``Task`` :

.. configuration-block::

    .. code-block:: html+jinja

        {# ... #}

        <h3>Category</h3>
        <div class="category">
            {{ form_row(form.category.name) }}
        </div>

        {# ... #}

    .. code-block:: html+php

        <!-- ... -->

        <h3>Category</h3>
        <div class="category">
            <?php echo $view['form']->row($form['category']['name']) ?>
        </div>

        <!-- ... -->

Lorsque l'utilisateur soumet le formulaire, les données soumises pour les champs
de ``Category`` sont utilisées pour construire une instance de ``Category`` qui
est ensuite affectée au champ ``category`` de l'instance de `Task``.

L'instance ``Category`` est accessible naturellement via ``$task->getCategory()``
et peut être persistée dans la base de données ou utilisée de toute autre manière
que vous souhaitez.

Imbriquer une Collection de Formulaires
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez aussi imbriquer une collection de formulaires dans un formulaire
(imaginez un formulaire ``Catégorie`` avec plusieurs sous-formulaires ``Produit``).
Cela peut être accompli en utilisant le type de champ ``collection``.


Pour plus d'informations, lisez le chapitre du cookbook ":doc:`/cookbook/form/form_collections`"
et le chapitre sur le type de champ :doc:`collection</reference/forms/types/collection>`.

.. index::
   single: Forms; Theming
   single: Forms; Customizing fields

.. _form-theming:

Habillage de Formulaire (« Theming »)
-------------------------------------

Chaque partie de l'affichage d'un formulaire peut être personnalisée. Vous êtes
libre de changer la manière dont chaque « partie » du formulaire est affichée,
de changer les balises utilisées pour afficher les erreurs, ou même de personnaliser
la manière dont la balise ``textarea`` doit être affichée. Tout est permis, et
différentes personnalisations peuvent être utilisées à différents endroits.

Symfony utilise des templates pour afficher chaque partie d'un formulaire, comme
les balises ``tags``, les balises ``input``, les messages d'erreur et tout le reste.

Dans Twig, chaque « fragment » de formulaire est représenté par un bloc Twig. Pour
personnaliser n'importe quelle partie d'un formulaire, vous avez juste besoin de
réécrire le bloc approprié.

En PHP, chaque « fragment » de formulaire est affiché via un fichier de template
individuel. Pour personnaliser n'importe quelle partie d'un formulaire, vous avez
juste besoin de réécrire le template existant en en créant un nouveau.

Pour comprendre comment tout cela fonctionne, personnalisons le fragment ``form_row``
et ajoutons l'attribut « class » à l'élément ``div`` qui entoure chaque ligne. Pour
faire cela, créez un nouveau fichier de template qui va stocker la nouvelle
balise :

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Form/fields.html.twig #}
        {% block form_row %}
        {% spaceless %}
            <div class="form_row">
                {{ form_label(form) }}
                {{ form_errors(form) }}
                {{ form_widget(form) }}
            </div>
        {% endspaceless %}
        {% endblock form_row %}

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Form/form_row.html.php -->
        <div class="form_row">
            <?php echo $view['form']->label($form, $label) ?>
            <?php echo $view['form']->errors($form) ?>
            <?php echo $view['form']->widget($form, $parameters) ?>
        </div>

Le fragment de formulaire ``form_row`` est utilisé pour afficher la plupart
des champs via la fonction ``form_row``. Pour dire au composant formulaire
d'utiliser votre nouveau fragment ``form_row`` défini ci-dessus, ajoutez
ce qui suit en haut du template qui affiche le formulaire :

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}
        {% form_theme form 'AcmeTaskBundle:Form:fields.html.twig' %}

        {% form_theme form 'AcmeTaskBundle:Form:fields.html.twig' 'AcmeTaskBundle:Form:fields2.html.twig' %}

        {{ form(form) }}

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Default/new.html.php -->
        <?php $view['form']->setTheme($form, array('AcmeTaskBundle:Form')) ?>

        <?php $view['form']->setTheme($form, array('AcmeTaskBundle:Form', 'AcmeTaskBundle:Form')) ?>

        <?php echo $view['form']->form($form) ?>

La balise ``form_theme`` (dans Twig) « importe » les fragments définis dans le
template donné et les utilise lorsqu'il affiche le formulaire. En d'autres termes,
quand la fonction ``form_row`` est appelée plus tard dans ce template, elle va
utiliser le bloc ``form_row`` de votre thème personnalisé (à la place du bloc
par défaut ``form_row`` qui est fourni avec Symfony).

Votre thème personnalisé n'a pas besoin de surcharger tous les blocs. Lorsqu'il
affiche un bloc qui n'est pas surchargé par votre thème personnalisé, le moteur de
thème se rabattra sur le thème global (défini au niveau du bundle).

Si plusieurs thèmes personnalisés sont fournis, ils seront pris selon l'ordre
dans lequel ils sont listés avant que le thème global ne soit pris en compte.

Pour personnaliser n'importe quelle portion d'un formulaire, vous devez juste
réécrire le fragment approprié. Connaître exactement quel bloc ou fichier
réécrire est le sujet de la prochaine section.

.. code-block:: html+jinja

    {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}

    {% form_theme form with 'AcmeTaskBundle:Form:fields.html.twig' %}

    {% form_theme form with ['AcmeTaskBundle:Form:fields.html.twig', 'AcmeTaskBundle:Form:fields2.html.twig'] %}

Pour plus de précisions, lisez :doc:`/cookbook/form/form_customization`.

.. index::
   single: Forms; Template fragment naming

.. _form-template-blocks:

Nommage de Fragment de Formulaire
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans Symfony, chaque partie d'un formulaire qui est affiché - éléments de formulaire
HTML, erreurs, labels, etc - est définie dans un thème de base, qui est une
collection de blocs dans Twig et une collection de fichiers de template dans PHP.

Dans Twig, chaque bloc nécessaire est défini dans un unique fichier de template
(`form_div_layout.html.twig`_) qui se trouve dans le `Twig Bridge`_. Dans ce fichier,
vous pouvez voir chaque bloc requis pour afficher un formulaire et chaque type
de champ par défaut.

En PHP, les fragments sont des fichiers de template individuels. Par défaut, ils
sont situés dans le répertoire `Resources/views/Form` du bundle du framework
(`voir sur GitHub`_).

Chaque nom de fragment suit le même schéma de base et est divisé en deux parties,
séparées par un unique underscore (``_``). Voici quelques exemples :

* ``form_row`` - utilisé par ``form_row`` pour afficher la plupart des champs ;
* ``textarea_widget`` - utilisé par ``form_widget`` pour afficher un champ de
  type ``textarea`` ;
* ``form_errors`` - utilisé par ``form_errors`` pour afficher les erreurs d'un champ.

Chaque fragment suit le même schéma de base : ``type_part``. La partie ``type``
correspond au *type* du champ qui doit être affiché (par exemple : ``textarea``,
``checkbox``, ``date``, etc) alors que la partie ``part`` correspond à *ce qui*
va être affiché (par exemple : ``label``, ``widget``, ``errors``, etc). Par défaut,
il y a 4 *parties* possibles d'un formulaire qui peuvent être affichées :

+-------------+---------------------------------+---------------------------------------------------------------+
| ``label``   | (par exemple : ``form_label``)  | affiche le label du champ                                     |
+-------------+---------------------------------+---------------------------------------------------------------+
| ``widget``  | (par exemple : ``form_widget``) | affiche la représentation HTML du champ                       |
+-------------+---------------------------------+---------------------------------------------------------------+
| ``errors``  | (par exemple : ``form_errors``) | affiche les erreurs du champ                                  |
+-------------+---------------------------------+---------------------------------------------------------------+
| ``row``     | (par exemple : ``form_row``)    | affiche la ligne entière du champ (label, widget, et erreurs) |
+-------------+---------------------------------+---------------------------------------------------------------+

.. note::

    Il y a en fait 2 autres *parties*, ``rows`` et ``rest``, mais
    vous ne devriez que rarement, voire jamais, avoir besoin de les réécrire.

En connaissant le type du champ (par exemple : ``textarea``) et quelle partie de
ce dernier vous souhaitez personnaliser (par exemple : ``widget``), vous pouvez
construire le nom du fragment qui a besoin d'être réécrit (par exemple : ``textarea_widget``).

.. index::
   single: Forms; Template fragment inheritance

Héritage de Fragment de Template
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans certains cas, le fragment que vous voulez personnaliser sera absent.
Par exemple, il n'y a pas de fragment ``textarea_errors`` dans les thèmes
fournis par défaut par Symfony. Alors comment les erreurs des textareas
sont-elles affichées ?

La réponse est : via le fragment ``form_errors``. Quand Symfony affiche les erreurs
d'un champ de type textarea, il recherche en premier un fragment ``textarea_errors``
avant de se replier sur le fragment de secours ``form_errors``. Chaque type de
champ a un type *parent* (le type parent de ``textarea`` est ``field``), et
Symfony l'utilise si le fragment de base n'existe pas.

Donc, afin de réécrire les erreurs pour les champs ``textarea`` *seulement*, copiez
le fragment ``form_errors``, renommez-le en ``textarea_errors`` et personnalisez-le.
Pour réécrire le rendu d'erreur par défaut pour *tous* les champs, copiez et personnalisez
le fragment ``form_errors`` directement.

.. tip::

    Le type « parent » de chaque type de champ est disponible dans la
    :doc:`référence de type de formulaire</reference/forms/types>` pour
    chaque type de champ.

.. index::
   single: Forms; Global Theming

Habillage global de Formulaire
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans l'exemple ci-dessus, vous avez utilisé la fonction d'aide ``form_theme``
(dans Twig) pour « importer » les fragments personnalisés de formulaire à
l'intérieur de ce formulaire uniquement. Vous pouvez aussi dire à Symfony
d'importer les personnalisations de formulaire à travers votre projet entier.

Twig
....

Pour inclure automatiquement les blocs personnalisés du template ``fields.html.twig``
créés plus tôt dans *tous* les templates, modifiez votre fichier de configuration
d'application :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            form:
                resources:
                    - 'AcmeTaskBundle:Form:fields.html.twig'
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <twig:config ...>
                <twig:form>
                    <resource>AcmeTaskBundle:Form:fields.html.twig</resource>
                </twig:form>
                <!-- ... -->
        </twig:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array(
            'form' => array(
                'resources' => array(
                    'AcmeTaskBundle:Form:fields.html.twig',
                ),
            ),
            // ...
        ));

Tous les blocs se trouvant dans le template ``fields.html.twig`` sont
maintenant utilisés pour l'affichage de tous vos formulaires.

.. sidebar::  Personnaliser l'affichage de formulaire dans un fichier unique avec Twig

    Dans Twig, vous pouvez aussi personnaliser un bloc de formulaire directement
    à l'intérieur du template nécessitant une personnalisation :

    .. code-block:: html+jinja

        {% extends '::base.html.twig' %}

        {# importe « _self » en tant qu'habillage du formulaire #}
        {% form_theme form _self %}

        {# effectue la personnalisation du fragment de formulaire #}
        {% block form_row %}
            {# personnalisez l'affichage de la ligne du champ #}
        {% endblock form_row %}

        {% block content %}
            {# ... #}

            {{ form_row(form.task) }}
        {% endblock %}

    La balise ``{% form_theme form _self %}`` permet aux blocs de formulaire d'être
    personnalisés directement à l'intérieur du template qui va utiliser ces
    personnalisations. Utilisez cette méthode pour faire des personnalisations
    de formulaire qui ne seront nécessaires que dans un unique template.

    .. caution::

        Cette fonctionnalité (``{% form_theme form _self %}``) ne marchera *que*
        si votre template en étend un autre. Si ce n'est pas le cas, vous devrez
        faire pointer ``form_theme`` vers un template séparé.

PHP
...

Pour inclure automatiquement les templates personnalisés du répertoire
``Acme/TaskBundle/Resources/views/Form`` créés plus tôt dans *tous* les templates,
modifiez votre fichier de configuration d'application :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            templating:
                form:
                    resources:
                        - 'AcmeTaskBundle:Form'
        # ...


    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config ...>
            <framework:templating>
                <framework:form>
                    <resource>AcmeTaskBundle:Form</resource>
                </framework:form>
            </framework:templating>
            <!-- ... -->
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'templating' => array(
                'form' => array(
                    'resources' => array(
                        'AcmeTaskBundle:Form',
                    ),
                ),
            )
            // ...
        ));

Tous les fragments à l'intérieur du répertoire ``Acme/TaskBundle/Resources/views/Form``
sont maintenant utilisés pour l'affichage de tous vos formulaires.

.. index::
   single: Forms; CSRF protection

.. _forms-csrf:

Protection CSRF
---------------

CSRF - ou `Cross-site request forgery`_ - est une méthode grâce à laquelle
un utilisateur malveillant tente de faire soumettre inconsciemment des
données à un utilisateur légitime qui n'avait pas l'intention de les soumettre.
Heureusement, les attaques CSRF peuvent être contrées en utilisant un jeton CSRF
à l'intérieur de vos formulaires.

La bonne nouvelle est que, par défaut, Symfony prend en charge et valide automatiquement
les jetons CSRF pour vous. Cela signifie que vous pouvez profiter de la protection
CSRF sans n'avoir rien à faire. En fait, chaque formulaire dans ce chapitre
a profité de la protection CSRF !

La protection CSRF fonctionne en ajoutant un champ caché dans votre formulaire -
appelé ``_token`` par défaut - qui contient une valeur que seuls vous et votre
utilisateur connaissez. Cela garantit que l'utilisateur - et lui seulement -
soumet les informations données. Symfony valide automatiquement la
présence et l'exactitude de ce jeton.

Le champ ``_token`` est un champ caché et sera affiché automatiquement si vous
incluez la fonction ``form_end()`` dans votre template, qui garantit que
tous les champs non affichés sont délivrés en sortie.

Le jeton CSRF peut être personnalisé pour chacun des formulaires. Par exemple::

    use Symfony\Component\OptionsResolver\OptionsResolver;

    class TaskType extends AbstractType
    {
        // ...

        public function configureOptions(OptionsResolver $resolver)
        {
            $resolver->setDefaults(array(
                'data_class'      => 'Acme\TaskBundle\Entity\Task',
                'csrf_protection' => true,
                'csrf_field_name' => '_token',
                // une clé unique pour aider à la génération du jeton secret
                'intention'       => 'task_item',
            ));
        }

        // ...
    }

Pour désactiver la protection CSRF, définissez l'option ``csrf_protection``
à false. Les personnalisations peuvent aussi être effectuées globalement dans
votre projet. Pour plus d'informations, voir la section de
:ref:`référence de configuration de formulaire<reference-framework-form>`.

.. note::

    L'option ``intention`` est optionnelle, mais améliore grandement la sécurité
    du jeton généré en le rendant différent pour chaque formulaire.

.. index::
   single: Forms; With no class

Utiliser un formulaire sans classe
----------------------------------

Dans la plupart des cas, un formulaire est associé à un objet, et les champs du 
formulaire affichent et stockent leurs données dans les propriétés d'un objet. C'est
exactement ce que vous avez vu jusqu'ici dans ce chapitre avec la classe `Task`.

Mais parfois, vous voudrez juste utiliser un formulaire sans une classe, et obtenir
un tableau des données soumises. C'est en fait très facile::

    // Assurez vous d'importer le namespace Request namespace en haut de la classe
    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function contactAction(Request $request)
    {
        $defaultData = array('message' => 'Type your message here');
        $form = $this->createFormBuilder($defaultData)
            ->add('name', 'text')
            ->add('email', 'email')
            ->add('message', 'textarea')
            ->getForm();

        $form->handleRequest($request);

        if ($form->isValid()) {
            // Les données sont un tableau avec les clés "name", "email", et "message"
            $data = $form->getData();
        }

        // ... affiche le formulaire
    }

Par défaut, en fait, un formulaire part du principe que vous voulez travailler avec
un tableau de données plutôt qu'avec un objet. Il y a exactement deux façons de changer
ce comportement et d'associer le formulaire avec un objet à la place :

#. Passez un objet lors de la création du formulaire (comme premier argument de ``createFormBuilder``
   ou deuxième argument de ``createForm``);

#. Définissez l'option ``data_class`` de votre formulaire.

Si vous ne faites *pas* l'un ou l'autre, alors le formulaire retournera les données
dans un tableau. Dans cet exemple, puisque ``$defaultData`` n'est pas un objet (et
que l'option ``data_class`` n'est pas définie), ``$form->getData()`` retournera
finalement un tableau.

.. tip::

    Vous pouvez également accéder directement aux valeurs POST (dans ce cas "name")
    par le biais de l'objet Request, comme ceci::

        $this->get('request')->request->get('name');

    Notez cependant que, dans la plupart des cas, utiliser la méthode getData() est le
    meilleur choix, puisqu'elle retourne la donnée (souvent un objet) après qu'elle
    soit transformée par le framework.


Ajouter la Validation
~~~~~~~~~~~~~~~~~~~~~

La seule pièce manquante est la validation. D'habitude, quand vous appelez ``$form->isValid()``,
l'objet est validé en parcourant les contraintes que vous appliquez à sa classe.
Si le formulaire est associé à un objet (c-a-d si vous utilisez l'option ``data_class``
ou si vous passez un object à votre formulaire), c'est l'approche que vous voudrez utiliser dans
la plupart des cas. lisez :doc:`/book/validation` pour plus de détails.

.. _form-option-constraints:

Mais si le formulaire n'est pas associé à un objet et que vous voulez plutôt utiliser un simple
tableau pour soumettre vos données, comment ajouter des contraintes aux données de votre formulaire ?

La réponse est de définir les contraintes vous-même, et de les attacher à chacun des champs.
L'approche globale est un peu plus expliquée dans le :ref:`chapitre validation<book-validation-raw-values>`,
mais voici un petit exemple:

.. code-block:: php

    use Symfony\Component\Validator\Constraints\Length;
    use Symfony\Component\Validator\Constraints\NotBlank;

    $builder
       ->add('firstName', 'text', array(
           'constraints' => new Length(array('min' => 3)),
       ))
       ->add('lastName', 'text', array(
           'constraints' => array(
               new NotBlank(),
               new Length(array('min' => 3)),
           ),
       ))
    ;

.. tip::

    Si vous utilisez les groupes de validation, vous devrez soit référencer
    un groupe ``Default`` lorsque vous créez le formulaire, ou soit définir le
    bon groupe à la contrainte que vous ajoutez.
    
.. code-block:: php

    new NotBlank(array('groups' => array('create', 'update'))

Réflexions finales
------------------

Vous connaissez maintenant tout sur les bases nécessaires à la construction
de formulaires complexes et fonctionnels pour votre application. Quand vous
construisez des formulaires, gardez à l'esprit que le but premier d'un
formulaire est de transcrire les données d'un objet (``Task``) en un formulaire
HTML afin que l'utilisateur puisse modifier ces données. Le second objectif d'un
formulaire est de prendre les données soumises par l'utilisateur et de les
réappliquer à l'objet.

Il y a beaucoup plus à apprendre à propos du puissant monde des
formulaires, par exemple comment gérer les :doc:`uploads de fichier
avec Doctrine</cookbook/doctrine/file_uploads>` ou comment créer un
formulaire où un nombre dynamique de sous-formulaires peut être ajouté
(par exemple : une liste de choses à faire où vous pouvez continuer d'ajouter
des champs via Javascript avant de soumettre le formulaire).
Voyez le cookbook pour ces sujets. Aussi, assurez-vous de vous appuyer sur la
:doc:`documentation de référence des types de champ</reference/forms/types>`,
qui inclut des exemples de comment utiliser chaque type de champ et leurs options.

En savoir plus grâce au Cookbook
--------------------------------

* :doc:`/cookbook/doctrine/file_uploads`
* :doc:`Référence du Champ Fichier </reference/forms/types/file>`
* :doc:`Créer des Types de Champ Personnalisés </cookbook/form/create_custom_field_type>`
* :doc:`/cookbook/form/form_customization`
* :doc:`/cookbook/form/dynamic_form_modification`
* :doc:`/cookbook/form/data_transformers`

.. _`Composant Formulaire Symfony2`: https://github.com/symfony/Form
.. _`DateTime`: http://php.net/manual/en/class.datetime.php
.. _`Twig Bridge`: https://github.com/symfony/symfony/tree/2.2/src/Symfony/Bridge/Twig
.. _`form_div_layout.html.twig`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_layout.html.twig
.. _`Cross-site request forgery`: http://fr.wikipedia.org/wiki/Cross-site_request_forgery
.. _`voir sur GitHub`: https://github.com/symfony/symfony/tree/master/src/Symfony/Bundle/FrameworkBundle/Resources/views/Form
