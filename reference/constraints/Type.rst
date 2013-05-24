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
                    'message' => 'La valeur {{ value }} n'est pas un type {{ type }} valide.',
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
  
message
~~~~~~~

**type**: ``string`` **default**: ``This value should be of type {{ type }}``

Le message qui sera affiché si la donnée n'est pas du bon type.