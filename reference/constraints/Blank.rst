Blank
=====

Valide qu'une valeur est vide, égale à une chaîne vide, ou égale à ``null``.
Pour forcer cette valeur à être strictement égale à ``null``, jetez un oeil à
la contrainte :doc:`/reference/constraints/Null`. Pour forcer cette valeur à
ne *pas* être vide, lisez :doc:`/reference/constraints/NotBlank`.

+----------------+-----------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`               |
+----------------+-----------------------------------------------------------------------+
| Options        | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Blank`            |
+----------------+-----------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\BlankValidator`   |
+----------------+-----------------------------------------------------------------------+

Utilisation de base
-------------------

Si, pour certaines raisons, vous voulez vous assurer que la la propriété ``firstName``
de la classe ``Author`` soit nulle, vous pouvez procéder comme ceci :

.. configuration-block::

    .. code-block:: yaml

        properties:
            firstName:
                - Blank: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Blank()
             */
            protected $firstName;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="Blank" />
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
                $metadata->addPropertyConstraint('firstName', new Assert\Blank());
            }
        }

Options
-------

message
~~~~~~~

**type**: ``string`` **default**: ``This value should be blank``

Ce message s'affiche si la valeur n'est pas vide.