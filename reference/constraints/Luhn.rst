Luhn
====

.. versionadded:: 2.2
    La validation Luhn validation est nouvelle depuis Symfony 2.2.

Cette contrainte est utilisée pour s'assurer qu'un numéro de carte de crédit
vérifie l'`algorithme Luhn`_. Elle est utile pour la première étape de la validation
d'une carte de crédit : avant de communiquer avec la plateforme de paiement.

+----------------+-----------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`               |
+----------------+-----------------------------------------------------------------------+
| Options        | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Luhn`             |
+----------------+-----------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\LuhnValidator`    |
+----------------+-----------------------------------------------------------------------+

Utilisation de base
-------------------

Pour utiliser le validateur Luhn, appliquez le simplement à une propriété
d'un objet qui contient un numéro de carte de crédit.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/SubscriptionBundle/Resources/config/validation.yml
        Acme\SubscriptionBundle\Entity\Transaction:
            properties:
                cardNumber:
                    - Luhn:
                        message: Veuillez vérifier votre numéro de carte de crédit.

    .. code-block:: xml

        <!-- src/Acme/SubscriptionBundle/Resources/config/validation.xml -->
        <class name="Acme\SubscriptionBundle\Entity\Transaction">
            <property name="cardNumber">
                <constraint name="Luhn">
                    <option name="message">Veuillez vérifier votre numéro de carte de crédit.</option>
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
             * @Assert\Luhn(message = "Veuillez vérifier votre numéro de carte de crédit.")
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
                $metadata->addPropertyConstraint('cardNumber', new Assert\Luhn(array(
                    'message' => 'Veuillez vérifier votre numéro de carte de crédit',
                )));
            }
        }

Options disponibles
-------------------

message
~~~~~~~

**type**: ``string`` **default**: ``Invalid card number``

Le message par défaut qui est affiché si la valeur ne vérifie pas la
validation Luhn.

.. _`algorithme Luhn`: http://fr.wikipedia.org/wiki/Formule_de_Luhn