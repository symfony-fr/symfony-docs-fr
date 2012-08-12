.. index::
   single: Assetic; Introduction

Comment utiliser Assetic pour gérer vos ressources
==================================================

Assetic associe deux concepts majeurs : les ressources et les filtres. Les ressources
sont des fichiers comme les feuilles de style, les JavaScript et les images. Les
filtres peuvent être appliqués à ces fichiers avant qu'ils soient servis au
navigateur. Cela permet de gérer séparemment les fichiers ressources qui sont stockés
par l'application des fichiers qui sont réellement présentés à l'utilisateur.

Sans Assetic, vous servez directement les fichiers qui sont stockés dans votre
application :

.. configuration-block::

    .. code-block:: html+jinja

        <script src="{{ asset('js/script.js') }}" type="text/javascript" />

    .. code-block:: php

        <script src="<?php echo $view['assets']->getUrl('js/script.js') ?>"
                type="text/javascript" />

Mais *avec* Assetic, vous pouvez manipuler ces ressources de la manière dont
vous le désirez (ou les charger où vous le voulez) avant de les    servir. Cela
signifie que vous pouvez :

* Minifier et combiner toutes vos CSS ou vos fichiers JavaScript

* Exécuter tous (ou juste une partie) vos fichiers CSS ou JS en passant par des
  compilateurs comme LESS, SASS ou CoffeeScript.

* Optimiser vos images

Ressources
----------

Utiliser Assetic plutôt que servir les fichiers directement offre de nombreux avantages.
Les fichiers n'ont pas besoin d'être stockés là où il seront servis, et peuvent
provenir de plusieurs sources, notamment d'un bundle :

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts
            '@AcmeFooBundle/Resources/public/js/*'
        %}
        <script type="text/javascript" src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*')) as $url): ?>
        <script type="text/javascript" src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. tip::

    Pour inclure une feuille de style, vous pouvez utiliser le même procédé que
	ci-dessus, sauf que vous utiliserez le tag `stylesheets` :

    .. configuration-block::

        .. code-block:: html+jinja

            {% stylesheets
                '@AcmeFooBundle/Resources/public/css/*'
            %}
            <link rel="stylesheet" href="{{ asset_url }}" />
            {% endstylesheets %}

        .. code-block:: html+php

            <?php foreach ($view['assetic']->stylesheets(
                array('@AcmeFooBundle/Resources/public/css/*')) as $url): ?>
            <link rel="stylesheet" href="<?php echo $view->escape($url) ?>" />
            <?php endforeach; ?>

Dans cet exemple, tous les fichiers du répertoire ``Resources/public/js/`` du bundle
``AcmeFooBundle`` seront chargés et servis depuis un autre répertoire. La balise
réellement générée pourrait ressembler à ceci :

.. code-block:: html

    <script src="/app_dev.php/js/abcd123.js"></script>

.. note::
    
    C'est un point essentiel : une fois que vous laissez Assetic gérer vos ressources,
    les fichiers sont servis depuis différents emplacements. Cela *peut* poser des
    problèmes avec les CSS qui utilisent des images référencées par des chemins
    relatifs. Pourtant, ce problème peut être résolu en utilisant le filtre
    ``cssrewrite`` qui met à jour les chemins dans les fichiers CSS pour prendre
    en compte leur nouvel emplacement.

Combiner des ressources
~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez aussi combiner plusieurs fichiers en un seul. Cela aide à réduire le
nombre de requêtes HTTP, ce qui est très important pour les performances. Cela
vous permet aussi de maintenir les fichiers plus facilement en les découpants
en petites parties faciles à gérer. Cela peut être un plus pour la réusabilité
de votre projet puisque vous pouvez facilement séparer les fichiers spécifiques
au projet des fichiers qui peuvent être réutilisés dans d'autres applications,
mais toujours les servir comme un fichier unique :

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts
            '@AcmeFooBundle/Resources/public/js/*'
            '@AcmeBarBundle/Resources/public/js/form.js'
            '@AcmeBarBundle/Resources/public/js/calendar.js'
        %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*',
                  '@AcmeBarBundle/Resources/public/js/form.js',
                  '@AcmeBarBundle/Resources/public/js/calendar.js')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

En environnement de `dev`, chaque fichier est toujours servi individuellement
pour que vous puissiez débugguer plus facilement. Cependant, en environnement de
`prod`, ils seront affichés dans une unique balise `script`.

.. tip::

    Si vous découvrez Assetic et essayez d'utiliser votre application en
    environnement de ``prod`` (en utilisant le contrôleur ``app.php``), vous
    verrez probablement que vos CSS et JS plantent. Pas de panique ! C'est
    fait exprès. Pour plus de détails sur l'utilisation d'Assetic en
    environnement de `prod`, lisez :ref:`cookbook-assetic-dumping`.

Et combiner les fichiers ne s'applique pas uniquement à *vos* fichiers. Vous
pouvez aussi utiliser Assetic pour combiner les ressources tierces, comme jQuery,
à vos fichiers dans un fichier unique :

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts
            '@AcmeFooBundle/Resources/public/js/thirdparty/jquery.js'
            '@AcmeFooBundle/Resources/public/js/*'
        %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/thirdparty/jquery.js',
                  '@AcmeFooBundle/Resources/public/js/*')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

Filtres
-------

Une fois qu'elles sont gérées par Assetic, vous pouvez appliquer des filtres
à vos ressources avant qu'elles soient servies. Cela inclut les filtres qui
compressent vos ressources pour réduire la taille des fichiers (pour de
meilleures performances). D'autres filtres peuvent compiler des fichiers
CoffeeScript en JavaScript ou couvertir vos fichiers SASS en CSS.
En fait, Assetic possède une longue liste de filtres.

Plusieurs de ces filtres ne font pas le travail directement, mais utilisent
des librairies tierces pour faire le gros du travail. Cela signifie que vous
devrez souvent installer une librairie tierce pour utiliser un filtre. Le grand
avantage d'utiliser Assetic pour faire appel à ces librairies (plutôt que de les
utiliser directement) est qu'au lieu de les exécuter à la main après avoir modifié
les fichiers, Assetic prendra tout en charge pour vous, et supprimera définitivement
cette étape du processus de développement et de déploiement.

Pour utiliser un filtre, vous aurez d'abord besoin de le spécifier dans la
configuration d'Assetic. Ajouter un filtre dans la configuration ne signifie
pas qu'il est utilisé, mais juste qu'il est prêt à l'être (nous allons l'utiliser
ci-dessous).

Par exemple, pour utiliser le JavaScript YUI Compressor, la configuration
suivante doit être ajoutée :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                yui_js:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="yui_js"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'yui_js' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
            ),
        ));

Maintenant, pour vraiment *utiliser* le filtre sur un groupe de fichiers, ajoutez
le dans votre template :

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts
            '@AcmeFooBundle/Resources/public/js/*'
            filter='yui_js'
        %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('yui_js')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

Vous pouvez trouver un guide plus détaillé sur la configuration et l'utilisation
des filtres Assetic ainsi que des informations sur le mode debug d'Assetic
en lisant :doc:`/cookbook/assetic/yuicompressor`.

Contrôler l'URL utilisée
------------------------

Si vous le voulez, vous pouvez contrôler les URLs générées par Assetic.
Cela se fait dans le template, et le chemin est relatif par rapport
à la racine publique :

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts
            '@AcmeFooBundle/Resources/public/js/*'
            output='js/compiled/main.js'
        %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array(),
            array('output' => 'js/compiled/main.js')
        ) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. note::

    Symfony contient également une méthode pour le *cache busting* (technique
    empêchant la mise en cache), où l'URL générée par Assetic contient un
    paramètre qui peut être incrémenté, via la configuration, à chaque
    déploiement. Pour plus d'informations, lisez l'option de configuration
    :ref:`ref-framework-assets-version`.

.. _cookbook-assetic-dumping:

Exporter les ressources
-----------------------

En environnement de ``dev``, Assetic génère des chemins vers des fichiers CSS et
JavaScript qui n'existent pas physiquement sur votre ordinateur. Mais ils sont
néanmoins affichés car un contrôleur interne de Symfony ouvre les fichiers et
sert leur contenu (après avoir exécuté tous les filtres).

Cette manière dynamique de servir des ressources traitées est géniale car
cela signifie que vous pouvez immédiatement voir les modifications que vous
apportez à vos fichiers. Mais l'inconvénient et que cela peut parfois être
un peu plus lent. Si vous utilisez beaucoup de filtres, cela peut être
carrément frustrant.

Heureusement, Assetic fournit une méthode pour exporter vos ressources vers
des fichiers réels au lieu de les générer dynamiquement.


Exporter les ressources en environnement de ``prod``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En environnement de ``prod``, vos fichiers JS et CSS sont représentés chacun
par une balise unique. En d'autres termes, plutôt que de voir chacun des fichiers
JavaScript que vous incluez dans votre code source, vous verrez probablement quelque
chose comme ceci :

.. code-block:: html

    <script src="/app_dev.php/js/abcd123.js"></script>

De plus, ce fichier n'existe **pas** vraiment et n'est pas non plus affiché
dynamiquement par Symfony (car les ressources sont en environnement de ``dev``).
C'est fait exprès : laisser Symfony générer ces fichiers dynamiquement en production
serait tout simplement trop lent.

Au lieu de cela, chaque fois que vous exécutez votre application dans l'environnement
de ``prod`` (et par conséquent, chaque fois que vous déployez), vous devriez lancer
la commande suivante :

.. code-block:: bash

    php app/console assetic:dump --env=prod --no-debug

Cela génèrera physiquement et écrira chaque fichier dont vous avez besoin
(ex ``/js/abcd123.js``). Si vous mettez à jour vos ressources, vous aurez besoin
de relancer cette commande pour regénérer vos fichiers.

Exporter les ressources en environnement de ``dev``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Par défaut, chaque chemin de ressource généré en environnement de ``dev``
est pris en charge dynamiquement par Symfony. Cela n'a pas d'inconvénient
(vous pouvez voir vos changements immédiatement), sauf que les ressources
peuvent être chargées plus lentement. Si vous trouvez que vos ressources sont
chargés trop lentement, suivez ce guide

Premièrement, dites à Symfony de ne plus essayer de traiter ces fichiers
dynamiquement. Apportez les modifications suivantes dans le fichier ``config_dev.yml`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        assetic:
            use_controller: false

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <assetic:config use-controller="false" />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('assetic', array(
            'use_controller' => false,
        ));

Ensuite, puisque Symfony ne génère plus ces fichiers pour vous, vous
aurez besoin de les exporter manuellement. Pour ce faire, lancez la commande
suivante :

.. code-block:: bash

    php app/console assetic:dump

Elle écrit physiquement tous les fichiers de ressource dont vous avez
besoin pour l'environnement de ``dev``. Le gros inconvénient est que vous
devrez faire cela chaque fois que vous modifiez une ressource. Heureusement,
en passant l'option ``--watch``, la commande regénèrera automatiquement les
ressources *modifiées* :

.. code-block:: bash

    php app/console assetic:dump --watch

Lancer cette commande en environnement de ``dev`` peut générer un florilège
de fichiers. Pour conserver votre projet bien organisé, il peut être intéressant
de mettre les fichiers générés dans un répertoire séparé (ex ``/js/compiled``) :

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts
            '@AcmeFooBundle/Resources/public/js/*'
            output='js/compiled/main.js'
        %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array(),
            array('output' => 'js/compiled/main.js')
        ) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>