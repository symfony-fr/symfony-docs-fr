Valid
=====

Cette contrainte est utilisée pour activer la validation sur les objets
imbriqués dans un objet qui doit être validé. Cela vous permet de valider
un objet et les sous-objets qui lui sont associés.

+----------------+---------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`             |
+----------------+---------------------------------------------------------------------+
| Options        | - `traverse`_                                                       |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Type`           |
+----------------+---------------------------------------------------------------------+

Utilisation de base
-------------------

Dans l'exemple suivant, nous créons deux classes ``Author`` et ``Address``
qui ont toutes les deux des contraintes sur leurs propriétés. De plus, 
l'objet ``Author`` stocke une instance d'``Address`` dans sa propriété ``$address``.

.. code-block:: php

    // src/Acme/HelloBundle/Address.php
    class Address
    {
        protected $street;
        protected $zipCode;
    }

.. code-block:: php

    // src/Acme/HelloBundle/Author.php
    class Author
    {
        protected $firstName;
        protected $lastName;
        protected $address;
    }

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/validation.yml
        Acme\HelloBundle\Address:
            properties:
                street:
                    - NotBlank: ~
                zipCode:
                    - NotBlank: ~
                    - Length: 
						max: 5

        Acme\HelloBundle\Author:
            properties:
                firstName:
                    - NotBlank: ~
                    - Length: 
						min: 4
                lastName:
                    - NotBlank: ~

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/validation.xml -->
        <class name="Acme\HelloBundle\Address">
            <property name="street">
                <constraint name="NotBlank" />
            </property>
            <property name="zipCode">
                <constraint name="NotBlank" />
                <constraint name="Length">
					<option name="max">5</option>
				</constraint>
            </property>
        </class>

        <class name="Acme\HelloBundle\Author">
            <property name="firstName">
                <constraint name="NotBlank" />
                <constraint name="Length">
					<option name="min">4</option>
				</constraint>
            </property>
            <property name="lastName">
                <constraint name="NotBlank" />
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HelloBundle/Address.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Address
        {
            /**
             * @Assert\NotBlank()
             */
            protected $street;

            /**
             * @Assert\NotBlank
             * @Assert\Length(max = "5")
             */
            protected $zipCode;
        }

        // src/Acme/HelloBundle/Author.php
        class Author
        {
            /**
             * @Assert\NotBlank
             * @Assert\Length(min = "4")
             */
            protected $firstName;

            /**
             * @Assert\NotBlank
             */
            protected $lastName;
            
            protected $address;
        }

    .. code-block:: php

        // src/Acme/HelloBundle/Address.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Length;
        
        class Address
        {
            protected $street;

            protected $zipCode;
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('street', new NotBlank());
                $metadata->addPropertyConstraint('zipCode', new NotBlank());
                $metadata->addPropertyConstraint(
					'zipCode', 
					new Length(array("max" => 5)));
            }
        }

        // src/Acme/HelloBundle/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Length;
        
        class Author
        {
            protected $firstName;

            protected $lastName;
            
            protected $address;
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new NotBlank());
                $metadata->addPropertyConstraint(
					'firstName', 
					new Length(array("min" => 4)));
                $metadata->addPropertyConstraint('lastName', new NotBlank());
            }
        }

Avec cette configuration, il est possible de valider un auteur dont l'adresse serait
incorrecte. Pour éviter ceci, ajouter la contrainte ``Valid`` à la propriété
``$address``.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/validation.yml
        Acme\HelloBundle\Author:
            properties:
                address:
                    - Valid: ~

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/validation.xml -->
        <class name="Acme\HelloBundle\Author">
            <property name="address">
                <constraint name="Valid" />
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HelloBundle/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /* ... */
            
            /**
             * @Assert\Valid
             */
            protected $address;
        }

    .. code-block:: php

        // src/Acme/HelloBundle/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Valid;
        
        class Author
        {
            protected $address;
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('address', new Valid());
            }
        }

Maintenant, si vous validez un auteur avec une adresse incorrecte, vous verrez
que la validation du champ ``Address`` échouera.

    Acme\HelloBundle\Author.address.zipCode:
    Cette valeur est trop longue. 5 caractères maximum sont autorisés

Options
-------

traverse
~~~~~~~~

**type**: ``string`` **default**: ``true``

Si cette contrainte est appliquée à une propriété qui contient un tableau
d'objets, alors chaque objet du tableau sera validé si cette option est
définie à ``true``.