.. index::
   single: Routing; _method

Comment utiliser des méthodes HTTP autres que GET et POST dans les routes
=========================================================================

La méthode HTTP d'une requête est l'un des prérequis qui peuvent être
vérifiés pour valider si une route correspond ou pas. Cela est abordé dans le
chapitre sur le routage du Book « :doc:`/book/routing` » avec des exemples
qui utilisent les méthodes GET et POST. Vous pouvez également utiliser d'autres
méthodes HTTP de la même manière. Par exemple, si vous avez un billet de blog, alors
vous pourriez utiliser le même schéma d'URL pour l'afficher, le modifier et le supprimer
grâce aux méthodes GET, PUT et DELETE.

.. configuration-block::

    .. code-block:: yaml

        blog_show:
            pattern:  /blog/{slug}
            defaults: { _controller: AcmeDemoBundle:Blog:show }
            requirements:
                _method:  GET

        blog_update:
            pattern:  /blog/{slug}
            defaults: { _controller: AcmeDemoBundle:Blog:update }
            requirements:
                _method:  PUT

        blog_delete:
            pattern:  /blog/{slug}
            defaults: { _controller: AcmeDemoBundle:Blog:delete }
            requirements:
                _method:  DELETE

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" pattern="/blog/{slug}">
                <default key="_controller">AcmeDemoBundle:Blog:show</default>
                <requirement key="_method">GET</requirement>
            </route>

            <route id="blog_update" pattern="/blog/{slug}">
                <default key="_controller">AcmeDemoBundle:Blog:update</default>
                <requirement key="_method">PUT</requirement>
            </route>

            <route id="blog_delete" pattern="/blog/{slug}">
                <default key="_controller">AcmeDemoBundle:Blog:delete</default>
                <requirement key="_method">DELETE</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog_show', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeDemoBundle:Blog:show',
        ), array(
            '_method' => 'GET',
        )));

        $collection->add('blog_update', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeDemoBundle:Blog:update',
        ), array(
            '_method' => 'PUT',
        )));

        $collection->add('blog_delete', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeDemoBundle:Blog:delete',
        ), array(
            '_method' => 'DELETE',
        )));

        return $collection;

Malheureusement, la vie n'est pas toujours si simple puisque plusieurs
navigateurs ne supportent pas l'envoi de requêtes PUT et DELETE.
Heureusement, Symfony2 vous fournit de manière simple de contourner cette
limitation. En incluant le paramètre ``_method`` dans la chaîne de caractères
de la requête, ou dans les paramètres d'une requête HTTP, Symfony l'utilisera
comme méthode pour trouver une route correspondante. Cela peut être fait très
facilement dans les formulaires grâce à un champ caché. Supposons que vous
ayez un formulaire pour éditer un billet de blog :

.. code-block:: html+jinja

    <form action="{{ path('blog_update', {'slug': blog.slug}) }}" method="post">
        <input type="hidden" name="_method" value="PUT" />
        {{ form_widget(form) }}
        <input type="submit" value="Update" />
    </form>

La requête soumise correspondra maintenant à la route ``blog_update`` et l'action
``updateAction`` sera utilisée pour traiter le formulaire.

De la même manière, le formulaire de suppression peut être modifié pour ressembler
à ceci :

.. code-block:: html+jinja

    <form action="{{ path('blog_delete', {'slug': blog.slug}) }}" method="post">
        <input type="hidden" name="_method" value="DELETE" />
        {{ form_widget(delete_form) }}
        <input type="submit" value="Delete" />
    </form>

Alors, la route ``blog_delete`` sera utilisée.