Language
========

Valide que la valeur est un code de langue valide.

+----------------+------------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`                |
+----------------+------------------------------------------------------------------------+
| Options        | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Language`          |
+----------------+------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\LanguageValidator` |
+----------------+------------------------------------------------------------------------+

Utilisation de base
-------------------

.. configuration-block::

    .. code-block:: yaml

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\User:
            properties:
                preferredLanguage:
                    - Language:

    .. code-block:: php-annotations

       // src/Acme/UserBundle/Entity/User.php
       namespace Acme\UserBundle\Entity;
       
       use Symfony\Component\Validator\Constraints as Assert;

       class User
       {
           /**
            * @Assert\Language
            */
            protected $preferredLanguage;
       }

Options
-------

message
~~~~~~~

**type**: ``string`` **default**: ``This value is not a valid language``

Le message s'affiche si la chaine de caractères n'est pas un code de langue valide.
