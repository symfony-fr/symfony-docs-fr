.. index::
   single: Console; Sending emails
   single: Console; Generating URLs

Comment générer des URLs et envoyer des emails depuis la Console
================================================================

Malheureusement, le contexte de la ligne de commande n'a pas "conscience" de
votre VirtualHost ou votre nom de domaine. Cela signifie que si vous générez
des URLs absolues à travers une Commande Console vous finirez sans doute par
obtenir quelque chose comme ``http://localhost/foo/bar``, ce qui n'est pas
très utile.

Pour corriger cela, vous devez configurer le "request context", qui est une
façon élégante de dire que vous avez besoin de configurer votre environnement
afin qu'il connaisse quelle URL il devrait utiliser à la génération d'URLs.

Il y a deux façons de configurer le request context : au niveau de l'application
ou via une commande.

Configurer le Request Contexte globalement
------------------------------------------

Pour configurer le Context Request - qui est utilisé par le générateur d'URL -
vous pouvez redéfinir les paramètres qu'il utilise comme valeur par défaut pour
changer l'host par défaut (localhost) et le scheme (http). Vous devez également
configurer le chemin de base si Symfony ne tourne pas à la racine de votre
serveur.

Notez que ceci n'impacte pas les URLs générées via les requêtes web normales,
puisqu'elles remplaces celles par défauts.

.. configuration-block::

    .. code-block:: yaml

        # app/config/parameters.yml
        parameters:
            router.request_context.host: example.org
            router.request_context.scheme: https
            router.request_context.base_url: my/path

    .. code-block:: xml

        <!-- app/config/parameters.xml -->
        <?xml version="1.0" encoding="UTF-8"?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <parameters>
                <parameter key="router.request_context.host">example.org</parameter>
                <parameter key="router.request_context.scheme">https</parameter>
                <parameter key="router.request_context.base_url">my/path</parameter>
            </parameters>
        </container>

    .. code-block:: php

        // app/config/config_test.php
        $container->setParameter('router.request_context.host', 'example.org');
        $container->setParameter('router.request_context.scheme', 'https');
        $container->setParameter('router.request_context.base_url', 'my/path');

Configurer le Request Context via la Command
--------------------------------------------

Pour le changer uniquement en une commande vous pouvez simplement récupérer
le Request Context depuis le service ``router`` et remplaçants ses réglages::

   // src/Acme/DemoBundle/Command/DemoCommand.php

   // ...
   class DemoCommand extends ContainerAwareCommand
   {
       protected function execute(InputInterface $input, OutputInterface $output)
       {
           $context = $this->getContainer()->get('router')->getContext();
           $context->setHost('example.com');
           $context->setScheme('https');
           $context->setBaseUrl('my/path');

           // ... votre code ici
       }
   }

Utiliser le Memory Spooling
---------------------------

Envoyer des e-mails depuis la commande console se fait de la même manière que
dans le cookbook :doc:`/cookbook/email/email` hormis le fait que le memory
spooling est utilisé.

En utilisant le memory spooling (consultez le cookbook :doc:`/cookbook/email/spool`
pour plus d'informations), vous devez savoir que c'est parce que Symfony gère la
commande console de manière particulière, que les e-mails ne sont pas envoyés
automatiquement. Vous devez prendre soin de néttoyer file vous même. Utiliser le code
suivant pour envoyer des emails depuis la commande console::

    $message = new \Swift_Message();

    // ... préparez le message

    $container = $this->getContainer();
    $mailer = $container->get('mailer');

    $mailer->send($message);

    // maintenant nettoyez la file manuellement
    $spool = $mailer->getTransport()->getSpool();
    $transport = $container->get('swiftmailer.transport.real');

    $spool->flushQueue($transport);

Une autre option est de créer un environnement qui ne serait utilisé
uniquement que par la commande console et utiliserait une autre méthode
de spooling.

.. note::

    S'occuper du spooling n'est uniquement nécessaie que si le memory spolling
    est utilisé. Si vous utilisez le ``file spooling`` (ou pas de spooling du tout),
    il n'est pas utile de nettoyer manuellement dans une commande.
