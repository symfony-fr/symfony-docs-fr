.. index::
   single: Formulaires

Formulaires
===========

Intéragir avec les formulaires HTML est l'une des plus communes - et stimulantes -
tâches pour un développeur web. Symfony2 intègre un composant « Form » qui rend
la chose facile. Dans ce chapitre, vous allez construire un formulaire complexe
depuis la base, tout en apprenant au fur et à mesure les caractéristiques les
plus importantes de la bibliothèque des formulaires.

.. note::

   Le composant formulaire de Symfony est une bibliothèque autonome qui peut être
   utilisée en dehors des projets Symfony2. Pour plus d'informations, voir le
   `Composant Formulaire Symfony2`_ sur Github.

.. index::
   single: Formulaires; Créer un formulaire simple

Créer un Formulaire Simple
--------------------------

Supposez que vous construisiez une application « todo list » (en français : « liste
de choses à faire ») simple qui doit afficher des « tâches ». Parce que vos
utilisateurs devront éditer et créer des tâches, vous allez avoir besoin de
construire des formulaires. Mais avant de commencer, concentrez-vous
d'abord sur la classe générique ``Task`` qui représente et stocke les données
pour une tâche :

.. code-block:: php

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
   acceptant toutes les options par défaut) :

   .. code-block:: bash

        php app/console generate:bundle --namespace=Acme/TaskBundle

Cette classe est un « bon vieil objet PHP » parce que, jusqu'ici, elle n'a rien
à voir avec Symfony ou quelconque autre bibliothèque. C'est tout simplement
un objet PHP normal qui répond directement à un besoin dans *votre*
application (c-a-d le besoin de représenter une tâche dans votre application).
Bien sûr, à la fin de ce chapitre, vous serez capable de soumettre des données
à une instance de ``Task`` (via un formulaire HTML), de valider ses données,
et de les persister dans la base de données.

.. index::
   single: Formulaires; Créer un formulaire dans un contrôleur

Construire le Formulaire
~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant que vous avez créé une classe ``Task``, la prochaine étape est
de créer et de retourner le formulaire HTML. Dans Symfony2, cela se fait
en construisant un objet formulaire qui est ensuite rendu dans un template.
Pour l'instant, tout ceci peut être effectué depuis un contrôleur ::

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
   votre formulaire dans une classe autonome, ce qui est recommandé car comme
   cela, vos formulaires deviennent réutilisables.

Créer un formulaire requiert relativement peu de code car les objets formulaires
de Symfony2 sont construits avec un « constructeur de formulaire » (« form builder »).
Le principe du constructeur de formulaire est de vous permettre d'écrire des
« conteneurs » de formulaire simples, et de le laisser prendre en charge toute
la plus grosse partie du travail qu'est la construction du formulaire.

Dans cet exemple, vous avez ajouté deux champs à votre formulaire - ``task`` et
``dueDate`` - correspondants aux propriétés ``task`` et ``dueDate`` de votre
classe ``Task``. Vous avez aussi assigné à chaque champ un « type » (par exemple :
``text``, ``date``), qui, entre autres, détermine quelle(s) balise(s) HTML
est rendue pour ce champ.

Symfony2 est livré avec beaucoup de types pré-définis qui seront présentés
rapidement plus tard (voir :ref:`book-forms-type-reference`).

.. index::
  single: Formulaires; Rendu de template basique

Rendu du Formulaire
~~~~~~~~~~~~~~~~~~~

Maintenant que le formulaire a été créé, la prochaine étape est de le rendre.
Ceci est fait en passant un objet spécial « vue de formulaire » à votre template
(notez le ``$form->createView()`` dans le contrôleur ci-dessus) et en utilisant
un ensemble de fonctions d'aide (helpers) pour les formulaires :

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}

        <form action="{{ path('task_new') }}" method="post" {{ form_enctype(form) }}>
            {{ form_widget(form) }}

            <input type="submit" />
        </form>

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Default/new.html.php -->

        <form action="<?php echo $view['router']->generate('task_new') ?>" method="post" <?php echo $view['form']->enctype($form) ?> >
            <?php echo $view['form']->widget($form) ?>

            <input type="submit" />
        </form>

.. image:: /images/book/form-simple.png
    :align: center

.. note::

    Cet exemple assume que vous avez créé une route nommée ``task_new``
    qui pointe sur le contrôleur ``AcmeTaskBundle:Default:new`` qui a
    été créé plus tôt.

C'est tout ! En affichant ``form_widget(form)``, chaque champ du formulaire
est rendu, avec un label et un message d'erreur (si erreur il y a). Aussi
facile que cela soit, ce n'est pas (encore) très flexible. Habituellement,
vous voudrez rendre chaque champ du formulaire individuellement afin de
pouvoir contrôler ce à quoi le formulaire ressemble. Vous apprendrez comment
faire cela dans la section «:ref:`form-rendering-template`».

Avant de continuer, notez comment le champ ``task`` rendu possède la
valeur de la propriété ``task`` de l'objet ``$task`` (c-a-d « Write a blog
post »). C'est le premier travail d'un formulaire : de prendre les données
d'un objet et de les traduire dans un format adapté pour être rendues dans
un formulaire HTML.

.. tip::

   Le système de formulaire est assez intelligent pour accéder la valeur de la
   propriété protégée ``task`` via les méthodes ``getTask()`` et ``setTask()``
   de la classe ``Task``. A moins qu'une propriété soit publique, elle *doit*
   avoir une méthode « getter » et une « setter » afin que le composant formulaire
   puisse récupérer et assigner des données à cette propriété. Pour une propriété
   booléenne, vous pouvez utiliser une méthode « isser » (par exemple :
   ``isPublished()``) à la place d'un getter (par exemple : ``getPublished()``).

.. index::
  single: Formulaires; Gérer la soumission des formulaires

Gérer la Soumission des Formulaires
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le second travail d'un formulaire est de transmettre les données soumises par
l'utilisateur aux propriétés d'un objet. Pour réaliser cela, les données
soumises par l'utilisateur doivent être liées au formulaire. Ajoutez la
fonctionnalité suivante à votre contrôleur ::

    // ...

    public function newAction(Request $request)
    {
        // initialisez simplement un objet $task (supprimez les données fictives)
        $task = new Task();

        $form = $this->createFormBuilder($task)
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

        if ($request->getMethod() == 'POST') {
            $form->bindRequest($request);

            if ($form->isValid()) {
                // effectuez quelques actions, comme sauvegarder la tâche dans
                // la base de données
                return $this->redirect($this->generateUrl('task_success'));
            }
        }

        // ...
    }

Maintenant, lorsque vous soumettez le formulaire, le contrôleur lie les données
soumises avec le formulaire, qui transmet ces données en retour aux propriétés
``task`` et ``dueDate`` de l'objet ``$task``. Tout ceci a lieu grâce à la
méthode ``bindRequest()``.

.. note::

    Aussitôt que ``bindRequest()`` est appelée, les données soumises sont
    transférées immédiatement à l'objet sous-jacent. Ceci intervient
    indépendamment du fait que les données sous-jacentes soient valides ou non.

Ce contrôleur suit un pattern commun dans la manière de gérer les formulaires,
et a trois scénarios possibles :

#. Lors du chargement initial de la page dans votre navigateur, la méthode
   de la requête est ``GET`` et le formulaire est simplement créé et rendu ;

#. Lorsque l'utilisateur soumet le formulaire (la méthode est ``POST``)
   avec des données non-valides (la validation est expliquée dans la prochaine
   section), le formulaire est lié puis rendu, affichant cette fois toutes les
   erreurs de validation ;

#. Lorsque l'utilisateur soumet le formulaire avec des données valides, ce
   dernier est lié et vous avez l'opportunité d'effectuer quelques actions
   en utilisant l'objet ``$task`` (par exemple : le persister dans la base
   de données) avant de rediriger l'utilisateur vers une autre page (par
   exemple : une page « merci » ou de « succès »).

.. note::

   Rediriger un utilisateur après une soumission de formulaire réussie empêche
   l'utilisateur de pouvoir rafraichir la page et de re-soumettre les données.

.. index::
   single: Formulaires; Validation

Validation de Formulaire
------------------------

Dans la section précédente, vous avez appris comment un formulaire peut être
soumis avec des données valides ou non-valides. Dans Symfony2, la validation
est appliquée à l'objet sous-jacent (par exemple : ``Task``). En d'autres termes,
la question n'est pas de savoir si le « formulaire » est valide, mais plutôt de
savoir si l'objet ``$task`` est valide ou non après que le formulaire lui ait
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
        <class name="Acme\TaskBundle\Entity\Task">	
            <property name="task">	
                <constraint name="NotBlank" />	
            </property>	
            <property name="dueDate">
                <constraint name="NotBlank" />
                <constraint name="Type">
                    <value>\DateTime</value>
                </constraint>
            </property>
        </class>

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
                $metadata->addPropertyConstraint('dueDate', new Type('\DateTime'));
            }
        }

C'est tout ! Si vous re-soumettez le formulaire avec des données non-valides,
vous allez voir les erreurs correspondantes affichées avec le formulaire.

.. _book-forms-html5-validation-disable:

.. sidebar:: Validation HTML5

   Grâce à HTML5, beaucoup de navigateurs peuvent nativement forcer certaines
   contraintes de validation côté client. La validation la plus commune est
   activée en rendant un attribut ``required`` sur les champs qui sont requis.
   Pour les navigateurs qui supportent HTML5, cela résultera en l'affichage
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
   single: Formulaires; Les Groupes de Validation

.. _book-forms-validation-groups:

Les Groupes de Validation
~~~~~~~~~~~~~~~~~~~~~~~~~

.. tip::

    Si vous n'utilisez pas :ref:`les groupes de validation <book-validation-validation-groups>`,
    alors vous pouvez sauter cette section.

Si votre objet profite des avantages des :ref:`groupes de validation <book-validation-validation-groups>`,
vous aurez besoin de spécifier quel(s) groupe(s) de validation votre formulaire doit utiliser ::

    $form = $this->createFormBuilder($users, array(
        'validation_groups' => array('registration'),
    ))->add(...)
    ;

Si vous créez :ref:`des classes de formulaire<book-form-creating-form-classes>` (une
bonne pratique), alors vous devrez ajouter ce qui suit à la méthode ``getDefaultOptions()`` ::

    public function getDefaultOptions(array $options)
    {
        return array(
            'validation_groups' => array('registration')
        );
    }

Dans les deux cas, *uniquement* le groupe de validation ``registration``
sera utilisé pour valider l'objet sous-jacent.

.. index::
   single: Formulaires; Types de Champ Intégrés

.. _book-forms-type-reference:

Types de Champ Intégrés
-----------------------

Symfony vient par défaut avec un grand nombre de types de champ qui couvre
la plupart des champs de formulaire existants et des types de données que vous
pourrez rencontrer :

.. include:: /reference/forms/types/map.rst.inc

Vous pouvez aussi créer vos propres types de champ personnalisés. Ce sujet
est couvert par l'article du cookbook « :doc:`/cookbook/form/create_custom_field_type` ».

.. index::
   single: Formulaires; Options des types de champ

Options des types de champ
~~~~~~~~~~~~~~~~~~~~~~~~~~

Chaque type de champ possède un nombre d'options qui peuvent être utilisées
pour le configurer. Par exemple, le champ ``dueDate`` est rendu par défaut
comme 3 champs select. Cependant, le :doc:`champ date</reference/forms/types/date>`
peut être configuré pour être rendu en tant qu'un unique champ texte (où
l'utilisateur entrerait la date « manuellement » comme une chaîne de caractères) ::

    ->add('dueDate', 'date', array('widget' => 'single_text'))

.. image:: /images/book/form-simple2.png
    :align: center

Chaque type de champ a un nombre d'options différentes qui peuvent lui être
passées. Beaucoup d'entre elles sont spécifiques à chacun et vous pouvez trouver
ces détails dans la documentation de chaque type.

.. sidebar:: L'option ``required``

    La plus commune des options est l'option ``required``, qui peut être appliquée à
    tous les champs. Par défaut, cette dernière est définie comme ``true``,
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

.. index::
   single: Formulaires; Prédiction de type de champ

.. _book-forms-field-guessing:

Prédiction de Type de Champ
---------------------------

Maintenant que vous avez ajouté les métadonnées de validation à la classe
``Task``, Symfony en sait déjà un peu plus à propos de vos champs. Si vous
l'autorisez, Symfony peut « prédire » le type de vos champs et les mettre
en place pour vous. Dans cet exemple, Symfony peut deviner depuis les règles
de validation que le champ ``task`` est un champ ``texte`` normal et que
``dueDate`` est un champ ``date`` ::

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
un peu plus haut), ces options sont appliquées au champ prédit.

.. caution::

    Si votre formulaire utilise un groupe de validation spéficique, le
    prédicteur de type de champ continuera toujours à considérer *toutes*
    les contraintes de validation lorsqu'il essaie de deviner ces derniers
    (incluant les contraintes qui ne font pas partie du ou des groupe(s)
    étant utilisés).

.. index::
   single: Formulaires; Prédiction de type de champ

Prédiction des Options de Type de Champ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En plus de pouvoir prédire le « type » d'un champ, Symfony peut aussi essayer
de deviner les valeurs correctes d'un certain nombre d'options de champ :


.. tip::
    Lorsque ces options sont définies, le champ sera rendu avec des attributs HTML
    spécifiques qui permettent la validation du champ côté client. Cependant, cela
    ne génère pas l'équivalent côté serveur (ex ``Assert\MaxLength``). Et puisque
    vous aurez besoin d'ajouter la validation côté serveur manuellement, ces options
    peuvent être déduites de cette information.

* ``required`` : L'option ``required`` peut être devinée grâce aux règles de
  validation (c-a-d est-ce que le champ est ``NotBlank`` ou ``NotNull``) ou
  grâce aux métadonnées de Doctrine (c-a-d est-ce que le champ est ``nullable``).
  Ceci est très utile car votre validation côté client va automatiquement
  correspondre à vos règles de validation.

* ``min_length`` : Si le champ est de type texte, alors l'option ``min_length``
  peut être devinée grâce aux contraintes de validation (si ``MinLength`` ou ``Min``
  est utilisée) ou grâce aux métadonnées de Doctrine (via la longueur du champ).

* ``max_length`` : Similaire à ``min_length``, la longueur maximale peut aussi
  être prédite.

.. note::

  Ces options de champ sont *uniquement* devinées si vous utilisez Symfony pour
  prédire le type des champs (c-a-d en omettant ou en passant ``null`` en tant
  que deuxième argument de la méthode ``add()``).

Si vous voulez changer une des valeurs prédites, vous pouvez l'outrepasser en
passant l'option au tableau des options de champ ::

    ->add('task', null, array('min_length' => 4))

.. index::
   single: Formulaires; Rendu dans un Template

.. _form-rendering-template:

Rendre un Formulaire dans un Template
-------------------------------------

Jusqu'içi, vous avez vu comment un formulaire entier peut être rendu avec
seulement une ligne de code. Bien sûr, vous aurez probablement besoin de
bien plus de flexibilité :

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}

        <form action="{{ path('task_new') }}" method="post" {{ form_enctype(form) }}>
            {{ form_errors(form) }}

            {{ form_row(form.task) }}
            {{ form_row(form.dueDate) }}

            {{ form_rest(form) }}

            <input type="submit" />
        </form>

    .. code-block:: html+php

        <!-- // src/Acme/TaskBundle/Resources/views/Default/newAction.html.php -->

        <form action="<?php echo $view['router']->generate('task_new') ?>" method="post" <?php echo $view['form']->enctype($form) ?>>
            <?php echo $view['form']->errors($form) ?>

            <?php echo $view['form']->row($form['task']) ?>
            <?php echo $view['form']->row($form['dueDate']) ?>

            <?php echo $view['form']->rest($form) ?>

            <input type="submit" />
        </form>

Jetons un coup d'oeil à chaque partie :

* ``form_enctype(form)`` - Si au moins un champ est un upload de fichier, ceci
  se charge de rendre l'attribut obligatoire ``enctype="multipart/form-data"`` ;

* ``form_errors(form)`` - Rend toutes les erreurs globales du formulaire (les
  erreurs spécifiques aux champs sont affichées à côté de chaque champ) ;

* ``form_row(form.dueDate)`` - Rend le label, toutes les erreurs, ainsi que le
  widget HTML pour le champ donné (par exemple : ``dueDate``) qui, par défaut, est
  dans une balise ``div`` ;

* ``form_rest(form)`` - Rend tous les champs qui n'ont pas encore été rendus.
  C'est généralement une bonne idée de placer un appel à cette fonction d'aide
  à la fin de chaque formulaire (au cas où vous auriez oublié d'afficher un
  champ ou si vous ne souhaitez pas vous embêter à rendre manuellement les
  champs cachés). Cette fonction d'aide est aussi utile pour profiter
  de la :ref:`Protection CSRF<forms-csrf>` automatique.

La plus grande partie du travail est effectuée par la fonction d'aide ``form_row``,
qui rend le label, les erreurs et le widget HTML de chaque champ dans une balise
``div`` par défaut. Dans la section :ref:`form-theming`, vous apprendrez comment
le rendu de ``form_row`` peut être personnalisé à différents niveaux.

.. index::
   single: Formulaires; Rendre chaque champ à la main

Rendre chaque Champ à la Main
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La fonction d'aide ``form_row`` est géniale car, grâce à elle, vous pouvez
rendre très rapidement chaque champ de votre formulaire (et les balises
utilisées pour ce champ peuvent être personnalisées aussi). Mais comme la
vie n'est pas toujours simple, vous pouvez aussi rendre chaque champ
entièrement à la main. Le produit fini décrit dans la suite revient au même
que lorsque vous avez utilisé la fonction d'aide ``form_row`` :

.. configuration-block::

    .. code-block:: html+jinja

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

        {{ form_rest(form) }}

    .. code-block:: html+php

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

        <?php echo $view['form']->rest($form) ?>

Si le label auto-généré pour un champ n'est pas tout à fait correct, vous
pouvez le spécifier explicitement :

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_label(form.task, 'Task Description') }}

    .. code-block:: html+php

        <?php echo $view['form']->label($form['task'], 'Task Description') ?>

Finalement, quelques types de champ ont des options de rendu supplémentaires
qui peuvent être passées au widget. Ces options sont documentées avec chaque
type, mais une qui est commune est l'option ``attr``, qui vous permet de modifier
les attributs d'un élément de formulaire. Ce qui suit ajouterait la classe
``task_field`` au champ texte rendu :

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_widget(form.task, { 'attr': {'class': 'task_field'} }) }}

    .. code-block:: html+php

        <?php echo $view['form']->widget($form['task'], array(
            'attr' => array('class' => 'task_field'),
        )) ?>

Fonctions de Référence des Templates Twig
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous utilisez Twig, un :doc:`manuel de référence</reference/forms/twig_reference>`
complet des fonctions de rendu de formulaire est mis à votre disposition.
Lisez-le afin de tout connaître sur des fonctions d'aide disponibles
ainsi que les options qui peuvent être utilisées avec ces dernières.

.. index::
   single: Formulaires; Créer des classes de formulaire

.. _book-form-creating-form-classes:

Créer des Classes de Formulaire
-------------------------------

Comme vous l'avez vu, un formulaire peut être créé et utilisé directement dans
un contrôleur. Cependant, une meilleure pratique est de construire le formulaire
dans une classe PHP séparée et autonome, qui peut ainsi être réutilisée n'importe
où dans votre application. Créez une nouvelle classe qui va héberger la logique
de construction du formulaire « task » :

.. code-block:: php

    // src/Acme/TaskBundle/Form/Type/TaskType.php

    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;

    class TaskType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
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
construire rapidement un objet formulaire dans le contrôleur :

.. code-block:: php

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

.. sidebar:: Définir la ``data_class``

    Chaque formulaire a besoin de savoir le nom de la classe qui détient les
    données sous-jacentes (par exemple : ``Acme\TaskBundle\Entity\Task``).
    Généralement, cette information est devinée grâce à l'objet passé en
    second argument de la méthode ``createForm`` (c-a-d ``$task``).
    Plus tard, quand vous commencerez à embarquer des formulaires les uns avec
    les autres, cela ne sera plus suffisant. Donc, bien que facultatif,
    c'est généralement une bonne idée de spécifier explicitement l'option
    ``data_class`` en ajoutant ce qui suit à votre classe formulaire ::

        public function getDefaultOptions(array $options)
        {
            return array(
                'data_class' => 'Acme\TaskBundle\Entity\Task',
            );
        }

.. index::
   pair: Formulaires; Doctrine

Formulaires et Doctrine
-----------------------

Le but d'un formulaire est de traduire les données d'un objet (par exemple :
``Task``) en un formulaire HTML et puis de transcrire en retour les données
soumises par l'utilisateur à l'objet original. En tant que tel, le sujet de la
persistance de l'objet ``Task`` dans la base de données n'a rien à voir avec
le sujet des formulaires. Mais, si vous avez configuré la classe ``Task`` de
telle sorte qu'elle soit persistée via Doctrine (c-a-d qye vous avez ajouté des
:ref:`métadonnées de correspondance<book-doctrine-adding-mapping>` pour cela),
alors sa persistance peut être effectuée après la soumission d'un formulaire
lorsque ce dernier est valide ::

    if ($form->isValid()) {
        $em = $this->getDoctrine()->getEntityManager();
        $em->persist($task);
        $em->flush();

        return $this->redirect($this->generateUrl('task_success'));
    }

Si, pour une quelconque raison, vous n'avez pas accès à votre objet ``$task``
d'origine, vous pouvez le récupérer depuis le formulaire ::

    $task = $form->getData();

Pour plus d'informations, voir le :doc:`chapitre Doctrine ORM</book/doctrine>`.

La chose principale à comprendre est que lorsque le formulaire est lié (« bindé »),
les données soumises sont transférées à l'objet sous-jacent immédiatement. Si
vous souhaitez persister ces données, vous avez simplement besoin de persister
l'objet lui-même (qui contient déjà les données soumises).

.. index::
   single: Formulaires; Formulaires embarqués

Formulaires imbriqués
---------------------

Souvent, vous allez vouloir construire un formulaire qui inclura des champs
de beaucoup d'objets différents. Par exemple, un formulaire de souscription
pourrait contenir des données appartenant à un objet ``User`` ainsi qu'à
plusieurs objets ``Address``. Heureusement, gérer cela est facile et naturel
avec le composant formulaire.

Imbriquer un Objet Unique
~~~~~~~~~~~~~~~~~~~~~~~~~

Supposez que chaque ``Task`` appartienne à un simple objet ``Category``.
Commencez, bien sûr, par créer l'objet ``Category`` ::

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

Ensuite, ajoutez une nouvelle propriété ``category`` à la classe ``Task`` ::

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
puisse être modifié par l'utilisateur ::

    // src/Acme/TaskBundle/Form/Type/CategoryType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;

    class CategoryType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('name');
        }

        public function getDefaultOptions(array $options)
        {
            return array(
                'data_class' => 'Acme\TaskBundle\Entity\Category',
            );
        }

        public function getName()
        {
            return 'category';
        }
    }

Le but final est d'autoriser la ``Category`` d'une ``Task`` à être modifiée
directement dans le formulaire de la tâche lui-même. Pour accomplir cela,
ajoutez un champ ``category`` à l'objet ``TaskType`` dont le type est une
instance de la nouvelle classe ``CategoryType`` :

.. code-block:: php

    public function buildForm(FormBuilder $builder, array $options)
    {
        // ...

        $builder->add('category', new CategoryType());
    }

Les champs de ``CategoryType`` peuvent maintenant être rendus à côté de ceux
de la classe ``TaskType``. Rendez les champs de ``Category`` de la même manière
que les champs de la classe ``Task`` :

.. configuration-block::

    .. code-block:: html+jinja

        {# ... #}

        <h3>Category</h3>
        <div class="category">
            {{ form_row(form.category.name) }}
        </div>

        {{ form_rest(form) }}
        {# ... #}

    .. code-block:: html+php

        <!-- ... -->

        <h3>Category</h3>
        <div class="category">
            <?php echo $view['form']->row($form['category']['name']) ?>
        </div>

        <?php echo $view['form']->rest($form) ?>
        <!-- ... -->

Lorsque l'utilisateur soumet le formulaire, les données soumises pour les champs
de ``Category`` sont utilisées pour construire une instance de ``Category``, qui
est ensuite affectée au champ ``category`` de l'instance de `Task``.

L'instance ``Category`` est accessible naturellement via ``$task->getCategory()``
et peut être persistée dans la base de données ou utilisée de quelconque manière
que vous souhaitez.

Imbriquer une Collection de Formulaires
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez aussi imbriquer une collection de formulaires dans un formulaire.
Cela est possible grâce au type de champ ``collection``. Pour plus d'informations,
voir la :doc:`référence du type de champ collection</reference/forms/types/collection>`.

.. index::
   single: Formulaires; Habillage
   single: Formulaires; Personnaliser les champs

.. _form-theming:

Habillage de Formulaire (« Theming »)
-------------------------------------

Chaque partie de l'affichage d'un formulaire peut être personnalisée. Vous êtes
libre de changer comment chaque ligne du formulaire est rendue, de changer les
balises utilisées pour afficher les erreurs, ou même de personnaliser comment
la balise ``textarea`` devrait être rendue. Rien n'est limité, et des différentes
personnalisations peuvent être utilisées à différents endroits.

Symfony utilise des templates pour rendre chaque partie d'un formulaire, comme
les balises ``tags``, les balises ``input``, les messages d'erreur et tout le reste.

Dans Twig, chaque « fragment » de formulaire est représenté par un bloc Twig. Pour
personnaliser n'importe quelle partie d'un formulaire, vous avez juste besoin de
réécrire le bloc approprié.

En PHP, chaque « fragment » de formulaire est rendu via un fichier de template
individuel. Pour personnaliser n'importe quelle partie d'un formulaire, vous avez
juste besoin de réécrire le template existant en en créant un nouveau.

Pour comprendre comment tout cela fonctionne, personnalisons le fragment ``form_row``
et ajoutons l'attribut « class » à l'élément ``div`` qui entoure chaque ligne. Pour
faire cela, créez un nouveau fichier de template qui va stocker la nouvelle
balise :

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Form/fields.html.twig #}

        {% block field_row %}
        {% spaceless %}
            <div class="form_row">
                {{ form_label(form) }}
                {{ form_errors(form) }}
                {{ form_widget(form) }}
            </div>
        {% endspaceless %}
        {% endblock field_row %}

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Form/field_row.html.php -->

        <div class="form_row">
            <?php echo $view['form']->label($form, $label) ?>
            <?php echo $view['form']->errors($form) ?>
            <?php echo $view['form']->widget($form, $parameters) ?>
        </div>

Le fragment de formulaire ``field_row`` est utilisé pour rendre la plupart
des champs via la fonction ``form_row``. Pour dire au composant formulaire
d'utiliser votre nouveau fragment ``field_row`` defini ci-dessus, ajoutez
ce qui suit en haut du template qui rend le formulaire :

.. configuration-block:: php

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}

        {% form_theme form 'AcmeTaskBundle:Form:fields.html.twig' %}

        <form ...>

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Default/new.html.php -->

        <?php $view['form']->setTheme($form, array('AcmeTaskBundle:Form')) ?>

        <form ...>

La balise ``form_theme`` (dans Twig) « importe » les fragments définis dans le
template donné et les utilise lorsqu'il rend le formulaire. En d'autres termes,
quand la fonction ``form_row`` est appelée plus tard dans ce template, elle va
utiliser le bloc ``field_row`` de votre thème personnalisé (à la place du bloc
par défaut ``field_row`` qui est délivré avec Symfony).

Pour personnaliser n'importe quelle portion d'un formulaire, vous devez juste
réécrire le fragment approprié. Connaître exactement quel bloc ou fichier
réécrire est le sujet de la prochaine section.

Pour une discussion plus développée, voir :doc:`/cookbook/form/form_customization`.

.. index::
   single: Formulaires; Nommage de fragment de template

.. _form-template-blocks:

Nommage de Fragment de Formulaire
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans Symfony, chaque partie d'un formulaire qui est rendue - éléments de formulaire
HTML, erreurs, labels, etc - est définie dans un thème de base, qui est une
collection de blocs dans Twig et une collection de fichiers de template dans PHP.

Dans Twig, chaque bloc nécessaire est défini dans un unique fichier de template
(`form_div_layout.html.twig`_) qui réside dans le `Twig Bridge`_. Dans ce fichier,
vous pouvez voir chaque bloc requis pour rendre un formulaire et chaque type
de champ par défaut.

En PHP, les fragments sont des fichiers de template individuels. Par défaut, ils
sont situés dans le répertoire `Resources/views/Form` du bundle du framework
(`voir sur GitHub`_).

Chaque nom de fragment suit le même pattern de base et est divisé en deux parties,
séparées par un unique underscore (``_``). Quelques exemples sont :

* ``field_row`` - utilisé par ``form_row`` pour rendre la plupart des champs ;
* ``textarea_widget`` - utilisé par ``form_widget`` pour rendre un champ de
  type ``textarea`` ;
* ``field_errors`` - utilisé par ``form_errors`` pour rendre les erreurs d'un champ.

Chaque fragment suit le même pattern de base : ``type_part``. La partie ``type``
correspond au *type* du champ qui doit être rendu (par exemple : ``textarea``,
``checkbox``, ``date``, etc) alors que la partie ``part`` correspond à *ce qui*
va être rendu (par exemple : ``label``, ``widget``, ``errors``, etc). Par défaut,
il y a 4 *parts* possibles d'un formulaire qui peuvent être rendues :

+-------------+-----------------------------------+------------------------------------------------------------+
| ``label``   | (par exemple : ``field_label``)   | rend le label du champ                                     |
+-------------+-----------------------------------+------------------------------------------------------------+
| ``widget``  | (par exemple : ``field_widget``)  | rend la représentation HTML du champ                       |
+-------------+-----------------------------------+------------------------------------------------------------+
| ``errors``  | (par exemple : ``field_errors``)  | rend les erreurs du champ                                  |
+-------------+-----------------------------------+------------------------------------------------------------+
| ``row``     | (par exemple : ``field_row``)     | rend la ligne entière du champ (label, widget, et erreurs) |
+-------------+-----------------------------------+------------------------------------------------------------+

.. note::

    Il y a en fait 3 autres *parts* - ``rows``, ``rest``, et ``enctype`` - mais
    vous ne devriez que rarement, voire jamais, avoir besoin de les réécrire.

En connaissant le type du champ (par exemple : ``textarea``) et quelle partie de
ce dernier vous souhaitez personnaliser (par exemple : ``widget``), vous pouvez
construire le nom du fragment qui a besoin d'être réécrit (par exemple : ``textarea_widget``).

.. index::
   single: Formulaires; Inhéritance de Fragment de Template

Héritage de Fragment de Template
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans certains cas, le fragment que vous voulez personnaliser sera absent.
Par exemple, il n'y a pas de fragment ``textarea_errors`` dans les thèmes
fournis par défaut par Symfony.

La réponse est : via le fragment ``field_errors``. Quand Symfony rend les erreurs
d'un champ de type textarea, il recherche en premier un fragment ``textarea_errors``
avant de se replier sur le fragment de secours ``field_errors``. Chaque type de
champ a un type *parent* (le type parent de ``textarea`` est ``field``), et
Symfony l'utilise si le fragment de base n'existe pas.

Donc, afin de réécrire les erreurs pour les champs ``textarea`` *seulement*, copiez
le fragment ``field_errors``, renommez-le en ``textarea_errors`` et personnalisez-le.
Pour réécrire le rendu d'erreur par défaut pour *tous* les champs, copiez et personnalisez
le fragment ``field_errors`` directement.

.. tip::

    Le type « parent » de chaque type de champ est disponible dans la
    :doc:`référence de type de formulaire</reference/forms/types>` pour
    chaque type de champ.

.. index::
   single: Formulaires; Habillage Global

Habillage Global de Formulaire
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
            'form' => array('resources' => array(
                'AcmeTaskBundle:Form:fields.html.twig',
             ))
            // ...
        ));

Tous les blocs se trouvant dans le template ``fields.html.twig`` sont
maintenant utilisés globalement pour définir le rendu de formulaire en sortie.

.. sidebar::  Personnaliser le Rendu de Formulaire en Sortie dans un Fichier Unique avec Twig

    Dans Twig, vous pouvez aussi personnaliser un bloc de formulaire directement
    à l'intérieur du template nécessitant une personnalisation :

    .. code-block:: html+jinja

        {% extends '::base.html.twig' %}

        {# importe « _self » en tant qu'habillage du formulaire #}
        {% form_theme form _self %}

        {# effectue la personnalisation du fragment de formulaire #}
        {% block field_row %}
            {# personnalisez le rendu en sortie de la ligne du champ #}
        {% endblock field_row %}

        {% block content %}
            {# ... #}

            {{ form_row(form.task) }}
        {% endblock %}

    La balise ``{% form_theme form _self %}`` permet aux blocs de formulaire d'être
    personnalisés directement à l'intérieur du template qui va utiliser ces
    personnalisations. Utilisez cette méthode pour faire des personnalisations
    de formulaire qui ne seront nécessaires que dans un unique template.

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
            'templating' => array('form' =>
                array('resources' => array(
                    'AcmeTaskBundle:Form',
             )))
            // ...
        ));

Tous les fragments à l'intérieur du répertoire ``Acme/TaskBundle/Resources/views/Form``
sont maintenant utilisés globalement pour définir le rendu en sortie des formulaires.

.. index::
   single: Formulaires; Protection CSRF

.. _forms-csrf:

Protection CSRF
---------------

CSRF - ou `Cross-site request forgery`_ - est une méthode grâce à laquelle
un utilisateur malveillant tente de faire soumettre inconsciemment des
données à un utilisateur légitime qui n'avait pas l'intention de les soumettre.
Heureusement, les attaques CSRF peuvent être contrées en utilisant un jeton CSRF
à l'intérieur de vos formulaires.

La bonne nouvelle est que, par défaut, Symfony embarque et valide automatiquement
les jetons CSRF pour vous. Cela signifie que vous pouvez profiter de la protection
CSRF sans n'avoir rien à faire. En fait, chaque formulaire dans ce chapitre
a profité de la protection CSRF !

La protection CSRF fonctionne en ajoutant un champ caché dans votre formulaire -
appelé ``_token`` par défaut - qui contient une valeur que seul vous et votre
utilisateur connaissez. Cela garantit que l'utilisateur - et non pas une autre
entité - soumet les informations données. Symfony valide automatiquement la
présence et l'exactitude de ce jeton.

Le champ ``_token`` est un champ caché et sera rendu automatiquement si vous
incluez la fonction ``form_rest()`` dans votre template, qui garantit que
tous les champs non-rendus sont délivrés en sortie.

Le jeton CSRF peut être personnalisé pour chacun des formulaires. Par exemple ::

    class TaskType extends AbstractType
    {
        // ...
    
        public function getDefaultOptions(array $options)
        {
            return array(
                'data_class'      => 'Acme\TaskBundle\Entity\Task',
                'csrf_protection' => true,
                'csrf_field_name' => '_token',
                // une clé unique pour aider à la génération du jeton secret
                'intention'       => 'task_item',
            );
        }
        
        // ...
    }

Pour désactiver la protection CSRF, définissez l'option ``csrf_protection``
à faux. Les personnalisations peuvent aussi être effectuées globalement dans
votre projet. Pour plus d'informations, voir la section de
:ref:`référence de configuration de formulaire</reference-frameworkbundle-forms>`.

.. note::

    L'option ``intention`` est optionnelle mais améliore grandement la sécurité
    du jeton généré en le rendant différent pour chaque formulaire.

Réflexions finales
------------------

Vous connaissez maintenant tout sur les bases nécessaires à la construction
de formulaires complexes et fonctionnels pour votre application. Quand vous
construisez des formulaires, gardez à l'esprit que le but premier d'un
formulaire est de transcrire les données d'un objet (``Task``) en un formulaire
HTML afin que l'utilisateur puisse modifier ces données. Le second objectif d'un
formulaire est de prendre les données soumises par l'utilisateur et de les
ré-appliquer à l'objet.

Il y a beaucoup plus à apprendre à propos de la puissance du monde des
formulaires, comme par exemple comment gérer les :doc:`uploads de fichier
avec Doctrine</cookbook/doctrine/file_uploads>` ou comment créer un
formulaire où un nombre dynamique de sous-formulaires peut être ajouté
(par exemple : une liste de choses à faire où vous pouvez continuer d'ajouter
des champs via Javascript avant de soumettre le formulaire).
Voyez le cookbook pour ces sujets. Aussi, assurez-vous de vous appuyer sur la
:doc:`documentation de référence des types de champ</reference/forms/types>`,
qui inclut des exemples de comment utiliser chaque type de champ et ses options.

En savoir plus grâce au Cookbook
--------------------------------

* :doc:`/cookbook/doctrine/file_uploads`
* :doc:`Référence du Champ Fichier </reference/forms/types/file>`
* :doc:`Créer des Types de Champ Personnalisés </cookbook/form/create_custom_field_type>`
* :doc:`/cookbook/form/form_customization`

.. _`Composant Formulaire Symfony2`: https://github.com/symfony/Form
.. _`DateTime`: http://php.net/manual/en/class.datetime.php
.. _`Twig Bridge`: https://github.com/symfony/symfony/tree/master/src/Symfony/Bridge/Twig
.. _`form_div_layout.html.twig`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_layout.html.twig
.. _`Cross-site request forgery`: http://en.wikipedia.org/wiki/Cross-site_request_forgery
.. _`voir sur GitHub`: https://github.com/symfony/symfony/tree/master/src/Symfony/Bundle/FrameworkBundle/Resources/views/Form
