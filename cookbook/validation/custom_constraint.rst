.. index::
   single: Validation; Custom constraints

Comment créer une Contrainte de Validation Personnalisée
--------------------------------------------------------

Vous pouvez créer une contrainte personnalisée en étendant la classe
:class:`Symfony\\Component\\Validator\\Constraint`. Les options pour vos
contraintes sont représentées par les propriétés publiques de la classe. Par
exemple, la contrainte :doc:`Url</reference/constraints/Url>` inclut
les propriétés ``message`` et ``protocols``:

.. code-block:: php

    namespace Symfony\Component\Validator\Constraints;
    
    use Symfony\Component\Validator\Constraint;

    /**
     * @Annotation
     */
    class Protocol extends Constraint
    {
        public $message = 'This value is not a valid protocol';
        public $protocols = array('http', 'https', 'ftp', 'ftps');
    }

.. note::

    L'annotation ``@Annotation`` est nécessaire pour cette nouvelle contrainte 
    afin de la rendre disponible via les annotations dans les autres classes.

Comme vous pouvez le voir, une classe « contrainte » est minimale. La validation est
réalisée par un autre validateur de contrainte. La classe de validation est définie
par l'implémentation de la méthode ``validatedBy()``, incluant une logique prédéfinie :

.. code-block:: php

    // dans la classe de base Symfony\Component\Validator\Constraint
    public function validatedBy()
    {
        return get_class($this).'Validator';
    }

Si vous créez une contrainte personnalisée (ex: ``MyConstraint``), Symfony2
recherchera automatiquement une autre classe, ``MyConstraintValidator`` lorsqu'il
effectue la validation.

La classe validatrice ne requiert qu'une méthode : ``validate``. Vous pouvez observer cela
dans l’exemple suivant, avec la classe ``ProtocolValidator``:

.. code-block:: php

    namespace Symfony\Component\Validator\Constraints;
    
    use Symfony\Component\Validator\Constraint;
    use Symfony\Component\Validator\ConstraintValidator;

    class ProtocolValidator extends ConstraintValidator
    {
        public function validate($value, Constraint $constraint)
        {
            if (!in_array($value, $constraint->protocols)){
                $this->context->addViolation($constraint->message, array('%protocols%' => $constraint->protocols));
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

                // associe le message d'erreur à la propriété foo
                $this->context->addViolationAtSubPath('foo', $constraint->getMessage(), array(), null);

                return false;
            }
 
            return true;
        }   
    }