Callback
========

.. versionadded:: 2.4
    L'assertion ``Callback`` a été simplifiée dans Symfony 2.4. Pour des exemples
    d'utilisation avec des versions antérieures de Symfony, lisez les versions
    correspondantes de cette page.

Le but de la contrainte Callback est de vous permettre de créer entièrement des
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
| S'applique à   | :ref:`class <validation-class-target>`                                 |
+----------------+------------------------------------------------------------------------+
| Options        | - :ref:`callback <callback-option>`                                    |
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
                - Callback: [validate]

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;
        use Symfony\Component\Validator\ExecutionContextInterface;

        class Author
        {
            /**
             * @Assert\Callback
             */
            public function validate(ExecutionContextInterface $context)
            {
                // ...
            }
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <constraint name="Callback">validate</constraint>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addConstraint(new Assert\Callback('validate'));
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

        public function validate(ExecutionContextInterface $context)
        {
            // Imaginons que vous avez un tableau de noms bidons
            $fakeNames = array(/* ... */);

            // Vérifie si le nom est bidon
            if (in_array($this->getFirstName(), $fakeNames)) {
                $context->buildViolation('Ce nom me semble complètement bidon !')
                    ->atPath('firstName')
                    ->addViolation()
                ;
            }
        }
    }

Callbacks statiques
-------------------

Vous pouvez aussi utiliser cette contrainte avec des méthodes statiques.
Étant donné que les méthodes statiques n'ont pas accès à l'instance de l'objet,
elles reçoivent l'objet en premier argument::

    public static function validate($object, ExecutionContextInterface $context)
    {
        // Imaginons que vous avez un tableau de noms bidons
        $fakeNames = array(/* ... */);

        // Vérifie si le nom est bidon
        if (in_array($object->getFirstName(), $fakeNames)) {
            $context->buildViolation('Ce nom me semble complètement bidon !')
                ->atPath('firstName')
                ->addViolation()
            ;
        }
    }

Callbacks externes et closures
------------------------------

Si vous voulez exécuter une callback statique qui ne se trouve pas dans la classe
de l'objet validé, vous pouvez configurer la contrainte pour qu'elle utilise un 
tableau de type callable tel que supporté par la fonction PHP :phpfunction:`call_user_func`.
Supposez que votre fonction de validation est ``Vendor\Package\Validator::validate()``::

    namespace Vendor\Package;

    use Symfony\Component\Validator\ExecutionContextInterface;

    class Validator
    {
        public static function validate($object, ExecutionContextInterface $context)
        {
            // ...
        }
    }

Vous pouvez alors utiliser la configuration suivante pour invoquer ce validateur :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            constraints:
                - Callback: [Vendor\Package\Validator, validate]

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        /**
         * @Assert\Callback({"Vendor\Package\Validator", "validate"})
         */
        class Author
        {
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <constraint name="Callback">
                    <value>Vendor\Package\Validator</value>
                    <value>validate</value>
                </constraint>
            </class>
        </constraint-mapping>

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
                    'Vendor\Package\Validator',
                    'validate',
                )));
            }
        }

.. note::

    La contrainte Callback ne supporte *pas* les fonctions de callback globales
    et il n'est pas possible de spécifier une fonction globale ou une méthode
    de :term:`service` en tant que callback. Pour valider en utilisant un service
    vous devez :doc:`créer une contrainte de validation personnalisée
    </cookbook/validation/custom_constraint>` et ajouter cette contrainte à votre 
    classe.

Quand vous configurez la contrainte en PHP, vous pouvez aussi passer une closure
au constructeur de la contrainte Callback::

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    use Symfony\Component\Validator\Mapping\ClassMetadata;
    use Symfony\Component\Validator\Constraints as Assert;

    class Author
    {
        public static function loadValidatorMetadata(ClassMetadata $metadata)
        {
            $callback = function ($object, ExecutionContextInterface $context) {
                // ...
            };

            $metadata->addConstraint(new Assert\Callback($callback));
        }
    }

Options
-------

.. _callback-option:

callback
~~~~~~~~

**type**: ``string``, ``array`` ou ``Closure`` [:ref:`default option <validation-default-option>`]

L'option callback accepte trois formats différents pour spécifier la méthode
de callback :

* Une **chaîne de caractères** contenant le nom d'une méthode concrète ou statique;

* Un tableau de type callable au format ``array('<Class>', '<method>')``;

* Une closure.

Les callbacks concrètes reçoivent une instance de :class:`Symfony\\Component\\Validator\\ExecutionContextInterface`
pour seul argument.

Les callbacks statiques ou de type closure reçoivent l'objet validé en premier
argument, et l'instance de :class:`Symfony\\Component\\Validator\\ExecutionContextInterface`
en second argument.
