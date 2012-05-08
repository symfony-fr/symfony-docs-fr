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

La classe validatrice ne requiert qu'une méthode : ``isValid``. Vous pouvez observer cela
dans l’exemple suivant, avec la classe ``ProtocolValidator``:

.. code-block:: php

    namespace Symfony\Component\Validator\Constraints;
    
    use Symfony\Component\Validator\Constraint;
    use Symfony\Component\Validator\ConstraintValidator;

    class ProtocolValidator extends ConstraintValidator
    {
        public function isValid($value, Constraint $constraint)
        {
            if (in_array($value, $constraint->protocols)){
                $this->setMessage($constraint->message, array('%protocols%' => $constraint->protocols));

                return true;
            }

            return false;
        }
    }

.. note::

    N'oubliez pas d'appeler ``setMessage`` pour construire un message d'erreur
    lorsque la valeur est non-valide.

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

Avec ceci, la méthode ``isValid()`` du validateur prend un objet comme premier argument::

    class ProtocolClassValidator extends ConstraintValidator
    {
        public function isValid($protocol, Constraint $constraint)
        {
            if ($protocol->getFoo() != $protocol->getBar()) {

                // associe le message d'erreur à la propriété foo
                $this->context->addViolationAtSubPath('foo', $constraint->getMessage(), array(), null);

                return true;
            }
 
            return false;
        }   
    }