.. index::
   single: Routage; Exigence système

Comment forcer les routes à toujours utiliser HTTPS ou HTTP
===========================================================

Quelquefois, vous voulez sécuriser certaines routes et être sûr qu'on y
accède toujours via le protocole HTTPS. Le composant « Routing »
vous permet de forcer le système de l'URI via la condition requise
``_scheme`` :

.. configuration-block::

    .. code-block:: yaml

        secure:
            pattern:  /secure
            defaults: { _controller: AcmeDemoBundle:Main:secure }
            requirements:
                _scheme:  https

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="secure" pattern="/secure">
                <default key="_controller">AcmeDemoBundle:Main:secure</default>
                <requirement key="_scheme">https</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('secure', new Route('/secure', array(
            '_controller' => 'AcmeDemoBundle:Main:secure',
        ), array(
            '_scheme' => 'https',
        )));

        return $collection;

La configuration ci-dessus force la route nommée ``secure`` à toujours
utiliser HTTPS.

Pendant la génération de l'URL de ``secure``, et si le système actuel est
HTTP, Symfony va automatiquement générer une URL absolue avec HTTPS comme
« scheme » :

.. code-block:: jinja

    {# Si le « scheme » actuel est HTTPS #}
    {{ path('secure') }}
    {# génère /secure

    {# Si le « scheme » actuel est HTTP #}
    {{ path('secure') }}
    {# génère https://example.com/secure #}

La condition requise est aussi forcée pour les requêtes entrantes. Si vous
essayez d'accéder au chemin ``/secure`` avec HTTP, vous serez automatiquement
redirigé à la même URL, mais avec le « scheme » HTTPS.

Les exemples ci-dessus utilisent ``https`` en tant que ``_scheme``, mais vous
pouvez aussi forcer une URL à toujours utiliser ``http``.

.. note::

    Le composant Security fournit une autre façon d'imposer HTTP ou HTTPS via
    le paramètre ``requires_channel``. Cette méthode alternative est mieux
    adaptée pour sécuriser une « zone » de votre site web (toutes les URLs dans
    la zone ``/admin``) ou pour sécuriser les URLs définies dans un bundle tiers.
