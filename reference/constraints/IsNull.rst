IsNull
======

Valide qu'une valeur est exactement égale à ``null``. Pour vous assurer qu'une
propriété soit simplement vide (une chaîne de caractères vide ou ``null``), lisez la
documentation de la contrainte :doc:`/reference/constraints/Blank`.
Pour vous assurer qu'une propriété ne soit pas nulle, lisez :doc:`/reference/constraints/NotNull`.


+----------------+-------------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`                 |
+----------------+-------------------------------------------------------------------------+
| Options        | - `message`_                                                            |
+----------------+-------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\IsNull`             |
+----------------+-------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\IsNullValidator`    |
+----------------+-------------------------------------------------------------------------+

Utilisation de base
-------------------

Si vous voulez vous assurer que la propriété ``firstName`` d'une classe ``Author``
soit exactement égale à ``null``, ajoutez le code suivant :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - 'IsNull': ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\IsNull()
             */
            protected $firstName;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="IsNull" />
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
                $metadata->addPropertyConstraint('firstName', Assert\IsNull());
            }
        }

Options
-------

message
~~~~~~~

**type**: ``string`` **default**: ``This value should be null``

Le message qui sera affiché si la valeur n'est pas égale à ``null``.
