Date
====

Valide qu'une valeur est une date valide, c'est-à-dire soit un objet ``DateTime``,
soit une chaine de caractères (ou un objet qui peut être converti en chaine de caractères)
qui respecte un format valide YYYY-MM-DD.


+----------------+--------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`            |
+----------------+--------------------------------------------------------------------+
| Options        | - `message`_                                                       |
+----------------+--------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Date`          |
+----------------+--------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\DateValidator` |
+----------------+--------------------------------------------------------------------+

Utilisation de base
-------------------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                birthday:
                    - Date: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Date()
             */
             protected $birthday;
        }

Options
-------

message
~~~~~~~

**type**: ``string`` **default**: ``This value is not a valid date``

Ce message s'affiche si la donnée finale n'est pas une date valide.
