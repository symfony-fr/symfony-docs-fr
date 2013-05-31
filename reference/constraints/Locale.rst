Locale
======

Valide que la valeur est une locale valide.

La « valeur » de chaque locale est soit le code *langue* ISO639-1 en deux lettres
(ex ``fr``), soit le code langue suivi d'un tiret du bas (``_``) puis du code *pays*
ISO3166 (ex ``fr_FR`` pour Français/France).

+----------------+------------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`                |
+----------------+------------------------------------------------------------------------+
| Options        | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Locale`            |
+----------------+------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\LocaleValidator`   |
+----------------+------------------------------------------------------------------------+

Utilisation de base
-------------------

.. configuration-block::

    .. code-block:: yaml

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\User:
            properties:
                locale:
                    - Locale:

    .. code-block:: php-annotations

        // src/Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class User
        {
            /**
             * @Assert\Locale
             */
             protected $locale;
        }

    .. code-block:: xml

        <!-- src/Acme/UserBundle/Resources/config/validation.xml -->
        <class name="Acme\UserBundle\Entity\User">
            <property name="locale">
                <constraint name="Locale" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class User
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('locale', new Assert\Locale());
            }
        }

Options
-------

message
~~~~~~~

**type**: ``string`` **default**: ``This value is not a valid locale``

Ce message s'affiche si la valeur n'est pas une locale valide.