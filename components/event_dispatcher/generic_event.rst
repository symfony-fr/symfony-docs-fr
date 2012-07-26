.. index::
   single: Répartiteur d'Evénement

L'objet Evénement Générique
===========================

.. versionadded:: 2.1
    La classe événement ``GenericEvent`` a été ajoutée dans Symfony 2.1.

La classe de base :class:`Symfony\\Component\\EventDispatcher\\Event` fournie par
le composant ``Event Dispatcher`` est délibérément spartiate afin de permettre
la création d'objets événement spécifiques à une API grâce à l'héritage en utilisant
la POO (« Programmation Orientée Objet »). Cela permet d'avoir du code élégant et
lisible dans des applications complexes.

La classe :class:`Symfony\\Component\\EventDispatcher\\GenericEvent` est
à disposition pour ceux souhaitant utiliser juste un objet événement à
travers leur application. Elle est adaptée à la plupart des besoins
« out of the box » car elle suit le pattern standard de l'observeur où
l'objet événement encapsule un « sujet » événement, tout en ayant la
possibilité d'avoir des arguments additionnels.

La classe :class:`Symfony\\Component\\EventDispatcher\\GenericEvent` possède
une API simple en plus de la classe de base
:class:`Symfony\\Component\\EventDispatcher\\Event` :

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::__construct`:
  Le constructeur prend le sujet de l'événement et n'importe quels arguments ;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::getSubject`:
  Récupère le sujet ;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::setArg`:
  Définit un argument par clé ;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::setArgs`:
  Définit un tableau d'arguments ;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::getArg`:
  Récupère un argument par clé ;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::getArgs`:
  Récupère un tableau d'arguments ;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::hasArg`:
  Retourne « true » si la clé de l'argument existe.

Le ``GenericEvent`` implémente aussi :phpclass:`ArrayAccess` sur les
arguments de l'événement qui fait que cela facilite réellement le passage
d'arguments supplémentaires eu égard au sujet de l'événement.

Les exemples suivants montrent des cas d'école pour vous donner une idée
général de la flexibilité. Les exemples assument que des « listeners »
d'événement ont été ajoutés au répartiteur.

Passer simplement un sujet::

    use Symfony\Component\EventDispatcher\GenericEvent;

    $event = GenericEvent($subject);
    $dispatcher->dispatch('foo', $event);

    class FooListener
    {
        public function handler(GenericEvent $event)
        {
            if ($event->getSubject() instanceof Foo) {
                // ...
            }
        }
    }

Passer et gérer des arguments en utilisant l'API :phpclass:`ArrayAccess` pour
accéder aux arguments de l'événement::

    use Symfony\Component\EventDispatcher\GenericEvent;

    $event = new GenericEvent($subject, array('type' => 'foo', 'counter' => 0)));
    $dispatcher->dispatch('foo', $event);

    echo $event['counter'];

    class FooListener
    {
        public function handler(GenericEvent $event)
        {
            if (isset($event['type']) && $event['type'] === 'foo') {
                // ... faites quelque chose
            }

            $event['counter']++;
        }
    }

Filtrer des données::

    use Symfony\Component\EventDispatcher\GenericEvent;

    $event = new GenericEvent($subject, array('data' => 'foo'));
    $dispatcher->dispatch('foo', $event);

    echo $event['data'];

    class FooListener
    {
        public function filter(GenericEvent $event)
        {
            strtolower($event['data']);
        }
    }
