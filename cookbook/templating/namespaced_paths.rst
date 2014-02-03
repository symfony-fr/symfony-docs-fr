.. index::
   single: Templating; Chemins Twig Namespacés

Comment utiliser et enregistrer des chemins Twig namespacés
===========================================================

Habituellement, lorsque vous vous référez à un template, vous allez utiliser le format
``MyBundle:Subdir:filename.html.twig`` (consultez :ref:`template-naming-locations`).

Twig offre également nativement une fonctionnalité appelée "chemins namespacés"
("namespaced paths" en anglais), et elle est intégrée automatiquement dans vos bundles.

Prenons les chemins suivants comme exemple :

.. code-block:: jinja

    {% extends "AcmeDemoBundle::layout.html.twig" %}
    {% include "AcmeDemoBundle:Foo:bar.html.twig" %}

Avec les chemins namespacés, ce qui suit fonctionne également :

.. code-block:: jinja

    {% extends "@AcmeDemo/layout.html.twig" %}
    {% include "@AcmeDemo/Foo/bar.html.twig" %}

Les deux chemins sont valides et fonctionnel par défaut en Symfony2.

.. tip::

    Comme bonus, la syntaxe namespacée est plus rapide.

Enregistrer vos propres namespaces
----------------------------------

Vous pouvez également enregistrer vos propes namespaces. Supposez que vous
utilisez une quelconque bibliothèque tierce incluant des templates Twig
placés dans ``vendor/acme/foo-bar/templates``. Premièrement, enregistrez un
namespace pour pour ce dossier :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            # ...
            paths:
                "%kernel.root_dir%/../vendor/acme/foo-bar/templates": foo_bar

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
                   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                   xmlns:twig="http://symfony.com/schema/dic/twig"
        >

            <twig:config debug="%kernel.debug%" strict-variables="%kernel.debug%">
                <twig:path namespace="foo_bar">%kernel.root_dir%/../vendor/acme/foo-bar/templates</twig:path>
            </twig:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array(
            'paths' => array(
                '%kernel.root_dir%/../vendor/acme/foo-bar/templates' => 'foo_bar',
            );
        ));

Le namespace enregistré est appelé ``foo_bar``, se référant au dossier
``vendor/acme/foo-bar/templates``. Supposant qu'il y est un fichier
nommé ``sidebar.twig`` dans ce dossier, vous pouvez utiliser facilement :

.. code-block:: jinja

    {% include '@foo_bar/side.bar.twig' %}
