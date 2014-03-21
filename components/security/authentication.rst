.. index::
   single: Security, Authentication

Authentification
================

Lorsqu'une requête pointe sur une zone sécurisée, et qu'un des écouteurs du
plan du pare-feu est capable d'extraire les identifiants d'un utilisateur depuis
l'objet :class:`Symfony\\Component\\HttpFoundation\\Request` courant, il doit
créer un jeton, contenant ces identifiants.
Ensuite, l'écouteur doit demander au gestionnaire d'authentification de valider le
jeton donné, et retourner un jeton *authentifié* si les identifiants fournis ont
pu être validés.
L'écouteur devrait ensuite enregistrer le jeton authentifié dans le contexte de
sécurité ::

    use Symfony\Component\Security\Http\Firewall\ListenerInterface;
    use Symfony\Component\Security\Core\SecurityContextInterface;
    use Symfony\Component\Security\Core\Authentication\AuthenticationManagerInterface;
    use Symfony\Component\HttpKernel\Event\GetResponseEvent;
    use Symfony\Component\Security\Core\Authentication\Token\UsernamePasswordToken;

    class SomeAuthenticationListener implements ListenerInterface
    {
        /**
         * @var SecurityContextInterface
         */
        private $securityContext;

        /**
         * @var AuthenticationManagerInterface
         */
        private $authenticationManager;

        /**
         * @var string Uniquely identifies the secured area
         */
        private $providerKey;

        // ...

        public function handle(GetResponseEvent $event)
        {
            $request = $event->getRequest();

            $username = ...;
            $password = ...;

            $unauthenticatedToken = new UsernamePasswordToken(
                $username,
                $password,
                $this->providerKey
            );

            $authenticatedToken = $this
                ->authenticationManager
                ->authenticate($unauthenticatedToken);

            $this->securityContext->setToken($authenticatedToken);
        }
    }

.. note::

    Un jeton peut provenir de n'importe quelle classe, du moment qu'elle implémente
    l'interface :class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\TokenInterface`.

Le gestionnaire d'authentification
----------------------------------

Le gestionnaire d'authentification par défaut est une instance de
:class:`Symfony\\Component\\Security\\Core\\Authentication\\AuthenticationProviderManager`::

    use Symfony\Component\Security\Core\Authentication\AuthenticationProviderManager;

    // instances de Symfony\Component\Security\Core\Authentication\AuthenticationProviderInterface
    $providers = array(...);

    $authenticationManager = new AuthenticationProviderManager($providers);

    try {
        $authenticatedToken = $authenticationManager
            ->authenticate($unauthenticatedToken);
    } catch (AuthenticationException $failed) {
        // authentification a échoué
    }

L'``AuthenticationProviderManager``, lorsqu'il est instancié, reçoit un certain nombre de
fournisseurs d'authentification ("providers" en anglais), chacun supportant différents types
de jetons.

.. note::

    Vous pouvez bien sûr écrire votre propre gestionnaire d'authentification, celui-ci doit
    simplement implémenter l'interface
    :class:`Symfony\\Component\\Security\\Core\\Authentication\\AuthenticationManagerInterface`.


.. _authentication_providers:

Fournisseurs d'Authentification
-------------------------------

Chaque fournisseur (puisqu'il implémente l'interface
:class:`Symfony\\Component\\Security\\Core\\Authentication\\Provider\\AuthenticationProviderInterface`)
possède une méthode :method:`Symfony\\Component\\Security\\Core\\Authentication\\Provider\\AuthenticationProviderInterface::supports`
par lequel le ``AuthenticationProviderManager`` peut déterminer si le jeton fournis est supporté.
Si c'est le cas, le gestionnaire appelle ensuite la méthode
:class:`Symfony\\Component\\Security\\Core\\Authentication\\Provider\\AuthenticationProviderInterface::authenticate`
du fournisseur.
Cette méthode devrait retourner un jeton authentifié ou jeter une :class:`Symfony\\Component\\Security\\Core\\Exception\\AuthenticationException`
(ou toute autre exception qui l'étendrait).

Authentifier les utilisateurs par leur nom d'utilisateur et leur mot de passe
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Un fournisseur d'authentification tentera d'authentifier un utilisateur
en se basant sur les identifiants fournis. Habituellement, ces derniers sont
un nom d'utilisateur ainsi qu'un "hash" du mot de passe combiné à un salt
généré aléatoirement. Cela signifie que l'authentification consiste en
général à récupérer le salt ainsi que le mot de passe "hashé" de
l'utilisateur depuis le stockage de données, "hashe" le mot de passe qu'il
vient tout juste de fournir (en utilisant un formulaire de login par exemple)
et de comparer les deux pour déterminer si le mot de passe donné est valide.

Cette fonctionnalité est offerte par la classe
:class:`Symfony\\Component\\Security\\Core\\Authentication\\Provider\\DaoAuthenticationProvider`.
Le fournisseur récupère les informations de l'utilisateur depuis l'interface
:class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface`, utilise un
:class:`Symfony\\Component\\Security\\Core\\Encoder\\PasswordEncoderInterface` pour créer un hash
du mot de passe et retourne un jeton authentifié si le mot de passe est valide ::

    use Symfony\Component\Security\Core\Authentication\Provider\DaoAuthenticationProvider;
    use Symfony\Component\Security\Core\User\UserChecker;
    use Symfony\Component\Security\Core\User\InMemoryUserProvider;
    use Symfony\Component\Security\Core\Encoder\EncoderFactory;

    $userProvider = new InMemoryUserProvider(
        array(
            'admin' => array(
                // le mot de passe est "foo"
                'password' => '5FZ2Z8QIkA7UTZ4BYkoC+GsReLf569mSKDsfods6LYQ8t+a8EW9oaircfMpmaLbPBh4FOBiiFyLfuZmTSUwzZg==',
                'roles'    => array('ROLE_ADMIN'),
            ),
        )
    );

    // pour certains contrôles supplémentaires : est-ce que le compte est activé, bloqué, expiré etc.?
    $userChecker = new UserChecker();

    // un tableau d'encodeurs (voir ci-dessous)
    $encoderFactory = new EncoderFactory(...);

    $provider = new DaoAuthenticationProvider(
        $userProvider,
        $userChecker,
        'secured_area',
        $encoderFactory
    );

    $provider->authenticate($unauthenticatedToken);

.. note::

    L'exemple ci-dessus explique l'utilisation du fournisseur d'utilisateur
    "in-memory", mais vous pouvez utiliser n'importe quel fournisseur d'utilisateur,
    du moment qu'il implémente l'interface
    :class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface`.
    Il est également possible de laisser plusieurs fournisseurs d'utilisateurs
    essayer de trouver les informations de l'utilisateur, en employant le
    :class:`Symfony\\Component\\Security\\Core\\User\\ChainUserProvider`.

La fabrique d'encodeur de mots de passes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La classe :class:`Symfony\\Component\\Security\\Core\\Authentication\\Provider\\DaoAuthenticationProvider`
utilise une fabrique d'encodeur pour créer un encodeur de mots de passes pour un
type donné d'utilisateur. Cela vous permet d'utiliser différentes stratégies
d'encodage pour différents types d'utilisateurs. Par défaut, la classe
:class:`Symfony\\Component\\Security\\Core\\Encoder\\EncoderFactory` reçoit
un tableau d'encodeurs ::

    use Symfony\Component\Security\Core\Encoder\EncoderFactory;
    use Symfony\Component\Security\Core\Encoder\MessageDigestPasswordEncoder;

    $defaultEncoder = new MessageDigestPasswordEncoder('sha512', true, 5000);
    $weakEncoder = new MessageDigestPasswordEncoder('md5', true, 1);

    $encoders = array(
        'Symfony\\Component\\Security\\Core\\User\\User' => $defaultEncoder,
        'Acme\\Entity\\LegacyUser'                       => $weakEncoder,

        // ...
    );

    $encoderFactory = new EncoderFactory($encoders);

Chaque encodeur devrait implémenter l'interface
:class:`Symfony\\Component\\Security\\Core\\Encoder\\PasswordEncoderInterface` ou
être un tableau avec une ``class`` et une clé ``arguments``, ce qui permet
à l'usine d'encodeur de construire un encodeur seulement quand c'est nécessaire.

Créer un encodeur de mots de passes personnalisé
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il y a de nombreux encodeurs de mots de passes fournis avec le composant. Mais si
vous avez besoin du votre, vous devez simplement suivre ces règles :

#. La classe doit implémenter l'interface :class:`Symfony\\Component\\Security\\Core\\Encoder\\PasswordEncoderInterface`;

#. ``$this->checkPasswordLength($raw);`` doit être la première instruction
   executée dans les méthodes ``encodePassword()`` et ``isPasswordValid()`` (voir `CVE-2013-5750`_).

Utiliser les encodeurs de mots de passes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Lorsque la méthode :method:`Symfony\\Component\\Security\\Core\\Encoder\\EncoderFactory::getEncoder`
de l'usine d'encodeur de mots de passes est appelée avec l'objet utilisateur comme
premier argument, elle retournera un encodeur de type
:class:`Symfony\\Component\\Security\\Core\\Encoder\\PasswordEncoderInterface` qui devrait
être utilisé pour encoder le mot de passe de cet utilisateur ::

    // fetch a user of type Acme\Entity\LegacyUser
    $user = ...

    $encoder = $encoderFactory->getEncoder($user);

    // retournera $weakEncoder (voir plus haut)

    $encodedPassword = $encoder->encodePassword($password, $user->getSalt());

    // vérifie si le mot de passe est valide :

    $validPassword = $encoder->isPasswordValid(
        $user->getPassword(),
        $password,
        $user->getSalt());

.. _`CVE-2013-5750`: http://symfony.com/blog/cve-2013-5750-security-issue-in-fosuserbundle-login-form
