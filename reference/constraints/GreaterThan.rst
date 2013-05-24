GreaterThan
===========

.. versionadded:: 2.3
    C'est une nouvelle contrainte depuis la version 2.3.

Valide si une valeur est plus grande qu'une autre, définie dans les options.
Pour vérifier qu'une valeur est plus grande ou égale, voir :doc:`/reference/constraints/GreaterThanOrEqual`.
Pour vérifier qu'une valeur est plus petite, voir :doc:`/reference/constraints/LessThan`.

+----------------+---------------------------------------------------------------------------+
| S'applique à   | :ref:`property or method<validation-property-target>`                     |
+----------------+---------------------------------------------------------------------------+
| Options        | - `value`_                                                                |
|                | - `message`_                                                              |
+----------------+---------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\GreaterThan`          |
+----------------+---------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\GreaterThanValidator` |
+----------------+---------------------------------------------------------------------------+

Utilisation de base
-------------------

Si vous voulez vous assurer que le champ ``age`` d'une classe ``Person`` est plus grand que
``18`` ,vous pouvez faire ceci :

.. configuration-block::

    .. code-block:: yaml

        # src/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - GreaterThan:
                        value: 18

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\GreaterThan(
             *     value = 18
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <class name="Acme\SocialBundle\Entity\Person">
            <property name="age">
                <constraint name="GreaterThan">
                    <option name="value">18</option>
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
                $metadata->addPropertyConstraint('age', new Assert\GreaterThan(array(
                    'value' => 18,
                )));
            }
        }

Options
-------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**type**: ``string`` **default**: ``This value should be greater than {{ compared_value }}``

Ce message sera affiché si la valeur n'est pas plus grande que la donnée de comparaison.
