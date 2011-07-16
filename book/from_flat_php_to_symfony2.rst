Symfony2 comparé à du PHP simple
================================

** Pourquoi est-ce que Symfony2 est mieux que juste simplement ouvrir un fichier 
et écrire du PHP simple?

Si vous n'avez jamais utilisé un framework PHP, que vous n'êtes pas familier avec 
la philosophie MVC, ou simplement que vous vous demander pourquoi il y a tant de
*hype* autours de Symfony2, ce chapitre est pour vous. au lieu de vous *dire* que
Symfony2 vous permet de déveloper plus rapidement et de meilleurs applications qu'avec 
du PHP simple, vous allez le voir de vos propres yeux.

Dans ce chapitre, vous aller écrire une application simple en PHP simple, puis refactoriser
le code afin d'en améliorer l'organisation. You voyagerez dans le temps, en comprenant
les décisions qui ont fait évoluer le développement web au cours de ces dernières années.

D'ici la fin, vous verrez comment Symfony peut vous épargner les tâches banales et vous
permettra de reprendre le contrôle de votre code.

Un simple blog en PHP
---------------------

Dans ce chapitre, vous batirez une application de blog en utilisant uniquement du 
code PHP simple.
Pour commencer, créer une page qui affiche les articles du blog qu ont été sauvegardés
dans la base de données. Écrire en PHP simple est "rapide et sale":

.. code-block:: html+php

    <?php
    // index.php

    $link = mysql_connect('localhost', 'myuser', 'mypassword');
    mysql_select_db('blog_db', $link);

    $result = mysql_query('SELECT id, title FROM post', $link);
    ?>

    <html>
        <head>
            <title>List of Posts</title>
        </head>
        <body>
            <h1>List of Posts</h1>
            <ul>
                <?php while ($row = mysql_fetch_assoc($result)): ?>
                <li>
                    <a href="/show.php?id=<?php echo $row['id'] ?>">
                        <?php echo $row['title'] ?>
                    </a>
                </li>
                <?php endwhile; ?>
            </ul>
        </body>
    </html>

    <?php
    mysql_close($link);

C'est rapide à écrire, rapide à exécuter et, comme votre application évoluera, 
impossible à maintenir. IL y a plusieurs problèmes qui doivent être résolus:

* **aucune gestion d'erreur**: Que se passe-t-il si la connection à la base de 
données échoue?

* **mauvaise organisation**: si votre application évolue, ce fichier deviendra de 
moins en moins maintenable. Où devriez vous mettre le code qui traite la soumission
d'un formulaire? Comment pouvez-vous valider les données? Où devriez-vous placer
le code qui envoie des courriels?

* **Difficulté de réutiliser du code**: puisque tout se trouve dans un seul 
fichier, il n'y a pas de moyen de réutiliser des parties de l'application pour 
d'autres pages du blog.

.. note::
    Un autre problème non mentionné ici est le fait que la base de données est 
    liée à MySQL. Même si le sujet n'est pas couvert ici, Symfony intègre 
.. note::
    Another problem not mentioned here is the fact that the database is `Doctrine`_,
    une librairie dédiée à l'abstraction des base de données 
    et aux mapping objet-relationnel.
    
Retroussons-nous les manches et résolvons ces problèmes ainsi que d'autres.

Isoler la présentation
~~~~~~~~~~~~~~~~~~~~~~

Le code sera mieux structuré en séparant la logique d'application du code qui s'occupe
de la présentation HTML:

.. code-block:: html+php

    <?php
    // index.php

    $link = mysql_connect('localhost', 'myuser', 'mypassword');
    mysql_select_db('blog_db', $link);

    $result = mysql_query('SELECT id, title FROM post', $link);

    $posts = array();
    while ($row = mysql_fetch_assoc($result)) {
        $posts[] = $row;
    }

    mysql_close($link);

    // inclure le code de la présentation HTML
    require 'templates/list.php';

Le code HTML est maintenant dans un fichier séparé (``templates/list.php``), 
qui est essentiellement un fichier HTML qui utilise une syntaxe PHP de template:

.. code-block:: html+php

    <html>
        <head>
            <title>List of Posts</title>
        </head>
        <body>
            <h1>List of Posts</h1>
            <ul>
                <?php foreach ($posts as $post): ?>
                <li>
                    <a href="/read?id=<?php echo $post['id'] ?>">
                        <?php echo $post['title'] ?>
                    </a>
                </li>
                <?php endforeach; ?>
            </ul>
        </body>
    </html>

Par convention, le fichier qui contient la logique d'applicaiton - ``index.php`` 
est appelé "controller". Le terme :term:`controller` est u mot que vous allez 
entendre souvent, quel que soit le langage ou le framework utilisé. Il fait
simplement référence à *votre* code qui traite les entrées de l'utilisateur
et prépare une réponse.

Dans notre cas, le controlleur prépare les données de la base de données et ensuite
inclut un template pour présenter ces données. Comme le controleur est isolé, 
vous pouvez facilement changer *uniquement* le fichier de template si vous désirez
afficher les articles du blog dans un autre format (par exemple ``list.json.php`` 
pour un format JSON).

Isoler la logique d'affaire
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour l'instant, l'application ne contient qu'une seule page. Mais que faire 
si une deuxième page a besoin d'utiliser la même connection à la base de données,
or même le même tableau d'articles du blog? Refactoriser le code pour les le comportement
principal et les fonctions d'accès qux données de l'application sont isolées dans un nouveau
fichier appelé ``model.php``:

.. code-block:: html+php

    <?php
    // model.php

    function open_database_connection()
    {
        $link = mysql_connect('localhost', 'myuser', 'mypassword');
        mysql_select_db('blog_db', $link);

        return $link;
    }

    function close_database_connection($link)
    {
        mysql_close($link);
    }

    function get_all_posts()
    {
        $link = open_database_connection();

        $result = mysql_query('SELECT id, title FROM post', $link);
        $posts = array();
        while ($row = mysql_fetch_assoc($result)) {
            $posts[] = $row;
        }
        close_database_connection($link);

        return $posts;
    }

.. tip::

   Le nom du fichier ``model.php`` est utilisé car la logique et l'accès aux données
   de l'application est traditionnellement connu sous le nom de couche "modèle".
   Dans une applicatio bien structurée, la majorité du code représentant la logique d'affaire
   devrait résider dans le modèle (plutôt que dans le controlleur). Et contrairement 
   à cette exemple, seulement une portion (or aucune portion) du modèle est en fait responsable
   d'accéder à la base de données.

Le controlleur (``index.php``) est maintenant très simple:

.. code-block:: html+php

    <?php
    require_once 'model.php';

    $posts = get_all_posts();

    require 'templates/list.php';

Maintenant, la seule responsabilité du controlleur est de récupérer les données
de la couche modèle de l'application (le modèle) et d'appeler le template to afficher
les données.
C'est un exemple très simple du patron de conception Modèle-Vue-Controlleur.

Isoler le layout
~~~~~~~~~~~~~~~~

À ce point-ci, l'application a été refactorisée en trois parties distinctes, offrant
plusieurs avantages et l'opportunité de réutiliser pratiquement la totalité du code
pour d'autres pages.
La seule partie du code qui *ne peut pas* être réutiliser est le layout de la page.
Corrigez cela en créant un nouveau fichier ``layout.php``:

.. code-block:: html+php

    <!-- templates/layout.php -->
    <html>
        <head>
            <title><?php echo $title ?></title>
        </head>
        <body>
            <?php echo $content ?>
        </body>
    </html>

Le template (``templates/list.php``) peut maintenant simplement "hériter" 
du layout:

.. code-block:: html+php

    <?php $title = 'List of Posts' ?>

    <?php ob_start() ?>
        <h1>List of Posts</h1>
        <ul>
            <?php foreach ($posts as $post): ?>
            <li>
                <a href="/read?id=<?php echo $post['id'] ?>">
                    <?php echo $post['title'] ?>
                </a>
            </li>
            <?php endforeach; ?>
        </ul>
    <?php $content = ob_get_clean() ?>

    <?php include 'layout.php' ?>

Vous avez maintenant une méthode qui permet la réutilisation du layout. 
Malheureusement, pour accomplir cela, vous êtes forcer d'utiliser quelques 
fonctions laides de PHP (``ob_start()``, ``ob_get_clean()``) dans le template.
Symfony utilise un composant de ``Templating`` qui permet d'accomplir ce résultat
proprement et facilement. Vous le verrez en action bientôt.

Ajout d'une page de détail d'article
------------------------------------

La page de liste a maintenant été refactorisé afin que le code soit mieux organisé
et réutilisable. Pour le prouver, ajoutez une page de détail d'article ("show" page),
qui affiche un article du blog identifié par un paramètre de requète ``id``.

Pour commencer, créer une fonction dans le fichier  ``model.php`` qui récupère un seul 
article du blog en fonction d'un id passé en paramètre::


    // model.php
    function get_post_by_id($id)
    {
        $link = open_database_connection();

        $id = mysql_real_escape_string($id);
        $query = 'SELECT date, title, body FROM post WHERE id = '.$id;
        $result = mysql_query($query);
        $row = mysql_fetch_assoc($result);

        close_database_connection($link);

        return $row;
    }

Puis créez un nouveau fichier appelé ``show.php`` - le controlleur pour cette 
nouvelle page:

.. code-block:: html+php

    <?php
    require_once 'model.php';

    $post = get_post_by_id($_GET['id']);

    require 'templates/show.php';

Finalement, créez un nouveau fichier de template - ``templates/show.php`` - afin
d'afficher un articl du blog:

.. code-block:: html+php

    <?php $title = $post['title'] ?>

    <?php ob_start() ?>
        <h1><?php echo $post['title'] ?></h1>

        <div class="date"><?php echo $post['date'] ?></div>
        <div class="body">
            <?php echo $post['body'] ?>
        </div>
    <?php $content = ob_get_clean() ?>

    <?php include 'layout.php' ?>

Créer cette nouvelle page est vraiment facile et aucun code n'est dupliqué.
Malgré tout, cette page introduit des problèmes persistant qu'un framework peut
résoudre. Par exemple, un paramètre de requête ``id`` manquant ou invalide va
générer une erreur fatale. Il serait mieux que cela génére une erreur 404, mais
cela ne peut pas vraiment être fait facilement. Pire, si vous oubliez d'échapper
le paramètre  ``id`` avec la fonction ``mysql_real_escape_string()``, votre base
de données est sujette à des attaques d'injection SQL.

Un autre problème est que chaque fichier controlleur doit inclure le fichier 
``model.php``. Que se passerait-il si chaque controlleur devait soudainement
inclure un fichier additionnel ou effectuer une quelconque tache globale (comme
renforcer la sécurité)? Dans l'état actuel, tout nouveau code devra être ajouter
à chaque fichier controlleur. Si vous oubliez quelqus chose à un fichier, il serait
bon que ce ne soit pas relier à la sécurité...

Un "contrôleur frontal" à la rescousse
--------------------------------------

La solution est d'utiliser un :term:`contrôleur frontal` (front controller):
a fichier PHP à travers lequel chaque requête est processé. Avec un contrôleur
frontal, les URIs de l'application change un peu, mais elles sont plus flexibles:

.. code-block:: text

    Sans contrôleur frontal
    /index.php          => page de liste des articles (éxecution de index.php)
    /show.php           => page de détail d'un article (show.php executé)

    avec index.php comme contrôleur frontal
    /index.php          => page de liste des articles (éxecution de index.php)
    /index.php/show     => page de détail d'un article (éxecution de index.php)

.. tip::
	La portion ``index.php`` de l'URI peut être enlevé sen utilisant les règles
	de réécritures d'URI (ou équivalent). Dans ce cas, l'URI de la page de détail
	d'un article serait simplement ``/show``.

En utilisant un contrôleur frontal, un seul fichier PHP (``index.php`` dans notre cas)
traite *chaque* requête. Pour la page de détail d'un article, ``/index.php/show``
va donc exécuter le fichier ``index.php``, qui est maintenant responsable de router
en interne les requêtes en fonction de l'URI complète. Comme vous allez le voir,
un controlêur frontal est un outil très puissant.

Créer le contrôleur frontal
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous êtes sur le point de franchir une étapes importante avec l'application. 
Avec un seul fichier qui traite toutes les requêtes, vous pouvez centraliser des
fonctionnalités comme la gestion de la sécurité, le chargement de la configuration,
et le routage. Dans cette application, ``index.php`` doit être assez intelligent pour
traiter la page de liste des articles *ou* la page de détail d'un article en fonction
de l'URI demandée:

.. code-block:: html+php

    <?php
    // index.php

    // charge et initialise les librairies globales
    require_once 'model.php';
    require_once 'controllers.php';

    // route la requête en interne
    $uri = $_SERVER['REQUEST_URI'];
    if ($uri == '/index.php') {
        list_action();
    } elseif ($uri == '/index.php/show' && isset($_GET['id'])) {
        show_action($_GET['id']);
    } else {
        header('Status: 404 Not Found');
        echo '<html><body><h1>Page Not Found</h1></body></html>';
    }

Pour des raisons d'organisation, chaque controlleur (initialement  ``index.php`` et ``show.php``)
sont maintenant des fonctions PHP et ont été placées dans le fichier 
For organization, both controllers (formerly ``index.php`` and ``show.php``)``controllers.php``:

.. code-block:: php

    function list_action()
    {
        $posts = get_all_posts();
        require 'templates/list.php';
    }

    function show_action($id)
    {
        $post = get_post_by_id($id);
        require 'templates/show.php';
    }

En tant que contrôleur frontal, ``index.php`` a pris un nouveau rôle, celui d'inclure
les librairies principales et de router l'application pour qu'un des deux contrôleur
(les fonctiones ``list_action()`` et ``show_action()``) soit appelé. En réalité,
le contrôleur frontal commence à ressembler et à agir comme le mécanisme de Symfony2 qui prend 
en charge de route les requêtes.

======================> ICI <===============================

.. tip::

   Another advantage of a front controller is flexible URLs. Notice that
   the URL to the blog post show page could be changed from ``/show`` to ``/read``
   by changing code in only one location. Before, an entire file needed to
   be renamed. In Symfony2, URLs are even more flexible.

By now, the application has evolved from a single PHP file into a structure
that is organized and allows for code reuse. You should be happier, but far
from satisfied. For example, the "routing" system is fickle, and wouldn't
recognize that the list page (``/index.php``) should be accessible also via ``/``
(if Apache rewrite rules were added). Also, instead of developing the blog,
a lot of time is being spent working on the "architecture" of the code (e.g.
routing, calling controllers, templates, etc.). More time will need to be
spent to handle form submissions, input validation, logging and security.
Why should you have to reinvent solutions to all these routine problems?

Add a Touch of Symfony2
~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 to the rescue. Before actually using Symfony2, you need to make
sure PHP knows how to find the Symfony2 classes. This is accomplished via
an autoloader that Symfony provides. An autoloader is a tool that makes it
possible to start using PHP classes without explicitly including the file
containing the class.

First, `download symfony`_ and place it into a ``vendor/symfony/`` directory.
Next, create an ``app/bootstrap.php`` file. Use it to ``require`` the two
files in the application and to configure the autoloader:

.. code-block:: html+php

    <?php
    // bootstrap.php
    require_once 'model.php';
    require_once 'controllers.php';
    require_once 'vendor/symfony/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';

    $loader = new Symfony\Component\ClassLoader\UniversalClassLoader();
    $loader->registerNamespaces(array(
        'Symfony' => __DIR__.'/vendor/symfony/src',
    ));

    $loader->register();

This tells the autoloader where the ``Symfony`` classes are. With this, you
can start using Symfony classes without using the ``require`` statement for
the files that contain them.

Core to Symfony's philosophy is the idea that an application's main job is
to interpret each request and return a response. To this end, Symfony2 provides
both a :class:`Symfony\\Component\\HttpFoundation\\Request` and a
:class:`Symfony\\Component\\HttpFoundation\\Response` class. These classes are
object-oriented representations of the raw HTTP request being processed and
the HTTP response being returned. Use them to improve the blog:

.. code-block:: html+php

    <?php
    // index.php
    require_once 'app/bootstrap.php';

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;

    $request = Request::createFromGlobals();

    $uri = $request->getPathInfo();
    if ($uri == '/') {
        $response = list_action();
    } elseif ($uri == '/show' && $request->query->has('id')) {
        $response = show_action($request->query->get('id'));
    } else {
        $html = '<html><body><h1>Page Not Found</h1></body></html>';
        $response = new Response($html, 404);
    }

    // echo the headers and send the response
    $response->send();

The controllers are now responsible for returning a ``Response`` object.
To make this easier, you can add a new ``render_template()`` function, which,
incidentally, acts quite a bit like the Symfony2 templating engine:

.. code-block:: php

    // controllers.php
    use Symfony\Component\HttpFoundation\Response;

    function list_action()
    {
        $posts = get_all_posts();
        $html = render_template('templates/list.php', array('posts' => $posts));

        return new Response($html);
    }

    function show_action($id)
    {
        $post = get_post_by_id($id);
        $html = render_template('templates/show.php', array('post' => $post));

        return new Response($html);
    }

    // helper function to render templates
    function render_template($path, array $args)
    {
        extract($args);
        ob_start();
        require $path;
        $html = ob_get_clean();

        return $html;
    }

By bringing in a small part of Symfony2, the application is more flexible and
reliable. The ``Request`` provides a dependable way to access information
about the HTTP request. Specifically, the ``getPathInfo()`` method returns
a cleaned URI (always returning ``/show`` and never ``/index.php/show``).
So, even if the user goes to ``/index.php/show``, the application is intelligent
enough to route the request through ``show_action()``.

The ``Response`` object gives flexibility when constructing the HTTP response,
allowing HTTP headers and content to be added via an object-oriented interface.
And while the responses in this application are simple, this flexibility
will pay dividends as your application grows.

The Sample Application in Symfony2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The blog has come a *long* way, but it still contains a lot of code for such
a simple application. Along the way, we've also invented a simple routing
system and a method using ``ob_start()`` and ``ob_get_clean()`` to render
templates. If, for some reason, you needed to continue building this "framework"
from scratch, you could at least use Symfony's standalone `Routing`_ and
`Templating`_ components, which already solve these problems.

Instead of re-solving common problems, you can let Symfony2 take care of
them for you. Here's the same sample application, now built in Symfony2:

.. code-block:: html+php

    <?php
    // src/Acme/BlogBundle/Controller/BlogController.php

    namespace Acme\BlogBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function listAction()
        {
            $posts = $this->get('doctrine')->getEntityManager()
                ->createQuery('SELECT p FROM AcmeBlogBundle:Post p')
                ->execute();

            return $this->render('AcmeBlogBundle:Post:list.html.php', array('posts' => $posts));
        }

        public function showAction($id)
        {
            $post = $this->get('doctrine')
                ->getEntityManager()
                ->getRepository('AcmeBlogBundle:Post')
                ->find($id);
            
            if (!$post) {
                // cause the 404 page not found to be displayed
                throw $this->createNotFoundException();
            }

            return $this->render('AcmeBlogBundle:Post:show.html.php', array('post' => $post));
        }
    }

The two controllers are still lightweight. Each uses the Doctrine ORM library
to retrieve objects from the database and the ``Templating`` component to
render a template and return a ``Response`` object. The list template is
now quite a bit simpler:

.. code-block:: html+php

    <!-- src/Acme/BlogBundle/Resources/views/Blog/list.html.php --> 
    <?php $view->extend('::layout.html.php') ?>

    <?php $view['slots']->set('title', 'List of Posts') ?>

    <h1>List of Posts</h1>
    <ul>
        <?php foreach ($posts as $post): ?>
        <li>
            <a href="<?php echo $view['router']->generate('blog_show', array('id' => $post->getId())) ?>">
                <?php echo $post->getTitle() ?>
            </a>
        </li>
        <?php endforeach; ?>
    </ul>

The layout is nearly identical:

.. code-block:: html+php

    <!-- app/Resources/views/layout.html.php -->
    <html>
        <head>
            <title><?php echo $view['slots']->output('title', 'Default title') ?></title>
        </head>
        <body>
            <?php echo $view['slots']->output('_content') ?>
        </body>
    </html>

.. note::

    We'll leave the show template as an exercise, as it should be trivial to
    create based on the list template.

When Symfony2's engine (called the ``Kernel``) boots up, it needs a map so
that it knows which controllers to execute based on the request information.
A routing configuration map provides this information in a readable format::

    # app/config/routing.yml
    blog_list:
        pattern:  /blog
        defaults: { _controller: AcmeBlogBundle:Blog:list }

    blog_show:
        pattern:  /blog/show/{id}
        defaults: { _controller: AcmeBlogBundle:Blog:show }

Now that Symfony2 is handling all the mundane tasks, the front controller
is dead simple. And since it does so little, you'll never have to touch
it once it's created (and if you use a Symfony2 distribution, you won't
even need to create it!):

.. code-block:: html+php

    <?php
    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->handle(Request::createFromGlobals())->send();

The front controller's only job is to initialize Symfony2's engine (``Kernel``)
and pass it a ``Request`` object to handle. Symfony2's core then uses the
routing map to determine which controller to call. Just like before, the
controller method is responsible for returning the final ``Response`` object.
There's really not much else to it.

For a visual representation of how Symfony2 handles each request, see the
:ref:`request flow diagram<request-flow-figure>`.

Where Symfony2 Delivers
~~~~~~~~~~~~~~~~~~~~~~~

In the upcoming chapters, you'll learn more about how each piece of Symfony
works and the recommended organization of a project. For now, let's see how
migrating the blog from flat PHP to Symfony2 has improved life:

* Your application now has **clear and consistently organized code** (though
  Symfony doesn't force you into this). This promotes **reusability** and
  allows for new developers to be productive in your project more quickly.

* 100% of the code you write is for *your* application. You **don't need
  to develop or maintain low-level utilities** such as :ref:`autoloading<autoloading-introduction-sidebar>`,
  :doc:`routing</book/routing>`, or rendering :doc:`controllers</book/controller>`.

* Symfony2 gives you **access to open source tools** such as Doctrine and the
  Templating, Security, Form, Validation and Translation components (to name
  a few).

* The application now enjoys **fully-flexible URLs** thanks to the ``Routing``
  component.

* Symfony2's HTTP-centric architecture gives you access to powerful tools
  such as **HTTP caching** powered by **Symfony2's internal HTTP cache** or
  more powerful tools such as `Varnish`_. This is covered in a later chapter
  all about :doc:`caching</book/http_cache>`.

And perhaps best of all, by using Symfony2, you now have access to a whole
set of **high-quality open source tools developed by the Symfony2 community**!
For more information, check out `Symfony2Bundles.org`_

Better templates
----------------

If you choose to use it, Symfony2 comes standard with a templating engine
called `Twig`_ that makes templates faster to write and easier to read.
It means that the sample application could contain even less code! Take,
for example, the list template written in Twig:

.. code-block:: html+jinja

    {# src/Acme/BlogBundle/Resources/views/Blog/list.html.twig #}

    {% extends "::layout.html.twig" %}
    {% block title %}List of Posts{% endblock %}

    {% block body %}
        <h1>List of Posts</h1>
        <ul>
            {% for post in posts %}
            <li>
                <a href="{{ path('blog_show', { 'id': post.id }) }}">
                    {{ post.title }}
                </a>
            </li>
            {% endfor %}
        </ul>
    {% endblock %}

The corresponding ``layout.html.twig`` template is also easier to write:

.. code-block:: html+jinja

    {# app/Resources/views/layout.html.twig #}

    <html>
        <head>
            <title>{% block title %}Default title{% endblock %}</title>
        </head>
        <body>
            {% block body %}{% endblock %}
        </body>
    </html>

Twig is well-supported in Symfony2. And while PHP templates will always
be supported in Symfony2, we'll continue to discuss the many advantages of
Twig. For more information, see the :doc:`templating chapter</book/templating>`.

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/service`

.. _`Doctrine`: http://www.doctrine-project.org
.. _`download symfony`: http://symfony.com/download
.. _`Routing`: https://github.com/symfony/Routing
.. _`Templating`: https://github.com/symfony/Templating
.. _`Symfony2Bundles.org`: http://symfony2bundles.org
.. _`Twig`: http://www.twig-project.org
.. _`Varnish`: http://www.varnish-cache.org
.. _`PHPUnit`: http://www.phpunit.de
