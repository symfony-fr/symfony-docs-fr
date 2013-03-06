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
le processus Requête -> Réponse en utilisant le :doc:`composant EventDispatcher</components/event_dispatcher/introduction>`.

Exemple de validation de jeton
------------------------------

Imaginez que vous devez développer une API dans laquelle certains contrôleurs
sont publics mais d'autres ont un accès restreint qui est réservé à un
ou plusieurs clients. Pour ces fonctionnalités privées, vous pourriez
fournir un jeton à vos clients afin qu'ils s'identifient eux-mêmes.

Donc, avant d'exécuter votre action de contrôleur, vous devez vérifier si
l'action est restreinte ou pas. Si elle est restreinte, vous devez valider
le jeton fourni.

.. note::

    Veuillez noter que, pour garder cet article suffisament simple, les jetons
    vont être définis dans la configuration et aucune mise en place de base de
    données ni de fournisseur d'authentification via le composant de Sécurité ne vont
    être utilisés.

Créer un filtre avant un évènement ``kernel.controller``
--------------------------------------------------------

Premièrement, stockez un jeton basique en configuration en utilisant le fichier
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Un « listener » de ``kernel.controller`` est notifié à *chaque* requête juste
avant que le contrôleur ne soit exécuté. D'abord, vous avez besoin de savoir si
le contrôleur qui correspond à la requête a besoin d'une validation de token.

Une façon propre et facile est de créer une interface vide et de faire en sorte
que les contrôleurs l'implémentent::

    namespace Acme\DemoBundle\Controller;

    interface TokenAuthenticatedController
    {
        // ...
    }

Un contrôleur qui implémente cette interface ressemble simplement à cela::

    namespace Acme\DemoBundle\Controller;

    use Acme\DemoBundle\Controller\TokenAuthenticatedController;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class FooController extends Controller implements TokenAuthenticatedController
    {
        // une action qui a besoin d'une authentification
        public function barAction()
        {
            // ...
        }
    }

Créer un « Listener » d'Évènement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ensuite, vous allez avoir besoin de créer un « listener » d'évènement, qui va
contenir la logique que vous souhaitez exécuter avant vos contrôleurs. Si
vous n'êtes pas familier avec les « listeners » d'évènement, vous pouvez
en apprendre plus sur eux en lisant :doc:`/cookbook/service_container/event_listener`::

    // src/Acme/DemoBundle/EventListener/TokenListener.php
    namespace Acme\DemoBundle\EventListener;

    use Acme\DemoBundle\Controller\TokenAuthenticatedController;
    use Symfony\Component\HttpKernel\Exception\AccessDeniedHttpException;
    use Symfony\Component\HttpKernel\Event\FilterControllerEvent;

    class TokenListener
    {
        private $tokens;

        public function __construct($tokens)
        {
            $this->tokens = $tokens;
        }

        public function onKernelController(FilterControllerEvent $event)
        {
            $controller = $event->getController();

            /*
             * $controller peut être une classe ou une closure. Ce n'est pas
             * courant dans Symfony2 mais ça peut arriver.
             * Si c'est une classe, elle est au format array
             */
            if (!is_array($controller)) {
                return;
            }

            if ($controller[0] instanceof TokenAuthenticatedController) {
                $token = $event->getRequest()->query->get('token');
                if (!in_array($token, $this->tokens)) {
                    throw new AccessDeniedHttpException('Cette action nécessite un jeton valide!');
                }
            }
        }
    }

Déclarez le « Listener »
~~~~~~~~~~~~~~~~~~~~~~~~

Finalement, déclarez votre « listener » comme un service et « taggez-le » en
tant que « listener » d'évènement. En écoutant le ``kernel.controller``, vous
dites à Symfony que vous voulez que votre « listener » soit appelé juste avant
qu'un contrôleur quelconque soit exécuté :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml (or inside your services.yml)
        services:
            demo.tokens.action_listener:
                class: Acme\DemoBundle\EventListener\TokenListener
                arguments: [ %tokens% ]
                tags:
                    - { name: kernel.event_listener, event: kernel.controller, method: onKernelController }

    .. code-block:: xml

        <!-- app/config/config.xml (or inside your services.xml) -->
        <service id="demo.tokens.action_listener" class="Acme\DemoBundle\EventListener\TokenListener">
            <argument>%tokens%</argument>
            <tag name="kernel.event_listener" event="kernel.controller" method="onKernelController" />
        </service>

    .. code-block:: php

        // app/config/config.php (or inside your services.php)
        use Symfony\Component\DependencyInjection\Definition;

        $listener = new Definition('Acme\DemoBundle\EventListener\TokenListener', array('%tokens%'));
        $listener->addTag('kernel.event_listener', array('event' => 'kernel.controller', 'method' => 'onKernelController'));
        $container->setDefinition('demo.tokens.action_listener', $listener);

Avec cette configuration, votre méthode ``onKernelController`` de ``TokenListener``
sera exécutée à chaque requête. Si le contrôleur qui doit être exécuté implémente
``TokenAuthenticatedController``, l'authentification par jeton est appliquée. Cela
vous permet d'avoir le filtre « avant » que vous vouliez sur tout les contrôleurs.

Créer un filtre après un évènement ``kernel.response``
------------------------------------------------------

En plus d'avoir un « hook » qui est exécuté avant notre contrôleur,
vous pouvez également ajouter un hook qui sera exécuté *après* votre
contrôleur. Pour cet exemple, imaginez que vous voulez ajouter un hash sha1
(avec un salage - ou salt - qui utilise le jeton) à chaque réponse qui a passé
notre authentification par jeton.

Un autre évènement du noyau de Symfony, appelé ``kernel.response``, est
notifié à chaque requête, mais après que le contrôleur a retourné un objet
Response. Créer un écouteur « après » est aussi simple que de créer une classe
écouteur et de l'enregistrer en tant que service sur cet évènement.

Par exemple, prenez le ``TokenListener`` de l'exemple précédent et enregistrez
d'abord le jeton d'authentification dans les attributs de la requête. Cela
indiquera que cette requête a subi une demande d'authentification par jeton::

    public function onKernelController(FilterControllerEvent $event)
    {
        // ...

        if ($controller[0] instanceof TokenAuthenticatedController) {
            $token = $event->getRequest()->query->get('token');
            if (!in_array($token, $this->tokens)) {
                throw new AccessDeniedHttpException('Cette action nécessite un jeton valide!');
            }

            // marque la requête après authentification
            $event->getRequest()->attributes->set('auth_token', $token);
        }
    }

Maintenant, ajoutons une autre méthode à cette classe, ``onKernelResponse``,
qui vérifiera que l'objet requête est marqué et, si c'est le cas, qui définira
un en-tête personnalisé pour la réponse::

    // ajoutez la nouvelle instruction use en haut de votre fichier
    use Symfony\Component\HttpKernel\Event\FilterResponseEvent;

    public function onKernelResponse(FilterResponseEvent $event)
    {
        // Vérifie que onKernelController est une requête authentifiée
        if (!$token = $event->getRequest()->attributes->get('auth_token')) {
            return;
        }

        $response = $event->getResponse();

        // crée un hash et le définit comme en-tête de la réponse
        $hash = sha1($response->getContent().$token);
        $response->headers->set('X-CONTENT-HASH', $hash);
    }

Enfin, un second « tag » est nécessaire dans la définition de service pour notifier
Symfony que l'évènement ``onKernelResponse`` doit être notifié pour l'évènement
``kernel.response`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml (or inside your services.yml)
        services:
            demo.tokens.action_listener:
                class: Acme\DemoBundle\EventListener\TokenListener
                arguments: [ %tokens% ]
                tags:
                    - { name: kernel.event_listener, event: kernel.controller, method: onKernelController }
                    - { name: kernel.event_listener, event: kernel.response, method: onKernelResponse }

    .. code-block:: xml

        <!-- app/config/config.xml (or inside your services.xml) -->
        <service id="demo.tokens.action_listener" class="Acme\DemoBundle\EventListener\TokenListener">
            <argument>%tokens%</argument>
            <tag name="kernel.event_listener" event="kernel.controller" method="onKernelController" />
            <tag name="kernel.event_listener" event="kernel.response" method="onKernelResponse" />
        </service>

    .. code-block:: php

        // app/config/config.php (or inside your services.php)
        use Symfony\Component\DependencyInjection\Definition;

        $listener = new Definition('Acme\DemoBundle\EventListener\TokenListener', array('%tokens%'));
        $listener->addTag('kernel.event_listener', array('event' => 'kernel.controller', 'method' => 'onKernelController'));
        $listener->addTag('kernel.event_listener', array('event' => 'kernel.response', 'method' => 'onKernelResponse'));
        $container->setDefinition('demo.tokens.action_listener', $listener);

C'est tout ! Le ``TokenListener`` est maintenant notifié avant chaque contrôleur
qui est exécuté (``onKernelController``) et après chaque réponse retournée par un
contrôleur (``onKernelResponse``). En faisant des contrôleurs spécifiques qui
implémentent l'interface ``TokenAuthenticatedController``, nos écouteurs savent
quels contrôleurs traiter. Et en stockant une valeur dans les attributs de la
requête, la méthode ``onKernelResponse`` sait quand ajouter notre nouvel en-tête.
Amusez-vous !