.. index::
   single: Sécurité; Fournisseur d'Utilisateur

Comment créer un Fournisseur d'Utilisateur personnalisé
=======================================================

Une partie du processus d'authentification standard de Symfony dépend des
« fournisseurs d'utilisateur ». Lorsqu'un utilisateur soumet un nom
d'utilisateur et un mot de passe, la couche d'authentification demande
au fournisseur d'utilisateur configuré de retourner un objet utilisateur
pour un nom d'utilisateur donné. Symfony vérifie alors si le mot de passe
de cet utilisateur est correct ou non et génère un token de sécurité afin
que l'utilisateur reste authentifié pendant la session courante. Symfony
possède par défaut un fournisseur d'utilisateur « in_memory » et « entity ».
Dans cet article, vous verrez comment vous pouvez créer votre
propre fournisseur d'utilisateur, ce qui pourrait être utile si vous accédez à
vos utilisateurs via une base de données personnalisée, un fichier, ou - comme
le montre cet exemple - à travers un service web.

Créer une Classe Utilisateur
----------------------------

Tout d'abord, quelle que soit la source de vos données utilisateurs, vous
allez avoir besoin de créer une classe ``User`` qui représente ces données.
Le ``User`` peut ressembler à ce que vous voulez et contenir n'importe
quelles données. La seule condition requise est que la classe implémente
:class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`. Les
méthodes de cette interface doivent donc être définies dans la classe
utilisateur personnalisée : ``getRoles()``, ``getPassword()``, ``getSalt()``,
``getUsername()``, ``eraseCredentials()``, ``equals()``.

Voyons cela en action::

    // src/Acme/WebserviceUserBundle/Security/User/WebserviceUser.php
    namespace Acme\WebserviceUserBundle\Security\User;

    use Symfony\Component\Security\Core\User\UserInterface;

    class WebserviceUser implements UserInterface
    {
        private $username;
        private $password;
        private $salt;
        private $roles;

        public function __construct($username, $password, $salt, array $roles)
        {
            $this->username = $username;
            $this->password = $password;
            $this->salt = $salt;
            $this->roles = $roles;
        }

        public function getRoles()
        {
            return $this->roles;
        }

        public function getPassword()
        {
            return $this->password;
        }

        public function getSalt()
        {
            return $this->salt;
        }

        public function getUsername()
        {
            return $this->username;
        }

        public function eraseCredentials()
        {
        }

        public function equals(UserInterface $user)
        {
            if (!$user instanceof WebserviceUser) {
                return false;
            }

            if ($this->password !== $user->getPassword()) {
                return false;
            }

            if ($this->getSalt() !== $user->getSalt()) {
                return false;
            }

            if ($this->username !== $user->getUsername()) {
                return false;
            }

            return true;
        }
    }

Si vous avez plus d'informations à propos de vos utilisateurs - comme un
« prénom » - alors vous pouvez ajouter un champ ``firstName`` pour contenir
cette donnée.

Pour plus de détails sur chacune de ces méthodes, voir l'interface
:class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`.

Créer un Fournisseur d'Utilisateur
----------------------------------

Maintenant que vous avez une classe ``User``, vous allez créer un fournisseur
d'utilisateur qui va récupérer les informations utilisateur depuis un service
web, et nous allons aussi créer un objet ``WebserviceUser`` et le remplir avec
des données.

Le fournisseur d'utilisateur est juste une classe PHP qui doit implémenter
:class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface`,
qui requiert que trois méthodes soient définies : ``loadUserByUsername($username)``,
``refreshUser(UserInterface $user)``, et ``supportsClass($class)``. Pour plus
de détails, voir l'interface
:class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface`.

Voici un exemple de ce à quoi cela pourrait ressembler::

    // src/Acme/WebserviceUserBundle/Security/User/WebserviceUserProvider.php
    namespace Acme\WebserviceUserBundle\Security\User;

    use Symfony\Component\Security\Core\User\UserProviderInterface;
    use Symfony\Component\Security\Core\User\UserInterface;
    use Symfony\Component\Security\Core\Exception\UsernameNotFoundException;
    use Symfony\Component\Security\Core\Exception\UnsupportedUserException;

    class WebserviceUserProvider implements UserProviderInterface
    {
        public function loadUserByUsername($username)
        {
            // effectuez un appel à votre service web ici
            $userData = ...
            // supposons qu'il retourne un tableau en cas de succès, ou bien
            // « false » s'il n'y a pas d'utilisateur

            if ($userData) {
                $password = '...';
                
                // ...

                return new WebserviceUser($username, $password, $salt, $roles);
            }

            throw new UsernameNotFoundException(sprintf('Username "%s" does not exist.', $username));            
        }

        public function refreshUser(UserInterface $user)
        {
            if (!$user instanceof WebserviceUser) {
                throw new UnsupportedUserException(sprintf('Instances of "%s" are not supported.', get_class($user)));
            }

            return $this->loadUserByUsername($user->getUsername());
        }

        public function supportsClass($class)
        {
            return $class === 'Acme\WebserviceUserBundle\Security\User\WebserviceUser';
        }
    }

Créer un Service pour le Fournisseur d'Utilisateur
--------------------------------------------------

Maintenant, vous allez rendre le fournisseur d'utilisateur disponible en tant que
service.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/WebserviceUserBundle/Resources/config/services.yml
        parameters:
            webservice_user_provider.class: Acme\WebserviceUserBundle\Security\User\WebserviceUserProvider

        services:
            webservice_user_provider:
                class: "%webservice_user_provider.class%"

    .. code-block:: xml

        <!-- src/Acme/WebserviceUserBundle/Resources/config/services.xml -->
        <parameters>
            <parameter key="webservice_user_provider.class">Acme\WebserviceUserBundle\Security\User\WebserviceUserProvider</parameter>
        </parameters>

        <services>
            <service id="webservice_user_provider" class="%webservice_user_provider.class%"></service>
        </services>

    .. code-block:: php

        // src/Acme/WebserviceUserBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('webservice_user_provider.class', 'Acme\WebserviceUserBundle\Security\User\WebserviceUserProvider');

        $container->setDefinition('webservice_user_provider', new Definition('%webservice_user_provider.class%');

.. tip::

    L'implémentation réelle du fournisseur d'utilisateur aura probablement
    certaines dépendances, des options de configuration ou d'autres services.
    Ajoutez ces derniers en tant qu'arguments dans la définition du service.

.. note::

    Assurez-vous que le fichier des services est importé. Lisez
    :ref:`service-container-imports-directive` pour plus de détails.

Modifier ``security.yml``
-------------------------

Dans le fichier ``/app/config/security.yml``, tout vient ensemble. Ajoutez le
fournisseur d'utilisateur à la liste des fournisseurs dans la section « security ».
Choisissez un nom pour le fournisseur d'utilisateur (par exemple : « webservice »)
et spécifiez l'id du service que vous venez de définir.

.. code-block:: yaml

    security:
        providers:
            webservice:
                id: webservice_user_provider

Symfony a aussi besoin de savoir comment encoder les mots de passe qui sont
soumis par les utilisateurs du site web, par exemple lorsque ces derniers
remplissent un formulaire de login. Vous pouvez effectuer cela en ajoutant
une ligne dans la section « encoders » dans le fichier ``/app/config/security.yml``.

.. code-block:: yaml

    security:
        encoders:
            Acme\WebserviceUserBundle\Security\User\WebserviceUser: sha512

La valeur spécifiée ici devrait correspondre à l'algorithme utilisé
initialement pour l'encodage des mots de passe lors de la création de vos
utilisateurs (qu'importe la manière dont vous avez créé ces utilisateurs).
Quand un utilisateur soumet son mot de passe, ce dernier est rajouté à la
valeur du salt et puis encodé en utilisant cet algorithme avant d'être
comparé avec le mot de passe crypté retourné par votre méthode ``getPassword()``.
De plus, en fonction de vos options, le mot de passe pourrait être encodé
plusieurs fois et encodé ensuite en base64.

.. sidebar:: Détails sur la manière dont les mots de passe sont encryptés

    Symfony utilise une méthode spécifique pour combiner le salt et encoder le
    mot de passe avant de le comparer avec votre mot de passe encodé. Si
    ``getSalt()`` ne retourne rien, alors le mot de passe soumis est simplement
    encodé en utilisant l'algorithme que vous avez spécifié dans le fichier
    ``security.yml``. Si un salt *est* spécifié, alors la valeur suivante est
    créée et *ensuite* assemblée pour former un « hash » via l'algorithme :

        ``$password.'{'.$salt.'}';``

    Si vos utilisateurs externes ont leurs mots de passe encodés d'une façon
    différente, alors vous aurez besoin de travailler un peu plus afin que
    Symfony encode correctement le mot de passe. Ceci est au-delà de la portée
    de cet article, mais devrait inclure de créer une sous-classe
    ``MessageDigestPasswordEncoder`` ainsi que surcharger la méthode
    ``mergePasswordAndSalt``.

    De surcroît, le « hash », par défaut, est encodé plusieurs fois puis
    encodé en base64. Pour des détails plus spécifiques, voir
    `MessageDigestPasswordEncoder`_. Pour éviter cela, configurez-le dans le
    fichier ``security.yml`` :

    .. code-block:: yaml

        security:
            encoders:
                Acme\WebserviceUserBundle\Security\User\WebserviceUser:
                    algorithm: sha512
                    encode_as_base64: false
                    iterations: 1

.. _MessageDigestPasswordEncoder: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Security/Core/Encoder/MessageDigestPasswordEncoder.php
