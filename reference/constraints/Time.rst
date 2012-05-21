Time
====

Valide qu'une valeur est une heure valide, c'est-à-dire soit un objet ``DateTime``,
soit une chaine de caractères (ou un objet converti en chaine de caractères) qui 
respecte un format « HH:MM:SS » valide.

+----------------+------------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`                |
+----------------+------------------------------------------------------------------------+
| Options        | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Time`              |
+----------------+------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\TimeValidator`     |
+----------------+------------------------------------------------------------------------+

Utilisation de base
-------------------

Supposons que vous avez une classe Event, avec un champ ``startAt`` qui est
l'heure à laquelle l'évènement commence :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Event:
            properties:
                startsAt:
                    - Time: ~

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Event.php
        namespace Acme\EventBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Event
        {
            /**
             * @Assert\Time()
             */
             protected $startsAt;
        }

Options
-------

message
~~~~~~~

**type**: ``string`` **default**: ``This value is not a valid time``

Le message qui est affiché si la donnée n'est pas une heure valide.