Email
=====

Valide que la valeur est une adresse email valide. La donnée finale est convertie
en une chaîne de caractères avant d'être validée.

+----------------+---------------------------------------------------------------------+
| S'applique à   | :ref:`property or method<validation-property-target>`               |
+----------------+---------------------------------------------------------------------+
| Options        | - `message`_                                                        |
|                | - `checkMX`_                                                        |
|                | - `checkHost`_                                                      |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Email`          |
+----------------+---------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\EmailValidator` |
+----------------+---------------------------------------------------------------------+

Utilisation de base
-------------------

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                email:
                    - Email:
                        message: "{{ value }}" n'est pas un email valide.
                        checkMX: true

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="email">
                    <constraint name="Email">
                        <option name="message">L'email "{{ value }}" n'est pas un email valide.</option>
                        <option name="checkMX">true</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /** 
             * @Assert\Email(
             *     message = "'{{ value }}' n'est pas un email valide.",
             *     checkMX = true
             * )
             */
             protected $email;
        }

Options
-------

message
~~~~~~~

**type**: ``string`` **default**: ``This value is not a valid email address``

Ce message s'affiche si la donnée finale n'est pas une adresse email valide.

checkMX
~~~~~~~

**type**: ``Boolean`` **default**: ``false``

Si cette option est définie à true, alors la fonction PHP :phpfunction:`checkdnsrr`
sera utilisée pour vérifier la validité de l'enregistrement MX du serveur de l'email donné.

checkHost
~~~~~~~~~

.. versionadded:: 2.1
    L'option ``checkHost`` est une nouveauté de Symfony 2.1

**type**: ``Boolean`` **default**: ``false``
 
Si cette option est définie à true, alors la fonction PHP :phpfunction:`checkdnsrr`
sera utilisée pour vérifier la validité de l'enregistrement MX *ou* 1 *ou* AAAA du serveur
de l'email donné.
