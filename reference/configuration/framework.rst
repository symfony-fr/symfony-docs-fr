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
* `http_method_override`_
* `ide`_
* `test`_
* `trusted_proxies`_
* `form`_
    * enabled
* `csrf_protection`_
    * enabled
    * field_name
* `session`_
    * `name`_
    * `cookie_lifetime`_
    * `cookie_path`_
    * `cookie_domain`_
    * `cookie_secure`_
    * `cookie_httponly`_
    * `gc_divisor`_
    * `gc_probability`_
    * `gc_maxlifetime`_
    * `save_path`_
* `serializer`_
    * :ref:`enabled<serializer.enabled>`
* `templating`_
    * `assets_base_urls`_
    * `assets_version`_
    * `assets_version_format`_
* `profiler`_
    * `collect`_
    * :ref:`enabled<profiler.enabled>`

secret
~~~~~~

**type**: ``string`` **requis**

Ceci est une chaîne de caractères qui devrait être unique à votre application.
En pratique, elle est utilisée pour générer les jetons CSRF, mais elle pourrait
être utilisée dans un autre contexte où avoir une chaîne de caractères unique
est utile. Ceci devient le paramètre du conteneur de service nommé
``kernel.secret``.

.. _configuration-framework-http_method_override:

http_method_override
~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3

    L'option ``http_method_override`` est nouvelle depuis Symfony 2.3.

**type**: ``Boolean`` **default**: ``true``

Cela détermine quand le paramètre de la requète ``_method`` est utilisé comme la
méthode HTTP attendu pour les requêtes POST. Si activer, la méthode
:method:`Request::enableHttpMethodParameterOverride <Symfony\\Component\\HttpFoundation\\Request::enableHttpMethodParameterOverride>`
est appelée automatiquement. Cela devient un service avec le nom
``kernel.http_method_override``. Pour plus d'informations, lire
:doc:`/cookbook/routing/method_parameters`.

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

trusted_proxies
~~~~~~~~~~~~~~~

**type**: ``array``

Définit les adresses IP qui seront considéré comme des proxies. Pour plus de détails,
lire :doc:`/components/http_foundation/trusting_proxies`.

.. versionadded:: 2.3

    le support de la notation CIDR a été ajouté, vous pouvez donc mettre en liste blanche,
    un ensemble de sous réseaux (ex. ``10.0.0.0/8``, ``fc00::/7``).

.. configuration-block::

    .. code-block:: yaml

        framework:
            trusted_proxies:  [192.0.0.1, 10.0.0.0/8]

    .. code-block:: xml

        <framework:config trusted-proxies="192.0.0.1, 10.0.0.0/8">
            <!-- ... -->
        </framework>

    .. code-block:: php

        $container->loadFromExtension('framework', array(
            'trusted_proxies' => array('192.0.0.1', '10.0.0.0/8'),
        ));

.. _reference-framework-form:


form
~~~~

csrf_protection
~~~~~~~~~~~~~~~

session
~~~~~~~

name
....

**type**: ``string`` **default**: ``null``

Spécifie le nom du cookie de session. Par défaut, c'est celui qui dans définie
dans le fichier ``php.ini`` avec la directive ``session.name`` .

cookie_lifetime
...............

**type**: ``integer`` **default**: ``0``

Détermine la durée de vie de la session en secondes. Par défaut, ``0``
est utilisé, d'ou le cookie est valide le temps de la session du navigateur.

cookie_path
...........

**type**: ``string`` **default**: ``/``

Détermine le chemin où est définie le cookie de session. Par défaut, ``/``
est utilisé.

cookie_domain
.............

**type**: ``string`` **default**: ``''``

Définie le domaine pour le cookie de session. Par défaut, il est vide,
ce sera donc le nom d'hote qui a généré le cookie selon les spécifications.

cookie_secure
.............

**type**: ``Boolean`` **default**: ``false``

Détermine que les cookies soient envoyés seulement par des connexions sécurisée.

cookie_httponly
...............

**type**: ``Boolean`` **default**: ``false``

Détermine si les cookies doivent être seulement accessible par le protocole HTTP.
Ce qui a pour effet, de ne pas permettre l'accès par les langages de script, tels
que Javascript. Ce paramétrage a pour effet de réduire le vol d'identité à travers
les attaques de type XSS.

gc_probability
..............

**type**: ``integer`` **default**: ``1``

Détermine les probabilités que le processus ``garbage collector (GC)`` est
démarré à chaque initialisation de session. la probabilité est calculé en
utilisant ``gc_probability`` / ``gc_divisor``, ex. 1/100 signifie qu'il y a
1% de chance que le processus GC soit démarré à chaque requête.

gc_divisor
..........

**type**: ``integer`` **default**: ``100``

Lire `gc_probability`_.

gc_maxlifetime
..............

**type**: ``integer`` **default**: ``14400``

Détermine le nombre de secondes après lesquelles, la donnée est considérée comme
"garbage" et peut être nettoyé. "Garbage collection" peut apparaître durant le démarrage
de la session et dépend des paramètres `gc_divisor`_ et `gc_probability`_.

save_path
.........

**type**: ``string`` **default**: ``%kernel.cache.dir%/sessions``

Détermine l'argument à passer au gestionnaire de sauvegarde. Si vous choisissez
le gestionnaire de fichier par défaut, ce sera le chemin où seront créés les fichiers.
Vous pouvez définir cette valeur dans la directive ``save_path`` de votre ``php.ini``
en mettant la valeur de l'option à ``null``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                save_path: null

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session save-path="null" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array(
                'save_path' => null,
            ),
        ));

.. _configuration-framework-serializer:

serializer
~~~~~~~~~~

.. _serializer.enabled:

enabled
.......

**type**: ``boolean`` **default**: ``false``

Détermine si le service ``serializer`` est chargé dans le conteneur de service.

Pour plus de détails, lire :doc:`/cookbook/serializer`.

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

Cela spécifie un « pattern » de :phpfunction:`sprintf` qui va être utilisé avec l'option
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
    paramètre, vous ne pouvez pas le modifier sur place (par exemple :
    ``/images/logo-v5.png``) ; cependant, vous pouvez préfixer le chemin du
    fichier en utilisant un pattern comme ``version-%%2$s/%%1$s``, qui donnerait
    un chemin tel ``version-5/images/logo.png``.

    Des règles de réécriture d'URL pourraient alors être utilisées afin de ne pas
    tenir compte du préfixe de version avant de servir le fichier. Une autre
    alternative pourrait être de copier les fichiers dans le répertoire approprié
    de la version lors de votre procédure de déploiement et ainsi de ne pas
    avoir à créer quelconque règle de réécriture d'URL. La dernière option est
    utile si vous souhaitez laisser les anciennes versions des fichiers
    accessibles depuis leur URL originale.

profiler
~~~~~~~~

.. versionadded:: 2.2

    L'option ``enabled`` a été ajouté dans Symfony 2.2. Avant, pour désactiver le  profiler
    il fallait omettre complètement la clé de configuration ``framework.profiler``.

.. _profiler.enabled:

enabled
.......

**default**: ``true`` dans les environements ``dev`` et ``test``

Le profiler peut être désactivé en passant la clé à ``false``.

.. versionadded:: 2.3

    L'option ``collect`` est nouvelle dans Symfony 2.3. Avant quand ``profiler.enabled``
    était à ``false``, le profiler *était* activé, mais les collecteurs étaient
    désactivés. Maintenant, le profiler et les collecteurs peuvent être contrôlés séparément.

collect
.......

**default**: ``true``

Cette option permet de configurer la façon dont le profiler se comporte quand il est activé.
Si il est à ``true``, le profiler collecte toutes les données pour toutes les requêtes. Si vous
souhaitez collecter les informations seulement à la demande, vous pouvez définir le flag ``collect``
à ``false`` et activer le collecteur de données comme ceci::

    $profiler->enable();

Toutes les Options de Configuration par Défaut
----------------------------------------------

.. configuration-block::

    .. code-block:: yaml

        framework:
            secret:               ~
            http_method_override: true
            trusted_proxies:      []
            ide:                  ~
            test:                 ~
            default_locale:       en

            # configuration du composant form
            form:
                enabled:              false
            csrf_protection:
                enabled:              false
                field_name:           _token

            # configuration esi
            esi:
                enabled:              false

            # configuration des fragments de modèle
            fragments:
                enabled:              false
                path:                 /_fragment

            # configuration du profiler
            profiler:
                enabled:              false
                collect:              true
                only_exceptions:      false
                only_master_requests: false
                dsn:                  file:%kernel.cache_dir%/profiler
                username:
                password:
                lifetime:             86400
                matcher:
                    ip:                   ~

                    # utiliser le format urldecoded
                    path:                 ~ # Exemple: ^/path to resource/
                    service:              ~

            # configuration du router
            router:
                resource:             ~ # Required
                type:                 ~
                http_port:            80
                https_port:           443

                # défini à true lève une exception quand le paramètre ne correspondant pas aux exigences
                # défini à false désactive les exceptions lève quand le paramètre ne correspondant pas aux exigences (retourne null)
                # défini à null désactive la vérification du paramètre par rapport aux exigences
                # 'true est la configuration de préférence en développement, par contre 'false' ou 'null' est préférable en
                # production
                strict_requirements:  true

            # session configuration
            session:
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

            # configuration du composant serializer
            serializer:
               enabled: false

            # configuration du composant templating
            templating:
                assets_version:       ~
                assets_version_format:  %%s?%%s
                hinclude_default_template:  ~
                form:
                    resources:

                        # Défaut:
                        - FrameworkBundle:Form
                assets_base_urls:
                    http:                 []
                    ssl:                  []
                cache:                ~
                engines:              # Required

                    # Exemple:
                    - twig
                loaders:              []
                packages:

                    # Prototype
                    name:
                        version:              ~
                        version_format:       %%s?%%s
                        base_urls:
                            http:                 []
                            ssl:                  []

            # configuration du composant translator
            translator:
                enabled:              false
                fallback:             en

            # configuration du composant validation
            validation:
                enabled:              false
                cache:                ~
                enable_annotations:   false
                translation_domain:   validators

            # configuration des annotations
            annotations:
                cache:                file
                file_cache_dir:       %kernel.cache_dir%/annotations
                debug:                %kernel.debug%

.. _`relative à un protocole`: http://tools.ietf.org/html/rfc3986#section-4.2