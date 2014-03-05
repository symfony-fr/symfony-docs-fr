.. index::
   single: Évènements; Créer un listener

Comment créer un « listener » (« écouteur » en français) d'évènement
====================================================================

Symfony possède divers évènements et « hooks » qui peuvent être utilisés
pour déclencher un comportement personnalisé dans votre application. Ces
évènements sont lancés par le composant HttpKernel et peuvent être consultés
dans la classe :class:`Symfony\\Component\\HttpKernel\\KernelEvents`.

Afin de personnaliser un évènement avec votre propre logique, vous devez créer
un service qui va agir en tant que « listener » d'évènement pour cet évènement.
Dans cet article, nous allons créer un service qui agit en tant que « Listener »
d'Exception, vous permettant de modifier comment les exceptions sont affichées par
notre application. L'évènement ``KernelEvents::EXCEPTION`` est l'un des évènements
du coeur du noyau::

    // src/Acme/DemoBundle/Listener/AcmeExceptionListener.php
    namespace Acme\DemoBundle\Listener;

    use Symfony\Component\HttpKernel\Event\GetResponseForExceptionEvent;
    use Symfony\Component\HttpFoundation\Response;
    use Symfony\Component\HttpKernel\Exception\HttpExceptionInterface;

    class AcmeExceptionListener
    {
        public function onKernelException(GetResponseForExceptionEvent $event)
        {
            // nous récupérons l'objet exception depuis l'évènement reçu
            $exception = $event->getException();
            $message = 'My Error says: ' . $exception->getMessage() . ' with code: ' . $exception->getCode();

            // personnalise notre objet réponse pour afficher les détails de notre exception
            $response = new Response();
            $response->setContent($message);

            // HttpExceptionInterface est un type d'exception spécial qui
            // contient le code statut et les détails de l'entête
            if ($exception instanceof HttpExceptionInterface) {
                $response->setStatusCode($exception->getStatusCode());
                $response->headers->replace($exception->getHeaders());
            } else {
                $response->setStatusCode(500);
            }

            // envoie notre objet réponse modifié à l'évènement
            $event->setResponse($response);
        }
    }

.. tip::

    Chaque évènement reçoit un objet de type ``$event`` légèrement différent.
    Pour l'évènement ``kernel.exception``, c'est
    :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`.
    Pour voir quel est le type d'objet que chaque « listener » d'évènement reçoit,
    voyez :class:`Symfony\\Component\\HttpKernel\\KernelEvents`.

Maintenant que la classe est créée, nous devons juste la définir en tant que
service et notifier Symfony que c'est un « listener » de l'évènement
``kernel.exception`` en utilisant un « tag » spécifique :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            kernel.listener.your_listener_name:
                class: Acme\DemoBundle\Listener\AcmeExceptionListener
                tags:
                    - { name: kernel.event_listener, event: kernel.exception, method: onKernelException }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <service id="kernel.listener.your_listener_name" class="Acme\DemoBundle\Listener\AcmeExceptionListener">
            <tag name="kernel.event_listener" event="kernel.exception" method="onKernelException" />
        </service>

    .. code-block:: php

        // app/config/config.php
        $container
            ->register('kernel.listener.your_listener_name', 'Acme\DemoBundle\Listener\AcmeExceptionListener')
            ->addTag('kernel.event_listener', array('event' => 'kernel.exception', 'method' => 'onKernelException'));

.. note::

    Il y a une autre option ``priority`` pour le tag qui est optionnelle et qui
    a pour valeur par défaut 0. Cette valeur peut aller de -255 à 255, et les
    « listeners » seront exécutées dans l'ordre de leur priorité (par ordre décroissant). Cela est utile
    lorsque vous avez besoin de garantir qu'un « listener » est exécuté avant un
    autre.


Évènement de requête, vérification des types
--------------------------------------------

Une même page peut faire plusieurs requêtes (une requête principale et plusieurs
sous-requêtes, c'est pourquoi, lorsque vous travaillez avec l'évènement ``KernelEvents::REQUEST``,
vous pourriez avoir besoin de vérifier le type de la requête. Cela peut être effectué
très facilement comme ceci::

    // src/Acme/DemoBundle/Listener/AcmeRequestListener.php
    namespace Acme\DemoBundle\Listener;

    use Symfony\Component\HttpKernel\Event\GetResponseEvent;
    use Symfony\Component\HttpKernel\HttpKernel;

    class AcmeRequestListener
    {
        public function onKernelRequest(GetResponseEvent $event)
        {
            if (HttpKernel::MASTER_REQUEST != $event->getRequestType()) {
                // ne rien faire si ce n'est pas la requête principale
                return;
            }

            // ...
        }
    }

.. tip::

    Deux types de requête sont disponibles dans l'interface
    :class:`Symfony\\Component\\HttpKernel\\HttpKernelInterface` :
    ``HttpKernelInterface::MASTER_REQUEST`` et ``HttpKernelInterface::SUB_REQUEST``.
