NotNull
=======

Valide qu'une valeur n'est strictement pas égale à ``null``. Pour vous assurer qu'une
valeur ne soit simplement pas vide (pas une chaîne de caractères vide), lisez la
documentation de la contrainte :doc:`/reference/constraints/NotBlank`.

+----------------+-----------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`               |
+----------------+-----------------------------------------------------------------------+
| Options        | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\NotNull`          |
+----------------+-----------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\NotNullValidator` |
+----------------+-----------------------------------------------------------------------+

Utilisation de base
-------------------

Si vous voulez vous assurer que la propriété ``firstName`` d'une classe ``Author``
ne soit strictement pas égale à ``null``, ajoutez le code suivant :

.. configuration-block::

    .. code-block:: yaml

        properties:
            firstName:
                - NotNull: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotNull()
             */
            protected $firstName;
        }

Options
-------

message
~~~~~~~

**type**: ``string`` **default**: ``This value should not be null``

Le message qui sera affiché si la valeur est égale à ``null``.