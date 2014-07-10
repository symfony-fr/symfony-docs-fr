.. index::
   single: Routing; Matching on Hostname

Comment faire correspondre une route en se basant sur l'hôte
============================================================

Vous pouvez aussi vous baser sur l'*hôte* HTTP de la requête entrante.

.. configuration-block::

    .. code-block:: yaml

        mobile_homepage:
            path:     /
            host:     m.example.com
            defaults: { _controller: AcmeDemoBundle:Main:mobileHomepage }

        homepage:
            path:     /
            defaults: { _controller: AcmeDemoBundle:Main:homepage }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd"
        >

            <route id="mobile_homepage" path="/" host="m.example.com">
                <default key="_controller">AcmeDemoBundle:Main:mobileHomepage</default>
            </route>

            <route id="homepage" path="/">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('mobile_homepage', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:mobileHomepage',
        ), array(), array(), 'm.example.com'));

        $collection->add('homepage', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
        )));

        return $collection;

Les deux routes correspondent au même chemin ``/``, cependant, la première ne correspondra
que dans le cas où l'hôte sera ``m.example.com``.

Utiliser des variables
----------------------

Cette option utilise la même syntaxe que celle utilisée pour les chemins.
Cela signifie que vous pouvez utiliser des variables comme noms d'hôte:

.. configuration-block::

    .. code-block:: yaml

        projects_homepage:
            path:     /
            host:     "{project_name}.example.com"
            defaults: { _controller: AcmeDemoBundle:Main:mobileHomepage }

        homepage:
            path:     /
            defaults: { _controller: AcmeDemoBundle:Main:homepage }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd"
        >

            <route id="projects_homepage" path="/" host="{project_name}.example.com">
                <default key="_controller">AcmeDemoBundle:Main:mobileHomepage</default>
            </route>

            <route id="homepage" path="/">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('project_homepage', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:mobileHomepage',
        ), array(), array(), '{project_name}.example.com'));

        $collection->add('homepage', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
        )));

        return $collection;

Vous pouvez également définir des prérequis et des options par défaut pour ces
variables. Par exemple, si vous voulez faire correspondre ``m.example.com`` et
``mobile.example.com``, vous pouvez procéder comme ceci:

.. configuration-block::

    .. code-block:: yaml

        mobile_homepage:
            path:     /
            host:     "{subdomain}.example.com"
            defaults:
                _controller: AcmeDemoBundle:Main:mobileHomepage
                subdomain: m
            requirements:
                subdomain: m|mobile

        homepage:
            path:     /
            defaults: { _controller: AcmeDemoBundle:Main:homepage }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd"
        >

            <route id="mobile_homepage" path="/" host="{subdomain}.example.com">
                <default key="_controller">AcmeDemoBundle:Main:mobileHomepage</default>
                <default key="subdomain">m</default>

                <requirement key="subdomain">m|mobile</requirement>
            </route>

            <route id="homepage" path="/">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('mobile_homepage', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:mobileHomepage',
            'subdomain'   => 'm',
        ), array(
            'subdomain' => 'm|mobile',
        ), array(), '{subdomain}.example.com'));

        $collection->add('homepage', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
        )));

        return $collection;

.. tip::

    Vous pouvez aussi utiliser des paramètres de service si vous ne voulez
    pas coder le nom d'hôte en dur:

    .. configuration-block::

        .. code-block:: yaml

            mobile_homepage:
                path:     /
                host:     "m.{domain}"
                defaults:
                    _controller: AcmeDemoBundle:Main:mobileHomepage
                    domain: "%domain%"
                requirements:
                    domain: "%domain%"

            homepage:
                path:  /
                defaults: { _controller: AcmeDemoBundle:Main:homepage }

        .. code-block:: xml

            <?xml version="1.0" encoding="UTF-8" ?>

            <routes xmlns="http://symfony.com/schema/routing"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

                <route id="mobile_homepage" path="/" host="m.{domain}">
                    <default key="_controller">AcmeDemoBundle:Main:mobileHomepage</default>
                    <default key="domain">%domain%</default>
                    <requirement key="domain">%domain%</requirement>
                </route>

                <route id="homepage" path="/">
                    <default key="_controller">AcmeDemoBundle:Main:homepage</default>
                </route>
            </routes>

        .. code-block:: php

            use Symfony\Component\Routing\RouteCollection;
            use Symfony\Component\Routing\Route;

            $collection = new RouteCollection();
            $collection->add('mobile_homepage', new Route('/', array(
                '_controller' => 'AcmeDemoBundle:Main:mobileHomepage',
                'domain' => '%domain%',
            ), array(
                'domain' => '%domain%',
            ), array(), 'm.{domain}'));

            $collection->add('homepage', new Route('/', array(
                '_controller' => 'AcmeDemoBundle:Main:homepage',
            )));

            return $collection;

.. tip::

    Assurez-vous également d'inclure une option par défaut pour la variable
    ``domain``. Autrement, vous devrez spécifier une valeur à chaque fois que vous
    génèrerez une URL en utilisant cette route.

.. _component-routing-host-imported:

Utiliser la correspondance d'hôte de routes importées
-----------------------------------------------------

Vous pouvez également définir cette option sur des routes importées:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"
            host:     "hello.example.com"

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" host="hello.example.com" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"), '', array(), array(), array(), 'hello.example.com');

        return $collection;

L'hôte ``hello.example.com`` sera défini pour chaque route chargée depuis la nouvelle ressource
de routage.

Tester vos contrôleurs
----------------------

Vous devez définir l'entête HTTP Host sur vos objets de requêtes si vous voulez
tester fonctionnellement vos routes.

.. code-block:: php

    $crawler = $client->request(
        'GET',
        '/homepage',
        array(),
        array(),
        array('HTTP_HOST' => 'm.' . $client->getContainer()->getParameter('domain'))
    );