All
===

Quand elle est appliquée à un tableau (ou un objet Traversable), cette contrainte vous
permet d'appliquer un ensemble de contraintes à chaque élément du tableau.

+----------------+------------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`                |
+----------------+------------------------------------------------------------------------+
| Options        | - `constraints`_                                                       |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\All`               |
+----------------+------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\AllValidator`      |
+----------------+------------------------------------------------------------------------+

Utilisation de base
-------------------

Supposons que vous ayez un tableau de chaînes de caractères, et que vous
vouliez valider chaque entrée du tableau :

.. configuration-block::

    .. code-block:: yaml

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\User:
            properties:
                favoriteColors:
                    - All:
                        - NotBlank:  ~
                        - MinLength: 5

    .. code-block:: php-annotations

       // src/Acme/UserBundle/Entity/User.php
       namespace Acme\UserBundle\Entity;

       use Symfony\Component\Validator\Constraints as Assert;

       class User
       {
           /**
            * @Assert\All({
            *     @Assert\NotBlank
            *     @Assert\MinLength(5),
            * })
            */
            protected $favoriteColors = array();
       }

Maintenant, chaque entrée du tableau ``favoriteColors`` sera validée
pour ne pas être vide et faire au moins 5 caractères.

Options
-------

constraints
~~~~~~~~~~~

**type**: ``array`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire est le tableau de contraintes de validation que
vous voulez appliquer à chaque élément du tableau sous-jacent.