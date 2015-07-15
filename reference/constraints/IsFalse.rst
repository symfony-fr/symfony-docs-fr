IsFalse
=======

Valide que la valeur est ``false``. Spécifiquement, cette contrainte vérifie que la
valeur est exactement ``false``, exactement l'entier ``0``, ou exactement la chaîne
de caractères « ``0`` ».

Vous pouvez également voir :doc:`IsTrue <IsTrue>`.

+----------------+---------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`             |
+----------------+---------------------------------------------------------------------+
| Options        | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\IsFalse`          |
+----------------+---------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\IsFalseValidator` |
+----------------+---------------------------------------------------------------------+

Utilisation de base
-------------------

La contrainte ``IsFalse`` s'applique à une propriété ou à une méthode « getter » mais
elle est le plus souvent utilisée dans le dernier cas. Par exemple, supposons 
que vous vouliez garantir qu'une propriété ``state`` n'est *pas* dans un tableau
dynamique ``invalidStates``. Premièrement, vous créerez une méthode « getter »::

    protected $state;

    protected $invalidStates = array();

    public function isStateInvalid()
    {
        return in_array($this->state, $this->invalidStates);
    }

Dans ce cas, l'objet sous-jacent n'est valide que si la méthode ``isStateInvalid``
retourne **false** :

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author
            getters:
                stateInvalid:
                    - "IsFalse":
                        message: Vous avez saisi un état non valide.

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\IsFalse(
             *     message = "Vous avez saisi un état non valide."
             * )
             */
             public function isStateInvalid()
             {
                // ...
             }
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <getter property="stateInvalid">
                <constraint name="IsFalse">
                    <option name="message">Vous avez saisi un état non valide.</option>
                </constraint>
            </getter>
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
                $metadata->addGetterConstraint('stateInvalid', new Assert\IsFalse());
            }
        }

.. caution::

    Si vous utilisez YAML, assurez vous de bien mettre les guillemets autour de
    ``IsFalse`` (``"IsFalse"``), sinon YAML le convertira en Booléen.

Options
-------

message
~~~~~~~

**type**: ``string`` **default**: ``This value should be false``

Ce message s'affiche si la donnée n'est pas à ``False``.
