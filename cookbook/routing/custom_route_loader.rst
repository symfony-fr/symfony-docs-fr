.. index::
    single: Routage; Chargeur de routes personnalisé

Comment créer un chargeur de routes personnalisé (custom loader)
================================================================

Qu'est-ce qu'un chargeur de routes personnalisé ?
-------------------------------------------------

Un chargeur de routes personnalisé vous permet de générer des
routes basées sur des conventions ou modèles. Un bon exemple
d'utilisation est le `FOSRestBundle`_ où les routes sont générées
en utilisant les noms des actions dans les contrôleurs.

Un chargeur de routes ne permet pas à votre bundle d'injecter des
routes sans modifier la configuration de routing (par exemple ``app/config/routing.yml``) manuellement.
Que votre bundle apporte des routes via un fichier de configuration
– comme le `WebProfilerBundle` fait – ou via un chargeur de routes
personnalisé – comme le `FOSRestBundle`_ fait – une entrée dans la
configuration du routing est toujours nécessaire.

.. note::

    Il y a plusieurs bundles qui utilisent leur propre chargeur de
    routes, par exemple `FOSRestBundle`_, `JMSI18nRoutingBundle`_,
    `KnpRadBundle`_ et `SonataAdminBundle`_.

Charger des routes
------------------

Dans une application Symfony, les routes sont chargées par la classe
:class:`Symfony\\Bundle\\FrameworkBundle\\Routing\\DelegatingLoader`.
Ce chargeur utilise d'autres chargeurs (il « délègue ») pour charger
depuis plusieurs types de ressources, par exemple les fichiers YAML
ou les annotations ``@Route`` et ``@Method`` dans les contrôleurs.
Le chargeur spécialisé implémente 
:class:`Symfony\\Component\\Config\\Loader\\LoaderInterface`
et possède donc deux méthodes importantes :
:method:`Symfony\\Component\\Config\\Loader\\LoaderInterface::supports`
et :method:`Symfony\\Component\\Config\\Loader\\LoaderInterface::load`.

Prenons ces lignes du fichier ``routing.yml`` de la
Symfony Standard Edition :

.. code-block:: yaml

    # app/config/routing.yml
    app:
        resource: @AppBundle/Controller/
        type:     annotation

Lorsque le premier chargeurs parse cela, il essaie tous les chargeur
enregistrés comme « déléguant » et appelle leur méthode
:method:`Symfony\\Component\\Config\\Loader\\LoaderInterface::supports`
avec la ressource (``@AppBundle/Controller/``) et le type (``annotation``)
en argument. Lorsque l'un des chargeurs retourne ``true``, sa méthode
:method:`Symfony\\Component\\Config\\Loader\\LoaderInterface::load` va 
être appelée, celle-ci doit retourner une :class:`Symfony\\Component\\Routing\\RouteCollection`
qui contient des objets :class:`Symfony\\Component\\Routing\\Route`.

Créer un chargeur personnalisé
------------------------------

Pour charger des routes depuis une source personnalisée (c-a-d
depuis quelque chose d'autre qu'annotations, YAML, ou XML), vous
devez créer un chargeur de routes personnalisé. Ce chargeur doit
implémenter la classe
:class:`Symfony\\Component\\Config\\Loader\\LoaderInterface`.

Dans la plupart des cas il vaut mieux ne pas implémenter 
:class:`Symfony\\Component\\Config\\Loader\\LoaderInterface`
par vous même mais étendre la classe
:class:`Symfony\\Component\\Config\\Loader\\Loader`.

L'exemple de chargeur suivant supporte le chargement de ressources de
routes avec le type ``extra``. Le type ``extra`` n'est pas important
– vous pouvez inventer le type de ressources que vous voulez. Le nom
de la ressource n'est pas utilisé dans l'exemple::

    // src/AppBundle/Routing/ExtraLoader.php
    namespace AppBundle\Routing;

    use Symfony\Component\Config\Loader\Loader;
    use Symfony\Component\Routing\Route;
    use Symfony\Component\Routing\RouteCollection;

    class ExtraLoader extends Loader
    {
        private $loaded = false;

        public function load($resource, $type = null)
        {
            if (true === $this->loaded) {
                throw new \RuntimeException('Do not add the "extra" loader twice');
            }

            $routes = new RouteCollection();

            // Préparation de la nouvelle route
            $path = '/extra/{parameter}';
            $defaults = array(
                '_controller' => 'AppBundle:Extra:extra',
            );
            $requirements = array(
                'parameter' => '\d+',
            );
            $route = new Route($path, $defaults, $requirements);

            // Ajout de la nouvelle route à la collection de routes
            $routeName = 'extraRoute';
            $routes->add($routeName, $route);

            $this->loaded = true;

            return $routes;
        }

        public function supports($resource, $type = null)
        {
            return 'extra' === $type;
        }
    }

Soyez sûr que le contrôleur que vous spécifiez existe vraiment.
Dans ce cas vous devez créer une méthode ``extraAction`` dans le
contrôleur ``ExtraController`` du bundle ``AppBundle``::

    // src/AppBundle/Controller/ExtraController.php
    namespace AppBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class ExtraController extends Controller
    {
        public function extraAction($parameter)
        {
            return new Response($parameter);
        }
    }

Définissez maintenant un service pour notre ``ExtraLoader`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            app.routing_loader:
                class: AppBundle\Routing\ExtraLoader
                tags:
                    - { name: routing.loader }

    .. code-block:: xml

        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="app.routing_loader" class="AppBundle\Routing\ExtraLoader">
                    <tag name="routing.loader" />
                </service>
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $container
            ->setDefinition(
                'app.routing_loader',
                new Definition('AppBundle\Routing\ExtraLoader')
            )
            ->addTag('routing.loader')
        ;

Remarquez le tag ``routing.loader``, tous les services qui possèdent
ce *tag* vont être marqués comme potentiel chargeur de routes et ajoutés
comme chargeur de routes spécifique au *service* ``routing.loader``, qui
est une instance de
:class:`Symfony\\Bundle\\FrameworkBundle\\Routing\\DelegatingLoader`.

Utiliser le chargeur personnalisé
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous n'avez rien fait d'autre, votre chargeur de route personnalisé
ne va *pas* être appelé. Vous devez ajouter quelques lignes à la
configuration du routing :

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        app_extra:
            resource: .
            type: extra

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="." type="extra" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import('.', 'extra'));

        return $collection;

La partie la plus importante est celle de la clé ``type``. Sa valeur
doit être « extra » car c'est le type supporté par le ``ExtraLoader``
et sans cela la méthode ``load()`` ne sera pas appelée. La clé
``resource`` est inutile pour le ``ExtraLoader`` donc la valeur "."
est spécifiée ici.

.. note::

    Les routes définies par un chargeur de routes personnalisé vont
    être automatiquement mises en cache par le framework. Donc
    lorsque vous modifiez quelque-chose dans la classe de chargement,
    n'oubliez pas de vider le cache.

Chargeurs plus avancés
----------------------

Si votre chargeur de routes étend de 
:class:`Symfony\\Component\\Config\\Loader\\Loader` comment
montré précédemment, vous pouvez aussi utiliser le résolveur fourni,
une instance de
:class:`Symfony\\Component\\Config\\Loader\\LoaderResolver`, pour
charger d'autres resources de routing.

Évidemment vous avez toujours besoin d'implémenter les méthodes
:method:`Symfony\\Component\\Config\\Loader\\LoaderInterface::supports`
et :method:`Symfony\\Component\\Config\\Loader\\LoaderInterface::load`.
Lorsque vous voulez charger une autre resource – par exemple un fichier
de configuration YAML ‑ vous pouvez appeler la méthode 
:method:`Symfony\\Component\\Config\\Loader\\Loader::import`::

    // src/AppBundle/Routing/AdvancedLoader.php
    namespace AppBundle\Routing;

    use Symfony\Component\Config\Loader\Loader;
    use Symfony\Component\Routing\RouteCollection;

    class AdvancedLoader extends Loader
    {
        public function load($resource, $type = null)
        {
            $collection = new RouteCollection();

            $resource = '@AppBundle/Resources/config/import_routing.yml';
            $type = 'yaml';

            $importedRoutes = $this->import($resource, $type);

            $collection->addCollection($importedRoutes);

            return $collection;
        }

        public function supports($resource, $type = null)
        {
            return 'advanced_extra' === $type;
        }
    }

.. note::

    Le nom de la ressource et le type du routing importé peut être
    n'importe quoi tant que c'est supporté par la configuration du
    chargeur de route (YAML, XML, PHP, annotation, etc.).

.. _`FOSRestBundle`: https://github.com/FriendsOfSymfony/FOSRestBundle
.. _`JMSI18nRoutingBundle`: https://github.com/schmittjoh/JMSI18nRoutingBundle
.. _`KnpRadBundle`: https://github.com/KnpLabs/KnpRadBundle
.. _`SonataAdminBundle`: https://github.com/sonata-project/SonataAdminBundle
