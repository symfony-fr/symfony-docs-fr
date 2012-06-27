Size
====

Valide que la longueur d'une chaîne de caractères donnée, ou le nombre d'éléments
d'une collection se situe *entre* une valeur minimum et une valeur maximum.

.. versionadded:: 2.1
    La contrainte Size a été ajoutée dans Symfony 2.1.

+----------------+--------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`            |
+----------------+--------------------------------------------------------------------+
| Options        | - `min`_                                                           |
|                | - `max`_                                                           |
|                | - `type`_                                                          |
|                | - `charset`_                                                       |
|                | - `minMessage`_                                                    |
|                | - `maxMessage`_                                                    |
|                | - `exactMessage`_                                                  |
+----------------+--------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Size`          |
+----------------+--------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\SizeValidator` |
+----------------+--------------------------------------------------------------------+

Utilisation de base
-------------------

Pour vérifier que la longueur du champ ``firstName`` d'une classe se situe entre
« 2 » et « 50 », vous pouvez procéder comme suit :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                firstName:
                    - Size:
                        min: 2
                        max: 50
                        minMessage: Votre nom doit faire au moins 2 caractères
                        maxMessage: Votre nom ne peut pas être plus long que 50 caractères

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Size(
             *      min = "2",
             *      max = "50",
             *      minMessage = "Votre nom doit faire au moins 2 caractères",
             *      maxMessage= "Votre nom ne peut pas être plus long que 50 caractères"
             * )
             */
             protected $firstName;
        }

Utilisation de base - Collections
---------------------------------

Pour vérifier que le champ ``emails`` contient entre 1 et 5 éléments, vous
pouvez procéder comme suit :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                emails:
                    - Size:
                        min: 1
                        max: 5
                        minMessage: Vous devez spécifier au moins un email
                        maxMessage: Vous ne pouvez pas spécifier plus de 5 emails

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Size(
             *      min = "1",
             *      max = "5",
             *      minMessage = "Vous devez spécifier au moins un email",
             *      maxMessage = "Vous ne pouvez pas spécifier plus de 5 emails"
             * )
             */
             protected $emails = array();
        }

Options
-------

min
~~~

**type**: ``integer`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire est la valeur de la longueur « minimale ». La validation échouera
si la longueur de la donnée saisie est **plus petite** que cette valeur minimale.

max
~~~

**type**: ``integer`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire est la valeur de la longueur « maximale ». La validation échouera
si la longueur de la donnée saisie est **plus grande** que cette valeur maximale.

type
~~~~

**type**: ``string``

Le type de donnée à valider. Cela peut être soit ``string``, soit ``collection``. Si rien
n'est spécifié, le validateur essaiera le bon type en se basant sur la donnée qui doit être
validée.

charset
~~~~~~~

**type**: ``string``  **default**: ``UTF-8``

Le charset qui sera utilisé pour calculer la longueur de la valeur. La fonction PHP
:phpfunction:`grapheme_strlen` est utilisée si elle est disponible. Sinon, la fonction PHP
:phpfunction:`mb_strlen` est utilisée si elle est disponible. Si aucune de ces deux fonctions
n'est disponible, la fonction :phpfunction:`strlen` sera utilisée.

minMessage
~~~~~~~~~~

**type**: ``string`` **default**: ``This value is too short. It should have {{ limit }} characters or more.`` lorsque vous validez une chaine de caractères, ou ``This collection should contain
{{ limit }} elements or more.`` lorsque vous validez une collection.

Le message qui sera affiché si la longueur de la valeur saisie ou le nombre d'éléments de la
collection est inférieur à l'option `min`_.

maxMessage
~~~~~~~~~~

**type**: ``string`` **default**: ``This value is too long. It should have {{ limit }} characters or less.`` lorsque vous validez une chaine de caractères, ou ``This collection should contain
{{ limit }} elements or less.`` lorsque vous validez une collection.

Le message qui sera affiché si la longueur de la valeur saisie ou le nombre d'éléments de la
collection est supérieur à l'option `max`_.


exactMessage
~~~~~~~~~~~~

**type**: ``string`` **default**: ``This value should have exactly {{ limit }} characters.`` lorsque
vous validez une chaine de caractères, ou ``This collection should contain exactly {{ limit }} elements.`` lorsque vous validez une collection.

Le message qui sera affiché si les valeurs min et max sont égales, et que la longueur
de la valeur soumise ou le nombre d'éléments de la collection n'est pas exactement cette valeur.
