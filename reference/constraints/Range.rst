Range
=====

Valide qu'un nombre donné se situe *entre* un nombre minimum et un nombre maximum.


+----------------+---------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`             |
+----------------+---------------------------------------------------------------------+
| Options        | - `min`_                                                            |
|                | - `max`_                                                            |
|                | - `minMessage`_                                                     |
|                | - `maxMessage`_                                                     |
|                | - `invalidMessage`_                                                 |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Range`          |
+----------------+---------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\RangeValidator` |
+----------------+---------------------------------------------------------------------+

Utilisation de base
-------------------

Pour vérifier que le champ « height » d'une classe se situe entre « 120 » et « 180 »,
vous pouvez procéder comme suit :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                height:
                    - Range:
                        min: 120
                        max: 180
                        minMessage: Vous devez faire au moins 120cm pour entrer
                        maxMessage: Vous ne devez pas dépasser 180cm

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Range(
             *      min = 120,
             *      max = 180,
             *      minMessage = "Vous devez faire au moins 120cm pour entrer",
             *      maxMessage = "Vous ne devez pas dépasser 180cm"
             * )
             */
             protected $height;
        }

    .. code-block:: xml

        <!-- src/Acme/EventBundle/Resources/config/validation.xml -->
        <class name="Acme\EventBundle\Entity\Participant">
            <property name="height">
                <constraint name="Range">
                    <option name="min">120</option>
                    <option name="max">180</option>
                    <option name="minMessage">Vous devez faire au moins 120cm pour entrer</option>
                    <option name="maxMessage">Vous ne devez pas dépasser 180cm</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('height', new Assert\Range(array(
                    'min'        => 120,
                    'max'        => 180,
                    'minMessage' => 'Vous devez faire au moins 120cm pour entrer',
                    'maxMessage' => 'Vous ne devez pas dépasser 180cm',
                )));
            }
        }
Options
-------

min
~~~

**type**: ``integer`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire est la valeur « minimale ». La validation échouera
si cette donnée saisie est **inférieure** à cette valeur minimale.

max
~~~

**type**: ``integer`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire est la valeur « maximale ». La validation échouera
si cette donnée saisie est **supérieure** à cette valeur maximale.

minMessage
~~~~~~~~~~

**type**: ``string`` **default**: ``This value should be {{ limit }} or more.``

Le message qui sera affiché si la valeur sous-jacente est inférieure à l'option `min`_.

maxMessage
~~~~~~~~~~

**type**: ``string`` **default**: ``This value should be {{ limit }} or less.``

Le message qui sera affiché si la valeur sous-jacente est supérieure à l'option `max`_.

invalidMessage
~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``This value should be a valid number.``

Le message qui sera affiché si la valeur sous-jacente n'est pas un nombre
(selon la fonction PHP `is_numeric`_).

.. _`is_numeric`: http://www.php.net/manual/fr/function.is-numeric.php
