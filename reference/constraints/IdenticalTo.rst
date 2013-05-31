IdenticalTo
===========

.. versionadded:: 2.3
    C'est une nouvelle contrainte depuis la version 2.3.

Valide si une valeur est identique à une autre, définie dans les options.
Pour vérifier qu'une valeur n'est pas identique, voir :doc:`/reference/constraints/NotIdenticalTo`.

.. caution::

    Cette contrainte comparative utilise ``===``, donc ``3`` et ``"3"`` *ne* sont *pas* considérés
    comme égale. Utilisez :doc:`/reference/constraints/EqualTo` pour comparer avec ``==``.

+----------------+--------------------------------------------------------------------------+
| S'applique à   | :ref:`property or method<validation-property-target>`                    |
+----------------+--------------------------------------------------------------------------+
| Options        | - `value`_                                                               |
|                | - `message`_                                                             |
+----------------+--------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\IdenticalTo`         |
+----------------+--------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\IdenticalToValidator`|
+----------------+--------------------------------------------------------------------------+

Utilisation de base
-------------------

Si vous voulez vous assurer que le champ ``age`` d'une classe ``Person`` est équal à
``20`` et que c'est un entier ,vous pouvez faire ceci :

.. configuration-block::

    .. code-block:: yaml

        # src/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - IdenticalTo:
                        value: 20

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\IdenticalTo(
             *     value = 20
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <class name="Acme\SocialBundle\Entity\Person">
            <property name="age">
                <constraint name="IdenticalTo">
                    <option name="value">20</option>
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
                $metadata->addPropertyConstraint('age', new Assert\IdenticalTo(array(
                    'value' => 20,
                )));
            }
        }

Options
-------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**type**: ``string`` **default**: ``This value should be identical to {{ compared_value_type }} {{ compared_value }}``

Ce message sera affiché si la valeur n'est pas identique.
