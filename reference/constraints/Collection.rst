Collection
==========

Cette contrainte est utilisée lorsque l'objet sous-jacent est une collection
(c'est-à-dire un tableau ou un objet qui implémentent ``Traversable`` et ``ArrayAccess``),
mais vous aimeriez valider les différentes clés de cette collection de différentes
manières. Par exemple, vous pourriez valider la clé ``email`` en utilisant la contrainte
``Email`` et la clé ``inventory`` avec la contrainte ``Min``.

Cette contrainte permet également de s'assurer que certaines clés de la collection
sont bien présente et que des clés supplémentaires ne le sont pas.

+----------------+--------------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`                  |
+----------------+--------------------------------------------------------------------------+
| Options        | - `fields`_                                                              |
|                | - `allowExtraFields`_                                                    |
|                | - `extraFieldsMessage`_                                                  |
|                | - `allowMissingFields`_                                                  |
|                | - `missingFieldsMessage`_                                                |
+----------------+--------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Collection`          |
+----------------+--------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\CollectionValidator` |
+----------------+--------------------------------------------------------------------------+

Utilisation de base
-------------------

La contrainte ``Collection`` vous permet de valider les différentes clés
d'une collection individuellement. Prenez l'exemple suivant::

    namespace Acme\BlogBundle\Entity;
    
    class Author
    {
        protected $profileData = array(
            'personal_email',
            'short_bio',
        );

        public function setProfileData($key, $value)
        {
            $this->profileData[$key] = $value;
        }
    }

Pour valider que l'élément ``personal_email`` de la propriété ``profileData``
soit une adresse email valide et que l'élement ``short_bio`` soit rempli
et d'une longueur de moins de 100 caractères, vous pouvez procéder comme ceci :

.. configuration-block::

    .. code-block:: yaml

        properties:
            profileData:
                - Collection:
                    fields:
                        personal_email: Email
                        short_bio:
                            - NotBlank
                            - MaxLength:
                                limit:   100
                                message: Votre bio est trop longue!
                    allowMissingfields: true

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Collection(
             *     fields = {
             *         "personal_email" = @Assert\Email,
             *         "short_bio" = {
             *             @Assert\NotBlank(),
             *             @Assert\MaxLength(
             *                 limit = 100,
             *                 message = "Votre bio est trop longue!"
             *             )
             *         }
             *     },
             *     allowMissingfields = true
             * )
             */
             protected $profileData = array(
                 'personal_email',
                 'short_bio',
             );
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="profileData">
                <constraint name="Collection">
                    <option name="fields">
                        <value key="personal_email">
                            <constraint name="Email" />
                        </value>
                        <value key="short_bio">
                            <constraint name="NotBlank" />
                            <constraint name="MaxLength">
                                <option name="limit">100</option>
                                <option name="message">Votre bio est trop longue!</option>
                            </constraint>
                        </value>
                    </option>
                    <option name="allowMissingFields">true</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Collection;
        use Symfony\Component\Validator\Constraints\Email;
        use Symfony\Component\Validator\Constraints\MaxLength;

        class Author
        {
            private $options = array();

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('profileData', new Collection(array(
                    'fields' => array(
                        'personal_email' => new Email(),
                        'lastName' => array(new NotBlank(), new MaxLength(100)),
                    ),
                    'allowMissingFields' => true,
                )));
            }
        }

Présence et Absence de champs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Par défaut, cette contrainte valide plus que le simple fait que les champs
individuels de la collection respectent leurs contraintes respectives.
En fait, si des clés de la collection sont manquantes, ou s'il y a des clés
non reconnues, une erreur de validation sera affichée.

Si vous voulez autoriser des clés à être absentes de la collection  ou si vous
voulez autoriser des clés « extra » (en plus), vous pouvez modifier respectivement
les options `allowMissingFields`_ et `allowExtraFields`_. Dans l'exemple ci-dessus,
l'option ``allowMissingFields`` a été définie à true, ce qui veut dire que si
l'un des élements ``personal_email`` ou ``short_bio`` était manquant dans la
propriété ``$personalData``, aucune erreur de validation ne se serait produite.

Options
-------

fields
~~~~~~

**type**: ``array`` [:ref:`default option<validation-default-option>`]

Cette option est requise et est un tableau associatif qui définit toutes les
clés de la collection et, pour chaque clé, quel(s) validateur(s) doit être
exécuté.

allowExtraFields
~~~~~~~~~~~~~~~~

**type**: ``Boolean`` **default**: false

Si cette option est définie à ``false`` et que la collection contient un ou plusieurs
éléments qui ne sont pas inclus dans l'option `fields`_, une erreur de validation sera
retournée. Si elle est définie à ``true``, les champs en plus seront tolérés.

extraFieldsMessage
~~~~~~~~~~~~~~~~~~

**type**: ``Boolean`` **default**: ``The fields {{ fields }} were not expected``

Le message affiché si `allowExtraFields`_ est à false et que des champs supplémentaires
sont détectés.

allowMissingFields
~~~~~~~~~~~~~~~~~~

**type**: ``Boolean`` **default**: false

Si cette option est définie à ``false`` et qu'un ou plusieurs champs de l'option `fields`_
sont manquants, une erreur de validation sera retournée. Si elle est définie à ``true``,
ce n'est pas grave si des champs de l'option `fields_` sont absents de la collection.

missingFieldsMessage
~~~~~~~~~~~~~~~~~~~~

**type**: ``Boolean`` **default**: ``The fields {{ fields }} are missing``

Le message affiché si `allowMissingFields`_ est à false et qu'un ou plusieurs champs
sont absents de la collection.