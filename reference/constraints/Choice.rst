Choice
======

Cette contrainte est utilisée pour s'assurer qu'une valeur donnée fait partie
d'un ensemble de choix *valides*. Elle peut aussi être utilisée pour valider
que chaque item d'un tableau d'items est l'un des choix valides.

+----------------+-----------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`               |
+----------------+-----------------------------------------------------------------------+
| Options        | - `choices`_                                                          |
|                | - `callback`_                                                         |
|                | - `multiple`_                                                         |
|                | - `min`_                                                              |
|                | - `max`_                                                              |
|                | - `message`_                                                          |
|                | - `multipleMessage`_                                                  |
|                | - `minMessage`_                                                       |
|                | - `maxMessage`_                                                       |
|                | - `strict`_                                                           |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Choice`           |
+----------------+-----------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\ChoiceValidator`  |
+----------------+-----------------------------------------------------------------------+

Utilisation de base
-------------------

L'idée principale de cette contrainte est que vous fournissez un tableau de valeurs
valides (cela peut être fait de différentes manières) et elle valide que la
valeur d'une propriété données est bien dans ce tableau.

Si votre liste de choix valides est simple, vous pouvez la passer directement
via l'option `choices`_ :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice:
                        choices:  [homme, femme]
                        message:  Choisissez un genre valide.

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(choices = {"homme", "femme"}, message = "Choisissez un genre valide.")
             */
            protected $gender;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="gender">
                <constraint name="Choice">
                    <option name="choices">
                        <value>homme</value>
                        <value>femme</value>
                    </option>
                    <option name="message">Choisissez un genre valide.</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/EntityAuthor.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            protected $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Assert\Choice(array(
                    'choices' => array('homme', 'femme'),
                    'message' => 'Choisissez un genre valide.',
                )));
            }
        }
Fournir les choix par une fonction callback
-------------------------------------------

Vous pouvez aussi utiliser une fonction callback pour spécifier vos options. C'est
utile si vous voulez garder vos choix dans un endroit centralisé pour, par exemple,
avoir accès facilement à ces choix pour la validation ou pour construire des listes
déroulantes pour les formulaires.

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    class Author
    {
        public static function getGenders()
        {
            return array('homme', 'femme');
        }
    }

Vous pouvez passer le nom de cette méthode à l'option `callback_` de la contrainte
``Choice``.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: { callback: getGenders }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(callback = "getGenders")
             */
            protected $gender;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="gender">
                <constraint name="Choice">
                    <option name="callback">getGenders</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/EntityAuthor.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            protected $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Assert\Choice(array(
                    'callback' => 'getGenders',
                )));
            }
        }

Si le callback statique est stocké dans une classe différente, par exemple
``Util``, vous pouvez passer le nom de la classe et la méthode dans un tableau.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: { callback: [Util, getGenders] }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(callback = {"Util", "getGenders"})
             */
            protected $gender;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="gender">
                <constraint name="Choice">
                    <option name="callback">
                        <value>Util</value>
                        <value>getGenders</value>
                    </option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/EntityAuthor.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            protected $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Assert\Choice(array(
                    'callback' => array('Util', 'getGenders'),
                )));
            }
        }

Options disponibles
-------------------

choices
~~~~~~~

**type**: ``array`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire (à moins que `callback`_ soit spécifié)
représente le tableau d'options qui doit être considéré comme un
ensemble valide. La donnée soumise sera comparée à ce tableau.

callback
~~~~~~~~

**type**: ``string|array|Closure``

C'est la méthode callback qui peut être utilisée au lieu de l'option `choices`_
pour retourner le tableau de choix. Lisez `Fournir les choix par une fonction callback`_
pour plus d'informations sur son utilisation.

multiple
~~~~~~~~

**type**: ``Boolean`` **default**: ``false``

Si cette option est définie à true, la valeur soumise attendue est un tableau
et non plus une simple valeur. La contrainte vérifiera que chaque valeur du tableau
soumis se trouve dans le tableau de choix valides. Si une seule des valeurs soumises
n'est pas trouvée, la validation échouera.

min
~~~

**type**: ``integer``

Si l'option ``multiple`` est à true, alors vous pouvez utiliser l'option ``min``
pour forcer qu'au moins XX valeurs doivent être sélectionnées. Par exemple,
si ``min`` vaut 3 et si le tableau soumis contient 2 items valides, la validation
échouera.

max
~~~

**type**: ``integer``

Si l'option ``multiple`` est à true, alors vous pouvez utiliser l'option ``max``
pour forcer que XX valeurs peuvent être sélectionnées au maximum. Par exemple,
si ``max`` vaut 3 et si le tableau soumis contient 4 items valides, la validation
échouera.

message
~~~~~~~

**type**: ``string`` **default**: ``The value you selected is not a valid choice``

C'est le message que vous verrez si l'option ``multiple`` est définie à ``false``,
et que la valeur soumise n'est pas dans le tableau de choix valides.

multipleMessage
~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``One or more of the given values is invalid``

C'est le message que vous verrez si l'option ``multiple`` est définie à ``true``,
et que l'une des valeurs du tableau soumis n'est pas dans le tableau de valeurs
valides.

minMessage
~~~~~~~~~~

**type**: ``string`` **default**: ``You must select at least {{ limit }} choices``

C'est le message d'erreur qui est affiché quand l'utilisateur choisit trop peu de choix
(en fonction de l'option `min`_).

maxMessage
~~~~~~~~~~

**type**: ``string`` **default**: ``You must select at most {{ limit }} choices``

C'est le message d'erreur qui est affiché quand l'utilisateur choisit trop de choix
(en fonction de l'option `max`_).

strict
~~~~~~

**type**: ``Boolean`` **default**: ``false``

Si cette option est à true, le validateur vérifiera également le type de la donnée soumise.
Spécifiquement, cette valeur est passée comme troisième argument de la méthode PHP :phpfunction:`in_array`
lorsque vous vérifiez qu'une valeur est bien dans le tableau de choix valides.