True
====

Valide qu'une valeur est ``vraie`` (« true » en anglais). Spécifiquement, cette contrainte
vérifie que la valeur est exactement ``true``, exactement l'entier ``1``, ou exactement
la chaîne de caractère « ``1`` ».

Lisez également :doc:`False <False>`.

+----------------+---------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`             |
+----------------+---------------------------------------------------------------------+
| Options        | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\True`           |
+----------------+---------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\TrueValidator`  |
+----------------+---------------------------------------------------------------------+

Utilisation de base
-------------------

Cette contrainte peut être appliquée à une propriété (par exemple : une propriété
``termsAccepted`` d'un formulaire d'inscription) ou une méthode « getter ». Elle est
plus puissante dans le second cas, où vous pouvez vérifier que la méthode retourne true.
Par exemple, supposons que vous ayez la méthode suivante :

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    class Author
    {
        protected $token;

        public function isTokenValid()
        {
            return $this->token == $this->generateToken();
        }
    }

Vous pouvez appliquer la contrainte ``True`` à cette méthode.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            getters:
                tokenValid:
                    - "True": { message: "Le token est non valide." }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            protected $token;

            /**
             * @Assert\True(message = "The token is invalid")
             */
            public function isTokenValid()
            {
                return $this->token == $this->generateToken();
            }
        }

    .. code-block:: xml

        <!-- src/Acme/Blogbundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <getter property="tokenValid">
                <constraint name="True">
                    <option name="message">Le token est non valide.</option>
                </constraint>
            </getter>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\True;
        
        class Author
        {
            protected $token;
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addGetterConstraint('tokenValid', new True(array(
                    'message' => 'Le token est non valide.',
                )));
            }

            public function isTokenValid()
            {
                return $this->token == $this->generateToken();
            }
        }
        
Si la méthode ``isTokenValid()`` retourne false, la validation échouera.

Options
-------

message
~~~~~~~

**type**: ``string`` **default**: ``This value should be true``

Le message qui sera affiché si la donnée ne vaut pas true.