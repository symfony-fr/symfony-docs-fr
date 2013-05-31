Iban
====

.. versionadded:: 2.3
    C'est une nouvelle contrainte depuis la version 2.3.

Cette contrainte est utilisée pour s'assurer qu'un numéro de compte bancaire a le bon format
de numéro `International Bank Account Number (IBAN)`_. IBAN est un moyen convenu au niveau international
d'identifier les comptes bancaires à travers les frontières nationales avec un risque réduit de propager
des erreurs de transcription.

+----------------+-----------------------------------------------------------------------+
| S'applique à   | :ref:`property or method<validation-property-target>`                 |
+----------------+-----------------------------------------------------------------------+
| Options        | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Iban`             |
+----------------+-----------------------------------------------------------------------+
| Validateur      | :class:`Symfony\\Component\\Validator\\Constraints\\IbanValidator`   |
+----------------+-----------------------------------------------------------------------+

Utilisation de base
-------------------

Pour utiliser le validateur Iban, il suffit de l'appliquer à une propriété d'un objet qui
contiendra un numéro de compte bancaire international.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/SubscriptionBundle/Resources/config/validation.yml
        Acme\SubscriptionBundle\Entity\Transaction:
            properties:
                bankAccountNumber:
                    - Iban:
                        message: Il ne s'agit pas d'un numéro de compte bancaire valide (IBAN).

    .. code-block:: xml

        <!-- src/Acme/SubscriptionBundle/Resources/config/validation.xml -->
        <class name="Acme\SubscriptionBundle\Entity\Transaction">
            <property name="bankAccountNumber">
                <constraint name="Iban">
                    <option name="message">Il ne s'agit pas d'un numéro de compte bancaire valide (IBAN).</option>
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
             * @Assert\Iban(message = "Il ne s'agit pas d'un numéro de compte bancaire valide (IBAN)")
             */
            protected $bankAccountNumber;
        }

    .. code-block:: php

        // src/Acme/SubscriptionBundle/Entity/Transaction.php
        namespace Acme\SubscriptionBundle\Entity\Transaction;
        
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Transaction
        {
            protected $bankAccountNumber;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('bankAccountNumber', new Assert\Iban(array(
                    'message' => 'Il ne s'agit pas d'un numéro de compte bancaire valide (IBAN).',
                )));
            }
        }

Options
-------

message
~~~~~~~

**type**: ``string`` **default**: ``This is not a valid International Bank Account Number (IBAN).``

Le message par défaut fourni lorsque la valeur ne passe pas le contrôle Iban.

.. _`International Bank Account Number (IBAN)`: http://en.wikipedia.org/wiki/International_Bank_Account_Number
