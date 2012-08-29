DoctrineMongoDBBundle Configuration
===================================

Exemple de configuration
------------------------

.. code-block:: yaml

    # app/config/config.yml
    doctrine_mongodb:
        connections:
            default:
                server: mongodb://localhost:27017
                options:
                    connect: true
        default_database: hello_%kernel.environment%
        document_managers:
            default:
                mappings:
                    AcmeDemoBundle: ~
                metadata_cache_driver: array # array, apc, xcache, memcache

Si vous voulez utiliser memcache pour mettre vos métadonnées en cache, vous
devez configurer l'instance ``Memcache``; par exemple, vous pouvez procéder
comme ceci :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_mongodb:
            default_database: hello_%kernel.environment%
            connections:
                default:
                    server: mongodb://localhost:27017
                    options:
                        connect: true
            document_managers:
                default:
                    mappings:
                        AcmeDemoBundle: ~
                    metadata_cache_driver:
                        type: memcache
                        class: Doctrine\Common\Cache\MemcacheCache
                        host: localhost
                        port: 11211
                        instance_class: Memcache

    .. code-block:: xml

        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine_mongodb="http://symfony.com/schema/dic/doctrine/odm/mongodb"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine/odm/mongodb http://symfony.com/schema/dic/doctrine/odm/mongodb/mongodb-1.0.xsd">

            <doctrine_mongodb:config default-database="hello_%kernel.environment%">
                <doctrine_mongodb:document-manager id="default">
                    <doctrine_mongodb:mapping name="AcmeDemoBundle" />
                    <doctrine_mongodb:metadata-cache-driver type="memcache">
                        <doctrine_mongodb:class>Doctrine\Common\Cache\MemcacheCache</doctrine_mongodb:class>
                        <doctrine_mongodb:host>localhost</doctrine_mongodb:host>
                        <doctrine_mongodb:port>11211</doctrine_mongodb:port>
                        <doctrine_mongodb:instance-class>Memcache</doctrine_mongodb:instance-class>
                    </doctrine_mongodb:metadata-cache-driver>
                </doctrine_mongodb:document-manager>
                <doctrine_mongodb:connection id="default" server="mongodb://localhost:27017">
                    <doctrine_mongodb:options>
                        <doctrine_mongodb:connect>true</doctrine_mongodb:connect>
                    </doctrine_mongodb:options>
                </doctrine_mongodb:connection>
            </doctrine_mongodb:config>
        </container>

Configuration de mapping 
~~~~~~~~~~~~~~~~~~~~~~~~

La seule configuration obligatoire pour l'ODM est la définition explicite
de tout les documents associés, et il y a plusieurs options de configuration
que vous pouvez contrôler. Les options suivantes existent pour un mapping :

- ``type`` Une des valeurs ``annotations``, ``xml``, ``yml``, ``php`` ou ``staticphp``.
  Elle spécifie quel type de métadonnées utilise votre mapping.

- ``dir`` Chemin vers le mapping ou le fichier entité (selon le driver). Si ce
  chemin est relatif, il est supposé être relatif par rapport à la racine du
  bundle. Cela ne fonctionne que si le nom de votre mapping est le nom du bundle.
  Si vous voulez utiliser cette option pour spécifier des chemins absolus, vous
  devrez préfixer le chemin avec les paramètres du kernel qui existent dans le
  conteneur d'injection de dépendance (par exemple %kernel.root_dir%).

- ``prefix`` Un préfixe d'espace de nom que tout les documents du mapping
  utilisent. Ce prefixe ne devrait jamais entrer en conflit avec les préfixes
  des autres mapping définis sinon certains de vos documents ne seront retrouvés
  par Doctrine. La valeur par défaut de cette option est l'espace de nom du bundle
  + ``Document``. Par exemple, pour un bundle appelé ``AcmeHelloBundle``, le préfixe
  serait ``Acme\HelloBundle\Document``.

- ``alias`` Doctrine propose une manière simple de spécifier des alias pour
  les espaces de nom de vos documents afin que des noms plus simples et plus courts
  soient utilisés dans les requêtes ou les accès au Repository.

- ``is_bundle`` Cette option est une valeur dérivée de ``dir`` et vaut true par défaut
  si ``dir`` est relative, c'est-à-dire si ``file_exists()`` retourne false. Elle vaudra
  false si la vérification de ``file_exists()`` retourne true. Dans ce cas, un chemin
  absolu a été spécifié et les fichiers de métadonnées sont vraisemblablement dans un
  dossier externe au bundle.

Pour éviter de devoir configurer plein d'informations pour votre mapping, vous
devriez suivre ces conventions :

1. Mettez tout vos documents dans un répertoire ``Document/`` dans votre bundle.
   Par exemple ``Acme/HelloBundle/Document/``.

2. Si vous utilisez un mapping xml, yml ou php, mettez tout les fichiers de
   configuration dans le répertoire ``Resources/config/doctrine/`` avec pour
   suffixes respectif mongodb.xml, mongodb.yml ou mongodb.php.

3. On suppose que les annotations sont utilisées si un répertoire ``Document/``
   existe mais qu'aucun répertoire ``Resources/config/doctrine/`` n'est trouvé.

La configuration suivante montre quelques exemples de mapping :

.. code-block:: yaml

    doctrine_mongodb:
        document_managers:
            default:
                mappings:
                    MyBundle1: ~
                    MyBundle2: yml
                    MyBundle3: { type: annotation, dir: Documents/ }
                    MyBundle4: { type: xml, dir: Resources/config/doctrine/mapping }
                    MyBundle5:
                        type: yml
                        dir: my-bundle-mappings-dir
                        alias: BundleAlias
                    doctrine_extensions:
                        type: xml
                        dir: %kernel.root_dir%/../src/vendor/DoctrineExtensions/lib/DoctrineExtensions/Documents
                        prefix: DoctrineExtensions\Documents\
                        alias: DExt

Connexions multiples
~~~~~~~~~~~~~~~~~~~~

Si vous avez besoin de plusieurs connexion et gestionnaires de document,
vous pouvez utiliser la syntaxe suivante :

.. configuration-block

    .. code-block:: yaml

        doctrine_mongodb:
            default_database: hello_%kernel.environment%
            default_connection: conn2
            default_document_manager: dm2
            metadata_cache_driver: apc
            connections:
                conn1:
                    server: mongodb://localhost:27017
                    options:
                        connect: true
                conn2:
                    server: mongodb://localhost:27017
                    options:
                        connect: true
            document_managers:
                dm1:
                    connection: conn1
                    metadata_cache_driver: xcache
                    mappings:
                        AcmeDemoBundle: ~
                dm2:
                    connection: conn2
                    mappings:
                        AcmeHelloBundle: ~

    .. code-block:: xml

        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine_mongodb="http://symfony.com/schema/dic/doctrine/odm/mongodb"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine/odm/mongodb http://symfony.com/schema/dic/doctrine/odm/mongodb/mongodb-1.0.xsd">

            <doctrine_mongodb:config
                    default-database="hello_%kernel.environment%"
                    default-document-manager="dm2"
                    default-connection="dm2"
                    proxy-namespace="MongoDBODMProxies"
                    auto-generate-proxy-classes="true">
                <doctrine_mongodb:connection id="conn1" server="mongodb://localhost:27017">
                    <doctrine_mongodb:options>
                        <doctrine_mongodb:connect>true</doctrine_mongodb:connect>
                    </doctrine_mongodb:options>
                </doctrine_mongodb:connection>
                <doctrine_mongodb:connection id="conn2" server="mongodb://localhost:27017">
                    <doctrine_mongodb:options>
                        <doctrine_mongodb:connect>true</doctrine_mongodb:connect>
                    </doctrine_mongodb:options>
                </doctrine_mongodb:connection>
                <doctrine_mongodb:document-manager id="dm1" metadata-cache-driver="xcache" connection="conn1">
                    <doctrine_mongodb:mapping name="AcmeDemoBundle" />
                </doctrine_mongodb:document-manager>
                <doctrine_mongodb:document-manager id="dm2" connection="conn2">
                    <doctrine_mongodb:mapping name="AcmeHelloBundle" />
                </doctrine_mongodb:document-manager>
            </doctrine_mongodb:config>
        </container>

Maintenant, vous pouvez retrouver les services de connexion configurés::

    $conn1 = $container->get('doctrine_mongodb.odm.conn1_connection');
    $conn2 = $container->get('doctrine_mongodb.odm.conn2_connection');

Et vous pouvez également retrouver les services du gestionnaire de document
configuré qui utilise les services de connexion ci-dessus::

    $dm1 = $container->get('doctrine_mongodb.odm.dm1_document_manager');
    $dm2 = $container->get('doctrine_mongodb.odm.dm2_document_manager');

Connexion à un pool de serveurs mongodb en 1 connexion
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il est possible de configurer plusieurs serveurs mongodb en une connexion,
si vous utilisez un ensemble répliqué, en listant tout les serveurs dans
la chaine qui configure la connexion, et en les séparant par une virgule.

.. configuration-block::

    .. code-block:: yaml

        doctrine_mongodb:
            # ...
            connections:
                default:
                    server: 'mongodb://mongodb-01:27017,mongodb-02:27017,mongodb-03:27017'

Où mongodb-01, mongodb-02 et mongodb-03 sont les noms d'hôte des machines. Vous
pouvez aussi utiliser les adresses IP si vous préférez.

Configuration par défaut complète
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        doctrine_mongodb:
            document_managers:

                # Prototype
                id:
                    connection:           ~
                    database:             ~
                    logging:              true
                    auto_mapping:         false
                    metadata_cache_driver:
                        type:                 ~
                        class:                ~
                        host:                 ~
                        port:                 ~
                        instance_class:       ~
                    mappings:

                        # Prototype
                        name:
                            mapping:              true
                            type:                 ~
                            dir:                  ~
                            prefix:               ~
                            alias:                ~
                            is_bundle:            ~
            connections:

                # Prototype
                id:
                    server:               ~
                    options:
                        connect:              ~
                        persist:              ~
                        timeout:              ~
                        replicaSet:           ~
                        username:             ~
                        password:             ~
            proxy_namespace:      MongoDBODMProxies
            proxy_dir:            %kernel.cache_dir%/doctrine/odm/mongodb/Proxies
            auto_generate_proxy_classes:  false
            hydrator_namespace:   Hydrators
            hydrator_dir:         %kernel.cache_dir%/doctrine/odm/mongodb/Hydrators
            auto_generate_hydrator_classes:  false
            default_document_manager:  ~
            default_connection:   ~
            default_database:     default
