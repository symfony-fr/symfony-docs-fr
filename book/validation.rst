.. index::
   single: Validation

Validation
==========

La validation est une tâche très commune dans les applications web. Les données saisies dans les formulaires
doivent être validées. Les données doivent également être validés avant d'être écrites
dans une base de données ou transmises à un service Web.

Symfony2 est livré avec un composant `Validator`_ qui rend cette tâche facile et transparente.
Ce composant est basé sur le `JSR303 Bean Validation specification`_. Quoi ?
Une spécification Java en PHP? Vous avez bien entendu, mais ce n'est pas aussi mauvais que ça en a l'air.
Regardons comment il peut être utilisé en PHP.


.. index:
   single: Validation; Les basiques

Les Basiques de la Validation
------------------------

La meilleure façon de comprendre la validation est de la voir en action. Pour commencer, supposons
que vous avez créé un bon vieil objet PHP que vous avez besoin d'utiliser quelque part dans
votre application:

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;
    
    class Author
    {
        public $name;
    }

Jusqu'à présent, ceci est juste une classe ordinaire qui est utile dans votre
application. L'objectif de la validation est de vous dire si oui ou non les données
d'un objet est valide. Pour que cela fonctionne, vous allez configurer une liste de règles
(appelée :ref:`constraints<validation-constraints>`) que l'objet doit
suivre pour être valide. Ces règles peuvent être spécifiées via un certain nombre de
différents formats (YAML, XML, les annotations, ou PHP).

Par exemple, pour garantir que la propriété ``$name`` n'est pas vide, ajoutez le code
suivant:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                name:
                    - NotBlank: ~

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/services/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="name">
                    <constraint name="NotBlank" />
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             */
            public $name;
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Author
        {
            public $name;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('name', new NotBlank());
            }
        }

.. tip::

    Les propriétés protégées et privées peuvent également être validées, ainsi que les
    méthodes "getter" (voir `validator-constraint-targets`).
	
.. index::
   single: Validation; Utiliser le validator

Utiliser le Service ``validator``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ensuite, pour vraiment valider un objet ``Author``, utilisez la méthode ``validate``
du service ``validator`` (class :class:`Symfony\\Component\\Validator\\Validator`).
Le travail du ``validator`` est simple : lire les contraintes (ex : règles)
d'une classe et vérifier si oui ou non les données sur l'objet satisfont ces
contraintes. Si la validation échoue, un tableau d'erreurs est retourné. Prenez cet
exemple simple provenant de l'intérieur d'un contrôleur:

.. code-block:: php

    use Symfony\Component\HttpFoundation\Response;
    use Acme\BlogBundle\Entity\Author;
    // ...

    public function indexAction()
    {
        $author = new Author();
        // ... do something to the $author object

        $validator = $this->get('validator');
        $errorList = $validator->validate($author);

        if (count($errorList) > 0) {
            return new Response(print_r($errorList, true));
        } else {
            return new Response('The author is valid! Yes!');
        }
    }

Si la propriété ``$name`` est vide, vous allez voir le message d'erreur
suivant:

.. code-block:: text

    Acme\BlogBundle\Author.name:
        This value should not be blank

Si vous insérez une valeur dans la propriété ``name``, le message de succès
va apparaître.

.. tip::

	La plupart du temps, vous n'aurez pas à interagir directement avec le service ``validator``
	ou besoin de vous inquiéter concernant l'affichage des erreurs. La plupart du temps,
	vous allez utiliser la validation indirectement lors de la soumission des données du
	formulaire. Pour plus d'informations, consultez le :ref:`book-validation-forms`.
    
Vous pouvez aussi passer une collection d'erreur à un template.

.. code-block:: php

    if (count($errorList) > 0) {
        return $this->render('AcmeBlogBundle:Author:validate.html.twig', array(
            'errorList' => $errorList,
        ));
    } else {
        // ...
    }

A l'intérieur d'un template, vous pouvez afficher la liste des erreurs comme vous voulez:
	

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Author/validate.html.twig #}

        <h3>The author has the following errors</h3>
        <ul>
        {% for error in errorList %}
            <li>{{ error.message }}</li>
        {% endfor %}
        </ul>

    .. code-block:: html+php

        <!-- src/Acme/BlogBundle/Resources/views/Author/validate.html.php -->

        <h3>The author has the following errors</h3>
        <ul>
        <?php foreach ($errorList as $error): ?>
            <li><?php echo $error->getMessage() ?></li>
        <?php endforeach; ?>
        </ul>

.. note::

    Chaque erreur de validation (appelée une "violation de contrainte"), est représentée
    par un objet :class:`Symfony\\Component\\Validator\\ConstraintViolation`.

.. index::
   single: Validation; Validation avec les formulaires

.. _book-validation-forms:

Validation et Formulaires
~~~~~~~~~~~~~~~~~~~~

Le service ``validator`` peut être utilisé à tout moment pour valider n'importe quel objet.
En réalité, cependant, vous travaillerez habituellement avec le ``validator`` indirectement
lorsque vous utilisez les formulaires. La bibliothèque de formulaires de Symfony utilise 
le service ``validator`` en interne pour valider l'objet implicite après que les valeurs ont 
été soumis et liés. Les violations de contraintes sur l'objet sont convertis en objets 
``FieldError`` qui peuvent être facilement affichés dans votre formulaire. Le workflow de 
soumission d'un formulaire typique ressemble à ce qui suit de l'intérieur d'un contrôleur::


    use Acme\BlogBundle\Entity\Author;
    use Acme\BlogBundle\Form\AuthorType;
    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function updateAction(Request $request)
    {
        $author = new Acme\BlogBundle\Entity\Author();
        $form = $this->createForm(new AuthorType(), $author);
        
        if ($request->getMethod() == 'POST') {
            $form->bindRequest($request);
            
            if ($form->isValid()) {
                // the validation passed, do something with the $author object
                
                $this->redirect($this->generateUrl('...'));
            }
        }
        
        return $this->render('BlogBundle:Author:form.html.twig', array(
            'form' => $form->createView(),
        ));
    }

.. note::

    Cet exemple utilise une classe de formulaire ``AuthorType``, qui n'est pas montrée ici.
	
Pour plus d'information, voir le chapitre :doc:`Forms</book/forms>`.

.. index::
   pair: Validation; Configuration

.. _book-validation-configuration:

Configuration
-------------

Le validateur Symfony2 est activée par défaut, mais vous devez activer explicitement
les annotations, si vous utilisez la méthode d'annotation pour spécifier vos contraintes :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            validation: { enable_annotations: true }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:validation enable_annotations="true" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array('validation' => array(
            'enable_annotations' => true,
        )));

.. index::
   single: Validation; Constraints

.. _validation-constraints:

Contraintes
-----------

The ``validator`` is designed to validate objects against *constraints* (i.e.
rules). In order to validate an object, simply map one or more constraints
to its class and then pass it to the ``validator`` service.

Behind the scenes, a constraint is simply a PHP object that makes an assertive
statement. In real life, a constraint could be: "The cake must not be burned".
In Symfony2, constraints are similar: they are assertions that a condition
is true. Given a value, a constraint will tell you whether or not that value
adheres to the rules of the constraint.

Supported Constraints
~~~~~~~~~~~~~~~~~~~~~

Symfony2 packages a large number of the most commonly-needed constraints.
The full list of constraints with details is available in the
:doc:`constraints reference section</reference/constraints>`.

.. index::
   single: Validation; Constraints configuration

.. _book-validation-constraint-configuration:

Constraint Configuration
~~~~~~~~~~~~~~~~~~~~~~~~

Some constraints, like :doc:`NotBlank</reference/constraints/NotBlank>`,
are simple whereas others, like the :doc:`Choice</reference/constraints/Choice>`
constraint, have several configuration options available. Suppose that the
``Author`` class has another property, ``gender`` that can be set to either
"male" or "female":

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: { choices: [male, female], message: Choose a valid gender. }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/services/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <option name="choices">
                            <value>male</value>
                            <value>female</value>
                        </option>
                        <option name="message">Choose a valid gender.</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(
             *     choices = { "male", "female" },
             *     message = "Choose a valid gender."
             * )
             */
            public $gender;
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Author
        {
            public $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Choice(array(
                    'choices' => array('male', 'female'),
                    'message' => 'Choose a valid gender.',
                )));
            }
        }

The options of a constraint can always be passed in as an array. Some constraints,
however, also allow you to pass the value of one, "*default*", option in place
of the array. In the case of the ``Choice`` constraint, the ``choices``
options can be specified in this way.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: [male, female]

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/services/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <value>male</value>
                        <value>female</value>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice({"male", "female"})
             */
            protected $gender;
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Choice;

        class Author
        {
            protected $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Choice(array('male', 'female')));
            }
        }

This is purely meant to make the configuration of the most common option of
a constraint shorter and quicker.

If you're ever unsure of how to specify an option, either check the API documentation
for the constraint or play it safe by always passing in an array of options
(the first method shown above).

.. index::
   single: Validation; Constraint targets

.. _validator-constraint-targets:

Constraint Targets
------------------

Constraints can be applied to a class property (e.g. ``name``) or a public
getter method (e.g. ``getFullName``). The first is the most common and easy
to use, but the second allows you to specify more complex validation rules.

.. index::
   single: Validation; Property constraints

Properties
~~~~~~~~~~

Validating class properties is the most basic validation technique. Symfony2
allows you to validate private, protected or public properties. The next
listing shows you how to configure the ``$firstName`` property of an ``Author``
class to have at least 3 characters.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - NotBlank: ~
                    - MinLength: 3

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="NotBlank" />
                <constraint name="MinLength">3</constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             * @Assert\MinLength(3)
             */
            private $firstName;
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\MinLength;

        class Author
        {
            private $firstName;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new NotBlank());
                $metadata->addPropertyConstraint('firstName', new MinLength(3));
            }
        }

.. index::
   single: Validation; Getter constraints

Getters
~~~~~~~

Constraints can also be applied to the return value of a method. Symfony2
allows you to add a constraint to any public method whose name starts with
"get" or "is". In this guide, both of these types of methods are referred
to as "getters".

The benefit of this technique is that it allows you to validate your object
dynamically. For example, suppose you want to make sure that a password field
doesn't match the first name of the user (for security reasons). You can
do this by creating an ``isPasswordLegal`` method, and then asserting that
this method must return ``true``:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            getters:
                passwordLegal:
                    - True: { message: "The password cannot match your first name" }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <getter property="passwordLegal">
                <constraint name="True">
                    <option name="message">The password cannot match your first name</option>
                </constraint>
            </getter>
        </class>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\True(message = "The password cannot match your first name")
             */
            public function isPasswordLegal()
            {
                // return true or false
            }
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\True;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addGetterConstraint('passwordLegal', new True(array(
                    'message' => 'The password cannot match your first name',
                )));
            }
        }

Now, create the ``isPasswordLegal()`` method, and include the logic you need::

    public function isPasswordLegal()
    {
        return ($this->firstName != $this->password);
    }

.. note::

    The keen-eyed among you will have noticed that the prefix of the getter
    ("get" or "is") is omitted in the mapping. This allows you to move the
    constraint to a property with the same name later (or vice versa) without
    changing your validation logic.

.. _book-validation-validation-groups:

Validation Groups
-----------------

So far, you've been able to add constraints to a class and ask whether or
not that class passes all of the defined constraints. In some cases, however,
you'll need to validate an object against only *some* of the constraints
on that class. To do this, you can organize each constraint into one or more
"validation groups", and then apply validation against just one group of
constraints.

For example, suppose you have a ``User`` class, which is used both when a
user registers and when a user updates his/her contact information later:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\User:
            properties:
                email:
                    - Email: { groups: [registration] }
                password:
                    - NotBlank: { groups: [registration] }
                    - MinLength: { limit: 7, groups: [registration] }
                city:
                    - MinLength: 2

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\User">
            <property name="email">
                <constraint name="Email">
                    <option name="groups">
                        <value>registration</value>
                    </option>
                </constraint>
            </property>
            <property name="password">
                <constraint name="NotBlank">
                    <option name="groups">
                        <value>registration</value>
                    </option>
                </constraint>
                <constraint name="MinLength">
                    <option name="limit">7</option>
                    <option name="groups">
                        <value>registration</value>
                    </option>
                </constraint>
            </property>
            <property name="city">
                <constraint name="MinLength">7</constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/User.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Security\Core\User\UserInterface
        use Symfony\Component\Validator\Constraints as Assert;

        class User implements UserInterface
        {
            /**
            * @Assert\Email(groups={"registration"})
            */
            private $email;

            /**
            * @Assert\NotBlank(groups={"registration"})
            * @Assert\MinLength(limit=7, groups={"registration"})
            */
            private $password;

            /**
            * @Assert\MinLength(2)
            */
            private $city;
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/User.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Email;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\MinLength;

        class User
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('email', new Email(array(
                    'groups' => array('registration')
                )));

                $metadata->addPropertyConstraint('password', new NotBlank(array(
                    'groups' => array('registration')
                )));
                $metadata->addPropertyConstraint('password', new MinLength(array(
                    'limit'  => 7,
                    'groups' => array('registration')
                )));

                $metadata->addPropertyConstraint('city', new MinLength(3));
            }
        }

With this configuration, there are two validation groups:

* ``Default`` - contains the constraints not assigned to any other group;

* ``registration`` - contains the constraints on the ``email`` and ``password``
  fields only.

To tell the validator to use a specific group, pass one or more group names
as the second argument to the ``validate()`` method::

    $errorList = $validator->validate($author, array('registration'));

Of course, you'll usually work with validation indirectly through the form
library. For information on how to use validation groups inside forms, see
:ref:`book-forms-validation-groups`.

Final Thoughts
--------------

The Symfony2 ``validator`` is a powerful tool that can be leveraged to
guarantee that the data of any object is "valid". The power behind validation
lies in "constraints", which are rules that you can apply to properties or
getter methods of your object. And while you'll most commonly use the validation
framework indirectly when using forms, remember that it can be used anywhere
to validate any object.

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/validation/custom_constraint`

.. _Validator: https://github.com/symfony/Validator
.. _JSR303 Bean Validation specification: http://jcp.org/en/jsr/detail?id=303
