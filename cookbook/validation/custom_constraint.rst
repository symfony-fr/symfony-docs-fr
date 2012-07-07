.. index::
   single: Validation; Custom constraints

Comment créer une Contrainte de Validation Personnalisée
========================================================

Vous pouvez créer une contrainte personnalisée en étendant la classe
:class:`Symfony\\Component\\Validator\\Constraint`.
A titre d'exemple, nous allons créer un simple validateur qui vérifie qu'une
chaine de caractères ne contient que des caractères alphanumériques.

Créer une classe de contrainte
------------------------------

Tout d'abord, vous devez créer une classe de contrainte
qui étend la classe :class:`Symfony\\Component\\Validator\\Constraint`:: 

    namespace Acme\DemoBundle\Validator\Constraints;
    
    use Symfony\Component\Validator\Constraint;

    /**
     * @Annotation
     */
    class ContainsAlphanumeric extends Constraint
    {
        public $message = 'La chaine "%string%" contient un caractère non autorisé : elle ne peut contenir que des lettres et des chiffres.';
    }

.. note::

    L'annotation ``@Annotation`` est nécessaire pour cette nouvelle contrainte 
    afin de la rendre disponible via les annotations dans les autres classes.
    Les options de votre contrainte sont représentées par des propriétés publiques
    dans la classe de contrainte. 

Créer le validateur en lui-même
-------------------------------

Comme vous pouvez le voir, une classe « contrainte » est minimale. La validation est
réalisée par un autre validateur de contrainte. La classe de validation est définie
par l'implémentation de la méthode ``validatedBy()``, incluant une logique prédéfinie::

    // dans la classe de base Symfony\Component\Validator\Constraint
    public function validatedBy()
    {
        return get_class($this).'Validator';
    }

Si vous créez une contrainte personnalisée (ex: ``MyConstraint``), Symfony2
recherchera automatiquement une autre classe, ``MyConstraintValidator`` lorsqu'il
effectue la validation.

La classe validatrice ne requiert qu'une méthode : ``validate``::

    namespace Acme\DemoBundle\Validator\Constraints;
    
    use Symfony\Component\Validator\Constraint;
    use Symfony\Component\Validator\ConstraintValidator;

    class ContainsAlphanumericValidator extends ConstraintValidator
    {
        public function validate($value, Constraint $constraint)
        {
            if (!preg_match('/^[a-zA-Za0-9]+$/', $value, $matches)) {
                $this->context->addViolation($constraint->message, array('%string%' => $value));
            }
        }
    }

.. note::

    La méthode ``validate`` ne retourne pas de valeur; à la place, elle ajoute
    des violations de contraintes à la propriété ``context`` du validateur grâce
    à l'appel à la méthode ``addViolation`` dans le cas d'erreurs de validation.
    Par conséquent, une valeur peut être considérée comme validée si aucune
    violation de contrainte n'est ajoutée au contexte. Le premier paramètre de l'appel
    à ``addViolation`` est le message d'erreur utilisé pour la violation.

.. versionadded:: 2.1
 
    La méthode ``isValid`` est dépréciée au profit de ``validate`` dans Symfony 2.1. La
    méthode ``setMessage`` est également dépréciée, en faveur de l'appel à la méthode
    ``addViolation`` du contexte.

Utiliser le nouveau validateur
------------------------------

Utiliser un validateur personnalisé est très facile, tout comme ceux fournis par Symfony2
lui-même :

.. configuration-block::

    .. code-block:: yaml
        
        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\DemoBundle\Entity\AcmeEntity:
            properties:
                name:
                    - NotBlank: ~
                    - Acme\DemoBundle\Validator\Constraints\ContainsAlphanumeric: ~

    .. code-block:: php-annotations

        // src/Acme/DemoBundle/Entity/AcmeEntity.php
    
        use Symfony\Component\Validator\Constraints as Assert;
        use Acme\DemoBundle\Validator\Constraints as AcmeAssert;
            
        class AcmeEntity
        {
            // ...
            
            /**
             * @Assert\NotBlank
             * @AcmeAssert\ContainsAlphanumeric
             */
            protected $name;
            
            // ...
        }

    .. code-block:: xml
        
        <!-- src/Acme/DemoBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\DemoBundle\Entity\AcmeEntity">
                <property name="name">
                    <constraint name="NotBlank" />
                    <constraint name="Acme\DemoBundle\Validator\Constraints\ContainsAlphanumeric" />
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php
        
        // src/Acme/DemoBundle/Entity/AcmeEntity.php

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Acme\DemoBundle\Validator\Constraints\ContainsAlphanumeric;

        class AcmeEntity
        {
            public $name;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('name', new NotBlank());
                $metadata->addPropertyConstraint('name', new ContainsAlphanumeric());
            }
        }

Si votre contrainte contient des options, alors elles devraient être des
propriétés publiques de la classe de contrainte personnalisée que vous avez
créée plus tôt. Ces options peuvent être configurées comme toutes les options
des contraintes de Symfony.

Contraintes de validation avec dépendances
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si votre validateur possède des dépendances, comme une connexion à une base de données,
il faudra le configurer comme un service dans le conteneur d'injection de dépendances.
Ce service doit inclure le tag ``validator.constraint_validator`` et un attribut ``alias`` :

.. configuration-block::

    .. code-block:: yaml

        services:
            validator.unique.your_validator_name:
                class: Fully\Qualified\Validator\Class\Name
                tags:
                    - { name: validator.constraint_validator, alias: alias_name }

    .. code-block:: xml

        <service id="validator.unique.your_validator_name" class="Fully\Qualified\Validator\Class\Name">
            <argument type="service" id="doctrine.orm.default_entity_manager" />
            <tag name="validator.constraint_validator" alias="alias_name" />
        </service>

    .. code-block:: php

        $container
            ->register('validator.unique.your_validator_name', 'Fully\Qualified\Validator\Class\Name')
            ->addTag('validator.constraint_validator', array('alias' => 'alias_name'))
        ;

Votre classe ``contrainte`` devrait maintenant utiliser cet alias afin de référencer
le validateur approprié::

    public function validatedBy()
    {
        return 'alias_name';
    }

Comme mentionné précédemment, Symfony2 recherchera automatiquement une classe
nommée d'après le nom de la contrainte et suffixée par ``Validator``.  Si votre
validateur de contrainte est défini comme un service, il est important de
surcharger la méthode ``validatedBy()`` afin qu'elle renvoie l'alias utilisé pour
définir le service ; autrement, Symfony2 n'utilisera pas le service de validation,
et instanciera la classe, sans injecter les dépendances requises.

Contrainte de validation de classe
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Outre la validation d'une propriété de classe, une contrainte peut avoir une portée
de classe en renseignant une cible::

    public function getTargets()
    {
        return self::CLASS_CONSTRAINT;
    }

Avec ceci, la méthode ``validate()`` du validateur prend un objet comme premier argument::

    class ProtocolClassValidator extends ConstraintValidator
    {
        public function validate($protocol, Constraint $constraint)
        {
            if ($protocol->getFoo() != $protocol->getBar()) {

                $this->context->addViolationAtSubPath('foo', $constraint->message, array(), null);
            }
        }
    }