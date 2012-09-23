.. index::
   single: Event Dispatcher; Service container aware

Le Répartiteur d'évènement du Container Aware
=============================================

.. versionadded:: 2.1
    Cette fonctionnalité a été déplacée dans l'« EventDispatcher » dans Symfony 2.1.

Introduction
------------

La classe :class:`Symfony\\Component\\EventDispatcher\\ContainerAwareEventDispatcher`
est une implémentation spéciale du répartiteur d'évènement qui est couplé au conteneur
de services qui fait partie du :doc:`composant d'Injection de Dépendance</components/dependency_injection/introduction>`.
Il permet aux services d'être spécifiés en tant que « listeners »
d'évènement rendant le répartiteur d'évènement extrêmement puissant.

Les services sont chargés de manière fainéante (« lazy loading » en anglais), ce qui
signifie que les services attachés en tant que « listeners » ne seront créés que si
un évènement qui est réparti nécessite ces « listeners ».

Installation
------------

L'installation est très facile et nécessite uniquement l'injection d'une interface
:class:`Symfony\\Component\\DependencyInjection\\ContainerInterface` dans la classe
:class:`Symfony\\Component\\EventDispatcher\\ContainerAwareEventDispatcher`::

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\EventDispatcher\ContainerAwareEventDispatcher;

    $container = new ContainerBuilder();
    $dispatcher = new ContainerAwareEventDispatcher($container);

Ajouter des « Listeners »
-------------------------

Le *Répartiteur d'Évènement du Container Aware* peut soit directement
charger des services spécifiques, soit des services qui implémentent
:class:`Symfony\\Component\\EventDispatcher\\EventSubscriberInterface`.

Les exemples suivants supposent que le conteneur de services a été chargé
avec tous les services qui sont mentionnés.

.. note::

    Les services doivent être marqués comme publics dans le conteneur.

Ajouter des services
~~~~~~~~~~~~~~~~~~~~

Pour connecter des définitions de service existantes, utilisez la méthode
:method:`Symfony\\Component\\EventDispatcher\\ContainerAwareEventDispatcher::addListenerService`
où le ``$callback`` est un tableau de ``array($serviceId, $methodName)``::

    $dispatcher->addListenerService($eventName, array('foo', 'logListener'));

Ajouter des services souscripteurs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les « ``EventSubscribers`` » peuvent être ajoutés en utilisant la méthode
:method:`Symfony\\Component\\EventDispatcher\\ContainerAwareEventDispatcher::addSubscriberService`
où le premier argument est l'ID du service souscripteur, et où le second argument
est le nom de la classe du service (qui doit implémenter
:class:`Symfony\\Component\\EventDispatcher\\EventSubscriberInterface`) comme suit::

    $dispatcher->addSubscriberService('kernel.store_subscriber', 'StoreSubscriber');

L'``EventSubscriberInterface`` va ressembler exactement à ce à quoi vous vous attendez::

    use Symfony\Component\EventDispatcher\EventSubscriberInterface;
    // ...

    class StoreSubscriber implements EventSubscriberInterface
    {
        static public function getSubscribedEvents()
        {
            return array(
                'kernel.response' => array(
                    array('onKernelResponsePre', 10),
                    array('onKernelResponsePost', 0),
                ),
                'store.order'     => array('onStoreOrder', 0),
            );
        }

        public function onKernelResponsePre(FilterResponseEvent $event)
        {
            // ...
        }

        public function onKernelResponsePost(FilterResponseEvent $event)
        {
            // ...
        }

        public function onStoreOrder(FilterOrderEvent $event)
        {
            // ...
        }
    }