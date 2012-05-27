.. index::
   single: Evénements; Créer un Listener

Comment créer un « listener » (« écouteur » en français) d'evénement
====================================================================

Symfony possède divers événements et « hooks » qui peuvent être utilisés
pour déclencher un comportement personnalisé dans votre application. Ces
événements sont lancés par le composant HttpKernel et peuvent être consultés
dans la classe :class:`Symfony\\Component\\HttpKernel\\KernelEvents`.

Afin de personnaliser un événement avec votre propre logique, vous devez créer
un service qui va agir en tant que « listener » d'événement pour cet événement.
Dans cet article, nous allons créer un service qui agit en tant que « Listener »
d'Exception, nous permettant de modifier comment les exceptions sont affichées par
notre application. L'événement ``KernelEvents::EXCEPTION`` est l'un des événements
du coeur du noyau::

    // src/Acme/DemoBundle/Listener/AcmeExceptionListener.php
    namespace Acme\DemoBundle\Listener;

    use Symfony\Component\HttpKernel\Event\GetResponseForExceptionEvent;

    class AcmeExceptionListener
    {
        public function onKernelException(GetResponseForExceptionEvent $event)
        {
            // nous récupérons l'objet exception depuis l'événement reçu
            $exception = $event->getException();
            $message = 'My Error says: ' . $exception->getMessage();
            
            // personnalise notre objet réponse pour afficher les détails de notre exception
            $response->setContent($message);
            $response->setStatusCode($exception->getStatusCode());
            
            // envoie notre objet réponse modifié à l'événement
            $event->setResponse($response);
        }
    }

.. tip::

    Chaque événement reçoit un objet de type ``$event`` légèrement différent.
    Pour l'événement ``kernel.exception``, c'est
    :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`.
    Pour voir quel est le type d'objet que chaque « listener » d'événement reçoit,
    voyez :class:`Symfony\\Component\\HttpKernel\\KernelEvents`.

Maintenant que la classe est créée, nous devons juste la définir en tant que
service et notifier Symfony que c'est un « listener » de l'événement
``kernel.exception`` en utilisant un « tag » spécifique :

.. configuration-block::

    .. code-block:: yaml

        services:
            kernel.listener.your_listener_name:
                class: Acme\DemoBundle\Listener\AcmeExceptionListener
                tags:
                    - { name: kernel.event_listener, event: kernel.exception, method: onKernelException }

    .. code-block:: xml

        <service id="kernel.listener.your_listener_name" class="Acme\DemoBundle\Listener\AcmeExceptionListener">
            <tag name="kernel.event_listener" event="kernel.exception" method="onKernelException" />
        </service>

    .. code-block:: php

        $container
            ->register('kernel.listener.your_listener_name', 'Acme\DemoBundle\Listener\AcmeExceptionListener')
            ->addTag('kernel.event_listener', array('event' => 'kernel.exception', 'method' => 'onKernelException'))
        ;
        
.. note::

    Il y a une autre option ``priority`` pour le tag qui est optionnelle et qui
    a pour valeur par défaut 0. Cette valeur peut aller de -255 à 255, et les
    « listeners » seront exécutés dans cet ordre de priorité. Cela est utile
    lorsque vous avez besoin de garantir qu'un « listener » est exécuté avant un
    autre.


Evènement de requête, vérification des types
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
                // ne rien faire si c'est la requête principale
                return;
             }
 
            // votre code
            }
        } 
    }
   
.. tip::

    Deux types de requête sont disponibles dans l'interface
    :class:`Symfony\\Component\\HttpKernel\\HttpKernelInterface` :
    ``HttpKernelInterface::MASTER_REQUEST`` et ``HttpKernelInterface::SUB_REQUEST``.