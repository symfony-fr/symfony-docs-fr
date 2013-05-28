Regex
=====

Valide qu'une valeur correspond à une expression régulière.

+----------------+-----------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`               |
+----------------+-----------------------------------------------------------------------+
| Options        | - `pattern`_                                                          |
|                | - `match`_                                                            |
|                | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Regex`            |
+----------------+-----------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\RegexValidator`   |
+----------------+-----------------------------------------------------------------------+

Utilisation de base
-------------------

Supposons que vous ayez un champ ``description`` et que vous vouliez vérifier
qu'il commence bien par un caractère alphanumérique. L'expression régulière
qui teste cela serait ``/^\w+/``, indiquant que vous cherchez au moins un ou
plusieurs caractères alphanumériques au début de votre chaîne :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                description:
                    - Regex: "/^\w+/"

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Regex("/^\w+/")
             */
            protected $description;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="description">
                <constraint name="Regex">
                    <option name="pattern">/^\w+/</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('description', new Assert\Regex(array(
                    'pattern' => '/^\w+/',
                )));
            }
        }

Alternativement, vous pouvez définir l'option `match`_ à ``false`` pour
vérifier qu'une chaîne donnée ne corresponde *pas*. Dans l'exemple suivant,
vous vérifiez que le champ ``firstName`` ne contient pas de nombre et vous
personnalisez également le message :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - Regex:
                        pattern: "/\d/"
                        match:   false
                        message: Votre nom ne peut pas contenir de nombre

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Regex(
             *     pattern="/\d/",
             *     match=false,
             *     message="Votre nom ne peut pas contenir de nombre"
             * )
             */
            protected $firstName;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="Regex">
                    <option name="pattern">/\d/</option>
                    <option name="match">false</option>
                    <option name="message">Votre nom ne peut pas contenir de nombre</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new Assert\Regex(array(
                    'pattern' => '/\d/',
                    'match'   => false,
                    'message' => 'Votre nom ne peut pas contenir de nombre',
                )));
            }
        }
Options
-------

pattern
~~~~~~~

**type**: ``string`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire est le masque (« pattern » en anglais) de l'expression
régulière à laquelle doit correspondre la donnée. Par défaut, le validateur
échouera si la chaîne de caractères *ne correspond pas* à cette expression
régulière (via la fonction PHP :phpfunction:`preg_match`).
Toutefois, si l'option `match`_ est définie à false, la validation échouera
si la chaîne *correspond* à l'expression régulière.

match
~~~~~

**type**: ``Boolean`` default: ``true``

Si cette option est à ``true`` (ou non définie), la validation passera si la chaîne
donnée correspond au `pattern`_ de l'expression régulière. Toutefois, si cette option
est définie à ``false``, l'inverse se passera : la validation passera uniquement si
la chaîne donnée ne correspond **pas** au `pattern`_ de l'expression régulière.

message
~~~~~~~

**type**: ``string`` **default**: ``This value is not valid``

Le message qui sera affiché si la validation échoue.