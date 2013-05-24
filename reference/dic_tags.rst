Les Tags de l'Injection de Dépendances
======================================

Les Tags de l'Injection de Dépendances sont de courtes chaînes de caractères
qui peuvent être appliquées à un service pour le « marquer » afin qu'il soit utilisé
d'une manière spécifique. Par exemple, si vous avez un service que vous voudriez
enregistrer comme écouteur de l'un des évènements du noyau de Symfony, vous pouvez le
marquer avec le tag ``kernel.event_listener``.

Vous pouvez en apprendre un peu plus sur les « tags » en lisant la section ":ref:`book-service-container-tags`"
du chapitre sur le Conteneur de Service.

Ci-dessous, vous avez des informations sur l'ensemble des tags disponibles dans Symfony2.
Il pourrait aussi y avoir des « tags » dans d'autres bundles que vous utilisez
et qui ne sont pas listés ici.

+-----------------------------------+---------------------------------------------------------------------------+
| Nom du tag                        | Utilisation                                                               |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.asset`_                  | Register an asset to the current asset manager                            |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.factory_worker`_         | Add a factory worker                                                      |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.filter`_                 | Register a filter                                                         |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.formula_loader`_         | Add a formula loader to the current asset manager                         |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.formula_resource`_       | Adds a resource to the current asset manager                              |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.templating.php`_         | Remove this service if php templating is disabled                         |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.templating.twig`_        | Remove this service if twig templating is disabled                        |
+-----------------------------------+---------------------------------------------------------------------------+
| `data_collector`_                 | Crée une classe qui collecte des données personnalisées pour le profileur |
+-----------------------------------+---------------------------------------------------------------------------+
| `doctrine.event_listener`_        | Add a Doctrine event listener                                             |
+-----------------------------------+---------------------------------------------------------------------------+
| `doctrine.event_subscriber`_      | Add a Doctrine event subscriber                                           |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type`_                      | Crée un type de champ de formulaire personnalisé                          |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type_extension`_            | Crée une « extension de formulaire » personnalisée                        |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type_guesser`_              | Ajoute votre propre logique pour la « prédiction de type de formulaire »  |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.cache_clearer`_           | Register your service to be called during the cache clearing process      |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.cache_warmer`_            | Enregistre votre service pour qu'il soit appelé durant la mise en cache   |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.event_listener`_          | Écoute différents évènements/hooks de Symfony                             |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.event_subscriber`_        | Pour s'abonner à un ensemble de différents évènements/hooks de Symfony    |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.fragment_renderer`_       | Add new HTTP content rendering strategies                                 |
+-----------------------------------+---------------------------------------------------------------------------+
| `monolog.logger`_                 | Pour écrire des logs dans un canal de log personnalisé                    |
+-----------------------------------+---------------------------------------------------------------------------+
| `monolog.processor`_              | Ajoute un processeur personnalisé pour les logs                           |
+-----------------------------------+---------------------------------------------------------------------------+
| `routing.loader`_                 | Enregistre un service personnalisé qui charge les routes                  |
+-----------------------------------+---------------------------------------------------------------------------+
| `security.voter`_                 | Ajoute un voteur personnalisé à la logique d'autorisation de Symfony      |
+-----------------------------------+---------------------------------------------------------------------------+
| `security.remember_me_aware`_     | Pour permettre l'authentification avec « se souvenir de moi »             |
+-----------------------------------+---------------------------------------------------------------------------+
| `serializer.encoder`_             | Register a new encoder in the ``serializer`` service                      |
+-----------------------------------+---------------------------------------------------------------------------+
| `serializer.normalizer`_          | Register a new normalizer in the ``serializer`` service                   |
+-----------------------------------+---------------------------------------------------------------------------+
| `swiftmailer.plugin`_             | Enregistre un plugin SwiftMailer personnalisé                             |
+-----------------------------------+---------------------------------------------------------------------------+
| `templating.helper`_              | Rend votre service accessible dans les templates PHP                      |
+-----------------------------------+---------------------------------------------------------------------------+
| `translation.loader`_             | Enregistre un service personnalisé qui charge les traductions             |
+-----------------------------------+---------------------------------------------------------------------------+
| `twig.extension`_                 | Enregistre une extension Twig personnalisée                               |
+-----------------------------------+---------------------------------------------------------------------------+
| `twig.loader`_                    | Register a custom service that loads Twig templates                       |
+-----------------------------------+---------------------------------------------------------------------------+
| `validator.constraint_validator`_ | Crée votre propre contrainte de validation personnalisée                  |
+-----------------------------------+---------------------------------------------------------------------------+
| `validator.initializer`_          | Enregistre un service qui initialise les objets avant validation          |
+-----------------------------------+---------------------------------------------------------------------------+

assetic.asset
-------------

**Purpose**: Register an asset with the current asset manager

assetic.factory_worker
----------------------

**Purpose**: Add a factory worker

A Factory worker is a class implementing ``Assetic\Factory\Worker\WorkerInterface``.
Its ``process($asset)`` method is called for each asset after asset creation.
You can modify an asset or even return a new one.

In order to add a new worker, first create a class::

    use Assetic\Asset\AssetInterface;
    use Assetic\Factory\Worker\WorkerInterface;

    class MyWorker implements WorkerInterface
    {
        public function process(AssetInterface $asset)
        {
            // ... change $asset or return a new one
        }

    }

And then add register it as a tagged service:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme.my_worker:
                class: MyWorker
                tags:
                    - { name: assetic.factory_worker }

    .. code-block:: xml

        <service id="acme.my_worker" class="MyWorker>
            <tag name="assetic.factory_worker" />
        </service>

    .. code-block:: php

        $container
            ->register('acme.my_worker', 'MyWorker')
            ->addTag('assetic.factory_worker')
        ;

assetic.filter
--------------

**Purpose**: Register a filter

AsseticBundle uses this tag to register common filters. You can also use
this tag to register your own filters.

First, you need to create a filter::

    use Assetic\Asset\AssetInterface;
    use Assetic\Filter\FilterInterface;

    class MyFilter implements FilterInterface
    {
        public function filterLoad(AssetInterface $asset)
        {
            $asset->setContent('alert("yo");' . $asset->getContent());
        }

        public function filterDump(AssetInterface $asset)
        {
            // ...
        }
    }

Second, define a service:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme.my_filter:
                class: MyFilter
                tags:
                    - { name: assetic.filter, alias: my_filter }

    .. code-block:: xml

        <service id="acme.my_filter" class="MyFilter">
            <tag name="assetic.filter" alias="my_filter" />
        </service>

    .. code-block:: php

        $container
            ->register('acme.my_filter', 'MyFilter')
            ->addTag('assetic.filter', array('alias' => 'my_filter'))
        ;

Finally, apply the filter:

.. code-block:: jinja

    {% javascripts
        '@AcmeBaseBundle/Resources/public/js/global.js'
        filter='my_filter'
    %}
        <script src="{{ asset_url }}"></script>
    {% endjavascripts %}

You can also apply your filter via the ``assetic.filters.my_filter.apply_to``
config option as it's described here: :doc:`/cookbook/assetic/apply_to_option`.
In order to do that, you must define your filter service in a separate xml
config file and point to this file's via the ``assetic.filters.my_filter.resource``
configuration key.

assetic.formula_loader
----------------------

**Purpose**: Add a formula loader to the current asset manager

A Formula loader is a class implementing
``Assetic\\Factory\Loader\\FormulaLoaderInterface`` interface. This class
is responsible for loading assets from a particular kind of resources (for
instance, twig template). Assetic ships loaders for php and twig templates.

An ``alias`` attribute defines the name of the loader.

assetic.formula_resource
------------------------

**Purpose**: Adds a resource to the current asset manager

A resource is something formulae can be loaded from. For instance, twig
templates are resources.

assetic.templating.php
----------------------

**Purpose**: Remove this service if php templating is disabled

The tagged service will be removed from the container if the
``framework.templating.engines`` config section does not contain php.

assetic.templating.twig
-----------------------

**Purpose**: Remove this service if twig templating is disabled

The tagged service will be removed from the container if
``framework.templating.engines`` config section does not contain twig.

data_collector
--------------

**But** : Crée une classe qui collecte des données personnalisées pour le profileur

Pour plus de détails sur la création de vos propres collections de données, lisez
l'article du Cookbook : :doc:`/cookbook/profiler/data_collector`.

doctrine.event_listener
--------------

**Purpose**: Add a Doctrine event listener

For details on creating Doctrine event listeners, read the cookbook article:
:doc:`/cookbook/doctrine/event_listeners_subscribers`.

doctrine.event_subscriber
--------------

**Purpose**: Add a Doctrine event subscriber

For details on creating Doctrine event subscribers, read the cookbook article:
:doc:`/cookbook/doctrine/event_listeners_subscribers`.

.. _dic-tags-form-type:

form.type
---------

**But** : Crée un type de champ de formulaire personnalisé

Pour plus de détails sur la création de vos propres types de formulaire, lisez
l'article du Cookbook : :doc:`/cookbook/form/create_custom_field_type`.

form.type_extension
-------------------

**But** : Crée une « extension de formulaire » personnalisée

Les extensions de type de formulaire sont une manière de prendre en
main la création des champs de formulaire. Par exemple, l'ajout du jeton
CSRF est fait grâce à une extension de type de formulaire
(:class:`Symfony\\Component\\Form\\Extension\\Csrf\\Type\\FormTypeCsrfExtension`).

Une extension de type de formulaire peut modifier n'importe quelle partie d'un
champ de votre formulaire. Pour créer une extension, créez d'abord une classe
qui implémente l'interface :class:`Symfony\\Component\\Form\\FormTypeExtensionInterface`.

Pour plus de simplicité, vous étendrez le plus souvent la classe
:class:`Symfony\\Component\\Form\\AbstractTypeExtension` plutôt que l'interface
directement::

    // src/Acme/MainBundle/Form/Type/MyFormTypeExtension.php
    namespace Acme\MainBundle\Form\Type;

    use Symfony\Component\Form\AbstractTypeExtension;

    class MyFormTypeExtension extends AbstractTypeExtension
    {
        // ... fill in whatever methods you want to override
        // like buildForm(), buildView(), finishView(), setDefaultOptions()
    }

Pour que Symfony connaisse l'existence de vos extension de formulaire et sache comment les utiliser,
attribuez leur le tag `form.type_extension` :

.. configuration-block::

    .. code-block:: yaml

        services:
            main.form.type.my_form_type_extension:
                class: Acme\MainBundle\Form\Type\MyFormTypeExtension
                tags:
                    - { name: form.type_extension, alias: field }

    .. code-block:: xml

        <service id="main.form.type.my_form_type_extension" class="Acme\MainBundle\Form\Type\MyFormTypeExtension">
            <tag name="form.type_extension" alias="field" />
        </service>

    .. code-block:: php

        $container
            ->register('main.form.type.my_form_type_extension', 'Acme\MainBundle\Form\Type\MyFormTypeExtension')
            ->addTag('form.type_extension', array('alias' => 'field'))
        ;

The ``alias`` key of the tag is the type of field that this extension should
be applied to. For example, to apply the extension to any form/field, use the
"form" value.

form.type_guesser
-----------------

**But** : Ajoute votre propre logique pour la « prédiction de type de formulaire »

Ce tag vous permet d'ajouter votre propre logique au processus de
:ref:`Prédiction de formulaire<book-forms-field-guessing>`. Par défaut,
la prédiction de formulaire est réalisée par des « prédicateurs » basés sur les metadonnées
de validation et de Doctrine (si vous utilisez Doctrine).

Pour ajouter votre propre prédicateur de type de formulaire, créez une classe qui implémente
l'interface :class:`Symfony\\Component\\Form\\FormTypeGuesserInterface`. Ensuite, taggez la
définition du service avec ``form.type_guesser`` (il n'y a pas d'option).

Pour voir un exemple de ce à quoi la classe ressemblerait, regardez la classe
``ValidatorTypeGuesser`` du composant ``Form``.

kernel.cache_clearer
--------------------

**Purpose**: Register your service to be called during the cache clearing process

Cache clearing occurs whenever you call ``cache:clear`` command. If your
bundle caches files, you should add custom cache clearer for clearing those
files during the cache clearing process.

In order to register your custom cache clearer, first you must create a
service class::

    // src/Acme/MainBundle/Cache/MyClearer.php
    namespace Acme\MainBundle\Cache;

    use Symfony\Component\HttpKernel\CacheClearer\CacheClearerInterface;

    class MyClearer implements CacheClearerInterface
    {
        public function clear($cacheDir)
        {
            // clear your cache
        }

    }

Then register this class and tag it with ``kernel.cache:clearer``:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_cache_clearer:
                class: Acme\MainBundle\Cache\MyClearer
                tags:
                    - { name: kernel.cache_clearer }

    .. code-block:: xml

        <service id="my_cache_clearer" class="Acme\MainBundle\Cache\MyClearer">
            <tag name="kernel.cache_clearer" />
        </service>

    .. code-block:: php

        $container
            ->register('my_cache_clearer', 'Acme\MainBundle\Cache\MyClearer')
            ->addTag('kernel.cache_clearer')
        ;

kernel.cache_warmer
-------------------

**But** : Enregistre votre service pour qu'il soit appelé durant la mise en cache

La mise en cache s'effectue lorsque vous exécutez la tâche ``cache:warmup`` ou
``cache:clear`` (à moins que vous passiez l'option ``--no-warmup`` à ``cache:clear``). Le
but est d'initialiser un cache quelconque dont l'application aura besoin et d'éviter
que le premier utilisateur ne subisse un ralentissement dû à la mise en cache
lorsque ce dernier est généré dynamiquement.

Pour enregistrer votre propre système de mise en cache, créez tout d'abord un
service qui implémente l'interface
:class:`Symfony\\Component\\HttpKernel\\CacheWarmer\\CacheWarmerInterface`::

    // src/Acme/MainBundle/Cache/MyCustomWarmer.php
    namespace Acme\MainBundle\Cache;

    use Symfony\Component\HttpKernel\CacheWarmer\CacheWarmerInterface;

    class MyCustomWarmer implements CacheWarmerInterface
    {
        public function warmUp($cacheDir)
        {
            // do some sort of operations to "warm" your cache
        }

        public function isOptional()
        {
            return true;
        }
    }

La méthode ``isOptional`` devrait retourner « true » s'il est possible d'utiliser
l'application sans avoir à appeler ce procédé de mise en cache. Dans Symfony 2.0,
ces procédés de mise en cache sont toujours exécutés de toute façon, donc cette
fonction n'a pas vraiment d'effet.

Pour enregistrer votre procédé de mise en cache dans Symfony, donnez-lui le
tag kernel.cache_warmer :

.. configuration-block::

    .. code-block:: yaml

        services:
            main.warmer.my_custom_warmer:
                class: Acme\MainBundle\Cache\MyCustomWarmer
                tags:
                    - { name: kernel.cache_warmer, priority: 0 }

    .. code-block:: xml

        <service id="main.warmer.my_custom_warmer" class="Acme\MainBundle\Cache\MyCustomWarmer">
            <tag name="kernel.cache_warmer" priority="0" />
        </service>

    .. code-block:: php

        $container
            ->register('main.warmer.my_custom_warmer', 'Acme\MainBundle\Cache\MyCustomWarmer')
            ->addTag('kernel.cache_warmer', array('priority' => 0))
        ;

La valeur ``priority`` est optionnelle, et vaut par défaut 0. Cette valeur
peut aller de -255 à 255, et les procédés de mise en cache seront exécutés
selon l'ordre de leur priorité.

.. _dic-tags-kernel-event-listener:

kernel.event_listener
---------------------

**But** : Écoute différents évènements/hooks de Symfony

Ce tag vous permet d'injecter vos propres classes dans le processus de Symfony à
différents points.

Pour un exemple complet de cet écouteur (« listener » en anglais), lisez l'article
du cookbook :doc:`/cookbook/service_container/event_listener`.

Pour un autre exemple pratique d'un écouteur du « kernel » (« noyau » en français),
référez-vous à l'article du cookbook suivant : :doc:`/cookbook/request/mime_type`.

Écouteurs d'évènements du noyau de référence
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Lorsque vous ajoutez vos propres écouteurs, cela peut être utile de connaître
les autres écouteurs du noyau de Symfony et leurs priorités.

.. note::

    Tout les écouteurs listés ici peuvent ne pas écouter selon votre environnement,
    votre configuration et vos bundles. De plus, les bundles tiers fournissent des
    écouteurs supplémentaires qui ne sont pas listés ici.

kernel.request
..............

+-------------------------------------------------------------------------------------------+-----------+
| Nom de classe de l'écouteur                                                               | Priorité  |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`                  | 1024      |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\TestSessionListener`             | 192       |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\SessionListener`                 | 128       |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\RouterListener`                    | 32        |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\LocaleListener`                    | 16        |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\Security\\Http\\Firewall`                                     | 8         |
+-------------------------------------------------------------------------------------------+-----------+

kernel.controller
.................

+-------------------------------------------------------------------------------------------+----------+
| Nom de classe de l'écouteur                                                               | Priorité |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\DataCollector\\RequestDataCollector`            | 0        |
+-------------------------------------------------------------------------------------------+----------+

kernel.response
...............

+-------------------------------------------------------------------------------------------+----------+
| Nom de classe de l'écouteur                                                               | Priorité |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\EsiListener`                       | 0        |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ResponseListener`                  | 0        |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\SecurityBundle\\EventListener\\ResponseListener`                 | 0        |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`                  | -100     |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\TestSessionListener`             | -128     |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener`       | -128     |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\StreamedResponseListener`          | -1024    |
+-------------------------------------------------------------------------------------------+----------+

kernel.exception
................

+-------------------------------------------------------------------------------------------+----------+
| Nom de classe de l'écouteur                                                               | Priorité |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`                  | 0        |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener`                 | -128     |
+-------------------------------------------------------------------------------------------+----------+

kernel.terminate
................

+-------------------------------------------------------------------------------------------+----------+
| Nom de classe de l'écouteur                                                               | Priorité |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\SwiftmailerBundle\\EventListener\\EmailSenderListener`           | 0        |
+-------------------------------------------------------------------------------------------+----------+


.. _dic-tags-kernel-event-subscriber:

kernel.event_subscriber
-----------------------

**But** : Pour s'abonner à un ensemble de différents évènements/hooks de Symfony

Pour activer un souscripteur personnalisé, ajoutez-le dans l'une de vos configurations
comme vous le feriez pour un service « normal », et taggez-le avec
``kernel.event_subscriber`` :

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

    Votre service doit implémenter l'interface
    :class:`Symfony\Component\EventDispatcher\EventSubscriberInterface`.

.. note::

    Si votre service est créé par une « factory » (« usine » en français), vous
    **devez** définir correctement le paramètre ``class`` afin que ce tag fonctionne
    sans problèmes.

kernel.fragment_renderer
------------------------

**Purpose**: Add a new HTTP content rendering strategy.

To add a new rendering strategy - in addition to the core strategies like
``EsiFragmentRenderer`` - create a class that implements
:class:`Symfony\\Component\\HttpKernel\\Fragment\\FragmentRendererInterface`,
register it as a service, then tag it with ``kernel.fragment_renderer``.

.. _dic_tags-monolog:

monolog.logger
--------------

**But** : Pour écrire des logs dans un canal de log personnalisé

Monolog vous permet de partager ses gestionnaires entre différents canaux
de logs. Le service de log utilise le canal ``app`` mais vous pouvez
changer ce dernier lorsque vous injectez le « logger » dans un service.

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Fully\Qualified\Loader\Class\Name
                arguments: ["@logger"]
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
        $container->register('my_service', $definition);

.. note::

    Cela fonctionne uniquement quand le service de log est un argument du
    constructeur, et pas lorsqu'il est injecté via un « setter ».

.. _dic_tags-monolog-processor:

monolog.processor
-----------------

**But** : Ajoute un processeur personnalisé pour les logs

Monolog vous permet d'ajouter des processeurs au service de log ou aux
gestionnaires afin d'ajouter des données supplémentaires aux enregistrements.
Un processeur reçoit l'enregistrement en tant qu'argument et doit le retourner
après avoir ajouté quelques données supplémentaires à l'attribut ``extra`` de
l'enregistrement.

Voyons voir comment vous pouvez utiliser le processeur intégré
``IntrospectionProcessor`` afin d'ajouter le fichier, la ligne, la classe
et la méthode depuis laquelle le service de log a été appelé.

Vous pouvez ajouter un processeur de manière globale.

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

    Si votre service n'est pas un « callable » (appelable via ``__invoke``)
    vous pouvez ajouter l'attribut ``method`` dans le tag afin de spécifier
    la méthode à utiliser.

Vous pouvez aussi ajouter un processeur pour un gestionnaire spécifique en
utilisant l'attribut ``handler`` :

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

De même, vous pouvez ajouter un processeur pour un canal spécifique de log en utilisant
l'attribut ``channel``. L'exemple qui suit va enregistrer le processeur uniquement pour
le canal de log ``security`` utilisé par le composant « Security » :

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

    Vous ne pouvez pas utiliser les deux attributs ``handler`` et ``channel``
    pour un même tag car les gestionnaires (« handlers » en anglais) sont
    partagés entre tous les canaux.

routing.loader
--------------

**But** : Enregistre un service personnalisé qui charge les routes

Pour activer un chargeur de routes personnalisé, ajoutez-le dans l'une de vos
configurations comme vous le feriez pour un service « normal », et taggez-le
avec ``routing.loader`` :

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

security.remember_me_aware
--------------------------

**But** : Pour permettre l'authentification avec « se souvenir de moi »

Ce tag est utilisé en interne pour permettre l'authentification « se souvenir
de moi » de fonctionner. Si vous avez une méthode d'authentification personnalisée
où un utilisateur peut être authentifié avec l'option « se souvenir de moi »,
alors vous pourriez avoir à utiliser ce tag.

Si votre « factory » d'authentification personnalisée étend
:class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\AbstractFactory`
et que votre écouteur d'authentification personnalisé étend
:class:`Symfony\\Component\\Security\\Http\\Firewall\\AbstractAuthenticationListener`,
alors ce dernier va automatiquement se voir appliquer ce tag et il fonctionnera automatiquement.

security.voter
--------------

**But** : Ajoute un voteur personnalisé à la logique d'autorisation de Symfony

Lorsque vous appelez ``isGranted`` dans le contexte de sécurité de Symfony, un
système de « voteurs » est utilisé en arrière-plan pour déterminer si l'utilisateur
devrait ou non avoir accès. Le tag ``security.voter`` vous permet d'ajouter votre
propre voteur personnalisé à ce système.

Pour plus d'informations, lisez l'article du cookbook :
:doc:`/cookbook/security/voters`.

.. _reference-dic-tags-serializer-encoder:

serializer.encoder
------------------

**Purpose**: Register a new encoder in the ``serializer`` service

The class that's tagged should implement the :class:`Symfony\\Component\\Serializer\\Encoder\\EncoderInterface`
and :class:`Symfony\\Component\\Serializer\\Encoder\\DecoderInterface`.

For more details, see :doc:`/cookbook/serializer`.

.. _reference-dic-tags-serializer-normalizer:

serializer.normalizer
---------------------

**Purpose**: Register a new normalizer in the Serializer service

The class that's tagged should implement the :class:`Symfony\\Component\\Serializer\\Normalizer\\NormalizerInterface`
and :class:`Symfony\\Component\\Serializer\\Normalizer\\DenormalizerInterface`.

For more details, see :doc:`/cookbook/serializer`.

swiftmailer.plugin
------------------

**But** : Enregistre un plugin SwiftMailer personnalisé

Si vous utilisez un plugin SwiftMailer personnalisé (ou souhaitez en créer un),
vous pouvez le déclarer via SwiftMailer en créant un service pour votre plugin
et en le « taggant » avec ``swiftmailer.plugin`` (il ne possède pas d'options).

Un plugin SwiftMailer doit implémenter l'interface ``Swift_Events_EventListener``.
Pour plus d'informations sur les plugins, voir la
`Documentation du Système de Plugin de SwiftMailer`_.

Plusieurs plugins SwiftMailer font partie du coeur de Symfony et peuvent être activés
grâce à différentes configurations. Pour plus de détails, lisez
:doc:`/reference/configuration/swiftmailer`.

templating.helper
-----------------

**But** : Rend votre service accessible dans les templates PHP

Pour activer un template d'aide personnalisé, ajoutez-le dans l'une de vos
configurations comme vous le feriez pour un service « normal », taggez-le
avec ``templating.helper`` et définissez un attribut ``alias`` (le template
d'aide sera ainsi accessible via cet alias dans les templates) :

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

translation.loader
------------------

**But** : Enregistre un service personnalisé qui charge les traductions

Par défaut, les traductions sont chargées depuis le système de fichiers dans différents
formats (YAML, XLIFF, PHP, etc.). Si vous avez besoin de charger des traductions
depuis une autre source, créez d'abord une classe qui implémente l'interface
:class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface`::

    // src/Acme/MainBundle/Translation/MyCustomLoader.php
    namespace Acme\MainBundle\Translation;

    use Symfony\Component\Translation\Loader\LoaderInterface;
    use Symfony\Component\Translation\MessageCatalogue;

    class MyCustomLoader implements LoaderInterface
    {
        public function load($resource, $locale, $domain = 'messages')
        {
            $catalogue = new MessageCatalogue($locale);

            // some how load up some translations from the "resource"
            // then set them into the catalogue
            $catalogue->set('hello.world', 'Hello World!', $domain);

            return $catalogue;
        }
    }

Votre méthode de chargement personnalisée ``load`` est chargée de retourner
un :Class:`Symfony\\Component\\Translation\\MessageCatalogue`.

Maintenant, vous pouvez enregistrer votre chargeur comme un service et le
tagger avec ``translation.loader`` :

.. configuration-block::

    .. code-block:: yaml

        services:
            main.translation.my_custom_loader:
                class: Acme\MainBundle\Translation\MyCustomLoader
                tags:
                    - { name: translation.loader, alias: bin }

    .. code-block:: xml

        <service id="main.translation.my_custom_loader" class="Acme\MainBundle\Translation\MyCustomLoader">
            <tag name="translation.loader" alias="bin" />
        </service>

    .. code-block:: php

        $container
            ->register('main.translation.my_custom_loader', 'Acme\MainBundle\Translation\MyCustomLoader')
            ->addTag('translation.loader', array('alias' => 'bin'))
        ;

L'option ``alias`` est requise et très importante : elle définit le « suffixe »
du fichier qui sera utilisé pour les fichiers de ressource qui utilisent ce
chargeur. Par exemple, supposons que vous ayez un format personnalisé ``bin``
que vous devez charger. Si vous avez un fichier ``bin`` qui contient des traductions
françaises pour le domaine ``messages``, alors vous auriez un fichier du type
``app/Resources/translations/messages.fr.bin``.

Lorsque Symfony essaye de charger le fichier ``bin``, il passe le chemin de votre
chargeur personnalisé en tant qu'argument ``$ressource``. Vous pouvez ainsi
effectuer n'importe quelle opération nécessaire sur ce fichier afin de pouvoir
charger vos traductions.

Si vous chargez des traductions depuis une base de données, vous aurez toujours
besoin d'un fichier de ressource, mais il pourrait soit être vide ou soit
contenir des informations sur le chargement de ces ressources depuis la
base de données. Le fichier est essentiel pour déclencher la méthode ``load`` de votre
chargeur personnalisé.

.. _reference-dic-tags-twig-extension:

twig.extension
--------------

**But** : Enregistre une extension Twig personnalisée

Pour activer une extension Twig, ajoutez-la dans l'une de vos
configurations comme vous le feriez pour un service « normal », et
taggez-la avec ``twig.extension``:

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

Pour plus d'informations sur comment créer la classe d'Extension Twig,
lisez la `documentation Twig`_ sur le sujet ou lisez l'article du cookbook :
:doc:`/cookbook/templating/twig_extension`.

Avant d'écrire vos propres extensions, jetez un oeil au
`dépôt officiel des extensions Twig`_ qui inclut déjà plusieurs extensions
utiles. Par exemple, ``Intl`` et son filtre ``localizeddate`` qui formatte
une date selon la locale de l'utilisateur. Ces extensions Twig officielles
doivent aussi être ajoutées comme les autres services « normaux » :

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

twig.loader
-----------

**Purpose**: Register a custom service that loads Twig templates

By default, Symfony uses only one `Twig Loader`_ -
:class:`Symfony\\Bundle\\TwigBundle\\Loader\\FilesystemLoader`. If you need
to load Twig templates from another resource, you can create a service for
the new loader and tag it with ``twig.loader``:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme.demo_bundle.loader.some_twig_loader:
                class: Acme\DemoBundle\Loader\SomeTwigLoader
                tags:
                    - { name: twig.loader }

    .. code-block:: xml

        <service id="acme.demo_bundle.loader.some_twig_loader" class="Acme\DemoBundle\Loader\SomeTwigLoader">
            <tag name="twig.loader" />
        </service>

    .. code-block:: php

        $container
            ->register('acme.demo_bundle.loader.some_twig_loader', 'Acme\DemoBundle\Loader\SomeTwigLoader')
            ->addTag('twig.loader')
        ;

validator.constraint_validator
------------------------------

**But** : Crée votre propre contrainte de validation personnalisée

Ce tag vous permet de créer et d'enregistrer votre propre contrainte de validation
personnalisée. Pour plus d'informations, lisez l'article du cookbook :
:doc:`/cookbook/validation/custom_constraint`.

validator.initializer
---------------------

**But** : Enregistre un service qui initialise les objets avant validation

Ce tag fournit un bout de fonctionnalité très peu commun qui vous permet
d'effectuer une action sur un objet juste avant qu'il ne soit validé. Par exemple,
cela est utilisé par Doctrine pour effectuer une requête de toutes les
données « chargées de manière fainéante » (« lazy loading » en anglais)
sur un objet avant qu'il ne soit validé. Sans cela, certaines données
d'une entité Doctrine apparaîtraient comme « manquantes » lorsque validées,
bien que cela ne soit pas réellement le cas.

Si vous devez utiliser ce tag, créez simplement une nouvelle classe qui
implémente l'interface
:class:`Symfony\\Component\\Validator\\ObjectInitializerInterface`.
Puis, taggez-le avec ``validator.initializer`` (ce tag ne possède pas
d'options).

Pour un exemple, jetez un oeil à la classe ``EntityInitializer`` dans le
« Doctrine Bridge ».

.. _`documentation Twig`: http://twig.sensiolabs.org/doc/advanced.html#creating-an-extension
.. _`dépôt officiel des extensions Twig`: https://github.com/fabpot/Twig-extensions
.. _`KernelEvents`: https://github.com/symfony/symfony/blob/2.0/src/Symfony/Component/HttpKernel/KernelEvents.php
.. _`Documentation du Système de Plugin de SwiftMailer`: http://swiftmailer.org/docs/plugins.html
.. _`Twig Loader`: http://twig.sensiolabs.org/doc/api.html#loaders
