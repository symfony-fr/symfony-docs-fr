.. index::
   single: Service Container; Tags

Comment utiliser les tags dans vos services
===========================================

De nombreux services du coeur de Symfony2 dépendent de tags permettant d'indiquer
quel services doivent être charger, de notifier des évènements, ou de permettre
d'autres traitements particuliers. Par exemple, Twig utilise le tag ``twig.extension``
pour charger les extensions ``extra``.

Vous pouvez aussi utiliser les tags dans vos propres bundles. Par exemple, vos
services implémentent une collection comprenant de nombreuses stratégies
alternatives qui seront successivement essayées jusqu'à ce que l'une d'elle 
réussisse.

Dans cet article, nous allons utiliser l'example de la "chaine de transport", une
collection de classe implémentant ``\Swift_Transport``. En utilisant cette chaine, 
la classe Swift mailer essaira différents protocoles, jusqu'à ce que l'un de
ceux-ci soit un succès. Nous nous concentrerons ici particulièrement sur 
l'injection de dépendance.

Pour commencer, définissons la classe ``TransportChain``::

    namespace Acme\MailerBundle;
    
    class TransportChain
    {
        private $transports;
    
        public function __construct()
        {
            $this->transports = array();
        }
    
        public function addTransport(\Swift_Transport  $transport)
        {
            $this->transports[] = $transport;
        }
    }

Définissons ensuite cette chaîne comme un service:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/MailerBundle/Resources/config/services.yml
        parameters:
            acme_mailer.transport_chain.class: Acme\MailerBundle\TransportChain
        
        services:
            acme_mailer.transport_chain:
                class: %acme_mailer.transport_chain.class%

    .. code-block:: xml

        <!-- src/Acme/MailerBundle/Resources/config/services.xml -->

        <parameters>
            <parameter key="acme_mailer.transport_chain.class">Acme\MailerBundle\TransportChain</parameter>
        </parameters>
    
        <services>
            <service id="acme_mailer.transport_chain" class="%acme_mailer.transport_chain.class%" />
        </services>
        
    .. code-block:: php
    
        // src/Acme/MailerBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        
        $container->setParameter('acme_mailer.transport_chain.class', 'Acme\MailerBundle\TransportChain');
        
        $container->setDefinition('acme_mailer.transport_chain', new Definition('%acme_mailer.transport_chain.class%'));

Définissons les services avec un tag personnalisé
-------------------------------------------------

Nous voulons à présent que de nombreuses instances de la classe 
``\Swift_Transport`` soient ajoutées automatiquement les unes à la suites des 
autres en utilisant la méthode ``addTransport()``. Nous définissons donc pour 
cela la configuration suivante:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/MailerBundle/Resources/config/services.yml
        services:
		    #...
            acme_mailer.transport.smtp:
                class: \Swift_SmtpTransport
                arguments:
                    - %mailer_host%
                tags:
                    -  { name: acme_mailer.transport }
            acme_mailer.transport.sendmail:
                class: \Swift_SendmailTransport
                tags:
                    -  { name: acme_mailer.transport }
    
    .. code-block:: xml

        <!-- src/Acme/MailerBundle/Resources/config/services.xml -->
        <service id="acme_mailer.transport.smtp" class="\Swift_SmtpTransport">
            <argument>%mailer_host%</argument>
            <tag name="acme_mailer.transport" />
        </service>
    
        <service id="acme_mailer.transport.sendmail" class="\Swift_SendmailTransport">
            <tag name="acme_mailer.transport" />
        </service>
        
    .. code-block:: php
    
        // src/Acme/MailerBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        
        $definitionSmtp = new Definition('\Swift_SmtpTransport', array('%mailer_host%'));
        $definitionSmtp->addTag('acme_mailer.transport');
        $container->setDefinition('acme_mailer.transport.smtp', $definitionSmtp);
        
        $definitionSendmail = new Definition('\Swift_SendmailTransport');
        $definitionSendmail->addTag('acme_mailer.transport');
        $container->setDefinition('acme_mailer.transport.sendmail', $definitionSendmail);

Notés les tags nommés "acme_mailer.transport". Nous voulons que le bundle reconnaissent
ces transports et les ajoutent à la chaîne de lui même. Nous commencerons donc
par ajouter une méthode ``build()`` à la classe ``AcmeMailerBundle``::

    namespace Acme\MailerBundle;
    
    use Symfony\Component\HttpKernel\Bundle\Bundle;
    use Symfony\Component\DependencyInjection\ContainerBuilder;
    
    use Acme\MailerBundle\DependencyInjection\Compiler\TransportCompilerPass;
    
    class AcmeMailerBundle extends Bundle
    {
        public function build(ContainerBuilder $container)
        {
            parent::build($container);
    
            $container->addCompilerPass(new TransportCompilerPass());
        }
    }

Créer un ``CompilerPass``
-------------------------

Vous allez indiquer une référence à la classes ``TransportCompilerPass``. Cette
classe s'assure que tous les services avec un tag ``acme_mailer.transport``
sont ajoutés à la class ``TransportChain`` en appelant la méthode ``addTransport()``::

    namespace Acme\MailerBundle\DependencyInjection\Compiler;
    
    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
    use Symfony\Component\DependencyInjection\Reference;
    
    class TransportCompilerPass implements CompilerPassInterface
    {
        public function process(ContainerBuilder $container)
        {
            if (false === $container->hasDefinition('acme_mailer.transport_chain')) {
                return;
            }
    
            $definition = $container->getDefinition('acme_mailer.transport_chain');
    
            foreach ($container->findTaggedServiceIds('acme_mailer.transport') as $id => $attributes) {
                $definition->addMethodCall('addTransport', array(new Reference($id)));
            }
        }
    }

La méthode ``process()``  vérifie l'existence du service ``acme_mailer.transport_chain``,
puis parcoure tous les services taggués ``acme_mailer.transport``. Elle ajoute à la 
définition du service ``acme_mailer.transport_chain`` un appel à ``addTransport()`` 
pour chaque service "acme_mailer.transport" trouvé. Le premier argument de chacun
de ces appels sera le service de transport de mail lui-même.

.. note::

    Par convention, les noms des tags sont composés par le nom du bundle (lettres
	minuscules avec des soulignements comme séparateurs), suivi par un point, et
	terminer par le nom réel du service, dans ce cas le tag "transport". Ainsi
	le bundle AcmeMailerBundle deviendra: ``acme_mailer.transport``.

La définition compilée du service
---------------------------------

Ajouter le ``compileur`` resultera en une génération automatique des lignes de
codes suivantes dans le container de service compilé. Si vous travaillez dans
l'environnement de développement, ouvrez le fichier ``app/cache/dev/appDevDebugProjectContainer.php``
et observer la méthode ``getTransportChainService()``. Elle devrait ressembler
à ceci::

    protected function getAcmeMailer_TransportChainService()
    {
        $this->services['acme_mailer.transport_chain'] = $instance = new \Acme\MailerBundle\TransportChain();

        $instance->addTransport($this->get('acme_mailer.transport.smtp'));
        $instance->addTransport($this->get('acme_mailer.transport.sendmail'));

        return $instance;
    }
