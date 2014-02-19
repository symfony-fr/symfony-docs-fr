Devise
======

.. versionadded:: 2.3
    Cette contrainte est une nouveauté de la version 2.3.

Valide que la valeur est un `code ISO 4217`_ de devise.

+----------------+---------------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`                   |
+----------------+---------------------------------------------------------------------------+
| Options        | - `message`_                                                              |
+----------------+---------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Currency`             |
+----------------+---------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\CurrencyValidator`    |
+----------------+---------------------------------------------------------------------------+

Utilisation basique
-------------------

Si vous voulez vous assurer que la propriété ``currency`` (devise) d'un objet
``Order`` (commande) est une devise valide, vous pouvez procéder comme ceci :

.. configuration-block::

    .. code-block:: yaml

        # src/EcommerceBundle/Resources/config/validation.yml
        Acme\EcommerceBundle\Entity\Order:
            properties:
                currency:
                    - Currency: ~

    .. code-block:: php-annotations

        // src/Acme/EcommerceBundle/Entity/Order.php
        namespace Acme\EcommerceBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Order
        {
            /**
             * @Assert\Currency
             */
            protected $currency;
        }

    .. code-block:: xml

        <!-- src/Acme/EcommerceBundle/Resources/config/validation.xml -->
        <class name="Acme\EcommerceBundle\Entity\Order">
            <property name="currency">
                <constraint name="Currency" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/EcommerceBundle/Entity/Order.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Order
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('currency', new Assert\Currency());
            }
        }

Options
-------

message
~~~~~~~

**type**: ``string`` **default**: ``This value is not a valid currency.``

C'est le message qui sera affiché si la valeur n'est pas une devise.

.. _`code ISO 4217`: http://fr.wikipedia.org/wiki/ISO_4217 