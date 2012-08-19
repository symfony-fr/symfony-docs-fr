.. index::
   single: Sécurité; Fournisseur d'authentification personnalisé

Comment créer un Fournisseur d'Authentification Personnalisé
============================================================

Si vous avez lu le chapitre :doc:`/book/security`, vous comprenez la
distinction que Symfony2 fait entre authentification et autorisation dans
l'implémentation de la sécurité. Ce chapitre traite des classes noyaux
impliquées dans le processus d'authentification, et de l'implémentation
d'un fournisseur d'authentification personnalisé. Comme l'authentification et
l'autorisation sont des concepts séparés, cette extension sera indépendante
du « fournisseur d'utilisateur », et fonctionnera avec les fournisseurs
d'utilisateur de votre application, qu'ils soient stockés en mémoire,
dans une base de données, ou n'importe où ailleurs.

A la rencontre de WSSE
----------------------

Le chapitre suivant démontre comment créer un fournisseur d'authentification
personnalisé pour une authentification WSSE. Le protocole de sécurité
pour WSSE fournit plusieurs avantages concernant la sécurité :

1. Encryptage du Nom d'utilisateur / Mot de passe
2. Sécurité contre les attaques de type « replay »
3. Aucune configuration de serveur web requise

WSSE est très utile pour sécuriser des services web, qu'ils soient SOAP
ou REST.

Il existe de nombreuses et très bonnes documentations sur `WSSE`_, mais
cet article ne va pas se focaliser sur le protocole de sécurité, mais
plutôt sur la manière dont un protocole personnalisé peut être ajouté
à votre application Symfony2. La base de WSSE est qu'un en-tête de requête
est vérifié pour y trouver des informations de connexions encryptées en
utilisant un « timestamp » et un « `nonce`_ », et est authentifié pour
l'utilisateur demandé en utilisant un « digest » du mot de passe.

.. note::

    WSSE supporte aussi la validation par clé d'application, qui est utile
    pour les services web, mais qui est hors-sujet dans ce chapitre.

Le Token
--------

Le rôle du token dans le contexte de sécurité de Symfony2 est important.
Un token représente les données d'authentification de l'utilisateur
présentes dans la requête. Une fois qu'une requête est authentifiée, le
token conserve les données de l'utilisateur, et délivre ces données au
travers du contexte de sécurité. Premièrement, nous allons créer notre
classe token. Cela va permettre de passer toutes les informations
pertinentes à notre fournisseur d'authentification.

.. code-block:: php

    // src/Acme/DemoBundle/Security/Authentication/Token/WsseUserToken.php
    namespace Acme\DemoBundle\Security\Authentication\Token;

    use Symfony\Component\Security\Core\Authentication\Token\AbstractToken;

    class WsseUserToken extends AbstractToken
    {
        public $created;
        public $digest;
        public $nonce;
   
        public function __construct(array $roles = array())
        {
            parent::__construct($roles);

            // Si l'utilisateur a des rôles, on le considère comme authentifié
            $this->setAuthenticated(count($roles) > 0);
        }

        public function getCredentials()
        {
            return '';
        }
    }

.. note::

    La classe ``WsseUserToken`` étend la classe du composant de sécurité
    :class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\AbstractToken`,
    qui fournit la fonctionnalité basique du token. Implémentez l'interface
    :class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\TokenInterface`
    dans chaque classe que vous souhaitez utiliser comme token.

Le Listener
-----------

Ensuite, vous avez besoin d'un listener pour « écouter » le contexte de
sécurité. Le listener est chargé de transmettre les requêtes au pare-feu et
d'appeler le fournisseur d'authentification. Un listener doit être une instance
de :class:`Symfony\\Component\\Security\\Http\\Firewall\\ListenerInterface`.
Un listener de sécurité devrait gérer l'évènement
:class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseEvent`, et définir
un token authentifié dans le contexte de sécurité en cas de succès.

.. code-block:: php

    // src/Acme/DemoBundle/Security/Firewall/WsseListener.php
    namespace Acme\DemoBundle\Security\Firewall;

    use Symfony\Component\HttpFoundation\Response;
    use Symfony\Component\HttpKernel\Event\GetResponseEvent;
    use Symfony\Component\Security\Http\Firewall\ListenerInterface;
    use Symfony\Component\Security\Core\Exception\AuthenticationException;
    use Symfony\Component\Security\Core\SecurityContextInterface;
    use Symfony\Component\Security\Core\Authentication\AuthenticationManagerInterface;
    use Symfony\Component\Security\Core\Authentication\Token\TokenInterface;
    use Acme\DemoBundle\Security\Authentication\Token\WsseUserToken;

    class WsseListener implements ListenerInterface
    {
        protected $securityContext;
        protected $authenticationManager;

        public function __construct(SecurityContextInterface $securityContext, AuthenticationManagerInterface $authenticationManager)
        {
            $this->securityContext = $securityContext;
            $this->authenticationManager = $authenticationManager;
        }

        public function handle(GetResponseEvent $event)
        {
            $request = $event->getRequest();

            if ($request->headers->has('x-wsse')) {

                $wsseRegex = '/UsernameToken Username="([^"]+)", PasswordDigest="([^"]+)", Nonce="([^"]+)", Created="([^"]+)"/';

                if (preg_match($wsseRegex, $request->headers->get('x-wsse'), $matches)) {
                    $token = new WsseUserToken();
                    $token->setUser($matches[1]);

                    $token->digest   = $matches[2];
                    $token->nonce    = $matches[3];
                    $token->created  = $matches[4];

                    try {
                        $returnValue = $this->authenticationManager->authenticate($token);

                        if ($returnValue instanceof TokenInterface) {
                            return $this->securityContext->setToken($returnValue);
                        } elseif ($returnValue instanceof Response) {
                            return $event->setResponse($returnValue);
                        }
                    } catch (AuthenticationException $e) {
                        // vous pourriez logger quelque chose ici
                    }
                }
            }

            $response = new Response();
            $response->setStatusCode(403);
            $event->setResponse($response);
        }
    }

Ce listener vérifie l'en-tête `X-WSSE` attendu dans la réponse, fait correspondre
la valeur retournée pour l'information WSSE attendue, crée un token utilisant
cette information, et passe le token au gestionnaire d'authentification. Si la
bonne information n'est pas fournie, ou si le gestionnaire d'authentification
lance une
:class:`Symfony\\Component\\Security\\Core\\Exception\\AuthenticationException`,
alors une réponse 403 est retournée.

.. note::

    Une classe non utilisée ci-dessus, la classe
    :class:`Symfony\\Component\\Security\\Http\\Firewall\\AbstractAuthenticationListener`,
    est une classe de base très utile qui fournit certaines fonctionnalités communes pour
    les extensions de sécurité. Ceci inclut le fait de maintenir le token dans la session, fournir
    des gestionnaires en cas de succès/échec, des URLs de formulaire de login, et plus
    encore. Comme WSSE ne requiert pas de maintenir les sessions d'authentification ou
    les formulaires de login, cela ne sera pas utilisé dans cet exemple.

Le Fournisseur d'Authentification
---------------------------------

Le fournisseur d'authentification va effectuer la vérification du
``WsseUserToken``. C'est-à-dire que le fournisseur va vérifier que la valeur
de l'en-tête ``Created`` est valide dans les cinq minutes, que la valeur de
l'en-tête ``Nonce`` est unique dans les cinq minutes, et que la valeur de
l'en-tête ``PasswordDigest`` correspond au mot de passe de l'utilisateur.

.. code-block:: php

    // src/Acme/DemoBundle/Security/Authentication/Provider/WsseProvider.php
    namespace Acme\DemoBundle\Security\Authentication\Provider;

    use Symfony\Component\Security\Core\Authentication\Provider\AuthenticationProviderInterface;
    use Symfony\Component\Security\Core\User\UserProviderInterface;
    use Symfony\Component\Security\Core\Exception\AuthenticationException;
    use Symfony\Component\Security\Core\Exception\NonceExpiredException;
    use Symfony\Component\Security\Core\Authentication\Token\TokenInterface;
    use Acme\DemoBundle\Security\Authentication\Token\WsseUserToken;

    class WsseProvider implements AuthenticationProviderInterface
    {
        private $userProvider;
        private $cacheDir;

        public function __construct(UserProviderInterface $userProvider, $cacheDir)
        {
            $this->userProvider = $userProvider;
            $this->cacheDir     = $cacheDir;
        }

        public function authenticate(TokenInterface $token)
        {
            $user = $this->userProvider->loadUserByUsername($token->getUsername());

            if ($user && $this->validateDigest($token->digest, $token->nonce, $token->created, $user->getPassword())) {
                $authenticatedToken = new WsseUserToken($user->getRoles());
                $authenticatedToken->setUser($user);

                return $authenticatedToken;
            }

            throw new AuthenticationException('The WSSE authentication failed.');
        }

        protected function validateDigest($digest, $nonce, $created, $secret)
        {
            // Expire le timestamp après 5 minutes
            if (time() - strtotime($created) > 300) {
                return false;
            }

            // Valide que le nonce est unique dans les 5 minutes
            if (file_exists($this->cacheDir.'/'.$nonce) && file_get_contents($this->cacheDir.'/'.$nonce) + 300 > time()) {
                throw new NonceExpiredException('Previously used nonce detected');
            }
            file_put_contents($this->cacheDir.'/'.$nonce, time());

            // Valide le Secret
            $expected = base64_encode(sha1(base64_decode($nonce).$created.$secret, true));

            return $digest === $expected;
        }

        public function supports(TokenInterface $token)
        {
            return $token instanceof WsseUserToken;
        }
    }

.. note::

    La classe :class:`Symfony\\Component\\Security\\Core\\Authentication\\Provider\\AuthenticationProviderInterface`
    requiert une méthode ``authenticate`` sur le token de l'utilisateur ainsi
    qu'une méthode ``supports``, qui dit au gestionnaire d'authentification
    d'utiliser ou non ce fournisseur pour le token donné. Dans le cas de
    fournisseurs multiples, le gestionnaire d'authentification se déplacera
    alors jusqu'au prochain fournisseur dans la liste.

La Factory (« l'usine » en français)
------------------------------------

Vous avez créé un token personnalisé, un listener personnalisé, et un
fournisseur personnalisé. Maintenant, vous avez besoin de les relier tous
ensemble. Comment mettez-vous votre fournisseur à disposition de votre
configuration de sécurité ? La réponse est : en utilisant une ``factory``.
Une « factory » est là où vous intervenez dans le composant de sécurité en
lui disant le nom de votre fournisseur ainsi que toutes ses options de
configuration disponibles. Tout d'abord, vous devez créer une
classe qui implémente
:class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\SecurityFactoryInterface`.

.. code-block:: php

    // src/Acme/DemoBundle/DependencyInjection/Security/Factory/WsseFactory.php
    namespace Acme\DemoBundle\DependencyInjection\Security\Factory;

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Reference;
    use Symfony\Component\DependencyInjection\DefinitionDecorator;
    use Symfony\Component\Config\Definition\Builder\NodeDefinition;
    use Symfony\Bundle\SecurityBundle\DependencyInjection\Security\Factory\SecurityFactoryInterface;

    class WsseFactory implements SecurityFactoryInterface
    {
        public function create(ContainerBuilder $container, $id, $config, $userProvider, $defaultEntryPoint)
        {
            $providerId = 'security.authentication.provider.wsse.'.$id;
            $container
                ->setDefinition($providerId, new DefinitionDecorator('wsse.security.authentication.provider'))
                ->replaceArgument(0, new Reference($userProvider))
            ;

            $listenerId = 'security.authentication.listener.wsse.'.$id;
            $listener = $container->setDefinition($listenerId, new DefinitionDecorator('wsse.security.authentication.listener'));

            return array($providerId, $listenerId, $defaultEntryPoint);
        }

        public function getPosition()
        {
            return 'pre_auth';
        }

        public function getKey()
        {
            return 'wsse';
        }

        public function addConfiguration(NodeDefinition $node)
        {        
        }
    }

La :class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\SecurityFactoryInterface`
requiert les méthodes suivantes :

* la méthode ``create``, qui ajoute le listener et le fournisseur
  d'authentification au conteneur d'Injection de Dépendances pour
  le contexte de sécurité approprié ;

* la méthode ``getPosition``, qui doit être de type ``pre_auth``, ``form``,
  ``http`` et ``remember_me`` et qui définit le moment auquel le fournisseur
  est appelé ;

* la méthode ``getKey`` qui définit la clé de configuration utilisée pour
  référencer le fournisseur ;

* la méthode ``addConfiguration``, qui est utilisée pour définir les
  options de configuration en dessous de la clé de configuration dans
  votre configuration de sécurité.
  Comment définir les options de configuration sera expliqué plus tard dans
  ce chapitre.

.. note::

    Une classe non utilisée dans cet exemple,
    :class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\AbstractFactory`,
    est une classe de base très utile qui fournit certaines fonctionnalités
    communes pour les « factories » de sécurité. Cela pourrait être utile
    lors de la définition d'un fournisseur d'authentification d'un type
    différent.

Maintenant que vous avez créé une classe factory, la clé ``wsse`` peut être
utilisée comme un pare-feu dans votre configuration de sécurité.

.. note::

    Vous vous demandez peut-être « pourquoi avons-nous besoin d'une classe
    factory spéciale pour ajouter des listeners et fournisseurs à un
    conteneur d'injection de dépendances ? ». Ceci est une très bonnne
    question. La raison est que vous pouvez utiliser votre pare-feu
    plusieurs fois afin de sécuriser plusieurs parties de votre application.
    Grâce à cela, chaque fois que votre pare-feu sera utilisé, un nouveau
    service sera créé dans le conteneur d'injection de dépendances.
    La factory est ce qui crée ces nouveaux services.

Configuration
-------------

Il est temps de voir votre fournisseur d'authentification en action. Vous
allez avoir besoin de faire quelques petites choses afin qu'il fonctionne.
La première chose est d'ajouter les services ci-dessus dans le conteneur
d'injection de dépendances. Votre classe factory ci-dessus fait référence
à des IDs de service qui n'existent pas encore :
``wsse.security.authentication.provider`` et
``wsse.security.authentication.listener``. Il est temps de définir ces
services.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/config/services.yml
        services:
          wsse.security.authentication.provider:
            class:  Acme\DemoBundle\Security\Authentication\Provider\WsseProvider
            arguments: ['', %kernel.cache_dir%/security/nonces]

          wsse.security.authentication.listener:
            class:  Acme\DemoBundle\Security\Firewall\WsseListener
            arguments: [@security.context, @security.authentication.manager]


    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/config/services.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

           <services>
               <service id="wsse.security.authentication.provider"
                 class="Acme\DemoBundle\Security\Authentication\Provider\WsseProvider" public="false">
                   <argument /> <!-- User Provider -->
                   <argument>%kernel.cache_dir%/security/nonces</argument>
               </service>

               <service id="wsse.security.authentication.listener"
                 class="Acme\DemoBundle\Security\Firewall\WsseListener" public="false">
                   <argument type="service" id="security.context"/>
                   <argument type="service" id="security.authentication.manager" />
               </service>
           </services>
        </container>

    .. code-block:: php

        // src/Acme/DemoBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->setDefinition('wsse.security.authentication.provider',
          new Definition(
            'Acme\DemoBundle\Security\Authentication\Provider\WsseProvider',
            array('', '%kernel.cache_dir%/security/nonces')
        ));

        $container->setDefinition('wsse.security.authentication.listener',
          new Definition(
            'Acme\DemoBundle\Security\Firewall\WsseListener', array(
              new Reference('security.context'),
              new Reference('security.authentication.manager'))
        ));

Maintenant que vos services sont définis, informez votre contexte de
sécurité de l'existence de votre factory dans la classe de votre bundle :


.. versionadded:: 2.1
    Avant 2.1, la factory ci-dessous était ajoutée via le fichier
    ``security.yml`` à la place.

.. code-block:: php

    // src/Acme/DemoBundle/AcmeDemoBundle.php
    namespace Acme\DemoBundle;

    use Acme\DemoBundle\DependencyInjection\Security\Factory\WsseFactory;
    use Symfony\Component\HttpKernel\Bundle\Bundle;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    class AcmeDemoBundle extends Bundle
    {
        public function build(ContainerBuilder $container)
        {
            parent::build($container);

            $extension = $container->getExtension('security');
            $extension->addSecurityListenerFactory(new WsseFactory());
        }
    }

Vous avez terminé ! Vous pouvez maintenant définir des parties de votre
application comme étant sous la protection de WSSE.

.. code-block:: yaml

    security:
        firewalls:
            wsse_secured:
                pattern:   /api/.*
                wsse:      true

Félicitations ! Vous avez écrit votre tout premier fournisseur d'authentification
de sécurité personnalisé !

Un Petit Extra
--------------

Que diriez-vous de rendre votre fournisseur d'authentification WSSE un peu
plus excitant ? Les possibilités sont sans fin. Voyons comment nous pouvons
apporter plus d'éclat à tout cela !

Configuration
~~~~~~~~~~~~~

Vous pouvez ajouter des options personnalisées sous la clé ``wsse`` de votre
configuration de sécurité. Par exemple, le temps alloué avant que l'en-tête
« Created » expire est, par défaut, 5 minutes. Rendez cela configurable, afin
que différents pares-feu puissent avoir des longueurs de « timeout » différentes.

Vous allez tout d'abord avoir besoin d'éditer ``WsseFactory`` puis ensuite
de définir la nouvelle option dans la méthode ``addConfiguration``.

.. code-block:: php

    class WsseFactory implements SecurityFactoryInterface
    {
        // ...

        public function addConfiguration(NodeDefinition $node)
        {
          $node
            ->children()
            ->scalarNode('lifetime')->defaultValue(300)
            ->end();
        }
    }

Maintenant, dans la méthode ``create`` de la factory, l'argument ``$config``
va contenir une clé « lifetime », déclarée à 5 minutes (300 secondes) à moins
qu'elle soit définie ailleurs dans la configuration. Passez cet argument à
votre fournisseur d'authentification afin qu'il l'utilise.

.. code-block:: php

    class WsseFactory implements SecurityFactoryInterface
    {
        public function create(ContainerBuilder $container, $id, $config, $userProvider, $defaultEntryPoint)
        {
            $providerId = 'security.authentication.provider.wsse.'.$id;
            $container
                ->setDefinition($providerId,
                  new DefinitionDecorator('wsse.security.authentication.provider'))
                ->replaceArgument(0, new Reference($userProvider))
                ->replaceArgument(2, $config['lifetime']);
            // ...
        }
        
        // ...
    }

.. note::

    Vous allez aussi avoir besoin d'ajouter un troisième argument à la
    configuration du service ``wsse.security.authentication.provider``,
    qui peut être vide, mais qui sera rempli avec la valeur « lifetime »
    dans la factory. La classe ``WsseProvider`` va maintenant avoir
    besoin d'accepter un troisième argument dans son constructeur - la
    valeur « lifetime » - qu'elle devrait utiliser à la place des 300
    secondes codées en dur. Ces deux étapes ne sont pas montrées ici.

La valeur « lifetime » de chaque requête wsse est maintenant configurable,
et peut être définie par quelconque valeur que ce soit par pare-feu.

.. code-block:: yaml

    security:
        firewalls:
            wsse_secured:
                pattern:   /api/.*
                wsse:      { lifetime: 30 }

Le reste dépend de vous ! N'importe quels autres points de configuration
peuvent être définis dans la factory et consommé ou passé à d'autres
classes dans le conteneur.

.. _`WSSE`: http://www.xml.com/pub/a/2003/12/17/dive.html
.. _`nonce`: http://en.wikipedia.org/wiki/Cryptographic_nonce