.. index::
   single: Le Contrôleur

Le Contrôleur
=============

Un contrôleur est une fonction PHP créée par vos soins qui prend l'information
provenant de la requête HTTP et construit puis retourne une réponse HTTP
(sous forme d'un objet Symfony2 ``Response``). La réponse pourrait être
une page HTML, un document XML, un tableau JSON sérialisé, une image, une
redirection, une erreur 404 ou quoi que ce soit d'autre dont vous pouvez
rêver. Le contrôleur contient n'importe quelle logique arbitraire dont
*votre application* a besoin pour retourner le contenu d'une page.

Pour en illustrer la simplicité, jetons un oeil à un contrôleur Symfony2
en action. Le contrôleur suivant rend une page qui écrit simplement
``Hello world!``::

    use Symfony\Component\HttpFoundation\Response;

    public function helloAction()
    {
        return new Response('Hello world!');
    }

Le but d'un contrôleur est toujours le même: créer et retourner un objet
``Response``. Durant son cheminement, il se peut qu'il lise de l'information
depuis la requête, qu'il charge une ressource depuis la base de données, qu'il
envoie un email, ou qu'il définisse une valeur dans la session de l'utilisateur.
Mais dans tous les cas, le contrôleur va finalement retourner l'objet ``Response``
qui sera délivré au client.

Il n'y a pas de magie et aucune autre exigence à prendre en compte! Suivent
quelques exemples communs:

* *Le contrôleur A* prépare un objet ``Response`` représentant le contenu de
  la page d'accueil.

* *Le contrôleur B* lit le paramètre ``slug`` contenu dans la requête pour
  charger une entrée du blog depuis la base de données et crée un objet
  ``Response`` affichant ce blog. Si le ``slug`` ne peut pas être trouvé
  dans la base de données, il crée et retourne un objet ``Response`` avec
  un code de statut 404.

* *Le contrôleur C* gère la soumission d'un formulaire de contact. Il lit
  l'information de ce dernier depuis la requête, enregistre les informations
  du contact dans la base de données et envoie ces dernières par email au webmaster.
  Enfin, il crée un objet ``Response`` qui redirige le navigateur du client vers
  la page "merci" du formulaire de contact.

.. index::
   single: Le Contrôleur; Cycle de vie Requête-contrôleur-réponse

Cycle de vie Requête, Contrôleur, Réponse
-----------------------------------------

Chaque requête gérée par un projet Symfony2 suit le même cycle de vie. Le
framework prend soin des tâches répétitives et exécute finalement un contrôleur
qui contient votre code applicatif personnalisé:

#. Chaque requête est gérée par un unique fichier contrôleur frontal (par exemple:
``app.php`` ou ``app_dev.php``) qui démarre l'application;

#. Le ``Router`` lit l'information depuis la requête (par exemple: l'URI), trouve
   une route qui correspond à cette information, et lit le paramètre ``_controller``
   depuis la route;

#. Le contrôleur correspondant à la route est exécuté et le code interne au
   contrôleur crée et retourne un objet ``Response``;

#. Les en-têtes HTTP et le contenu de l'objet ``Response`` sont envoyés au client.

Créer une page est aussi facile que de créer un contrôleur (#3) et d'implémenter une
route qui fasse correspondre une URL à ce dernier (#2).

.. note::

    Bien que nommé de la même manière, un "contrôleur frontal" est différent
    des "contrôleurs" dont nous allons parler dans ce chapitre. Un contrôleur
    frontal est un petit fichier PHP qui se situe dans votre répertoire web et
    à travers lequel toutes les requêtes sont dirigées. Une application typique
    va avoir un contrôleur frontal de production (par exemple: ``app.php``) et
    un contrôleur frontal de développement (par exemple: ``app_dev.php``). Vous
    n'aurez vraisemblablement jamais besoin d'éditer, de regarder ou de vous
    occuper des contrôleurs frontaux dans votre application.

.. index::
   single: Le Contrôleur; Un exemple simple

Un contrôleur simple
--------------------

Bien qu'un contrôleur puisse être n'importe quel "chose PHP" appelable (une
fonction, une méthode d'un objet, ou une ``Closure``), dans Symfony2, un
contrôleur est généralement une unique méthode à l'intérieur d'un objet contrôleur.
Les contrôleurs sont aussi appelés *actions*.

.. code-block:: php
    :linenos:

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

.. tip::

    Notez que le *contrôleur* est la méthode ``indexAction``, qui réside
    dans une *classe contrôleur* (``HelloController``). Ne soyez pas confus
    par le nommage: une *classe contrôleur* est simplement une manière
    pratique de grouper plusieurs contrôleurs/actions ensemble. Typiquement,
    la classe contrôleur va héberger plusieurs contrôleurs/actions (par exemple:
    ``updateAction``, ``deleteAction``, etc).

Ce contrôleur est relativement simple, mais parcourons-le tout de même:

* *ligne 3*: Symfony2 tire avantage de la fonctionnalité des espaces de noms
  ("namespaces") de PHP 5.3 afin de donner un espace de noms à la classe entière
  du contrôleur. Le mot-clé ``use`` importe la classe ``Response``, que notre
  contrôleur doit retourner.

* *ligne 6*: Le nom de la classe est la concaténation d'un nom pour la classe
  du contrôleur (par exemple: ``Hello``) et du mot ``Controller``. Ceci est une
  convention qui fournit une uniformité aux contrôleurs et qui leurs permet
  d'être référencés seulement par la première partie du nom (par exemple: ``Hello``)
  dans la configuration de routage.

* *ligne 8*: Chaque action dans une classe contrôleur est suffixée avec ``Action``
  et est référencée dans la configuration du routage par le nom de l'action
  (``index``). Dans la prochaine section, vous allez créer une route qui fait
  correspondre une URI à son action. Vous allez apprendre comment les paramètres
  substituables de la route (``{name}``) deviennent les arguments de la méthode
  action (``$name``).

* *ligne 10*: Le contrôleur crée et retourne un objet ``Response``.

.. index::
   single: Controller; Routes and controllers

Mapping a URL to a Controller
-----------------------------

The new controller returns a simple HTML page. To actually view this page
in your browser, you need to create a route, which maps a specific URL pattern
to the controller:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            pattern:      /hello/{name}
            defaults:     { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hello" pattern="/hello/{name}">
            <default key="_controller">AcmeHelloBundle:Hello:index</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

Going to ``/hello/ryan`` now executes the ``HelloController::indexAction()``
controller and passes in ``ryan`` for the ``$name`` variable. Creating a
"page" means simply creating a controller method and associated route.

Notice the syntax used to refer to the controller: ``AcmeHelloBundle:Hello:index``.
Symfony2 uses a flexible string notation to refer to different controllers.
This is the most common syntax and tells Symfony2 to look for a controller
class called ``HelloController`` inside a bundle named ``AcmeHelloBundle``. The
method ``indexAction()`` is then executed.

For more details on the string format used to reference different controllers,
see :ref:`controller-string-syntax`.

.. note::

    This example places the routing configuration directly in the ``app/config/``
    directory. A better way to organize your routes is to place each route
    in the bundle it belongs to. For more information on this, see
    :ref:`routing-include-external-resources`.

.. tip::

    You can learn much more about the routing system in the :doc:`Routing chapter</book/routing>`.

.. index::
   single: Controller; Controller arguments

.. _route-parameters-controller-arguments:

Route Parameters as Controller Arguments
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You already know that the ``_controller`` parameter ``AcmeHelloBundle:Hello:index``
refers to a ``HelloController::indexAction()`` method that lives inside the
``AcmeHelloBundle`` bundle. What's more interesting is the arguments that are
passed to that method:

.. code-block:: php

    <?php
    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
          // ...
        }
    }

The controller has a single argument, ``$name``, which corresponds to the
``{name}`` parameter from the matched route (``ryan`` in our example). In
fact, when executing your controller, Symfony2 matches each argument of
the controller with a parameter from the matched route. Take the following
example:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            pattern:      /hello/{first_name}/{last_name}
            defaults:     { _controller: AcmeHelloBundle:Hello:index, color: green }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hello" pattern="/hello/{first_name}/{last_name}">
            <default key="_controller">AcmeHelloBundle:Hello:index</default>
            <default key="color">green</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{first_name}/{last_name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
            'color'       => 'green',
        )));

The controller for this can take several arguments::

    public function indexAction($first_name, $last_name, $color)
    {
        // ...
    }

Notice that both placeholder variables (``{first_name}``, ``{last_name}``)
as well as the default ``color`` variable are available as arguments in the
controller. When a route is matched, the placeholder variables are merged
with the ``defaults`` to make one array that's available to your controller.

Mapping route parameters to controller arguments is easy and flexible. Keep
the following guidelines in mind while you develop.

* **The order of the controller arguments does not matter**

    Symfony is able to match the parameter names from the route to the variable
    names in the controller method's signature. In other words, it realizes that
    the ``{last_name}`` parameter matches up with the ``$last_name`` argument.
    The arguments of the controller could be totally reordered and still work
    perfectly::

        public function indexAction($last_name, $color, $first_name)
        {
            // ..
        }

* **Each required controller argument must match up with a routing parameter**

    The following would throw a ``RuntimeException`` because there is no ``foo``
    parameter defined in the route::

        public function indexAction($first_name, $last_name, $color, $foo)
        {
            // ..
        }

    Making the argument optional, however, is perfectly ok. The following
    example would not throw an exception::

        public function indexAction($first_name, $last_name, $color, $foo = 'bar')
        {
            // ..
        }

* **Not all routing parameters need to be arguments on your controller**

    If, for example, the ``last_name`` weren't important for your controller,
    you could omit it entirely::

        public function indexAction($first_name, $color)
        {
            // ..
        }

.. tip::

    Every route also has a special ``_route`` parameter, which is equal to
    the name of the route that was matched (e.g. ``hello``). Though not usually
    useful, this is equally available as a controller argument.

The ``Request`` as a Controller Argument
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For convenience, you can also have Symfony pass you the ``Request`` object
as an argument to your controller. This is especially convenient when you're
working with forms, for example::

    use Symfony\Component\HttpFoundation\Request;

    public function updateAction(Request $request)
    {
        $form = $this->createForm(...);
        
        $form->bindRequest($request);
        // ...
    }

.. index::
   single: Controller; Base controller class

The Base Controller Class
-------------------------

For convenience, Symfony2 comes with a base ``Controller`` class that assists
with some of the most common controller tasks and gives your controller class
access to any resource it might need. By extending this ``Controller`` class,
you can take advantage of several helper methods.

Add the ``use`` statement atop the ``Controller`` class and then modify the
``HelloController`` to extend it:

.. code-block:: php

    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
          return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

This doesn't actually change anything about how your controller works. In
the next section, you'll learn about the helper methods that the base controller
class makes available. These methods are just shortcuts to using core Symfony2
functionality that's available to you with or without the use of the base
``Controller`` class. A great way to see the core functionality in action
is to look in the
:class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller` class
itself.

.. tip::

    Extending the base class is *optional* in Symfony; it contains useful
    shortcuts but nothing mandatory. You can also extend
    ``Symfony\Component\DependencyInjection\ContainerAware``. The service
    container object will then be accessible via the ``container`` property.

.. note::

    You can also define your :doc:`Controllers as Services
    </cookbook/controller/service>`.

.. index::
   single: Controller; Common Tasks

Common Controller Tasks
-----------------------

Though a controller can do virtually anything, most controllers will perform
the same basic tasks over and over again. These tasks, such as redirecting,
forwarding, rendering templates and accessing core services, are very easy
to manage in Symfony2.

.. index::
   single: Controller; Redirecting

Redirecting
~~~~~~~~~~~

If you want to redirect the user to another page, use the ``redirect()`` method::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'));
    }

The ``generateUrl()`` method is just a helper function that generates the URL
for a given route. For more information, see the :doc:`Routing </book/routing>`
chapter.

By default, the ``redirect()`` method performs a 302 (temporary) redirect. To
perform a 301 (permanent) redirect, modify the second argument::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'), 301);
    }

.. tip::

    The ``redirect()`` method is simply a shortcut that creates a ``Response``
    object that specializes in redirecting the user. It's equivalent to:

    .. code-block:: php

        use Symfony\Component\HttpFoundation\RedirectResponse;

        return new RedirectResponse($this->generateUrl('homepage'));

.. index::
   single: Controller; Forwarding

Forwarding
~~~~~~~~~~

You can also easily forward to another controller internally with the ``forward()``
method. Instead of redirecting the user's browser, it makes an internal sub-request,
and calls the specified controller. The ``forward()`` method returns the ``Response``
object that's returned from that controller::

    public function indexAction($name)
    {
        $response = $this->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green'
        ));

        // further modify the response or return it directly
        
        return $response;
    }

Notice that the `forward()` method uses the same string representation of
the controller used in the routing configuration. In this case, the target
controller class will be ``HelloController`` inside some ``AcmeHelloBundle``.
The array passed to the method becomes the arguments on the resulting controller.
This same interface is used when embedding controllers into templates (see
:ref:`templating-embedding-controller`). The target controller method should
look something like the following::

    public function fancyAction($name, $color)
    {
        // ... create and return a Response object
    }

And just like when creating a controller for a route, the order of the arguments
to ``fancyAction`` doesn't matter. Symfony2 matches the index key names
(e.g. ``name``) with the method argument names (e.g. ``$name``). If you
change the order of the arguments, Symfony2 will still pass the correct
value to each variable.

.. tip::

    Like other base ``Controller`` methods, the ``forward`` method is just
    a shortcut for core Symfony2 functionality. A forward can be accomplished
    directly via the ``http_kernel`` service. A forward returns a ``Response``
    object::
    
        $httpKernel = $this->container->get('http_kernel');
        $response = $httpKernel->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green',
        ));

.. index::
   single: Controller; Rendering templates

.. _controller-rendering-templates:

Rendering Templates
~~~~~~~~~~~~~~~~~~~

Though not a requirement, most controllers will ultimately render a template
that's responsible for generating the HTML (or other format) for the controller.
The ``renderView()`` method renders a template and returns its content. The
content from the template can be used to create a ``Response`` object::

    $content = $this->renderView('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

    return new Response($content);

This can even be done in just one step with the ``render()`` method, which
returns a ``Response`` object containing the content from the template::

    return $this->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

In both cases, the ``Resources/views/Hello/index.html.twig`` template inside
the ``AcmeHelloBundle`` will be rendered.

The Symfony templating engine is explained in great detail in the
:doc:`Templating </book/templating>` chapter.

.. tip::

    The ``renderView`` method is a shortcut to direct use of the ``templating``
    service. The ``templating`` service can also be used directly::
    
        $templating = $this->get('templating');
        $content = $templating->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

.. index::
   single: Controller; Accessing services

Accessing other Services
~~~~~~~~~~~~~~~~~~~~~~~~

When extending the base controller class, you can access any Symfony2 service
via the ``get()`` method. Here are several common services you might need::

    $request = $this->getRequest();

    $response = $this->get('response');

    $templating = $this->get('templating');

    $router = $this->get('router');

    $mailer = $this->get('mailer');

There are countless other services available and you are encouraged to define
your own. To list all available services, use the ``container:debug`` console
command:

.. code-block:: bash

    php app/console container:debug

For more information, see the :doc:`/book/service_container` chapter.

.. index::
   single: Controller; Managing errors
   single: Controller; 404 pages

Managing Errors and 404 Pages
-----------------------------

When things are not found, you should play well with the HTTP protocol and
return a 404 response. To do this, you'll throw a special type of exception.
If you're extending the base controller class, do the following::

    public function indexAction()
    {
        $product = // retrieve the object from database
        if (!$product) {
            throw $this->createNotFoundException('The product does not exist');
        }

        return $this->render(...);
    }

The ``createNotFoundException()`` method creates a special ``NotFoundHttpException``
object, which ultimately triggers a 404 HTTP response inside Symfony.

Of course, you're free to throw any ``Exception`` class in your controller -
Symfony2 will automatically return a 500 HTTP response code.

.. code-block:: php

    throw new \Exception('Something went wrong!');

In every case, a styled error page is shown to the end user and a full debug
error page is shown to the developer (when viewing the page in debug mode).
Both of these error pages can be customized. For details, read the
":doc:`/cookbook/controller/error_pages`" cookbook recipe.

.. index::
   single: Controller; The session
   single: Session

Managing the Session
--------------------

Symfony2 provides a nice session object that you can use to store information
about the user (be it a real person using a browser, a bot, or a web service)
between requests. By default, Symfony2 stores the attributes in a cookie
by using the native PHP sessions.

Storing and retrieving information from the session can be easily achieved
from any controller::

    $session = $this->getRequest()->getSession();

    // store an attribute for reuse during a later user request
    $session->set('foo', 'bar');

    // in another controller for another request
    $foo = $session->get('foo');

    // set the user locale
    $session->setLocale('fr');

These attributes will remain on the user for the remainder of that user's
session.

.. index::
   single Session; Flash messages

Flash Messages
~~~~~~~~~~~~~~

You can also store small messages that will be stored on the user's session
for exactly one additional request. This is useful when processing a form:
you want to redirect and have a special message shown on the *next* request.
These types of messages are called "flash" messages.

For example, imagine you're processing a form submit::

    public function updateAction()
    {
        $form = $this->createForm(...);

        $form->bindRequest($this->getRequest());
        if ($form->isValid()) {
            // do some sort of processing

            $this->get('session')->setFlash('notice', 'Your changes were saved!');

            return $this->redirect($this->generateUrl(...));
        }

        return $this->render(...);
    }

After processing the request, the controller sets a ``notice`` flash message
and then redirects. The name (``notice``) isn't significant - it's just what
you're using to identify the type of the message.

In the template of the next action, the following code could be used to render
the ``notice`` message:

.. configuration-block::

    .. code-block:: html+jinja

        {% if app.session.hasFlash('notice') %}
            <div class="flash-notice">
                {{ app.session.flash('notice') }}
            </div>
        {% endif %}

    .. code-block:: php
    
        <?php if ($view['session']->hasFlash('notice')): ?>
            <div class="flash-notice">
                <?php echo $view['session']->getFlash('notice') ?>
            </div>
        <?php endif; ?>

By design, flash messages are meant to live for exactly one request (they're
"gone in a flash"). They're designed to be used across redirects exactly as
you've done in this example.

.. index::
   single: Controller; Response object

The Response Object
-------------------

The only requirement for a controller is to return a ``Response`` object. The
:class:`Symfony\\Component\\HttpFoundation\\Response` class is a PHP
abstraction around the HTTP response - the text-based message filled with HTTP
headers and content that's sent back to the client::

    // create a simple Response with a 200 status code (the default)
    $response = new Response('Hello '.$name, 200);
    
    // create a JSON-response with a 200 status code
    $response = new Response(json_encode(array('name' => $name)));
    $response->headers->set('Content-Type', 'application/json');

.. tip::

    The ``headers`` property is a
    :class:`Symfony\\Component\\HttpFoundation\\HeaderBag` object with several
    useful methods for reading and mutating the ``Response`` headers. The
    header names are normalized so that using ``Content-Type`` is equivalent
    to ``content-type`` or even ``content_type``.

.. index::
   single: Controller; Request object

The Request Object
------------------

Besides the values of the routing placeholders, the controller also has access
to the ``Request`` object when extending the base ``Controller`` class::

    $request = $this->getRequest();

    $request->isXmlHttpRequest(); // is it an Ajax request?

    $request->getPreferredLanguage(array('en', 'fr'));

    $request->query->get('page'); // get a $_GET parameter

    $request->request->get('page'); // get a $_POST parameter

Like the ``Response`` object, the request headers are stored in a ``HeaderBag``
object and are easily accessible.

Final Thoughts
--------------

Whenever you create a page, you'll ultimately need to write some code that
contains the logic for that page. In Symfony, this is called a controller,
and it's a PHP function that can do anything it needs to to in order to return
the final ``Response`` object that will be returned to the user.

To make life easier, you can choose to extend a base ``Controller`` class,
which contains shortcut methods for many common controller tasks. For example,
since you don't want to put put HTML code in your controller, you can use
the ``render()`` method to render and return the content from a template.

In other chapters, you'll see how the controller can be used to persist and
fetch objects from a database, process form submissions, handle caching and
more.

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/controller/service`
