.. index::
   single: Configuration; Sémantique
   single: Bundle; Configuration d'extension

Comment exposer une configuration sémantique pour un Bundle
===========================================================

Si vous ouvrez le fichier de configuration de votre application (en général
``app/config/config.yml``), vous allez y trouver un certain nombre de différents
« espaces de noms » de configuration, tel que ``framework``, ``twig``, et ``doctrine``.
Chacun d'entre eux configure un bundle spécifique, vous permettant de paramétrer des
choses à un haut niveau et ainsi de laisser le bundle effectuer tous les changements
qui restent ; ces derniers étant complexes et de bas niveau.

Par exemple, ce qui suit dit au ``FrameworkBundle`` d'activer l'intégration
de formulaire, qui implique de définir pas mal de services ainsi que
l'intégration d'autres composants liés :

.. configuration-block::

    .. code-block:: yaml

        framework:
            # ...
            form:            true

    .. code-block:: xml

        <framework:config>
            <framework:form />
        </framework:config>

    .. code-block:: php

        $container->loadFromExtension('framework', array(
            // ...
            'form'            => true,
            // ...
        ));

Lorsque vous créez un bundle, vous avez deux choix concernant la gestion de la
configuration :

1. **Configuration normale des services** (*facile*) :

    Vous pouvez spécifier vos services dans un fichier de configuration (par
    exemple : ``services.yml``) qui se trouve dans votre bundle et puis l'importer
    depuis la configuration principale de votre application. Cela est vraiment
    facile, rapide et totalement efficace. Si vous utilisez des
    :ref:`paramètres<book-service-container-parameters>`, alors vous avez toujours
    la flexibilité de personnaliser votre bundle depuis la configuration de
    votre application. Lisez « :ref:`service-container-imports-directive`" » pour
    plus de détails.

2. **Exposer la configuration sémantique** (*avancé*) :

    C'est la manière dont la configuration est faite pour les bundles coeurs
    (comme décrit ci-dessus). L'idée de base est que, à la place d'avoir
    l'utilisateur qui surcharge les paramètres individuels, vous laissez
    l'utilisateur configurer seulement certaines options spécifiquement créées.
    En tant que développeur du bundle, vous analysez donc cette configuration
    et chargez les services dans une classe « Extension ». Avec cette méthode,
    vous n'aurez pas besoin d'importer quelconque ressource de configuration
    depuis la configuration principale de votre application : la classe
    Extension peut gérer tout cela.

La seconde option - que vous allez approfondir dans cet article - est
beaucoup plus flexible, mais requiert aussi plus de temps à mettre en place.
Si vous vous demandez quelle méthode vous devriez utiliser, c'est probablement
une bonne idée de commencer avec la méthode #1, et de changer pour la #2 si
besoin est.

La seconde méthode possède plusieurs avantages spécifiques :

* Beaucoup plus puissante que de définir simplement des paramètres : une valeur
  d'option spécifique pourrait déclencher la création de plusieurs définitions
  de services ;

* Possibilité d'avoir une hiérarchie dans la configuration ;

* Fusion intelligente quand plusieurs fichiers de configuration (par
  exemple : ``config_dev.yml`` et ``config.yml``) se surchargent entre eux ;

* Validation de la configuration (si vous utilisez une
  :ref:`Classe de Configuration<cookbook-bundles-extension-config-class>`) ;

* Autocomplétion via l'IDE lorsque vous créez un XSD et que les développeurs
  utilisent XML.

.. sidebar:: Surcharger les paramètres d'un bundle

    Si un Bundle fournit une classe d'Extension, alors vous *ne* devriez
    généralement *pas* surcharger l'un de ses paramètres de conteneur
    de service. L'idée est que si une classe d'Extension est présente, chaque
    paramètre qui doit être configurable devrait être présent dans la configuration
    rendue accessible par cette classe. En d'autres termes, la classe d'extension
    définit tous les paramètres publics de configuration supportés pour lesquels
    une rétro-compatibilité sera maintenue.

.. index::
   single: Bundle; Extension
   single: Injection de Dépendances; Extension

Créer une classe d'Extension
----------------------------

Si vous choisissez d'exposer une configuration sémantique pour votre bundle,
vous aurez d'abord besoin de créer une nouvelle classe « Extension », qui va
gérer le processus. Cette classe devrait se trouver dans le répertoire
``DependencyInjection`` de votre bundle et son nom devrait être construit en
remplaçant le suffixe ``Bundle`` du nom de la classe du Bundle par ``Extension``.
Par exemple, la classe d'Extension de ``AcmeHelloBundle`` serait nommée
``AcmeHelloExtension``::

    // Acme/HelloBundle/DependencyInjection/AcmeHelloExtension.php
    namespace Acme\HelloBundle\DependencyInjection;

    use Symfony\Component\HttpKernel\DependencyInjection\Extension;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    class AcmeHelloExtension extends Extension
    {
        public function load(array $configs, ContainerBuilder $container)
        {
            // là où toute la logique principale est effectuée
        }

        public function getXsdValidationBasePath()
        {
            return __DIR__.'/../Resources/config/';
        }

        public function getNamespace()
        {
            return 'http://www.example.com/symfony/schema/';
        }
    }

.. note::

    Les méthodes ``getXsdValidationBasePath`` et ``getNamespace`` sont
    requises uniquement si le bundle fournit un XSD optionnel pour la
    configuration.

La présence de la classe précédente signifie que vous pouvez maintenant définir
un espace de noms de configuration ``acme_hello`` dans n'importe quel fichier de
configuration. L'espace de noms ``acme_hello`` est construit sur la base du nom
de la classe d'extension en enlevant le mot ``Extension`` et en passant le reste
du nom en minuscules avec des tirets bas (underscores). En d'autres termes, ``AcmeHelloExtension``
devient ``acme_hello``.

Vous pouvez immédiatement commencer à spécifier la configuration sous cet espace
de noms :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        acme_hello: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:acme_hello="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

           <acme_hello:config />
           
           <!-- ... -->
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('acme_hello', array());

.. tip::

    Si vous suivez les conventions de nommage décrites ci-dessus, alors la
    méthode ``load()`` du code de votre extension est toujours appelée tant
    que votre bundle est déclaré dans le Kernel. En d'autres termes, même si
    l'utilisateur ne fournit aucune configuration (c-a-d que l'entrée ``acme_hello``
    n'apparaît même pas), la méthode ``load()`` sera appelée avec
    un tableau ``$configs`` vide comme argument . Vous pouvez toujours fournir des
    valeurs par défaut si vous le voulez.

Analyser le tableau ``$configs``
--------------------------------

Chaque fois qu'un utilisateur inclut l'espace de noms ``acme_hello`` dans un
fichier de configuration, la configuration incluse dans cet espace est ajoutée
à un tableau de configurations et passée à la méthode ``load()`` de votre
extension (Symfony2 convertit automatiquement XML et YAML en un tableau).

Prenez la configuration suivante :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        acme_hello:
            foo: fooValue
            bar: barValue

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:acme_hello="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

            <acme_hello:config foo="fooValue">
                <acme_hello:bar>barValue</acme_hello:bar>
            </acme_hello:config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('acme_hello', array(
            'foo' => 'fooValue',
            'bar' => 'barValue',
        ));

Le tableau passé à votre méthode ``load()`` ressemblera à cela::

    array(
        array(
            'foo' => 'fooValue',
            'bar' => 'barValue',
        )
    )

Notez que ceci est un *tableau de tableaux*, et pas juste un unique tableau plat
contenant les valeurs de configuration. Cela est intentionnel. Par exemple, si
``acme_hello`` apparaît dans un autre fichier de configuration - disons
``config_dev.yml`` - avec des valeurs différentes, alors le tableau passé à
votre méthode ``load()`` pourrait ressembler à ça::

    array(
        array(
            'foo' => 'fooValue',
            'bar' => 'barValue',
        ),
        array(
            'foo' => 'fooDevValue',
            'baz' => 'newConfigEntry',
        ),
    )

L'ordre des deux tableaux dépend duquel est défini en premier.

C'est alors votre travail de décider comment ces configurations devraient
être fusionnées ensemble. Par exemple, vous pourriez avoir les dernières
valeurs écrasant les précédentes ou en quelque sorte les fusionner ensemble.

Enfin, dans la section
:ref:`Classe de Configuration<cookbook-bundles-extension-config-class>`, vous
apprendrez une manière réellement robuste de gérer cela. Mais pour le moment,
vous pourriez simplement les fusionner manuellement::

    public function load(array $configs, ContainerBuilder $container)
    {
        $config = array();
        foreach ($configs as $subConfig) {
            $config = array_merge($config, $subConfig);
        }

        // maintenant utilisez le tableau plat $config
    }

.. caution::

    Assurez-vous que la technique de fusion ci-dessus ait sens pour votre
    bundle. Ceci est juste un exemple et vous devriez être attentif à ne pas
    l'utiliser aveuglément.

Utiliser la méthode ``load()``
------------------------------

Dans la méthode ``load()``, la variable ``$container`` fait référence à un conteneur
qui connaît uniquement cet espace de noms de configuration (c-a-d qu'il ne contient
pas d'informations de service chargées depuis d'autres bundles). Le but de la
méthode ``load()`` est de manipuler le conteneur, en ajoutant et en configurant
n'importe quelles méthodes ou services nécessaires à votre bundle.

Charger des ressources de configuration externes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Une chose commune à faire est de charger un fichier de configuration externe
qui pourrait contenir l'ensemble des services nécessaires à votre bundle. Par
exemple, supposons que vous ayez un fichier ``services.xml`` contenant la plupart
des configurations de service de votre bundle::

    use Symfony\Component\DependencyInjection\Loader\XmlFileLoader;
    use Symfony\Component\Config\FileLocator;

    public function load(array $configs, ContainerBuilder $container)
    {
        // préparer votre variable $config

        $loader = new XmlFileLoader($container, new FileLocator(__DIR__.'/../Resources/config'));
        $loader->load('services.xml');
    }

Vous pourriez même faire ceci conditionnellement, en vous basant sur l'une des valeurs de
configuration. Par exemple, supposons que vous vouliez charger un ensemble de
services seulement si une option ``enabled`` est passée et définie comme « true »::

    public function load(array $configs, ContainerBuilder $container)
    {
        // préparer votre variable $config

        $loader = new XmlFileLoader($container, new FileLocator(__DIR__.'/../Resources/config'));

        if (isset($config['enabled']) && $config['enabled']) {
            $loader->load('services.xml');
        }
    }

Configurer les services et définir les paramètres
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Une fois que vous avez chargé des configurations de service, vous pourriez
avoir besoin de modifier la configuration basée sur certaines valeurs saisies.
Par exemple, supposons que vous ayez un service dont le premier
argument est une chaîne de caractères « type » que le service va utiliser
en interne. Vous voudriez que ceci soit facilement configurable par
l'utilisateur du bundle, donc dans votre fichier de configuration du services
(par exemple : ``services.xml``), vous définissez ce service et utilisez un
paramètre vide - ``acme_hello.my_service_type`` - en tant que premier argument :

.. code-block:: xml

    <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
    <container xmlns="http://symfony.com/schema/dic/services"
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

        <parameters>
            <parameter key="acme_hello.my_service_type" />
        </parameters>

        <services>
            <service id="acme_hello.my_service" class="Acme\HelloBundle\MyService">
                <argument>%acme_hello.my_service_type%</argument>
            </service>
        </services>
    </container>

Mais pourquoi définir un paramètre vide et puis le passer à votre service ?
La réponse est que vous allez définir ce paramètre dans votre classe d'extension,
en vous basant sur les valeurs de configuration saisies. Par exemple, supposons que vous
souhaitiez autoriser l'utilisateur à définir cette option *type* via une clé
nommée ``my_type``. Pour effectuer cela, ajoutez ce qui suit à la méthode
``load()``::

    public function load(array $configs, ContainerBuilder $container)
    {
        // préparer votre variable $config

        $loader = new XmlFileLoader($container, new FileLocator(__DIR__.'/../Resources/config'));
        $loader->load('services.xml');

        if (!isset($config['my_type'])) {
            throw new \InvalidArgumentException('The "my_type" option must be set');
        }

        $container->setParameter('acme_hello.my_service_type', $config['my_type']);
    }

Maintenant, l'utilisateur peut effectivement configurer le service en spécifiant
la valeur de configuration ``my_type`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        acme_hello:
            my_type: foo
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:acme_hello="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

            <acme_hello:config my_type="foo">
                <!-- ... -->
            </acme_hello:config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('acme_hello', array(
            'my_type' => 'foo',
            // ...
        ));

Paramètres globaux
~~~~~~~~~~~~~~~~~~

Lorsque vous configurez le conteneur, soyez conscient que vous avez à
disposition les paramètres globaux suivants :

* ``kernel.name``
* ``kernel.environment``
* ``kernel.debug``
* ``kernel.root_dir``
* ``kernel.cache_dir``
* ``kernel.logs_dir``
* ``kernel.bundle_dirs``
* ``kernel.bundles``
* ``kernel.charset``

.. caution::

    Tous les noms de paramètres et services commençant par un ``_`` sont réservés
    pour le framework, et aucun autre ne doit être défini par des bundles.

.. _cookbook-bundles-extension-config-class:

Validation et fusion avec une classe de configuration
-----------------------------------------------------

Jusqu'ici, vous avez effectué la fusion de vos tableaux de configuration
manuellement et avez verifié la présence de valeurs de configuration à la
main en utilisant la fonction PHP ``isset()``. Un système de *Configuration*
optionnel est aussi disponible et peut vous aider avec la fusion, la
validation, les valeurs par défaut, et la normalisation des formats.

.. note::

    La normalisation des formats se réfère au fait que certains formats -
    principalement XML - résultent en de légères différences concernant
    les tableaux de configuration et que ces tableaux ont besoin d'être
    « normalisés » afin de correspondre avec tout le reste.

Pour profiter de ce système, vous allez créer une classe de ``Configuration``
et construire un arbre qui définit votre configuration dans cette classe::

    // src/Acme/HelloBundle/DependencyInjection/Configuration.php
    namespace Acme\HelloBundle\DependencyInjection;

    use Symfony\Component\Config\Definition\Builder\TreeBuilder;
    use Symfony\Component\Config\Definition\ConfigurationInterface;

    class Configuration implements ConfigurationInterface
    {
        public function getConfigTreeBuilder()
        {
            $treeBuilder = new TreeBuilder();
            $rootNode = $treeBuilder->root('acme_hello');

            $rootNode
                ->children()
                ->scalarNode('my_type')->defaultValue('bar')->end()
                ->end();

            return $treeBuilder;
        }

Ceci est un exemple *très* simple, mais vous pouvez maintenant utiliser cette
classe dans votre méthode ``load()`` pour fusionner votre configuration et
forcer la validation. Si n'importe quelle autre option que ``my_type`` est
passée, l'utilisateur sera notifié avec une exception disant qu'une option
non-supportée a été passée::

    public function load(array $configs, ContainerBuilder $container)
    {
        $configuration = new Configuration();
        $config = $this->processConfiguration($configuration, $configs);

        // ...
    }

La méthode ``processConfiguration()`` utilise l'arbre de configuration que
vous avez défini dans la classe de ``Configuration`` pour valider, normaliser
et fusionner tous les tableaux de configuration ensemble.

La classe de ``Configuration`` peut être bien plus compliquée que ce qui
est montré ici, supportant des noeuds de tableaux, des noeuds « prototypes »,
une validation avancée, une normalisation spécifique à XML et une fusion
avancée. La meilleure façon de voir cela en action est d'effectuer un
« checkout » d'une des classes de Configuration coeurs, comme par exemple
la `Configuration du FrameworkBundle`_ ou la `Configuration du TwigBundle`_.

Dump de la Configuration par Défaut
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    La commande ``config:dump-reference`` a été ajoutée dans Symfony 2.1

La commande ``config:dump-reference`` permet d'afficher sur la console la
configuration yaml par défaut du bundle.

Tant que votre configuration de bundle est située à l'emplacement standard
(``YourBundle\DependencyInjection\Configuration``) et qu'elle n'a pas de
``__constructor()``, cela fonctionnera automatiquement. Si vous avez
quelque chose de différent, votre classe ``Extension`` devra surcharger
la méthode ``Extension::getConfiguration()``. Cette dernière devant
retourner une instance de votre ``Configuration``.

Des commentaires et exemples peuvent être ajoutés à vos noeuds de configuration
en utilisant les méthodes ``->info()`` et ``->example()``::

    // src/Acme/HelloBundle/DependencyExtension/Configuration.php
    namespace Acme\HelloBundle\DependencyInjection;

    use Symfony\Component\Config\Definition\Builder\TreeBuilder;
    use Symfony\Component\Config\Definition\ConfigurationInterface;

    class Configuration implements ConfigurationInterface
    {
        public function getConfigTreeBuilder()
        {
            $treeBuilder = new TreeBuilder();
            $rootNode = $treeBuilder->root('acme_hello');

            $rootNode
                ->children()
                    ->scalarNode('my_type')
                        ->defaultValue('bar')
                        ->info('ce que my_type configure')
                        ->example('exemple de paramètre')
                    ->end()
                ->end()
            ;

            return $treeBuilder;
        }

Ce texte apparaît en tant que commentaires yaml sur la sortie de la commande
``config:dump-reference``.

.. index::
   pair: Convention; Configuration

Conventions concernant les Extensions
-------------------------------------

Quand vous créez une extension, suivez ces conventions simples :

* L'extension doit être stockée dans le sous-espace de noms ``DependencyInjection`` ;

* L'extension doit être nommée d'après le nom du bundle et suffixée avec ``Extension``
  (``AcmeHelloExtension`` pour ``AcmeHelloBundle``) ;

* L'extension devrait fournir un schéma XSD.

Si vous suivez ces conventions simples, vos extensions seront déclarées
automatiquement par Symfony2. Sinon, réécrivez la méthode
:method:`Symfony\\Component\\HttpKernel\\Bundle\\Bundle::build` de Bundle
dans votre bundle::

    // ...
    use Acme\HelloBundle\DependencyInjection\UnconventionalExtensionClass;

    class AcmeHelloBundle extends Bundle
    {
        public function build(ContainerBuilder $container)
        {
            parent::build($container);

            // déclare manuellement les extensions qui ne suivent pas les conventions
            $container->registerExtension(new UnconventionalExtensionClass());
        }
    }

Dans ce cas, la classe d'extension doit aussi implémenter une méthode
``getAlias()`` et retourner un alias unique nommé après le bundle (par
exemple : ``acme_hello``). Ceci est requis car le nom de la classe ne suit
pas les standards en se terminant par ``Extension``.

De plus, la méthode ``load()`` de votre extension sera appelée *uniquement*
si l'utilisateur spécifie l'alias ``acme_hello`` dans au moins un fichier de
configuration. De nouveau, ceci est dû au fait que la classe d'Extension ne
suit pas les standards définis plus haut, donc rien ne se fait automatiquement.

.. _`Configuration du FrameworkBundle`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/FrameworkBundle/DependencyInjection/Configuration.php
.. _`Configuration du TwigBundle`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/TwigBundle/DependencyInjection/Configuration.php
