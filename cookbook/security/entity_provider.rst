.. index::
   single: Sécurité; Fournisseur d'utilisateur
   single: Sécurité; Fournisseur d'entité

Comment charger les utilisateurs depuis la base de données (le fournisseur d'Entité)
====================================================================================

La couche de sécurité est l'un des outils les plus intelligents de Symfony. Il
gère deux choses : les procédés d'authentification et d'autorisation. Bien qu'il
puisse être difficile de comprendre comment cela fonctionne en interne, le
système de sécurité est très flexible et vous permet d'intégrer votre application
avec n'importe quel « backend » d'authentification, comme Active Directory, un
serveur OAuth ou une base de données.

Introduction
------------

Cet article traite de l'authentification des utilisateurs à travers
une table de base de données gérée par une classe entité Doctrine. Le contenu
de cet article du Cookbook est divisé en trois parties. La première partie
parle de la conception d'une classe entité Doctrine ``User`` ainsi que du
fait de la rendre utilisable par la couche de sécurité de Symfony. La deuxième
partie décrit comment authentifier facilement un utilisateur avec l'objet Doctrine
:class:`Symfony\\Bridge\\Doctrine\\Security\\User\\EntityUserProvider` fourni
avec le « framework » et quelques éléments de configurations. Finalement, le
tutoriel va démontrer comment créer un objet personnalisé
:class:`Symfony\\Bridge\\Doctrine\\Security\\User\\EntityUserProvider` pour
récupérer des utilisateurs depuis la base de données sous certaines
conditions personnalisées.

Ce tutoriel suppose qu'il existe un bundle ``Acme\UserBundle`` démarré et chargé
dans le kernel de l'application.

Le Modèle de Données
--------------------

Pour cet article du cookbook, le bundle ``AcmeUserBundle`` contient une classe
entité ``User`` avec les champs suivants : ``id``, ``username``, ``salt``,
``password``, ``email`` et ``isActive``. Le champ ``isActive`` précise si oui
ou non le compte de l'utilisateur est activé.

Pour faire court, les méthodes « getter » et « setter » de chacun des champs
ont été supprimées pour se concentrer sur les méthodes les plus importantes
qui proviennent de l'interface
:class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`.


.. code-block:: php

    // src/Acme/UserBundle/Entity/User.php
    namespace Acme\UserBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Symfony\Component\Security\Core\User\UserInterface;

    /**
     * Acme\UserBundle\Entity\User
     *
     * @ORM\Table(name="acme_users")
     * @ORM\Entity(repositoryClass="Acme\UserBundle\Entity\UserRepository")
     */
    class User implements UserInterface, \Serializable
    {
        /**
         * @ORM\Column(type="integer")
         * @ORM\Id
         * @ORM\GeneratedValue(strategy="AUTO")
         */
        private $id;

        /**
         * @ORM\Column(type="string", length=25, unique=true)
         */
        private $username;

        /**
         * @ORM\Column(type="string", length=32)
         */
        private $salt;

        /**
         * @ORM\Column(type="string", length=40)
         */
        private $password;

        /**
         * @ORM\Column(type="string", length=60, unique=true)
         */
        private $email;

        /**
         * @ORM\Column(name="is_active", type="boolean")
         */
        private $isActive;

        public function __construct()
        {
            $this->isActive = true;
            $this->salt = md5(uniqid(null, true));
        }

        /**
         * @inheritDoc
         */
        public function getUsername()
        {
            return $this->username;
        }

        /**
         * @inheritDoc
         */
        public function getSalt()
        {
            return $this->salt;
        }

        /**
         * @inheritDoc
         */
        public function getPassword()
        {
            return $this->password;
        }

        /**
         * @inheritDoc
         */
        public function getRoles()
        {
            return array('ROLE_USER');
        }

        /**
         * @inheritDoc
         */
        public function eraseCredentials()
        {
        }

       /**
         * @see \Serializable::serialize()
         */
        public function serialize()
        {
            return serialize(array(
                $this->id,
            ));
        }

        /**
         * @see \Serializable::unserialize()
         */
        public function unserialize($serialized)
        {
            list (
                $this->id,
            ) = unserialize($serialized);
        }
    }

Afin d'utiliser une instance de la classe ``AcmeUserBundle:User`` dans la couche
de sécurité de Symfony, la classe entité doit implémenter l'interface
:class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`. Cette interface
force la classe à implémenter les cinq méthodes suivantes :
* ``getRoles()``,
* ``getPassword()``,
* ``getSalt()``,
* ``getUsername()``,
* ``eraseCredentials()``

Pour plus de détails sur chacune d'entre elles, voir
:class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`.

.. note::

    L'interface :phpclass:`Serializable` ainsi que ses méthodes ``serialize`` et ``unserialize``
    ont été ajoutées pour permettre à la classe ``User`` d'être sérialisable
    dans la session. Cela peut ou non être nécessaire en fonction de votre configuration,
    mais c'est certainement une bonne idée. Seule la propriété ``id`` a besoin d'être
    sérialisée, car la méthode :method:`Symfony\\Bridge\\Doctrine\\Security\\User\\EntityUserProvider::refreshUser`
    recharge l'utilisateur à chaque requête en utilisant la propriété ``id``.

.. code-block:: php

    // src/Acme/UserBundle/Entity/User.php
    namespace Acme\UserBundle\Entity;

    use Symfony\Component\Security\Core\User\EquatableInterface;

    // ...

    public function isEqualTo(UserInterface $user)
    {
        return $this->username === $user->getUsername();
    }

Voici, ci-dessous, un export de ma table ``User`` depuis MySQL. Pour plus de détails sur
la création des entrées utilisateur et l'encodage de leur mot de passe, lisez le
chapitre :ref:`book-security-encoding-user-password`.

.. code-block:: text

    mysql> select * from user;
    +----+----------+----------------------------------+------------------------------------------+--------------------+-----------+
    | id | username | salt                             | password                                 | email              | is_active |
    +----+----------+----------------------------------+------------------------------------------+--------------------+-----------+
    |  1 | hhamon   | 7308e59b97f6957fb42d66f894793079 | 09610f61637408828a35d7debee5b38a8350eebe | hhamon@example.com |         1 |
    |  2 | jsmith   | ce617a6cca9126bf4036ca0c02e82dee | 8390105917f3a3d533815250ed7c64b4594d7ebf | jsmith@example.com |         1 |
    |  3 | maxime   | cd01749bb995dc658fa56ed45458d807 | 9764731e5f7fb944de5fd8efad4949b995b72a3c | maxime@example.com |         0 |
    |  4 | donald   | 6683c2bfd90c0426088402930cadd0f8 | 5c3bcec385f59edcc04490d1db95fdb8673bf612 | donald@example.com |         1 |
    +----+----------+----------------------------------+------------------------------------------+--------------------+-----------+
    4 rows in set (0.00 sec)

La base de données contient désormais quatre utilisateurs avec différents
noms d'utilisateurs, emails et statuts. La prochaine partie va traiter de
l'authentification de l'un de ces utilisateurs grâce au fournisseur
d'entité utilisateur Doctrine et à quelques lignes de configuration.

Authentifier quelqu'un à travers une base de données
----------------------------------------------------

Authentifier un utilisateur Doctrine à travers une base de données avec la
couche de sécurité de Symfony est vraiment très facile. Tout réside dans la
configuration du :doc:`SecurityBundle</reference/configuration/security>`
stockée dans le fichier ``app/config/security.yml``.

Vous trouverez ci-dessous un exemple de configuration où l'utilisateur
va entrer son nom d'utilisateur et son  mot de passe via une authentification
basique HTTP. Cette information sera alors comparée et vérifiée avec nos
entrées d'entité « User » de la base de données :

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            encoders:
                Acme\UserBundle\Entity\User:
                    algorithm:        sha1
                    encode_as_base64: false
                    iterations:       1

            role_hierarchy:
                ROLE_ADMIN:       ROLE_USER
                ROLE_SUPER_ADMIN: [ ROLE_USER, ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH ]

            providers:
                administrators:
                    entity: { class: AcmeUserBundle:User, property: username }

            firewalls:
                admin_area:
                    pattern:    ^/admin
                    http_basic: ~

            access_control:
                - { path: ^/admin, roles: ROLE_ADMIN }

La section ``encoders`` associe l'encodeur de mot de passe ``sha1`` à la classe
entité. Cela signifie que Symfony va s'attendre à ce que le mot de passe stocké
dans la base de données soit encodé à l'aide de cet algorithme. Pour plus de détails
sur la création d'un nouvel objet « User » avec un mot de passe encrypté
correctement, lisez la section :ref:`book-security-encoding-user-password` du
chapitre sur la sécurité.

La section ``providers`` définit un fournisseur d'utilisateur ``administrators``.
Un fournisseur d'utilisateur est une « source » indiquant où les utilisateurs
sont chargés lors de l'authentification. Dans ce cas, le mot-clé ``entity``
signifie que Symfony va utiliser le fournisseur d'entité utilisateur Doctrine
pour charger des objets entité « User » depuis la base de données en utilisant
le champ unique ``username``. En d'autres termes, cela indique à Symfony
comment récupérer l'utilisateur depuis la base de données avant de vérifier
la validité du mot de passe.

Ce code et cette configuration fonctionnent mais ce n'est pas suffisant pour
sécuriser l'application pour des utilisateurs **activés**. En effet, maintenant,
vous pouvez toujours vous authentifier avec ``maxime``. La section suivante
explique comment interdire l'accès aux utilisateurs non-activés.

Interdire les Utilisateurs non-activés
--------------------------------------

La manière la plus facile d'exclure des utilisateurs non-activés est
d'implémenter l'interface
:class:`Symfony\\Component\\Security\\Core\\User\\AdvancedUserInterface`
qui se charge de vérifier le statut du compte de l'utilisateur.
L'interface :class:`Symfony\\Component\\Security\\Core\\User\\AdvancedUserInterface`
étend l'interface :class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`,
donc vous devez simplement utiliser la nouvelle interface dans la classe
entité ``AcmeUserBundle:User`` afin de bénéficier de comportements
d'authentification simples et avancés.

L'interface :class:`Symfony\\Component\\Security\\Core\\User\\AdvancedUserInterface`
ajoute quatre méthodes supplémentaires pour valider le statut du compte :

* ``isAccountNonExpired()`` vérifie si le compte de l'utilisateur a expiré,
* ``isAccountNonLocked()`` vérifie si l'utilisateur est verrouillé,
* ``isCredentialsNonExpired()`` vérifie si les informations de connexion de
  l'utilisateur (mot de passe) ont expiré,
* ``isEnabled()`` vérifie si l'utilisateur est activé.

Pour cet exemple, les trois premières méthodes vont retourner ``true`` alors
que la méthode ``isEnabled()`` va retourner la valeur booléenne du champ
``isActive``.

.. code-block:: php

    // src/Acme/UserBundle/Entity/User.php
    namespace Acme\UserBundle\Entity;

    // ...
    use Symfony\Component\Security\Core\User\AdvancedUserInterface;


    class User implements AdvancedUserInterface, \Serializable
    {
        // ...

        public function isAccountNonExpired()
        {
            return true;
        }

        public function isAccountNonLocked()
        {
            return true;
        }

        public function isCredentialsNonExpired()
        {
            return true;
        }

        public function isEnabled()
        {
            return $this->isActive;
        }
    }

Si vous essayez de vous authentifier avec ``maxime``, l'accès est maintenant
interdit puisque cet utilisateur n'a pas un compte activé. La prochaine section
va se concentrer sur l'implémentation d'un fournisseur d'entité personnalisé
pour authentifier un utilisateur avec son nom d'utilisateur ou avec son adresse
email.

Authentifier quelqu'un avec un fournisseur d'entité personnalisé
----------------------------------------------------------------

La prochaine étape est de permettre à un utilisateur de s'authentifier avec son
nom d'utilisateur ou avec son adresse email comme ils sont tous les deux uniques
dans la base de données. Malheureusement, le fournisseur d'entité natif est
seulement capable de gérer une propriété unique pour récupérer l'utilisateur
depuis la base de données.

Pour réaliser ceci, créez un fournisseur d'entité personnalisé qui cherche
un utilisateur dont le champ « nom d'utilisateur » *ou* « email » correspond
au nom d'utilisateur soumis lors de la phase de connexion/login. La bonne
nouvelle est qu'un objet Repository Doctrine peut agir comme un fournisseur
d'entité utilisateur s'il implémente
:class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface`.
Cette interface est fournie avec trois méthodes à implémenter :
``loadUserByUsername($username)``, ``refreshUser(UserInterface $user)``,
et ``supportsClass($class)``. Pour plus de détails, lisez
:class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface`.

Le code ci-dessous montre l'implémentation de
:class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface` dans
la classe ``UserRepository``::

    // src/Acme/UserBundle/Entity/UserRepository.php
    namespace Acme\UserBundle\Entity;

    use Symfony\Component\Security\Core\User\UserInterface;
    use Symfony\Component\Security\Core\User\UserProviderInterface;
    use Symfony\Component\Security\Core\Exception\UsernameNotFoundException;
    use Symfony\Component\Security\Core\Exception\UnsupportedUserException;
    use Doctrine\ORM\EntityRepository;
    use Doctrine\ORM\NoResultException;

    class UserRepository extends EntityRepository implements UserProviderInterface
    {
        public function loadUserByUsername($username)
        {
            $q = $this
                ->createQueryBuilder('u')
                ->where('u.username = :username OR u.email = :email')
                ->setParameter('username', $username)
                ->setParameter('email', $username)
                ->getQuery();

            try {
                // La méthode Query::getSingleResult() lance une exception
                // s'il n'y a pas d'entrée correspondante aux critères
                $user = $q->getSingleResult();
            } catch (NoResultException $e) {
                throw new UsernameNotFoundException(sprintf('Unable to find an active admin AcmeUserBundle:User object identified by "%s".', $username), 0, $e);
            }

            return $user;
        }

        public function refreshUser(UserInterface $user)
        {
            $class = get_class($user);
            if (!$this->supportsClass($class)) {
                throw new UnsupportedUserException(
                    sprintf(
                        'Instances of "%s" are not supported.',
                        $class
                    )
                );
            }

            return $this->find($user->getId());
        }

        public function supportsClass($class)
        {
            return $this->getEntityName() === $class || is_subclass_of($class, $this->getEntityName());
        }
    }

Pour finir l'implémentation, la configuration de la couche de sécurité doit
être modifiée pour dire à Symfony d'utiliser le nouveau fournisseur d'entité
personnalisé à la place du fournisseur d'entité Doctrine générique. Ceci est
facile à réaliser en supprimant le champ ``property`` dans la section
``security.providers.administrators.entity`` du fichier ``security.yml``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            providers:
                administrators:
                    entity: { class: AcmeUserBundle:User }
            # ...

En faisant cela, la couche de sécurité va utiliser une instance de
``UserRepository`` et appeler sa méthode ``loadUserByUsername()`` pour récupérer
un utilisateur depuis la base de données, qu'il ait saisi son nom d'utilisateur
ou son adresse email.

Gérer les rôles via la Base de Données
--------------------------------------

La fin de ce tutoriel se concentre sur comment stocker et récupérer une liste
de rôles depuis la base de données. Comme précisé précédemment, lorsque votre
utilisateur est « chargé », sa méthode ``getRoles()`` retourne le tableau contenant
ses rôles de sécurité qui doivent lui être assignés. Vous pouvez charger ces
données depuis n'importe où - une liste codée en dur et utilisée pour tous les
utilisateurs (par exemple : ``array('ROLE_USER')``), un tableau Doctrine en tant
que propriété nommée ``roles``, ou via une relation Doctrine, comme vous allez
le voir dans cette section.

.. caution::

    Avec une installation typique, vous devriez toujours retourner au moins 1 rôle
    avec la méthode ``getRoles()``. Par convention, un rôle appelé ``ROLE_USER``
    est généralement retourné. Si vous ne réussissez pas à retourner un quelconque
    rôle, cela voudrait dire que votre utilisateur n'est pas authentifié du tout.

Dans cet exemple, la classe entité ``AcmeUserBundle:User`` définit une relation
« many-to-many » avec une classe entité ``AcmeUserBundle:Group``. Un utilisateur
peut être relié à plusieurs groupes et un groupe peut être composé d'un ou plusieurs
utilisateurs. Comme un groupe est aussi un rôle, la méthode précédente ``getRoles()``
retourne maintenant la liste des groupes reliés::

    // src/Acme/UserBundle/Entity/User.php
    namespace Acme\UserBundle\Entity;

    use Doctrine\Common\Collections\ArrayCollection;

    // ...

    class User implements AdvancedUserInterface
    {
        /**
         * @ORM\ManyToMany(targetEntity="Group", inversedBy="users")
         *
         */
        private $groups;

        public function __construct()
        {
            $this->groups = new ArrayCollection();
        }

        // ...

        public function getRoles()
        {
            return $this->groups->toArray();
        }
    }

La classe entité ``AcmeUserBundle:Group`` définit trois champs de table (``id``,
``name`` et ``role``). Le champ unique ``role`` contient le nom du rôle utilisé
par la couche de sécurité de Symfony pour sécuriser des parties de l'application.
La chose la plus importante à noter est que la classe entité ``AcmeUserBundle:Group``
implémente l'interface :class:`Symfony\\Component\\Security\\Core\\Role\\RoleInterface`
qui la force à avoir une méthode ``getRole()``::

    // src/Acme/Bundle/UserBundle/Entity/Group.php
    namespace Acme\UserBundle\Entity;

    use Symfony\Component\Security\Core\Role\RoleInterface;
    use Doctrine\Common\Collections\ArrayCollection;
    use Doctrine\ORM\Mapping as ORM;

    /**
     * @ORM\Table(name="acme_groups")
     * @ORM\Entity()
     */
    class Group implements RoleInterface
    {
        /**
         * @ORM\Column(name="id", type="integer")
         * @ORM\Id()
         * @ORM\GeneratedValue(strategy="AUTO")
         */
        private $id;

        /**
         * @ORM\Column(name="name", type="string", length=30)
         */
        private $name;

        /**
         * @ORM\Column(name="role", type="string", length=20, unique=true)
         */
        private $role;

        /**
         * @ORM\ManyToMany(targetEntity="User", mappedBy="groups")
         */
        private $users;

        public function __construct()
        {
            $this->users = new ArrayCollection();
        }

        // ... getters and setters for each property

        /**
         * @see RoleInterface
         */
        public function getRole()
        {
            return $this->role;
        }
    }

Pour améliorer les performances et éviter le « lazy loading » de groupes lors de
la récupération d'un utilisateur depuis le fournisseur d'entité personnalisé, la
meilleure solution est d'effectuer une jointure avec la relation des groupes dans
la méthode ``UserRepository::loadUserByUsername()``. Cela va récupérer l'utilisateur
ainsi que ses rôles/groupes associés avec une requête unique::

    // src/Acme/UserBundle/Entity/UserRepository.php
    namespace Acme\UserBundle\Entity;

    // ...

    class UserRepository extends EntityRepository implements UserProviderInterface
    {
        public function loadUserByUsername($username)
        {
            $q = $this
                ->createQueryBuilder('u')
                ->select('u, g')
                ->leftJoin('u.groups', 'g')
                ->where('u.username = :username OR u.email = :email')
                ->setParameter('username', $username)
                ->setParameter('email', $username)
                ->getQuery();

            // ...
        }

        // ...
    }

La méthode ``QueryBuilder::leftJoin()`` joint et cherche les groupes liés
depuis la classe modèle ``AcmeUserBundle:User`` lorsqu'un utilisateur est
récupéré grâce à son adresse email ou à son nom d'utilisateur.
