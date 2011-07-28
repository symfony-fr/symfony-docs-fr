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
  dans la configuration de routage ("routing").

* *ligne 8*: Chaque action dans une classe contrôleur est suffixée avec ``Action``
  et est référencée dans la configuration du routage par le nom de l'action
  (``index``). Dans la prochaine section, vous allez créer une route qui fait
  correspondre une URI à son action. Vous allez apprendre comment les paramètres
  substituables de la route (``{name}``) deviennent les arguments de la méthode
  action (``$name``).

* *ligne 10*: Le contrôleur crée et retourne un objet ``Response``.

.. index::
   single: Le Contrôleur; Routes et contrôleurs

Faire correspondre une URL à un Contrôleur
------------------------------------------

Le nouveau contrôleur retourne une simple page HTML. Pour voir cette page dans
votre navigateur, vous avez besoin de créer une route qui va faire correspondre
un pattern d'URL spécifique à ce contrôleur:

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

Naviguer à l'URL ``/hello/ryan`` va maintenant exécuter le contrôleur
``HelloController::indexAction()`` et passer en tant que variable ``$name`` la
valeur ``ryan``. Créer une "page" signifie simplement créer une méthode contrôleur
et une route associée.

Notez la syntaxe utilisée pour faire référence au contrôleur: ``AcmeHelloBundle:Hello:index``.
Symfony2 utilise une notation de chaîne de caractères flexible pour référer aux
différents contrôleurs. Ceci est la syntaxe la plus commune qui spécifie à Symfony2 de
chercher une classe contrôleur appelée ``HelloController`` dans un bundle appelé
``AcmeHelloBundle``. La méthode ``indexAction()`` est alors exécutée.

Pour plus de détails sur le format de chaîne de caractères utilisé pour référencer
les différents contrôleurs, regardez du côté de :ref:`controller-string-syntax`.

.. note::

    Cet exemple place la configuration de routage directement dans le répertoire
    ``app/config/``. Une meilleure façon d'organiser vos routes est de placer
    chacune d'entre elles dans le bundle auquel elle appartient. Pour plus
    d'informations sur ceci, voyez :ref:`routing-include-external-resources`.

.. tip::

    Vous pouvez apprendre beaucoup plus de choses à propos du système de routage dans
    :doc:`Routing chapter</book/routing>`.

.. index::
   single: Le Contrôleur; Les arguments du contrôleur

.. _route-parameters-controller-arguments:

Les paramètres de la route en tant qu'arguments du contrôleur
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous savez déjà que le paramètre ``_controller`` ``AcmeHelloBundle:Hello:index``
réfère à une méthode ``HelloController::indexAction()`` qui réside dans le bundle
``AcmeHelloBundle``. Ce qui est plus intéressant sont les arguments qui sont passés
à cette méthode:

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

Le contrôleur possède un argument unique, ``$name``, qui correspond au
paramètre ``{name}`` de la route associée (``ryan`` dans notre exemple).
En fait, lorsque vous exécutez votre contrôleur, Symfony2 fait correspondre
chaque argument du contrôleur avec un paramètre de la route correspondante.
Prenez l'exemple suivant:

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

Le contrôleur dans cet exemple peut prendre plusieurs arguments::

    public function indexAction($first_name, $last_name, $color)
    {
        // ...
    }

Notez que les deux variables de substitution (``{first_name}``, ``{last_name}``)
ainsi que la variable par défaut ``color`` sont disponibles en tant qu'arguments
dans le contrôleur. Quand une route correspond, les variables de substitution
sont fusionnées avec celles ``par défaut`` afin de construire un tableau
qui est à la disposition de votre contrôleur.

Faire correspondre les paramètres de la route aux arguments du contrôleur est
facile et flexible. Gardez les directives suivantes en tête quand vous développez.

* **L'ordre des arguments du contrôleur n'a pas d'importance**

    Symfony est capable de faire correspondre les noms des paramètres de la route
    aux noms des variables de la signature de la méthode du contrôleur. En d'autres
    termes, il réalise que le paramètre ``{last_name}`` correspond à l'argument
    ``$last_name``. Les arguments du contrôleur pourraient être totalement
    réorganisés que cela fonctionnerait toujours parfaitement::

        public function indexAction($last_name, $color, $first_name)
        {
            // ..
        }

* **Chaque argument requis du contrôleur doit correspondre à un paramètre de la route**

    Le code suivant lancerait une ``RuntimeException`` parce qu'il n'y a pas
    de paramètre ``foo`` défini dans la route::

        public function indexAction($first_name, $last_name, $color, $foo)
        {
            // ..
        }

    Cependant, définir l'argument en tant qu'optionnel est parfaitement valide.
    L'exemple suivant ne lancerait pas d'exception::

        public function indexAction($first_name, $last_name, $color, $foo = 'bar')
        {
            // ..
        }

* **Tous les paramètres de la route n'ont pas besoin d'être des arguments de votre contrôleur**

    Si, par exemple, le paramètre ``last_name`` n'était pas important pour votre
    contrôleur, vous pourriez complètement l'omettre::

        public function indexAction($first_name, $color)
        {
            // ..
        }

.. tip::

    Chaque route possède aussi un paramètre spécial ``_route`` qui est égal
    au nom de la route qui a correspondu (par exemple: ``hello``). Bien que
    pas très utile généralement, il est néanmoins disponible en tant qu'argument
    du contrôleur au même titre que les autres.

La ``Requête`` en tant qu'argument du Contrôleur
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour plus de commodité, Symfony peut aussi vous passer l'objet ``Request``
en tant qu'argument de votre contrôleur. Ceci est spécialement pratique
lorsque vous travaillez avec les formulaires, par exemple::

    use Symfony\Component\HttpFoundation\Request;

    public function updateAction(Request $request)
    {
        $form = $this->createForm(...);
        
        $form->bindRequest($request);
        // ...
    }

.. index::
   single: Le Contrôleur; La classe contrôleur de base

La Classe Contrôleur de Base
----------------------------

Afin de vous faciliter le travail, Symfony2 vient avec une classe ``Controller``
de base qui vous assiste dans les tâches les plus communes d'un contrôleur et
qui donne à votre propre classe contrôleur l'accès à n'importe quelle ressource
dont elle pourrait avoir besoin. En étendant cette classe ``Controller``, vous
pouvez tirer parti de plusieurs méthodes d'aide ("helper").

Ajoutez le mot-clé ``use`` au-dessus de la classe ``Controller`` et modifiez
``HelloController`` pour qu'il l'étende:

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

Cela ne change en fait rien au fonctionnement de votre contrôleur. Dans la
prochaine section, vous apprendrez des choses sur les méthodes d'aide que la
classe contrôleur de base met à votre disposition. Ces méthodes sont juste
des raccourcis pour utiliser des fonctionnalités coeurs de Symfony2 qui sont
à votre disposition en utilisant ou non la classe ``Controller`` de base.
Une bonne façon de voir cette fonctionnalité coeur en action est de regarder
la classe :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`
elle-même.

.. tip::

    Etendre la classe de base est *optionnel* dans Symfony; elle contient
    des raccourcis utiles mais rien d'obligatoire. Vous pouvez aussi étendre
    ``Symfony\Component\DependencyInjection\ContainerAware``. L'objet conteneur
    de service ("service container") sera ainsi accessible à travers la
    propriété ``container``.

.. note::

    Vous pouvez aussi définir vos :doc:`Contrôleurs en tant que Services
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
