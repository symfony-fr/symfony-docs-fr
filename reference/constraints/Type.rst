Type
====

Valide qu'une valeur est d'un type spécifique. Par exemple, si une variable doit
être un tableau, vous pouvez utiliser cette contrainte avec l'option type définie
comme ``array`` pour la valider.

+----------------+---------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`             |
+----------------+---------------------------------------------------------------------+
| Options        | - :ref:`type<reference-constraint-type-type>`                       |
|                | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Type`           |
+----------------+---------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\TypeValidator`  |
+----------------+---------------------------------------------------------------------+

Utilisation de base
-------------------

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                age:
                    - Type:
                        type: integer
                        message: La valeur {{ value }} n'est pas un type {{ type }} valide.

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Type(type="integer", message="La valeur {{ value }} n'est pas un type {{ type }} valide.")
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="age">
                <constraint name="Type">
                    <option name="type">integer</option>
                    <option name="message">La valeur {{ value }} n'est pas un type {{ type }} valide.</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php
        
        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('age', new Assert\Type(array(
                    'type'    => 'integer',
                    'message' => "La valeur {{ value }} n'est pas un type {{ type }} valide.",
                )));
            }
        }
        
Options
-------

.. _reference-constraint-type-type:

type
~~~~

**type**: ``string`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire est le nom de classe de l'un des types de données PHP
déterminés par les fonctions PHP ``is_``.

  * `array <http://php.net/is_array>`_
  * `bool <http://php.net/is_bool>`_
  * `callable <http://php.net/is_callable>`_
  * `float <http://php.net/is_float>`_ 
  * `double <http://php.net/is_double>`_
  * `int <http://php.net/is_int>`_ 
  * `integer <http://php.net/is_integer>`_
  * `long <http://php.net/is_long>`_
  * `null <http://php.net/is_null>`_
  * `numeric <http://php.net/is_numeric>`_
  * `object <http://php.net/is_object>`_
  * `real <http://php.net/is_real>`_
  * `resource <http://php.net/is_resource>`_
  * `scalar <http://php.net/is_scalar>`_
  * `string <http://php.net/is_string>`_


Il est également possible d'utiliser le nom de l'une des fonctions de `l'extension
PHP <http://php.net/book.ctype.php>`_ ``ctype_``.
Voici `la liste des fonctions ctype <http://php.net/ref.ctype.php>`_:

  * `alnum <http://php.net/function.ctype-alnum.php>`_
  * `alpha <http://php.net/function.ctype-alpha.php>`_
  * `cntrl <http://php.net/function.ctype-cntrl.php>`_
  * `digit <http://php.net/function.ctype-digit.php>`_
  * `graph <http://php.net/function.ctype-graph.php>`_
  * `lower <http://php.net/function.ctype-lower.php>`_
  * `print <http://php.net/function.ctype-print.php>`_
  * `punct <http://php.net/function.ctype-punct.php>`_
  * `space <http://php.net/function.ctype-space.php>`_
  * `upper <http://php.net/function.ctype-upper.php>`_
  * `xdigit <http://php.net/function.ctype-xdigit.php>`_

Pour cela, assurez-vous que le paramètre `locale <http://php.net/function.setlocale.php>`_ 
est bien défini avant d'utiliser une de ces fonctions.

message
~~~~~~~

**type**: ``string`` **default**: ``This value should be of type {{ type }}``

Le message qui sera affiché si la donnée n'est pas du bon type.