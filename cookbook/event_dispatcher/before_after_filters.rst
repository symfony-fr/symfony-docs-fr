.. index::
   single: Event Dispatcher

Comment mettre en place des filtres avant et après un processus donné
=====================================================================

Il est très commun, dans le développement d'application web, d'avoir besoin
qu'un bout de logique soit exécuté juste avant ou juste après vos actions
de contrôleur agissant comme des filtres ou des « hooks ».

Dans Symfony1, cela était effectué avec les méthodes « preExecute » et
« postExecute » ; la plupart des principaux « frameworks » ont des méthodes
similaires mais il n'y a rien de semblable dans Symfony2.
La bonne nouvelle est qu'il y a une bien meilleure manière d'interférer
le processus Requête -> Réponse en utilisant le composant « EventDispatcher ».

Exemple de validation de jeton
------------------------------

Imaginez que vous devez développer une API dans laquelle certains contrôleurs
sont publics mais d'autres ont un accès restreint qui est réservé à un
ou plusieurs clients. Pour ces fonctionnalités privées, vous pourriez
fournir un jeton à vos clients afin qu'ils s'identifient eux-mêmes.

Donc, avant d'exécuter votre action de contrôleur, vous devez vérifier si
l'action est restreinte ou pas. Et si elle est restreinte, vous devez valider
le jeton fourni.

.. note::

    Veuillez noter que, pour plus de simplicité, les jetons vont être
    définis dans la configuration et aucune mise en place de base de données
    ni de fournisseur d'authentification via le composant de Sécurité ne vont
    être utilisés.

Créer un filtre interférant avant un processus avec un événement controller.request
-----------------------------------------------------------------------------------

Mise en place basique
~~~~~~~~~~~~~~~~~~~~~

Vous pouvez ajouter une configuration de token basique en utilisant le fichier
``config.yml`` et la clé « parameters » :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            tokens:
                client1: pass1
                client2: pass2

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="tokens" type="collection">
                <parameter key="client1">pass1</parameter>
                <parameter key="client2">pass2</parameter>
            </parameter>
        </parameters>

    .. code-block:: php

        // app/config/config.php
        $container->setParameter('tokens', array(
            'client1' => 'pass1',
            'client2' => 'pass2',
        ));

Les contrôleurs de Tag devant être vérifiés
-------------------------------------------

Un « listener » de ``kernel.controller`` est notifié à chaque requête, juste
avant que le contrôleur ne soit exécuté. D'abord, vous avez besoin de savoir si
le contrôleur qui correspond à la requête a besoin d'une validation de token.

Une façon propre et facile est de créer une interface vide et de faire que les
contrôleurs l'implémentent::

    namespace Acme\DemoBundle\Controller;

    interface TokenAuthenticatedController
    {
        // rien ici
    }

Un contrôleur qui implémente cette interface ressemble simplement à cela::

    class FooController implements TokenAuthenticatedController
    {
        // vos actions qui ont besoin d'authentification
    }

Créer un « Listener » d'Evénement
---------------------------------

Ensuite, vous allez avoir besoin de créer un « listener » d'événement, qui va
contenir la logique que vous souhaitez exécuter avant vos contrôleurs. Si
vous n'êtes pas familier avec les « listeners » d'événement, vous pouvez
en apprendre plus sur eux ici :doc:`/cookbook/service_container/event_listener`::

    // src/Acme/DemoBundle/EventListener/BeforeListener.php
    namespace Acme\DemoBundle\EventListener;

    use Acme\DemoBundle\Controller\TokenAuthenticatedController;
    use Symfony\Component\HttpKernel\Exception\AccessDeniedHttpException;
    use Symfony\Component\HttpKernel\Event\FilterControllerEvent;

    class BeforeListener
    {
        private $tokens;

        public function __contruct($tokens)
        {
            $this->tokens = $tokens;
        }

        public function onKernelController(FilterControllerEvent $event)
        {
            $controller = $event->getController();

            /*
             * la variable $controller passée peut être une classe ou une Closure. Ce n'est pas
             * courant dans Symfony2 mais cela peut arriver.
             * Si c'est une classe, elle est donnée sous forme de tableau
             */
            if (!is_array($controller)) {
                return;
            }

            if($controller[0] instanceof TokenAuthenticatedController) {
                $token = $event->getRequest()->get('token');
                if (!in_array($token, $this->tokens)) {
                    throw new AccessDeniedHttpException('This action needs a valid token!');
                }
            }
        }
    }

Déclarer le « Listener »
------------------------

Finalement, déclarez votre « listener » comme un service et « taggez-le » en
tant que « listener » d'événement. En écoutant le ``kernel.controller``, vous
dites à Symfony que vous voulez que votre « listener » soit appelé juste avant
qu'un contrôleur quelconque soit exécuté :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml (ou dans votre services.yml)
        services:
            demo.tokens.action_listener:
              class: Acme\DemoBundle\EventListener\BeforeListener
              arguments: [ %tokens% ]
              tags:
                    - { name: kernel.event_listener, event: kernel.controller, method: onKernelController }

    .. code-block:: xml

        <!-- app/config/config.xml (ou dans votre services.xml) -->
        <service id="demo.tokens.action_listener" class="Acme\DemoBundle\EventListener\BeforeListener">
            <argument>%tokens%</argument>
            <tag name="kernel.event_listener" event="kernel.controller" method="onKernelController" />
        </service>

    .. code-block:: php

        // app/config/config.php (ou dans votre services.php)
        use Symfony\Component\DependencyInjection\Definition;

        $listener = new Definition('Acme\DemoBundle\EventListener\BeforeListener', array('%tokens%'));
        $listener->addTag('kernel.event_listener', array('event' => 'kernel.controller', 'method' => 'onKernelController'));
        $container->setDefinition('demo.tokens.action_listener', $listener);

