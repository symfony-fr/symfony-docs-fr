UserPassword
============

.. versionadded:: 2.1
   Cette contrainte n'existe que depuis la version 2.1.

Cette contrainte valide qu'une valeur saisie est égale au mot de passe
de l'utilisateur courant. C'est utile dans un formulaire où l'utilisateur
peut changer son mot de passe mais doit d'abord entrer son ancien mot de
passe pour des raisons de sécurité.

.. note::

    Elle ne devrait *pas* être utilisée dans un formulaire de connexion,
    puisque cela est géré automatiquement par le système de sécurité.

Lorsqu'elle est appliquée à un tableau (ou à un objet de type Traversable), cette
contrainte vous permet d'appliquer une collection de contraintes à chaque élément du tableau.

+----------------+-------------------------------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`                                   |
+----------------+-------------------------------------------------------------------------------------------+
| Options        | - `message`_                                                                              |
+----------------+-------------------------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Security\\Core\\Validator\\Constraint\\UserPassword`          |
+----------------+-------------------------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Security\\Core\\Validator\\Constraint\\UserPasswordValidator` |
+----------------+-------------------------------------------------------------------------------------------+

Utilisation de base
-------------------

Supposons que vous ayez une classe `PasswordChange` qui est utilisée dans un
formulaire où l'utilisateur peut changer son mot de passe en entrant son
ancien mot de passe et son nouveau mot de passe. Cette contrainte validera
que l'ancien mot de passe est correct :

.. configuration-block::

    .. code-block:: yaml

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Form\Model\ChangePassword:
            properties:
                oldPassword:
                    - Symfony\Component\Security\Core\Validator\Constraint\UserPassword:
                        message: "Votre mot de passe actuel est erroné"

    .. code-block:: php-annotations

       // src/Acme/UserBundle/Form/Model/ChangePassword.php
       namespace Acme\UserBundle\Form\Model;
       
       use Symfony\Component\Security\Core\Validator\Constraint as SecurityAssert;

       class ChangePassword
       {
           /**
            * @SecurityAssert\UserPassword(
            *     message = "Votre mot de passe actuel est erroné"
            * )
            */
            protected $oldPassword;
       }

Options
-------

message
~~~~~~~

**type**: ``message`` **default**: ``This value should be the user current password``

Le message qui sera affiché si la chaîne de caractères ne correspond *pas*
au mot de passe actuel de l'utilisateur.
