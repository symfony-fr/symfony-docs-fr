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

Cette approche très simple est excellente, car elle correspond à la façon dont 
fonctionne le Web. Chaque interaction sur le Web est initiée par une requête
HTTP. Le but de votre application est simplement d'interpréter cette requête 
et de lui retourner une Response HTTP appropriée.

Symfony2 suit cette philosophie et vous fournit des outils et conventions pour
garder votre application organisée tout en pouvant devenir plus complexe et fréquentée.

Cela vous parait suffisamment simple ? Alors, allons-y !

.. index::
   single: Page creation; Example

La page « Hello Symfony ! »
---------------------------

Commençons avec une application traditionnelle « Hello World ! ». Quand nous
aurons terminé, l'utilisateur sera capable de recevoir un message de
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
   Pour des instructions détaillées à ce sujet, lisez la documentation du serveur
   web que vous utilisez.
   Voici les documentations de serveur que vous utilisez probablement :

    * Pour Apache HTTP Server, allez voir la `documentation Apache's DirectoryIndex`_.
    * Pour Nginx, allez voir `documentation Nginx HttpCoreModule`_.


Avant de commencer : Créez un Bundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Avant de commencer, vous devez créer un *bundle*. Dans Symfony2, un :term:`bundle`
est comme un plugin, excepté le fait que tout le code de votre application
siégera dans un bundle.

Un bundle n'est rien d'autre qu'un répertoire qui contient tout ce qui est relatif
à une fonctionnalité spécifique, ce qui inclut les classes PHP, la configuration
et même les feuilles de style et le javascript (voir :ref:`page-creation-bundles`).

Afin de créer un bundle nommé ``AcmeHelloBundle`` (un bundle fictif que vous 
créerez dans ce chapitre), lancez la commande suivante et suivez les instructions
affichées à l'écran (choisissez les options par défaut) :

.. code-block:: bash
	
    $ php app/console generate:bundle --namespace=Acme/HelloBundle --format=yml

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
         acme_hello:
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

Ce code est très basique : il dit à Symfony de charger la configuration de routage
depuis le fichier ``Resources/config/routing.yml`` qui se trouve dans le ``AcmeHelloBundle``.
Cela signifie que vous pouvez placer votre configuration de routage directement
dans le fichier ``app/config/routing.yml`` ou organiser vos routes dans votre
application et les importer depuis ce fichier.

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
pattern (``{name}``) est un joker. Cela signifie que ``hello/Ryan``, ``hello/Bernard``
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

Quand une URL comme ``/hello/Ryan`` est traitée par l'application, la route
``hello`` est reconnue et le contrôleur ``AcmeHelloBundle:Hello/index``
est exécuté par le framework. L'étape suivante est de créer ce contrôleur.

Le contrôleur - ``AcmeHelloBundle:Hello:index`` est le nom *logique* du contrôleur,
et il est associé à la méthode ``indexAction`` d'une classe PHP appelée
``Acme\HelloBundle\Controller\HelloController``. Commencez par créer ce fichier dans votre
``AcmeHelloBundle`` :

.. code-block:: php

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    class HelloController
    {
    }

En réalité, un contrôleur n'est rien d'autre qu'une méthode PHP que vous créez
et que Symfony exécute. C'est à cet endroit que le code propre à l'application
utilisera les informations de la requête afin de construire et préparer la 
ressource demandée par la requête. Excepté dans certaines situations avancées, 
le résultat final d'un contrôleur sera toujours le même :
un objet ``Response`` Symfony2.

Créez la méthode ``indexAction`` que Symfony exécutera lorsque la route ``hello``
sera identifiée :

.. code-block:: php

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

        $ php app/console cache:clear --env=prod --no-debug

Une troisième étape optionnelle dans ce processus est de créer un template.

.. note::

   Les contrôleurs sont le point central de votre code et un élément clé
   de la création de pages. Pour plus d'informations lisez le chapitre
   :doc:`Chapitre Contrôleurs </book/controller>`.

Étape 3 facultative : Créez le Template
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

   Afin d'utiliser la méthode
   :method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::render`,
   votre contrôleur doit étendre la classe
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
les templates classiques PHP et les simples, mais puissants templates ``Twig``.
Ne paniquez pas, vous êtes libres de choisir celui que vous désirez
voire les deux.

Le contrôleur rend le template ``AcmeHelloBundle:Hello:index.html.twig``,
qui suit la convention de nommage :

    **NomBundle**:**NomContrôleur**:**NomTemplate**

C'est le nom *logique* du template, qui est associé à une location physique selon
la convention suivante.

    **/chemin/vers/NomBundle**/Resources/views/**NomContrôleur**/**NomTemplate**

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
**NomBundle** et **NomContrôleur** (remarquez les deux points (``::``) au début). Ceci
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
                <?php $view['slots']->output('javascripts') ?>
            </body>
        </html>

Le fichier du template de base définit le layout HTML en rend le bloc ``body``
que vous avez défini dans le template ``index.html.twig``. Il rend également 
un bloc ``title``, que vous pouvez choisir de définir dans le template 
``index.html.twig``. Si vous ne définissez pas le bloc ``title`` dans le template
enfant, il aura pour valeur par défaut ``Welcome!``.

Les templates sont une façon puissante de rendre et d'organiser le contenu de 
votre page. Les templates peuvent tout rendre, des layouts HTML au code CSS,
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

* ``vendor/``: Par convention, toutes les bibliothèques tierces (additionnelles) sont placées ici;

* ``web/``: Le répertoire racine web qui contient tous les fichiers publiquement accessibles;

Le répertoire Web
~~~~~~~~~~~~~~~~~

Le répertoire web contient tous les fichiers publics et statiques incluant les
images, les feuilles de style et les javascripts. 
Il contient également le « :term:`contrôleur frontal` » (« front controller » en
anglais) :

.. code-block:: php

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
   Lorsqu'on utilise un contrôleur frontal, les URLs sont formatées comme suit :

    .. code-block:: text

       http://localhost/app.php/hello/Ryan

   Le contrôleur frontal, ``app.php``, est exécuté et l'URL « interne: » ``/hello/Ryan``
   est traitée par l'application en se basant sur la configuration du routage. 
   En utilisant les règles du module Apache ``mod_rewrite``, 
   vous pouvez forcer le script ``app.php`` à être exécuté sans
   avoir besoin de le mentionner dans l'URL::

    .. code-block:: text

    http://localhost/hello/Ryan

Les contrôleurs frontaux sont essentiels pour traiter chaque requête. 
Cependant, vous aurez rarement besoin de les modifier ou même d'y penser. 
Ils seront abordés dans la section `Environnements`_ .

Le répertoire de l'application (``app``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Comme nous l'avons vu dans le contrôleur frontal, la classe ``AppKernel`` est le
point d'entrée principal de l'application et est chargée de toute la configuration. 
De ce fait, elle est stockée dans le répertoire ``app/``.

Cette classe doit implémenter deux méthodes qui définissent tout ce dont Symfony
a besoin de savoir à propos de votre application. Vous n'avez pas à vous soucier
de ces méthodes en commençant - Symfony les complète pour vous avec des valeurs
par défaut.

* ``registerBundles()``: renvoie un tableau de tous les bundles dont l'application
  a besoin pour fonctionner (voir le :ref:`page-creation-bundles`) ;

* ``registerContainerConfiguration()``: Charge le fichier de configuration ressources
   principal de l'application (voir la section `Configuration de l'Application`_).


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
    inclus. Ce fichier s'occupe de configurer l'autoloader qui chargera automatiquement
    tous vos fichiers depuis le répertoire ``src/`` et toutes les bibliothèques tierces
    depuis le répertoire ``vendor/``.

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
    est quand vous inclurez des bibliothèques tierces dans le repertoire ``vendor/``.
    Pour plus d'informations sur le chargement automatique, voir 
    :doc: `Comment charger automatiquement des classes</components/class_loader>`.

Le répertoire des sources (``src/``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
d'utiliser des fonctionnalités préconstruites packagées dans des `bundles tiers`_
ou de distribuer vos propres bundles. Cela rend facile de sélectionner quelles
fonctionnalités activer dans votre application et de les optimiser comme vous voulez.

.. note::

   Alors que vous apprendrez les bases ici, une entrée du cookbook est entièrement
   dédiée à l'organisation et aux bonnes pratiques : :doc:`bundles</cookbook/bundles/best_practices>`.

Un bundle est simplement un ensemble structuré de fichiers au sein d'un répertoire
et qui implémentent une fonctionnalité unique. Vous pourrez ainsi créer un
``BlogBundle``, un ``ForumBundle`` ou un bundle pour la gestion des utilisateurs
(beaucoup de ces bundles existent déjà et sont open-source). Chaque répertoire
contient tout ce qui est lié à cette fonctionnalité incluant les fichiers PHP,
les templates, les feuilles de style, le javascript, les tests et tout le reste.
Chaque aspect d'une fonctionnalité se trouve dans le bundle, et chaque fonctionnalité
aussi.

Une application est faite de bundles qui sont définis dans la méthode ``registerBundles()``
de la classe ``AppKernel`` :

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

Avec la méthode ``registerBundles()``, vous avez le contrôle total des bundles
qui sont utilisés dans votre application (incluant les bundles du coeur de Symfony).

.. tip::
   Un bundle peut se trouver *n'importe où* pour peu qu'il puisse être chargé (via
   l'autoloader configuré à ``app/autoload.php``).

Créer un Bundle
~~~~~~~~~~~~~~~

La Symfony Standard Edition est fournie avec une tâche qui crée un bundle
totalement fonctionnel pour vous. Bien sûr, vous pouvez tout aussi facilement créer un
bundle à la main.

Pour vous montrer à quel point le système de bundle est simple, créons un nouveau
bundle appelé ``AcmeTestBundle`` et activons-le.

.. tip::

    La partie ``Acme`` est juste un nom idiot qui peut être remplacé par un autre
    nom qui vous représente ou votre entreprise (ex ``ABCTestBundle`` pour une
    entreprise nommée ``ABC``).

Commencez par créer un répertoire ``src/Acme/TestBundle/`` et ajoutez y un nouveau
fichier appelé ``AcmeTestBundle.php`` :

.. code-block:: php

    // src/Acme/TestBundle/AcmeTestBundle.php
    namespace Acme\TestBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeTestBundle extends Bundle
    {
    }

.. tip::

   Le nom ``AcmeTestBundle`` suit les :ref:`conventions de nommage des bundles<bundles-naming-conventions>`.
   Vous pourriez aussi choisir de raccourcir le nom du bundle pour ``TestBundle``
   en nommant sa classe ``TestBundle`` (et en appelant le fichier ``TestBundle.php``).

Cette classe vide est la seule pièce dont vous avez besoin afin de créer un nouveau
bundle. Bien que souvent vide, cette classe est très puissante et peut être utilisée
pour personnaliser le comportement du bundle.

Maintenant que vous avez créé le bundle, activez-le via la classe ``AppKernel`` :

.. code-block:: php

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

Et même s'il ne fait encore rien de spécial, ``AcmeTestBundle`` est prêt à être utilisé.

Et bien que ce soit très facile, Symfony fournit également une commande qui génère
un squelette de bundle de base :

.. code-block:: bash

    $ php app/console generate:bundle --namespace=Acme/TestBundle

Le squelette du bundle contient un contrôleur de base, un template et une configuration
de routage qui peuvent être personnalisés. Vous en apprendrez plus sur les commandes
Symfony2 plus tard.

.. tip::
   Peu importe que vous créiez un bundle ou que vous utilisiez un bundle tiers, 
   assurez-vous toujours qu'il soit activé dans ``registerBundles()``. Si vous 
   utilisez la commande ``generate:bundle``, c'est fait automatiquement pour vous.

Structure des répertoires des bundles
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La structure des répertoires d'un bundle est simple et flexible. Par défaut le système
de bundle suit un ensemble de conventions qui aident à garder le code homogène
entre tous les bundles Symfony2. Jetez un oeil au ``AcmeHelloBundle``, car il contient
certains des éléments les plus communs d'un bundle :

* ``Controller/`` contient les contrôleurs du bundle (ex ``HelloController.php``);

* ``DependencyInjection/``  contient certaines classes d'extension d'injection
  de dépendances, qui peuvent importer des configurations de services, enregistrer
  des passes de compilation ou plus encore (ce répertoire n'est pas obligatoire);

* ``Resources/config/`` contient la configuration, notamment la configuration
  de routage (ex ``routing.yml``);

* ``Resources/views/`` contient les templates organisés par nom de contrôleur (ex
  ``Hello/index.html.twig``);

* ``Resources/public/`` contient les ressources web (images, feuilles de style, etc) 
  et sont copiées ou liées par un lien symbolique dans le répertoire de projet ``web/``
  grâce à la commande ``assets:install``;

* ``Tests/`` contient tous les tests du bundle.

Un bundle peut être très petit ou très grand selon la fonctionnalité qu'il implémente.
Il contient seulement les fichiers dont vous avez besoin et rien d'autre.

En parcourant le Book, vous apprendrez comment persister des objets en base de données,
créer et valider des formulaires, créer des traductions pour votre application,
écrire des tests et bien plus encore. Chacun de ces aspects a sa propre place et
son propre rôle au sein d'un bundle.

Configuration de l'Application
------------------------------

Une application consiste en un ensemble de bundles qui représente toutes les
fonctionnalités et capacités de votre application. Chaque bundle peut être
personnalisé via les fichiers de configuration écrits en YAML, XML ou PHP. Par
défaut, la configuration principale se situe dans le répertoire ``app/config/`` 
et se trouve dans un fichier appelé ``config.yml``, ``config.xml`` ou ``config.php``
selon le format que vous préférez :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: parameters.ini }
            - { resource: security.yml }
        
        framework:
            secret:          "%secret%"
            router:          { resource: "%kernel.root_dir%/config/routing.yml" }
            #...

        # Twig Configuration
        twig:
            debug:            "%kernel.debug%"
            strict_variables: "%kernel.debug%"

        # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <import resource="parameters.ini" />
            <import resource="security.yml" />
        </imports>
        
        <framework:config secret="%secret%">
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
            <!-- ... -->
        </framework:config>

        <!-- Twig Configuration -->
        <twig:config debug="%kernel.debug%" strict-variables="%kernel.debug%" />

        <!-- ... -->

    .. code-block:: php

        $this->import('parameters.ini');
        $this->import('security.yml');

        $container->loadFromExtension('framework', array(
            'secret'          => '%secret%',
            'router'          => array('resource' => '%kernel.root_dir%/config/routing.php'),
            // ...
        ));

        // Twig Configuration
        $container->loadFromExtension('twig', array(
            'debug'            => '%kernel.debug%',
            'strict_variables' => '%kernel.debug%',
        ));

        // ...

.. note::
   Vous apprendrez exactement comment charger chaque fichier/format dans la prochaine
   section `Environnements`_.

Chaque entrée de niveau zéro comme ``framework`` ou ``twig`` définit la configuration
d'un bundle particulier. Par exemple, la clé ``framework`` définit la configuration
du bundle du noyau de Symfony ``FrameworkBundle`` et inclut la configuration pour le
routage, les templates et d'autres fonctionnalités du noyau.

Pour le moment, ne vous inquiétez pas des options de configuration spécifiques à
chaque section. Le fichier de configuration a des valeurs par défaut optimisées.
Au fur et à mesure que vous lirez et explorerez chaque recoin de Symfony2, vous
en apprendrez plus sur les options de configuration spécifiques à chaque fonctionnalité.

.. sidebar:: Formats de Configuration

    A travers les chapitres, tous les exemples de configuration seront montrés
    dans les trois formats (YAML, XML and PHP). Chacun a ses avantages et ses 
    inconvénients. Le choix vous appartient :

    * *YAML*: Simple, propre et lisible;

    * *XML*: Plus puissant que YAML parfois et support de l'autocomplétion sur les IDE;

    * *PHP*: Très puissant, mais moins lisible que les formats de configuration standards.


Dump de configuration par défaut
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

. versionadded:: 2.1
    La commande ``config:dump-reference`` a été ajoutée dans la version 2.1 de Symfony

Vous pouvez dumper la configuration par défaut d'un bundle en yaml vers la console
en utilisant la commande ``config:dump-reference``. Voici un exemple de dump de la
configuration du FrameworkBundle :

.. code-block:: text

    app/console config:dump-reference FrameworkBundle


L'alias de l'extension (clé de configuration) peut aussi être utilisé :

.. code-block:: text

    app/console config:dump-reference framework

.. note::

    Lisez l'article du Cookbook : :doc:`Comment exposer une configuration sémantique
    pour un Bundle </cookbook/bundles/extension>` pour avoir des informations sur
    l'ajout de configuration dans votre bundle.

.. index::
   single: Environments; Introduction

.. _environments-summary:

Environnements
--------------

Une application peut tourner sous différents environnements. Les différents 
environnements partagent le même code PHP (exepté celui du contrôleur frontal),
mais utilisent une configuration différente. Par exemple, l'environnement de ``dev``
enregistrera les erreurs et les warnings dans les logs, tandis que l'environnement
de ``prod`` enregistrera seulement les erreurs. Certains fichiers sont reconstruits
à chaque requête en environnement de ``dev`` (pour rendre le développement plus pratique),
mais sont mis en cache en environnement de ``prod``. Tous les environnements peuvent
tourner ensemble sur la même machine et exécutent la même application.

Un projet Symfony2 commence en général avec 3 environnements (``dev``, ``test``
et ``prod``), la création d'un nouvel environnement étant très facile. Vous pouvez
voir l'application sous différents environnements en changeant simplement le contrôleur
frontal dans votre navigateur. Pour voir l'application en environnement de ``dev``, 
accédez à l'application via le contrôleur frontal de développement :

.. code-block:: text

    http://localhost/app_dev.php/hello/Ryan

Si vous désirez voir comment votre application se comporterait en environnement
de production, appelez le contrôleur frontal de ``prod`` :

.. code-block:: text

    http://localhost/app.php/hello/Ryan


Puisque l'environnement de ``prod`` est optimisé pour la vitesse; la configuration,
les routes et les templates Twig sont compilés en classes PHP et cachés.
Quand vous voudrez voir des changements en environnement de ``prod``, vous aurez
besoin de nettoyer ces fichiers cachés afin de permettre leur régénération :

.. code-block:: bash

    $ php app/console cache:clear --env=prod --no-debug

.. note::

   Si vous ouvrez le fichier ``web/app.php``, vous trouverez ce qui est explicitement
   configuré en environnement de ``prod`` :
   
   .. code-block:: php

       $kernel = new AppKernel('prod', false);

   Vous pouvez créer un nouveau contrôleur frontal pour un nouvel environnement
   en copiant ce fichier et en changeant ``prod`` par une autre valeur.

.. note::

    L'environnement de  ``test`` est utilisé pour lancer des tests automatiques et
    n'est pas accessible directement dans un navigateur. Lisez le chapitre :doc:`tests</book/testing>`
    pour plus de détails.

.. index::
   single: Environnements; Configuration

Configuration d'Environnement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La classe ``AppKernel`` est responsable du chargement du fichier de configuration
que vous avez choisi :

.. code-block:: php

    // app/AppKernel.php
    public function registerContainerConfiguration(LoaderInterface $loader)
    {
        $loader->load(__DIR__.'/config/config_'.$this->getEnvironment().'.yml');
    }

Vous savez déjà que l'extension ``.yml`` peut être changée en ``.xml`` ou
``.php`` si vous préférez utiliser le format XML ou PHP pour écrire votre configuration.
Notez également que chaque environnement charge sa propre configuration. Considérez
le fichier de configuration pour l'environnement de ``dev``.

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

La clé ``imports`` est similaire à l'instruction PHP ``include`` et garantit
que le fichier de configuration principal (``config.yml``) est chargé en premier.
Le reste du fichier modifie la configuration par défaut pour une augmentation
des logs et d'autres paramètres relatifs à un environnement de développement.

Les environnements de ``prod`` et de ``test`` suivent le même modèle : chaque
environnement importe le fichier de configuration de base et modifie ses valeurs
pour s'adapter aux besoins spécifiques à l'environnement. C'est juste une convention,
mais elle vous permet de réutiliser la plupart de la configuration et d'en modifier une
partie en fonction de l'environnement.

Résumé
------

Félicitations ! Vous avez maintenant eu un aperçu de chaque aspect fondamental de 
Symfony2 et avez découvert sa facilité et sa flexibilité. Et même s'il y a encore
*beaucoup* de fonctionnalité à découvrir, gardez les principes de base suivants
en tête :

* créer une page est un processus en trois étapes impliquant une **route**, un **contrôleur**
  et (optionnellement) un **template**.

* chaque projet contient juste quelques répertoires principaux : ``web/`` (ressources
  web et contrôleurs frontaux), ``app/`` (configuration), ``src/`` (vos bundles),
  et ``vendor/`` (bibliothèques tierces) (il y a aussi un répertoire ``bin/`` qui est utilisé
  pour la mise à jour des bibliothèques vendors);

* chaque fonctionnalité de Symfony2 (incluant le noyau du framework) est organisée
  dans un *bundle*, qui est un ensemble structuré de fichiers pour cette fonctionnalité;

* la **configuration** de chaque bundle se trouve dans le répertoire ``Ressources/config``
  du bundle et peut être écrite en YAML, XML ou PHP;

* La **configuration globale de l'application** se trouve dans le répertoire ``app/config``

* chaque **environnement** est accessible via des contrôleurs frontaux différents
  (ex ``app.php`` et ``app_dev.php``) et charge un fichier de configuration différent.

A partir de maintenant, chaque chapitre vous introduira de plus en plus d'outils
puissants et de concepts avancés. Plus vous connaîtrez Symfony2, plus vous apprécierez
la flexibilité de son architecture et le pouvoir qu'il vous donne pour développer
rapidement des applications.

.. _`Twig`: http://twig.sensiolabs.org
.. _`bundles tiers`: http://knpbundles.com
.. _`Symfony Standard Edition`: http://symfony.com/download
.. _`documentation Apache's DirectoryIndex`: http://httpd.apache.org/docs/2.0/mod/mod_dir.html
.. _`documentation Nginx HttpCoreModule`: http://wiki.nginx.org/HttpCoreModule#location