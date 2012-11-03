.. index::
   single: Dependency Injection; Tags

Travailler avec des Services Taggés
===================================

Les tags sont des chaînes de caractères génériques (accompagnées de quelques
options) qui peuvent être appliquées à n'importe quel service. Les tags en
eux-mêmes n'altèrent en rien la fonctionnalité de vos services. Mais si vous
le décidez, vous pouvez demander à un constructeur de conteneur de vous
donner la liste de tous les services étant taggés avec un tag spécifique.
Cela est utile dans les passes de compilation où vous pouvez trouver ces
services et les utiliser ou les modifier d'une manière spécifique.

Par exemple, si vous utilisez Swift Mailer, vous pourriez imaginer que vous
souhaitez implémenter une « chaîne de transport », qui est une collection de
classes implémentant ``\Swift_Transport``. En utilisant la chaîne, vous allez
vouloir que Swift Mailer essaye plusieurs manières de transporter le message
jusqu'à ce que l'une d'entre elle fonctionne.

Pour commencer, définissez la classe ``TransportChain``::

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

Puis, définissez la chaîne en tant que service :

.. configuration-block::

    .. code-block:: yaml

        parameters:
            acme_mailer.transport_chain.class: TransportChain

        services:
            acme_mailer.transport_chain:
                class: "%acme_mailer.transport_chain.class%"

    .. code-block:: xml

        <parameters>
            <parameter key="acme_mailer.transport_chain.class">TransportChain</parameter>
        </parameters>

        <services>
            <service id="acme_mailer.transport_chain" class="%acme_mailer.transport_chain.class%" />
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('acme_mailer.transport_chain.class', 'TransportChain');

        $container->setDefinition('acme_mailer.transport_chain', new Definition('%acme_mailer.transport_chain.class%'));

Définir des services avec un tag personnalisé
---------------------------------------------

Maintenant, vous voulez peut être que plusieurs classes ``\Swift_Transport`` soient
instanciées et ajoutées à la chaîne automatiquement en utilisant la méthode
``addTransport()``. Par exemple, vous pouvez ajouter les transports
suivants en tant que services :

.. configuration-block::

    .. code-block:: yaml

        services:
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

        <service id="acme_mailer.transport.smtp" class="\Swift_SmtpTransport">
            <argument>%mailer_host%</argument>
            <tag name="acme_mailer.transport" />
        </service>

        <service id="acme_mailer.transport.sendmail" class="\Swift_SendmailTransport">
            <tag name="acme_mailer.transport" />
        </service>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definitionSmtp = new Definition('\Swift_SmtpTransport', array('%mailer_host%'));
        $definitionSmtp->addTag('acme_mailer.transport');
        $container->setDefinition('acme_mailer.transport.smtp', $definitionSmtp);

        $definitionSendmail = new Definition('\Swift_SendmailTransport');
        $definitionSendmail->addTag('acme_mailer.transport');
        $container->setDefinition('acme_mailer.transport.sendmail', $definitionSendmail);

Notez qu'un tag nommé ``acme_mailer.transport`` a été attribué à chacun.
C'est le tag personnalisé que vous allez utiliser dans votre passe de
compilateur. La passe de compilateur est ce qui donne un sens à ce tag.

Créer une ``CompilerPass`` (« Passe de Compilateur » en français)
-----------------------------------------------------------------

Votre passe de compilateur peut maintenant interroger le conteneur pour
n'importe quel service ayant le tag personnalisé::

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
    use Symfony\Component\DependencyInjection\Reference;

    class TransportCompilerPass implements CompilerPassInterface
    {
        public function process(ContainerBuilder $container)
        {
            if (!$container->hasDefinition('acme_mailer.transport_chain')) {
                return;
            }

            $definition = $container->getDefinition(
                'acme_mailer.transport_chain'
            );

            $taggedServices = $container->findTaggedServiceIds(
                'acme_mailer.transport' 
            ); 
            foreach ($taggedServices as $id => $attributes) {
                $definition->addMethodCall(
                    'addTransport',
                    array(new Reference($id))
                );
            }
        }
    }

La méthode ``process()`` vérifie l'existence du service ``acme_mailer.transport_chain``,
puis recherche tous les services taggés avec ``acme_mailer.transport``. Elle ajoute
un appel à ``addTransport()`` à la définition du service ``acme_mailer.transport_chain``
pour chaque service ``acme_mailer.transport`` qu'elle trouve. Le premier argument
de chacun de ces appels sera le service de transport d'email lui-même.

Enregistrer la passe dans le Conteneur
--------------------------------------

Vous avez aussi besoin d'enregistrer la passe dans le conteneur ; elle
sera ensuite exécutée lorsque le conteneur sera compilé::

    use Symfony\Component\DependencyInjection\ContainerBuilder;

    $container = new ContainerBuilder();
    $container->addCompilerPass(new TransportCompilerPass);

.. note::

    Les passes de compilateur sont enregistrées différemment si vous
    utilisez le framework full stack. Lisez :doc:`/cookbook/service_container/compiler_passes`
    pour plus de détails.

Ajouter des attributs additionnels aux tags
-------------------------------------------

Quelquefois, vous avez besoin d'informations additionnelles à propos de
chaque service qui est taggé avec votre tag. Par exemple, vous pourriez
vouloir ajouter un alias pour chaque TransportChain.

Pour commencer, changez la classe ``TransportChain``::

    class TransportChain
    {
        private $transports;

        public function __construct()
        {
            $this->transports = array();
        }

        public function addTransport(\Swift_Transport $transport, $alias)
        {
            $this->transports[$alias] = $transport;
        }

        public function getTransport($alias)
        {
            if (array_key_exists($alias, $this->transports)) {
               return $this->transports[$alias];
            }
            else {
               return null;
            }
        }
    }

Comme vous pouvez le voir, lorsque ``addTransport`` est appelée, elle ne prend pas
que l'objet ``Swift_Transport``, mais aussi un alias sous forme de chaîne de
caractères pour ce transport. Donc, comment pouvez-vous autoriser chaque transport
taggé à fournir aussi un alias ?

Pour répondre à cette question, changez la déclaration du service comme suit :

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_mailer.transport.smtp:
                class: \Swift_SmtpTransport
                arguments:
                    - %mailer_host%
                tags:
                    -  { name: acme_mailer.transport, alias: foo }
            acme_mailer.transport.sendmail:
                class: \Swift_SendmailTransport
                tags:
                    -  { name: acme_mailer.transport, alias: bar }


    .. code-block:: xml

        <service id="acme_mailer.transport.smtp" class="\Swift_SmtpTransport">
            <argument>%mailer_host%</argument>
            <tag name="acme_mailer.transport" alias="foo" />
        </service>

        <service id="acme_mailer.transport.sendmail" class="\Swift_SendmailTransport">
            <tag name="acme_mailer.transport" alias="bar" />
        </service>

Notez que vous avez ajouté une clé générique ``alias`` au tag. Pour
utiliser cette dernière, mettez à jour votre compilateur::

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
    use Symfony\Component\DependencyInjection\Reference;

    class TransportCompilerPass implements CompilerPassInterface
    {
        public function process(ContainerBuilder $container)
        {
            if (!$container->hasDefinition('acme_mailer.transport_chain')) {
                return;
            }

            $definition = $container->getDefinition(
                'acme_mailer.transport_chain'
            );

            $taggedServices = $container->findTaggedServiceIds(
                'acme_mailer.transport'
            );
            foreach ($taggedServices as $id => $tagAttributes) {
                foreach ($tagAttributes as $attributes) {
                    $definition->addMethodCall(
                        'addTransport',
                        array(new Reference($id), $attributes["alias"])
                    );
                }
            }
        }
    }

Ici, la partie importante est la variable ``$attributes``. Comme vous pouvez
utiliser le même tag plusieurs fois avec le même service (par exemple : vous
pourriez, en théorie, tagger le même service cinq fois avec le tag
``acme_mailer.transport``), ``$attributes`` est un tableau contenant l'information
du tag pour chaque tag de ce service.
