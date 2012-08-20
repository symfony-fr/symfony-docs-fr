.. index::
   single: Dispatcher d'Evènements

Comment étendre une Classe sans utiliser l'Héritage
===================================================

Pour permettre à plusieurs classes d'ajouter des méthodes à une autre, vous
pouvez définir la méthode magique ``__call()`` dans la classe que vous voulez
étendre comme cela :

.. code-block:: php

    class Foo
    {
        // ...

        public function __call($method, $arguments)
        {
            // crée un évènement nommé 'foo.method_is_not_found'
            $event = new HandleUndefinedMethodEvent($this, $method, $arguments);
            $this->dispatcher->dispatch('foo.method_is_not_found', $event);

            // aucun listener n'a été capable d'analyser l'évènement ? La méthode
            // n'existe pas
            if (!$event->isProcessed()) {
                throw new \Exception(sprintf('Call to undefined method %s::%s.', get_class($this), $method));
            }

            // retourne la valeur retournée par le listener
            return $event->getReturnValue();
        }
    }

Cela utilise un évènement ``HandleUndefinedMethodEvent`` spécial qui devrait
aussi être créé. C'est en fait une classe générique qui pourrait être réutilisée
chaque fois que vous avez besoin de ce pattern d'extension de classe :

.. code-block:: php

    use Symfony\Component\EventDispatcher\Event;

    class HandleUndefinedMethodEvent extends Event
    {
        protected $subject;
        protected $method;
        protected $arguments;
        protected $returnValue;
        protected $isProcessed = false;

        public function __construct($subject, $method, $arguments)
        {
            $this->subject = $subject;
            $this->method = $method;
            $this->arguments = $arguments;
        }

        public function getSubject()
        {
            return $this->subject;
        }

        public function getMethod()
        {
            return $this->method;
        }

        public function getArguments()
        {
            return $this->arguments;
        }

        /**
         * Définit la valeur à retourner et stoppe les autres listeners allant
         * être notifiés
         */
        public function setReturnValue($val)
        {
            $this->returnValue = $val;
            $this->isProcessed = true;
            $this->stopPropagation();
        }

        public function getReturnValue($val)
        {
            return $this->returnValue;
        }

        public function isProcessed()
        {
            return $this->isProcessed;
        }
    }

Ensuite, créez une classe qui va écouter l'évènement ``foo.method_is_not_found``
et *ajoutez* la méthode ``bar()`` :

.. code-block:: php

    class Bar
    {
        public function onFooMethodIsNotFound(HandleUndefinedMethodEvent $event)
        {
            // nous voulons répondre seulement aux appels de la méthode 'bar'
            if ('bar' != $event->getMethod()) {
                // autorise un autre listener à prendre en charge cette méthode
                // inconnue
                return;
            }

            // le sujet de l'objet (l'instance foo)
            $foo = $event->getSubject();

            // les arguments de la méthode bar
            $arguments = $event->getArguments();

            // faites quelque chose
            // ...

            // définit la valeur retournée
            $event->setReturnValue($someValue);
        }
    }

Finalement, ajoutez la nouvelle méthode ``bar`` à la classe ``Foo`` en
déclarant une instance de ``Bar`` avec l'évènement ``foo.method_is_not_found`` :

.. code-block:: php

    $bar = new Bar();
    $dispatcher->addListener('foo.method_is_not_found', array($bar, 'onFooMethodIsNotFound'));