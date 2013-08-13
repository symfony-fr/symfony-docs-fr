Callback
========

Le but de l'assertion Callback est de vous permettre de créer entièrement des
règles de validation personnalisées et d'assigner des erreurs de validation à
des champs spécifiques de votre objet. Si vous utilisez la validation de formulaires,
cela signifie que vous pouvez faire en sorte que ces erreurs personnalisées s'affichent
à côté d'un champ spécifique plutôt qu'en haut de votre formulaire.

Cela fonctionne en spécifiant des méthodes *callback*, chacune d'elle étant appelée
durant le processus de validation. Chacune de ces méthodes peut faire toute
sorte de choses, incluant la création et l'assignation d'erreurs de validation.

.. note::
    
    Une méthode callback elle-même n'*échoue* pas et ne retourne aucune valeur.
    Au lieu de cela, comme vous le verrez dans l'exemple, cette méthode a la
    possibilité d'ajouter directement des « violations » de validateur.

+----------------+------------------------------------------------------------------------+
| S'applique à   | :ref:`classe<validation-class-target>`                                 |
+----------------+------------------------------------------------------------------------+
| Options        | - `methods`_                                                          |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Callback`          |
+----------------+------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\CallbackValidator` |
+----------------+------------------------------------------------------------------------+

Configuration
-------------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            constraints:
                - Callback:
                    methods:   [isAuthorValid]

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        /**
         * @Assert\Callback(methods={"isAuthorValid"})
         */
        class Author
        {
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <constraint name="Callback">
                <option name="methods">
                    <value>isAuthorValid</value>
                </option>
            </constraint>
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
                $metadata->addConstraint(new Assert\Callback(array(
                    'methods' => array('isAuthorValid'),
                )));
            }
        }

La méthode Callback
-------------------

Un objet spécial ``ExecutionContext`` est passé à la méthode callback. Vous
pouvez définir des « violations » directement sur cet objet et déterminer à
quel champ ces erreurs seront attribuées ::

    // ...
    use Symfony\Component\Validator\ExecutionContextInterface;

    class Author
    {
        // ...
        private $firstName;

        public function isAuthorValid(ExecutionContextInterface $context)
        {
            // Vous avez un tableau de « faux noms »
            $fakeNames = array();

            // vérifie si le nom est un faux
            if (in_array($this->getFirstName(), $fakeNames)) {
                $context->addViolationAt('firstname', 'This name sounds totally fake!', array(), null);
            }
        }
    }

Options
-------

methods
~~~~~~~

**type**: ``array`` **default**: ``array()`` [:ref:`option par défaut<validation-default-option>`]

Il s'agit d'un tableau de méthodes qui doivent être exécutées durant le
processus de validation. Chacune de ces méthodes peut avoir l'un des formats
suivants :

1) **Nom de la méthode sous forme de chaîne de caractères**

    Si le nom de la méthode est une simple chaîne de caractères (par exemple : ``isAuthorValid``),
    cette méthode sera appelée sur le même objet que celui qui est en train d'être validé
    et ``ExecutionContext`` sera le seul argument (voyez l'exemple ci-dessus).

2) **Tableau statique**

    Chaque méthode peut également être spécifiée sous forme de tableau standard :

    .. configuration-block::

        .. code-block:: yaml

            # src/Acme/BlogBundle/Resources/config/validation.yml
            Acme\BlogBundle\Entity\Author:
                constraints:
                    - Callback:
                        methods:
                            -    [Acme\BlogBundle\MyStaticValidatorClass, isAuthorValid]

        .. code-block:: php-annotations

            // src/Acme/BlogBundle/Entity/Author.php
            use Symfony\Component\Validator\Constraints as Assert;

            /**
             * @Assert\Callback(methods={
             *     { "Acme\BlogBundle\MyStaticValidatorClass", "isAuthorValid"}
             * })
             */
            class Author
            {
            }

        .. code-block:: xml

            <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
            <class name="Acme\BlogBundle\Entity\Author">
                <constraint name="Callback">
                    <option name="methods">
                        <value>Acme\BlogBundle\MyStaticValidatorClass</value>
                        <value>isAuthorValid</value>
                    </option>
                </constraint>
            </class>

        .. code-block:: php

            // src/Acme/BlogBundle/Entity/Author.php

            use Symfony\Component\Validator\Mapping\ClassMetadata;
            use Symfony\Component\Validator\Constraints\Callback;

            class Author
            {
                public $name;

                public static function loadValidatorMetadata(ClassMetadata $metadata)
                {
                    $metadata->addConstraint(new Callback(array(
                        'methods' => array('isAuthorValid'),
                    )));
                }
            }
    
    Dans ce cas, la méthode statique ``isAuthorValid`` sera appelée sur la classe
    ``Acme\BlogBundle\MyStaticValidatorClass``. Deux objets sont passés en paramètre,
    l'objet en cours de validation (par exemple : ``Author``) et le ``ExecutionContext``::

        namespace Acme\BlogBundle;
    
        use Symfony\Component\Validator\ExecutionContext;
        use Acme\BlogBundle\Entity\Author;
    
        class MyStaticValidatorClass
        {
            static public function isAuthorValid(Author $author, ExecutionContext $context)
            {
                // ...
            }
        }

    .. tip::

        Si vous spécifiez votre contrainte ``Callback`` via PHP, alors vous avez
        également le choix de faire votre callback en closure PHP, ou en non-statique.
        Il n'est, en revanche, *pas* possible de spécifier un :term:`service` comme
        contrainte. Pour faire de la validation en utilisant un service, vous devriez
        :doc:`créer une contrainte de validation personnalisée</cookbook/validation/custom_constraint>`
        et ajouter cette nouvelle contrainte à votre classe.
