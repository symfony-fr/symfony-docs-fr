.. index::
   single: Console; Generating URLs

Comment générer des URL avec un hôte personnalisé en ligne de commande
======================================================================

Malheureusement, le contexte de la ligne de commande ne connait pas votre
VirtualHost ou votre nom de domaine. Cela signifie que si vous générez des
URLs absolues en ligne de commande, vous vous retrouverez probablement avec
quelque chose du genre ``http://localhost/foo/bar``, ce qui n'est pas très
utile.

Pour corriger cela, vous devrez configurer le « contexte requête » qui est
une manière étrange de dire que vous devrez configurer votre environnement
pour qu'il sache quelle URL il doit utiliser pour générer les URLs.

Il y a deux manières de configurer le contexte de la requête : au niveau
de l'application, ou dans la commande

Configurer le contexte de manière globale
-----------------------------------------

.. versionadded: 2.1

    Les paramètres host et scheme sont disponibles depuis Symfony 2.1

.. versionadded: 2.2

    Le paramètre base_url est disponible depuis Symfony 2.2

Pour configurer le contexte de la requête, qui utilisé par le générateur d'URL, vous
devrez définir les paramètres qu'il utilise comme valeurs par défaut pour changer
l'hôte par défaut (localhost) et le schéma (http). Notez que cela n'impacte pas les
URLs générées via les requêtes web normales, puisqu'elles surchargeront les valeurs
par défaut.

.. configuration-block::

    .. code-block:: yaml

        # app/config/parameters.yml
        parameters:
            router.request_context.host: example.org
            router.request_context.scheme: https

    .. code-block:: xml

        <!-- app/config/parameters.xml -->
        <?xml version="1.0" encoding="UTF-8"?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <parameters>
                <parameter key="router.request_context.host">example.org</parameter>
                <parameter key="router.request_context.scheme">https</parameter>
            </parameters>
        </container>

    .. code-block:: php

        // app/config/config_test.php
        $container->setParameter('router.request_context.host', 'example.org');
        $container->setParameter('router.request_context.scheme', 'https');

Configurer le contexte dans la commande
---------------------------------------

Pour le changer seulement dans une commande, vous pouvez simplement
récupérer le service Contexte de la requête et surcharger sa configuration::

    // src/Acme/DemoBundle/Command/DemoCommand.php

    // ...
    class DemoCommand extends ContainerAwareCommand
    {
        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $context = $this->getContainer()->get('router')->getContext();
            $context->setHost('example.com');
            $context->setScheme('https');

            // ... votre code ici
        }
    }
