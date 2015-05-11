.. index::
   single: Validation

Validation
==========

La validation est une tâche très commune dans les applications web. Les données 
saisies dans les formulaires doivent être validées. Les données doivent également
être validées avant d'être écrites dans une base de données ou transmises à un
service Web.

Symfony2 est livré avec un composant `Validator`_ qui rend cette tâche facile et
transparente. Ce composant est basé sur la `spécification JSR303`_.
Quoi ? Une spécification Java en PHP? Vous avez bien entendu, mais ce n'est pas
aussi terrible que ça en a l'air. Regardons comment elle peut être utilisée en PHP.

.. index::
   single: Validation; Les bases

Les Bases de la Validation
--------------------------

La meilleure façon de comprendre la validation est de la voir en action. Pour
commencer, supposons que vous avez créé un bon vieil objet PHP que vous avez
besoin d'utiliser quelque part dans votre application :

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;
    
    class Author
    {
        public $name;
    }

Jusqu'à présent, c'est juste une classe ordinaire qui est utile dans votre application.
L'objectif de la validation est de vous dire si oui ou non les données d'un objet
sont valides. Pour que cela fonctionne, vous allez configurer une liste de règles
(appelée :ref:`constraints<validation-constraints>`) que l'objet doit respecter pour
être valide. Ces règles peuvent être spécifiées via un certain nombre de formats
(YAML, XML, les annotations, ou PHP).

Par exemple, pour garantir que la propriété ``$name`` ne soit pas vide, ajoutez le code
suivant :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                name:
                    - NotBlank: ~

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

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="name">
                    <constraint name="NotBlank" />
                </property>
            </class>
        </constraint-mapping>

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

    Les propriétés protégées et privées peuvent également être validées, ainsi
    que les méthodes « getter » (voir validator-constraint-targets).
    
.. index::
   single: Validation; Utiliser le validator

Utiliser le Service ``validator``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ensuite, pour vraiment valider un objet ``Author``, utilisez la méthode ``validate``
du service validator (class :class:`Symfony\\Component\\Validator\\Validator`).
Le travail du ``validator`` est simple : lire les contraintes (règles) d'une classe
et vérifier si les données sur l'objet satisfont ces contraintes ou non. Si la
validation échoue, un tableau d'erreurs est retourné. Prenez cet exemple simple
d'une action d'un contrôleur :

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

Si la propriété ``$name`` est vide, vous allez voir le message d'erreur suivant :

.. code-block:: text

    Acme\BlogBundle\Author.name:
        This value should not be blank

Si vous insérez une valeur dans la propriété ``name``, le message de succès va apparaître.

.. tip::

	La plupart du temps, vous n'aurez pas à interagir directement avec le
        service ``validator`` ou besoin de vous inquiéter concernant l'affichage des erreurs.
        La plupart du temps, vous allez utiliser la validation indirectement lors
        de la soumission des données du formulaire. Pour plus d'informations,
        consultez-le :ref:`book-validation-forms`.
    
Vous pouvez aussi passer une collection d'erreurs à un template.

.. code-block:: php

    if (count($errorList) > 0) {
        return $this->render('AcmeBlogBundle:Author:validate.html.twig', array(
            'errorList' => $errorList,
        ));
    } else {
        // ...
    }

A l'intérieur d'un template, vous pouvez afficher la liste des erreurs comme
vous voulez :
	

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

    Chaque erreur de validation (appelée une « violation de contrainte »), est
    représentée par un objet :class:`Symfony\\Component\\Validator\\ConstraintViolation`.

.. index::
   single: Validation; Validation et formulaires

.. _book-validation-forms:

Validation et Formulaires
~~~~~~~~~~~~~~~~~~~~~~~~~

Le service ``validator`` peut être utilisé à tout moment pour valider n'importe
quel objet. En réalité, vous travaillerez le plus souvent avec le ``validator``
indirectement lorsque vous utilisez les formulaires. La bibliothèque de formulaires
de Symfony utilise le service ``validator`` en interne pour valider l'objet
après que les valeurs ont été soumises. Les violations de contraintes sur
l'objet sont converties en objets ``FieldError`` qui peuvent être facilement affichés
dans votre formulaire. Le processus de soumission d'un formulaire typique ressemble
au code suivant::

    // ...
    use Acme\BlogBundle\Entity\Author;
    use Acme\BlogBundle\Form\AuthorType;
    use Symfony\Component\HttpFoundation\Request;

    public function updateAction(Request $request)
    {
        $author = new Author();
        $form = $this->createForm(new AuthorType(), $author);

        $form->handleRequest($request);

        if ($form->isValid()) {
            // the validation passed, do something with the $author object

            return $this->redirect($this->generateUrl(...));
        }

        return $this->render('BlogBundle:Author:form.html.twig', array(
            'form' => $form->createView(),
        ));
    }

.. note::

    Cet exemple utilise une classe de formulaire ``AuthorType``, qui n'est pas montrée ici.

Pour plus d'informations, voir le chapitre :doc:`Forms</book/forms>`.

.. index::
   pair: Validation; Configuration

.. _book-validation-configuration:

Configuration
-------------

Le validateur Symfony2 est activé par défaut, mais vous devez activer explicitement
les annotations, si vous voulez les utiliser pour spécifier vos contraintes :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            validation: { enable_annotations: true }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:validation enable-annotations="true" />
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

Le ``validator`` est conçu pour valider des objets selon des *contraintes* 
(règles). Afin de valider un objet, il suffit d'associer une ou plusieurs contraintes
à sa classe et ensuite de le passer au service ``validator``.

Dans les coulisses, une contrainte est simplement un objet PHP qui déclare une règle.
Dans la vraie vie, une contrainte pourrait être : « Le gâteau ne doit pas être brûlé ».
En Symfony2, les contraintes sont similaires : ce sont des garanties qu'une condition
est vraie. Pour une valeur donnée, une contrainte vous dira si cette valeur obéit 
aux règles ou non.
 

Contraintes supportées
~~~~~~~~~~~~~~~~~~~~~~

Symfony2 est fourni avec un grand nombre des contraintes les plus utiles habituellement.

.. include:: /reference/constraints/map.rst.inc

Vous pouvez aussi créer vos propres contraintes. Ce sujet est abordé dans l'article «:doc:`/cookbook/validation/custom_constraint`» du cookbook.

.. index::
   single: Validation; Constraints configuration

.. _book-validation-constraint-configuration:

Configuration de contraintes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Certaines contraintes, comme :doc:`NotBlank</reference/constraints/NotBlank>`,
sont simples alors que d'autres, comme la contrainte :doc:`Choice</reference/constraints/Choice>`
ont plusieurs options de configuration disponibles. Supposons que la classe
``Author`` a une autre propriété, ``gender`` qui peut prendre comme valeur
« homme » ou « femme » :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: { choices: [homme, femme], message: Choose a valid gender. }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(
             *     choices = { "homme", "femme" },
             *     message = "Choose a valid gender."
             * )
             */
            public $gender;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <option name="choices">
                            <value>homme</value>
                            <value>femme</value>
                        </option>
                        <option name="message">Choose a valid gender.</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>


    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Choice;

        class Author
        {
            public $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Choice(array(
                    'choices' => array('homme', 'femme'),
                    'message' => 'Choose a valid gender.',
                )));
            }
        }

.. _validation-default-option:

Les options d'une contrainte peuvent toujours être passées en tant que tableau. Certaines contraintes,
cependant, vous permettent également de passer la valeur d'une option « *par défaut* »
au lieu du tableau. Dans le cas de la contrainte ``Choice``, les options de ``choices``
peuvent être spécifiées de cette manière.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: [homme, femme]

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice({"homme", "femme"})
             */
            protected $gender;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <value>homme</value>
                        <value>femme</value>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Choice;

        class Author
        {
            protected $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Choice(array('homme', 'femme')));
            }
        }

Ceci a pour unique but de rendre la configuration de l'option la plus probable
plus courte et plus rapide.

Si jamais vous n'êtes pas certain de la façon dont spécifier une option, soit vérifiez la documentation de l'API
pour la contrainte concernée, soit jouez la sécurité en passant toujours dans un tableau d'options
(la première méthode indiquée ci-dessus).


Traduction des messages de contrainte
-------------------------------------
 
Pour des informations sur la traduction des messages de contrainte,
allez voir :ref:`book-translation-constraint-messages`.

.. index::
   single: Validation; Constraint targets

.. _validator-constraint-targets:

Objectifs des contraintes
-------------------------

Les contraintes peuvent être appliquées à une propriété de classe (par ex. ``name``) ou une
méthode publique getter (par ex. ``getFullName``). La première méthode est la plus
commune et facile à utiliser, mais la seconde vous permet de spécifier des règles
de validation plus complexes.

.. index::
   single: Validation; Property constraints

.. _validation-property-target:

Propriétés
~~~~~~~~~~

Valider des propriétés de classe est la technique de validation la plus basique. Symfony2
vous permet de valider des propriétés privées, protégées ou publiques. Le prochain
exemple vous montre comment configurer la propriété ``$firstName`` d'une classe ``Author``
pour avoir au moins 3 caractères.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - NotBlank: ~
                    - Length: 
                    	min: 3

    .. code-block:: php-annotations

        // Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             * @Assert\Length(min = "3")
             */
            private $firstName;
        }


    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="NotBlank" />
                <constraint name="Length">
					<option name="min">3</option>
				</constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Length;

        class Author
        {
            private $firstName;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new NotBlank());
                $metadata->addPropertyConstraint(
					'firstName', 
					new Length(array("min" => 3)));
            }
        }

.. index::
   single: Validation; Getter constraints

Getters
~~~~~~~

Les contraintes peuvent également être appliquées à la valeur de retour d'une méthode. Symfony2
vous permet d'ajouter une contrainte à toute méthode publique dont le nom commence par
« get » ou « is ». Dans ce guide, ces deux types de méthodes sont désignées 
comme « getters ».

L'avantage de cette technique est qu'elle vous permet de valider votre objet
dynamiquement. Par exemple, supposons que vous voulez vous assurer qu'un champ
mot de passe ne correspond pas au prénom de l'utilisateur (pour des raisons de sécurité).
Vous pouvez le faire en créant une méthode ``isPasswordLegal``, puis en spécifiant que
cette méthode doit retourner ``true`` :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            getters:
                passwordLegal:
                    - "True": { message: "Le mot de passe et le prénom doivent être différents" }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\True(message = "Le mot de passe et le prénom doivent être différents")
             */
            public function isPasswordLegal()
            {
                // return true or false
            }
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <getter property="passwordLegal">
                <constraint name="True">
                    <option name="message">Le mot de passe et le prénom doivent être différents</option>
                </constraint>
            </getter>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\True;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addGetterConstraint('passwordLegal', new True(array(
                    'message' => 'Le mot de passe et le prénom doivent être différents',
                )));
            }
        }

Maintenant, créez la méthode ``isPasswordLegal()``, et faites ce dont vous avez besoin:: 

    public function isPasswordLegal()
    {
        return ($this->firstName != $this->password);
    }

.. note::

    Les plus perspicaces d'entre vous auront remarqué que le préfixe du getter
    (« get » ou « is ») est omis dans le mapping. Cela vous permet de déplacer la
    contrainte sur une propriété du même nom plus tard (ou vice versa) sans
    changer votre logique de validation.
	
.. _validation-class-target:

Classes	
~~~~~~~

Certaines contraintes s'appliquent à toute la classe qui est validée. Par exemple,
la contrainte :doc:`Callback</reference/constraints/Callback>` est une contrainte
générique qui s'applique à la classe elle-même. Lorsque cette classe est validée,
les méthodes spécifiées par cette contrainte sont simplement exécutées pour personnaliser
encore plus la validation
	
.. _book-validation-validation-groups:

Groupes de Validation
---------------------

Jusqu'ici, vous avez été en mesure d'ajouter des contraintes à une classe et demander si oui ou
non cette classe satisfait toutes les contraintes définies. Dans certains cas, cependant,
vous aurez besoin de valider un objet en ne prenant en compte que *certaines* des contraintes
de la classe. Pour ce faire, vous pouvez organiser chaque contrainte en un ou plusieurs
« groupes de validation », et ensuite appliquer la validation sur un groupe de
contraintes seulement.

Par exemple, supposons que vous avez une classe  ``User``, qui est utilisée à la fois quand un
utilisateur s'enregistre et quand un utilisateur met à jour son profil plus tard : 

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\User:
            properties:
                email:
                    - Email: { groups: [registration] }
                password:
                    - NotBlank: { groups: [registration] }
                    - Length: { min: 7, groups: [registration] }
                city:
                    - Length:
						min: 2

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/User.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Security\Core\User\UserInterface;
        use Symfony\Component\Validator\Constraints as Assert;

        class User implements UserInterface
        {
            /**
            * @Assert\Email(groups={"registration"})
            */
            private $email;

            /**
            * @Assert\NotBlank(groups={"registration"})
            * @Assert\Length(min=7, groups={"registration"})
            */
            private $password;

            /**
            * @Assert\Length(min = "2")
            */
            private $city;
        }

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
                <constraint name="Length">
                    <option name="min">7</option>
                    <option name="groups">
                        <value>registration</value>
                    </option>
                </constraint>
            </property>
            <property name="city">
                <constraint name="Length">
                    <option name="min">7</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/User.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Email;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Length;

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
                $metadata->addPropertyConstraint('password', new Length(array(
                    'min'  => 7,
                    'groups' => array('registration')
                )));

                $metadata->addPropertyConstraint(
				'city', 
				Length(array("min" => 3)));
            }
        }

Avec cette configuration, il y a deux groupes de validation :

* ``Default`` - contient les contraintes non affectées à tout autre groupe ;

* ``registration`` - contient les contraintes sur les champs ``email`` and ``password``
  seulement.

Pour dire au validateur d'utiliser un groupe spécifique, passer un ou plusieurs noms de groupe
comme deuxième argument de la méthode ``validate()`` ::

    $errorList = $validator->validate($author, array('registration'));

Bien sûr, vous travaillerez bien souvent indirectement avec la validation via la bibliothèque de
formulaire. Pour plus d'informations sur la façon d'utiliser les groupes de validation
à l'intérieur des formulaires, voir :ref:`book-forms-validation-groups`.

.. index::
   single: Validation; Validating raw values

.. _book-validation-raw-values:

Valider des valeurs et des tableaux
-----------------------------------

Jusqu'ici, vous avez vu comment valider des objets entiers. Mais souvent,
vous voudrez juste valider une simple valeur, comme par exemple vérifier qu'une
chaîne de caractères est une adresse email valide. C'est en fait très facile à
faire. Dans un contrôleur, ça ressemble à ceci::

    // Ajouter ceci en haut de votre classe
    use Symfony\Component\Validator\Constraints\Email;
    
    public function addEmailAction($email)
    {
        $emailConstraint = new Email();
        // toutes les "options" de contrainte sont définies comme suit
        $emailConstraint->message = 'Invalid email address';

        // utiliser le validator pour valider la valeur
        $errorList = $this->get('validator')->validateValue($email, $emailConstraint);

        if (count($errorList) == 0) {
            // c'est une adresse valide, faire quelque chose
        } else {
            // ce n'est *pas* une adresse valide
            $errorMessage = $errorList[0]->getMessage()
            
            // faire quelque chose avec l'erreur
        }
        
        // ...
    }

En appelant ``validateValue`` du validator, vous pouvez passer une valeur brute et
l'objet contrainte dont vous voulez valider la valeur. Une liste complète des
contraintes disponibles - ainsi que le nom de classe complet de chaque contrainte -
est disponible dans le chapitre :doc:`contraintes</reference/constraints>`.

La méthode ``validateValue`` retourne un objet :class:`Symfony\\Component\\Validator\\ConstraintViolationList`
qui se comporte comme un tableau d'erreurs. Chaque erreur de la collection est un
objet :class:`Symfony\\Component\\Validator\\ConstraintViolation`, qui contient
le message d'erreur dans sa méthode `getMessage`.

Le mot de la fin
----------------

Le ``validator`` de Symfony2 est un outil puissant qui peut être un atout pour
garantir que les données de n'importe quel objet sont « valides ». La puissance
de la validation réside dans les « contraintes », qui sont des règles que vous
pouvez appliquer aux propriétés ou aux méthodes getter de votre objet. Et tandis
que vous utiliserez plus communément le système de validation indirectement
lors de l'utilisation des formulaires, n'oubliez pas qu'il peut être utilisé partout
pour valider n'importe quel objet.

En savoir plus avec le Cookbook
-------------------------------

* :doc:`/cookbook/validation/custom_constraint`

.. _Validator: https://github.com/symfony/Validator
.. _spécification JSR303: http://jcp.org/en/jsr/detail?id=303
