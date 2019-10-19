.. index::
    single: Routage; Informations simplémentaires

Comment passer des informations d'une route à un contrôleur
===========================================================

Les paramètres présents à l'intérieur de la collection ``defaults`` ne
doivent pas spécialement être en rapport avec un élément du ``path``. En
fait, vous pouvez utiliser le tableau ``defaults`` pour ajouter des
paramètres supplémentaires qui sont accessible depuis votre contrôleur.

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        blog:
            path:      /blog/{page}
            defaults:
                _controller: AppBundle:Blog:index
                page:        1
                title:       "Hello world!"

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" path="/blog/{page}">
                <default key="_controller">AppBundle:Blog:index</default>
                <default key="page">1</default>
                <default key="title">Hello world!</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AppBundle:Blog:index',
            'page'        => 1,
            'title'       => 'Hello world!',
        )));

        return $collection;

Vous avez maintenant accès à un paramètre supplémentaire dans votre
contrôleur::

    public function indexAction($page, $title)
    {
        // ...
    }

Comme vous pouvez le constater, la variable ``$title`` n'a jamais été définie
dans le path de la route, mais vous pouvez tout de même accéder à sa valeur
dans votre contrôleur.