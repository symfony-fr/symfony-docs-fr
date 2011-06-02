Introduction
============

Commencez à utiliser Symfony2 en 10 minutes! Ce tutoriel vous guide à travers
quelques-uns des concepts les plus importants de Symfony2. Il explique
comment démarrer rapidement en vous montrant la structure d'un projet simple.

Si vous avez déjà utilisé un framework web avant, vous devriez vous sentir à l'aise
avec Symfony2. Sinon, bienvenue dans une toute nouvelle façon de développer des
applications web!

.. tip:

    Vous voulez savoir quand utiliser un framework et pourquoi? Lisez "`Symfony
    en 5 minutes`_".

Télécharger Symfony2
--------------------

Tout d'abord, vérifiez que vous avez installé et configuré un serveur web (comme
Apache) avec PHP 5.3.2 ou supérieur.

Vous êtes prêts? Commençons par télécharter la "`Symfony2 Standard Edition`_",
une :term:`distribution` Symfony préconfigurée pour répondre à la plupart des besoins,
et qui contient également du code expliquant comment fonctionne Symfony2
(téléchargez l'archive avec les *vendors* pour gagner encore plus de temps).

Après avoir décompressé l'archive dans la racine de votre serveur web, vous devriez
avoir un répertoire ``Symfony/`` qui ressemble à:

.. code-block:: text

    www/ <- votre répertoire racine
        Symfony/ <- l'archive décompressée
            app/
                cache/
                config/
                logs/
            src/
                Acme/
                    DemoBundle/
                        Controller/
                        Resources/
            vendor/
                symfony/
                doctrine/
                ...
            web/
                app.php

Vérifier la Configuration
-------------------------

Symfony2 est livré avec une interface de test de votre configuration pour
vous éviter tous maux de tête dûs à des problèmes de serveur ou à une mauvaise 
configuration de PHP. Utilisez l'URL suivante pour consulter le diagnostic de 
votre serveur:

.. code-block:: text

    http://localhost/Symfony/web/config.php

S'il y encore des problèmes listés, vous devez les corriger. Vous pouvez également
modifier votre configuration en suivant les recommandations données.
Lorsque tout est bon, cliquez sur "Aller à la page d'accueil" pour afficher votre
première "vraie" page Symfony2:

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/

Symfony2 devrait vous féliciter pour le travail accompli jusqu'à présent!

.. image:: /images/quick_tour/welcome.jpg

Comprendre les fondamentaux
---------------------------

L'un des principaux objectifs d'un framework est de garantir la séparation des
tâches. Cela permet à votre code de rester organisé et à votre application d'évoluer
facilement au fil du temps en évitant de mélanger dans le même script les appels
de base de données, le code HTML et la logique métier. Pour atteindre cet objectif
avec Symfony, vous aurez d'abord besoin d'apprendre quelques notions et connaitre
les termes fondamentaux.

.. tip::

    Vous voulez une preuve qu'utiliser un framework est mieux que tout mélanger
    dans le même script? Lisez le chapitre ":doc:`/book/from_flat_php_to_symfony2`".

La distribution est fournie avec des exemples de code que vous pouvez utiliser pour
comprendre les concepts de Symfony2. Entrez l'URL suivante pour être salué par
Symfony2 (remplacez *Fabien* par votre prénom):

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/demo/hello/Fabien

Que se passe t-il ici? Décortiquons cette URL:

* ``app_dev.php``: C'est un :term:`contrôleur frontal`. C'est l'unique point
  d'entrée de votre application et cela prend en charge toutes les requêtes.

* ``/demo/hello/Fabien``: C'est le *chemin virtuel* vers la ressource à laquelle
  l'utilisateur veut accéder.

Votre responsabilité en tant que développeur est d'écrire le code qui permet
d'associer la *requête* d'un utilisateur (``/demo/hello/Fabien``) à la *ressource*
qui y est rattachée (``Hello Fabien!``).

Routage
~~~~~~~

Symfony2 achemenine la requête au code qui la gère en essayant d'associer l'URL
demandée à des masques prédéfinis. Par défaut, ces masques (appelés routes) sont
définis dans le fichier de configuration ``app/config/routing.yml``:

.. code-block:: yaml

    # app/config/routing.yml
    _welcome:
        pattern:  /
        defaults: { _controller: AcmeDemoBundle:Welcome:index }

    _demo:
        resource: "@AcmeDemoBundle/Controller/DemoController.php"
        type:     annotation
        prefix:   /demo

Les trois premières lignes (après le commentaire) définissent le code qui sera
exécuté quand l'utilisateur demandera la ressource "``/``" (c'est-à-dire la page
d'accueil). Suite à cette requête, le contrôleur ``AcmeDemoBundle:Welcome:index``
sera exécuté.


.. tip::
    
    La Symfony2 Standard Edition utilise le format `YAML`_ pour ses fichiers de
    configuration, mais Symfony2 supporte également nativement le XML, le PHP,
    et les annotations. Les différents formats sont compatibles et peuvent être
    utilisées de façon interchangeable dans une application. Enfin, les performances
    de votre application ne dépendent pas du format de configuration que vous aurez
    choisi puisque tout est mis en cache lors de la première requête.

Contrôleurs
~~~~~~~~~~~

Un contrôleur prend en charge les *requêtes* entrantes et retourne des *réponses*
(souvent du code HTML). PLutôt que d'utiliser des variables globales PHP et des
fonctions (comme ``$_GET`` ou ``header()``) pour gérer ces messages HTTP, Symfony
utilise des objets:
:class:`Symfony\\Component\\HttpFoundation\\Request` et
:class:`Symfony\\Component\\HttpFoundation\\Response`. Le plus simple contrôleur
qu'il puisse exister crée une réponse à la main, basée sur la requête::

    use Symfony\Component\HttpFoundation\Response;

    $name = $request->query->get('name');

    return new Response('Hello '.$name, 200, array('Content-Type' => 'text/plain'));

.. note::

    Ne vous laissez pas berner par la simplicité de ces concepts et leur puissance.
    Lisez le chapitre ":doc:`/book/http_fundamentals`" pour en savoir plus sur la
    manière dont Symfony2 gère HTTP et pourquoi il rend les choses plus simples
    et plus puissantes à la fois.

Symfony2 choisit le contrôleur en se basant sur la valeur du paramètre ``_controller``
du fichier de routage: ``AcmeDemoBundle:Welcome:index``. Cette chaîne de caractères
est le *nom logique* du contrôleur et elle fait référence à la méthode ``indexAction``
de la classe ``Acme\DemoBundle\Controller\WelcomeController``::

    // src/Acme/DemoBundle/Controller/WelcomeController.php
    namespace Acme\DemoBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class WelcomeController extends Controller
    {
        public function indexAction()
        {
            return $this->render('AcmeDemoBundle:Welcome:index.html.twig');
        }
    }

.. tip::

    Vous auriez pu utiliser 
    ``Acme\DemoBundle\Controller\WelcomeController::indexAction`` comme valeur du
    paramètre ``_controller`` mais en suivant des conventions simples, le nom 
    logique est plus court et vous actroie plus de flexibilité.

La classe ``WelcomeController`` étend la classe ``Controller`` qui fournit des
raccourcis très pratiques vers des méthodes comme la méthode
:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::render`
qui charge et rend un template
(``AcmeDemoBundle:Welcome:index.html.twig``). La valeur retournée est un objet
Response qui contient le contenu rendu. Donc, si le besoin s'en fait sentir, vous
pouvez modifier facilement l'objet Response avant de l'envoyer au navigateur::

    public function indexAction()
    {
        $response = $this->render('AcmeDemoBundle:Welcome:index.txt.twig');
        $response->headers->set('Content-Type', 'text/plain');

        return $response;
    }

.. tip::

    Etendre la classe ``Controller`` est facultatif. En fait, un contrôleur peut
    être une simple fonction PHP ou même une closure.
    Le chapitre ":doc:`The Controller</book/controller>`" vous dira tout ce que
    vous voudrez savoir sur les contrôleurs.

Le nom du template, ``AcmeDemoBundle:Welcome:index.html.twig``, est son *nom logique*
et il fait référence au fichier ``src/Acme/DemoBundle/Resources/views/Welcome/index.html.twig``.
La section ci-dessous sur les bundles vous expliquera en quoi cela peut être utile.

Maintenant, jetez un oeil à la fin de la configuration de routage:

.. code-block:: yaml

    # app/config/routing.yml
    _demo:
        resource: "@AcmeDemoBundle/Controller/DemoController.php"
        type:     annotation
        prefix:   /demo

Symfony2 peut lire 
Symfony2 can read the routing information from different resources written in
YAML, XML, PHP, or even embedded in PHP annotations. Here, the resource
*logical name* is ``@AcmeDemoBundle/Controller/DemoController.php`` and refers
to the ``src/Acme/DemoBundle/Controller/DemoController.php`` file. In this
file, routes are defined as annotations on action methods::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    class DemoController extends Controller
    {
        /**
         * @Route("/hello/{name}", name="_demo_hello")
         * @Template()
         */
        public function helloAction($name)
        {
            return array('name' => $name);
        }

        // ...
    }

The ``@Route()`` annotation defines a new route with a pattern of
``/hello/{name}`` that executes the ``helloAction`` method when matched. A
string enclosed in curly brackets like ``{name}`` is called a placeholder. As
you can see, its value can be retrieved through the ``$name`` method argument.

.. note::

    Even if annotations are not natively supported by PHP, you use them
    extensively in Symfony2 as a convenient way to configure the framework
    behavior and keep the configuration next to the code.

If you take a closer look at the action code, you can see that instead of
rendering a template like before, it just returns an array of parameters. The
``@Template()`` annotation tells Symfony to render the template for you,
passing in each variable of the array to the template. The name of the
template that's rendered follows the name of the controller. So, in this
example, the ``AcmeDemoBundle:Demo:hello.html.twig`` template is rendered
(located at ``src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig``).

.. tip::

    The ``@Route()`` and ``@Template()`` annotations are more powerful than
    the simple examples shown in this tutorial. Learn more about "`annotations
    in controllers`_" in the official documentation.

Templates
~~~~~~~~~

The controller renders the
``src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig`` template (or
``AcmeDemoBundle:Demo:hello.html.twig`` if you use the logical name):

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {% block title "Hello " ~ name %}

    {% block content %}
        <h1>Hello {{ name }}!</h1>
    {% endblock %}

By default, Symfony2 uses `Twig`_ as its template engine but you can also use
traditional PHP templates if you choose. The next chapter will introduce how
templates work in Symfony2.

Bundles
~~~~~~~

You might have wondered why the :term:`bundle` word is used in many names we
have seen so far. All the code you write for your application is organized in
bundles. In Symfony2 speak, a bundle is a structured set of files (PHP files,
stylesheets, JavaScripts, images, ...) that implements a single feature (a
blog, a forum, ...) and which can be easily shared with other developers. As
of now, we have manipulated one bundle, ``AcmeDemoBundle``. You will learn
more about bundles in the last chapter of this tutorial.

Working with Environments
-------------------------

Now that you have a better understanding of how Symfony2 works, have a closer
look at the bottom of the page; you will notice a small bar with the Symfony2
logo. This is called the "Web Debug Toolbar" and it is the developer's best
friend. But this is only the tip of the iceberg; click on the weird hexadecimal
number to reveal yet another very useful Symfony2 debugging tool: the profiler.

Of course, you won't want to show these tools when you deploy your application
to production. That's why you will find another front controller in the
``web/`` directory (``app.php``), which is optimized for the production environment:

.. code-block:: text

    http://localhost/Symfony/web/app.php/demo/hello/Fabien

And if you use Apache with ``mod_rewrite`` enabled, you can even omit the
``app.php`` part of the URL:

.. code-block:: text

    http://localhost/Symfony/web/demo/hello/Fabien

Last but not least, on the production servers, you should point your web root
directory to the ``web/`` directory to secure your installation and have an
even better looking URL:

.. code-block:: text

    http://localhost/demo/hello/Fabien

To make you application respond faster, Symfony2 maintains a cache under the
``app/cache/`` directory. In the development environment (``app_dev.php``),
this cache is flushed automatically whenever you make changes to any code or
configuration. But that's not the case in the production environment
(``app.php``) where performance is key. That's why you should always use
the development environment when developing your application.

Different :term:`environments<environment>` of a given application differ
only in their configuration. In fact, a configuration can inherit from another
one:

.. code-block:: yaml

    # app/config/config_dev.yml
    imports:
        - { resource: config.yml }

    web_profiler:
        toolbar: true
        intercept_redirects: false

The ``dev`` environment (defined in ``config_dev.yml``) inherits from the
global ``config.yml`` file and extends it by enabling the web debug toolbar.

Final Thoughts
--------------

Congratulations! You've had your first taste of Symfony2 code. That wasn't so
hard, was it? There's a lot more to explore, but you should already see how
Symfony2 makes it really easy to implement web sites better and faster. If you
are eager to learn more about Symfony2, dive into the next section: "The
View".

.. _Symfony2 Standard Edition:      http://symfony.com/download
.. _Symfony en 5 minutes:           http://symfony.com/symfony-in-five-minutes
.. _Separation of Concerns:         http://en.wikipedia.org/wiki/Separation_of_concerns
.. _YAML:                           http://www.yaml.org/
.. _annotations in controllers:     http://bundles.symfony-reloaded.org/frameworkextrabundle/
.. _Twig:                           http://www.twig-project.org/
