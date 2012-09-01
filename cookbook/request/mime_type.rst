.. index::
   single: Requête; Ajouter un format de requête et un type mime

Comment déclarer un nouveau Format de Requête et un Type Mime
=============================================================

Chaque ``Requête`` a un « format » (par exemple : ``html``, ``json``), qui
est utilisé pour déterminer quel type de contenu retourner dans la ``Réponse``.
En fait, le format de la requête, accessible via la méthode
:method:`Symfony\\Component\\HttpFoundation\\Request::getRequestFormat`,
est utilisé pour définir le type MIME de l'en-tête ``Content-Type`` de
l'objet ``Response``. En interne, Symfony contient un tableau des formats
les plus communs (par exemple : ``text/html``, ``application/json``). Bien
sûr, des types de format MIME additionnels peuvent aisément être ajoutés.
Ce document va vous montrer comment vous pouvez ajouter le format ``jsonp``
ainsi que le type MIME correspondant.

Créez un Listener ``kernel.request``
------------------------------------

La solution pour définir un nouveau type MIME est de créer une classe qui va
« écouter » l'évènement ``kernel.request`` « dispatché » (« réparti » en français)
par le kernel de Symfony. L'évènement ``kernel.request`` est dispatché très tôt dans
le processus de gestion de la requête de Symfony et vous permet de modifier
l'objet requête.

Créez la classe suivante, en remplaçant le chemin par un chemin vers un bundle de
votre projet::

    // src/Acme/DemoBundle/RequestListener.php
    namespace Acme\DemoBundle;

    use Symfony\Component\HttpKernel\HttpKernelInterface;
    use Symfony\Component\HttpKernel\Event\GetResponseEvent;

    class RequestListener
    {
        public function onKernelRequest(GetResponseEvent $event)
        {
            $event->getRequest()->setFormat('jsonp', 'application/javascript');
        }
    }

Déclarer votre Listener
-----------------------

Comme pour n'importe quel autre listener, vous avez besoin de l'ajouter dans l'un
de vos fichiers de configuration et de le déclarer comme listener en ajoutant le
tag ``kernel.event_listener`` :

.. configuration-block::

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">
            <services>
            <service id="acme.demobundle.listener.request" class="Acme\DemoBundle\RequestListener">
                <tag name="kernel.event_listener" event="kernel.request" method="onKernelRequest" />
            </service>
            </services>
        </container>

    .. code-block:: yaml

        # app/config/config.yml
        services:
            acme.demobundle.listener.request:
                class: Acme\DemoBundle\RequestListener
                tags:
                    - { name: kernel.event_listener, event: kernel.request, method: onKernelRequest }

    .. code-block:: php

        # app/config/config.php
        $definition = new Definition('Acme\DemoBundle\RequestListener');
        $definition->addTag('kernel.event_listener', array('event' => 'kernel.request', 'method' => 'onKernelRequest'));
        $container->setDefinition('acme.demobundle.listener.request', $definition);

A ce stade, le service ``acme.demobundle.listener.request`` a été configuré
et sera notifié lorsque le kernel de Symfony dispatchera l'évènement
``kernel.request``.

.. tip::

    Vous pouvez aussi déclarer le listener dans une configuration de classe
    extension (voir :ref:`service-container-extension-configuration` pour
    plus d'informations).
