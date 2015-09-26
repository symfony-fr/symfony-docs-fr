.. index::
    single: Routage; Redirection des URLs finissant par un slash

Redirection des URLs finissant par un slash
===========================================

Le but de ce cookbook est de présenter comment faire une redirection d'une
URL finissant par slash à la même URL sans le slash (par exemple ``/en/blog/`` à ``/en/blog``).

Pour cela vous devez créer un contrôleur qui va fonctionner avec toutes les
URLs finissant par un slash (en conservant les paramêtres de la requête si
il y en a) et qui redirige à la nouvelle URL avec un code HTTP 301::

    // src/AppBundle/Controller/RedirectingController.php
    namespace AppBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Request;

    class RedirectingController extends Controller
    {
        public function removeTrailingSlashAction(Request $request)
        {
            $pathInfo = $request->getPathInfo();
            $requestUri = $request->getRequestUri();

            $url = str_replace($pathInfo, rtrim($pathInfo, ' /'), $requestUri);

            return $this->redirect($url, 301);
        }
    }

Après cela, créez une route vers ce contrôleur qui fera référence à toute URL
finissant par un slash. Soyez certain que cette route est la dernière chargée
dans votre application.

.. configuration-block::

    .. code-block:: yaml

        remove_trailing_slash:
            path: /{url}
            defaults: { _controller: AppBundle:Redirecting:removeTrailingSlash }
            requirements:
                url: .*/$
            methods: [GET]

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing">
            <route id="remove_trailing_slash" path="/{url}" methods="GET">
                <default key="_controller">AppBundle:Redirecting:removeTrailingSlash</default>
                <requirement key="url">.*/$</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add(
            'remove_trailing_slash',
            new Route(
                '/{url}',
                array(
                    '_controller' => 'AppBundle:Redirecting:removeTrailingSlash',
                ),
                array(
                    'url' => '.*/$',
                ),
                array(),
                '',
                array(),
                array('GET')
            )
        );

.. note::

    Rediriger une requête POST ne fonctionne pas bien dans les anciens
    navigateurs. Une réponse 302 lors d'une requête en POST va
    transformer la requête en GET après la redirection pour des raisons
    historiques. Pour cette raison, la route présentée ici ne fonctionne
    qu'avec les requêtes GET.

.. caution::

    Soyez sûr d'inclure cette route à la toute fin de votre configuration
    de routes. Sinon, vous risquez de rediriger des véritables routes
    (y compris les routes du core de Symfony) qui ne finissent pas par un
    slash.