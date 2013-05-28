CardScheme
==========

.. versionadded:: 2.2
    La validation CardScheme validation est nouvelle en Symfony 2.2.

Cette contrainte s'assure qu'un numéro de carte de crédit est valide pour une banque
donnée. Cela peut être utilisé pour valider un numéro avant de lancer une transaction
vers une passerelle de paiement.

+----------------+--------------------------------------------------------------------------+
| S'applique à   | :ref:`property or method<validation-property-target>`                    |
+----------------+--------------------------------------------------------------------------+
| Options        | - `schemes`_                                                             |
|                | - `message`_                                                             |
+----------------+--------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\CardScheme`          |
+----------------+--------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\CardSchemeValidator` |
+----------------+--------------------------------------------------------------------------+

Utilisation de base
-------------------

Pour utiliser le validateur ``CardScheme``, appliquez le simplement
à une méthode ou une propriété d'un objet qui contient un numéro de
carte de crédit.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/SubscriptionBundle/Resources/config/validation.yml
        Acme\SubscriptionBundle\Entity\Transaction:
            properties:
                cardNumber:
                    - CardScheme:
                        schemes: [VISA]
                        message: Votre numéro de carte n'est pas valide.

    .. code-block:: xml

        <!-- src/Acme/SubscriptionBundle/Resources/config/validation.xml -->
        <class name="Acme\SubscriptionBundle\Entity\Transaction">
            <property name="cardNumber">
                <constraint name="CardScheme">
                    <option name="schemes">
                        <value>VISA</value>
                    </option>
                    <option name="message">Votre numéro de carte n'est pas valide.</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/SubscriptionBundle/Entity/Transaction.php
        namespace Acme\SubscriptionBundle\Entity\Transaction;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Transaction
        {
            /**
             * @Assert\CardScheme(schemes = {"VISA"}, message = "Votre numéro de carte n'est pas valide.")
             */
            protected $cardNumber;
        }

    .. code-block:: php

        // src/Acme/SubscriptionBundle/Entity/Transaction.php
        namespace Acme\SubscriptionBundle\Entity\Transaction;
        
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Transaction
        {
            protected $cardNumber;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('cardNumber', new Assert\CardScheme(array(
                    'schemes' => array(
                        'VISA'
                    ),
                    'message' => 'Votre numéro de carte n'est pas valide',
                )));
            }
        }

Options
-------

schemes
-------

**type**: ``mixed`` [:ref:`option par défaut<validation-default-option>`]

Cette option est requise. Elle représente le nom du schéma de nombre utilisé
pour valider le numéro de carte de crédit. Ce peut être une chaine ou un tableau.
Les options valides sont :

* ``AMEX``
* ``CHINA_UNIONPAY``
* ``DINERS``
* ``DISCOVER``
* ``INSTAPAYMENT``
* ``JCB``
* ``LASER``
* ``MAESTRO``
* ``MASTERCARD``
* ``VISA``

Pour plus d'informations sur les schémas utilisés, consultez `Wikipedia: Issuer identification number (IIN)`_.

message
~~~~~~~

**type**: ``string`` **default**: ``Unsupported card type or invalid card number``

Le message qui sera affiché quand la valeur ne passe pas ``CardScheme``.

.. _`Wikipedia: Issuer identification number (IIN)`: http://en.wikipedia.org/wiki/Bank_card_number#Issuer_identification_number_.28IIN.29
