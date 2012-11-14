.. index::
   single: Routage; Autoriser un / dans un paramètre de route

Comment autoriser un caractère « / » dans un paramètre de route
===============================================================

Parfois, on a besoin de construire des URLs avec des paramètres qui
peuvent contenir un slash ``/``. Prenons par exemple la route classique
``/hello/{name}``. Par défaut, ``/hello/Fabien`` va correspondre à cette
route mais pas ``/hello/Fabien/Kris``. Cela est dû au fait que Symfony
utilise ce caractère comme séparateur entre les parties de la route.

Ce guide explique comment vous pouvez modifier une route afin que
``/hello/Fabien/Kris`` corresponde à la route ``/hello/{name}``, où ``{name}``
équivaut à ``Fabien/Kris``.

Configurer la Route
-------------------

Par défaut, les composants de routage de Symfony requièrent que les paramètres
correspondent au pattern de regex suivant : ``[^/]+``. Cela veut dire que tous
les caractères sont autorisés sauf ``/``.

Vous devez explicitement autoriser le caractère ``/`` à faire partie de votre
paramètre en spécifiant un pattern de regex plus permissif.

.. configuration-block::

    .. code-block:: yaml

        _hello:
            pattern: /hello/{name}
            defaults: { _controller: AcmeDemoBundle:Demo:hello }
            requirements:
                name: ".+"

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="_hello" pattern="/hello/{name}">
                <default key="_controller">AcmeDemoBundle:Demo:hello</default>
                <requirement key="name">.+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('_hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeDemoBundle:Demo:hello',
        ), array(
            'name' => '.+',
        )));

        return $collection;

    .. code-block:: php-annotations

        use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

        class DemoController
        {
            /**
             * @Route("/hello/{name}", name="_hello", requirements={"name" = ".+"})
             */
            public function helloAction($name)
            {
                // ...
            }
        }

C'est tout ! Maintenant, le paramètre ``{name}`` peut contenir le caractère ``/``.