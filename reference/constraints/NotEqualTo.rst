NotEqualTo
==========

.. versionadded:: 2.3
    C'est une nouvelle contrainte depuis la version 2.3.

Valide si une valeur n'est **pas** égale à une autre, définie dans les options.
Pour vérifier qu'une valeur est égale, voir :doc:`/reference/constraints/EqualTo`.

.. caution::

    Cette contrainte comparative utilise ``!=``, donc ``3`` et ``"3"`` sont considérés
    comme égale. Utilisez :doc:`/reference/constraints/NotIdenticalTo`
    pour comparer avec ``!==``.

+----------------+-------------------------------------------------------------------------+
| S'applique à   | :ref:`property or method<validation-property-target>`                   |
+----------------+-------------------------------------------------------------------------+
| Options        | - `value`_                                                              |
|                | - `message`_                                                            |
+----------------+-------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\NotEqualTo`         |
+----------------+-------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\NotEqualToValidator`|
+----------------+-------------------------------------------------------------------------+

Utilisation de base
-------------------

Si vous voulez vous assurer que le champ ``age`` d'une classe ``Person`` n'est pas
équal à ``15`` ,vous pouvez faire ceci :

.. configuration-block::

    .. code-block:: yaml

        # src/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - NotEqualTo:
                        value: 15

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\NotEqualTo(
             *     value = 15
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <class name="Acme\SocialBundle\Entity\Person">
            <property name="age">
                <constraint name="NotEqualTo">
                    <option name="value">15</option>
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
                $metadata->addPropertyConstraint('age', new Assert\NotEqualTo(array(
                    'value' => 15,
                )));
            }
        }

Options
-------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**type**: ``string`` **default**: ``This value should not be equal to {{ compared_value }}``

Le message qui sera affiché si la valeur est égale.
