.. index::
   single: Environnements

Comment Maîtriser et Créer de nouveaux Environnements
=====================================================

Chaque application est une combinaison de code et d'un ensemble de configurations
qui dicte comment ce code doit fonctionner. La configuration peut définir la
base de données étant utilisée, si oui ou non quelque chose doit être mis en cache, ou
à quel point le « logging » doit être détaillé. Dans Symfony2, l'idée
d'« environnements » repose sur le fait que la même base de code peut être
exécutée en utilisant des configurations diverses et variées. Par exemple,
l'environnement ``dev`` devrait utiliser une configuration qui rend le
développement facile et sympa, alors que l'environnement de ``prod`` devrait
utiliser un ensemble de configurations optimisées pour la rapidité.

.. index::
   single: Environnements; Fichiers de Configuration

Environnements Différents, Fichiers de Configuration Différents
---------------------------------------------------------------

Une application Symfony2 typique démarre avec trois environnements : ``dev``,
``prod``, et ``test``. Comme nous l'avons vu, chaque « environnement » représente
simplement une façon d'exécuter la même base de code avec des configurations
différentes. Ainsi, cela ne devrait pas être une surprise pour vous que chaque
environnement charge son propre fichier de configuration. Si vous utilisez
le format de configuration YAML, les fichiers suivants sont utilisés :

* pour l'environnement ``dev`` : ``app/config/config_dev.yml``
* pour l'environnement ``prod`` : ``app/config/config_prod.yml``
* pour l'environnement ``test`` : ``app/config/config_test.yml``

Cela fonctionne grâce à un simple standard utilisé par défaut dans la classe
``AppKernel`` :

.. code-block:: php

    // app/AppKernel.php

    // ...
    
    class AppKernel extends Kernel
    {
        // ...

        public function registerContainerConfiguration(LoaderInterface $loader)
        {
            $loader->load(__DIR__.'/config/config_'.$this->getEnvironment().'.yml');
        }
    }

Comme vous pouvez le voir, lorsque Symfony2 est chargé, il utilise l'environnement
donné pour déterminer quel fichier de configuration charger. Cela permet
d'avoir des environnements multiples d'une manière élégante, puissante
et transparente.

Bien sûr, dans la réalité, chaque environnement diffère seulement quelque peu
des autres. Généralement, tous les environnements vont avoir en commun une même
configuration de base conséquente. Ouvrez le fichier de configuration « dev »
et vous verrez facilement comment ceci est accompli :

.. configuration-block::

    .. code-block:: yaml

        imports:
            - { resource: config.yml }
        # ...

    .. code-block:: xml

        <imports>
            <import resource="config.xml" />
        </imports>
        <!-- ... -->

    .. code-block:: php

        $loader->import('config.php');
        // ...

Pour partager une configuration commune, chaque fichier de configuration d'un
environnement importe en premier lieu un fichier de configuration central
(``config.yml``). Le reste du fichier peut ainsi dévier de la configuration
par défaut en surchargeant des paramètres individuels. Par exemple, par défaut,
la barre d'outils ``web_profiler`` est désactivée. Cependant, en environnement
``dev``, la barre d'outils est activée en modifiant la valeur par défaut dans
le fichier de configuration ``dev`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        imports:
            - { resource: config.yml }

        web_profiler:
            toolbar: true
            # ...

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <imports>
            <import resource="config.xml" />
        </imports>

        <webprofiler:config
            toolbar="true"
            .../>

    .. code-block:: php

        // app/config/config_dev.php
        $loader->import('config.php');

        $container->loadFromExtension('web_profiler', array(
            'toolbar' => true,
            ...,
        ));

.. index::
   single: Environnements; Exécuter différents environnements

Exécuter une Application dans Différents Environnements
-------------------------------------------------------

Pour exécuter l'application dans chaque environnement, chargez l'application en
utilisant soit le contrôleur frontal ``app.php`` (pour l'environnement ``prod``),
soit ``app_dev.php`` (pour l'environnement ``dev``) :

.. code-block:: text

    http://localhost/app.php      -> environnement *prod*
    http://localhost/app_dev.php  -> environnement *dev*

.. note::

   Les URLs données supposent que votre serveur web est configuré pour utiliser
   le répertoire ``web/`` de l'application en tant que racine. Lisez-en plus
   sur :doc:`Installer Symfony2</book/installation>`.

Si vous ouvrez l'un de ces fichiers, vous allez rapidement voir que l'environnement
utilisé pour chacun est explicitement défini :

.. code-block:: php
   :linenos:

    <?php

    require_once __DIR__.'/../app/bootstrap_cache.php';
    require_once __DIR__.'/../app/AppCache.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppCache(new AppKernel('prod', false));
    $kernel->handle(Request::createFromGlobals())->send();

Comme vous pouvez le voir, la clé ``prod`` spécifie que cet environnement
va être exécuté dans l'environnement ``prod``. Une application Symfony2
peut être exécutée dans n'importe quel environnement en utilisant ce code
et en changeant la chaîne de caractères de l'environnement.

.. note::

   L'environnement de ``test`` est utilisé lorsque vous écrivez des tests
   fonctionnels et n'est pas accessible directement dans le navigateur via
   un contrôleur frontal. En d'autres termes, comparé aux autres environnements,
   il n'y a pas de fichier de contrôleur frontal ``app_test.php``.

.. index::
   single: Configuration; Mode de Débuggage

.. sidebar:: Mode de *Débuggage*

    Quelque chose d'important - sans rapport avec le thème des *environnements* -
    est la clé ``false`` à la ligne 8 du contrôleur frontal ci-dessus. Cette
    ligne spécifie si oui ou non l'application doit être exécutée en « mode
    de débuggage ». Peu importe l'environnement, une application Symfony2
    peut être exécutée avec le mode débuggage activé ou désactivé (``true``
    ou ``false``). Cela affecte beaucoup de choses dans l'application, comme
    par exemple si oui ou non les erreurs doivent être affichées ou si les
    fichiers de cache sont dynamiquement reconstruits à chaque requête. Bien
    que ce ne soit pas une condition requise, le mode de débuggage est
    généralement défini comme ``true`` pour les environnements ``dev`` et
    ``test``, et comme ``false`` pour l'environnement ``prod``.

    En interne, la valeur du mode de débuggage devient le paramètre
    ``kernel.debug`` utilisé dans le
    :doc:`conteneur de service</book/service_container>`. Si vous regardez
    le fichier de configuration de l'application, vous verrez le paramètre
    utilisé pour, par exemple, activer ou désactiver le « logging » quand
    vous utilisez le DBAL de Doctrine :

    .. configuration-block::

        .. code-block:: yaml

            doctrine:
               dbal:
                   logging:  "%kernel.debug%"
                   # ...

        .. code-block:: xml

            <doctrine:dbal logging="%kernel.debug%" ... />

        .. code-block:: php

            $container->loadFromExtension('doctrine', array(
                'dbal' => array(
                    'logging'  => '%kernel.debug%',
                    ...,
                ),
                ...,
            ));

.. index::
   single: Environnements; Créer un nouvel environnement

Créer un Nouvel Environnement
-----------------------------

Par défaut, une application Symfony2 possède trois environnements qui gèrent
la plupart des cas. Bien sûr, comme un environnement n'est rien d'autre qu'une
chaîne de caractères qui correspond à un ensemble de configurations, créer un
nouvel environnement est assez facile.

Par exemple, supposons qu'avant un déploiement, vous ayez besoin d'effectuer
des essais sur votre application. Une manière de faire cela est
d'utiliser presque les mêmes paramètres qu'en production, mais avec le
``web_profiler`` de Symfony2 activé. Cela permet à Symfony2 d'enregistrer
des informations à propos de votre application lorsque vous effectuez vos essais.

La meilleure manière d'accomplir ceci est grâce à un nouvel environnement nommé,
par exemple, ``benchmark``. Commencez par créer un nouveau fichier de configuration :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_benchmark.yml
        imports:
            - { resource: config_prod.yml }

        framework:
            profiler: { only_exceptions: false }

    .. code-block:: xml

        <!-- app/config/config_benchmark.xml -->
        <imports>
            <import resource="config_prod.xml" />
        </imports>

        <framework:config>
            <framework:profiler only-exceptions="false" />
        </framework:config>

    .. code-block:: php

        // app/config/config_benchmark.php        
        $loader->import('config_prod.php')

        $container->loadFromExtension('framework', array(
            'profiler' => array('only-exceptions' => false),
        ));

Grâce à ce simple ajout, l'application supporte désormais un nouvel
environnement appelé ``benchmark``.

Ce nouveau fichier de configuration importe la configuration de l'environnement
``prod`` et la modifie. Cela garantit que le nouvel environnement est identique
à l'environnement ``prod``, excepté les changements effectués explicitement ici.

Comme vous allez vouloir accéder à cet environnement via un navigateur, vous
devriez aussi créer un contrôleur frontal pour lui. Copiez le fichier
``web/app.php`` vers ``web/app_benchmark.php`` et éditez l'environnement afin
qu'il contienne la valeur ``benchmark`` :

.. code-block:: php

    <?php

    require_once __DIR__.'/../app/bootstrap.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('benchmark', false);
    $kernel->handle(Request::createFromGlobals())->send();

Le nouvel environnement est maintenant accessible via::

    http://localhost/app_benchmark.php

.. note::

   Certains environnements, comme ``dev``, n'ont jamais pour but d'être accédés par
   le public sur quelconque serveur déployé. La raison de ceci est que certains
   environnements, pour des raisons de débuggage, pourraient donner trop d'informations
   à propos de l'application ou de l'infrastructure sous-jacente. Afin d'être sûr que
   ces environnements ne soient pas accessibles, le contrôleur frontal est généralement
   protégé des adresses IP externes grâce au code suivant placé en haut du contrôleur :
   
    .. code-block:: php

        if (!in_array(@$_SERVER['REMOTE_ADDR'], array('127.0.0.1', '::1'))) {
            die('You are not allowed to access this file. Check '.basename(__FILE__).' for more information.');
        }

.. index::
   single: Environnements; Le répertoire de Cache

Les environnements et le répertoire de Cache
--------------------------------------------

Symfony2 profite du cache de différentes manières : la configuration de
l'application, la configuration du routage, les templates Twig et encore plus
sont cachés dans des objets PHP stockés dans des fichiers sur le système de
fichiers.

Par défaut, ces fichiers cachés sont largement stockés dans le répertoire
``app/cache``. Cependant, chaque environnement cache son propre ensemble de fichiers :

.. code-block:: text

    app/cache/dev   - répertoire de cache pour l'environnement *dev*
    app/cache/prod  - répertoire de cache pour l'environnement *prod*

Quelquefois, lorsque vous débuggez, il pourrait être utile d'inspecter un
fichier caché pour comprendre comment quelque chose fonctionne. Quand vous
faites ça, rappelez-vous de regarder dans le répertoire de l'environnement
que vous êtes en train d'utiliser (la plupart du temps ``dev`` lorsque vous
développez et débuggez). Bien que celui-ci puisse varier, le répertoire
``app/cache/dev`` inclut ce qui suit :

* ``appDevDebugProjectContainer.php`` - le « conteneur de service » caché
  qui représente la configuration cachée de l'application ;

* ``appdevUrlGenerator.php`` - la classe PHP générée sur la base de la
  configuration de routage et utilisée lors de la génération d'URLs ;

* ``appdevUrlMatcher.php`` - la classe PHP utilisée pour la correspondance
  des routes - regardez ici pour voir la logique des expressions régulières
  compilées et utilisées pour faire correspondre les URLs entrantes aux
  différentes routes ;

* ``twig/`` - ce répertoire contient tous les templates Twig cachés.

Aller plus loin
---------------

Lisez l'article sur :doc:`/cookbook/configuration/external_parameters`.