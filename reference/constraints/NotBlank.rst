NotBlank
========

Valide qu'une valeur n'est pas vide, pas égale à une chaîne de caractères vide,
et pas égale à ``null``. Pour forcer une valeur à ne pas être simplement égale
à ``null``, lisez la documentation de la contrainte :doc:`/reference/constraints/NotNull`.

+----------------+------------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`                |
+----------------+------------------------------------------------------------------------+
| Options        | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\NotBlank`          |
+----------------+------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\NotBlankValidator` |
+----------------+------------------------------------------------------------------------+

Utilisation de base
-------------------

Si vous voulez vous assurer que la propriété ``firstName`` d'une classe ``Author``
ne soit pas vide, ajoutez le code suivant :

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - NotBlank: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             */
            protected $firstName;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="NotBlank" />
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
                $metadata->addPropertyConstraint('firstName', new Assert\NotBlank());
            }
        }

Options
-------

message
~~~~~~~

**type**: ``string`` **default**: ``This value should not be blank``

Le message qui sera affiché si la valeur est vide.