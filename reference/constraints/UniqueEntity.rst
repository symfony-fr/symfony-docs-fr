UniqueEntity
============

Valide qu'un champ particulier (ou plusieurs champs) d'une entité Doctrine sont
uniques.
C'est souvent utilisé, par exemple, pour éviter qu'un nouvel utilisateur ne
s'enregistre avec une adresse email qui existe déjà dans le système.

+----------------+-------------------------------------------------------------------------------------+
| S'applique à   | :ref:`classe<validation-class-target>`                                              |
+----------------+-------------------------------------------------------------------------------------+
| Options        | - `fields`_                                                                         |
|                | - `message`_                                                                        |
|                | - `em`_                                                                             |
+----------------+-------------------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Bridge\\Doctrine\\Validator\\Constraints\\UniqueEntity`            |
+----------------+-------------------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Bridge\\Doctrine\\Validator\\Constraints\\UniqueEntityValidator`   |
+----------------+-------------------------------------------------------------------------------------+

Utilisation de base
-------------------

Supposons que vous avez un ``AcmeUserBundle`` avec une entité ``User`` qui
a un champ ``email``. Vous pouvez utiliser la contrainte ``Unique`` pour garantir
que le champ ``email``soit unique dans votre table d'utilisateurs :

.. configuration-block::

    .. code-block:: php-annotations

        // Acme/UserBundle/Entity/User.php
        use Symfony\Component\Validator\Constraints as Assert;
        use Doctrine\ORM\Mapping as ORM;

        // N'OUBLIEZ pas d'inclure ce use !
        use Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity;

        /**
         * @ORM\Entity
         * @UniqueEntity("email")
         */
        class Author
        {
            /**
             * @var string $email
             *
             * @ORM\Column(name="email", type="string", length=255, unique=true)
             * @Assert\Email()
             */
            protected $email;
            
            // ...
        }

    .. code-block:: yaml

        # src/Acme/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\Author:
            constraints:
                - Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity: email
            properties:
                email:
                    - Email: ~

    .. code-block:: xml

        <class name="Acme\UserBundle\Entity\Author">
            <constraint name="Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity">
                <option name="fields">email</option>
                <option name="message">Cet email existe déjà.</option>
            </constraint>
             <property name="email">
                <constraint name="Email" />
            </property>
        </class>

Options
-------

fields
~~~~~~

**type**: ``array``|``string`` [:ref:`default option<validation-default-option>`]

Cette option obligatoire est le champ (ou la liste de champs) qui sont
uniques pour l'entité. Par exemple, si vous spécifiez les champs
``email`` et ``name`` dans la même contrainte ``UniqueEntity``, alors vous êtes
assuré que chaque combinaison de ces deux champs est unique (deux utilisateurs
peuvent donc avoir le même email tant qu'il n'ont pas aussi le même nom).

Si vous voulez que deux champs soient unique de façon individuelle (c'est-à-dire
que ``email`` est unique *et* que ``name`` est unique), vous utilisez deux
entrées ``UniqueEntity``, une pour chaque champ.


message
~~~~~~~

**type**: ``string`` **default**: ``This value is already used.``

Le message qui sera affiché si la validation échoue.

em
~~

**type**: ``string``

Le nom du gestionnaire d'entité (entity manager) à utiliser pour faire la
requête qui déterminera l'unicité. Si elle est vide, le gestionnaire sera
déterminé automatiquement pour cette classe. Pour cette raison, vous n'aurez
probablement pas besoin d'utiliser cette option.