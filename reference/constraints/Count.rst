Count
=====

Valide que le nombre d'éléments d'une collection (c-a-d un tableau ou un objet qui implémente
Countable) se situe *entre* une valeur minimum et une valeur maximum.

+----------------+---------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`             |
+----------------+---------------------------------------------------------------------+
| Options        | - `min`_                                                            |
|                | - `max`_                                                            |
|                | - `minMessage`_                                                     |
|                | - `maxMessage`_                                                     |
|                | - `exactMessage`_                                                   |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Count`          |
+----------------+---------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\CountValidator` |
+----------------+---------------------------------------------------------------------+

Utilisation de base
-------------------

Pour vérifier que le champ ``emails`` contient entre 1 et 5 éléments, vous
pouvez procéder comme suit :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                emails:
                    - Count:
                        min: 1
                        max: 5
                        minMessage: "Vous devez spécifier au moins un email"
                        maxMessage: "Vous ne pouvez pas spécifier plus de {{ limit }} emails"

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Count(
             *      min = "1",
             *      max = "5",
             *      minMessage = "Vous devez spécifier au moins un email",
             *      maxMessage = "Vous ne pouvez pas spécifier plus de {{ limit }} emails"
             * )
             */
             protected $emails = array();
        }

    .. code-block:: xml

        <!-- src/Acme/EventBundle/Resources/config/validation.xml -->
        <class name="Acme\EventBundle\Entity\Participant">
            <property name="emails">
                <constraint name="Count">
                    <option name="min">1</option>
                    <option name="max">5</option>
                    <option name="minMessage">Vous devez spécifier au moins un email</option>
                    <option name="maxMessage">Vous ne pouvez pas spécifier plus de {{ limit }} emails</option>
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
            public static function loadValidatorMetadata(ClassMetadata $data)
            {
                $metadata->addPropertyConstraint('emails', new Assert\Count(array(
                    'min'        => 1,
                    'max'        => 5,
                    'minMessage' => 'Vous devez spécifier au moins un email',
                    'maxMessage' => 'Vous ne pouvez pas spécifier plus de {{ limit }} emails',
                )));
            }
        }

Options
-------

min
~~~

**type**: ``integer`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire est la valeur du nombre « minimal ». La validation échouera
si le nombre d'éléments de la collection est **plus petit** que cette valeur minimale.

max
~~~

**type**: ``integer`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire est la valeur du nombre « maximal ». La validation échouera
si le nombre d'éléments de la collection est **plus grand** que cette valeur maximale.

minMessage
~~~~~~~~~~

**type**: ``string`` **default**: ``This collection should contain {{ limit }} elements or more.``.

Le message qui sera affiché si le nombre d'éléments de la collection est inférieur à l'option `min`_.

maxMessage
~~~~~~~~~~

**type**: ``string`` **default**: ``This collection should contain {{ limit }} elements or less.``.

Le message qui sera affiché si le nombre d'éléments de la collection est supérieur à l'option `max`_.

exactMessage
~~~~~~~~~~~~

**type**: ``string`` **default**: ``This collection should contain exactly {{ limit }} elements.``.

Le message qui sera affiché si les valeurs min et max sont égales, et que le nombre d'éléments
de la collection n'est pas exactement cette valeur.
