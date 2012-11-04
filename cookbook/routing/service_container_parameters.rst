.. index::
   single: Routing; Service Container Parameters

Comment utiliser des paramètres du conteneur de services dans vos routes
========================================================================

.. versionadded:: 2.1
    La possibilité d'utiliser ces paramètres dans vos routes est une
    nouveauté de Symfony 2.1.

Parfois, vous pouvez trouver utile de rendre certaines parties de vos
routes configurables de façon globale. Par exemple, si vous construisez
un site internationalisé, vous commencerez probablement pas une ou deux
locales. Vous ajouterez certainement un prérequis dans vos routes pour
empêcher l'utilisateur d'accéder à une autre locale que celles que vous
supportez.

Vous *pourriez* coder en dure votre prérequis ``_locale`` dans toutes vos
routes. Mais une meilleure solution sera d'utiliser un paramètre configurable
du conteneur de services dans la configuration de votre routage :

.. configuration-block::

    .. code-block:: yaml

        contact:
            pattern:  /{_locale}/contact
            defaults: { _controller: AcmeDemoBundle:Main:contact }
            requirements:
                _locale: %acme_demo.locales%

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact" pattern="/{_locale}/contact">
                <default key="_controller">AcmeDemoBundle:Main:contact</default>
                <requirement key="_locale">%acme_demo.locales%</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/{_locale}/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contact',
        ), array(
            '_locale' => '%acme_demo.locales%',
        )));

        return $collection;

Vous pouvez maintenant contrôler et définir le paramètre ``acme_demo.locales``
quelque part dans votre conteneur :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            acme_demo.locales: en|es

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="acme_demo.locales">en|es</parameter>
        </parameters>

    .. code-block:: php

        # app/config/config.php
        $container->setParameter('acme_demo.locales', 'en|es');

Vous pouvez aussi utiliser un paramètre pour définir votre schéma de route
(ou une partie de votre schéma) :

.. configuration-block::

    .. code-block:: yaml

        some_route:
            pattern:  /%acme_demo.route_prefix%/contact
            defaults: { _controller: AcmeDemoBundle:Main:contact }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="some_route" pattern="/%acme_demo.route_prefix%/contact">
                <default key="_controller">AcmeDemoBundle:Main:contact</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('some_route', new Route('/%acme_demo.route_prefix%/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contact',
        )));

        return $collection;

.. note::

    Tout comme dans les fichiers de configuration classiques du conteneur, si vous
    avez besoin d'un ``%`` dans votre route, vous pouvez échapper le signe popurcentage
    en le doublant. Par exemple, ``/score-50%%`` deviendra ``/score-50%``.