.. index::
   single: Les fondamentaux de Symfony2

Les fondamentaux de Symfony2 et HTTP
====================================

Félicitations! Grâce à l'apprentissage de Symfony2, vous êtes sur la bonne voie pour
devenir un développeur web plus *productif* et *populaire* (en fait vous serez livré
à vous-même pour la dernière partie). Symfony2 est construit de manière à revenir à
l'essentiel: pour implémenter des outils qui vous aide à développer plus rapidement
et à construire des applications plus robustes, tout en restant hors de votre chemin.
Symfony repose sur les meilleures idées provenant de diverses technologies: les outils
et concepts que vous êtes sur le point d'apprendre représentent les efforts de
milliers de personnes depuis de nombreuses années. En d'autres termes, vous
n'apprenez pas juste "Symfony", vous apprenez les fondamentaux du web,
les bonnes pratiques de développement, et comment utiliser de nombreuses
nouvelles bibliothèques PHP, internes ou indépendantes de Symfony2. Alors,
soyez prêt!

Fidèle à la philosophie de Symfony2, ce chapitre débute par une explication du
concept fondamental du développement web: HTTP. Quelles que soient vos
connaissances ou votre langage de programmation préféré, ce chapitre **doit
être lu** par tout un chacun.

HTTP est Simple
---------------

HTTP (Hypertext Transfer Protocol pour les geeks) est un langage texte qui
permet à deux machines de communiquer ensemble. C'est tout! Par exemple,
lorsque vous regardez la dernière BD de `xkcd`_, la conversation suivante
(approximative) se déroule:

.. image:: /images/http-xkcd.png
   :align: center

Et alors que l'actuel langage utilisé est un peu plus formel, cela reste
toujours très simple. HTTP est le terme utilisé pour décrire ce simple
langage basé sur le texte. Et peu importe comment vous développez sur
le web, le but de votre serveur est *toujours* de comprendre de simples
requêtes composées de texte, et de retourner de simples réponses composées
elles aussi de texte.

Symfony2 est construit sur les bases de cette réalité. Que vous le
réalisiez ou non, HTTP est quelque chose que vous utilisez tous les jours.
Avec Symfony2, vous allez apprendre comment le maîtriser.

.. index::
   single: HTTP; Paradigme requête-réponse

Etape 1: Le Client envoie une Requête
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Chaque conversation sur le web débute avec une *requête*. La requête est
un message textuel créé par un client (par exemple: un navigateur, une
application iPhone, etc...) dans un format spécial connu sous le nom d'HTTP.
Le client envoie cette requête à un serveur, et puis attend la réponse.

Jetez un oeil à la première partie de l'intéraction (la requête) entre un
navigateur et le serveur web xkcd:

.. image:: /images/http-xkcd-request.png
   :align: center

Dans le langage HTTP, cette requête HTTP ressemblerait à quelque chose
comme ça:

.. code-block:: text

    GET / HTTP/1.1
    Host: xkcd.com
    Accept: text/html
    User-Agent: Mozilla/5.0 (Macintosh)

Ce simple message communique *tout* ce qui est nécessaire concernant la
ressource que le client a demandé. La première ligne d'une requête HTTP
est la plus importante et contient deux choses: l'URI et la méthode HTTP.

L'URI (par exemple: ``/``, ``/contact``, etc...) est l'adresse unique ou
la localisation qui identifie la ressource que le client veut. La méthode
HTTP (par exemple: ``GET``) définit ce que vous voulez *faire* avec la
ressource. Les méthodes HTTP sont les *verbes* de la requête et définissent
les quelques moyens avec lesquels vous pouvez agir sur la ressource:

+----------+-----------------------------------------+
| *GET*    | Récupère la ressource depuis le serveur |
+----------+-----------------------------------------+
| *POST*   | Crée une ressource sur le serveur       |
+----------+-----------------------------------------+
| *PUT*    | Met à jour la ressource sur le serveur  |
+----------+-----------------------------------------+
| *DELETE* | Supprime la ressource sur le serveur    |
+----------+-----------------------------------------+

Avec ceci en mémoire, vous pouvez imaginer ce à quoi ressemblerait une
requête HTTP pour supprimer une entrée spécifique d'un blog, par exemple:

.. code-block:: text

    DELETE /blog/15 HTTP/1.1

.. note::

    Il y a en fait neuf méthodes HTTP définies par la spécification HTTP,
    mais beaucoup d'entre elles ne sont pas largement utilisées ou supportées.
    En réalité, beaucoup de navigateurs modernes ne supportent pas les méthodes
    ``PUT`` et ``DELETE``.

En plus de la première ligne, une requête HTTP contient invariablement
d'autres lignes d'informations appelées en-têtes de requête. Les en-têtes
peuvent fournir un large éventail d'informations tels que l'en-tête requise
``Host``, le format de réponse que le client accepte (``Accept``) et
l'application que le client utilise pour effectuer la requête (``User-Agent``).
Beaucoup d'autres en-têtes existent et peuvent être trouvées sur la page
Wikipedia `List of HTTP header fields`_ (anglais).

Etape 2: Le Serveur retourne une réponse
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Une fois que le serveur a reçu la requête, il connaît exactement quelle ressource
le client a besoin (via l'URI) et ce que le client souhaite faire avec cette
ressource (via la méthode). Par exemple, dans le cas d'une requête GET, le
serveur prépare la ressource et la retourne dans une réponse HTTP. Considérez
la réponse du serveur web xkcd:

.. image:: /images/http-xkcd.png
   :align: center

Traduit en HTTP, la réponse envoyée au navigateur va ressembler à quelque chose
comme ça:

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 02 Apr 2011 21:05:05 GMT
    Server: lighttpd/1.4.19
    Content-Type: text/html

    <html>
      <!-- HTML for the xkcd comic -->
    </html>

La réponse HTTP contient la ressource demandé (le contenu HTML dans ce cas),
ainsi que d'autres informations à propos de la réponse. La première ligne
est spécialement importante et contient le code de statut de la réponse
HTTP (200 dans ce cas). Le code de statut communique le résultat global
de la requête retournée au client. A-t-elle réussie? Y'a-t-il eu une
erreur? Différents codes de statut existent qui indiquent le succès, une
erreur, ou que le client a besoin de faire quelque chose (par exemple:
rediriger sur une autre page). Une liste complète peut être trouvée sur
la page Wikipedia `List of HTTP status codes`_ (anglais).

Comme la requête, une réponse HTTP contient de l'information additionnelle
appelée en-têtes HTTP. Par exemple, une importante en-tête de réponse HTTP
est le ``Content-Type``. Le corps d'une même ressource peut être retournée
dans de multiples formats incluant HTML, XML ou JSON pour en nommer quelques
uns. L'en-tête ``Content-Type`` dit au client quel format va être retourné.

De nombreuses autres en-têtes existent, dont quelques unes sont très puissantes.
Par exemple, certaines en-têtes peuvent être utilisées pour créer un puissant
système de cache.

Requêtes, Réponses et Développement Web
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cette conversation requête-réponse est le procédé fondamental qui dirige
toute communication sur le web. Et tout aussi important et puissant que ce
procédé soit, il est inéluctablement simple.

Le plus important fait est: quel que soit le langage que vous utilisez, le
type d'application que vous construisez (web, mobile, API JSON), ou la
philosophie de développement que vous suivez, l'objectif final d'une
application est **toujours** de comprendre chaque requête et de créer et
retourner la réponse appropriée.

Symfony est architecturé pour correspondre à cette réalité.

.. tip::

    Pour en savoir plus à propos de la spécification HTTP, lisez l'original
    `HTTP 1.1 RFC`_ ou le `HTTP Bis`_, qui est effort actif pour clarifier la
     spécification originale. Un super outil pour inspecter/vérifier les en-têtes
     de la requête et de la réponse durant votre navigation est l'extension
     pour Firefox `Live HTTP Headers`_.

.. index::
   single: Symfony2 Fundamentals; Requests and responses

Requests and Responses in PHP
-----------------------------

So how do you interact with the "request" and create a "response" when using
PHP? In reality, PHP abstracts you a bit from the whole process:

.. code-block:: php

    <?php
    $uri = $_SERVER['REQUEST_URI'];
    $foo = $_GET['foo'];

    header('Content-type: text/html');
    echo 'The URI requested is: '.$uri;
    echo 'The value of the "foo" parameter is: '.$foo;

As strange as it sounds, this small application is in fact taking information
from the HTTP request and using it to create an HTTP response. Instead of
parsing the raw HTTP request message, PHP prepares superglobal variables
such as ``$_SERVER`` and ``$_GET`` that contain all the information from
the request. Similarly, instead of returning the HTTP-formatted text response,
you can use the ``header()`` function to create response headers and simply
print out the actual content that will be the content portion of the response
message. PHP will create a true HTTP response and return it to the client:

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 03 Apr 2011 02:14:33 GMT
    Server: Apache/2.2.17 (Unix)
    Content-Type: text/html

    The URI requested is: /testing?foo=symfony
    The value of the "foo" parameter is: symfony

Requests and Responses in Symfony
---------------------------------

Symfony provides an alternative to the raw PHP approach via two classes that
allow you to interact with the HTTP request and response in an easier way.
The :class:`Symfony\\Component\\HttpFoundation\\Request` class is a simple
object-oriented representation of the HTTP request message. With it, you
have all the request information at your fingertips::

    use Symfony\Component\HttpFoundation\Request;

    $request = Request::createFromGlobals();

    // the URI being requested (e.g. /about) minus any query parameters
    $request->getPathInfo();

    // retrieve GET and POST variables respectively
    $request->query->get('foo');
    $request->request->get('bar');

    // retrieves an instance of UploadedFile identified by foo
    $request->files->get('foo');

    $request->getMethod();          // GET, POST, PUT, DELETE, HEAD
    $request->getLanguages();       // an array of languages the client accepts

As a bonus, the ``Request`` class does a lot of work in the background that
you'll never need to worry about. For example, the ``isSecure()`` method
checks the *three* different values in PHP that can indicate whether or not
the user is connecting via a secured connection (i.e. ``https``).

Symfony also provides a ``Response`` class: a simple PHP representation of
an HTTP response message. This allows your application to use an object-oriented
interface to construct the response that needs to be returned to the client::

    use Symfony\Component\HttpFoundation\Response;
    $response = new Response();

    $response->setContent('<html><body><h1>Hello world!</h1></body></html>');
    $response->setStatusCode(200);
    $response->headers->set('Content-Type', 'text/html');

    // prints the HTTP headers followed by the content
    $response->send();

If Symfony offered nothing else, you would already have a toolkit for easily
accessing request information and an object-oriented interface for creating
the response. Even as you learn the many powerful features in Symfony, keep
in mind that the goal of your application is always *to interpret a request
and create the appropriate response based on your application logic*.

.. tip::

    The ``Request`` and ``Response`` classes are part of a standalone component
    included with Symfony called ``HttpFoundation``. This component can be
    used entirely independent of Symfony and also provides classes for handling
    sessions and file uploads.

The Journey from the Request to the Response
--------------------------------------------

Like HTTP itself, the ``Request`` and ``Response`` objects are pretty simple.
The hard part of building an application is writing what's comes in between.
In other words, the real work comes in writing the code that interprets the
request information and creates the response.

Your application probably does many things, like sending emails, handling
form submissions, saving things to a database, rendering HTML pages and protecting
content with security. How can you manage all of this and still keep your
code organized and maintainable?

Symfony was created to solve these problems so that you don't have to.

The Front Controller
~~~~~~~~~~~~~~~~~~~~

Traditionally, applications were built so that each "page" of a site was
its own physical file:

.. code-block:: text

    index.php
    contact.php
    blog.php

There are several problems with this approach, including the inflexibility
of the URLs (what if you wanted to change ``blog.php`` to ``news.php`` without
breaking all of your links?) and the fact that each file *must* manually
include some set of core files so that security, database connections and
the "look" of the site can remain consistent.

A much better solution is to use a :term:`front controller`: a single PHP
file that handles every request coming into your application. For example:

+------------------------+------------------------+
| ``/index.php``         | executes ``index.php`` |
+------------------------+------------------------+
| ``/index.php/contact`` | executes ``index.php`` |
+------------------------+------------------------+
| ``/index.php/blog``    | executes ``index.php`` |
+------------------------+------------------------+

.. tip::

    Using Apache's ``mod_rewrite`` (or equivalent with other web servers),
    the URLs can easily be cleaned up to be just ``/``, ``/contact`` and
    ``/blog``.

Now, every request is handled exactly the same. Instead of individual URLs
executing different PHP files, the front controller is *always* executed,
and the routing of different URLs to different parts of your application
is done internally. This solves both problems with the original approach.
Almost all modern web apps do this - including apps like WordPress.

Stay Organized
~~~~~~~~~~~~~~

But inside your front controller, how do you know which page should
be rendered and how can you render each in a sane way? One way or another, you'll need to
check the incoming URI and execute different parts of your code depending
on that value. This can get ugly quickly:

.. code-block:: php

    // index.php

    $request = Request::createFromGlobals();
    $path = $request->getPathInfo(); // the URL being requested

    if (in_array($path, array('', '/')) {
        $response = new Response('Welcome to the homepage.');
    } elseif ($path == '/contact') {
        $response = new Response('Contact us');
    } else {
        $response = new Response('Page not found.', 404);
    }
    $response->send();

Solving this problem can be difficult. Fortunately it's *exactly* what Symfony
is designed to do.

The Symfony Application Flow
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When you let Symfony handle each request, life is much easier. Symfony follows
the same simple pattern for every request:

.. _request-flow-figure:

.. figure:: /images/request-flow.png
   :align: center
   :alt: Symfony2 request flow

   Incoming requests are interpreted by the routing and passed to controller
   functions that return ``Response`` objects.

Each "page" of your site is defined in a routing configuration file that
maps different URLs to different PHP functions. The job of each PHP function,
called a :term:`controller`, is to use information from the request - along
with many other tools Symfony makes available - to create and return a ``Response``
object. In other words, the controller is where *your* code goes: it's where
you interpret the request and create a response.

It's that easy! Let's review:

* Each request executes a front controller file;

* The routing system determines which PHP function should be executed based
  on information from the request and routing configuration you've created;

* The correct PHP function is executed, where your code creates and returns
  the appropriate ``Response`` object.

A Symfony Request in Action
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Without diving into too much detail, let's see this process in action. Suppose
you want to add a ``/contact`` page to your Symfony application. First, start
by adding an entry for ``/contact`` to your routing configuration file:

.. code-block:: yaml

    contact:
        pattern:  /contact
        defaults: { _controller: AcmeDemoBundle:Main:contact }

.. note::

   This example uses :doc:`YAML</reference/YAML>` to define the routing configuration.
   Routing configuration can also be written in other formats such as XML
   or PHP.

When someone visits the ``/contact`` page, this route is matched, and the
specified controller is executed. As you'll learn in the :doc:`routing chapter</book/routing>`,
the ``AcmeDemoBundle:Main:contact`` string is a short syntax that points to a
specific PHP method ``contactAction`` inside a class called ``MainController``:

.. code-block:: php

    class MainController
    {
        public function contactAction()
        {
            return new Response('<h1>Contact us!</h1>');
        }
    }

In this very simple example, the controller simply creates a ``Response``
object with the HTML "<h1>Contact us!</h1>". In the :doc:`controller chapter</book/controller>`,
you'll learn how a controller can render templates, allowing your "presentation"
code (i.e. anything that actually writes out HTML) to live in a separate
template file. This frees up the controller to worry only about the hard
stuff: interacting with the database, handling submitted data, or sending
email messages. 

Symfony2: Build your App, not your Tools.
-----------------------------------------

You now know that the goal of any app is to interpret each incoming request
and create an appropriate response. As an application grows, it becomes more
difficult to keep your code organized and maintainable. Invariably, the same
complex tasks keep coming up over and over again: persisting things to the
database, rendering and reusing templates, handling form submissions, sending
emails, validating user input and handling security.

The good news is that none of these problems is unique. Symfony provides
a framework full of tools that allow you to build your application, not your
tools. With Symfony2, nothing is imposed on you: you're free to use the full
Symfony framework, or just one piece of Symfony all by itself.

.. index::
   single: Symfony2 Components

Standalone Tools: The Symfony2 *Components*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

So what *is* Symfony2? First, Symfony2 is a collection of over twenty independent
libraries that can be used inside *any* PHP project. These libraries, called
the *Symfony2 Components*, contain something useful for almost any situation,
regardless of how your project is developed. To name a few:

* `HttpFoundation`_ - Contains the ``Request`` and ``Response`` classes, as
  well as other classes for handling sessions and file uploads;

* `Routing`_ - Powerful and fast routing system that allows you to map a
  specific URI (e.g. ``/contact``) to some information about how that request
  should be handled (e.g. execute the ``contactAction()`` method);

* `Form`_ - A full-featured and flexible framework for creating forms and
  handing form submissions;

* `Validator`_ A system for creating rules about data and then validating
  whether or not user-submitted data follows those rules;

* `ClassLoader`_ An autoloading library that allows PHP classes to be used
  without needing to manually ``require`` the files containing those classes;

* `Templating`_ A toolkit for rendering templates, handling template inheritance
  (i.e. a template is decorated with a layout) and performing other common
  template tasks;

* `Security`_ - A powerful library for handling all types of security inside
  an application;

* `Translation`_ A framework for translating strings in your application.

Each and every one of these components is decoupled and can be used in *any*
PHP project, regardless of whether or not you use the Symfony2 framework.
Every part is made to be used if needed and replaced when necessary.

The Full Solution: The Symfony2 *Framework*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

So then, what *is* the Symfony2 *Framework*? The *Symfony2 Framework* is
a PHP library that accomplishes two distinct tasks:

#. Provides a selection of components (i.e. the Symfony2 Components) and
   third-party libraries (e.g. ``Swiftmailer`` for sending emails);

#. Provides sensible configuration and a "glue" library that ties all of these
   pieces together.

The goal of the framework is to integrate many independent tools in order
to provide a consistent experience for the developer. Even the framework
itself is a Symfony2 bundle (i.e. a plugin) that can be configured or replaced
entirely.

Symfony2 provides a powerful set of tools for rapidly developing web applications
without imposing on your application. Normal users can quickly start development
by using a Symfony2 distribution, which provides a project skeleton with
sensible defaults. For more advanced users, the sky is the limit.

.. _`xkcd`: http://xkcd.com/
.. _`HTTP 1.1 RFC`: http://www.w3.org/Protocols/rfc2616/rfc2616.html
.. _`HTTP Bis`: http://datatracker.ietf.org/wg/httpbis/
.. _`Live HTTP Headers`: https://addons.mozilla.org/en-US/firefox/addon/3829/
.. _`List of HTTP status codes`: http://en.wikipedia.org/wiki/List_of_HTTP_status_codes
.. _`List of HTTP header fields`: http://en.wikipedia.org/wiki/List_of_HTTP_header_fields
.. _`HttpFoundation`: https://github.com/symfony/HttpFoundation
.. _`Routing`: https://github.com/symfony/Routing
.. _`Form`: https://github.com/symfony/Form
.. _`Validator`: https://github.com/symfony/Validator
.. _`ClassLoader`: https://github.com/symfony/ClassLoader
.. _`Templating`: https://github.com/symfony/Templating
.. _`Security`: https://github.com/symfony/Security
.. _`Translation`: https://github.com/symfony/Translation
