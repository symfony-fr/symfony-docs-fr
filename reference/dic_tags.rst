Les Tags de l'Injection de Dépendances
======================================

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

Pour activer un helper de template personnalisé, ajoutez le en tant que
service dans votre configuration, taggez le avec ``templating.helper`` et
définissez l'attribut ``alias`` (le helper sera accessible par cet alias dans
vos templates) :

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

Activer une extension Twig personnalisée
----------------------------------------

Pour activer une extension Twig, ajoutez la comme service dans votre
configuration et taggez la avec ``twig.extension`` :

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

Pour plus d'informations sur comment créer une classe d'Extension Twig, lisez
la `documentation Twig`_ sur le sujet.

Avant d'écrire votre propre extension, jetez un oeil au
`dépôt officiel des extensions Twig`_ qui contient déjà plusieurs extensions
très utiles. Par exemple ``Intl`` et ses filtres ``localizeddate`` qui formattent
une date selon la locale de l'utilisateur. Ces extensions Twig officielles ont
également besoin d'être ajoutées comme services :

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.intl:
                class: Twig_Extensions_Extension_Intl
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <service id="twig.extension.intl" class="Twig_Extensions_Extension_Intl">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $container
            ->register('twig.extension.intl', 'Twig_Extensions_Extension_Intl')
            ->addTag('twig.extension')
        ;

.. _dic-tags-kernel-event-listener:

Activer des listeners personnalisés
-----------------------------------

Pour activer un listener personnalisé, ajoutez le comme service dans 
votre configuration et taggez le avec ``kernel.event_listener``. Vous devez
définir le nom de l'évènement que votre service écoute, ainsi que la méthode
qui sera appelée :

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

    Vous pouvez aussi spécifier l'attribut priority du tag kernel.event_listener 
    (tout comme les attributs method ou event) avec un entier positif ou négatif.
	Cela vous permet d'être sûr que votre listener sera toujours appelé avant ou
	après un autre listener qui écoute le même évènement.

.. _dic-tags-kernel-event-subscriber:

Activer les abonnements personnalisés
-------------------------------------

.. versionadded:: 2.1
   
   La possibilité d'ajouter des abonnements à un évènement du noyau est une nouveauté
   de la version 2.1.

Pour activer un abonnement personnalisé, ajoutez le comme service dans votre configuration
et taggez le avec ``kernel.event_subscriber`` :

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

    Si votre service est créé par une factory, vous *DEVEZ* définir correctement
    le paramètre ``class`` pour que ce tag fonctionne bien.

Activer les moteurs de template personnalisés
---------------------------------------------

Pour activer un moteur de template personnalisé, ajoutez le comme service
dans votre configuration, et taggez le avec ``templating.engine`` :

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

Activer un chargeur de route personnalisé
-----------------------------------------

Pour ajouter un chargeur de route personnalisé, ajoutez le comme service dans
votre configuration et taggez le avec ``routing.loader``:

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

Utiliser un canal d'authentification personnalisé avec Monolog
--------------------------------------------------------------

Monolog vous permet de partager ses gestionnaires entre plusieurs canaux
d'authentification. Le service logger utilise le canal ``app``
mais vous pouvez changer de canal en l'injectant dans le service.

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

    Ce ne fonctionne que quand le service logger est un argument du constructeur,
    pas quand il est injecté avec un setter.

.. _dic_tags-monolog-processor:

Ajouter un processeur pour Monolog
----------------------------------

Monolog vous permet d'ajouter des processeurs dans le logger ou dans les 
gestionnaires pour ajouter des données supplémentaires à l'enregistrement.
Un processeur recoit l'enregistrement comme argument et doit le retourner
après avoir ajouté des données supplémentaires dans l'attribut ``extra`` de
l'enregistrement.

Regardons comment vous pouvez utiliser le processeur préconstruit ``IntrospectionProcessor``
pour ajouter le fichier, la ligne, la classe et la méthode où le logger a été déclenché.

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
    Si votre service n'est pas appelable directement (en utilisant ``__invoke``), vous
    pouvez ajouter l'attribut ``method`` dans le tag pour utiliser une méthode spécifique.

vous pouvez ajouter aussi un processeur pour un gestionnaire spécifique en utilisant
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

Vous pouvez également ajouter un processuer pour un canal d'authentification spécifique
en utilisant l'attribut ``channel``. Cela enregistrera le processeur uniquement pour le canal d'authentification ``security`` qui est utilisé dans le composant Security :

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

    Vous ne pouvez pas utiliser les attributs ``handler`` et ``channel`` simultannément
    dans un même tag car les gestionnaires sont partagés avec tous les canaux.

..  _`documentation Twig`: http://twig.sensiolabs.org/doc/extensions.html
..  _`dépôt officiel des extensions Twig`: http://github.com/fabpot/Twig-extensions
