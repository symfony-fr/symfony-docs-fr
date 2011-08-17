.. index::
   single: Formulaires

Formulaires
===========

Intéragir avec les formulaires HTML est l'une des plus communes - et stimulantes -
tâches pour un développeur web. Symfony2 intègre un composant «Form» qui rend
la chose facile. Dans ce chapitre, vous allez construire un formulaire complexe
depuis la base, tout en apprenant au fur et à mesure les caractéristiques les
plus importantes de la bibliothèque des formulaires.

.. note::

   Le composant formulaire de Symfony est une bibliothèque autonome qui peut être
   utilisée en dehors des projets Symfony2. Pour plus d'informations, voir le
   `Symfony2 Form Component`_ sur Github.

.. index::
   single: Formulaires; Créer un formulaire simple

Créer un Formulaire Simple
--------------------------

Supposez que vous construisiez une application «todo list» (en français : «liste
de choses à faire») simple qui doit afficher des «tâches». Parce que vos
utilisateurs devront éditer et créer des tâches, vous allez avoir besoin de
construire des formulaires. Mais avant que vous commenciez, concentrez-vous
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

Cette classe est un «bon vieux objet PHP» parce que, jusqu'ici, elle n'a rien
à voir avec Symfony ou quelconque autre bibliothèque. C'est tout simplement
un objet PHP normal qui solutionne directement un problème dans *votre*
application (i.e. le besoin de représenter une tâche dans votre application).
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
   «:ref:`book-form-creating-form-classes`», vous apprendrez à construire
   votre formulaire dans une classe autonome, ce qui est recommandé car comme
   cela, vos formulaires deviennent réutilisables.

Créer un formulaire requiert relativement peu de code car les objets formulaires
de Symfony2 sont construits avec un «constructeur de formulaire» («form builder»).
Le principe du constructeur de formulaire est de vous permettre d'écrire des
«conteneurs» de formulaires simples, et de le laisser prendre en charge toute
la plus grosse partie du travail qu'est la construction du formulaire.

Dans cet exemple, vous avez ajouté deux champs à votre formulaire - ``task`` et
``dueDate`` - correspondants aux propriétés ``task`` et ``dueDate`` de votre
classe ``Task``. Vous avez aussi assigné à chacune un «type» (par exemple :
``text``, ``date``), qui, parmi d'autres choses, détermine quelle(s) balise(s) HTML
est rendue pour ce champ.

Symfony2 est livré avec beaucoup de types pré-définis qui seront présentés
rapidement plus tard (voir :ref:`book-forms-type-reference`).

.. index::
  single: Formulaires; Rendu de template basique

Rendu du Formulaire
~~~~~~~~~~~~~~~~~~~

Maintenant que le formulaire a été créé, la prochaine étape est de le rendre.
Ceci est fait en passant un objet spécial «vue de formulaire» à votre template
(notez le ``$form->createView()`` dans le contrôleur ci-dessus) et en utilisant
un ensemble de fonctions d'aide pour les formulaires :

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
valeur de la propriété ``task`` de l'objet ``$task`` (i.e. «Write a blog
post»). C'est le premier travail d'un formulaire : de prendre les données
d'un objet et de les traduire dans un format adapté pour être rendues dans
un formulaire HTML.

.. tip::

   Le système de formulaire est assez intelligent pour accéder la valeur de la
   propriété protégée ``task`` via les méthodes ``getTask()`` et ``setTask()``
   de la classe ``Task``. A moins qu'une propriété soit publique, elle *doit*
   avoir une méthode «getter» et une «setter» afin que le composant formulaire
   puisse récupérer et assigner des données à cette propriété. Pour une propriété
   booléenne, vous pouvez utiliser une méthode «isser» (par exemple :
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
    indépendamment du fait que les donnés sous-jacentes soient valides ou non.

Ce contrôleur suit un pattern commun dans la manière de gérer les formulaires,
et a trois scénarios possibles :

#. Lors du chargement initial de la page dans votre navigateur, la méthode
   de la requête est ``GET`` et le formulaire est simplement créé et rendu ;

#. Lorsque l'utilisateur soumet le formulaire (i.e. la méthode est ``POST``)
   avec des données non-valides (la validation est expliquée dans la prochaine
   section), le formulaire est lié puis rendu, affichant cette fois toutes les
   erreurs de validation ;

#. Lorsque l'utilisateur soumet le formulaire avec des données valides, ce
   dernier est lié et vous avez l'opportunité d'effectuer quelques actions
   en utilisant l'objet ``$task`` (par exemple : le persister dans la base
   de données) avant de rediriger l'utilisateur vers une autre page (par
   exemple : une page «merci» ou de «succès»).

.. note::

   Rediriger un utilisateur après une soumission de formulaire réussie empêche
   l'utilisateur de pouvoir presser «Rafraîchir» et de re-soumettre les données.

.. index::
   single: Formulaires; Validation

Validation de Formulaire
------------------------

Dans la section précédente, vous avez appris comment un formulaire peut être
soumis avec des données valides ou non-valides. Dans Symfony2, la validation
est appliquée à l'objet sous-jacent (par exemple : ``Task``). En d'autres mots,
la question n'est pas de savoir si le «formulaire» est valide, mais plutôt de
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
   single: Forms; Built-in Field Types

.. _book-forms-type-reference:

Built-in Field Types
--------------------

Symfony comes standard with a large group of field types that cover all of
the common form fields and data types you'll encounter:

.. include:: /reference/forms/types/map.rst.inc

You can also create your own custom field types. This topic is covered in
the ":doc:`/cookbook/form/create_custom_field_type`" article of the cookbook.

.. index::
   single: Forms; Field type options

Field Type Options
~~~~~~~~~~~~~~~~~~

Each field type has a number of options that can be used to configure it.
For example, the ``dueDate`` field is currently being rendered as 3 select
boxes. However, the :doc:`date field</reference/forms/types/date>` can be
configured to be rendered as a single text box (where the user would enter
the date as a string in the box)::

    ->add('dueDate', 'date', array('widget' => 'single_text'))

.. image:: /images/book/form-simple2.png
    :align: center

Each field type has a number of different options that can be passed to it.
Many of these are specific to the field type and details can be found in
the documentation for each type.

.. sidebar:: The ``required`` option

    The most common option is the ``required`` option, which can be applied to
    any field. By default, the ``required`` option is set to ``true``, meaning
    that HTML5-ready browsers will apply client-side validation if the field
    is left blank. If you don't want this behavior, either set the ``required``
    option on your field to ``false`` or :ref:`disable HTML5 validation<book-forms-html5-validation-disable>`.
    
    Also note that setting the ``required`` option to ``true`` will **not**
    result in server-side validation to be applied. In other words, if a
    user submits a blank value for the field (either with an old browser
    or web service, for example), it will be accepted as a valid value unless
    you use Symfony's ``NotBlank`` or ``NotNull`` validation constraint.
    
    In other words, the ``required`` option is "nice", but true server-side
    validation should *always* be used.

.. index::
   single: Forms; Field type guessing

.. _book-forms-field-guessing:

Field Type Guessing
-------------------

Now that you've added validation metadata to the ``Task`` class, Symfony
already knows a bit about your fields. If you allow it, Symfony can "guess"
the type of your field and set it up for you. In this example, Symfony can
guess from the validation rules that both the ``task`` field is a normal
``text`` field and the ``dueDate`` field is a ``date`` field::

    public function newAction()
    {
        $task = new Task();

        $form = $this->createFormBuilder($task)
            ->add('task')
            ->add('dueDate', null, array('widget' => 'single_text'))
            ->getForm();
    }

The "guessing" is activated when you omit the second argument to the ``add()``
method (or if you pass ``null`` to it). If you pass an options array as the
third argument (done for ``dueDate`` above), these options are applied to
the guessed field.

.. caution::

    If your form uses a specific validation group, the field type guesser
    will still consider *all* validation constraints when guessing your
    field types (including constraints that are not part of the validation
    group(s) being used).

.. index::
   single: Forms; Field type guessing

Field Type Options Guessing
~~~~~~~~~~~~~~~~~~~~~~~~~~~

In addition to guessing the "type" for a field, Symfony can also try to guess
the correct values of a number of field options:

* ``required``: The ``required`` option can be guessed based off of the validation
  rules (i.e. is the field ``NotBlank`` or ``NotNull``) or the Doctrine metadata
  (i.e. is the field ``nullable``). This is very useful, as your client-side
  validation will automatically match your validation rules.

* ``min_length``: If the field is some sort of text field, then the ``min_length``
  option can be guessed from the validation constrains (if ``MinLength``
  or ``Min`` is used) or from the Doctrine metadata (via the field's length).

* ``max_length``: Similar to ``min_length``, the maximum length can also 
  be guessed.

.. note::

  These field options are *only* guessed if you're using Symfony to guess
  the field type (i.e. omit or pass ``null`` as the second argument to ``add()``).

If you'd like to change one of the guessed values, you can override it by
passing the option in the options field array::

    ->add('task', null, array('min_length' => 4))

.. index::
   single: Forms; Rendering in a Template

.. _form-rendering-template:

Rendering a Form in a Template
------------------------------

So far, you've seen how an entire form can be rendered with just one line
of code. Of course, you'll usually need much more flexibility when rendering:

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

Let's take a look at each part:

* ``form_enctype(form)`` - If at least one field is a file upload field, this
  renders the obligatory ``enctype="multipart/form-data"``;

* ``form_errors(form)`` - Renders any errors global to the whole form
  (field-specific errors are displayed next to each field);

* ``form_row(form.dueDate)`` - Renders the label, any errors, and the HTML
  form widget for the given field (e.g. ``dueDate``) inside, by default, a
  ``div`` element;

* ``form_rest(form)`` - Renders any fields that have not yet been rendered.
  It's usually a good idea to place a call to this helper at the bottom of
  each form (in case you forgot to output a field or don't want to bother
  manually rendering hidden fields). This helper is also useful for taking
  advantage of the automatic :ref:`CSRF Protection<forms-csrf>`.

The majority of the work is done by the ``form_row`` helper, which renders
the label, errors and HTML form widget of each field inside a ``div`` tag
by default. In the :ref:`form-theming` section, you'll learn how the ``form_row``
output can be customized on many different levels.

.. index::
   single: Forms; Rendering each field by hand

Rendering each Field by Hand
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``form_row`` helper is great because you can very quickly render each
field of your form (and the markup used for the "row" can be customized as
well). But since life isn't always so simple, you can also render each field
entirely by hand. The end-product of the following is the same as when you
used the ``form_row`` helper:

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

If the auto-generated label for a field isn't quite right, you can explicitly
specify it:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_label(form.task, 'Task Description') }}

    .. code-block:: html+php

        <?php echo $view['form']->label($form['task'], 'Task Description') ?>

Finally, some field types have additional rendering options that can be passed
to the widget. These options are documented with each type, but one common
options is ``attr``, which allows you to modify attributes on the form element.
The following would add the ``task_field`` class to the rendered input text
field:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_widget(form.task, { 'attr': {'class': 'task_field'} }) }}

    .. code-block:: html+php

        <?php echo $view['form']->widget($form['task'], array(
            'attr' => array('class' => 'task_field'),
        )) ?>

Twig Template Function Reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you're using Twig, a full reference of the form rendering functions is
available in the :doc:`reference manual</reference/forms/twig_reference>`.
Read this to know everything about the helpers available and the options
that can be used with each.

.. index::
   single: Forms; Creating form classes

.. _book-form-creating-form-classes:

Creating Form Classes
---------------------

As you've seen, a form can be created and used directly in a controller.
However, a better practice is to build the form in a separate, standalone PHP
class, which can then be reused anywhere in your application. Create a new class
that will house the logic for building the task form:

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

This new class contains all the directions needed to create the task form
(note that the ``getName()`` method should return a unique identifier for this
form "type"). It can be used to quickly build a form object in the controller:

.. code-block:: php

    // src/Acme/TaskBundle/Controller/DefaultController.php

    // add this new use statement at the top of the class
    use Acme\TaskBundle\Form\Type\TaskType;

    public function newAction()
    {
        $task = // ...
        $form = $this->createForm(new TaskType(), $task);

        // ...
    }

Placing the form logic into its own class means that the form can be easily
reused elsewhere in your project. This is the best way to create forms, but
the choice is ultimately up to you.

.. sidebar:: Setting the ``data_class``

    Every form needs to know the name of the class that holds the underlying
    data (e.g. ``Acme\TaskBundle\Entity\Task``). Usually, this is just guessed
    based off of the object passed to the second argument to ``createForm``
    (i.e. ``$task``). Later, when you begin embedding forms, this will no
    longer be sufficient. So, while not always necessary, it's generally a
    good idea to explicitly specify the ``data_class`` option by add the
    following to your form type class::

        public function getDefaultOptions(array $options)
        {
            return array(
                'data_class' => 'Acme\TaskBundle\Entity\Task',
            );
        }

.. index::
   pair: Forms; Doctrine

Forms and Doctrine
------------------

The goal of a form is to translate data from an object (e.g. ``Task``) to an
HTML form and then translate user-submitted data back to the original object. As
such, the topic of persisting the ``Task`` object to the database is entirely
unrelated to the topic of forms. But, if you've configured the ``Task`` class
to be persisted via Doctrine (i.e. you've added
:ref:`mapping metadata<book-doctrine-adding-mapping>` for it), then persisting
it after a form submission can be done when the form is valid::

    if ($form->isValid()) {
        $em = $this->getDoctrine()->getEntityManager();
        $em->persist($task);
        $em->flush();

        return $this->redirect($this->generateUrl('task_success'));
    }

If, for some reason, you don't have access to your original ``$task`` object,
you can fetch it from the form::

    $task = $form->getData();

For more information, see the :doc:`Doctrine ORM chapter</book/doctrine>`.

The key thing to understand is that when the form is bound, the submitted
data is transferred to the underlying object immediately. If you want to
persist that data, you simply need to persist the object itself (which already
contains the submitted data).

.. index::
   single: Forms; Embedded forms

Embedded Forms
--------------

Often, you'll want to build a form that will include fields from many different
objects. For example, a registration form may contain data belonging to
a ``User`` object as well as many ``Address`` objects. Fortunately, this
is easy and natural with the form component.

Embedding a Single Object
~~~~~~~~~~~~~~~~~~~~~~~~~

Suppose that each ``Task`` belongs to a simple ``Category`` object. Start,
of course, by creating the ``Category`` object::

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

Next, add a new ``category`` property to the ``Task`` class::

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

Now that your application has been updated to reflect the new requirements,
create a form class so that a ``Category`` object can be modified by the user::

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

The end goal is to allow the ``Category`` of a ``Task`` to be modified right
inside the task form itself. To accomplish this, add a ``category`` field
to the ``TaskType`` object whose type is an instance of the new ``CategoryType``
class:

.. code-block:: php

    public function buildForm(FormBuilder $builder, array $options)
    {
        // ...

        $builder->add('category', new CategoryType());
    }

The fields from ``CategoryType`` can now be rendered alongside those from
the ``TaskType`` class. Render the ``Category`` fields in the same way
as the original ``Task`` fields:

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

When the user submits the form, the submitted data for the ``Category`` fields
are used to construct an instance of ``Category``, which is then set on the
``category`` field of the ``Task`` instance.

The ``Category`` instance is accessible naturally via ``$task->getCategory()``
and can be persisted to the database or used however you need.

Embedding a Collection of Forms
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You can also embed a collection of forms into one form. This is done by
using the ``collection`` field type. For more information, see the
:doc:`collection field type reference</reference/forms/types/collection>`.

.. index::
   single: Forms; Theming
   single: Forms; Customizing fields

.. _form-theming:

Form Theming
------------

Every part of how a form is rendered can be customized. You're free to change
how each form "row" renders, change the markup used to render errors, or
even customize how a ``textarea`` tag should be rendered. Nothing is off-limits,
and different customizations can be used in different places.

Symfony uses templates to render each and every part of a form, such as
``label`` tags, ``input`` tags, error messages and everything else.

In Twig, each form "fragment" is represented by a Twig block. To customize
any part of how a form renders, you just need to override the appropriate block.

In PHP, each form "fragment" is rendered via an individual template file.
To customize any part of how a form renders, you just need to override the
existing template by creating a new one.

To understand how this works, let's customize the ``form_row`` fragment and
add a class attribute to the ``div`` element that surrounds each row. To
do this, create a new template file that will store the new markup:

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

The ``field_row`` form fragment is used when rendering most fields via the
``form_row`` function. To tell the form component to use your new ``field_row``
fragment defined above, add the following to the top of the template that
renders the form:

.. configuration-block:: php

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}

        {% form_theme form 'AcmeTaskBundle:Form:fields.html.twig' %}

        <form ...>

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Default/new.html.php -->

        <?php $view['form']->setTheme($form, array('AcmeTaskBundle:Form')) ?>

        <form ...>

The ``form_theme`` tag (in Twig) "imports" the fragments defined in the given
template and uses them when rendering the form. `In other words, when the
``form_row`` function is called later in this template, it will use the ``field_row``
block from your custom theme (instead of the default ``field_row`` block
that ships with Symfony).

To customize any portion of a form, you just need to override the appropriate
fragment. Knowing exactly which block or file to override is the subject of
the next section.

For a more extensive discussion, see :doc:`/cookbook/form/form_customization`.

.. index::
   single: Forms; Template fragment naming

.. _form-template-blocks:

Form Fragment Naming
~~~~~~~~~~~~~~~~~~~~

In Symfony, every part a form that is rendered - HTML form elements, errors,
labels, etc - is defined in a base theme, which is a collection of blocks
in Twig and a collection of template files in PHP.

In Twig, every block needed is defined in a single template file (`form_div_layout.html.twig`_)
that lives inside the `Twig Bridge`_. Inside this file, you can see every block
needed to render a form and every default field type.

In PHP, the fragments are individual template files. By default they are located in
the `Resources/views/Form` directory of the framework bundle (`view on GitHub`_).

Each fragment name follows the same basic pattern and is broken up into two pieces,
separated by a single underscore character (``_``). A few examples are:

* ``field_row`` - used by ``form_row`` to render most fields;
* ``textarea_widget`` - used by ``form_widget`` to render a ``textarea`` field
  type;
* ``field_errors`` - used by ``form_errors`` to render errors for a field;

Each fragment follows the same basic pattern: ``type_part``. The ``type`` portion
corresponds to the field *type* being rendered (e.g. ``textarea``, ``checkbox``,
``date``, etc) whereas the ``part`` portion corresponds to *what* is being
rendered (e.g. ``label``, ``widget``, ``errors``, etc). By default, there
are 4 possible *parts* of a form that can be rendered:

+-------------+--------------------------+---------------------------------------------------------+
| ``label``   | (e.g. ``field_label``)   | renders the field's label                               |
+-------------+--------------------------+---------------------------------------------------------+
| ``widget``  | (e.g. ``field_widget``)  | renders the field's HTML representation                 |
+-------------+--------------------------+---------------------------------------------------------+
| ``errors``  | (e.g. ``field_errors``)  | renders the field's errors                              |
+-------------+--------------------------+---------------------------------------------------------+
| ``row``     | (e.g. ``field_row``)     | renders the field's entire row (label, widget & errors) |
+-------------+--------------------------+---------------------------------------------------------+

.. note::

    There are actually 3 other *parts*  - ``rows``, ``rest``, and ``enctype`` -
    but you should rarely if ever need to worry about overriding them.

By knowing the field type (e.g. ``textarea``) and which part you want to
customize (e.g. ``widget``), you can construct the fragment name that needs
to be overridden (e.g. ``textarea_widget``).

.. index::
   single: Forms; Template Fragment Inheritance

Template Fragment Inheritance
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In some cases, the fragment you want to customize will appear to be missing.
For example, there is no ``textarea_errors`` fragment in the default themes
provided with Symfony. So how are the errors for a textarea field rendered?

The answer is: via the ``field_errors`` fragment. When Symfony renders the errors
for a textarea type, it looks first for a ``textarea_errors`` fragment before
falling back to the ``field_errors`` fragment. Each field type has a *parent*
type (the parent type of ``textarea`` is ``field``), and Symfony uses the
fragment for the parent type if the base fragment doesn't exist.

So, to override the errors for *only* ``textarea`` fields, copy the
``field_errors`` fragment, rename it to ``textarea_errors`` and customize it. To
override the default error rendering for *all* fields, copy and customize the
``field_errors`` fragment directly.

.. tip::

    The "parent" type of each field type is available in the
    :doc:`form type reference</reference/forms/types>` for each field type.

.. index::
   single: Forms; Global Theming

Global Form Theming
~~~~~~~~~~~~~~~~~~~

In the above example, you used the ``form_theme`` helper (in Twig) to "import"
the custom form fragments into *just* that form. You can also tell Symfony
to import form customizations across your entire project.

Twig
....

To automatically include the customized blocks from the ``fields.html.twig``
template created earlier in *all* templates, modify your application configuration
file:

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

Any blocks inside the ``fields.html.twig`` template are now used globally
to define form output.

.. sidebar::  Customizing Form Output all in a Single File with Twig

    In Twig, you can also customize a form block right inside the template
    where that customization is needed:

    .. code-block:: html+jinja

        {% extends '::base.html.twig' %}

        {# import "_self" as the form theme #}
        {% form_theme form _self %}

        {# make the form fragment customization #}
        {% block field_row %}
            {# custom field row output #}
        {% endblock field_row %}

        {% block content %}
            {# ... #}

            {{ form_row(form.task) }}
        {% endblock %}

    The ``{% form_theme form _self %}`` tag allows form blocks to be customized
    directly inside the template that will use those customizations. Use
    this method to quickly make form output customizations that will only
    ever be needed in a single template.

PHP
...

To automatically include the customized templates from the ``Acme/TaskBundle/Resources/views/Form``
directory created earlier in *all* templates, modify your application configuration
file:

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

Any fragments inside the ``Acme/TaskBundle/Resources/views/Form`` directory
are now used globally to define form output.

.. index::
   single: Forms; CSRF Protection

.. _forms-csrf:

CSRF Protection
---------------

CSRF - or `Cross-site request forgery`_ - is a method by which a malicious
user attempts to make your legitimate users unknowingly submit data that
they don't intend to submit. Fortunately, CSRF attacks can be prevented by
using a CSRF token inside your forms.

The good news is that, by default, Symfony embeds and validates CSRF tokens
automatically for you. This means that you can take advantage of the CSRF
protection without doing anything. In fact, every form in this chapter has
taken advantage of the CSRF protection!

CSRF protection works by adding a hidden field to your form - called ``_token``
by default - that contains a value that only you and your user knows. This
ensures that the user - not some other entity - is submitting the given data.
Symfony automatically validates the presence and accuracy of this token.

The ``_token`` field is a hidden field and will be automatically rendered
if you include the ``form_rest()`` function in your template, which ensures
that all un-rendered fields are output.

The CSRF token can be customized on a form-by-form basis. For example::

    class TaskType extends AbstractType
    {
        // ...
    
        public function getDefaultOptions(array $options)
        {
            return array(
                'data_class'      => 'Acme\TaskBundle\Entity\Task',
                'csrf_protection' => true,
                'csrf_field_name' => '_token',
                // a unique key to help generate the secret token
                'intention'       => 'task_item',
            );
        }
        
        // ...
    }

To disable CSRF protection, set the ``csrf_protection`` option to false.
Customizations can also be made globally in your project. For more information,
see the :ref:`form configuration reference </reference-frameworkbundle-forms>`
section.

.. note::

    The ``intention`` option is optional but greatly enhances the security of
    the generated token by making it different for each form.

Final Thoughts
--------------

You now know all of the building blocks necessary to build complex and
functional forms for your application. When building forms, keep in mind that
the first goal of a form is to translate data from an object (``Task``) to an
HTML form so that the user can modify that data. The second goal of a form is to
take the data submitted by the user and to re-apply it to the object.

There's still much more to learn about the powerful world of forms, such as
how to handle :doc:`file uploads with Doctrine
</cookbook/doctrine/file_uploads>` or how to create a form where a dynamic
number of sub-forms can be added (e.g. a todo list where you can keep adding
more fields via Javascript before submitting). See the cookbook for these
topics. Also, be sure to lean on the
:doc:`field type reference documentation</reference/forms/types>`, which
includes examples of how to use each field type and its options.

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/doctrine/file_uploads`
* :doc:`File Field Reference </reference/forms/types/file>`
* :doc:`Creating Custom Field Types </cookbook/form/create_custom_field_type>`
* :doc:`/cookbook/form/form_customization`

.. _`Symfony2 Form Component`: https://github.com/symfony/Form
.. _`DateTime`: http://php.net/manual/en/class.datetime.php
.. _`Twig Bridge`: https://github.com/symfony/symfony/tree/master/src/Symfony/Bridge/Twig
.. _`form_div_layout.html.twig`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_layout.html.twig
.. _`Cross-site request forgery`: http://en.wikipedia.org/wiki/Cross-site_request_forgery
.. _`view on GitHub`: https://github.com/symfony/symfony/tree/master/src/Symfony/Bundle/FrameworkBundle/Resources/views/Form
