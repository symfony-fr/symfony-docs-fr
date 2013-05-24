LessThanOrEqual
===============

.. versionadded:: 2.3
    C'est une nouvelle contrainte depuis la version 2.3.

Valide si une valeur est plus petite ou égalequ'une autre, définie dans les options.
Pour vérifier qu'une valeur est plus petite qu'une autre, voir :doc:`/reference/constraints/LessThan`.

+----------------+-------------------------------------------------------------------------------+
| S'applique à   | :ref:`property or method<validation-property-target>`                         |
+----------------+-------------------------------------------------------------------------------+
| Options        | - `value`_                                                                    |
|                | - `message`_                                                                  |
+----------------+-------------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\LessThanOrEqual`          |
+----------------+-------------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\LessThanOrEqualValidator` |
+----------------+-------------------------------------------------------------------------------+

Utilisation de base
-------------------

Si vous voulez vous assurer que le champ ``age`` d'une classe ``Person`` est plus
petit ou égale à ``80``, vous pouvez faire ceci :

.. configuration-block::

    .. code-block:: yaml

        # src/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - LessThanOrEqual:
                        value: 80

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\LessThanOrEqual(
             *     value = 80
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <class name="Acme\SocialBundle\Entity\Person">
            <property name="age">
                <constraint name="LessThanOrEqual">
                    <option name="value">80</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('age', new Assert\LessThanOrEqual(array(
                    'value' => 80,
                )));
            }
        }

Options
-------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**type**: ``string`` **default**: ``This value should be less than or equal to {{ compared_value }}``

Le message qui sera affiché si la valeur n'est pas plus petite ou égale.