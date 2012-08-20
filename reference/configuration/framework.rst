.. index::
   single: Configuration reference; Framework

Configuration du FrameworkBundle ("framework")
==============================================

Ce document de référence est un travail en cours. Il devrait être exact, mais
toutes les options ne sont pas encore totalement décrites.

Le ``FrameworkBundle`` contient la majeure partie de la fonctionnalité « de base »
du framework et peut être configuré sous la clé ``framework`` dans la configuration
de votre application. Cela inclut la définition de paramètres liés aux sessions,
à la traduction, aux formulaires, à la validation, au routage et plus encore.

Configuration
-------------

* `secret`_
* `ide`_
* `test`_
* `trust_proxy_headers`_
* `form`_
    * enabled
* `csrf_protection`_
    * enabled
    * field_name
* `session`_
    * `lifetime`_
* `templating`_
    * `assets_base_urls`_
    * `assets_version`_
    * `assets_version_format`_

secret
~~~~~~

**type**: ``string`` **requis**

Ceci est une chaîne de caractères qui devrait être unique à votre application.
En pratique, elle est utilisée pour générer les jetons CSRF, mais elle pourrait
être utilisée dans un autre contexte où avoir une chaîne de caractères unique
est utile. Ceci devient le paramètre du conteneur de service nommé
``kernel.secret``.

ide
~~~

**type**: ``string`` **par défaut**: ``null``

Si vous utilisez un IDE (« Integrated Development Environment », ou « Environnement
de Développement Intégré » en français) comme TextMate ou Mac Vim, alors Symfony
peut par exemple transformer tous les chemins de fichier d'un message d'exception
en un lien, qui va ouvrir ce fichier dans votre IDE.

Si vous utilisez TextMate ou Mac Vim, vous pouvez simplement utiliser l'une des
valeurs prédéfinies suivantes :

* ``textmate``
* ``macvim``

Vous pouvez aussi spécifier une chaîne de caractères personnalisée représentant
un lien vers un fichier. Si vous faites ceci, tous les symboles pourcentage
(``%``) doivent être doublés pour échapper ce caractère. Par exemple, la chaîne
de caractères complète pour TextMate ressemblerait à cela :

.. code-block:: yaml

    framework:
        ide:  "txmt://open?url=file://%%f&line=%%l"

Bien sûr, comme chaque développeur utilise un IDE différent, il est préférable
de définir ceci au niveau du système. Cela peut être effectué en définissant
la valeur ``xdebug.file_link_format`` du fichier PHP.ini comme étant la
chaîne de caractères du lien vers le fichier. Si cette valeur de configuration
est définie, alors l'option ``ide`` n'a pas besoin d'être spécifiée.

.. _reference-framework-test:

test
~~~~

**type**: ``Boolean``

Si ce paramètre de configuration est présent (et n'est pas défini comme ``false``),
alors les services liés au test de votre application (par exemple :
``test.client``) sont chargés. Ce paramètre devrait être présent dans votre
environnement ``test`` (généralement via ``app/config/config_test.yml``).
Pour plus d'informations, voir :doc:`/book/testing`.

trust_proxy_headers
~~~~~~~~~~~~~~~~~~~

**type**: ``Boolean``

Configure si les entêtes HTTP (comme ``HTTP_X_FORWARDED_FOR``, ``X_FORWARDED_PROTO``, et
``X_FORWARDED_HOST``) sont conformes pour une connexion SSL. Par défaut, elle est définie
à ``false`` et seules les connexions SSL_HTTPS sont considérées comme sécurisées.

Vous devriez activer cette configuration si votre application est derrière un reverse proxy.

.. _reference-framework-form:

form
~~~~

csrf_protection
~~~~~~~~~~~~~~~

session
~~~~~~~

lifetime
........

**type**: ``integer`` **par défaut**: ``0``

Ceci détermine la durée de vie de la session - en secondes. Par défaut, cette
valeur sera définie à ``0``, ce qui signifie que le cookie est valide pour la
durée de la session du navigateur.

templating
~~~~~~~~~~

assets_base_urls
................

**default**: ``{ http: [], ssl: [] }``

Cette option vous permet de définir la base des URLs à utiliser pour les fichiers
référencés depuis des pages ``http`` et ``ssl`` (``https``). Une valeur exprimée
via une chaîne de caractères pourrait être fournie à la place d'un tableau.
Si plusieurs bases d'URL sont fournies, Symfony2 va sélectionner l'une d'entre
elles chaque fois qu'il génère un chemin vers un fichier.

Pour votre confort, la valeur de ``assets_base_urls`` peut être définie
directement avec une chaîne de caractères ou avec un tableau de chaînes de
caractères, qui sera automatiquement organisé en une collection de bases d'URL
pour les requêtes ``http`` et ``https`. Si une URL commence par ``https://`` ou
est `relative à un protocole`_ (par exemple : commence avec `//`) elle sera
ajoutée aux deux collections. Les URLs commençant par ``http://`` seront
ajoutées uniquement à la collection ``http``.

.. versionadded:: 2.1
    A la différence de la plupart des blocs de configuration, des valeurs
    successives pour ``assets_base_urls`` vont s'outrepasser entre elles au
    lieu d'être fusionnées. Ce comportement a été choisi car les développeurs
    vont généralement définir une URL de base pour chaque environnement.
    Sachant que la plupart des projets ont tendance à hériter les configurations
    (par exemple : ``config_test.yml`` importe ``config_dev.yml``) et/ou à
    partager une configuration de base (i.e. ``config.yml``), fusionner reviendrait
    à avoir un ensemble d'URLs de base pour de multiples environnements.

.. _ref-framework-assets-version:

assets_version
..............

**type**: ``string``

Cette option est utilisée pour *invalider* le cache de fichiers en ajoutant
de façon globale un paramètre de requête à tous les chemins de fichier
rendus (par exemple : ``/images/logo.png?v2``). Cela s'applique uniquement
aux fichiers rendus via la fonction ``asset`` de Twig (ou son équivalent PHP)
ainsi qu'aux fichiers rendus avec Assetic.

Par exemple, supposons que vous ayez ce qui suit :

.. configuration-block::

    .. code-block:: html+jinja

        <img src="{{ asset('images/logo.png') }}" alt="Symfony!" />

    .. code-block:: php

        <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" alt="Symfony!" />

Par défaut, cela va retourner un chemin vers votre image tel ``/images/logo.png``.
Maintenant, activez l'option ``assets_version`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating: { engines: ['twig'], assets_version: v2 }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:templating assets-version="v2">
            <framework:engine id="twig" />
        </framework:templating>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'engines' => array('twig'),
                'assets_version' => 'v2',
            ),
        ));

Maintenant, le même fichier sera rendu tel ``/images/logo.png?v2``. Si vous
utilisez cette fonctionnalité, vous **devez** manuellement incrémenter la
valeur de ``assets_version`` avant chaque déploiement afin que le paramètre
de la requête change.

Vous pouvez aussi contrôler comment la chaîne de caractères de la requête
fonctionne via l'option `assets_version_format`_.

assets_version_format
.....................

**type**: ``string`` **par défaut**: ``%%s?%%s``

Cela spécifie un « pattern » de `sprintf()`_ qui va être utilisé avec l'option
`assets_version`_ pour construire un chemin vers un fichier. Par défaut, le
« pattern » ajoute la version du fichier en tant que chaîne de caractères dans
la requête. Par exemple, si ``assets_version_format`` est défini avec la valeur
``%%s?version=%%s`` et que ``assets_version`` est défini avec ``5``, le chemin
du fichier serait ``/images/logo.png?version=5``.

.. note::

    Tous les symboles pourcentage (``%``) dans la chaîne de caractères du format
    doivent être doublés pour échapper le caractère. Sans échappement, les
    valeurs pourraient être interprétées par inadvertance comme des
    :ref:`book-service-container-parameters`.

.. tip::

    Certains « CDNs » (« Content Delivery Network ») ne supporte pas l'invalidation
    du cache via des chaînes de caractères de requête, alors l'injection de
    la version dans le chemin du fichier actuel est nécessaire. Heureusement,
    ``assets_version_format`` n'est pas limité à la production de chaînes de
    caractères de requête versionnées.

    Le pattern reçoit respectivement le chemin original du fichier et la version
    en tant que premier et second paramètre. Comme le chemin du fichier est un
    paramètre, nous ne pouvons pas le modifier sur place (par exemple :
    ``/images/logo-v5.png``); cependant, nous pouvons préfixer le chemin du
    fichier en utilisant un pattern comme ``version-%%2$s/%%1$s``, qui donnerait
    un chemin tel ``version-5/images/logo.png``.

    Des règles de réécriture d'URL pourraient alors être utilisées afin de ne pas
    tenir compte du préfixe de version avant de servir le fichier. Une autre
    alternative pourrait être de copier les fichiers dans le répertoire approprié
    de la version lors de votre procédure de déploiement et ainsi de ne pas
    avoir à créer quelconque règle de réécriture d'URL. La dernière option est
    utile si vous souhaitez laisser les anciennes versions des fichiers
    accessibles depuis leur URL originale.

Toutes les Options de Configuration par Défaut
----------------------------------------------

.. configuration-block::

    .. code-block:: yaml

        framework:

            # configuration générale
            trust_proxy_headers:  false
            secret:               ~ # Requis
            ide:                  ~
            test:                 ~
            default_locale:       en

            # configuration des formulaires
            form:
                enabled:              true
            csrf_protection:
                enabled:              true
                field_name:           _token

            # configuration esi 
            esi:
                enabled:              true

            # configuration du profileur
            profiler:
                only_exceptions:      false
                only_master_requests:  false
                dsn:                  file:%kernel.cache_dir%/profiler
                username:
                password:
                lifetime:             86400
                matcher:
                    ip:                   ~

                    # utilise le format urldecoded
                    path:                 ~ # Exemple: ^/path to resource/
                    service:              ~

            # configuration du routeur
            router:
                resource:             ~ # Required
                type:                 ~
                http_port:            80
                https_port:           443
                # si false, une URL vierge sera générée si une route a un paramètre obligatoire manquant
                strict_requirements:  %kernel.debug%

            # configuration de la session
            session:
                auto_start:           false
                storage_id:           session.storage.native
                handler_id:           session.handler.native_file
                name:                 ~
                cookie_lifetime:      ~
                cookie_path:          ~
                cookie_domain:        ~
                cookie_secure:        ~
                cookie_httponly:      ~
                gc_divisor:           ~
                gc_probability:       ~
                gc_maxlifetime:       ~
                save_path:            %kernel.cache_dir%/sessions

                # DEPRECIE ! utilisez : cookie_lifetime
                lifetime:             ~

                # DEPRECIE ! utilisez : cookie_path
                path:                 ~

                # DEPRECIE ! utilisez : cookie_domain
                domain:               ~

                # DEPRECIE ! utilisez : cookie_secure
                secure:               ~

                # DEPRECIE ! utilisez : cookie_httponly
                httponly:             ~

            # Configuration du moteur du templating
            templating:
                assets_version:       ~
                assets_version_format:  %%s?%%s
                hinclude_default_template:  ~
                form:
                    resources:

                        # Par défaut:
                        - FrameworkBundle:Form
                assets_base_urls:
                    http:                 []
                    ssl:                  []
                cache:                ~
                engines:              # Requis

                    # Exemple:
                    - twig
                loaders:              []
                packages:

                    # Une collection de noms de packages
                    some_package_name:
                        version:              ~
                        version_format:       %%s?%%s
                        base_urls:
                            http:                 []
                            ssl:                  []

            # configuration du traducteur
            translator:
                enabled:              true
                fallback:             en

            # configuration de la validation
            validation:
                enabled:              true
                cache:                ~
                enable_annotations:   false

            # configuration des annotations
            annotations:
                cache:                file
                file_cache_dir:       "%kernel.cache_dir%/annotations"
                debug:                true

.. _`relative à un protocole`: http://tools.ietf.org/html/rfc3986#section-4.2
.. _`sprintf()`: http://php.net/manual/en/function.sprintf.php
