.. index::
   single: Dependency Injection; Lazy Services

Les services paresseux (« lazy services » en anglais)
=====================================================

.. versionadded:: 2.3
   les services ``lazy`` ont été ajouté depuis Symfony 2.3.

Pourquoi les services ``lazy`` ?
--------------------------------

Dans certains cas, vous souhaiteriez injecter un service qui est vraiment lourd
à instancier mais il n'est pas toujours utiliser dans l'objet. Par exemple, imaginez
que vous ayez un ``NewsletterManager`` et que vous injectiez le service ``mailer``.
Seulement quelques méthodes de votre ``NewsletterManager`` utilise le service
``mailer``, mais même quand vous n'en avez pas besoin, le service ``mailer`` est
toujours instancié à la construction de votre ``NewsletterManager``.

Configurer les services comme ``lazy`` est une réponse à ce problème. Avec le
service ``lazy``, un "proxy" du service ``mailer`` est injecté. Il ressemble et
agit comme le service ``mailer``, à l'exception que le service ``mailer`` n'est
pas instancié, et le sera qu'au moment où vous interagirez avec le proxy.

Installation
------------

En premier pour utilisez l'instantiation d'un service ``lazy``, vous aurez besoin
d'installer `ProxyManager bridge`_:

.. code-block:: bash

    $ php composer.phar require symfony/proxy-manager-bridge:2.3.*

.. note::

    Si vous utilisez le framework full-stack, le paquet n'est pas inclus et vous
    aurez besoin de l'ajouter à votre ``composer.json`` et l'installer ( ce que
    produit la commande ci-dessus).

Configuration
-------------

Vous pouvez configurer un service comme ``lazy`` en modifiant sa définition:

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Acme\Foo
             lazy: true

    .. code-block:: xml

        <service id="foo" class="Acme\Foo" lazy="true" />

    .. code-block:: php

        $definition = new Definition('Acme\Foo');
        $definition->setLazy(true);
        $container->setDefinition('foo', $definition);

Vous pouvez récupérer le service depuis le conteneur::

    $service = $container->get('foo');

A ce moment, l'argument ``$service``récupéré doit être un `proxy`_ virtuel et
a la même signature que la classe représentant le service. Vous pouvez aussi
injecter ce service normalement à d'autres services. L'objet qui est réellement
injecter est le service proxy.

.. note::

    Si vous n'installez pas le `ProxyManager bridge`_, le conteneur passera
    outre le drapeau ``lazy`` et instanciera normalement le service.

Le proxy est initialisé et le service demandé sera instancié au moment où vous interagirez
avec l'objet.

Ressources additionnelles
-------------------------

Vous pouvez en apprendre plus sur les proxies, comment ils sont instanciés,
générés et initialisés dans la `documentation du ProxyManager`_.

.. _`ProxyManager bridge`: https://github.com/symfony/symfony/tree/master/src/Symfony/Bridge/ProxyManager
.. _`proxy`: http://en.wikipedia.org/wiki/Proxy_pattern
.. _`document du ProxyManager`: https://github.com/Ocramius/ProxyManager/blob/master/docs/lazy-loading-value-holder.md