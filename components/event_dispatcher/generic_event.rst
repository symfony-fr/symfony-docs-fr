.. index::
   single: Répartiteur d'Évènement

L'objet Évènement Générique
===========================

.. versionadded:: 2.1
    La classe évènement ``GenericEvent`` a été ajoutée dans Symfony 2.1.

La classe de base :class:`Symfony\\Component\\EventDispatcher\\Event` fournie par
le composant ``Event Dispatcher`` est délibérément spartiate afin de permettre
la création d'objets évènement spécifiques à une API grâce à l'héritage en utilisant
la POO (« Programmation Orientée Objet »). Cela permet d'avoir du code élégant et
lisible dans des applications complexes.

La classe :class:`Symfony\\Component\\EventDispatcher\\GenericEvent` est
à disposition pour ceux qui souhaitent juste utiliser un objet évènement à
travers leur application. Elle est adaptée à la plupart des besoins
« out of the box » car elle suit le pattern standard de l'observeur où
l'objet évènement encapsule un « sujet » évènement, tout en ayant la
possibilité d'avoir des arguments additionnels.

La classe :class:`Symfony\\Component\\EventDispatcher\\GenericEvent` possède
une API simple en plus de la classe de base
:class:`Symfony\\Component\\EventDispatcher\\Event` :

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::__construct`:
  Le constructeur prend le sujet de l'évènement et n'importe quels arguments ;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::getSubject`:
  Récupère le sujet ;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::setArgument`:
  Définit un argument par clé ;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::setArguments`:
  Définit un tableau d'arguments ;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::getArgument`:
  Récupère un argument par clé ;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::getArguments`:
  Récupère un tableau d'arguments ;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::hasArgument`:
  Retourne « true » si la clé de l'argument existe.

Le ``GenericEvent`` implémente aussi :phpclass:`ArrayAccess` sur les
arguments de l'évènement qui fait que cela facilite réellement le passage
d'arguments supplémentaires eu égard au sujet de l'évènement.

Les exemples suivants montrent des cas d'école pour vous donner une idée
générale de la flexibilité. Les exemples supposent que des « listeners »
d'évènement ont été ajoutés au répartiteur.

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
accéder aux arguments de l'évènement::

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
