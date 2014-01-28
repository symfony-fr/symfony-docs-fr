.. index::
   single: Dependency Injection; Lazy Services

Les services instanciés à la demande (« lazy services » en anglais)
===================================================================

.. versionadded:: 2.3
   Les services ``lazy`` (instancés à la demande) ont été ajoutés depuis Symfony 2.3.

Pourquoi les services ``lazy`` ?
--------------------------------

Dans certains cas, vous souhaiteriez injecter un service qui est vraiment lourd
à instancier mais il n'est pas toujours utilisé dans l'objet. Par exemple, imaginez
que vous ayez un ``NewsletterManager`` et que vous injectiez le service ``mailer``.
Seules quelques méthodes de votre ``NewsletterManager`` utilisent le service
``mailer``, mais même quand vous n'en avez pas besoin, le service ``mailer`` est
toujours instancié à la construction de votre ``NewsletterManager``.

Configurer les services comme ``lazy`` est une réponse à ce problème. Avec le
service ``lazy``, un "proxy" du service ``mailer`` est injecté. Il ressemble et
agit comme le service ``mailer``, à l'exception près que le service ``mailer`` n'est
pas instancié, et ne le sera qu'au moment où vous interagirez avec le proxy.

Installation
------------

Tout d'abord, pour utiliser l'instantiation d'un service ``lazy``, vous aurez besoin
d'installer `ProxyManager bridge`_:

.. code-block:: bash

    $ php composer.phar require symfony/proxy-manager-bridge:2.3.*

.. note::

    Si vous utilisez le framework full-stack, le paquet n'est pas inclus et vous
    aurez besoin de l'ajouter à votre ``composer.json`` et l'installer (ce que
    fait la commande ci-dessus).

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

A ce moment, l'argument ``$service`` récupéré doit être un `proxy`_ virtuel et
a la même signature que la classe représentant le service. Vous pouvez aussi
injecter ce service normalement dans d'autres services. L'objet qui est réellement
injecté est le service proxy.

Pour vérifier si votre proxy fonctionne, vous pouvez simplement vérifier l'interface
de l'objet reçu.

.. code-block:: php

    var_dump(class_implements($service));

Si la classe implémente l'interface ``ProxyManager\Proxy\LazyLoadingInterface`` vos
services instanciés à la demande (lazy) fonctionnent.

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
.. _`documentation du ProxyManager`: https://github.com/Ocramius/ProxyManager/blob/master/docs/lazy-loading-value-holder.md
