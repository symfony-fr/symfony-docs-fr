.. index::
   single: Page creation

La création de pages avec Symfony2
==================================

Créer des pages avec Symfony2 se fait en simplement deux étapes :

* *Créez une route* : Une route définit l'URL (ex: ``/apropos``) pour votre
  page et spécifie un contrôleur (une fonction PHP) que Symfony2 devrait
  exécuter quand l'URL d'une requête HTTP correspond à une route que vous 
  avez définie.

* *Créez un contrôleur* : Un contrôleur est une fonction PHP qui traitera la 
  requête HTTP et la transformera en un objet ``Response`` Symfony2 qui sera
  retourné à l'utilisateur.

Cette approche très simple est excellente car elle correspond à la façon dont 
fonctionne le Web. Chaque interaction sur le Web est initiée par une requête
HTTP. Le but de votre application est simplement d'interpréter cette requête 
et de lui retourner une Response HTTP appropriée.

Symfony2 suit cette philosophie et vous fournit des outils et conventions pour
garder votre application organisée tout en pouvant devenir plus complexe et fréquentée.

Cela vous parait suffisamment simple ? Alors allons-y !

.. index::
   single: Page creation; Example

La page « Hello Symfony ! »
---------------------------

Commençons avec une application traditionnelle « Hello World ! ». Quand nous
aurons terminé, l'utilisateur sera capable de reçevoir un message de 
salutation personnalisé (ex « Hello Symfony ») en se rendant à l'URL suivante :

.. code-block:: text

   http://localhost/app_dev.php/hello/Symfony

En fait, vous serez capable de pouvoir remplacer ``Symfony`` par n'importe
quel autre nom qui doit être salué. Afin de créer cette page, suivez ce simple
processus en deux étapes.

.. note::

   Ce tutoriel suppose que vous ayez déjà téléchargé et configuré
   Symfony2 sur votre serveur web. L'URL ci-dessus suppose que ``localhost``
   pointe vers le répertoire ``web`` de votre nouveau projet Symfony2.
   Pour des instructions détaillées à ce sujet, rendez vous dans la section
   traitant de la documentation :doc:`Installez Symfony2</book/installation>`.


Avant de commencer : Créez un Bundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Avant de commencer, vous devez créer un *bundle*. Dans Symfony2, un :term:`bundle`
est comme un plugin, excepté le fait que tout le code de votre application
siègera dans un bundle.

Un bundle n'est rien d'autre qu'un répertoire qui contient tout ce qui est relatif
à une fonctionnalité spécifique, ce qui inclut les classes PHP, la configuration
et même les feuilles de style et le javascript (voir :ref:`page-creation-bundles`).
Afin de créer un bundle nommé ``AcmeHelloBundle`` (un bundle fictif que vous 
créerez dans ce chapitre), lancez la commande suivante et suivez les instructions
affichées à l'écran (choisissez les options par défaut) :

.. code-block:: bash

	
    php app/console generate:bundle --namespace=Acme/HelloBundle --format=yml

Un répertoire ``src/Acme/HelloBundle`` est créé pour le bundle. Une ligne est aussi
automatiquement ajoutée au fichier ``app/AppKernel.php`` afin que le bundle soit
correctement enregistré::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Acme\HelloBundle\AcmeHelloBundle(),
        );        
        // ...

        return $bundles;
    }

Maintenant que votre bundle est mis en place, vous pouvez commencer à
construire votre application à l'intérieur du bundle.

Etape 1 : Créez la Route
~~~~~~~~~~~~~~~~~~~~~~~~

Dans une application Symfony2, le fichier de configuration des routes
se trouve par défaut dans ``app/config/routing.yml``. Comme toute
configuration dans Symfony2, vous pouvez également choisir d'utiliser
des fichiers XML ou PHP afin de configurer vos routes.

Si vous regardez le fichier de routage principal, vous verrez que Symfony a déjà
ajouté une entrée lorsque vous avez généré le ``AcmeHelloBundle`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
         AcmeHelloBundle:
             resource: "@AcmeHelloBundle/Resources/config/routing.yml"
             prefix:   /

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" prefix="/" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->addCollection(
            $loader->import('@AcmeHelloBundle/Resources/config/routing.php'),
            '/',
        );
        return $collection;

Les premières lignes d'un fichier de configuration de routage définissent quel
code appeler quand l'utilisateur demande la ressource « ``/`` » (la page d'accueil)
et servent d'exemple de configuration de routage que vous pouvez trouver dans ces
fichiers. La dernière partie est plus intéressante, elle importe un autre fichier
de configuration qui se trouve dans le ``AcmeHelloBundle`` :

Ce code est très basique : il dit à Symfony de charger la configuration de routage
depuis le fichier ``Resources/config/routing.yml`` qui se trouve dans le ``AcmeHelloBundle``.
Cela signifie que vous pouvez placer votre configuration de routage directement
dans le fichier ``app/config/routing.yml`` ou organiser vos routes dans votre
applications et les importer depuis ce fichier.

Maintenant que le fichier ``routing.yml`` du bundle est importé, ajoutez la nouvelle
route qui définit l'URL de la page que vous êtes sur le point de créer :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/routing.yml
        hello:
            pattern:  /hello/{name}
            defaults: { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="hello" pattern="/hello/{name}">
                <default key="_controller">AcmeHelloBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

        return $collection;

Le routage est constitué de deux parties principales : le ``pattern``, qui est
l'URL correspondante à cette route, et un tableau ``par défaut``, qui spécifie
le contrôleur qui devra être exécuté. La syntaxe pour le paramètre dans le 
pattern (``{name}``) est un joker. Cela signifie que ``hello/Jean``, ``hello/Bernard``
ou n'importe quelle URL similaire correspondra à cette route. Le paramètre ``{name}``
sera également passé à notre contrôleur afin que nous puissions utiliser la valeur
afin de saluer l'utilisateur.

.. note::

   Le système de routage dispose de beaucoup d'autres fonctionnalités
   qui vous permettront de créer des structures d'URL puissantes et flexibles
   dans votre application. Pour plus de détails, lisez le chapitre dédié
   aux routes :doc:`Routing </book/routing>`.  

Etape 2 : Créez le Contrôleur
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quand une URL comme ``/hello/Jean`` est traitée par l'application, la route
``hello`` est reconnue et le contrôleur ``AcmeHelloBundle:Hello/index``
est exécuté par le framework. L'étape suivante est de créer ce contrôleur.

Le contrôleur - ``AcmeHelloBundle:Hello:index`` est le nom *logique* du contrôleur,
et il est associé à la méthode ``indexAction`` d'une classe PHP appelée
``Acme\HelloBundle\Controller\Hello``. Commencez par créer ce fichier dans votre
``AcmeHelloBundle``.

En réalité, un contrôleur n'est rien d'autre qu'une méthode PHP que vous créez
et que Symfony exécute. C'est à cet endroit que le code propre à l'application
utilisera les informations de la requête afin de construire et préparer la 
ressource demandée par la requête. Excepté dans certaines situations avancées, 
le résultat final d'un contrôleur sera toujours le même :
un objet ``Response`` Symfony2.

Créez la méthode ``indexAction`` que Symfony exécutera lorsque la route ``hello``
sera identifiée::

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
        public function indexAction($name)
        {
            return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

Le contrôleur est simple : il crée un nouvel objet ``Response`` qui a pour 
premier argument le contenu qui doit être utilisé dans la réponse (une petite
page HTML dans notre cas).


Félicitations ! Après avoir n'avoir créé qu'une route et un contrôleur, vous
avez une page pleinement fonctionnelle ! Si vous avez tout effectué correctement,
votre application devrait vous saluer::

    http://localhost/app_dev.php/hello/Ryan

.. tip::

    Vous pouvez aussi voir votre application en :ref:`environnement<environments-summary>`
    de « prod » en allant à l'adresse :

    .. code-block:: text
	
        http://localhost/app.php/hello/Ryan
 
    Si vous avez une erreur, c'est certainement parce que vous avez besoin de vider
    votre cache grâce à la commande :

    .. code-block:: bash

        php app/console cache:clear --env=prod --no-debug

Une troisième étape optionelle dans ce processus est de créer un template.

.. note::

   Les contrôleurs sont le point central de votre code et un élément clé
   de la création de pages. Pour plus d'informations lisez le chapitre
   :doc:`Chapitre Contrôleurs </book/controller>`.

Etape 3 facultative : Créez le Template
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les templates vous permettent de déplacer toute la présentation (ex: code HTML)
dans un fichier séparé et de réutiliser différentes portions d'un layout.
A la place d'écrire le code HTML dans le contrôleur, retournez plutôt un template :

.. code-block:: php	
    :linenos:

	
    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
            return $this->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

            // render a PHP template instead
            // return $this->render('AcmeHelloBundle:Hello:index.html.php', array('name' => $name));
        }
    }

.. note::

   Afin d'utiliser la méthode ``render()``, votre contrôleur doit étendre la classe
   `Symfony\Bundle\FrameworkBundle\Controller\Controller`` (API 
   docs: :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`), qui
   ajoute des raccourcis pour des tâches fréquemment utilisées dans les contrôleurs.
   Dans l'exemple ci-dessus, c'est ce qui est fait en ajoutant la ligne ``use``
   et en étendant la classe ``Controller`` à la ligne 6.

La méthode ``render()`` crée un objet ``Response`` qui contient le contenu
d'un template rendu. Comme tout autre contrôleur, vous retournerez cet objet
``Response``.

Notez qu'il y a deux différents exemples afin de rendre un template.
Par défaut, Symfony2 supporte deux langages différents de templates :
les templates classiques PHP et les simples mais puissants templates ``Twig``.
Ne paniquez pas, vous êtes libres de choisir celui que vous désirez
voire les deux.

Le contrôleur rend le template ``AcmeHelloBundle:Hello:index.html.twig``,
qui suit la convention de nommage :

    **NomBundle**:**NomControleur**:**NomTemplate**

C'est le nom *logique* du template, qui est associé à une location physique selon
la convention suivante.

	
    **/path/to/BundleName**/Resources/views/**ControllerName**/**TemplateName**

Dans ce cas, ``AcmeHelloBundle`` est le nom de bundle, ``Hello`` est le
nom du contrôleur et enfin ``index.html.twig`` est le template :

.. configuration-block::

    .. code-block:: jinja
       :linenos:

        {# src/Acme/HelloBundle/Resources/views/Hello/index.html.twig #}
        {% extends '::base.html.twig' %}

        {% block body %}
            Hello {{ name }}!
        {% endblock %}

    .. code-block:: php

        <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
        <?php $view->extend('::base.html.php') ?>

        Hello <?php echo $view->escape($name) ?>!

Analysons maintenant le template Twig ligne par ligne :

* *Ligne 2* : Le symbole ``extends`` définit un template parent. Le template
  définit explicitement un fichier layout dans lequel il sera inséré.

* *Ligne 4* : Le symbole ``block`` indique que tout ce qui est à l'intérieur doit
  être placé à l'intérieur d'un bloc appelé ``body``. Comme vous le voyez, c'est
  en définitive la responsabilité du template parent (``base.html.twig``) de rendre
  le bloc ``body``.

Le nom de fichier du template parent, ``::base.html.twig``, est dispensé des portions
**NomBundle** et **NomControleur** (remarquez les deux points (``::``) au début). Ceci
signifie que le template se situe en dehors du bundle et dans le répertoire ``app``.

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block title %}Welcome!{% endblock %}</title>
                {% block stylesheets %}{% endblock %}
                <link rel="shortcut icon" href="{{ asset('favicon.ico') }}" />
            </head>
            <body>
                {% block body %}{% endblock %}
		{% block javascripts %}{% endblock %}
            </body>
        </html>

    .. code-block:: php

        <!-- app/Resources/views/base.html.php -->
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title><?php $view['slots']->output('title', 'Welcome!') ?></title>	
                <?php $view['slots']->output('stylesheets') ?>
                <link rel="shortcut icon" href="<?php echo $view['assets']->getUrl('favicon.ico') ?>" />
            </head>
            <body>
                <?php $view['slots']->output('_content') ?>
		<?php $view['slots']->output('stylesheets') ?>
            </body>
        </html>

Le fichier du template de base définit le layout HTML en rend le bloc ``body``
que vous avez défini dans le template ``index.html.twig``. Il rend également 
un bloc ``title``, que vous pouvez choisir de définir dans le template 
``index.html.twig``. Si vous ne définissez pas le bloc ``title`` dans le template
enfant, il aura pour valeur par défaut ``Welcome!``.

Les templates sont une façon puissante de rendre et d'organiser le contenu de 
votre page. Les templates peuvent tout rendre, des layouts HTML au codes CSS,
ou n'importe quoi d'autre que le contrôleur peut avoir besoin de retourner à l'utilisateur.

Dans le cycle de vie d'une requête, le template est un outil facultatif. Souvenez
vous que le but de chaque contrôleur est de renvoyer un objet ``Response``.
Le moteur de templates est un outil puissant, bien qu'optionnel, pour créer le
contenu de cet objet ``Response``.

.. index::
   single: Directory Structure

La structure des répertoires
----------------------------

Après seulement quelques courtes sections, vous comprenez déjà la philosophie
derrière la création et le rendu de pages dans Symfony2. Vous avez déjà commencé
à voir comment les projets Symfony2 sont structurés et organisés. A la fin de
cette section, vous saurez où trouver et où mettre les différents types de fichiers
et pourquoi.

Bien qu'entièrement flexible, chaque :term:`application` 
a par défaut la même structure de répertoires basique et recommandée :

* ``app/``: Ce répertoire contient la configuration de l'application;

* ``src/``: Tout le code PHP du projet est stocké ici;

* ``vendor/``: Par convention, toutes les librairies tierces (additionnelles) sont placées ici;

* ``web/``: Le répertoire racine web qui contient tous les fichiers publiquement accessibles;

Le répertoire Web
~~~~~~~~~~~~~~~~~

Le répertoire web contient tous les fichiers publics et statiques incluant les
images, les feuilles de style et les javascripts. 
Il contient également le :term:`front controller` (contrôleur frontal)::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

Le contrôleur frontal (``app.php`` dans cet exemple) est le fichier
exécuté lorsque l'on appelle une application Symfony2. Son rôle est d'utiliser une
classe Kernel, ``AppKernel``, pour initialiser l'application (bootstrap).

.. tip::
	   
   Avoir un contrôleur frontal signifie des URL différentes et plus flexibles
   que dans une application en pur php. 
   Lorsqu'on utilise un contrôleur frontal, les URLs sont formatées comme suit::

       http://localhost/app.php/hello/Ryan

   Le contrôleur frontal, ``app.php``, est exécuté et l'URL « interne: » ``/hello/Ryan``
   est traitée par l'application en se basant sur la configuration du routage. 
   En utilisant les règles du module Apache ``mod_rewrite``, 
   vous pouvez forcer le script ``app.php`` à être exécuté sans
   avoir besoin de le mentionner dans l'URL::

    http://localhost/hello/Ryan

Les contrôleurs frontaux sont essentiels pour traiter chaque requête. 
Cependant, vous aurez rarement besoin de les modifier ou même d'y penser. 
Nous en reparlerons dans la section `Environnements`_ .

Le répertoire de l'application (``app``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Comme nous l'avons vu dans le contrôleur frontal, la classe ``AppKernel`` est le
point d'entrée principal de l'application et est chargée de toute la configuration. 
De ce fait, elle est stockée dans le répertoire ``app/``.

Cette classe doit implémenter deux méthodes qui définissent tout ce dont Symfony
a besoin de savoir à propos de votre application. Vous n'avez pas à vous soucier
de ces méthodes en commençant - Symfony les complète pour vous avec des valeurs
par défaut.

*``registerBundles()``: renvoit un tableau de tous les bundles dont l'application
a besoin pour fonctionner (voir le :ref:`page-creation-bundles`);

*``registerContainerConfiguration()``: Charge le fichier de configuration ressources
   principal de l'application (voir la section `Configuration de l'Application`_)


Dans le développement au quotidien, vous utiliserez principalement le répertoire
``app/`` pour modifier les fichiers de configuration et de routage dans le répertoire
``app/config`` (voir `Configuration de l'Application`_). ``app/`` contient également
le répertoire de cache de l'application (``app/cache``), le répertoire des logs
(``app/logs``) et un répertoire pour les ressources communes à l'application
entière (``app/Resources``).
 
Nous en apprendrons plus sur ces répertoires dans de prochains chapitres.

.. _autoloading-introduction-sidebar:

.. sidebar:: Autoloading

Lorsque Symfony se charge, un fichier spécial - ``app/autoload.php`` - est
inclu. Ce fichier s'occupe de configurer l'autoloader qui chargera automatiquement
tous vos fichiers depuis le répertoire ``src/`` et toutes les librairies tierces
depuis le repertoire ``vendor/``.

Grace à l'autoloader, vous n'avez jamais à vous soucier d'utiliser les instructions
``include`` ou ``require``. Symfony2 se base sur l'espace de nom (namespace) d'une
classe pour déterminer son emplacement et l'inclure automatiquement le fichier à
votre place à l'instant où vous en avez besoin.
    

L'autoloader est déjà configuré pour regarder dans le dossier ``src/`` pour chacune
de vos classes PHP. Pour que le chargement automatique fonctionne, le nom de la
classe et le chemin du fichier doivent avoir une structure similaire :

    .. code-block:: text

        Nom de classe:
            Acme\HelloBundle\Controller\HelloController
        Chemin:
            src/Acme/HelloBundle/Controller/HelloController.php

Typiquement, le seul moment où vous devrez vous soucier du fichier ``app/autoload.php``	
est quand vous incluerez des librairies tierces dans le repertoire ``vendor/``.
Pour plus d'informations sur le chargement automatique, voir 
:doc: `Comment charger automatiquement des classes</cookbook/tools/autoloader>`.

Le répertoire des sources (``src/``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour faire simple, le répertoire ``src/`` contient tout le code (code PHP, templates,
fichiers de configuration, feuilles de style, etc) qui fait tourner *votre* application. 
En fait en développant, le plus gros du travail sera fait à l'intérieur d'un ou
plusieurs bundles que vous créerez dans ce répertoire.

Mais qu'est-ce qu'un :term:`bundle`?

.. _page-creation-bundles:

Le Système de Bundles
---------------------

Un bundle est similaire aux plugins que l'on peut trouver dans d'autres logiciels,
mais en mieux. La différence clé est que *tout* est un bundle dans Symfony2, ce qui
inclut le coeur du framework et le code de votre application.
Les bundles sont aux premières loges dans Symfony2. Ils vous offrent la flexibilité
d'utiliser des fonctionnalités pré-construites packagées dans des `bundles tiers`_
ou de distribuer vos propres bundles. Cela rend facile de sélectionner quelles
fonctionnalités activer dans votre application et de les optimiser comme vous voulez.

.. note::

   Alors que vous apprendrez les bases ici, une entrée du cookbook est entièrement
   dévouée à l'organisation et aux bonnes pratiques : :doc:`bundles</cookbook/bundles/best_practices>`.

Un bundle est simplement un ensemble structuré de fichiers au sein d'un répertoire
et qui implémentent une fonctionnalité unique. Vous pourrez ainsi créer un
``BlogBundle``, un ``ForumBundle`` ou un bundle pour la gestion des utilisateurs
(beaucoup de ces bundles existent déjà et sont open-source). Chaque répertoire
contient tout ce qui est lié à cette fonctionnalité incluant les fichiers PHP,
les templates, les feuilles de style, le javascript, les tests et tout le reste.
Chaque aspect d'une fonctionnalité se trouve dans le bundle, et chaque fonctionnalité
aussi.

Une application est faite de bundles qui sont définis dans la méthode ``registerBundles()``
de la classe ``AppKernel``::

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

Avec la méthode ``registerBundles()``, vous avez le contrôle totale des bundles
qui sont utilisés dans votre application (incluant les bundles du coeur de Symfony).

.. tip::

   A bundle can live *anywhere* as long as it can be autoloaded (via the
   autoloader configured at ``app/autoload.php``).

Creating a Bundle
~~~~~~~~~~~~~~~~~

The Symfony Standard Edition comes with a handy task that creates a fully-functional
bundle for you. Of course, creating a bundle by hand is pretty easy as well.

To show you how simple the bundle system is, create a new bundle called
``AcmeTestBundle`` and enable it.

.. tip::

    The ``Acme`` portion is just a dummy name that should be replaced by
    some "vendor" name that represents you or your organization (e.g. ``ABCTestBundle``
    for some company named ``ABC``).

Start by creating a ``src/Acme/TestBundle/`` directory and adding a new file
called ``AcmeTestBundle.php``::

    // src/Acme/TestBundle/AcmeTestBundle.php
    namespace Acme\TestBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeTestBundle extends Bundle
    {
    }

.. tip::

   The name ``AcmeTestBundle`` follows the standard :ref:`Bundle naming conventions<bundles-naming-conventions>`.
   You could also choose to shorten the name of the bundle to simply ``TestBundle``
   by naming this class ``TestBundle`` (and naming the file ``TestBundle.php``).

This empty class is the only piece you need to create the new bundle. Though
commonly empty, this class is powerful and can be used to customize the behavior
of the bundle.

Now that you've created the bundle, enable it via the ``AppKernel`` class::

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
generating a basic bundle skeleton:

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/TestBundle

The bundle skeleton generates with a basic controller, template and routing
resource that can be customized. You'll learn more about Symfony2's command-line
tools later.

.. tip::

   Whenever creating a new bundle or using a third-party bundle, always make
   sure the bundle has been enabled in ``registerBundles()``. When using
   the ``generate:bundle`` command, this is done for you.

Bundle Directory Structure
~~~~~~~~~~~~~~~~~~~~~~~~~~

The directory structure of a bundle is simple and flexible. By default, the
bundle system follows a set of conventions that help to keep code consistent
between all Symfony2 bundles. Take a look at ``AcmeHelloBundle``, as it contains
some of the most common elements of a bundle:

* ``Controller/`` contains the controllers of the bundle (e.g. ``HelloController.php``);

* ``Resources/config/`` houses configuration, including routing configuration
  (e.g. ``routing.yml``);

* ``Resources/views/`` holds templates organized by controller name (e.g.
  ``Hello/index.html.twig``);

* ``Resources/public/`` contains web assets (images, stylesheets, etc) and is
  copied or symbolically linked into the project ``web/`` directory via
  the ``assets:install`` console command;

* ``Tests/`` holds all tests for the bundle.

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

        # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <import resource="parameters.ini" />
            <import resource="security.yml" />
        </imports>
        
        <framework:config charset="UTF-8" secret="%secret%">
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
            <framework:form />
            <framework:csrf-protection />
            <framework:validation annotations="true" />
            <framework:templating assets-version="SomeVersionScheme">
                <framework:engine id="twig" />
            </framework:templating>
            <framework:session default-locale="%locale%" auto-start="true" />
        </framework:config>

        <!-- Twig Configuration -->
        <twig:config debug="%kernel.debug%" strict-variables="%kernel.debug%" />

        <!-- ... -->

    .. code-block:: php

        $this->import('parameters.ini');
        $this->import('security.yml');

        $container->loadFromExtension('framework', array(
            'secret'          => '%secret%',
            'charset'         => 'UTF-8',
            'router'          => array('resource' => '%kernel.root_dir%/config/routing.php'),
            'form'            => array(),
            'csrf-protection' => array(),
            'validation'      => array('annotations' => true),
            'templating'      => array(
                'engines' => array('twig'),
                #'assets_version' => "SomeVersionScheme",
            ),
            'session' => array(
                'default_locale' => "%locale%",
                'auto_start'     => true,
            ),
        ));

        // Twig Configuration
        $container->loadFromExtension('twig', array(
            'debug'            => '%kernel.debug%',
            'strict_variables' => '%kernel.debug%',
        ));

        // ...

.. note::

   You'll learn exactly how to load each file/format in the next section
   `Environments`_.

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
share the same PHP code (apart from the front controller), but use different
configuration. For instance, a ``dev`` environment will log warnings and
errors, while a ``prod`` environment will only log errors. Some files are
rebuilt on each request in the ``dev`` environment (for the developer's convenience),
but cached in the ``prod`` environment. All environments live together on
the same machine and execute the same application.

A Symfony2 project generally begins with three environments (``dev``, ``test``
and ``prod``), though creating new environments is easy. You can view your
application in different environments simply by changing the front controller
in your browser. To see the application in the ``dev`` environment, access
the application via the development front controller:

.. code-block:: text

    http://localhost/app_dev.php/hello/Ryan

If you'd like to see how your application will behave in the production environment,
call the ``prod`` front controller instead:

.. code-block:: text

    http://localhost/app.php/hello/Ryan


Since the ``prod`` environment is optimized for speed; the configuration,	
routing and Twig templates are compiled into flat PHP classes and cached.
When viewing changes in the ``prod`` environment, you'll need to clear these
cached files and allow them to rebuild::

    php app/console cache:clear --env=prod --no-debug


.. note::

   If you open the ``web/app.php`` file, you'll find that it's configured explicitly
   to use the ``prod`` environment::

       $kernel = new AppKernel('prod', false);

   You can create a new front controller for a new environment by copying
   this file and changing ``prod`` to some other value.

.. note::

    The ``test`` environment is used when running automated tests and cannot
    be accessed directly through the browser. See the :doc:`testing chapter</book/testing>`
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

You already know that the ``.yml`` extension can be changed to ``.xml`` or
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

        # ...

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <imports>
            <import resource="config.xml" />
        </imports>

        <framework:config>
            <framework:router resource="%kernel.root_dir%/config/routing_dev.xml" />
            <framework:profiler only-exceptions="false" />
        </framework:config>

        <!-- ... -->

    .. code-block:: php

        // app/config/config_dev.php
        $loader->import('config.php');

        $container->loadFromExtension('framework', array(
            'router'   => array('resource' => '%kernel.root_dir%/config/routing_dev.php'),
            'profiler' => array('only-exceptions' => false),
        ));

        // ...

The ``imports`` key is similar to a PHP ``include`` statement and guarantees
that the main configuration file (``config.yml``) is loaded first. The rest
of the file tweaks the default configuration for increased logging and other
settings conducive to a development environment.

Both the ``prod`` and ``test`` environments follow the same model: each environment
imports the base configuration file and then modifies its configuration values
to fit the needs of the specific environment. This is just a convention,
but one that allows you to reuse most of your configuration and customize
just pieces of it between environments.

Summary
-------

Congratulations! You've now seen every fundamental aspect of Symfony2 and have
hopefully discovered how easy and flexible it can be. And while there are
*a lot* of features still to come, be sure to keep the following basic points
in mind:

* creating a page is a three-step process involving a **route**, a **controller**
  and (optionally) a **template**.

* each project contains just a few main directories: ``web/`` (web assets and
  the front controllers), ``app/`` (configuration), ``src/`` (your bundles),
  and ``vendor/`` (third-party code) (there's also a ``bin/`` directory that's
  used to help updated vendor libraries);

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

.. _`Twig`: http://twig.sensiolabs.org
.. _`bundles tiers`: http://symfony2bundles.org/
.. _`Symfony Standard Edition`: http://symfony.com/download
