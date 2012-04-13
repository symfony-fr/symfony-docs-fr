.. index::
   single: Dispatcher d'Evènements

Comment personnaliser le Comportement d'une Méthode sans utiliser l'Héritage
============================================================================

Faire quelque chose avant ou après l'Appel d'une Méthode
--------------------------------------------------------

Si vous souhaitez faire quelque chose juste avant, ou juste après qu'une méthode
a été appelée, vous pouvez « dispatcher » un évènement respectivement au
début ou à la fin d'une méthode::

    class Foo
    {
        // ...

        public function send($foo, $bar)
        {
            // faites quelque chose avant le début de la méthode
            $event = new FilterBeforeSendEvent($foo, $bar);
            $this->dispatcher->dispatch('foo.pre_send', $event);

            // récupérez $foo et $bar depuis l'évènement, ils pourraient
            // avoir été modifiés
            $foo = $event->getFoo();
            $bar = $event->getBar();
            // l'implémentation réelle est ici
            // $ret = ...;

            // faites quelque chose après la fin de la méthode
            $event = new FilterSendReturnValue($ret);
            $this->dispatcher->dispatch('foo.post_send', $event);

            return $event->getReturnValue();
        }
    }

Dans cet exemple, deux évènements sont lancés : ``foo.pre_send``, avant que la
méthode soit exécutée, et ``foo.post_send`` après que la méthode est exécutée.
Chacun utilise une classe Event personnalisée pour communiquer des informations
aux listeners des deux évènements. Ces classes d'évènements devraient être créées
par vous-même et devraient permettre, dans cet exemple, aux variables ``$foo``,
``$bar`` et ``$ret`` d'être récupérées et définies par les listeners.

Par exemple, supposons que ``FilterSendReturnValue`` possède une méthode
``setReturnValue``, un listener pourrait alors ressembler à ceci :

.. code-block:: php

    public function onFooPostSend(FilterSendReturnValue $event)
    {
        $ret = $event->getReturnValue();
        // modifie la valeur originale de ``$ret``

        $event->setReturnValue($ret);
    }
