.. index::
   single: Assetic; Apply Filters

Comment appliquer un filtre Assetic à une extension de fichier spécifique
=========================================================================

Les filtres Assetic peuvent être appliqués à des fichiers individuels, à
des groupes de fichiers ou même, comme vous allez le voir ici, à des fichiers
qui ont une extension spécifique. Pour vous montrer comment gérer chaque cas,
supposons que vous ayez le filtre Assetic CoffeeScript qui compile les fichiers
CoffeeScript en JavaScript.

La configuration principale contient juste les chemins vers coffee et node.
Leurs valeurs respectives par défaut sont ``/usr/bin/coffee`` et ``/usr/bin/node``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                coffee:
                    bin: /usr/bin/coffee
                    node: /usr/bin/node

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="coffee"
                bin="/usr/bin/coffee"
                node="/usr/bin/node" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'coffee' => array(
                    'bin' => '/usr/bin/coffee',
                    'node' => '/usr/bin/node',
                ),
            ),
        ));

Filtrer un fichier unique
-------------------------

Vous pouvez maintenant compiler un fichier unique CoffeeScript en JavaScript
depuis vos templates :

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/example.coffee'
            filter='coffee'
        %}
        <script src="{{ asset_url }}" type="text/javascript"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/example.coffee'),
            array('coffee')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>" type="text/javascript"></script>
        <?php endforeach; ?>

C'est tout ce dont vous avez besoin pour compiler ce fichier CoffeeScript
et le servir comme JavaScript compilé.

Filtrer des fichiers multiples
------------------------------

Vous pouvez aussi combiner plusieurs fichiers CoffeeScript en un unique
fichier en sortie :

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/example.coffee'
                       '@AcmeFooBundle/Resources/public/js/another.coffee'
            filter='coffee'
        %}
        <script src="{{ asset_url }}" type="text/javascript"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/example.coffee',
                  '@AcmeFooBundle/Resources/public/js/another.coffee'),
            array('coffee')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>" type="text/javascript"></script>
        <?php endforeach; ?>

Les deux fichiers seront maintenant délivrés comme un unique fichier compilé
en JavaScript.

.. _cookbook-assetic-apply-to:

Filtrer en se basant sur les extensions
---------------------------------------

Un des plus grands avantages d'Assetic est de pouvoir réduire le nombre de
fichiers de ressources pour réduire le nombre de requêtes HTTP. Dans le but
d'en tirer le plus grand avantage possible, il pourrait être intéressant de combiner
*tous* vos fichiers CoffeeScript et JavaScript ensembles puisqu'ils seront
finalement délivrés comme JavaScript. Malheureusement, se contenter d'ajouter
les fichiers JavaScript aux fichiers à combiner ne fonctionnera pas car
le JavaScript ne passera pas la compilation CoffeeScript.

Ce problème peut être évité en ajoutant l'option ``apply_to`` à la configuration,
ce qui vous permettra de spécifier qu'un filtre devra toujours être appliqué
à une extension de fichier particulière. Dans ce cas, vous pouvez spécifier que
le filtre Coffee s'applique à tous les fichiers ``.coffee`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                coffee:
                    bin: /usr/bin/coffee
                    node: /usr/bin/node
                    apply_to: "\.coffee$"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="coffee"
                bin="/usr/bin/coffee"
                node="/usr/bin/node"
                apply_to="\.coffee$" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'coffee' => array(
                    'bin' => '/usr/bin/coffee',
                    'node' => '/usr/bin/node',
                    'apply_to' => '\.coffee$',
                ),
            ),
        ));

Avec cela, vous n'avez plus besoin de spécifier le filtre ``coffee`` dans le template.
Vous pouvez aussi lister les fichiers JavaScript classique, chacun d'eux sera combiné
et délivré en un unique fichier JavaScript (avec les fichiers ``.coffee`` seulement qui
passeront à travers le filtre CoffeeScript) :

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/example.coffee'
                       '@AcmeFooBundle/Resources/public/js/another.coffee'
                       '@AcmeFooBundle/Resources/public/js/regular.js'
        %}
        <script src="{{ asset_url }}" type="text/javascript"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/example.coffee',
                  '@AcmeFooBundle/Resources/public/js/another.coffee',
                  '@AcmeFooBundle/Resources/public/js/regular.js'),
            as $url): ?>
        <script src="<?php echo $view->escape($url) ?>" type="text/javascript"></script>
        <?php endforeach; ?>
