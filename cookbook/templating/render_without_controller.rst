.. index::
   single: Templating; Rendre un template sans créer de contrôleur

Comment rendre un template sans passer par un contrôleur
========================================================

Normalement, quand vous avez besoin de créer une page, vous devez créer
un contrôleur et rendre un template depuis ce contrôleur. Mais si vous
avez besoin d'afficher un simple template, qui ne nécessite pas de passage
de paramètre, vous pouvez vous passer complètement de la création d'un
contrôleur, en utilisant le contrôleur intégré ``FrameworkBundle:Template:template``.

Par exemple, supposons que vous vouliez afficher un template
``AcmeBundle:Static:privacy.html.twig``, qui ne nécessite aucun passage de variable.
Vous pouvez le faire sans créer de contrôleur.

.. configuration-block::

    .. code-block:: yaml

        acme_privacy:
            path: /privacy
            defaults:
                _controller: FrameworkBundle:Template:template
                template: 'AcmeBundle:Static:privacy.html.twig'

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="acme_privacy" path="/privacy">
                <default key="_controller">FrameworkBundle:Template:template</default>
                <default key="template">AcmeBundle:Static:privacy.html.twig</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('acme_privacy', new Route('/privacy', array(
            '_controller'  => 'FrameworkBundle:Template:template',
            'template'     => 'AcmeBundle:Static:privacy.html.twig',
        )));

        return $collection;

Le contrôleur ``FrameworkBundle:Template:template`` va simplement afficher le template
que vous aurez définit comme paramètre ``template``.

Vous pouvez aussi, bien sûr, utiliser cette astuce lors du rendu de contrôleurs
imbriqués dans un template. Le but de rendre un controleur depuis un template est
typiquement de préparer des données pour un contrôleur personalisé. C'est probablement
utile, uniquement, si vous voulez mettre partiellement en cache cette page (voir
:ref:`cookbook-templating-no-controller-caching`).

.. configuration-block::

    .. code-block:: html+jinja

        {{ render(url('acme_privacy')) }}

    .. code-block:: html+php

        <?php echo $view['actions']->render(
            $view['router']->generate('acme_privacy', array(), true)
        ) ?>

.. _cookbook-templating-no-controller-caching:

Mettre en cache les templates statiques
---------------------------------------

.. versionadded:: 2.2
    La possibilité de mettre en cache les templates rendu par ``FrameworkBundle:Template:template``
    est nouvelle en Symfony 2.2.

Puisque les templates affichés par cette méthode sont souvent statiques, il
pourrait être judicieux de les mettre en cache. Par chance, c'est facile!
En configurant quelques variables dans votre route, vous pourrez finement contrôler
la façon dont vos pages sont mise en cache:

.. configuration-block::

    .. code-block:: yaml

        acme_privacy:
            path: /privacy
            defaults:
                _controller: FrameworkBundle:Template:template
                template: 'AcmeBundle:Static:privacy.html.twig'
                maxAge: 86400
                sharedMaxAge: 86400

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="acme_privacy" path="/privacy">
                <default key="_controller">FrameworkBundle:Template:template</default>
                <default key="template">AcmeBundle:Static:privacy.html.twig</default>
                <default key="maxAge">86400</default>
                <default key="sharedMaxAge">86400</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('acme_privacy', new Route('/privacy', array(
            '_controller'  => 'FrameworkBundle:Template:template',
            'template'     => 'AcmeBundle:Static:privacy.html.twig',
            'maxAge'       => 86400,
            'sharedMaxAge' => 86400,
        )));

        return $collection;

Les valeurs de ``maxAge`` et ``sharedMaxAge`` sont utilisées pour modifier 
l'objet Response créé dans le contrôleur. Pour plus d'informations sur la mise
en cache, voir :doc:`/book/http_cache</book/http_cache>`.

Il existe également un paramètre ``private`` (non présenté ici). Par défaut, la réponse
est faite de manière public (la réponse peut être mise en cache, à la fois par les 
caches privés et les caches publics) tant que les paramètres ``maxAge`` ou ``sharedMaxAge``
sont renseignés. Si elle est définie à ``true`` la réponse devient privé (la réponse
concerne un unique utilisateur et ne doit pas être stockée dans les caches publics).
