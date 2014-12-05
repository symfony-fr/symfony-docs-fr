.. index::
   single: Event Dispatcher
   single: Components; EventDispatcher

Le Composant répartiteur d'évènement
====================================

Introduction
------------

La programmation Orientée Objet a parcouru du chemin afin de vous assurer une
certaine extensibilité. En créant des classes qui ont des responsabilités bien
définies, votre code devient plus flexible et un développeur peut les étendre
avec des sous-classes pour modifier leurs comportements. Mais s'il souhaite
partager ses changements avec d'autres développeurs qui ont aussi créé leurs
propres sous-classes, l'héritage de code n'est plus la bonne réponse.

Considérez l'exemple réel suivant : vous souhaitez fournir un système de plugin
pour votre projet. Un plugin devrait être capable d'ajouter des méthodes, ou
de faire quelque chose avant ou après qu'une méthode soit exécutée, sans
interférer avec d'autres plugins. Ceci n'est pas un problème facile à
résoudre avec de l'héritage simple, et l'héritage multiple (s'il était
possible avec PHP) possède ses propres inconvénients.

Le composant Répartiteur d'Évènement de Symfony2 implémente le pattern
`Mediator` d'une manière simple et efficace pour rendre toutes ces
choses possibles et pour réaliser des projets vraiment extensibles.

Prenez un exemple simple du `composant HttpKernel de Symfony2`_. Une fois
qu'un objet ``Response`` a été créé, il pourrait être utile d'autoriser la
modification de ce dernier par d'autres éléments du système (par exemple :
ajouter des en-têtes de cache) avant que cet objet ne soit utilisé. Pour rendre
cela possible, le noyau Symfony2 lance un évènement - ``kernel.response``.
Voilà comment cela fonctionne :

* Un *listener* ou *écouteur* en français (objet PHP) informe un objet
  *répartiteur* central qu'il souhaite écouter l'évènement ``kernel.response`` ;

* A un moment donné, le noyau Symfony2 dit à l'objet *répartiteur* de « répartir »
  (« dispatch » en anglais) l'évènement ``kernel.response``, en passant avec lui un
  objet ``Event`` qui a accès à l'objet ``Response`` ;

* Le répartiteur notifie (c-a-d appelle une méthode) tous les « listeners » de
  l'évènement ``kernel.response``, autorisant chacun d'entre eux à effectuer
  des modifications sur l'objet ``Response``.

.. index::
   single: Event Dispatcher; Events

Installation
------------

Vous pouvez installer le composant de différentes manières :

* Utilisez le dépôt Git officiel (https://github.com/symfony/EventDispatcher);
* Installez le via Composer (``symfony/event-dispatcher`` sur `Packagist`_).

Utilisation
-----------

Évènements
~~~~~~~~~~

Lorsqu'un évènement est réparti, il est identifié par un nom unique (par
exemple : ``kernel.response``), que plusieurs « listeners » peuvent écouter.
Une instance de :class:`Symfony\\Component\\EventDispatcher\\Event` est
aussi créée et passée à tous les « listeners ». Comme vous le verrez plus
tard, l'objet ``Event`` lui-même contient souvent des données à propos de
l'évènement qui est réparti.

.. index::
   pair: Event Dispatcher; Naming conventions

Conventions de Nommage
......................

Le nom d'évènement unique peut être n'importe quelle chaîne de caractères,
mais suit quelques conventions de nommage simples mais optionnelles :

* utilisez uniquement des lettres minuscules, des nombres, des points (``.``),
  et des tirets du bas (``_``) ;

* préfixez les noms par un espace de noms suivi d'un point (par exemple :
  ``kernel.``) ;

* terminez les noms par un verbe qui indique quelle action est en train d'être
  effectuée (par exemple : ``request``).

Voici quelques exemples de nom d'évènement correct :

* ``kernel.response``
* ``form.pre_set_data``

.. index::
   single: Event Dispatcher; Event subclasses

Noms d'évènement et objets évènements
.....................................

Lorsque le répartiteur notifie les « listeners », il passe un objet ``Event``
à ces derniers. La classe de base ``Event`` est très simple : elle contient
une méthode pour stopper la :ref:`propagation de l'évènement
<event_dispatcher-event-propagation>`, et rien de plus.

Souvent, des données à propos de l'évènement spécifique ont besoin d'être
passées avec l'objet ``Event`` afin que les « listeners » possèdent l'information
nécessaire. Dans le cas de l'évènement ``kernel.response``, l'objet ``Event``
qui est créé et passé à chaque « listener » est en fait de type
:class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`, une sous-classe
de l'objet ``Event`` de base. Cette classe contient des méthodes telles que
``getResponse`` et ``setResponse``, permettant aux « listeners » de récupérer
ou même de remplacer l'objet ``Response``.

La morale de l'histoire est la suivante : lorsque vous créez un « listener » d'un
évènement, l'objet ``Event`` qui est passé au « listener » peut être une
sous-classe spéciale qui possède des méthodes additionnelles pour récupérer
de l'information et répondre à l'évènement.

Le Répartiteur
~~~~~~~~~~~~~~

Le répartiteur est l'objet central du système de répartition d'évènement.
En général, un répartiteur unique est créé, et qui maintient un registre des
« listeners ». Lorsqu'un évènement est réparti via le répartiteur, il
notifie tous les « listeners » qui se sont enregistrés auprès de cet évènement::

    use Symfony\Component\EventDispatcher\EventDispatcher;

    $dispatcher = new EventDispatcher();

.. index::
   single: Event Dispatcher; Listeners

Connecter des Listeners
~~~~~~~~~~~~~~~~~~~~~~~

Pour tirer parti d'un évènement existant, vous avez besoin de connecter
un « listener » au répartiteur afin qu'il soit notifié lorsque l'évènement
est réparti. Un appel à la méthode ``addListener()`` du répartiteur associe
n'importe quel « callable » PHP valide à un évènement::

    $listener = new AcmeListener();
    $dispatcher->addListener('foo.action', array($listener, 'onFooAction'));

La méthode ``addListener()`` prend jusqu'à trois arguments :

* Le nom de l'évènement (chaîne de caractères) auquel ce « listener »
  souhaite se connecter ;

* Un « callable » PHP qui sera notifié lorsqu'un évènement qu'il écoute est
  lancé ;

* Un nombre entier optionnel faisant office de priorité (plus grand signifie
  plus important) qui détermine quand un « listener »  est exécuté par rapport
  à d'autres « listeners » (vaut par défaut ``0``). Si deux « listeners » ont
  la même priorité, ils sont exécutés dans l'ordre dans lequel ils ont été
  ajoutés au répartiteur.

.. note::

    Un `callable PHP`_ est une variable PHP qui peut être utilisée par la
    fonction ``call_user_func()`` et qui retourne ``true`` lorsque passée
    à la fonction ``is_callable()``. Cela peut être une instance de
    ``\Closure``, un objet implémentant la méthode __invoke (qui est en fait
    ce que les closures font), une chaîne de caractères représentant une fonction,
    ou un tableau représentant une méthode d'objet ou une méthode de classe.

    Jusqu'ici, vous avez vu comment des objets PHP peuvent être enregistrés
    comme des « listeners ». Vous pouvez aussi enregistrer des `Closures`_ PHP
    en tant que « listeners »::

        use Symfony\Component\EventDispatcher\Event;

        $dispatcher->addListener('foo.action', function (Event $event) {
            // sera exécuté lorsque l'évènement foo.action est réparti
        });

Une fois qu'un « listener » est enregistré dans le répartiteur, il attend que
l'évènement soit notifié. Dans l'exemple ci-dessus, lorsque l'évènement ``foo.action``
est réparti, le répartiteur appelle la méthode ``AcmeListener::onFooAction`` et
lui passe l'objet ``Event`` en tant qu'argument unique::

    use Symfony\Component\EventDispatcher\Event;

    class AcmeListener
    {
        // ...

        public function onFooAction(Event $event)
        {
            // faire quelque chose ici
        }
    }

Dans beaucoup de cas, une sous-classe spéciale d'``Event`` qui est spécifique
à l'évènement donné est passée au « listener ». Cela permet au « listener »
d'accéder à des informations spéciales concernant l'évènement. Jetez un oeil
à la documentation ou à l'implémentation de chaque évènement pour déterminer
l'instance exacte de ``Symfony\Component\EventDispatcher\Event`` qui est passée.
Par exemple, l'évènement ``kernel.event`` passe une instance de
``Symfony\Component\HttpKernel\Event\FilterResponseEvent``::

    use Symfony\Component\HttpKernel\Event\FilterResponseEvent

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $response = $event->getResponse();
        $request = $event->getRequest();

        // ...
    }

.. _event_dispatcher-closures-as-listeners:

.. index::
   single: Event Dispatcher; Creating and dispatching an event

Créer et répartir un évènement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En plus d'enregistrer des « listeners » auprès d'évènements existants, vous
pouvez créer et répartir vos propres évènements. Cela est utile lorsque vous
créez des bibliothèques tierces ainsi que lorsque vous souhaitez garder différents
composants de votre propre système flexibles et découplés.

La classe statique ``Events``
.............................

Supposons que vous vouliez créer un nouvel évènement - ``store.order`` - qui
est lancé chaque fois qu'une commande est créée dans votre application. Pour
garder les choses organisées, commencez par créer une classe ``StoreEvents``
dans votre application qui sert à définir et documenter votre évènement::

    namespace Acme\StoreBundle;

    final class StoreEvents
    {
        /**
         * L'évènement store.order est lancé chaque fois qu'une commande
         * est créée dans le système.
         *
         * Le « listener » de l'évènement reçoit une instance de
         * Acme\StoreBundle\Event\FilterOrderEvent
         *
         * @var string
         */
        const STORE_ORDER = 'store.order';
    }

Notez que cette classe n'effectue en fait *aucune* action. Le but de la classe
``StoreEvents`` est simplement d'avoir un endroit où l'information à propos
d'évènements communs puisse être centralisée. Notez aussi qu'une classe
spéciale ``FilterOrderEvent`` sera passée à chacun des « listeners » de
cet évènement.

Créer un objet « Event »
........................

Plus tard, lorsque vous répartirez ce nouvel évènement, vous allez créer une
instance de ``Event`` et la passer au répartiteur. Ce dernier passe cette
même instance à chacun des « listeners » de l'évènement. Si vous n'avez pas à
passer d'information à vos « listeners », vous pouvez utiliser la classe par
défaut ``Symfony\Component\EventDispatcher\Event``. La plupart du temps,
cependant, vous *aurez besoin* de passer de l'information concernant l'évènement
à chaque « listener ». Pour accomplir cela, vous allez créer une nouvelle
classe qui étend ``Symfony\Component\EventDispatcher\Event``.

Dans cet exemple, chaque « listener » va avoir besoin d'accéder à un
prétendu objet ``Order``. Créez une classe ``Event`` qui rendra cela possible::

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

Chaque « listener » a maintenant accès à l'objet ``Order`` via la méthode
``getOrder``.

Répartir l'évènement
....................

La méthode :method:`Symfony\\Component\\EventDispatcher\\EventDispatcher::dispatch`
notifie tous les « listeners » de l'évènement donné. Elle prend deux arguments :
le nom de l'évènement à répartir et l'instance d'``Event`` à passer à chacun des
« listeners » de cet évènement::

    use Acme\StoreBundle\StoreEvents;
    use Acme\StoreBundle\Order;
    use Acme\StoreBundle\Event\FilterOrderEvent;

    // la commande est d'une façon ou d'une autre créée ou récupérée
    $order = new Order();
    // ...

    // crée le FilterOrderEvent et le répartit
    $event = new FilterOrderEvent($order);
    $dispatcher->dispatch(StoreEvents::STORE_ORDER, $event);

Notez que l'objet spécifique ``FilterOrderEvent`` est créé et passé à la
méthode ``dispatch``. Maintenant, tout « listener » de l'évènement
``store.order`` va recevoir le ``FilterOrderEvent`` et avoir accès à
l'objet ``Order`` via la méthode ``getOrder``::

    // quelconque classe « listener » qui a été enregistrée pour l'évènement "STORE_ORDER"
    use Acme\StoreBundle\Event\FilterOrderEvent;

    public function onStoreOrder(FilterOrderEvent $event)
    {
        $order = $event->getOrder();
        // faites quelque chose avec ou sur la commande
    }

.. index::
   single: Event Dispatcher; Event subscribers

.. _event_dispatcher-using-event-subscribers:

Utiliser les souscripteurs d'évènement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La manière la plus commune d'écouter un évènement est d'enregistrer un
*« listener » d'évènement* avec le répartiteur. Ce « listener » peut
écouter un ou plusieurs évènements et est notifié chaque fois que ces
évènements sont répartis.

Une autre façon d'écouter des évènements est via un *souscripteur d'évènement*.
Un souscripteur d'évènement est une classe PHP qui est capable de dire au
répartiteur exactement à quels évènements elle souhaite s'inscrire. Elle
implémente l'interface
:class:`Symfony\\Component\\EventDispatcher\\EventSubscriberInterface`,
qui requiert une unique méthode nommée ``getSubscribedEvents``. Prenez
l'exemple suivant d'un souscripteur qui s'inscrit aux évènements
``kernel.response`` et ``store.order``::

    namespace Acme\StoreBundle\Event;

    use Symfony\Component\EventDispatcher\EventSubscriberInterface;
    use Symfony\Component\HttpKernel\Event\FilterResponseEvent;

    class StoreSubscriber implements EventSubscriberInterface
    {
        static public function getSubscribedEvents()
        {
            return array(
                'kernel.response' => array(
                    array('onKernelResponsePre', 10),
                    array('onKernelResponseMid', 5),
                    array('onKernelResponsePost', 0),
                ),
                'store.order'     => array('onStoreOrder', 0),
            );
        }

        public function onKernelResponsePre(FilterResponseEvent $event)
        {
            // ...
        }

        public function onKernelResponseMid(FilterResponseEvent $event)
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

Ceci est très similaire à une classe « listener », excepté que la classe
elle-même peut dire au répartiteur quels évènements il devrait écouter.
Pour enregistrer un souscripteur dans le répartiteur, utilisez la méthode
:method:`Symfony\\Component\\EventDispatcher\\EventDispatcher::addSubscriber`::

    use Acme\StoreBundle\Event\StoreSubscriber;

    $subscriber = new StoreSubscriber();
    $dispatcher->addSubscriber($subscriber);

Le répartiteur va automatiquement enregistrer le souscripteur pour chaque
évènement retourné par la méthode ``getSubscribedEvents``. Cette méthode
retourne un tableau indexé par les noms des évènements et dont les valeurs
sont soit le nom de la méthode à appeler ou soit un tableau composé de la
méthode à appeler et d'une priorité. L'exemple ci-dessus montre comment
enregistrer plusieurs méthodes de « listener » pour le même évènement
dans le souscripteur et montre aussi comment passer la priorité de chaque
méthode du « listener ».

.. index::
   single: Event Dispatcher; Stopping event flow

.. _event_dispatcher-event-propagation:

Arrêter le déroulement/la propagation d'évènements
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans certains cas, cela peut être utile pour un « listener » d'empêcher
n'importe quel(s) autre(s) « listener(s) » d'être appelé(s). En d'autres termes,
le « listener » a besoin de pouvoir dire au répartiteur de stopper toute
propagation de l'évènement aux prochains « listeners » (c-a-d de ne plus notifier
d'autres « listeners »). Ceci peut être accompli depuis l'intérieur du « listener »
via la méthode :method:`Symfony\\Component\\EventDispatcher\\Event::stopPropagation`::

   use Acme\StoreBundle\Event\FilterOrderEvent;

   public function onStoreOrder(FilterOrderEvent $event)
   {
       // ...

       $event->stopPropagation();
   }

Maintenant, tout « listener » de ``store.order`` qui n'a pas encore
été appelé *ne* sera *pas* appelé.

Il est possible de détecter si un évènement a été stoppé en utilisant la méthode
:method:`Symfony\\Component\\EventDispatcher\\Event::isPropagationStopped`
qui retourne une valeur booléenne::

    $dispatcher->dispatch('foo.event', $event);
    if ($event->isPropagationStopped()) {
        // ...
    }

.. index::
   single: Event Dispatcher; Event Dispatcher aware events and listeners

.. _event_dispatcher-dispatcher-aware-events:

Évènements et « Listeners » connaissant le répartiteur d'évènements
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    L'objet ``Event`` contient une référence au répartiteur l'ayant invoqué depuis
    Symfony 2.1.

Le ``Répartiteur d'Évènement`` injecte toujours une référence de lui-même dans
l'objet « évènement » passé. Cela signifie que tous les « listeners » ont un
accès direct à l'objet ``EventDispatcher`` qui a notifié le « listener » via
la méthode de l'objet ``Event`` passé
:method:`Symfony\\Component\\EventDispatcher\\Event::getDispatcher`.

Cela peut amener à certaines utilisations avancées de l'``EventDispatcher`` incluant
le fait de laisser des « listeners » répartir d'autres évènements, le chaînage
d'évènements ou même le « chargement fainéant » (« lazy loading » en anglais)
de plus de « listeners » dans l'objet répartiteur. Voyez les exemples suivants :

« Chargement fainéant » de « listeners »::

    use Symfony\Component\EventDispatcher\Event;
    use Acme\StoreBundle\Event\StoreSubscriber;

    class Foo
    {
        private $started = false;

        public function myLazyListener(Event $event)
        {
            if (false === $this->started) {
                $subscriber = new StoreSubscriber();
                $event->getDispatcher()->addSubscriber($subscriber);
            }

            $this->started = true;

            // ... plus de code
        }
    }

Répartir un autre évènement depuis un « listener »::

    use Symfony\Component\EventDispatcher\Event;

    class Foo
    {
        public function myFooListener(Event $event)
        {
            $event->getDispatcher()->dispatch('log', $event);

            // ... plus de code
        }
    }

Bien que le code ci-dessus soit suffisant dans la plupart des cas, si votre
application utilise plusieurs instances d'``EventDispatcher``, vous pourriez
avoir besoin d'injecter une instance spécifiquement connue de l'``EventDispatcher``
dans vos « listeners ». Cela pourrait être effectué en utilisant l'injection
via constructeur ou « setter » comme suit :

Injection via le constructeur::

    use Symfony\Component\EventDispatcher\EventDispatcherInterface;

    class Foo
    {
        protected $dispatcher = null;

        public function __construct(EventDispatcherInterface $dispatcher)
        {
            $this->dispatcher = $dispatcher;
        }
    }

Ou injection via « setter »::

    use Symfony\Component\EventDispatcher\EventDispatcherInterface;

    class Foo
    {
        protected $dispatcher = null;

        public function setEventDispatcher(EventDispatcherInterface $dispatcher)
        {
            $this->dispatcher = $dispatcher;
        }
    }

Choisir entre les deux est une question de goût. Beaucoup préfèrent l'injection
via le constructeur car les objets sont totalement initialisés au moment de la
construction. Mais lorsque vous avez une longue liste de dépendances, utiliser
une injection via « setter » peut être la manière de faire, et plus particulièrement
lorsqu'il s'agit de dépendances optionnelles.

.. index::
   single: Event Dispatcher; Dispatcher shortcuts

.. _event_dispatcher-shortcuts:

Raccourcis du Répartiteur
~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    La méthode ``EventDispatcher::dispatch()`` retourne l'évènement depuis
    Symfony 2.1.

La méthode
:method:`EventDispatcher::dispatch<Symfony\\Component\\EventDispatcher\\EventDispatcher::dispatch>`
retourne toujours un objet :class:`Symfony\\Component\\EventDispatcher\\Event`.
Cela permet d'utiliser plusieurs raccourcis. Par exemple, si vous n'avez pas besoin
d'avoir un objet évènement personnalisé, vous pouvez simplement utiliser un
objet :class:`Symfony\\Component\\EventDispatcher\\Event`. Vous ne devez même
pas passer ce dernier au répartiteur car il va en créer un par défaut à moins
que vous ne lui en passiez un spécifiquement::

    $dispatcher->dispatch('foo.event');

De plus, l'« EventDisptacher » retourne toujours tout évènement qui a
été réparti, c'est-a-dire soit l'évènement qui a été passé, soit l'évènement qui a été
créé en interne pas le répartiteur. Cela permet d'utiliser des raccourcis
sympas::

    if (!$dispatcher->dispatch('foo.event')->isPropagationStopped()) {
        // ...
    }

Ou::

    $barEvent = new BarEvent();
    $bar = $dispatcher->dispatch('bar.event', $barEvent)->getBar();

Ou::

    $response = $dispatcher->dispatch('bar.event', new BarEvent())->getBar();

et ainsi de suite...

.. index::
   single: Event Dispatcher; Event name introspection

.. _event_dispatcher-event-name-introspection:

Introspection du nom de l'évènement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    Le nom de l'évènement a été ajouté à l'objet ``Event`` depuis Symfony 2.1.

Comme l'``EventDispatcher`` connaît déjà le nom de l'évènement lorsqu'il le
répartit, le nom de l'évènement est aussi injecté dans les objets
:class:`Symfony\\Component\\EventDispatcher\\Event`, le rendant disponible aux
« listeners » d'évènement via la méthode
:method:`Symfony\\Component\\EventDispatcher\\Event::getName`.

Le nom de l'évènement (comme pour n'importe quelle autre donnée dans un objet
évènement personnalisé) peut être utilisé à part entière dans la logique
d'exécution du « listener »::

    use Symfony\Component\EventDispatcher\Event;

    class Foo
    {
        public function myEventListener(Event $event)
        {
            echo $event->getName();
        }
    }

.. _Observer: http://fr.wikipedia.org/wiki/Observateur_(patron_de_conception)
.. _`composant HttpKernel de Symfony2`: https://github.com/symfony/HttpKernel
.. _Closures: http://www.php.net/manual/fr/functions.anonymous.php
.. _callable PHP: http://www.php.net/manual/fr/language.pseudo-types.php#language.types.callback
.. _Packagist: https://packagist.org/packages/symfony/event-dispatcher
