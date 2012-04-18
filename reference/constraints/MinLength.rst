MinLength
=========

Valide que la longueur d'une chaine de caractères est au moins supérieure à la
limite donnée.

+----------------+-------------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`                 |
+----------------+-------------------------------------------------------------------------+
| Options        | - `limit`_                                                              |
|                | - `message`_                                                            |
|                | - `charset`_                                                            |
+----------------+-------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\MinLength`          |
+----------------+-------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\MinLengthValidator` |
+----------------+-------------------------------------------------------------------------+

Utilisation de base
-------------------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Blog:
            properties:
                firstName:
                    - MinLength: { limit: 3, message: "Votre nom doit faire au moins {{ limit }} caractères." }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Blog.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Blog
        {
            /**
             * @Assert\MinLength(
             *     limit=3,
             *     message="Votre nom doit faire au moins {{ limit }} caractères."
             * )
             */
            protected $summary;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Blog">
            <property name="summary">
                <constraint name="MinLength">
                    <option name="limit">3</option>
                    <option name="message">Votre nom doit faire au moins {{ limit }} caractères.</option>
                </constraint>
            </property>
        </class>

Options
-------

limit
~~~~~

**type**: ``integer`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire est la valeur « minimale». La validation échouera
si la longueur de la chaine de caractères donnée est **inférieure** à ce
nombre.

message
~~~~~~~

**type**: ``string`` **default**: ``This value is too short. It should have {{ limit }} characters or more``

Le message qui sera affiché si la longueur de la chaine de caractères est
inférieure à l'option `limit`_.

charset
~~~~~~~

**type**: ``charset`` **default**: ``UTF-8``


Si l'extension PHP « mbstring » est installée, alors la fonction PHP `mb_strlen`_
sera utilisée pour calculer la longueur de la chaine. La valeur de l'option
``charset`` est passée comme second argument de cette fonction.

.. _`mb_strlen`: http://php.net/manual/fr/function.mb-strlen.php
