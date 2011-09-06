L'Architecture
==============

Vous êtes mon héros ! Qui aurait pensé que vous seriez encore là après les trois
premières parties ? Vos efforts seront récompensés dans un instant.

Les trois premières parties n'explorent pas trop profondément l'architecture du
framework. Comme Symfony2 se distingue des autres frameworks, nous allons
nous y atteler dès maintenant.

Comprendre l'arborescence
-------------------------

L'arborescence d'une :term:`application` Symfony2 est plutôt flexible mais
celle de la distribution *Standard Edition* reflète la structure typique et 
recommandée d'une application Symfony2 :

* ``app/``:    La configuration de l'application,
* ``src/``:    Le code PHP du projet,
* ``vendor/``: Les librairies tierces,
* ``web/``:    Le répertoire Web racine.

Le répertoire ``web/``
~~~~~~~~~~~~~~~~~~~~~~

Le répertoire Web racine est l'endroit ou se situent tous les fichiers statiques
et publics comme les images, les feuilles de styles et les fichiers javascript. 
C'est aussi ici que se situeront les :term:`contrôleurs frontaux` :

.. code-block:: php

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

Le noyau (kernel) requiert d'abord le fichier ``bootstrap.php.cache``, qui amorce le
framework et l'autoloader (voir ci-dessous).

Comme tout contrôleur frontal, ``app.php`` utilise une classe Kernel ``AppKernel``
pour amorcer une application.

.. _the-app-dir:

Le répertoire ``app/``
~~~~~~~~~~~~~~~~~~~~~~

La classe ``AppKernel`` est le point d'entrée principal de la configuration de
l'application et, en tant que tel, il est placé dans le répertoire ``app/``.

Cette classe doit implémenter deux méthodes :

* ``registerBundles()`` doit retourner un tableau de tous les Bundles nécessaires au 
  fonctionnement de l'application.

* ``registerContainerConfiguration()`` charge la configuration de l'application
  (cette partie sera détaillée ultérieurement).

L'autoloading PHP peut être configuré via ``app/autoload.php`` :

.. code-block:: php

    // app/autoload.php
    use Symfony\Component\ClassLoader\UniversalClassLoader;

    $loader = new UniversalClassLoader();
    $loader->registerNamespaces(array(
        'Symfony'          => array(__DIR__.'/../vendor/symfony/src', __DIR__.'/../vendor/bundles'),
        'Sensio'           => __DIR__.'/../vendor/bundles',
        'JMS'              => __DIR__.'/../vendor/bundles',
        'Doctrine\\Common' => __DIR__.'/../vendor/doctrine-common/lib',
        'Doctrine\\DBAL'   => __DIR__.'/../vendor/doctrine-dbal/lib',
        'Doctrine'         => __DIR__.'/../vendor/doctrine/lib',
        'Monolog'          => __DIR__.'/../vendor/monolog/src',
        'Assetic'          => __DIR__.'/../vendor/assetic/src',
        'Metadata'         => __DIR__.'/../vendor/metadata/src',
    ));
    $loader->registerPrefixes(array(
        'Twig_Extensions_' => __DIR__.'/../vendor/twig-extensions/lib',
        'Twig_'            => __DIR__.'/../vendor/twig/lib',
    ));

    // ...

    $loader->registerNamespaceFallbacks(array(
        __DIR__.'/../src',
    ));
    $loader->register();

La classe :class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader` est utilisée 
pour charger automatiquement les fichiers qui respectent les `standards`_ 
d'interopérabilité technique pour les namespace PHP 5.3, ou les `conventions`_ 
de nommage PEAR pour les classes. Comme vous pouvez le voir ici, toutes les 
dépendances sont stockées dans le répertoire ``vendor/``, mais ce n'est juste
qu'une convention. Vous pouvez les stocker où vous le souhaitez, sur votre 
serveur ou au sein même de vos projets.


.. note::

    Si vous voulez en savoir plus sur la flexibilité de l'autoloader de Symfony2,
    lisez l'article « :doc:`/cookbook/tools/autoloader`» dans le cookbook.

Comprendre le système de bundles
--------------------------------

Cette section présente une des plus géniales et puissantes fonctionnalités de
Symfony2, le système de :term:`Bundles`.

Un bundle est une sorte de plugin comme on peut en trouver dans d'autres logiciels. 
Alors pourquoi l'a t-on nommé *bundle* et non pas *plugin* ? C'est parce que *tout*
est un bundle dans Symfony2, des fonctionnalités du noyau du framework au code que
vous écrirez pour votre application. Les bundles sont les citoyens de premier 
rang pour Symfony2. Ils vous donnent la flexibilité d'utiliser des fonctionnalités
pré-construites dans des bundles tiers ou de distribuer vos propres bundles. 
Ils facilitent la synergie et le choix des fonctionnalités à activer pour votre 
application et les optimisent de la manière que vous désirez. Et après tout, le
code de votre application est au moins aussi *important* que celui du framework.

Définir un bundle
~~~~~~~~~~~~~~~~~

Une application est constituée de bundles définis dans la méthode
``registerBundles()`` de la classe ``AppKernel``. Chaque bundle est un répertoire
qui contient une unique classe ``Bundle`` qui le décrit :

.. code-block:: php

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            new Symfony\Bundle\FrameworkBundle\FrameworkBundle(),
            new Symfony\Bundle\SecurityBundle\SecurityBundle(),
            new Symfony\Bundle\TwigBundle\TwigBundle(),
            new Symfony\Bundle\MonologBundle\MonologBundle(),
            new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
            new Symfony\Bundle\DoctrineBundle\DoctrineBundle(),
            new Symfony\Bundle\AsseticBundle\AsseticBundle(),
            new Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBundle(),
            new JMS\SecurityExtraBundle\JMSSecurityExtraBundle(),
        );

        if (in_array($this->getEnvironment(), array('dev', 'test'))) {
	    $bundles[] = new Acme\DemoBundle\AcmeDemoBundle();
            $bundles[] = new Symfony\Bundle\WebProfilerBundle\WebProfilerBundle();
            $bundles[] = new Sensio\Bundle\DistributionBundle\SensioDistributionBundle();
            $bundles[] = new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle();
        }

        return $bundles;
    }

En plus du bundle ``AcmeDemoBundle`` dont nous avons déjà parlé, notez que le
noyau active aussi d'autres bundles comme les bundles ``FrameworkBundle``,
``DoctrineBundle``, ``SwiftmailerBundle`` et ``AsseticBundle``. Ils sont tous
intégrés au framework.

Configurer un bundle
~~~~~~~~~~~~~~~~~~~~

Chaque bundle peut être personnalisé via des fichiers de configuration écrits en
YAML, XML ou PHP. Jetons un oeil à la configuration par défaut :

.. code-block:: yaml

    # app/config/config.yml
    imports:
        - { resource: parameters.ini }
        - { resource: security.yml }

    framework:
        secret:          %secret%
        charset:         UTF-8
        router:          { resource: "%kernel.root_dir%/config/routing.yml" }
        form:            true
        csrf_protection: true        
        validation:      { enable_annotations: true }
        templating:      { engines: ['twig'] } #assets_version: SomeVersionScheme
        session:
            default_locale: %locale%            
            auto_start:     true

    # Twig Configuration
    twig:
        debug:            %kernel.debug%
        strict_variables: %kernel.debug%

    # Assetic Configuration
    assetic:
        debug:          %kernel.debug%
        use_controller: false
        filters:	
            cssrewrite: ~	
            # closure:
            #     jar: %kernel.root_dir%/java/compiler.jar
            # yui_css:
            #     jar: %kernel.root_dir%/java/yuicompressor-2.4.2.jar

    # Doctrine Configuration
    doctrine:
        dbal:
            driver:   %database_driver%
            host:     %database_host%
            dbname:   %database_name%
            user:     %database_user%
            password: %database_password%
            charset:  UTF8

        orm:
            auto_generate_proxy_classes: %kernel.debug%
            auto_mapping: true

    # Swiftmailer Configuration
    swiftmailer:
        transport: %mailer_transport%
        host:      %mailer_host%
        username:  %mailer_user%
        password:  %mailer_password%

    jms_security_extra:
        secure_controllers:  true
        secure_all_services: false

Chaque entrée, comme ``framework``, définit la configuration pour un
bundle donné. Par exemple, ``framework`` configure le bundle ``FrameworkBundle``,
tandis que ``swiftmailer`` configure le bundle ``SwiftmailerBundle``.

Chaque :term:`environnement` peut surcharger la configuration par défaut en
apportant un fichier de configuration spécifique. Par exemple, l'environnement
``dev`` charge le fichier ``config_dev.yml`` qui va charger la configuration
principale (i.e. ``config.yml``) puis la modifier pour ajouter des outils de debug :

.. code-block:: yaml

    # app/config/config_dev.yml
    imports:
        - { resource: config.yml }

    framework:
        router:   { resource: "%kernel.root_dir%/config/routing_dev.yml" }
        profiler: { only_exceptions: false }

    web_profiler:
        toolbar: true
        intercept_redirects: false

    monolog:
        handlers:
            main:
                type:  stream
                path:  %kernel.logs_dir%/%kernel.environment%.log	
                level: debug	
            firephp:	
                type:  firephp	
                level: info

    assetic:
        use_controller: true

Etendre un bundle
~~~~~~~~~~~~~~~~~

En plus d'être une façon sympathique d'organiser et de configurer votre code,
les bundles peuvent étendre d'autres bundles. L'héritage entre bundle vous permet
de surcharger n'importe quel bundle pour personnaliser ses contrôleurs, ses templates,
ou n'importe lequel de ses fichiers.
C'est ici que les noms logiques (ex ``@AcmeDemoBundle/Controller/SecuredController.php``)
se revèlent pratiques : ils font abstraction de l'endroit où est stockée la ressource.

Noms logiques de fichiers	
.........................

Quand vous voulez faire référence à un fichier depuis un bundle, utilisez cette
notation :
``@NOM_DU_BUNDLE/path/to/file``; Symfony2 remplacera ``@NOM_DU_BUNDLE`` par le chemin
du bundle. A titre d'exemple, le chemin logique
``@AcmeDemoBundle/Controller/DemoController.php`` sera transformé en
``src/Acme/DemoBundle/Controller/DemoController.php``, car Symfony connait la
la location du bundle ``AcmeDemoBundle``.
	
Noms logiques de contrôleurs	
............................

Pour les contrôleurs, vous aurez besoin de référencer les noms de méthode en 
utilisant le format suivant ``NOM_DU_BUNDLE:NOM_DU_CONTROLEUR:NOM_DE_ACTION``.
Par exemple, ``AcmeDemoBundle:Welcome:index`` référencera la méthode ``indexAction``
de la classe ``Acme\DemoBundle\Controller\WelcomeController``.

Noms logiques de templates	
..........................

Pour les templates, le nom logique ``AcmeDemoBundle:Welcome:index.html.twig``
mènera vers le fichier ``src/Acme/DemoBundle/Resources/views/Welcome/index.html.twig``.
Les templates deviennent encore plus intéressants quand on réalise qu'ils n'ont 
pas besoin d'être stockés sur le système de fichiers. Vous pouvez par exemple les
stocker dans une base de données.


Etendre les bundles
...................
	
Si vous suivez ces conventions, alors vous pouvez utiliser :doc:`l'héritage de bundle</cookbook/bundles/inheritance>`
pour «surcharger» les fichiers, les contrôleurs et les templates. Par exemple, 
vous pouvez créer un nouveau bundle (``AcmeNewBundle``) et spécifier que son parent
est ``AcmeDemoBundle``. Lorsque Symfony chargera le contrôleur ``AcmeDemoBundle:Welcome:index``,
il cherchera d'abord la classe ``WelcomeController`` dans le bundle ``AcmeNewBundle``
puis dans le bundle ``AcmeDemoBundle``. Cela signifie qu'un bundle peut surcharger
presque toutes les parties d'un autre bundle!

Vous comprenez maintenant pourquoi Symfony2 est si flexible ? Partagez vos
Bundles entre applications, stockez-les localement ou globalement, c'est vous
qui décidez.

.. _using-vendors:

Utilisation de librairies externes (Vendors)
--------------------------------------------

Il y a de fortes probabilités que votre application dépende de librairies tierces.
Celles-ci doivent être stockées dans le répertoire ``vendor/``. Ce
répertoire contient déjà les librairies de Symfony2, la librairie SwiftMailer,
l'ORM Doctrine, le système de template Twig et d'autres librairies et bundles.

Comprendre le Cache et les Logs
-------------------------------

Symfony2 est probablement l'un des plus rapides framework full-stack existant.
Mais comment peut-il être si rapide s'il analyse et interprète des dizaines de
fichiers YAML et XML à chaque requête ? Cette rapidité est en partie due à son
système de cache. La configuration de l'application est uniquement analysée
lors de la première requête, puis compilée en PHP pur stocké dans le répertoire
``app/cache/`` de l'application. Dans l'environnement de développement, Symfony2 est
assez intelligent pour vider le cache lorsque vous modifiez un fichier. Mais
dans l'environnement de production, il est de votre ressort de vider le
cache lorsque vous mettez à jour votre code ou modifiez sa configuration.

Quand vous développez une application Web, les choses peuvent mal tourner, et 
ce de multiples façons. Le fichier log dans le répertoire ``app/logs/`` de votre 
application vous dira tout concernant les requêtes et vous aidera à résoudre 
vos soucis rapidement.

Utiliser l'Interface en Ligne de Commande (CLI)
--------------------------------------

Chaque application est fournie avec une interface en ligne de commandes
(``app/console``) qui vous aidera à maintenir votre application. Cette interface
met à votre disposition des commandes qui augmentent votre productivité en
automatisant les tâches fastidieuses et répétitives.

Lancez-la sans aucun argument pour en apprendre plus sur ses possibilités :

.. code-block:: bash

    $ php app/console

L'option ``--help`` vous renseignera sur une commande précise :

.. code-block:: bash

    $ php app/console router:debug --help

Le mot de la fin
----------------

Vous pouvez trouver ça insensé mais après avoir lu cette partie, vous
devriez être suffisament à l'aise pour faire vos premières griffes et laisser
Symfony2 travailler pour vous. Tout est fait dans Symfony2 pour que vous traciez
votre voie. Alors, n'hésitez pas à renommer et déplacer des répertoires comme
bon vous semble.

C'en est tout pour ce Quick Tour. Des tests à l'envoi d'e-mails, vous
avez encore besoin d'en apprendre beaucoup pour devenir un expert Symfony2. Prêt
à plonger dans ces thèmes maintenant ? Ne cherchez plus, consultez le
:doc:`/book/index` et choisissez le sujet qui vous intéresse.

.. _standards:               http://groups.google.com/group/php-standards/web/psr-0-final-proposal
.. _conventions:              http://pear.php.net/
