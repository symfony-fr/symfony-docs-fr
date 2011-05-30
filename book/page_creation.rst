.. index::
   single: Page creation

La création de pages avec Symfony2
==================================

Créer des pages avec Symfony2 se fait en simplement deux étapes:

* *Créez une route*: Une route définit l'URI (ex: ``/apropos``) pour votre
  page et spécifie un contrôleur (une fonction PHP) que Symfony2 devrait
  exécuter quand l'URI d'une requête HTTP correspond à une route que vous 
  avez défini.

* *Créez un contrôleur*: Un contrôleur est une fonction PHP qui traitera la 
  requête HTTP et la transformera en un objet ``Response`` Symfony2.

Nous aimons cette approche simplissime car elle correspond à la façon dont 
fonctionne le Web. Chaque interaction sur le Web est initiée par une requête
HTTP. Le but de votre application est simplement d'interpréter cette requête 
et de lui retourner une Response HTTP appropriée. Symfony2 suit cette philosophie 
et vous fournit des outils et conventions pour garder votre application organisée
tout en pouvant devenir plus complexe et fréquentée.

.. index::
   single: Page creation; Example

La page "Hello Symfony !"
-------------------------

Commençons avec une application traditionnelle "Hello World !". Quand nous
aurons terminé, l'utilisateur sera capable de reçevoir un message de 
salutation personnalisé en se rendant à l'URL suivante :

.. code-block:: text

   http://localhost/app_dev.php/hello/Symfony

En réalité, vous serez capables de pouvoir remplacer ``Symfony`` par n'importe
quel autre nom qui doit être salué. Afin de créer cette page, nous allons 
effectuer les deux étapes citées plus haut.

.. note::

   Ce tutoriel suppose que vous ayez déjà téléchargé et configuré
   Symfony2 sur votre serveur web. L'URL ci-dessus suppose que ``localhost``
   pointe vers le répertoire ``web`` de votre nouveau projet Symfony2.
   Pour des instructions détaillées à ce sujet, rendez vous dans la section
   respective de la documentation :doc:`Installez Symfony2</book/installation>`.


Créez le  Bundle
~~~~~~~~~~~~~~~~~

Avant de commencer, vous devez créer un *bundle*. Dans Symfony2, un bundle
est comme un plugin, excepté le fait que tout le code de votre application
siègera dans un bundle.

Un bundle n'est rien d'autre qu'un répertoire (avec un namespace PHP) qui
abrite tout ce qui est relatif à une fonctionnalité spécifique (voyez :ref:`page-creation-bundles`).
Afin de créer un bundle nommé ``AcmeStudyBundle`` (un bundle fictif que vous 
créerez dans ce chapitre), lancez la commande suivante :

.. code-block:: text

   php app/console init:bundle "Acme\StudyBundle" src

Ensuite, assurez-vous que le namespace ``Acme`` soit bien ajouté au fichier
``app/autoload.php`` (voyez :ref:`Autoloading sidebar<autoloading-introduction-sidebar>`):

.. code-block:: php

        $loader->registerNamespaces(array(
             'Acme' => __DIR__.'/../src',
             // ...
        ));

Pour terminer, initialisez le bundle en l'ajoutant à la méthode ``registerBundles``
de la classe ``AppKernel`` :

.. code-block:: php

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Acme\StudyBundle\AcmeStudyBundle(),
        );
        
        // ...

        return $bundles;
    }

Maintenant que votre bundle est mis en place, vous pouvez commencer à
construire votre application à l'intérieur du bundle.

Créez la Route
~~~~~~~~~~~~~~

Dans une application Symfony2, le fichier de configuration des routes
se trouve par défaut dans ``app/config/routing.yml``. Comme toute
configuration dans Symfony2, vous pouvez également choisir d'utiliser
des fichiers XML ou PHP afin de configurer vos routes :

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        homepage:
            pattern:  /
            defaults: { _controller: FrameworkBundle:Default:index }

        hello:
            resource: "@AcmeStudyBundle/Resources/config/routing.yml"

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="homepage" pattern="/">
                <default key="_controller">FrameworkBundle:Default:index</default>
            </route>

            <import resource="@AcmeStudyBundle/Resources/config/routing.xml" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('homepage', new Route('/', array(
            '_controller' => 'FrameworkBundle:Default:index',
        )));
        $collection->addCollection($loader->import("@AcmeStudyBundle/Resources/config/routing.php"));

        return $collection;

Les premières lignes d'un fichier de configuration de routage définit quel
code appeler quand l'utilisateur demande la ressource "``/``" (la page d'accueil)
et servent d'exemple de configurations de routage que vous pouvez trouver dans ces
fichiers. La dernière partie est plus intéressante, elle importe un autre fichier
de configuration qui se trouve dans le ``AcmeStudyBundle`` :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/StudyBundle/Resources/config/routing.yml
        hello:
            pattern:  /hello/{name}
            defaults: { _controller: AcmeStudyBundle:Hello:index }

    .. code-block:: xml

        <!-- src/Acme/StudyBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="hello" pattern="/hello/{name}">
                <default key="_controller">AcmeStudyBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Acme/StudyBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeStudyBundle:Hello:index',
        )));

        return $collection;

Le routage est constitué de deux parties principales: le ``motif``, qui est
l'URI correspondante à cette route, et un array ``par défaut``, qui spécifie
le contrôleur qui devra être exécuté. La syntaxe pour le paramètre dans le 
motif (``{name}``) est un joker. Cela signifier que ``hello/Jean``, ``hello/Bernard``
ou n'importe quelle URI similaire correspondra à cette route. Le paramètre ``{name}``
sera également passé à notre contrôleur afin que nous puissions utiliser la valeur
afin de saluer l'utilisateur.

.. note::

   Le système de routage dispose de beaucoup d'autres fonctionnalités
   qui vous permettront de créer des structures d'URI puissantes et flexibles
   dans votre application. Pour plus de détails, lisez le chapitre dédié
   aux routes :doc:`Routing </book/routing>`.  

Créez le Contrôleur
~~~~~~~~~~~~~~~~~~~

Quand une URI comme ``/hello/Jean`` is traitée par l'application, la route
``hello`` est correspondante et le contrôleur ``AcmeStudyBundle:Hello/index``
est excécuté par le framework. L'étape suivante est de créer ce contrôleur.

En réalité, un contrôleur n'est rien d'autre qu'une méthode PHP que vous créez
et que Symfony exécute. C'est à cet endroit que le code propre à l'application
utilisera les informations de la requête afin de construire et préparer la 
ressource demandée par la requête. Excepté dans certains situations avancées, 
le résultat final d'un contrôleur sera toujours le même :
un objet ``Response`` Symfony2 ::

    // src/Acme/StudyBundle/Controller/HelloController.php

    namespace Acme\StudyBundle\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
        public function indexAction($name)
        {
            return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

Le contrôleur est simple: il crée un nouvel objet ``Response``, qui a pour 
premier argument le contenu qui doit être renvoyé par la Response (une petite
page HTML dans ce cas-ci).


Félicitations ! Après avoir n'avoir créé qu'une route et un contrôleur, vous
avez une page pleinement fonctionnelle ! Si vous avez tout effectué correctement,
votre application devrait vous saluer::

    http://localhost/app_dev.php/hello/Christophe

Une troisième étape optionelle dans ce processus est de créer un template.

.. note::

   Les contrôleurs sont le point central de votre code et un élément clé
   pendant la création de pages. Plus d'informations peuvent être trouvées
   dans le :doc:`Chapitre Contrôleurs </book/controller>`.

Créez le Template
~~~~~~~~~~~~~~~~~

Les templates vous permettent de déplacer toute la présentation (ex: code HTML)
dans un fichier séparé et de réutiliser différentes portions d'un gabarit.
A la place d'écrire le code HTML dans le contrôleur, utilisez plutôt un template::


    // src/Acme/StudyBundle/Controller/HelloController.php

    namespace Acme\StudyBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
            return $this->render('AcmeStudyBundle:Hello:index.html.twig', array('name' => $name));

            // render a PHP template instead
            // return $this->render('AcmeStudyBundle:Hello:index.html.php', array('name' => $name));
        }
    }

.. note::

   Afin d'utiliser la méthode ``render()``, vous devez étendre la classe 
   :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`, qui
   contient des raccourcis pour des tâches fréquemment utilisées.

La méthode ``render()`` crée un objet ``Response`` qui contient le contenu
d'un template rendu. Comme tout autre contrôleur, vous retournerez cet objet
``Response``.

Notez qu'il y a deux différents exemples afin de rendre un template.
Par défaut, Symfony2 supporte deux différents langages de templates :
les templates classiques PHP et les succints mais puissants templates ``Twig``.
Ne paniquez pas, vous êtes libres de choisir de choisir celui que vous désirez
voire même les deux.

Le contrôleur rend le template ``AcmeStudyBundle:Hello:index.html.twig``,
qui suit la convention de nommage :

*BundleName*:*ControllerName*:*TemplateName*

Dans ce cas, ``AcmeStudyBundle`` est le nom de bundle, ``Hello`` est le
nom du contrôleur et enfin ``index.html.twig`` est le template :

.. configuration-block::

    .. code-block:: jinja
       :linenos:

        {# src/Acme/StudyBundle/Resources/views/Hello/index.html.twig #}
        {% extends '::layout.html.twig' %}

        {% block body %}
            Hello {{ name }}!
        {% endblock %}

    .. code-block:: php

        <!-- src/Acme/StudyBundle/Resources/views/Hello/index.html.php -->
        <?php $view->extend('::layout.html.php') ?>

        Hello <?php echo $view->escape($name) ?>!

Analysons maintenant le template Twig ligne par ligne :

* *Ligne 2* : Le symbole ``extends`` définit un template parent. Le template
  définit explicitement un fichier layout dans lequel il sera inséré.

* *Ligne 4* : Le symbole ``block`` indique que tout ce qui est à l'intérieur doit
  être placé à l'intérieur d'un bloc appelé ``body``. Comme nous le voyons, c'est
  en définitive la responsabilité du template parent (``layout.html.twig``) de rendre
  le bloc ``body``.

Le nom de fichier du template parent, ``::layout.html.twig``, est exempté des portions
du nom de bundle et contrôleur (remarquez les deux points (``::``) au début). Ceci
signifie que le template se site en dehors du bundle et dans le répertoire ``app``.

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/layout.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block title %}Hello Application{% endblock %}</title>
            </head>
            <body>
                {% block body %}{% endblock %}
            </body>
        </html>

    .. code-block:: php

        <!-- app/Resources/views/layout.html.php -->
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title><?php $view['slots']->output('title', 'Hello Application') ?></title>
            </head>
            <body>
                <?php $view['slots']->output('_content') ?>
            </body>
        </html>

Le fichier du template de base définit le layout HTML en rend le bloc ``body``
que nous avons défini dans le template ``index.html.twig``. Il rend également 
un bloc ``title``, que nous pouvons choisir de définir dans le template 
``index.html.twig``. Si nous ne définissons pas le bloc ``title`` dans le template
enfant, il aura pour valeur par défaut ``Hello Application``.

Les templates sont une façon puissante de rendre et d'organiser le contenu
pour votre page et peuvent être des layouts HTML, codes CSS, ou n'importe
quoi d'autre que le contrôleur peut avoir besoin de retourner à l'utilisateur.
Le moteur de templates n'est qu'une façon d'arrive à sa fin. Le but de chaque
contrôleur est de renvoyer un objet ``Response``. Le moteur de templates est
un outil puissant, bien qu'optionnel, pour créer le contenu de l'objet ``Response``.

.. index::
   single: Directory Structure

La structure des répertoires
----------------------------

Après seulement quelques courtes sections, vous comprenez déjà la philosophie derrière la création et le rendu de pages dans Symfony2. 
Vous avez déjà commencé à voir commet les projets Symfony2 sont structurés et organisés. 
A la fin de cette section, vous saurez où trouver et mettre les différents types de fichiers et pourquoi.

Malgré le fait que ce soit tout à fait flexible, chaque :term:`application` 
a par défaut la même structure de répertoires basique mais recommandée:

* ``app/``: Ce répertoire contient la configuration de l'application;

* ``src/``: Tout le code PHP du projet est stocké ici;

* ``vendor/``: ar convention, toutes les librairies tierces (additionnelles) sont placées ici;

* ``web/``: Le répertoire racine web qui contient tous les fichiers publiquement accessibles;

Le répertoire Web
~~~~~~~~~~~~~~~~~

Le répertoire web contient tous les fichiers publics et statiques tels que les images, les feuilles de style et les javascripts. 
Il contient également le
:term:`front controller` ::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->handle(Request::createFromGlobals())->send();

Le fichier du contrôleur frontal (``app.php`` dans cet exemple) est le fichier exécuté lorsqu'on appelle une application Symfony2. 
Son job est d'utiliser une classe Kernel, ``AppKernel``, pour lancer (amorcer, bootstrapper) l'application.

.. tip::
	
   
   avoir un front contrôler signifie des URL différentes et plus flexibles que dans une application en pur php. 
   Lorsqu'on utilise un front contrôler, les URL sont formatée comme suit:

       http://localhost/app.php/hello/Ryan

   Le contrôlleur frontal, ``app.php``, est exécuté et l'URI ``/hello/Ryan``
   est routée en interne en se basant sur la configuration du routage. En utilisant les règles du module Apache
   ``mod_rewrite``, vous pouvez forceer le script ``app.php`` à être exécuté sans
   avoir besoin de le mentionner dans l'URL ::

    http://localhost/hello/Ryan

Les contrôleurs frontaux sont essentiels pour traiter chaque requête. 
Cepeandant, vous aurez rarement besoin de les modifier ou même d'y penser. 
Nous en reparlerons dans la section `Environments`_ .

Le répertoire de l'application (``app``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Comme nous l'avons vu dans le contrôleur frontal, la classe ``AppKernel`` est le point d'entrée principal de l'application et est chargée de toute la configuration. 
De ce fait, elle est stockée dans le répertoire ``app/``.

Cette classe doit implémenter trois méthodes  qui définissent tout ce dont Symfony a besoin de savoir à propos de votre application.
Vous n'avez pas à vous soucier de ces méthodes en commençant - Symfony les complète pour vous avec de valeurs par défaut.
*``registerBundles()``: renvoit un tableau de tous les bundles dont l'application a besoin pour fonctionner (voir le [Système de Bundles]);

*``registerCoontainerConfiguration()``: Charge le fichier de ressources de l'application principal (voir la section `Configuration de l'Application`_]

*``registerRootDir()``: renvoie le répertoire racine de l'application (``app/``, par défaut)

Dans le développement au quotidien, vous utiliserez principalement le répertoire ``app/` 
pour modifier les fichiers de configuration et de routage dans le répertoire ``app/config`` (voir `Configuration de l'application`_). 
``app/`` contient également le répertoire de cache de l'application (``app/cache``), le répertoire des logs (``app/logs``) 
et un répertoire pour les ressources communes à l'application entière (``app/Resources``). 
Nous en apprendrons plus sur ces répertoires dans de prochains chapitres.

.. _autoloading-introduction-sidebar:

.. sidebar:: Autoloading
Au démarrage de l'application, un fichier spécial - ``app/autoload.php`` - est inclus. Ce fichier s'occupe du chargement automatique de tous les fichiers dans les répertoires ``src/`` et ``vendor/``.
Grace à l'autoloader, vous n'avez jamais à vous soucier d'utiliser les instructions ``include`` ou ``require``. Symfony2 se base sur l'espace de nom d'une classe pour déterminer son emplacement et l'inclure automatiquement le fichier à votre place à l'instant où vous avez besoin de votre classe::
    
        $loader->registerNamespaces(array(
            'Acme' => __DIR__.'/../src',
            // ...
        ));
    
    Avec cette configuration, Symfony2 va rechercher toutes les classes de l'espace de nom ``Acme`` (le nom de votre fausse société)  dans le répertoire ``src/``.
Pour que le chargement automatique fonctionne, le nom de la classe et le chemin du fichier doivent avoir une structure similaire::

    .. code-block:: text

        Nom de classe:
            Acme\StudyBundle\Controller\HelloController
        Chemin:
            src/Acme/StudyBundle/Controller/HelloController.php

    ``app/autoload.php`` configure le chargeur automatique pour chercher différents espaces de nom dans différents répertoires 
   	et peut être personnalisé si nécessaire. 
	Pour plus d'informations sur le chargement automatique, voir :doc: `Comment charger automatiquement des classes</cookbook/tools/autoloader>`.

Le répertoire des sources (``src/``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour faire simple, le répertoire ``src/`` contient tout le code php qui fait tourner l'application. 
En fait en développant, le plus gros du travail sera faire à l'intérieur de ce répertoire. Ce répertoire est vide par défaut. 
Quand vous commencez le développement, vous êtes amené à peupler ce répertoire avec les *bundles* qui contiennent le code de votre application.

Mais qu'est-ce au juste qu'un :term:`bundle`?

.. _page-creation-bundles:

Le Système de Bundles
---------------------

A bundle is similar to a plugin in other software, but even better. The key
difference is that *everything* is a bundle in Symfony2, including both the
core framework functionality and the code written for your application.
Bundles are first-class citizens in Symfony2. This gives you the flexibility
to use pre-built features packaged in `third-party bundles`_ or to distribute
your own bundles. It makes it easy to pick and choose which features to enable
in your application and to optimize them the way you want.

.. note::

   While we'll cover the basics here, an entire chapter is devoted to the topic
   of :doc:`/book/bundles`.

A bundle is simply a structured set of files within a directory that
implement a single feature. You might create a BlogBundle, a ForumBundle
or a bundle for user management (many of these exist already as open source
bundles). Each directory contains everything related to that feature, including
PHP files, templates, stylesheets, Javascripts, tests and anything else.
Every aspect of a feature exists in a bundle and every feature lives in a
bundle.

An application is made up of bundles as defined in the ``registerBundles()``
method of the ``AppKernel`` class::

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

            // register your bundles
            new Acme\StudyBundle\AcmeStudyBundle(),
        );

        if (in_array($this->getEnvironment(), array('dev', 'test'))) {
            $bundles[] = new Symfony\Bundle\WebProfilerBundle\WebProfilerBundle();
        }

        return $bundles;
    }

With the ``registerBundles()`` method, you have total control over which bundles
are used by your application (including the core Symfony bundles).

.. tip::

   A bundle can live *anywhere* as long as it can be autoloaded by Symfony2.
   For example, if ``AcmeStudyBundle`` lives inside the ``src/Acme``
   directory, be sure that the ``Acme`` namespace has been added to the
   ``app/autoload.php`` file and mapped to the ``src`` directory.

Creating a Bundle
~~~~~~~~~~~~~~~~~

To show you how simple the bundle system is, let's create a new bundle called
``AcmeTestBundle`` and enable it.

First, create a ``src/Acme/TestBundle/`` directory and add a new file
called ``AcmeTestBundle.php``::

    // src/Acme/TestBundle/AcmeTestBundle.php
    namespace Acme\TestBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeTestBundle extends Bundle
    {
    }

.. tip::

   The name ``AcmeTestBundle`` follows the :ref:`Bundle naming conventions<bundles-naming-conventions>`.

This empty class is the only piece we need to create our new bundle. Though
commonly empty, this class is powerful and can be used to customize the behavior
of the bundle.

Now that we've created our bundle, we need to enable it via the ``AppKernel``
class::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...

            // register your bundles
            new Acme\TestBundle\AcmeTestBundle(),
        );

        // ...

        return $bundles;
    }

And while it doesn't do anything yet, ``AcmeTestBundle`` is now ready to
be used.

And as easy as this is, Symfony also provides a command-line interface for
generating a basic bundle skeleton::

    php app/console init:bundle "Acme\TestBundle" src

The bundle skeleton generates with a basic controller, template and routing
resource that can be customized. We'll talk more about Symfony2's command-line
tools later.

.. tip::

   Whenever creating a new bundle or using a third-party bundle, always make
   sure the bundle has been enabled in ``registerBundles()``.

Bundle Directory Structure
~~~~~~~~~~~~~~~~~~~~~~~~~~

The directory structure of a bundle is simple and flexible. By default, the
bundle system follows a set of conventions that help to keep code consistent
between all Symfony2 bundles. Let's take a look at ``AcmeStudyoverBundle``, as it
contains some of the most common elements of a bundle:

* *Controller/* contains the controllers of the bundle (e.g. ``HelloController.php``);

* *Resources/config/* houses configuration, including routing configuration
  (e.g. ``routing.yml``);

* *Resources/views/* templates organized by controller name (e.g. ``Hello/index.html.twig``);

* *Resources/public/* contains web assets (images, stylesheets, etc) and is
  copied or symbolically linked into the project ``web/`` directory;

* *Tests/* holds all tests for the bundle.

A bundle can be as small or large as the feature it implements. It contains
only the files you need and nothing else.

As you move through the book, you'll learn how to persist objects to a database,
create and validate forms, create translations for your application, write
tests and much more. Each of these has their own place and role within the
bundle.

Application Configuration
-------------------------

An application consists of a collection of bundles representing all of the
features and capabilities of your application. Each bundle can be customized
via configuration files written in YAML, XML or PHP. By default, the main
configuration file lives in the ``app/config/`` directory and is called
either ``config.yml``, ``config.xml`` or ``config.php`` depending on which
format you prefer:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            charset:         UTF-8
            secret:          xxxxxxxxxx
            form:            true
            csrf_protection: true
            router:          { resource: "%kernel.root_dir%/config/routing.yml" }
            validation:      { enable_annotations: true }
            templating:      { engines: ['twig'] } #assets_version: SomeVersionScheme
            session:
                default_locale: en
                lifetime:       3600
                auto_start:     true

        # Twig Configuration
        twig:
            debug:            %kernel.debug%
            strict_variables: %kernel.debug%

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config charset="UTF-8" error-handler="null" cache-warmer="false" secret="xxxxxxxxxx">
            <framework:router resource="%kernel.root_dir%/config/routing.xml" cache-warmer="true" />
            <framework:validation annotations="true" />
            <framework:session default-locale="en" lifetime="3600" auto-start="true" />
            <framework:templating assets-version="SomeVersionScheme" cache-warmer="true">
                <framework:engine id="twig" />
            </framework:templating>
            <framework:form />
            <framework:csrf-protection />
        </framework:config>

        <!-- Twig Configuration -->
        <twig:config debug="%kernel.debug%" strict-variables="%kernel.debug%" cache-warmer="true" />

    .. code-block:: php

        $container->loadFromExtension('framework', array(
            'charset'         => 'UTF-8',
            'secret'          => 'xxxxxxxxxx',
            'form'            => array(),
            'csrf-protection' => array(),
            'router'          => array('resource' => '%kernel.root_dir%/config/routing.php'),
            'validation'      => array('annotations' => true),
            'templating'      => array(
                'engines' => array('twig'),
                #'assets_version' => "SomeVersionScheme",
            ),
            'session' => array(
                'default_locale' => "en",
                'lifetime'       => "3600",
                'auto_start'     => true,
            ),
        ));

        // Twig Configuration
        $container->loadFromExtension('twig', array(
            'debug'            => '%kernel.debug%',
            'strict_variables' => '%kernel.debug%',
        ));

.. note::

   We'll show you how to choose exactly which file/format to load in the
   next section `Environments`_.

Each top-level entry like ``framework`` or ``twig`` defines the configuration
for a particular bundle. For example, the ``framework`` key defines the configuration
for the core Symfony ``FrameworkBundle`` and includes configuration for the
routing, templating, and other core systems.

For now, don't worry about the specific configuration options in each section.
The configuration file ships with sensible defaults. As you read more and
explore each part of Symfony2, you'll learn about the specific configuration
options of each feature.

.. sidebar:: Configuration Formats

    Throughout the chapters, all configuration examples will be shown in all
    three formats (YAML, XML and PHP). Each has its own advantages and
    disadvantages. The choice of which to use is up to you:

    * *YAML*: Simple, clean and readable;

    * *XML*: More powerful than YAML at times and supports IDE autocompletion;

    * *PHP*: Very powerful but less readable than standard configuration formats.

.. index::
   single: Environments; Introduction

.. _environments-summary:

Environments
------------

An application can run in various environments. The different environments
share the same PHP code (apart from the front controller), but can have completely
different configurations. For instance, a ``dev`` environment will log warnings
and errors, while a ``prod`` environment will only log errors. Some files
are rebuilt on each request in the ``dev`` environment, but cached in the
``prod`` environment. All environments live together on the same machine.

A Symfony2 project generally begins with three environments (``dev``, ``test``
and ``prod``), though creating new environments is easy. You can view your
application in different environments simply by changing the front controller
in your browser. To see the application in the ``dev`` environment, access
the application via the development front controller::

    http://localhost/app_dev.php/hello/Ryan

If you'd like to see how your application will behave in the production environment,
call the ``prod`` front controller instead::

    http://localhost/app.php/hello/Ryan

.. note::

   If you open the ``web/app.php`` file, you'll find that it's configured explicitly
   to use the ``prod`` environment::
   
       $kernel = new AppCache(new AppKernel('prod', false));
   
   You can create a new front controller for a new environment by copying
   this file and changing ``prod`` to some other value.

Since the ``prod`` environment is optimized for speed; the configuration,
routing and Twig templates are compiled into flat PHP classes and cached.
When viewing changes in the ``prod`` environment, you'll need to clear these
cached files and allow them to rebuild::

    rm -rf app/cache/*

.. note::

    The ``test`` environment is used when running automated tests and cannot
    be accessed directly through the browser. See the :doc:`testing chapter </book/testing>`
    for more details.

.. index::
   single: Environments; Configuration

Environment Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~

The ``AppKernel`` class is responsible for actually loading the configuration
file of your choice::

    // app/AppKernel.php
    public function registerContainerConfiguration(LoaderInterface $loader)
    {
        $loader->load(__DIR__.'/config/config_'.$this->getEnvironment().'.yml');
    }

We already know that the ``.yml`` extension can be changed to ``.xml`` or
``.php`` if you prefer to use either XML or PHP to write your configuration.
Notice also that each environment loads its own configuration file. Consider
the configuration file for the ``dev`` environment.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        imports:
            - { resource: config.yml }

        framework:
            router:   { resource: "%kernel.root_dir%/config/routing_dev.yml" }
            profiler: { only_exceptions: false }

        web_profiler:
            toolbar: true
            intercept_redirects: true

        zend:
            logger:
                priority: debug
                path:     %kernel.logs_dir%/%kernel.environment%.log

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <imports>
            <import resource="config.xml" />
        </imports>

        <framework:config>
            <framework:router resource="%kernel.root_dir%/config/routing_dev.xml" />
            <framework:profiler only-exceptions="false" />
        </framework:config>

        <webprofiler:config
            toolbar="true"
            intercept-redirects="true"
        />

        <zend:config>
            <zend:logger priority="info" path="%kernel.logs_dir%/%kernel.environment%.log" />
        </zend:config>

    .. code-block:: php

        // app/config/config_dev.php
        $loader->import('config.php');

        $container->loadFromExtension('framework', array(
            'router'   => array('resource' => '%kernel.root_dir%/config/routing_dev.php'),
            'profiler' => array('only-exceptions' => false),
        ));

        $container->loadFromExtension('web_profiler', array(
            'toolbar' => true,
            'intercept-redirects' => true,
        ));

        $container->loadFromExtension('zend', array(
            'logger' => array(
                'priority' => 'info',
                'path'     => '%kernel.logs_dir%/%kernel.environment%.log',
            ),
        ));

The ``imports`` key is similar to a PHP ``include`` statement and guarantees
that the main configuration file (``config.yml``) is loaded first. The rest
of the file tweaks the default configuration for increased logging and other
settings conducive to a development environment.

Both the ``prod`` and ``test`` environments follow the same model: each environment
imports the base configuration file and then modifies its configuration values
to fit the needs of the specific environment.

Summary
-------

Congratulations! You've now seen every fundamental aspect of Symfony2 and have
hopefully discovered how easy and flexible it can be. And while there are
*a lot* of features still to come, be sure to keep the following basic points
in mind:

* creating a page is a three-step process involving a **route**, a **controller**
  and (optionally) a **template**.

* each application should contain only four directories: **web/** (web assets and
  the front controllers), **app/** (configuration), **src/** (your bundles),
  and **vendor/** (third-party code);

* each feature in Symfony2 (including the Symfony2 framework core) is organized
  into a *bundle*, which is a structured set of files for that feature;

* the **configuration** for each bundle lives in the ``app/config`` directory
  and can be specified in YAML, XML or PHP;

* each **environment** is accessible via a different front controller (e.g.
  ``app.php`` and ``app_dev.php``) and loads a different configuration file.

From here, each chapter will introduce you to more and more powerful tools
and advanced concepts. The more you know about Symfony2, the more you'll
appreciate the flexibility of its architecture and the power it gives you
to rapidly develop applications.

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/controller/service`
* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/tools/autoloader`
* :doc:`/cookbook/symfony1`

.. _`Twig`: http://www.twig-project.org
.. _`third-party bundles`: http://symfony2bundles.org/
.. _`Symfony Standard Edition`: http://symfony.com/download
