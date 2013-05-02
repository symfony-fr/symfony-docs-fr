.. index::
   single: Routing

Routage
=======

De belles URLs sont une obligation absolue pour une quelconque application web
sérieuse. Cela implique d'oublier les URLs moches comme
``index.php?article_id=57`` en faveur de quelque chose comme
``/read/intro-to-symfony``.

Avoir de la flexibilité est encore plus important. Que se passe-t-il si
vous avez besoin de changer l'URL d'une page de ``/blog`` pour ``/news`` ?
Combien de liens aurez-vous besoin de traquer et mettre à jour afin
de prendre en compte ce changement ? Si vous utilisez le routeur de Symfony,
le changement est simple.

Le routeur Symfony2 vous laisse définir des URLs créatives que vous faites
correspondre à différents points de votre application. A la fin de ce
chapitre, vous serez capable de :

* Créer des routes complexes qui correspondent à des contrôleurs
* Générer des URLs à l'intérieur des templates et des contrôleurs
* Charger des ressources de routage depuis des bundles (ou depuis ailleurs)
* Débugger vos routes

.. index::
   single: Routing; Basics

Le routage en Action
--------------------

Une *route* est une correspondance entre un pattern d'URL et un contrôleur.
Par exemple, supposez que vous vouliez faire correspondre n'importe quelle
URL comme ``/blog/my-post`` ou ``/blog/all-about-symfony`` et l'envoyer vers
un contrôleur qui puisse chercher et retourner ce billet de blog.
La route est simple :

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" pattern="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog_show', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

Le pattern défini par la route ``blog_show`` agit comme ``/blog/*`` où le
joker (l'étoile) possède le nom ``slug``. Pour l'URL ``/blog/my-blog-post``,
la variable ``slug`` prend la valeur de ``my-blog-post``, qui est à votre
disposition dans votre contrôleur (continuez à lire).

Le paramètre ``_controller`` est une clé spéciale qui dit à Symfony quel
contrôleur doit être exécuté lorsqu'une URL correspond à cette route.
La chaîne de caractères ``_controller`` est appelée le
:ref:`nom logique<controller-string-syntax>`. Il suit un pattern qui pointe
vers une classe et une méthode PHP spécifique::

    // src/Acme/BlogBundle/Controller/BlogController.php    
    namespace Acme\BlogBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function showAction($slug)
        {
            $blog = // utilisez la variable $slug pour interroger la base de données

            return $this->render('AcmeBlogBundle:Blog:show.html.twig', array(
                'blog' => $blog,
            ));
        }
    }

Félicitations ! Vous venez juste de créer votre première route et de la connecter
à un contrôleur. Maintenant, quand vous visitez ``/blog/my-post``, le contrôleur
``showAction`` va être exécuté et la variable ``$slug`` aura la valeur ``my-post``.

Ceci est le but du routeur Symfony2 : faire correspondre l'URL d'une requête
à un contrôleur. Tout au long du chemin, vous allez apprendre toutes sortes
d'astuces qui rendent même facile la création des URLs les plus complexes.

.. index::
   single: Routing; Under the hood

Routage: Sous le Capot
----------------------

Quand une requête est lancée vers votre application, elle contient une adresse
pointant sur la « ressource » exacte que le client désire. Cette adresse est
appelée l'URL, (ou l'URI), et pourrait être ``/contact``, ``/blog/read-me``,
ou n'importe quoi d'autre. Prenez l'exemple de la requête HTTP suivante :

.. code-block:: text

    GET /blog/my-blog-post

Le but du système de routage de Symfony2 est d'analyser cette URL et de
déterminer quel contrôleur devrait être exécuté. Le déroulement complet
ressemble à ça :

#. La requête est gérée par le contrôleur frontal de Symfony2 (par exemple :
   ``app.php``) ;

#. Le coeur de Symfony2 (c-a-d Kernel) demande au routeur d'inspecter la
   requête ;

#. Le routeur fait correspondre l'URL entrante à une route spécifique et retourne
   l'information à propos de la route, incluant le contrôleur qui devrait
   être exécuté ;

#. Le Kernel Symfony2 exécute le contrôleur, qui finalement retourne un
   objet ``Response``.

.. figure:: /images/request-flow.png
   :align: center
   :alt: Symfony2 request flow

   La partie routage est un outil qui traduit l'URL entrante en un contrôleur
   spécifique à exécuter.

.. index::
   single: Routing; Creating routes

Créer des Routes
----------------

Symfony charge toutes les routes de votre application depuis un fichier unique
de configuration du routage. Le fichier est généralement localisé dans le
répertoire ``app/config/routing.yml``, mais peut être configuré afin d'être
n'importe quoi d'autre (incluant un fichier XML ou PHP) via le fichier de
configuration de l'application :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config ...>
            <!-- ... -->
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'router'        => array('resource' => '%kernel.root_dir%/config/routing.php'),
        ));

.. tip::

    Bien que toutes les routes soient chargées depuis un fichier unique, c'est
    une pratique courante d'inclure des ressources de routage additionnelles.
    Pour faire cela, il vous suffit de spécifier quels fichiers externes doivent
    être inclus dans le fichier de configuration de routage principal.Référez-vous
    à la section :ref:`routing-include-external-resources` pour plus d'informations.

Configuration Basique des Routes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Définir une route est facile, et une application typique va avoir de nombreuses
routes. Une route basique consiste simplement de deux parties : le ``pattern``
à comparer et un tableau ``defaults`` :

.. configuration-block::

    .. code-block:: yaml

        _welcome:
            pattern:   /
            defaults:  { _controller: AcmeDemoBundle:Main:homepage }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="_welcome" pattern="/">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
            </route>

        </routes>

    ..  code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('_welcome', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
        )));

        return $collection;

Cette route correspond à la page d'accueil (``/``) et la relie avec le
contrôleur ``AcmeDemoBundle:Main:homepage``. La chaîne de caractères
``_controller`` est traduite par Symfony2 en une fonction PHP qui est
ensuite exécutée. Ce processus sera expliqué rapidement dans la section
:ref:`controller-string-syntax`.

.. index::
   single: Routing; Placeholders

Routage avec les Paramètres de substitution
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Évidemment, le système de routage supporte des routes beaucoup plus
intéressantes. De nombreuses routes vont contenir un ou plusieurs
paramètres de substitution nommés « joker » :

.. configuration-block::

    .. code-block:: yaml

        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" pattern="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog_show', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

Le pattern va faire correspondre tout ce qui ressemble à ``/blog/*``.
Mieux encore, la valeur correspondante au paramètre de substitution
``{slug}`` sera disponible dans votre contrôleur. En d'autres termes, si
l'URL est ``/blog/hello-world``, une variable ``$slug``, avec la valeur
``hello-world``, sera à votre disposition dans le contrôleur. Ceci
peut être utilisé, par exemple, pour récupérer l'entrée du blog qui
correspond à cette chaîne de caractères.

Cependant, le pattern *ne va pas* faire correspondre simplement ``/blog``
à cette route. Et cela parce que, par défaut, tous les paramètres de substitution
sont requis. Ce comportement peut être modifié en ajoutant une valeur
au paramètre de substitution dans le tableau ``defaults``.

Paramètres de substitution Requis et Optionnels
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour rendre les choses plus excitantes, ajoutez une nouvelle route qui
affiche une liste de toutes les entrées du blog disponibles pour cette
application imaginaire :

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
        )));

        return $collection;

Jusqu'ici, cette route est aussi simple que possible - elle ne contient
pas de paramètres de substitution et va correspondre uniquement à l'URL
exacte ``/blog``. Mais que se passe-t-il si vous avez besoin que cette
route supporte la pagination, où ``blog/2`` affiche la seconde page des
entrées du blog ? Mettez la route à jour afin qu'elle ait un nouveau
paramètre de substitution ``{page}`` :

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
        )));

        return $collection;

Comme le paramètre de substitution ``{slug}`` d'avant, la valeur correspondante
à ``{page}`` va être disponible dans votre contrôleur. Cette dernière peut être
utilisée pour déterminer quel ensemble d'entrées blog doit être délivré
pour la page donnée.

Mais attendez ! Sachant que les paramètres substitutifs sont requis par
défaut, cette route va maintenant arrêter de correspondre à une requête
contenant simplement l'URL ``/blog``. A la place, pour voir la page 1 du
blog, vous devriez utiliser l'URL ``/blog/1`` ! Ceci n'étant pas une solution
« viable » pour une telle application web, modifiez la route et faites en sorte
que le paramètre ``{page}`` soit optionnel. Vous pouvez faire cela en
l'incluant dans la collection des ``defaults`` :

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page' => 1,
        )));

        return $collection;

En ajoutant ``page`` aux clés ``defaults``, le paramètre substitutif ``{page}``
n'est donc plus obligatoire. L'URL ``/blog`` va correspondre à cette route
et la valeur du paramètre ``page`` sera défini comme étant ``1``. L'URL
``/blog/2`` quant à elle va aussi correspondre, donnant au paramètre ``page``
la valeur ``2``. Parfait.

+---------+------------+
| /blog   | {page} = 1 |
+---------+------------+
| /blog/1 | {page} = 1 |
+---------+------------+
| /blog/2 | {page} = 2 |
+---------+------------+

.. tip::

    Les routes avec des paramètres optionnels à la fin ne correspondront pas à la requête
    demandée s'il y a un slash à la fin (ex ``/blog/`` ne correspondra pas, ``/blog`` correspondra).
    
.. index::
   single: Routing; Requirements

Ajouter des Conditions Requises
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Regardez rapidement les routes qui ont été créées jusqu'ici :

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }

        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
            </route>

            <route id="blog_show" pattern="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page' => 1,
        )));

        $collection->add('blog_show', new Route('/blog/{show}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

Pouvez-vous voir le problème ? Notez que les deux routes possèdent des
patterns qui correspondent aux URLs ressemblant à ``/blog/*``. Le routeur
Symfony choisira toujours la **première** route correspondante qu'il trouve.
En d'autres mots, la route ``blog_show`` ne sera *jamais* utilisée. Néanmoins,
une URL comme ``/blog/my-blog-post`` correspondra à la première route (``blog``)
qui retournera une valeur (n'ayant aucun sens) ``my-blog-post`` au paramètre
``{page}``.

+--------------------+-------+-----------------------+
| URL                | route | paramètres            |
+====================+=======+=======================+
| /blog/2            | blog  | {page} = 2            |
+--------------------+-------+-----------------------+
| /blog/my-blog-post | blog  | {page} = my-blog-post |
+--------------------+-------+-----------------------+

La réponse au problème est d'ajouter des conditions requises à la route. Les
routes de l'exemple ci-dessus fonctionneraient parfaitement si le pattern
``/blog/{page}`` correspondait *uniquement* aux URLs avec la partie ``{page}``
étant un nombre entier. Heureusement, des conditions requises sous forme
d'expression régulière peuvent être facilement ajoutées pour chaque paramètre.
Par exemple :

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }
            requirements:
                page:  \d+

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
                <requirement key="page">\d+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page' => 1,
        ), array(
            'page' => '\d+',
        )));

        return $collection;

La condition requise ``\d+`` est une expression régulière qui oblige la valeur
du paramètre ``{page}`` à être un nombre (composé d'un ou plusieurs chiffres).
La route ``blog`` correspondra toujours à une URL comme ``/blog/2`` (parce
que 2 est un nombre), mais elle ne correspondra par contre plus à une URL comme
``/blog/my-blog-post`` (car ``my-blog-post`` n'est *pas* un nombre).

Comme résultat, une URL comme ``/blog/my-blog-post`` va dorénavant correspondre
correctement à la route ``blog_show``.

+--------------------+-----------+-----------------------+
| URL                | route     | paramètres            |
+====================+===========+=======================+
| /blog/2            | blog      | {page} = 2            |
+--------------------+-----------+-----------------------+
| /blog/my-blog-post | blog_show | {slug} = my-blog-post |
+--------------------+-----------+-----------------------+

.. sidebar:: Les Routes précédentes Gagnent toujours

    Tout cela signifie que l'ordre des routes est très important. Si la
    route ``blog_show`` était placée au-dessus de la route ``blog``, l'URL
    ``/blog/2`` correspondrait à ``blog_show`` au lieu de ``blog`` puisque
    le paramètre {slug}`` de ``blog_show`` n'a pas de conditions requises.
    En utilisant un ordre clair et intelligent, vous pouvez accomplir tout
    ce que vous voulez.

Avec la possibilité de définir des conditions requises pour les paramètres à
l'aide d'expressions régulières, la complexité et la flexibilité de chaque
condition est entièrement dépendante de ce que vous en faites. Supposez que
la page d'accueil de votre application soit disponible en deux langues
différentes, basée sur l'URL :

.. configuration-block::

    .. code-block:: yaml

        homepage:
            pattern:   /{culture}
            defaults:  { _controller: AcmeDemoBundle:Main:homepage, culture: en }
            requirements:
                culture:  en|fr

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="homepage" pattern="/{culture}">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
                <default key="culture">en</default>
                <requirement key="culture">en|fr</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('homepage', new Route('/{culture}', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
            'culture' => 'en',
        ), array(
            'culture' => 'en|fr',
        )));

        return $collection;

Pour les requêtes entrantes, la partie ``{culture}`` de l'URL est comparée à
l'expression régulière ``(en|fr)``.

+-----+-------------------------------------+
| /   | {culture} = en                      |
+-----+-------------------------------------+
| /en | {culture} = en                      |
+-----+-------------------------------------+
| /fr | {culture} = fr                      |
+-----+-------------------------------------+
| /es | *ne correspondra pas à cette route* |
+-----+-------------------------------------+

.. index::
   single: Routing; Method requirement

Ajouter des Conditions Requises pour la Méthode HTTP
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En plus de l'URL, vous pouvez aussi comparer la *méthode* (c-a-d GET, HEAD,
POST, PUT, DELETE) de la requête entrante avec celle définie dans les
conditions requises de la route. Supposez que vous ayez un formulaire de
contact avec deux contrôleurs - un pour afficher le formulaire (quand on
a une requête GET) et un pour traiter le formulaire une fois qu'il a été
soumis (avec une requête POST). Ceci peut être accompli avec la configuration
de routage suivante :

.. configuration-block::

    .. code-block:: yaml

        contact:
            pattern:  /contact
            defaults: { _controller: AcmeDemoBundle:Main:contact }
            requirements:
                _method:  GET

        contact_process:
            pattern:  /contact
            defaults: { _controller: AcmeDemoBundle:Main:contactProcess }
            requirements:
                _method:  POST

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact" pattern="/contact">
                <default key="_controller">AcmeDemoBundle:Main:contact</default>
                <requirement key="_method">GET</requirement>
            </route>

            <route id="contact_process" pattern="/contact">
                <default key="_controller">AcmeDemoBundle:Main:contactProcess</default>
                <requirement key="_method">POST</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contact',
        ), array(
            '_method' => 'GET',
        )));

        $collection->add('contact_process', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contactProcess',
        ), array(
            '_method' => 'POST',
        )));

        return $collection;

Malgré le fait que ces deux routes aient des patterns identiques (``/contact``),
la première route correspondra uniquement aux requêtes GET et la seconde route
correspondra seulement aux requêtes POST. Cela signifie que vous pouvez
afficher le formulaire et le soumettre via la même URL, tout en utilisant
des contrôleurs distincts pour les deux actions.

.. note::
    Si aucune condition requise ``_method`` n'est spécifiée, la route
    correspondra à *toutes* les méthodes.

Comme les autres, la condition requise ``_method`` est analysée en tant
qu'expression régulière. Pour faire correspondre les requêtes à la méthode
``GET`` *ou* à ``POST``, vous pouvez utiliser ``GET|POST``.

.. index::
   single: Routing; Advanced example
   single: Routing; _format parameter

.. _advanced-routing-example:

Exemple de Routage Avancé
~~~~~~~~~~~~~~~~~~~~~~~~~

A ce point, vous connaissez tout ce dont vous avez besoin pour créer une
structure de routage puissante dans Symfony. Ce qui suit est un exemple
montrant simplement à quel point le système de routage peut être flexible :

.. configuration-block::

    .. code-block:: yaml

        article_show:
          pattern:  /articles/{culture}/{year}/{title}.{_format}
          defaults: { _controller: AcmeDemoBundle:Article:show, _format: html }
          requirements:
              culture:  en|fr
              _format:  html|rss
              year:     \d+

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="article_show" pattern="/articles/{culture}/{year}/{title}.{_format}">
                <default key="_controller">AcmeDemoBundle:Article:show</default>
                <default key="_format">html</default>
                <requirement key="culture">en|fr</requirement>
                <requirement key="_format">html|rss</requirement>
                <requirement key="year">\d+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('homepage', new Route('/articles/{culture}/{year}/{title}.{_format}', array(
            '_controller' => 'AcmeDemoBundle:Article:show',
            '_format' => 'html',
        ), array(
            'culture' => 'en|fr',
            '_format' => 'html|rss',
            'year' => '\d+',
        )));

        return $collection;

Comme vous l'avez vu, cette route correspondra uniquement si la partie
``{culture}`` de l'URL est ``en`` ou ``fr`` et si ``{year}`` est
un nombre. Cette route montre aussi comment vous pouvez utiliser un point
entre les paramètres de substitution à la place d'un slash. Les URLs
qui correspondent à cette route pourraient ressembler à ça :

* ``/articles/en/2010/my-post``
* ``/articles/fr/2010/my-post.rss``

.. _book-routing-format-param:

.. sidebar:: Le Paramètre Spécial de Routage ``_format``

    Cet exemple met aussi en valeur le paramètre spécial de routage ``_format``.
    Lorsque vous utilisez ce paramètre, la valeur correspondante devient alors
    le « format de la requête » de l'objet ``Request``. Finalement, le format de
    la requête est utilisé pour des taches comme spécifier le ``Content-Type``
    de la réponse (par exemple : un format de requête ``json`` se traduit
    en un ``Content-Type`` ayant pour valeur ``application/json``). Il peut
    aussi être utilisé dans le contrôleur pour délivrer un template différent
    pour chaque valeur de ``_format``. Le paramètre ``_format`` est une
    manière très puissante de délivrer le même contenu dans différents formats.


Paramètres spéciaux de routing
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Comme vous l'avez vu, chaque paramètre ou valeur par défaut peut être passé comme
argument à la méthode contrôleur. De plus, il y a 3 paramètres spéciaux : chacun
d'eux apporte une fonctionnalité unique à votre application :

* ``_controller``: Comme vous l'avez vu, ce paramètre est utilisé pour déterminer
  quel contrôleur est éxecuté lorsque l'URL est reconnue ;
* ``_format``: Utilisé pour définir le format de la requête(:ref:`en savoir plus<book-routing-format-param>`) ;
* ``_locale``: Utilisé pour définir la locale de la session (:ref:`en savoir plus<book-translation-locale-url>`).

.. tip::

    Si vous utilisez le paramètre ``_locale`` dans une route, cette valeur sera
    également stockée en session pour que les futures requêtes la conservent.

.. index::
   single: Routing; Controllers
   single: Controller; String naming format

.. _controller-string-syntax:

Pattern de Nommage du Contrôleur
--------------------------------

Chaque route doit avoir un paramètre ``_controller``, qui lui dicte quel
contrôleur doit être exécuté lorsque cette route correspond à l'URL.
Ce paramètre utilise un pattern de chaîne de caractères simple appelé
*nom logique du contrôleur*, que Symfony fait correspondre à une méthode
et à une classe PHP spécifique. Le pattern a trois parties, chacune
séparée par deux-points :

    **bundle**:**contrôleur**:**action**

Par exemple, la valeur ``AcmeBlogBundle:Blog:show`` pour le paramètre
``_controller`` signifie :

+----------------+----------------------+-------------------+
| Bundle         | Classe du Contrôleur | Nom de la Méthode |
+================+======================+===================+
| AcmeBlogBundle | BlogController       | showAction        |
+----------------+----------------------+-------------------+

Le contrôleur pourrait ressembler à quelque chose comme ça::

    // src/Acme/BlogBundle/Controller/BlogController.php    
    namespace Acme\BlogBundle\Controller;
    
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    
    class BlogController extends Controller
    {
        public function showAction($slug)
        {
            // ...
        }
    }

Notez que Symfony ajoute la chaîne de caractères ``Controller`` au nom de la
classe (``Blog`` => ``BlogController``) et ``Action`` au nom de la méthode
(``show`` => ``showAction``).

Vous pourriez aussi faire référence à ce contrôleur en utilisant le nom complet
de sa classe et de sa méthode : ``Acme\BlogBundle\Controller\BlogController::showAction``.
Mais si vous suivez quelques conventions simples, le nom logique est plus
concis et permet aussi plus de flexibilité.

.. note::

   En plus d'utiliser le nom logique ou le nom complet de la classe, Symfony
   supporte une troisième manière de référer à un contrôleur. Cette méthode
   utilise un seul séparateur deux-points (par exemple : ``service_name:indexAction``)
   et réfère au contrôleur en tant que service (see :doc:`/cookbook/controller/service`).

Les Paramètres de la Route et les Arguments du Contrôleur
---------------------------------------------------------

Les paramètres de la route (par exemple : ``{slug}``) sont spécialement
importants parce que chacun d'entre eux est mis à disposition en tant
qu'argument de la méthode contrôleur::

    public function showAction($slug)
    {
      // ...
    }

En réalité, la collection entière ``defaults`` est fusionnée avec les valeurs
des paramètres afin de former un unique tableau. Chaque clé du tableau est
disponible en tant qu'argument dans le contrôleur.

En d'autres termes, pour chaque argument de votre méthode contrôleur, Symfony
recherche un paramètre de la route avec ce nom et assigne sa valeur à cet
argument. Dans l'exemple avancé ci-dessus, n'importe quelle combinaison (dans
n'importe quel ordre) des variable suivantes pourrait être utilisée en tant
qu'arguments de la méthode ``showAction()`` :

* ``$culture``
* ``$year``
* ``$title``
* ``$_format``
* ``$_controller``

Sachant que les paramètres de substitution et la collection ``defaults`` sont
fusionnés ensemble, même la variable ``$_controller`` est disponible. Pour une
discussion plus détaillée sur le sujet, lisez :ref:`route-parameters-controller-arguments`.

.. tip::

    Vous pouvez aussi utiliser une variable ``$_route`` spéciale, qui est
    définie comme étant le nom de la route qui a correspondu.

.. index::
   single: Routing; Importing routing resources

.. _routing-include-external-resources:

Inclure des Ressources Externes de Routage
------------------------------------------

Toutes les routes sont chargées via un unique fichier de configuration - généralement
``app/config/routing.yml`` (voir `Créer des Routes`_ ci-dessus). Cependant, la plupart
du temps, vous voudrez charger les routes depuis d'autres endroits, comme un fichier de
routage qui se trouve dans un bundle. Ceci peut être fait en « important » ce fichier :

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"));

        return $collection;

.. note::

   Lorsque vous importez des ressources depuis YAML, la clé (par exemple :
   ``acme_hello``) n'a pas de sens précis. Assurez-vous simplement que cette
   dernière soit unique afin qu'aucune autre ligne ne la surcharge.

La clé de la ``ressource`` charge la ressource de routage donnée. Dans cet
exemple, la ressource est le répertoire entier d'un fichier, où la syntaxe
raccourcie ``@AcmeHelloBundle`` est traduite par le répertoire de ce bundle.
Le fichier importé pourrait ressembler à quelque chose comme ça :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/routing.yml
       acme_hello:
            pattern:  /hello/{name}
            defaults: { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="acme_hello" pattern="/hello/{name}">
                <default key="_controller">AcmeHelloBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('acme_hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

        return $collection;

Les routes de ce fichier sont analysées et chargées de la même manière que
pour le fichier de routage principal.

Préfixer les Routes Importées
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez aussi choisir de définir un « préfixe » pour les routes importées. Par
exemple, supposez que vous vouliez que la route ``acme_hello`` ait un pattern
final ``/admin/hello/{name}`` à la place de simplement ``/hello/{name}`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"
            prefix:   /admin

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" prefix="/admin" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"), '/admin');

        return $collection;

La chaîne de caractères ``/admin`` sera maintenant ajoutée devant le pattern
de chaque route chargée depuis la nouvelle ressource de routage.

.. tip::

    Vous pouvez également définir les routes en utilisant les annotations.
    Lisez la :doc:`documentation du FrameworkExtraBundle</bundles/SensioFrameworkExtraBundle/annotations/routing>`
    pour savoir comment faire.

.. index::
   single: Routing; Debugging

Visualiser et Debugger les Routes
---------------------------------

Lorsque vous ajoutez et personnalisez des routes, cela aide beaucoup de
pouvoir visualiser et d'avoir des informations détaillées à propos de ces
dernières. Une manière géniale de voir chaque route de votre application est
d'utiliser la commande de la console ``router:debug``. Exécutez la commande
suivante depuis la racine de votre projet.

.. code-block:: bash

    $ php app/console router:debug

La commande va retourner une liste de *toutes* les routes configurées
dans votre application :

.. code-block:: text

    homepage              ANY       /
    contact               GET       /contact
    contact_process       POST      /contact
    article_show          ANY       /articles/{culture}/{year}/{title}.{_format}
    blog                  ANY       /blog/{page}
    blog_show             ANY       /blog/{slug}

Vous pouvez aussi avoir des informations très spécifiques pour une seule
route en incluant le nom de cette dernière après la commande :

.. code-block:: bash

    $ php app/console router:debug article_show

.. versionadded:: 2.1
    La commande ``router:match`` a été ajoutée dans Symfony 2.1

Vous pouvez vérifier quelle route, s'il y en a, correspond à un chemin
avec la commande ``router:match`` :

.. code-block:: bash

    $ php app/console router:match /articles/en/2012/article.rss
    Route "article_show" matches

.. index::
   single: Routing; Generating URLs

Générer des URLs
----------------

Le système de routage devrait aussi être utilisé pour générer des URLs. En
réalité, le routage est un système bi-directionnel : faire correspondre une
URL à un contrôleur+paramètres et une route+paramètres à une URL. Les méthodes
:method:`Symfony\\Component\\Routing\\Router::match` et
:method:`Symfony\\Component\\Routing\\Router::generate` forment ce système
bi-directionnel. Prenez l'exemple de la route ``blog_show`` vue plus haut::

    $params = $router->match('/blog/my-blog-post');
    // array('slug' => 'my-blog-post', '_controller' => 'AcmeBlogBundle:Blog:show')

    $uri = $router->generate('blog_show', array('slug' => 'my-blog-post'));
    // /blog/my-blog-post

Pour générer une URL, vous avez besoin de spécifier le nom de la route (par
exemple : ``blog_show``) ainsi que quelconque joker (par exemple :
``slug = my-blog-post``) utilisé dans le pattern de cette route. Avec cette
information, n'importe quelle URL peut être générée facilement::

    class MainController extends Controller
    {
        public function showAction($slug)
        {
          // ...

          $url = $this->get('router')->generate('blog_show', array('slug' => 'my-blog-post'));
        }
    }

Dans une prochaine section, vous apprendrez comment générer des URLs directement
depuis les templates.

.. tip::

    Si le front de votre application utilise des requêtes AJAX, vous voudrez sûrement
    être capable de générer des URLs en javascript en vous basant sur votre configuration.
    En utilisant le `FOSJsRoutingBundle`_, vous pourrez faire cela :

    .. code-block:: javascript
	
        var url = Routing.generate('blog_show', { "slug": 'my-blog-post'});

    Pour plus d'informations, lisez la documentation du bundle.

.. index::
   single: Routing; Absolute URLs

Générer des URLs Absolues
~~~~~~~~~~~~~~~~~~~~~~~~~

Par défaut, le routeur va générer des URLs relatives (par exemple : ``/blog``).
Pour générer une URL absolue, passez simplement ``true`` comme troisième argument
de la méthode ``generate()``::

    $router->generate('blog_show', array('slug' => 'my-blog-post'), true);
    // http://www.example.com/blog/my-blog-post

.. note::

    L'host qui est utilisé lorsque vous générez une URL absolue est celui
    de l'objet courant ``Request``. Celui-ci est détecté automatiquement basé
    sur les informations du serveur fournies par PHP. Lorsque vous générez
    des URLs absolues pour des scripts exécutés depuis la ligne de commande,
    vous devrez spécifier manuellement l'host désiré sur l'objet ``RequestContext``::
    
        $router->getContext()->setHost('www.example.com');

.. index::
   single: Routing; Generating URLs in a template

Générer des URLs avec « Query Strings »
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La méthode ``generate`` prend un tableau de valeurs jokers pour générer l'URI.
Mais si vous en passez d'autres, elles seront ajoutées à l'URI en tant que
« query string » ::

    $router->generate('blog', array('page' => 2, 'category' => 'Symfony'));
    // /blog/2?category=Symfony

Générer des URLs depuis un template
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'endroit principal où vous générez une URL est depuis un template lorsque vous
créez des liens entre les pages de votre application. Cela se fait comme on l'a
vu auparavant, mais en utilisant une fonction d'aide (helper) du template :

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('blog_show', { 'slug': 'my-blog-post' }) }}">
          Lire cette entrée blog.
        </a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('blog_show', array('slug' => 'my-blog-post')) ?>">
            Lire cette entrée blog.
        </a>

Des URLs absolues peuvent aussi être générées.

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ url('blog_show', { 'slug': 'my-blog-post' }) }}">
            Lire cette entrée blog.
        </a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('blog_show', array('slug' => 'my-blog-post'), true) ?>">
            Lire cette entrée blog.
        </a>

Résumé
------

Le routage est un système permettant de faire correspondre l'URL des requêtes
entrantes à une fonction contrôleur qui devrait être appelée pour traiter la
requête. Cela vous permet d'une part de spécifier de belles URLs et d'autre part
de conserver la fonctionnalité de votre application découplée de ces URLs. Le
routage est un mécanisme bi-directionnel, ce qui signifie qu'il doit aussi
être utilisé pour générer des URLs.

En savoir plus grâce au Cookbook
--------------------------------

* :doc:`/cookbook/routing/scheme`

.. _`FOSJsRoutingBundle`: https://github.com/FriendsOfSymfony/FOSJsRoutingBundle
