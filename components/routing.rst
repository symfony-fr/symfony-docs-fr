.. index::
   single: Routing
   single: Components; Routing

Le Composant de Routage
=======================

    Le Composant de Routage fait correspondre une requête HTTP à un ensemble
    de variables de configuration.

Installation
------------

Vous pouvez installer le composant de différentes manières :

* Utilisez le dépôt Git officiel (https://github.com/symfony/Routing) ;
* Installez le via PEAR (`pear.symfony.com/Routing`) ;
* Installez le via Composer (`symfony/routing` dans Packagist).

Utilisation
-----------

Afin d'installer un système de routage basique, vous avez besoin de trois blocs :

* Une :class:`Symfony\\Component\\Routing\\RouteCollection`, qui contient les définitions des
  routes (instances de la classe :class:`Symfony\\Component\\Routing\\Route`)
* Une :class:`Symfony\\Component\\Routing\\RequestContext`, qui possède les informations
  concernant la requête
* Une classe :class:`Symfony\\Component\\Routing\\Matcher\\UrlMatcher`, qui s'occupe de la
  correspondance entre la requête et une route unique

Jetons un oeil à un exemple simple. Notez que ce dernier suppose que vous ayez déjà
configuré votre « autoloader » afin qu'il charge le composant de routage::

    use Symfony\Component\Routing\Matcher\UrlMatcher;
    use Symfony\Component\Routing\RequestContext;
    use Symfony\Component\Routing\RouteCollection;
    use Symfony\Component\Routing\Route;

    $routes = new RouteCollection();
    $routes->add('route_name', new Route('/foo', array('controller' => 'MyController')));

    $context = new RequestContext($_SERVER['REQUEST_URI']);

    $matcher = new UrlMatcher($routes, $context);

    $parameters = $matcher->match( '/foo' );
    // array('controller' => 'MyController', '_route' => 'route_name')

.. note::

    Faites attention lorsque vous utilisez ``$_SERVER['REQUEST_URI']``, car cette
    variable pourrait contenir quelconques paramètres de requête dans l'URL, ce
    qui causerait des problèmes avec la correspondance de la route. Une manière
    facile de résoudre cela est d'utiliser le composant « HttpFoundation » comme
    expliqué :ref:`ci-dessous<components-routing-http-foundation>`.

Vous pouvez ajouter autant de routes que vous le souhaitez dans une
:class:`Symfony\\Component\\Routing\\RouteCollection`.

La méthode :method:`RouteCollection::add()<Symfony\\Component\\Routing\\RouteCollection::add>`
prend deux arguments. Le premier est le nom de la route. Le deuxième est un
objet :class:`Symfony\\Component\\Routing\\Route`, qui s'attend à recevoir une URL et
tout tableau de variables personnalisées dans son constructeur. Ce tableau
peut être *n'importe quoi* qui ait du sens pour votre application, et est retourné
lorsque cette route correspond à la requête.

Si aucune correspondance de route ne peut être trouvée, une
:class:`Symfony\\Component\\Routing\\Exception\\ResourceNotFoundException`
sera lancée.

En plus de votre tableau de variables personnalisées, une clé ``_route``
qui contient le nom de la route correspondante est ajoutée

Définition des routes
~~~~~~~~~~~~~~~~~~~~~

Une définition du routage complète peut contenir jusqu'à quatre parties :

1. Le pattern de l'URL de la route. Une correspondance tente d'être effectuée entre la
route et l'URL passée au `RequestContext`, et peut contenir des valeurs de substitution
jokers nommées (par exemple : ``{placeholders}``) afin de faire correspondre les
parties dynamiques de l'URL.

2. Un tableau de valeurs par défaut. Ce dernier contient un tableau de
valeurs arbitraires qui seront retournées lorsque la requête correspond à
la route.

3. Un tableau de conditions requises. Ce dernier définit les contraintes concernant
le contenu des valeurs de substitution sous forme d'expressions régulières.

4. Un tableau d'options. Ce dernier contient des paramètres internes pour la
route et sont généralement ceux qui sont le moins souvent nécessaires.

Prenez la route suivante, qui combine plusieurs de ces idées::

   $route = new Route(
       '/archive/{month}', // chemin
       array('controller' => 'showArchive'), // valeurs par défaut
       array('month' => '[0-9]{4}-[0-9]{2}'), // conditions requises
       array() // options
   );

   // ...

   $parameters = $matcher->match('/archive/2012-01');
   // array('controller' => 'showArchive', 'month' => '2012-01', '_route' => '...')

   $parameters = $matcher->match('/archive/foo');
   // lance une ResourceNotFoundException

Dans ce cas, la route correspond avec l'URL ``/archive/2012-01``, car le joker
``{month}`` correspond à l'expression régulière donnée. Cependant, ``/archive/foo``
*ne* correspond *pas*, car « foo » n'a pas de correspondance avec le joker « {month} ».

En plus des contraintes dictées par les expressions régulières, il y a deux
conditions requises spécifiques que vous pouvez définir :

* ``_method`` impose une certaine méthode HTTP pour la requête (``HEAD``, ``GET``, ``POST``, ...)
* ``_scheme`` impose un certain schème HTTP (``http``, ``https``)

Par exemple, la route suivante ne va accepter que les requêtes vers « /foo »
avec une méthode POST et une connexion sécurisée::

   $route = new Route('/foo', array(), array('_method' => 'post', '_scheme' => 'https' ));

.. tip::

    Si vous voulez avoir une correspondance pour toutes les URLs qui commencent
    par un certain chemin et qui se terminent par un suffixe déterminé, vous
    pouvez utiliser la définition de route suivante::

        $route = new Route('/start/{suffix}', array('suffix' => ''), array('suffix' => '.*'));

Utiliser des préfixes
~~~~~~~~~~~~~~~~~~~~~

Vous pouvez ajouter des routes ou d'autres instances de
:class:`Symfony\\Component\\Routing\\RouteCollection` à une *autre* collection.
De cette façon, vous pouvez construire un arbre de routes. De plus, vous pouvez
définir un préfixe, des conditions requises par défaut ainsi que des options par
défaut pour toutes les routes d'un sous-arbre::

    $rootCollection = new RouteCollection();

    $subCollection = new RouteCollection();
    $subCollection->add( /*...*/ );
    $subCollection->add( /*...*/ );

    $rootCollection->addCollection($subCollection, '/prefix', array('_scheme' => 'https'));

Définir les paramètres de requête
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La classe :class:`Symfony\\Component\\Routing\\RequestContext` fournit des
informations à propos de la requête courante. Vous pouvez définir tous les
paramètres d'une requête HTTP avec cette classe via son constructeur::

    public function __construct($baseUrl = '', $method = 'GET', $host = 'localhost', $scheme = 'http', $httpPort = 80, $httpsPort = 443)

.. _components-routing-http-foundation:

Normalement, vous pouvez passer les valeurs depuis la variable ``$_SERVER`` afin de
fournir les données au :class:`Symfony\\Component\\Routing\\RequestContext`. Mais
si vous utilisez le composant :doc:`HttpFoundation</components/http_foundation/index>`,
vous pouvez vous servir de sa classe :class:`Symfony\\Component\\HttpFoundation\\Request`
pour récupérer le :class:`Symfony\\Component\\Routing\\RequestContext` via un
raccourci::

    use Symfony\Component\HttpFoundation\Request;

    $context = new RequestContext();
    $context->fromRequest(Request::createFromGlobals());

Générer une URL
~~~~~~~~~~~~~~~

Alors que la classe :class:`Symfony\\Component\\Routing\\Matcher\\UrlMatcher`
essaye de trouver une route qui corresponde à la requête donnée, vous pouvez
aussi construire une URL depuis une certaine route::

    use Symfony\Component\Routing\Generator\UrlGenerator;

    $routes = new RouteCollection();
    $routes->add('show_post', new Route('/show/{slug}'));

    $context = new RequestContext($_SERVER['REQUEST_URI']);

    $generator = new UrlGenerator($routes, $context);

    $url = $generator->generate('show_post', array(
        'slug' => 'my-blog-post'
    ));
    // /show/my-blog-post

.. note::

    Si vous avez défini la condition requise ``_scheme``, une URL absolue est
    générée si le schème du :class:`Symfony\\Component\\Routing\\RequestContext`
    courant ne respecte pas cette condition.

Charger des routes depuis un fichier
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous avez déjà vu comment vous pouvez ajouter facilement des routes à une
collection directement depuis PHP. Mais vous pouvez aussi charger des routes
depuis différents fichiers.

Le composant de Routage est fourni avec un certain nombre de classes de chargement,
chacune vous fournissant la possibilité de charger une collection de définitions
de route depuis un fichier externe d'un certain format.
Chaque chargeur attend une instance de :class:`Symfony\\Component\\Config\\FileLocator` en
tant qu'argument du constructeur. Vous pouvez utiliser le
:class:`Symfony\\Component\\Config\\FileLocator` pour définir un tableau de chemins dans
lequel le chargeur va chercher les fichiers requis.
Si le fichier est trouvé, le chargeur retourne une :class:`Symfony\\Component\\Routing\\RouteCollection`.

Si vous utilisez le chargeur ``YamlFileLoader``, alors les définitions de route ressemblent
à cela :

.. code-block:: yaml

    # routes.yml
    route1:
        pattern: /foo
        defaults: { controller: 'MyController::fooAction' }

    route2:
        pattern: /foo/bar
        defaults: { controller: 'MyController::foobarAction' }

Pour charger ce fichier, vous pouvez utiliser le code suivant. Cela suppose
que votre fichier ``routes.yml`` est dans le même répertoire que le code
ci-dessus::

    use Symfony\Component\Config\FileLocator;
    use Symfony\Component\Routing\Loader\YamlFileLoader;

    // cherche dans *ce* répertoire
    $locator = new FileLocator(array(__DIR__));
    $loader = new YamlFileLoader($locator);
    $collection = $loader->load('routes.yml');

En plus du chargeur :class:`Symfony\\Component\\Routing\\Loader\\YamlFileLoader`, il
y a d'autres chargeurs qui fonctionnent de la même manière :

* :class:`Symfony\\Component\\Routing\\Loader\\XmlFileLoader`
* :class:`Symfony\\Component\\Routing\\Loader\\PhpFileLoader`

Si vous utilisez le chargeur :class:`Symfony\\Component\\Routing\\Loader\\PhpFileLoader`,
vous devez fournir le nom d'un fichier PHP qui retourne une :class:`Symfony\\Component\\Routing\\RouteCollection`::

    // RouteProvider.php
    use Symfony\Component\Routing\RouteCollection;
    use Symfony\Component\Routing\Route;

    $collection = new RouteCollection();
    $collection->add('route_name', new Route('/foo', array('controller' => 'ExampleController')));
    // ...

    return $collection;

Des routes en tant que closures
...............................

Il y a aussi le chargeur :class:`Symfony\\Component\\Routing\\Loader\\ClosureLoader`, qui
appelle une closure et utilise son résultat en tant que :class:`Symfony\\Component\\Routing\\RouteCollection`::

    use Symfony\Component\Routing\Loader\ClosureLoader;

    $closure = function() {
        return new RouteCollection();
    };

    $loader = new ClosureLoader();
    $collection = $loader->load($closure);

Des Routes en tant qu'annotations
.................................

Enfin, il existe aussi le :class:`Symfony\\Component\\Routing\\Loader\\AnnotationDirectoryLoader`
et le :class:`Symfony\\Component\\Routing\\Loader\\AnnotationFileLoader` qui
permettent de charger des définitions de route depuis des annotations de classe.
Les détails spécifiques ne sont pas expliqués ici.

Le Routeur tout-en-un
~~~~~~~~~~~~~~~~~~~~~

La classe :class:`Symfony\\Component\\Routing\\Router` est un « package » tout-en-un
permettant d'utiliser rapidement le composant de Routage. Le constructeur s'attend
à recevoir une instance de chargeur, un chemin vers la définition principale des
routes et d'autres paramètres::

    public function __construct(LoaderInterface $loader, $resource, array $options = array(), RequestContext $context = null, array $defaults = array());

Avec l'option ``cache_dir``, vous pouvez activer le cache pour les routes (si
vous fournissez un chemin) ou désactiver le cache (si le paramètre est défini comme
``null``). Le mécanisme de cache est géré automatiquement en arrière-plan si vous
souhaitez l'utiliser. Un exemple basique de la classe
:class:`Symfony\\Component\\Routing\\Router` ressemblerait à cela::

    $locator = new FileLocator(array(__DIR__));
    $requestContext = new RequestContext($_SERVER['REQUEST_URI']);

    $router = new Router(
        new YamlFileLoader($locator),
        "routes.yml",
        array('cache_dir' => __DIR__.'/cache'),
        $requestContext,
    );
    $router->match('/foo/bar');

.. note::

    Si vous utilisez le cache, le composant de Routage va compiler de nouvelles
    classes qui seront sauvegardées dans le ``cache_dir``. Cela signifie que votre
    script doit avoir les permissions d'écriture nécessaires pour ce chemin.