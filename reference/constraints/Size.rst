Size
====

Valide qu'un nombre donné se situe *entre* un nombre minimum et un nombre maximum.

+----------------+--------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`            |
+----------------+--------------------------------------------------------------------+
| Options        | - `min`_                                                           |
|                | - `max`_                                                           |
|                | - `minMessage`_                                                    |
|                | - `maxMessage`_                                                    |
|                | - `invalidMessage`_                                                |
+----------------+--------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Size`          |
+----------------+--------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\SizeValidator` |
+----------------+--------------------------------------------------------------------+

Utilisation de base
-------------------

Pour vérifier qu'un champ « hauteur » (« height » en anglais) se situe entre « 120 » et
« 180 », vous pouvez procéder comme suit :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                height:
                    - Size:
                        min: 120
                        max: 180
                        minMessage: Vous devez mesurer au moins 120cm pour entrer
                        maxMessage: Vous ne devez pas mesurer plus de 180cm pour entrer

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Size(
             *      min = "120",
             *      max = "180",
             *      minMessage = "Vous devez mesurer au moins 120cm pour entrer",
             *      maxMessage="Vous ne devez pas mesurer plus de 180cm pour entrer"
             * )
             */
             protected $height;
        }

Options
-------

min
~~~

**type**: ``integer`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire est la valeur « minimale ». La validation échouera
si la donnée saisie est **plus petite** que cette valeur minimale.

max
~~~

**type**: ``integer`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire est la valeur « maximale ». La validation échouera
si la donnée saisie est **plus grande** que cette valeur maximale.

minMessage
~~~~~~~~~~

**type**: ``string`` **default**: ``This value should be {{ limit }} or more.``

Le message qui sera affiché si la donnée saisie est inférieure à l'option `min`_.

maxMessage
~~~~~~~~~~

**type**: ``string`` **default**: ``This value should be {{ limit }} or less.``

Le message qui sera affiché si la donnée saisie est supérieure à l'option `max`_.

invalidMessage
~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``This value should be a valid number.``

Le message qui sera affiché si la donnée saisie n'est pas un nombre (pour
la fonction PHP `is_numeric`_).

.. _`is_numeric`: http://www.php.net/manual/fr/function.is-numeric.php