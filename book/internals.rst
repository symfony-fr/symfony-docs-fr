.. index::
   single: Composants Internes

Composants Internes
===================

Il paraît que vous voulez comprendre comment Symfony2 fonctionne et comment
l'étendre. Cela me rend très heureux ! Cette section est une explication en
profondeur des composants internes de Symfony2.

.. note::

    Vous avez besoin de lire cette section uniquement si vous souhaitez comprendre
    comment Symfony2 fonctionne en arrière-plan, ou si vous voulez étendre Symfony2.

Vue Globale
-----------

Le code de Symfony2 se compose de plusieurs couches indépendantes. Chacune
d'entre elles est construite par-dessus celles qui la précèdent.

.. tip::

    L'autoloading (« chargement automatique ») n'est pas géré par le
    framework directement ; ceci est effectué indépendemment à l'aide de
    la classe :class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader`
    et du fichier ``src/autoload.php``. Lisez le :doc:`chapitre dédié
    </cookbook/tools/autoloader>` pour plus d'informations.

Le Composant ``HttpFoundation``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le composant de plus bas niveau est :namespace:`Symfony\\Component\\HttpFoundation`.
HttpFoundation fournit les objets principaux nécessaires pour traiter avec HTTP.
C'est une abstraction orientée objet de quelques fonctions et variables PHP
natives :

* La classe :class:`Symfony\\Component\\HttpFoundation\\Request` abstrait
  les principales variables globales PHP que sont ``$_GET``, ``$_POST``, ``$_COOKIE``,
  ``$_FILES``, et ``$_SERVER`` ;

* Le classe :class:`Symfony\\Component\\HttpFoundation\\Response` abstrait quelques
  fonctions PHP comme ``header()``, ``setcookie()``, et ``echo`` ;

* La classe :class:`Symfony\\Component\\HttpFoundation\\Session` et l'interface
  :class:`Symfony\\Component\\HttpFoundation\\SessionStorage\\SessionStorageInterface`
  font abstraction des fonctions de gestion des sessions ``session_*()``.

Le Composant ``HttpKernel``
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Par-dessus HttpFoundation se trouve le composant :namespace:`Symfony\\Component\\HttpKernel`.
HttpKernel gère la partie dynamique de HTTP ; c'est une fine surcouche au-dessus
des classes Request et Response pour standardiser la façon dont les requêtes
sont gérées. Il fournit aussi des points d'extension et des outils qui en font
un point de démarrage idéal pour créer un framework Web sans trop d'efforts.

Optionnellement, il ajoute de la configurabilité et de l'extensibilité, grâce
au composant Dependency Injection et à un puissant système de plugin (bundles).

.. seealso::

    Lisez-en plus à propos des composants :doc:`HttpKernel <kernel>` et
    :doc:`Dependency Injection </book/service_container>` ainsi que sur les
    :doc:`Bundles </cookbook/bundles/best_practices>`.

Le Bundle ``FrameworkBundle``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le bundle :namespace:`Symfony\\Bundle\\FrameworkBundle` est le bundle qui lie
les principaux composants et bibliothèques ensemble afin de fournir un framework
MVC léger et rapide. Il vient avec une configuration par défaut ainsi
qu'avec des conventions afin d'en faciliter l'apprentissage.

.. index::
   single: Internals; Kernel

Le Kernel
---------

La classe :class:`Symfony\\Component\\HttpKernel\\HttpKernel` est la classe
centrale de Symfony2 et est responsable de la gestion des requêtes clientes.
Son but principal est de « convertir » un objet
:class:`Symfony\\Component\\HttpFoundation\\Request` en un objet
:class:`Symfony\\Component\\HttpFoundation\\Response`.

Chaque Kernel Symfony2 implémente
:class:`Symfony\\Component\\HttpKernel\\HttpKernelInterface` ::

    function handle(Request $request, $type = self::MASTER_REQUEST, $catch = true)

.. index::
   single: Composants Internes; Résolution du Contrôleur

Les Contrôleurs
~~~~~~~~~~~~~~~

Pour convertir une Requête en une Réponse, le Kernel repose sur un « Contrôleur ».
Un Contrôleur peut être n'importe quel « callable » PHP.

Le Kernel délègue la sélection de quel Contrôleur devrait être exécuté à une
implémentation de
:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface` ::

    public function getController(Request $request);

    public function getArguments(Request $request, $controller);

La méthode
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getController`
retourne le Contrôleur (un « callable » PHP) associé à la Requête donnée. L'implémentation par
défaut (:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver`) recherche un
attribut de la requête ``_controller`` qui représente le nom du contrôleur (une chaîne de
caractères « class::method », comme ``Bundle\BlogBundle\PostController:indexAction``).

.. tip::
    L'implémentation par défaut utilise le
    :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener` pour définir
    l'attribut de la Requête ``_controller`` (voir :ref:`kernel-core-request`).

La méthode
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getArguments`
retourne un tableau d'arguments à passer au Contrôleur. L'implémentation par défaut résoud
automatiquement les arguments de la méthode, basé sur les attributs de la Requête.

.. sidebar:: Faire correspondre les arguments de la méthode du Contrôleur aux attributs de la Requête

    Pour chaque argument d'une méthode, Symfony2 essaye d'obtenir la valeur d'un attribut
    d'une Requête avec le même nom. S'il n'est pas défini, la valeur par défaut de l'argument
    est utilisée si elle est définie ::

        // Symfony2 va rechercher un attribut « id » (obligatoire)
        // et un nommé « admin » (optionnel)
        public function showAction($id, $admin = true)
        {
            // ...
        }

.. index::
  single: Composants Internes; Gestion de la Requête

Gestion des Requêtes
~~~~~~~~~~~~~~~~~~~~

La méthode ``handle()`` prend une ``Requête`` et retourne *toujours* une ``Réponse``.
Pour convertir la ``Requête``, ``handle()`` repose sur le « Resolver » et sur une
chaîne ordonnée de notifications d'évènements (voir la prochaine section pour plus
d'informations à propos de chaque évènement) :

1. Avant de faire quoi que ce soit d'autre, l'évènement ``kernel.request`` est
   notifié -- si l'un des listeners (« écouteurs » en français) retourne une
   ``Réponse``, il saute directement à l'étape 8 ;

2. Le « Resolver » est appelé pour déterminer le Contrôleur à exécuter ;

3. Les listeners de l'évènement ``kernel.controller`` peuvent maintenant
   manipuler le « callable » Contrôleur de la manière dont ils souhaitent
   (le changer, créer un « wrapper » au-dessus de lui, ...) ;

4. Le Kernel vérifie que le Contrôleur est un « callable » PHP valide ;

5. Le « Resolver » est appelé pour déterminer les arguments à passer au Contrôleur ;

6. Le Kernel appelle le Contrôleur ;

7. Si le Contrôleur ne retourne pas une ``Réponse``, les listeners de l'évènement
   ``kernel.view`` peuvent convertir la valeur retournée par le Contrôleur en une ``Réponse`` ;

8. Les listeners de l'évènement ``kernel.response`` peuvent manipuler la ``Réponse``
   (contenu et en-têtes) ;

9. La Réponse est retournée.

Si une Exception est jetée pendant le traitement de la Requête, l'évènement
``kernel.exception`` est notifié et les listeners ont alors une chance de
convertir l'Exception en une Réponse. Si cela fonctionne, l'évènement
``kernel.response`` sera notifié ; si non, l'Exception sera re-jetée.

Si vous ne voulez pas que les Exceptions soient capturées (pour des requêtes embarquées
par exemple), désactivez l'évènement ``kernel.exception`` en passant ``false`` en tant
que troisième argument de la méthode ``handle()``.

.. index::
  single: Composants Internes; Requêtes Internes

Requêtes Internes
~~~~~~~~~~~~~~~~~

A tout moment durant la gestion de la requête (la « master »), une sous-requête
peut être gérée. Vous pouvez passer le type de requête à la méthode ``handle()``
(son second argument) :

* ``HttpKernelInterface::MASTER_REQUEST``;
* ``HttpKernelInterface::SUB_REQUEST``.

Le type est passé à tous les évènements et les listeners peuvent ainsi agir
en conséquence (le traitement doit seulement intervenir sur la requête
« master »).

.. index::
   pair: Kernel; Evènement

Les Evènements
~~~~~~~~~~~~~~

Chaque évènement jeté par le Kernel est une sous-classe de
:class:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent`. Cela signifie que
chaque évènement a accès aux mêmes informations de base :

* ``getRequestType()`` - retourne le *type* de la requête
  (``HttpKernelInterface::MASTER_REQUEST`` ou ``HttpKernelInterface::SUB_REQUEST``) ;

* ``getKernel()`` - retourne le Kernel gérant la requête ;

* ``getRequest()`` - retourne la ``Requête`` courante qui est en train d'être gérée.

``getRequestType()``
....................

La méthode ``getRequestType()`` permet aux listeners de connaître le type
de la requête. Par exemple, si un listener doit seulement être activé pour les
requêtes « master », ajoutez le code suivant au début de votre méthode listener ::

    use Symfony\Component\HttpKernel\HttpKernelInterface;

    if (HttpKernelInterface::MASTER_REQUEST !== $event->getRequestType()) {
        // retourne immédiatement
        return;
    }

.. tip::

    Si vous n'êtes pas encore familier avec le « Dispatcher d'Evènements » de
    Symfony2, lisez la section :ref:`event_dispatcher` en premier.

.. index::
   single: Evènement; kernel.request

.. _kernel-core-request:

L'Evènement ``kernel.request``
..............................

*La Classe Evènement* : :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseEvent`

Le but de cet évènement est soit de retourner un objet ``Response`` immédiatement ou bien
de définir des variables afin qu'un Contrôleur puisse être appelé après l'évènement.
Tout listener peut retourner un objet ``Response`` via la méthode ``setResponse()``
sur l'évènement. Dans ce cas, tous les autres listeners ne seront pas appelés.

Cet évènement est utilisé par le ``FrameworkBundle`` afin de remplir l'attribut de la
``Requête`` ``_controller``, via
:class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener`. RequestListener
utilise un objet :class:`Symfony\\Component\\Routing\\RouterInterface` pour faire correspondre
la ``Requête`` et déterminer le nom du Contrôleur (stocké dans l'attribut de la
``Requête`` ``_controller``).

.. index::
   single: Evènement; kernel.controller

L'évènement ``kernel.controller``
.................................

*La Classe Evènement*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterControllerEvent`

Cet évènement n'est pas utilisé par le ``FrameworkBundle``, mais peut être un point
d'entrée utilisé pour modifier le contrôleur qui devrait être exécuté :

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\FilterControllerEvent;

    public function onKernelController(FilterControllerEvent $event)
    {
        $controller = $event->getController();
        // ...

        // le contrôleur peut être remplacé par n'importe quel « callable » PHP
        $event->setController($controller);
    }

.. index::
   single: Evènement; kernel.view

L'évènement ``kernel.view``
...........................

*La Classe Evènement*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForControllerResultEvent`

Cet évènement n'est pas utilisé par le ``FrameworkBundle``, mais il peut être utilisé
pour implémenter un sous-système de vues. Cet évènement est appelé *seulement* si le
Contrôleur *ne* retourne *pas* un objet ``Response``. Le but de cet évènement est
de permettre à d'autres valeurs retournées d'être converties en une ``Réponse``.

La valeur retournée par le Contrôleur est accessible via la méthode ``getControllerResult`` ::

    use Symfony\Component\HttpKernel\Event\GetResponseForControllerResultEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelView(GetResponseForControllerResultEvent $event)
    {
        $val = $event->getReturnValue();
        $response = new Response();
        // personnalisez d'une manière ou d'une autre la Réponse
        // en vous basant sur la valeur retournée

        $event->setResponse($response);
    }

.. index::
   single: Evènement; kernel.response

L'évènement ``kernel.response``
...............................

*La Classe Evènement*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`

L'objectif de cet évènement est de permettre à d'autres systèmes de modifier ou
de remplacer l'objet ``Response`` après sa création :

.. code-block:: php

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $response = $event->getResponse();
        // .. modifiez l'objet Response
    }

Le ``FrameworkBundle`` enregistre plusieurs listeners :

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`:
  collecte les données pour la requête courante ;

* :class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener`:
  injecte la Web Debug Toolbar (« Barre d'outils de Debugging Web » en français) ;

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\ResponseListener`: définit la
  valeur du ``Content-Type`` de la Réponse basé sur le format de la requête ;

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\EsiListener`: ajoute un
  en-tête HTTP ``Surrogate-Control`` lorsque la Réponse a besoin d'être analysée
  pour trouver des balises ESI.

.. index::
   single: Evènement; kernel.exception

.. _kernel-kernel.exception:

L'évènement ``kernel.exception``
................................

*La Classe Evènement*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`

Le ``FrameworkBundle`` enregistre un
:class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener` qui
transmet la ``Requête`` à un Contrôleur donné (la valeur du paramètre
``exception_listener.controller`` -- doit être exprimé suivant la notation
``class::method``).

Un listener sur cet évènement peut créer et définir un objet ``Response``,
créer et définir un nouvel objet ``Exception``, ou ne rien faire :

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\GetResponseForExceptionEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelException(GetResponseForExceptionEvent $event)
    {
        $exception = $event->getException();
        $response = new Response();
        // définissez l'objet Response basé sur l'exception capturée
        $event->setResponse($response);

        // vous pouvez alternativement définir une nouvelle Exception
        // $exception = new \Exception('Some special exception');
        // $event->setException($exception);
    }

.. index::
   single: Event Dispatcher

The Event Dispatcher
--------------------

Objected Oriented code has gone a long way to ensuring code extensibility. By
creating classes that have well defined responsibilities, your code becomes
more flexible and a developer can extend them with subclasses to modify their
behaviors. But if he wants to share his changes with other developers who have
also made their own subclasses, code inheritance is moot.

Consider the real-world example where you want to provide a plugin system for
your project. A plugin should be able to add methods, or do something before
or after a method is executed, without interfering with other plugins. This is
not an easy problem to solve with single inheritance, and multiple inheritance
(were it possible with PHP) has its own drawbacks.

The Symfony2 Event Dispatcher implements the `Observer`_ pattern in a simple
and effective way to make all these things possible and to make your projects
truly extensible.

Take a simple example from the `Symfony2 HttpKernel component`_. Once a
``Response`` object has been created, it may be useful to allow other elements
in the system to modify it (e.g. add some cache headers) before it's actually
used. To make this possible, the Symfony2 kernel throws an event -
``kernel.response``. Here's how it work:

* A *listener* (PHP object) tells a central *dispatcher* object that it wants
  to listen to the ``kernel.response`` event;

* At some point, the Symfony2 kernel tells the *dispatcher* object to dispatch
  the ``kernel.response`` event, passing with it an ``Event`` object that has
  access to the ``Response`` object;

* The dispatcher notifies (i.e. calls a method on) all listeners of the
  ``kernel.response`` event, allowing each of them to make any modification to
  the ``Response`` object.

.. index::
   single: Event Dispatcher; Events

.. _event_dispatcher:

Events
~~~~~~

When an event is dispatched, it's identified by a unique name (e.g.
``kernel.response``), which any number of listeners might be listening to. A
:class:`Symfony\\Component\\EventDispatcher\\Event` instance is also created
and passed to all of the listeners. As you'll see later, the ``Event`` object
itself often contains data about the event being dispatched.

.. index::
   pair: Event Dispatcher; Naming conventions

Naming Conventions
..................

The unique event name can be any string, but optionally follows a few simple
naming conventions:

* use only lowercase letters, numbers, dots (``.``), and underscores (``_``);

* prefix names with a namespace followed by a dot (e.g. ``kernel.``);

* end names with a verb that indicates what action is being taken (e.g.
  ``request``).

Here are some examples of good event names:

* ``kernel.response``
* ``form.pre_set_data``

.. index::
   single: Event Dispatcher; Event Subclasses

Event Names and Event Objects
.............................

When the dispatcher notifies listeners, it passes an actual ``Event`` object
to those listeners. The base ``Event`` class is very simple: it contains a
method for stopping :ref:`event
propagation<event_dispatcher-event-propagation>`, but not much else.

Often times, data about a specific event needs to be passed along with the
``Event`` object so that the listeners have needed information. In the case of
the ``kernel.response`` event, the ``Event`` object that's created and passed to
each listener is actually of type
:class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`, a
subclass of the base ``Event`` object. This class contains methods such as
``getResponse`` and ``setResponse``, allowing listeners to get or even replace
the ``Response`` object.

The moral of the story is this: when creating a listener to an event, the
``Event`` object that's passed to the listener may be a special subclass that
has additional methods for retrieving information from and responding to the
event.

The Dispatcher
~~~~~~~~~~~~~~

The dispatcher is the central object of the event dispatcher system. In
general, a single dispatcher is created, which maintains a registry of
listeners. When an event is dispatched via the dispatcher, it notifies all
listeners registered with that event.

.. code-block:: php

    use Symfony\Component\EventDispatcher\EventDispatcher;

    $dispatcher = new EventDispatcher();

.. index::
   single: Event Dispatcher; Listeners

Connecting Listeners
~~~~~~~~~~~~~~~~~~~~

To take advantage of an existing event, you need to connect a listener to the
dispatcher so that it can be notified when the event is dispatched. A call to
the dispatcher ``addListener()`` method associates any valid PHP callable to
an event:

.. code-block:: php

    $listener = new AcmeListener();
    $dispatcher->addListener('foo.action', array($listener, 'onFooAction'));

The ``addListener()`` method takes up to three arguments:

* The event name (string) that this listener wants to listen to;

* A PHP callable that will be notified when an event is thrown that it listens
  to;

* An optional priority integer (higher equals more important) that determines
  when a listener is triggered versus other listeners (defaults to ``0``). If
  two listeners have the same priority, they are executed in the order that
  they were added to the dispatcher.

.. note::

    A `PHP callable`_ is a PHP variable that can be used by the
    ``call_user_func()`` function and returns ``true`` when passed to the
    ``is_callable()`` function. It can be a ``\Closure`` instance, a string
    representing a function, or an array representing an object method or a
    class method.

    So far, you've seen how PHP objects can be registered as listeners. You
    can also register PHP `Closures`_ as event listeners:

    .. code-block:: php

        use Symfony\Component\EventDispatcher\Event;

        $dispatcher->addListener('foo.action', function (Event $event) {
            // will be executed when the foo.action event is dispatched
        });

Once a listener is registered with the dispatcher, it waits until the event is
notified. In the above example, when the ``foo.action`` event is dispatched,
the dispatcher calls the ``AcmeListener::onFooAction`` method and passes the
``Event`` object as the single argument:

.. code-block:: php

    use Symfony\Component\EventDispatcher\Event;

    class AcmeListener
    {
        // ...

        public function onFooAction(Event $event)
        {
            // do something
        }
    }

.. tip::

    If you use the Symfony2 MVC framework, listeners can be registered via
    your :ref:`configuration <dic-tags-kernel-event-listener>`. As an added
    bonus, the listener objects are instantiated only when needed.

In many cases, a special ``Event`` subclass that's specific to the given event
is passed to the listener. This gives the listener access to special
information about the event. Check the documentation or implementation of each
event to determine the exact ``Symfony\Component\EventDispatcher\Event``
instance that's being passed. For example, the ``kernel.event`` event passes an
instance of ``Symfony\Component\HttpKernel\Event\FilterResponseEvent``:

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\FilterResponseEvent

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $response = $event->getResponse();
        $request = $event->getRequest();

        // ...
    }

.. _event_dispatcher-closures-as-listeners:

.. index::
   single: Event Dispatcher; Creating and Dispatching an Event

Creating and Dispatching an Event
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In addition to registering listeners with existing events, you can create and
throw your own events. This is useful when creating third-party libraries and
also when you want to keep different components of your own system flexible
and decoupled.

The Static ``Events`` Class
...........................

Suppose you want to create a new Event - ``store.order`` - that is dispatched
each time an order is created inside your application. To keep things
organized, start by creating a ``StoreEvents`` class inside your application
that serves to define and document your event:

.. code-block:: php

    namespace Acme\StoreBundle;

    final class StoreEvents
    {
        /**
         * The store.order event is thrown each time an order is created
         * in the system.
         *
         * The event listener receives an Acme\StoreBundle\Event\FilterOrderEvent
         * instance.
         *
         * @var string
         */
        const onStoreOrder = 'store.order';
    }

Notice that this class doesn't actually *do* anything. The purpose of the
``StoreEvents`` class is just to be a location where information about common
events can be centralized. Notice also that a special ``FilterOrderEvent``
class will be passed to each listener of this event.

Creating an Event object
........................

Later, when you dispatch this new event, you'll create an ``Event`` instance
and pass it to the dispatcher. The dispatcher then passes this same instance
to each of the listeners of the event. If you don't need to pass any
information to your listeners, you can use the default
``Symfony\Component\EventDispatcher\Event`` class. Most of the time, however,
you *will* need to pass information about the event to each listener. To
accomplish this, you'll create a new class that extends
``Symfony\Component\EventDispatcher\Event``.

In this example, each listener will need access to some pretend ``Order``
object. Create an ``Event`` class that makes this possible:

.. code-block:: php

    namespace Acme\StoreBundle\Event;

    use Symfony\Component\EventDispatcher\Event;
    use Acme\StoreBundle\Order;

    class FilterOrderEvent extends Event
    {
        protected $order;

        public function __construct(Order $order)
        {
            $this->order = $order;
        }

        public function getOrder()
        {
            return $this->order;
        }
    }

Each listener now has access to to ``Order`` object via the ``getOrder``
method.

Dispatch the Event
..................

The :method:`Symfony\\Component\\EventDispatcher\\EventDispatcher::dispatch`
method notifies all listeners of the given event. It takes two arguments: the
name of the event to dispatch and the ``Event`` instance to pass to each
listener of that event:

.. code-block:: php

    use Acme\StoreBundle\StoreEvents;
    use Acme\StoreBundle\Order;
    use Acme\StoreBundle\Event\FilterOrderEvent;

    // the order is somehow created or retrieved
    $order = new Order();
    // ...

    // create the FilterOrderEvent and dispatch it
    $event = new FilterOrderEvent($order);
    $dispatcher->dispatch(StoreEvents::onStoreOrder, $event);

Notice that the special ``FilterOrderEvent`` object is created and passed to
the ``dispatch`` method. Now, any listener to the ``store.order`` event will
receive the ``FilterOrderEvent`` and have access to the ``Order`` object via
the ``getOrder`` method:

.. code-block:: php

    // some listener class that's been registered for onStoreOrder
    use Acme\StoreBundle\Event\FilterOrderEvent;

    public function onStoreOrder(FilterOrderEvent $event)
    {
        $order = $event->getOrder();
        // do something to or with the order
    }

Passing along the Event Dispatcher Object
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you have a look at the ``EventDispatcher`` class, you will notice that the
class does not act as a Singleton (there is no ``getInstance()`` static method).
That is intentional, as you might want to have several concurrent event
dispatchers in a single PHP request. But it also means that you need a way to
pass the dispatcher to the objects that need to connect or notify events.

The best practice is to inject the event dispatcher object into your objects,
aka dependency injection.

You can use constructor injection::

    class Foo
    {
        protected $dispatcher = null;

        public function __construct(EventDispatcher $dispatcher)
        {
            $this->dispatcher = $dispatcher;
        }
    }

Or setter injection::

    class Foo
    {
        protected $dispatcher = null;

        public function setEventDispatcher(EventDispatcher $dispatcher)
        {
            $this->dispatcher = $dispatcher;
        }
    }

Choosing between the two is really a matter of taste. Many tend to prefer the
constructor injection as the objects are fully initialized at construction
time. But when you have a long list of dependencies, using setter injection
can be the way to go, especially for optional dependencies.

.. tip::

    If you use dependency injection like we did in the two examples above, you
    can then use the `Symfony2 Dependency Injection component`_ to elegantly
    manage these objects.

.. index::
   single: Event Dispatcher; Event subscribers

Using Event Subscribers
~~~~~~~~~~~~~~~~~~~~~~~

The most common way to listen to an event is to register an *event listener*
with the dispatcher. This listener can listen to one or more events and is
notified each time those events are dispatched.

Another way to listen to events is via an *event subscriber*. An event
subscriber is a PHP class that's able to tell the dispatcher exactly which
events it should subscribe to. It implements the
:class:`Symfony\\Component\\EventDispatcher\\EventSubscriberInterface`
interface, which requires a single static method called
``getSubscribedEvents``. Take the following example of a subscriber that
subscribes to the ``kernel.response`` and ``store.order`` events:

.. code-block:: php

    namespace Acme\StoreBundle\Event;

    use Symfony\Component\EventDispatcher\EventSubscriberInterface;
    use Symfony\Component\HttpKernel\Event\FilterResponseEvent;

    class StoreSubscriber implements EventSubscriberInterface
    {
        static public function getSubscribedEvents()
        {
            return array(
                'kernel.response' => 'onKernelResponse',
                'store.order'     => 'onStoreOrder',
            );
        }

        public function onKernelResponse(FilterResponseEvent $event)
        {
            // ...
        }

        public function onStoreOrder(FilterOrderEvent $event)
        {
            // ...
        }
    }

This is very similar to a listener class, except that the class itself can
tell the dispatcher which events it should listen to. To register a subscriber
with the dispatcher, use the
:method:`Symfony\\Component\\EventDispatcher\\EventDispatcher::addSubscriber`
method:

.. code-block:: php

    use Acme\StoreBundle\Event\StoreSubscriber;

    $subscriber = new StoreSubscriber();
    $dispatcher->addSubscriber($subscriber);

The dispatcher will automatically register the subscriber for each event
returned by the ``getSubscribedEvents`` method. This method returns an array
indexed by event names and whose values are either the method name to call or
an array composed of the method name to call and a priority.

.. index::
   single: Event Dispatcher; Stopping event flow

.. _event_dispatcher-event-propagation:

Stopping Event Flow/Propagation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In some cases, it may make sense for a listener to prevent any other listeners
from being called. In other words, the listener needs to be able to tell the
dispatcher to stop all propagation of the event to future listeners (i.e. to
not notify any more listeners). This can be accomplished from inside a
listener via the
:method:`Symfony\\Component\\EventDispatcher\\Event::stopPropagation` method:

.. code-block:: php

   use Acme\StoreBundle\Event\FilterOrderEvent;

   public function onStoreOrder(FilterOrderEvent $event)
   {
       // ...

       $event->stopPropagation();
   }

Now, any listeners to ``store.order`` that have not yet been called will *not*
be called.

.. index::
   single: Profiler

Profiler
--------

When enabled, the Symfony2 profiler collects useful information about each
request made to your application and store them for later analysis. Use the
profiler in the development environment to help you to debug your code and
enhance performance; use it in the production environment to explore problems
after the fact.

You rarely have to deal with the profiler directly as Symfony2 provides
visualizer tools like the Web Debug Toolbar and the Web Profiler. If you use
the Symfony2 Standard Edition, the profiler, the web debug toolbar, and the
web profiler are all already configured with sensible settings.

.. note::

    The profiler collects information for all requests (simple requests,
    redirects, exceptions, Ajax requests, ESI requests; and for all HTTP
    methods and all formats). It means that for a single URL, you can have
    several associated profiling data (one per external request/response
    pair).

.. index::
   single: Profiler; Visualizing

Visualizing Profiling Data
~~~~~~~~~~~~~~~~~~~~~~~~~~

Using the Web Debug Toolbar
...........................

In the development environment, the web debug toolbar is available at the
bottom of all pages. It displays a good summary of the profiling data that
gives you instant access to a lot of useful information when something does
not work as expected.

If the summary provided by the Web Debug Toolbar is not enough, click on the
token link (a string made of 13 random characters) to access the Web Profiler.

.. note::

    If the token is not clickable, it means that the profiler routes are not
    registered (see below for configuration information).

Analyzing Profiling data with the Web Profiler
..............................................

The Web Profiler is a visualization tool for profiling data that you can use
in development to debug your code and enhance performance; but it can also be
used to explore problems that occur in production. It exposes all information
collected by the profiler in a web interface.

.. index::
   single: Profiler; Using the profiler service

Accessing the Profiling information
...................................

You don't need to use the default visualizer to access the profiling
information. But how can you retrieve profiling information for a specific
request after the fact? When the profiler stores data about a Request, it also
associates a token with it; this token is available in the ``X-Debug-Token``
HTTP header of the Response::

    $profile = $container->get('profiler')->loadProfileFromResponse($response);

    $profile = $container->get('profiler')->loadProfile($token);

.. tip::

    When the profiler is enabled but not the web debug toolbar, or when you
    want to get the token for an Ajax request, use a tool like Firebug to get
    the value of the ``X-Debug-Token`` HTTP header.

Use the ``find()`` method to access tokens based on some criteria::

    // get the latest 10 tokens
    $tokens = $container->get('profiler')->find('', '', 10);

    // get the latest 10 tokens for all URL containing /admin/
    $tokens = $container->get('profiler')->find('', '/admin/', 10);

    // get the latest 10 tokens for local requests
    $tokens = $container->get('profiler')->find('127.0.0.1', '', 10);

If you want to manipulate profiling data on a different machine than the one
where the information were generated, use the ``export()`` and ``import()``
methods::

    // on the production machine
    $profile = $container->get('profiler')->loadProfile($token);
    $data = $profiler->export($profile);

    // on the development machine
    $profiler->import($data);

.. index::
   single: Profiler; Visualizing

Configuration
.............

The default Symfony2 configuration comes with sensible settings for the
profiler, the web debug toolbar, and the web profiler. Here is for instance
the configuration for the development environment:

.. configuration-block::

    .. code-block:: yaml

        # load the profiler
        framework:
            profiler: { only_exceptions: false }

        # enable the web profiler
        web_profiler:
            toolbar: true
            intercept_redirects: true
            verbose: true

    .. code-block:: xml

        <!-- xmlns:webprofiler="http://symfony.com/schema/dic/webprofiler" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/webprofiler http://symfony.com/schema/dic/webprofiler/webprofiler-1.0.xsd"> -->

        <!-- load the profiler -->
        <framework:config>
            <framework:profiler only-exceptions="false" />
        </framework:config>

        <!-- enable the web profiler -->
        <webprofiler:config
            toolbar="true"
            intercept-redirects="true"
            verbose="true"
        />

    .. code-block:: php

        // load the profiler
        $container->loadFromExtension('framework', array(
            'profiler' => array('only-exceptions' => false),
        ));

        // enable the web profiler
        $container->loadFromExtension('web_profiler', array(
            'toolbar' => true,
            'intercept-redirects' => true,
            'verbose' => true,
        ));

When ``only-exceptions`` is set to ``true``, the profiler only collects data
when an exception is thrown by the application.

When ``intercept-redirects`` is set to ``true``, the web profiler intercepts
the redirects and gives you the opportunity to look at the collected data
before following the redirect.

When ``verbose`` is set to ``true``, the Web Debug Toolbar displays a lot of
information. Setting ``verbose`` to ``false`` hides some secondary information
to make the toolbar shorter.

If you enable the web profiler, you also need to mount the profiler routes:

.. configuration-block::

    .. code-block:: yaml

        _profiler:
            resource: @WebProfilerBundle/Resources/config/routing/profiler.xml
            prefix:   /_profiler

    .. code-block:: xml

        <import resource="@WebProfilerBundle/Resources/config/routing/profiler.xml" prefix="/_profiler" />

    .. code-block:: php

        $collection->addCollection($loader->import("@WebProfilerBundle/Resources/config/routing/profiler.xml"), '/_profiler');

As the profiler adds some overhead, you might want to enable it only under
certain circumstances in the production environment. The ``only-exceptions``
settings limits profiling to 500 pages, but what if you want to get
information when the client IP comes from a specific address, or for a limited
portion of the website? You can use a request matcher:

.. configuration-block::

    .. code-block:: yaml

        # enables the profiler only for request coming for the 192.168.0.0 network
        framework:
            profiler:
                matcher: { ip: 192.168.0.0/24 }

        # enables the profiler only for the /admin URLs
        framework:
            profiler:
                matcher: { path: "^/admin/" }

        # combine rules
        framework:
            profiler:
                matcher: { ip: 192.168.0.0/24, path: "^/admin/" }

        # use a custom matcher instance defined in the "custom_matcher" service
        framework:
            profiler:
                matcher: { service: custom_matcher }

    .. code-block:: xml

        <!-- enables the profiler only for request coming for the 192.168.0.0 network -->
        <framework:config>
            <framework:profiler>
                <framework:matcher ip="192.168.0.0/24" />
            </framework:profiler>
        </framework:config>

        <!-- enables the profiler only for the /admin URLs -->
        <framework:config>
            <framework:profiler>
                <framework:matcher path="^/admin/" />
            </framework:profiler>
        </framework:config>

        <!-- combine rules -->
        <framework:config>
            <framework:profiler>
                <framework:matcher ip="192.168.0.0/24" path="^/admin/" />
            </framework:profiler>
        </framework:config>

        <!-- use a custom matcher instance defined in the "custom_matcher" service -->
        <framework:config>
            <framework:profiler>
                <framework:matcher service="custom_matcher" />
            </framework:profiler>
        </framework:config>

    .. code-block:: php

        // enables the profiler only for request coming for the 192.168.0.0 network
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('ip' => '192.168.0.0/24'),
            ),
        ));

        // enables the profiler only for the /admin URLs
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('path' => '^/admin/'),
            ),
        ));

        // combine rules
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('ip' => '192.168.0.0/24', 'path' => '^/admin/'),
            ),
        ));

        # use a custom matcher instance defined in the "custom_matcher" service
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('service' => 'custom_matcher'),
            ),
        ));

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/testing/profiling`
* :doc:`/cookbook/profiler/data_collector`
* :doc:`/cookbook/event_dispatcher/class_extension`
* :doc:`/cookbook/event_dispatcher/method_behavior`

.. _Observer: http://en.wikipedia.org/wiki/Observer_pattern
.. _`Symfony2 HttpKernel component`: https://github.com/symfony/HttpKernel
.. _Closures: http://php.net/manual/en/functions.anonymous.php
.. _`Symfony2 Dependency Injection component`: https://github.com/symfony/DependencyInjection
.. _PHP callable: http://www.php.net/manual/en/language.pseudo-types.php#language.types.callback
