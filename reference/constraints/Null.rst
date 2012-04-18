Null
====

Valide qu'une valeur est exactement égale à ``null``. Pour vous assurer qu'une
propriété soit simplement vide (une chaine de caractères vide ou ``null``), lisez la
documentation de la contrainte :doc:`/reference/constraints/Blank`.
Pour vous assurer qu'une propriété ne soit pas nulle, lisez :doc:`/reference/constraints/NotNull`.


+----------------+-----------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`               |
+----------------+-----------------------------------------------------------------------+
| Options        | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Null`             |
+----------------+-----------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\NullValidator`    |
+----------------+-----------------------------------------------------------------------+

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
                    - Null: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Null()
             */
            protected $firstName;
        }

Options
-------

message
~~~~~~~

**type**: ``string`` **default**: ``This value should be null``

Le message qui sera affiché si la valeur n'est pas égale à ``null``.
