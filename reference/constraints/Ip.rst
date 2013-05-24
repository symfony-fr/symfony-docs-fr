Ip
==

Valide que la valeur est une adresse IP valide. Par défaut, cela validera la valeur
comme IPv4, mais différentes options existent pour valider une IPv6 et plusieurs
autres combinaisons.

+----------------+---------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`             |
+----------------+---------------------------------------------------------------------+
| Options        | - `version`_                                                        |
|                | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Ip`             |
+----------------+---------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\IpValidator`    |
+----------------+---------------------------------------------------------------------+

Utilisation de base
-------------------

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                ipAddress:
                    - Ip:

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Ip
             */
             protected $ipAddress;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="ipAddress">
                <constraint name="Ip" />
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
                $metadata->addPropertyConstraint('ipAddress', new Assert\Ip());
            }
        }

Options
-------

version
~~~~~~~

**type**: ``string`` **default**: ``4``

Cette option détermine *comment* l'adresse IP est validée et elle peut prendre l'une
de ces différentes valeurs :

**Toutes les plages**

* ``4`` - Valide une adresse IPv4
* ``6`` - Valide une adresse IPv6
* ``all`` - Valide tous les formats d'IP

**Pas de plage d'adresses privées**

* ``4_no_priv`` - Valide une adresse IPv4 mais sans plage d'adresses privées
* ``6_no_priv`` - Valide une adresse IPv6 mais sans plage d'adresses privées
* ``all_no_priv`` - Valide tous les formats d'IP mais sans plage d'adresses privées

**Pas de plage d'adresses réservées**

* ``4_no_res`` - Valide une adresse IPv4 mais sans plage d'adresses réservées
* ``6_no_res`` - Valide une adresse IPv6 mais sans plage d'adresses réservées
* ``all_no_res`` - Valide tous les formats d'IP mais sans plage d'adresses réservées

**Plage d'adresses publiques seulement**

* ``4_public`` - Valide une adresse IPv4 mais sans plage d'adresses privées ou réservées
* ``6_public`` - Valide une adresse IPv6 mais sans plage d'adresses privées ou réservées
* ``all_public`` - Valide tous les formats d'IP mais sans plage d'adresses privées ou réservées

message
~~~~~~~

**type**: ``string`` **default**: ``This is not a valid IP address``

Ce message s'affiche si la chaîne de caractères n'est pas une adresse IP valide.