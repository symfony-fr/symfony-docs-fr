.. index::
   single: Sécurité, Voteurs

Comment implémenter votre propre Voteur pour ajouter des Adresses IP sur une liste noire
========================================================================================

Le composant de sécurité de Symfony2 fournit plusieurs couches pour authentifier
les utilisateurs. L'une de ces couches est appelée un `voteur`. Un voteur est
une classe dédiée qui vérifie si l'utilisateur possède les droits nécessaires
pour se connecter à l'application. Par exemple, Symfony2 fournit une couche
qui vérifie si l'utilisateur est totalement authentifié ou s'il possède quelques
rôles attendus.

Il est parfois utile de créer un voteur personnalisé pour gérer un cas spécifique
non-géré par le framework. Dans cette section, vous allez apprendre comment créer
un voteur qui vous permettra d'ajouter des utilisateurs sur une liste noire suivant
leur adresse IP.

L'Interface Voter
-----------------

Un voteur personnalisé doit implémenter
:class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface`,
qui requiert les trois méthodes suivantes :

.. code-block:: php

    interface VoterInterface
    {
        function supportsAttribute($attribute);
        function supportsClass($class);
        function vote(TokenInterface $token, $object, array $attributes);
    }


La méthode ``supportsAttribute()`` est utilisée pour vérifier si le voteur
supporte l'attribut de l'utilisateur donné (i.e. un rôle, une ACL, etc.).

La méthode ``supportsClass()`` est utilisée pour vérifier si le voteur
supporte la classe du token de l'utilisateur courant.

La méthode ``vote()`` doit implémenter la logique métier qui vérifie si oui
ou non un utilisateur est autorisé à accéder à l'application. Cette méthode
doit retourner l'une des valeurs suivantes :

* ``VoterInterface::ACCESS_GRANTED``: L'utilisateur est autorisé à accéder à l'application
* ``VoterInterface::ACCESS_ABSTAIN``: Le voteur ne peut pas décider si l'utilisateur est
  oui ou non autorisé à accéder à l'application
* ``VoterInterface::ACCESS_DENIED``: L'utilisateur n'est pas autorisé à accéder à
  l'application

Dans cet exemple, nous allons vérifier si l'adresse IP de l'utilisateur correspond
à l'une des adresses de la liste noire. Si c'est le cas, nous retournerons
``VoterInterface::ACCESS_DENIED``, sinon nous retournerons
``VoterInterface::ACCESS_ABSTAIN`` comme le but de ce voteur est uniquement de
refuser l'accès, et non pas de l'autoriser.

Créer un Voteur personnalisé
----------------------------

Pour ajouter un utilisateur sur la liste noire en se basant sur son IP, nous
pouvons utiliser le service ``request`` et comparer l'adresse IP avec un
ensemble d'adresses IP de la liste noire :

.. code-block:: php

    namespace Acme\DemoBundle\Security\Authorization\Voter;

    use Symfony\Component\DependencyInjection\ContainerInterface;
    use Symfony\Component\Security\Core\Authorization\Voter\VoterInterface;
    use Symfony\Component\Security\Core\Authentication\Token\TokenInterface;

    class ClientIpVoter implements VoterInterface
    {
        public function __construct(ContainerInterface $container, array $blacklistedIp = array())
        {
            $this->container     = $container;
            $this->blacklistedIp = $blacklistedIp;
        }

        public function supportsAttribute($attribute)
        {
            // nous n'allons pas vérifier l'attribut de l'utilisateur, alors nous retournons true
            return true;
        }

        public function supportsClass($class)
        {
            // notre voteur supporte tous les types de classes de token, donc nous retournons true
            return true;
        }

        function vote(TokenInterface $token, $object, array $attributes)
        {
            $request = $this->container->get('request');
            if (in_array($request->getClientIp(), $this->blacklistedIp)) {
                return VoterInterface::ACCESS_DENIED;
            }

            return VoterInterface::ACCESS_ABSTAIN;
        }
    }

C'est tout ! Votre voteur est terminé. La prochaine étape est d'injecter
le voteur dans la couche de sécurité. Cela peut être effectué facilement
à l'aide du conteneur de service.

Déclarer le Voteur comme Service
--------------------------------

Pour injecter le voteur dans la couche de sécurité, nous devons le déclarer
en tant que service, et le tagger comme un « security.voter » :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/AcmeBundle/Resources/config/services.yml

        services:
            security.access.blacklist_voter:
                class:      Acme\DemoBundle\Security\Authorization\Voter\ClientIpVoter
                arguments:  [@service_container, [123.123.123.123, 171.171.171.171]]
                public:     false
                tags:
                    -       { name: security.voter }

    .. code-block:: xml

        <!-- src/Acme/AcmeBundle/Resources/config/services.xml -->

        <service id="security.access.blacklist_voter"
                 class="Acme\DemoBundle\Security\Authorization\Voter\ClientIpVoter" public="false">
            <argument type="service" id="service_container" strict="false" />
            <argument type="collection">
                <argument>123.123.123.123</argument>
                <argument>171.171.171.171</argument>
            </argument>
            <tag name="security.voter" />
        </service>

    .. code-block:: php

        // src/Acme/AcmeBundle/Resources/config/services.php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $definition = new Definition(
            'Acme\DemoBundle\Security\Authorization\Voter\ClientIpVoter',
            array(
                new Reference('service_container'),
                array('123.123.123.123', '171.171.171.171'),
            ),
        );
        $definition->addTag('security.voter');
        $definition->setPublic(false);

        $container->setDefinition('security.access.blacklist_voter', $definition);

.. tip::

   Soyez sûr d'importer ce fichier de configuration depuis le fichier de configuration
   de votre application principale (par exemple : ``app/config/config.yml``). Pour plus
   d'informations, voyez :ref:`service-container-imports-directive`. Pour en savoir plus
   concernant la définition de services en général, voyez le chapitre
   :doc:`/book/service_container`.

Changer la Stratégie de Décision d'Accès
----------------------------------------

Afin que votre nouveau voteur soit utilisé, nous devons changer la stratégie de
décision d'accès par défaut, qui d'habitude autorise l'accès si *quelconque*
voteur autorise l'accès.

Dans notre cas, nous allons choisir la stratégie ``unanimous``. Contrairement
à la stratégie par défaut ``affirmative``, avec la stratégie ``unanimous``, si
seulement un voteur refuse l'accès (par exemple : le ``ClientIpVoter``), alors
l'accès n'est pas autorisé pour l'utilisateur final.

Pour faire cela, outrepassez la section par défaut ``access_decision_manager``
du fichier de configuration de votre application avec le code suivant.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            access_decision_manager:
                # La valeur de « Strategy » peut être : affirmative, unanimous ou consensus
                strategy: unanimous

C'est tout ! Maintenant, lors de la décision de savoir si oui ou non un utilisateur
devrait avoir accès, le nouveau voteur va refuser l'accès à quiconque possédant une IP
qui se trouve dans la liste noire.