.. index::
   single: Assetic; YUI Compressor

Comment minifier les JavaScripts et les feuilles de style avec YUI Compressor
=============================================================================

Yahoo! fournit un excellent utilitaire pour minifier les JavaScripts et les
feuilles de style pour qu'elles soient plus rapides à charger, `YUI Compressor`_.
Grâce à Assetic, vous pourrez tirer profit de cet outil très facilement.

Téléchargez le JAR YUI Compressor
---------------------------------

YUI Compressor est écrit en Java est distribué sous forme de JAR. `Téléchargez le JAR`_
sur le site de Yahoo! et enregistrez le sous ``app/Resources/java/yuicompressor.jar``.

Configurez les filtres YUI
--------------------------

Maintenant vous devez configurer les deux filtres Assetic dans votre application,
l'un pour minifier les JavaScripts avec YUI Compressor et l'autre pour minifier
les feuilles de style :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            # java: "/usr/bin/java"
            filters:
                yui_css:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"
                yui_js:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="yui_css"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
            <assetic:filter
                name="yui_js"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            // 'java' => '/usr/bin/java',
            'filters' => array(
                'yui_css' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
                'yui_js' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
            ),
        ));

.. note::
    
    Les utilisateurs de Windows ne doivent pas oublier de mettre à jour l'emplacement de Java.
    Dans Windows8 x64 bit, il s'agit de
    ``C:\Program Files (x86)\Java\jre6\bin\java.exe`` par défaut
    
Vous avez maintenant accès aux deux nouveaux filtres Assetic dans votre
application : ``yui_css`` et ``yui_js``. Ils utiliseront YUI Compressor
pour minifier respectivement les feuilles de style et les JavaScripts.

Minifiez vos Ressources
-----------------------

Maintenant YUI Compressor est configuré, mais rien ne se passera tant que vous
n'appliquez pas ces filtres à une ressource (asset). Puisque vos ressources font
partie de la couche Vue, ce travail doit être fait dans vos templates :

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' filter='yui_js' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('yui_js')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. note::

    L'exemple ci-dessus part du principe que vous avez un bundle appelé ``AcmeFooBundle``
    et que vos fichiers JavaScript se trouvent dans le répertoire ``Resources/public/js``
    dans votre bundle. Ce n'est, en fait, pas très important car vous pouvez inclure vos
    fichiers JavaScript où vous le voulez.

En rajoutant le filtre ``yui_js`` à la ressource ci-dessus, vous devriez voir que les
JavaScripts minifiés sont chargés beaucoup plus rapidement. Le même procédé peut être
utilisé pour minifier vos feuilles de style.

.. configuration-block::

    .. code-block:: html+jinja

        {% stylesheets '@AcmeFooBundle/Resources/public/css/*' filter='yui_css' %}
            <link rel="stylesheet" type="text/css" media="screen" href="{{ asset_url }}" />
        {% endstylesheets %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->stylesheets(
            array('@AcmeFooBundle/Resources/public/css/*'),
            array('yui_css')
        ) as $url): ?>
            <link rel="stylesheet" type="text/css" media="screen" href="<?php echo $view->escape($url) ?>" />
        <?php endforeach; ?>

Désactiver la minification en Mode Debug
----------------------------------------

Les JavaScripts et feuilles de styles minifiés sont très difficiles à lire;
et encore moins à débugguer. Pour palier cela, Assetic vous permet de désactiver
un filtre lorsque votre application est en mode debug. Vous pouvez faire cela
en préfixant le nom du filtre dans votre template par un point d'interrogation :
``?``. Cela indique à Assetic de n'appliquer les filtres que si le mode debug
n'est pas actif.

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' filter='?yui_js' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('?yui_js')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. tip::
    
    Plutôt que d'ajouter le filtre à vos balises assets, vous pouvez aussi
    l'activer de façon globale en ajoutant l'attribut apply-to à la configuration
    du filtre, par exemple ``apply_to: "\.js$"`` pour le filtre yui_js.
    Pour que le filtre ne s'applique qu'en production, ajoutez le au fichier
    config_prod au lieu du fichier de configuration commun. Pour plus de détails
    sur comment appliquer des filtres en fonction des extensions de fichiers, lisez
    :ref:`cookbook-assetic-apply-to`.

.. _`YUI Compressor`: http://developer.yahoo.com/yui/compressor/
.. _`Téléchargez le JAR`: http://yuilibrary.com/projects/yuicompressor/
