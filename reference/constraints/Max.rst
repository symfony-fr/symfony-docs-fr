Max
===

Valide qu'un nombre donné est *inférieur* à un nombre maximum.

+----------------+--------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`            |
+----------------+--------------------------------------------------------------------+
| Options        | - `limit`_                                                         |
|                | - `message`_                                                       |
|                | - `invalidMessage`_                                                |
+----------------+--------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Max`           |
+----------------+--------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\MaxValidator`  |
+----------------+--------------------------------------------------------------------+

Utilisation de base
-------------------

Pour vérifier qu'un champ « âge » d'une classe n'est pas supérieur à « 50 »,
vous pourriez ajouter le code suivant :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                age:
                    - Max: { limit: 50, message: Vous devez avoir moins de 50 ans pour entrer. }

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Max(limit = 50, message = "Vous devez avoir moins de 50 ans pour entrer.")
             */
             protected $age;
        }

Options
-------

limit
~~~~~

**type**: ``integer`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire est la valeur « maximale ». La validation échouera
si la valeur soumise est **supérieure** à cette valeur maximale.

message
~~~~~~~

**type**: ``string`` **default**: ``This value should be {{ limit }} or less``

Le message qui sera affiché si la valeur est supérieur à l'option `limit`_.

invalidMessage
~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``This value should be a valid number``

Le message qui sera affiché si la valeur soumise n'est pas un nombre
(pour la fonction PHP `is_numeric`_).

.. _`is_numeric`: http://www.php.net/manual/fr/function.is-numeric.php