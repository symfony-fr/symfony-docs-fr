Les Tags de l'Injection de Dépendance
====================================

Tags:

* ``data_collector``
* ``form.type``
* ``form.type_extension``
* ``form.type_guesser``
* ``kernel.cache_warmer``
* ``kernel.event_listener``
* ``kernel.event_subscriber``
* ``monolog.logger``
* ``monolog.processor``
* ``templating.helper``
* ``routing.loader``
* ``translation.loader``
* ``twig.extension``
* ``validator.initializer``

Activer les helpers de template PHP personnalisés
-------------------------------------------------

Pour activer un helper de template personnalisé, ajoutez le comme service dans 
votre configuration, taguez avec ``templating.helper`` puis définissez un attribut
``alias`` (le helper sera accessible via cet alias dans les templates) :

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.helper.your_helper_name:
                class: Fully\Qualified\Helper\Class\Name
                tags:
                    - { name: templating.helper, alias: alias_name }

    .. code-block:: xml

        <service id="templating.helper.your_helper_name" class="Fully\Qualified\Helper\Class\Name">
            <tag name="templating.helper" alias="alias_name" />
        </service>

    .. code-block:: php

        $container
            ->register('templating.helper.your_helper_name', 'Fully\Qualified\Helper\Class\Name')
            ->addTag('templating.helper', array('alias' => 'alias_name'))
        ;

.. _reference-dic-tags-twig-extension:

Activer les extension Twig personnalisées
-----------------------------------------

Pour activer une extension Twig, ajoutez la comme service dans l'une de vos
configurations, et taguez la avec ``twig.extension`` :

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.your_extension_name:
                class: Fully\Qualified\Extension\Class\Name
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <service id="twig.extension.your_extension_name" class="Fully\Qualified\Extension\Class\Name">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $container
            ->register('twig.extension.your_extension_name', 'Fully\Qualified\Extension\Class\Name')
            ->addTag('twig.extension')
        ;

Pour plus d'informations sur comment créer une classe d'extension Twig, lisez
la `documentation Twig`_ sur ce sujet.


.. _dic-tags-kernel-event-listener:

Activer les Listeners personnalisés
-----------------------------------

Pour activer un listener personnalisé, ajoutez le comme service dans l'une de vos
configurations, et taguez le avec ``kernel.event_listener``. Vous devez spécifier
le nom de l'évènement que votre service écoute, tout comme la méthode qui sera
appelée :

.. configuration-block::

    .. code-block:: yaml

        services:
            kernel.listener.your_listener_name:
                class: Fully\Qualified\Listener\Class\Name
                tags:
                    - { name: kernel.event_listener, event: xxx, method: onXxx }

    .. code-block:: xml

        <service id="kernel.listener.your_listener_name" class="Fully\Qualified\Listener\Class\Name">
            <tag name="kernel.event_listener" event="xxx" method="onXxx" />
        </service>

    .. code-block:: php

        $container
            ->register('kernel.listener.your_listener_name', 'Fully\Qualified\Listener\Class\Name')
            ->addTag('kernel.event_listener', array('event' => 'xxx', 'method' => 'onXxx'))
        ;

.. note::

    Vous pouvez aussi spécifier la priorité comme attribut du tag kernel.event_listener 
    (un peu comme la méthode ou les attributs de l'évènement), avec un valeur entière
    positive ou négative. Cela permettra à votre listener d'être toujours appelé
    avant ou après un autre listerner qui écoute le même évènement.


.. _dic-tags-kernel-event-subscriber:

Activer les abonnements personnalisés
-------------------------------------

.. versionadded:: 2.1
   
   Le fait d'ajouter des abonnements (subscribers) à l'évènement kernel est apparu
   avec la version 2.1.

Pour activer un abonnement, ajoutez le comme service dans l'une de vos configurations,
et taguez le avec ``kernel.event_subscriber`` :

.. configuration-block::

    .. code-block:: yaml

        services:
            kernel.subscriber.your_subscriber_name:
                class: Fully\Qualified\Subscriber\Class\Name
                tags:
                    - { name: kernel.event_subscriber }

    .. code-block:: xml

        <service id="kernel.subscriber.your_subscriber_name" class="Fully\Qualified\Subscriber\Class\Name">
            <tag name="kernel.event_subscriber" />
        </service>

    .. code-block:: php

        $container
            ->register('kernel.subscriber.your_subscriber_name', 'Fully\Qualified\Subscriber\Class\Name')
            ->addTag('kernel.event_subscriber')
        ;

.. note::

    Votre service doit implémenter l'interface :class:`Symfony\Component\EventDispatcher\EventSubscriberInterface`.

.. note::

    Si votre service est créé par un factory, vous **DEVEZ** définir correctement
    le paramètre ``class`` pour que ce tag fonctionne bien.

Activer les moteurs de Template personnalisés
---------------------------------------------

Pour activer un moteur de template personnalisé, ajoutez le comme service dans l'une
de vos configurations puis taguez le avec ``templating.engine`` :

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.engine.your_engine_name:
                class: Fully\Qualified\Engine\Class\Name
                tags:
                    - { name: templating.engine }

    .. code-block:: xml

        <service id="templating.engine.your_engine_name" class="Fully\Qualified\Engine\Class\Name">
            <tag name="templating.engine" />
        </service>

    .. code-block:: php

        $container
            ->register('templating.engine.your_engine_name', 'Fully\Qualified\Engine\Class\Name')
            ->addTag('templating.engine')
        ;

Activer un chargeur de routes personnalisé
------------------------------------------

Pour activer un chargeur de routes personnalisé, ajoutez le comme service dans l'une
de vos configurations puis taguez le avec ``routing.loader`` :

.. configuration-block::

    .. code-block:: yaml

        services:
            routing.loader.your_loader_name:
                class: Fully\Qualified\Loader\Class\Name
                tags:
                    - { name: routing.loader }

    .. code-block:: xml

        <service id="routing.loader.your_loader_name" class="Fully\Qualified\Loader\Class\Name">
            <tag name="routing.loader" />
        </service>

    .. code-block:: php

        $container
            ->register('routing.loader.your_loader_name', 'Fully\Qualified\Loader\Class\Name')
            ->addTag('routing.loader')
        ;

.. _dic_tags-monolog:

Utiliser un canal de logging personnalisé avec Monolog
------------------------------------------------------

Monolog vous permet de partager ses handlers entre différents canaux de logging.
Le service logger utilise le canal ``app`` mais vous pouvez le changer au moment
d'injecter le logger dans un service.

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Fully\Qualified\Loader\Class\Name
                arguments: [@logger]
                tags:
                    - { name: monolog.logger, channel: acme }

    .. code-block:: xml

        <service id="my_service" class="Fully\Qualified\Loader\Class\Name">
            <argument type="service" id="logger" />
            <tag name="monolog.logger" channel="acme" />
        </service>

    .. code-block:: php

        $definition = new Definition('Fully\Qualified\Loader\Class\Name', array(new Reference('logger'));
        $definition->addTag('monolog.logger', array('channel' => 'acme'));
        $container->register('my_service', $definition);;

.. note::

    Cela fonctionne uniquement lorsque le service logger est un argument de constructeur,
    et non pas lorsqu'il est injecté via un setter.

.. _dic_tags-monolog-processor:


Ajouter un processeur pour Monolog
----------------------------------

Monolog vous autorise à ajouter des processeurs dans le logger ou dans les handlers
pour ajouter des données en plus dans les enregistrements. Un processeur recoit
l'enregistrement comme argument et doit le retourner après avoir ajouté des données
en plus dans l'attribut ``extra``.

Voyons comment vous pouvez utiliser ``IntrospectionProcessor`` pour ajouter
le fichier, la ligne, la classe et la méthode où le logger a été déclenché.
Vous pouvez ajouter un processeur de façon globale :

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor');
        $container->register('my_service', $definition);

.. tip::

    Si votre service n'est pas appelable (en utilisant``__invoke``) vous pouvez
    ajouter l'attribut ``method`` dans  le tag pour utiliser une méthode spécifique.

Vous pouvez aussi ajouter un processeur pour un handler spécifique en utilisant
l'attribut ``handler`` :

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor, handler: firephp }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" handler="firephp" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor', array('handler' => 'firephp');
        $container->register('my_service', $definition);

Vous pouvez aussi ajouter un processeur pour un canal de logging spécifique en
utilisant l'attribut ``channel``. Cela enregistrera seulement le processeur pour
le canal de logging ``security`` qui est utilisé dans le composant Security :

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor, channel: security }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" channel="security" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor', array('channel' => 'security');
        $container->register('my_service', $definition);

.. note::
 
    Vous ne pouvea pas utiliser les attributs ``handler`` et ``channel`` en même
    temps pour le même tag car les handlers sont partagés par un même canal.

..  _`documentation Twig`: http://twig.sensiolabs.org/doc/extensions.html
