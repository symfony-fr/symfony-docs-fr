Expression
==========

.. versionadded:: 2.4
    La contrainte Expression a été ajoutée à Symfony 2.4.

Cette contrainte vous permet d'utiliser une :ref:`expression <component-expression-language-examples>`
pour une validation plus complexe et dynamique. Voir `Utilisation de base`_ pour un
exemple. Voir :doc:`/reference/constraints/Callback` pour une contrainte
offrant une flexibilité similaire.

+----------------+-----------------------------------------------------------------------------------------------+
| S'applique à   | :ref:`class <validation-class-target>` ou :ref:`property/method <validation-property-target>` |
+----------------+-----------------------------------------------------------------------------------------------+
| Options        | - :ref:`expression <reference-constraint-expression-option>`                                  |
|                | - `message`_                                                                                  |
+----------------+-----------------------------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Expression`                               |
+----------------+-----------------------------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\ExpressionValidator`                      |
+----------------+-----------------------------------------------------------------------------------------------+

Utilisation de base
-------------------

Imaginez que vous avez une classe  ``BlogPost`` avec les propriétés ``category``
et ``isTechnicalPost`` ::

    namespace Acme\DemoBundle\Model;

    use Symfony\Component\Validator\Constraints as Assert;

    class BlogPost
    {
        private $category;

        private $isTechnicalPost;

        // ...

        public function getCategory()
        {
            return $this->category;
        }

        public function setIsTechnicalPost($isTechnicalPost)
        {
            $this->isTechnicalPost = $isTechnicalPost;
        }

        // ...
    }

Pour valider cet objet, vous avez quelques exigences spéciales :

A) Si ``isTechnicalPost`` est vrai, alors ``category`` doit être soit ``php``
   soit ``symfony``;
B) Si ``isTechnicalPost`` est faux, alors ``category`` peut être de n'importe
   quelle valeur.

Un des moyens d'accomplir cela est la contrainte Expression:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/config/validation.yml
        Acme\DemoBundle\Model\BlogPost:
            constraints:
                - Expression:
                    expression: "this.getCategory() in ['php', 'symfony'] or !this.isTechnicalPost()"
                    message: "If this is a tech post, the category should be either php or symfony!"

    .. code-block:: php-annotations

        // src/Acme/DemoBundle/Model/BlogPost.php
        namespace Acme\DemoBundle\Model;

        use Symfony\Component\Validator\Constraints as Assert;

        /**
         * @Assert\Expression(
         *     "this.getCategory() in ['php', 'symfony'] or !this.isTechnicalPost()",
         *     message="If this is a tech post, the category should be either php or symfony!"
         * )
         */
        class BlogPost
        {
            // ...
        }

    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">
            <class name="Acme\DemoBundle\Model\BlogPost">
                <constraint name="Expression">
                    <option name="expression">
                        this.getCategory() in ['php', 'symfony'] or !this.isTechnicalPost()
                    </option>
                    <option name="message">
                        If this is a tech post, the category should be either php or symfony!
                    </option>
                </constraint>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/DemoBundle/Model/BlogPost.php
        namespace Acme\DemoBundle\Model;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class BlogPost
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addConstraint(new Assert\Expression(array(
                    'expression' => 'this.getCategory() in ["php", "symfony"] or !this.isTechnicalPost()',
                    'message' => 'If this is a tech post, the category should be either php or symfony!',
                )));
            }

            // ...
        }

L'option :ref:`expression <reference-constraint-expression-option>` est l'expression
qui doit retourner true afin de valider la contrainte. Pour en savoir plus
sur la syntaxe des expressions, voir :doc:`/components/expression_language/syntax`.

.. sidebar:: Associer l'erreur à un champ spécifique

    Vous pouvez également associer la contrainte à une propriété spécifique
    tout en validant la totalité de l'entité. C'est pratique pour associer
    une erreur à un champ spécifique. Dans le contexte ci-dessous, ``value``
    représente la valeur de ``isTechnicalPost``.

    .. configuration-block::

        .. code-block:: yaml

            # src/Acme/DemoBundle/Resources/config/validation.yml
            Acme\DemoBundle\Model\BlogPost:
                properties:
                    isTechnicalPost:
                        - Expression:
                            expression: "this.getCategory() in ['php', 'symfony'] or value == false"
                            message: "If this is a tech post, the category should be either php or symfony!"

        .. code-block:: php-annotations

            // src/Acme/DemoBundle/Model/BlogPost.php
            namespace Acme\DemoBundle\Model;

            use Symfony\Component\Validator\Constraints as Assert;

            class BlogPost
            {
                // ...

                /**
                 * @Assert\Expression(
                 *     "this.getCategory() in ['php', 'symfony'] or value == false",
                 *     message="If this is a tech post, the category should be either php or symfony!"
                 * )
                 */
                private $isTechnicalPost;

                // ...
            }

        .. code-block:: xml

            <!-- src/Acme/DemoBundle/Resources/config/validation.xml -->
            <?xml version="1.0" encoding="UTF-8" ?>
            <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

                <class name="Acme\DemoBundle\Model\BlogPost">
                    <property name="isTechnicalPost">
                        <constraint name="Expression">
                            <option name="expression">
                                this.getCategory() in ['php', 'symfony'] or value == false
                            </option>
                            <option name="message">
                                If this is a tech post, the category should be either php or symfony!
                            </option>
                        </constraint>
                    </property>
                </class>
            </constraint-mapping>

        .. code-block:: php

            // src/Acme/DemoBundle/Model/BlogPost.php
            namespace Acme\DemoBundle\Model;

            use Symfony\Component\Validator\Constraints as Assert;
            use Symfony\Component\Validator\Mapping\ClassMetadata;

            class BlogPost
            {
                public static function loadValidatorMetadata(ClassMetadata $metadata)
                {
                    $metadata->addPropertyConstraint('isTechnicalPost', new Assert\Expression(array(
                        'expression' => 'this.getCategory() in ["php", "symfony"] or value == false',
                        'message' => 'If this is a tech post, the category should be either php or symfony!',
                    )));
                }

                // ...
            }

    .. caution::

        Dans Symfony 2.4 et Symfony 2.5, si la propriété (ex ``isTechnicalPost``)
        valait ``null``, l'expression n'était jamais appellée et sa valeur était
        considérée comme valide. Pour s'assurer que la valeur n'est pas ``null``,
        utilisez la :doc:`contrainte NotNull</reference/constraints/NotNull>`.

Pour plus d'informations sur l'expression et quelles variables sont disponibles
, voir :ref:`expression <reference-constraint-expression-option>` options
en détail ci-dessous.

Options
-------

.. _reference-constraint-expression-option:

expression
~~~~~~~~~~

**type**: ``string`` [:ref:`default option <validation-default-option>`]

L'expression qui sera évaluée. Si l'expression est évaluée à false (en utilisant
==, pas ===), la validation échouera.

Pour en savoir plus sur la syntaxe des expressions, voir
:doc:`/components/expression_language/syntax`.

Dans l'expression, vous pouvez avoir accès jusqu'à deux variables :

En fonction de votre façon d'utiliser la contrainte, vous avez accès à une
ou deux variables dans votre expression :

* ``this``: L'objet qui est validé (ex une instance de BlogPost);
* ``value``: La valeur de l'objet qui est validé (uniquement valable lorsque
  la contrainte s'applique directement à une propriété);

message
~~~~~~~

**type**: ``string`` **default**: ``This value is not valid.``

Le message par défaut quand l'expression revoit false.
