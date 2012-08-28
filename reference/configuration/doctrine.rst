.. index::
   single: Doctrine; Configuration de référence de l'ORM
   single: Configuration de référence; ORM Doctrine

Configuration de Référence
==========================

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                default_connection:   default
                types:
                    # Collection de types personnalisés
                    # Exemple
                    some_custom_type:
                        class:                Acme\HelloBundle\MyCustomType
                        commented:            true

                connections:
                    default:
                        dbname:               database

                    # Collection de différents noms de connexions
                    # (par exemple : default, conn2, etc)
                    default:
                        dbname:               ~
                        host:                 localhost
                        port:                 ~
                        user:                 root
                        password:             ~
                        charset:              ~
                        path:                 ~
                        memory:               ~

                        # Le socket Unix à utiliser pour MySQL
                        unix_socket:          ~

                        # True pour une connexion persistente pour le driver ibm_db2
                        persistent:           ~

                        # Le protocole à utiliser pour le driver ibm_db2 (par défaut : TCPIP)
                        protocol:             ~

                        # True pour utiliser dbname comme nom de service au lieu de SID pour Oracle
                        service:              ~

                        # Le mode de session à utiliser pour le driver oci8
                        sessionMode:          ~

                        # True pour utiliser un pooled server avec le driver oci8
                        pooled:               ~

                        # Configure MultipleActiveResultSets pour le driver pdo_sqlsrv
                        MultipleActiveResultSets:  ~
                        driver:               pdo_mysql
                        platform_service:     ~
                        logging:              %kernel.debug%
                        profiling:            %kernel.debug%
                        driver_class:         ~
                        wrapper_class:        ~
                        options:
                            # Un tableau d'options
                            key:                  []
                        mapping_types:
                            # Un tableau de types de mapping
                            name:                 []
                        slaves:

                            # Une collection de noms de connexions esclaves (par exemple : slave1, slave2)
                            slave1:
                                dbname:               ~
                                host:                 localhost
                                port:                 ~
                                user:                 root
                                password:             ~
                                charset:              ~
                                path:                 ~
                                memory:               ~

                                # Le socket Unix à utiliser avec MySQL
                                unix_socket:          ~

                                # True pour une connexion persistante pour le driver ibm_db2
                                persistent:           ~

                                # Le protocole à utiliser pour le driver ibm_db2 (par défaut : TCPIP)
                                protocol:             ~

                                # True pour utiliser dbname comme nom de service au lieu de SID pour Oracle
                                service:              ~

                                # Le mode de session à utiliser pour le driver oci8
                                sessionMode:          ~

                                # True pour utiliser un pooled server avec le driver oci8
                                pooled:               ~

                                # Configure MultipleActiveResultSets pour le driver pdo_sqlsrv
                                MultipleActiveResultSets:  ~

            orm:
                default_entity_manager:  ~
                auto_generate_proxy_classes:  false
                proxy_dir:            %kernel.cache_dir%/doctrine/orm/Proxies
                proxy_namespace:      Proxies
                # cherchez la classe "ResolveTargetEntityListener" pour avoir un mode d'emploi
                resolve_target_entities: []
                entity_managers:
                    # Une collection de différents noms de gestionnaires d'entités (par exemple : some_em, another_em)
                    some_em:
                        query_cache_driver:
                            type:                 array # Requis
                            host:                 ~
                            port:                 ~
                            instance_class:       ~
                            class:                ~
                        metadata_cache_driver:
                            type:                 array # Requis
                            host:                 ~
                            port:                 ~
                            instance_class:       ~
                            class:                ~
                        result_cache_driver:
                            type:                 array # Requis
                            host:                 ~
                            port:                 ~
                            instance_class:       ~
                            class:                ~
                        connection:           ~
                        class_metadata_factory_name:  Doctrine\ORM\Mapping\ClassMetadataFactory
                        default_repository_class:  Doctrine\ORM\EntityRepository
                        auto_mapping:         false
                        hydrators:

                            # Un tableau de noms d'hydrateurs
                            hydrator_name:                 []
                        mappings:
                            # Un tableau de mapping, qui peut être un nom de bundle ou autre chose
                            mapping_name:
                                mapping:              true
                                type:                 ~
                                dir:                  ~
                                alias:                ~
                                prefix:               ~
                                is_bundle:            ~
                        dql:
                            # Une collection de fonctions de chaînes de caractères
                            string_functions:
                                # exemple
                                # test_string: Acme\HelloBundle\DQL\StringFunction

                            # Une collection de fonctions numériques
                            numeric_functions:
                                # exemple
                                # test_numeric: Acme\HelloBundle\DQL\NumericFunction

                            # Une collection de fonctions datetime
                            datetime_functions:
                                # exemple
                                # test_datetime: Acme\HelloBundle\DQL\DatetimeFunction

                        # Enregistre les filtres SQL du gestionnaire d'entités
                        filters:
                            # Un tableau de filtres
                            some_filter:
                                class:                ~ # Requis
                                enabled:              false

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection
                        name="default"
                        dbname="database"
                        host="localhost"
                        port="1234"
                        user="user"
                        password="secret"
                        driver="pdo_mysql"
                        driver-class="MyNamespace\MyDriverImpl"
                        path="%kernel.data_dir%/data.sqlite"
                        memory="true"
                        unix-socket="/tmp/mysql.sock"
                        wrapper-class="MyDoctrineDbalConnectionWrapper"
                        charset="UTF8"
                        logging="%kernel.debug%"
                        platform-service="MyOwnDatabasePlatformService"
                    >
                        <doctrine:option key="foo">bar</doctrine:option>
                        <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                    </doctrine:connection>
                    <doctrine:connection name="conn1" />
                    <doctrine:type name="custom">Acme\HelloBundle\MyCustomType</doctrine:type>
                </doctrine:dbal>

                <doctrine:orm default-entity-manager="default" auto-generate-proxy-classes="false" proxy-namespace="Proxies" proxy-dir="%kernel.cache_dir%/doctrine/orm/Proxies">
                    <doctrine:entity-manager name="default" query-cache-driver="array" result-cache-driver="array" connection="conn1" class-metadata-factory-name="Doctrine\ORM\Mapping\ClassMetadataFactory">
                        <doctrine:metadata-cache-driver type="memcache" host="localhost" port="11211" instance-class="Memcache" class="Doctrine\Common\Cache\MemcacheCache" />
                        <doctrine:mapping name="AcmeHelloBundle" />
                        <doctrine:dql>
                            <doctrine:string-function name="test_string>Acme\HelloBundle\DQL\StringFunction</doctrine:string-function>
                            <doctrine:numeric-function name="test_numeric>Acme\HelloBundle\DQL\NumericFunction</doctrine:numeric-function>
                            <doctrine:datetime-function name="test_datetime>Acme\HelloBundle\DQL\DatetimeFunction</doctrine:datetime-function>
                        </doctrine:dql>
                    </doctrine:entity-manager>
                    <doctrine:entity-manager name="em2" connection="conn2" metadata-cache-driver="apc">
                        <doctrine:mapping
                            name="DoctrineExtensions"
                            type="xml"
                            dir="%kernel.root_dir%/../vendor/gedmo/doctrine-extensions/lib/DoctrineExtensions/Entity"
                            prefix="DoctrineExtensions\Entity"
                            alias="DExt"
                        />
                    </doctrine:entity-manager>
                </doctrine:orm>
            </doctrine:config>
        </container>

Aperçu global de la Configuration
---------------------------------

L'exemple de configuration suivant montre toutes les options de configuration
par défaut que l'ORM utilise si non définies :

.. code-block:: yaml

    doctrine:
        orm:
            auto_mapping: true
            # la distribution standard outrepasse ceci pour être à « true » en mode débuggage,
            # à « false » sinon
            auto_generate_proxy_classes: false
            proxy_namespace: Proxies
            proxy_dir: %kernel.cache_dir%/doctrine/orm/Proxies
            default_entity_manager: default
            metadata_cache_driver: array
            query_cache_driver: array
            result_cache_driver: array

Il y a beaucoup d'autres options de configuration que vous pouvez utiliser
pour outrepasser certaines classes, mais celles-ci sont réservées seulement à
des cas d'utilisation très avancés.

Drivers de Cache
~~~~~~~~~~~~~~~~

Pour les drivers de cache, vous pouvez spécifier les valeurs « array », « apc »,
« memcache », « memcached », « xcache » ou « service ».

L'exemple suivant montre un aperçu global des options de configuration du cache :

.. code-block:: yaml

    doctrine:
        orm:
            auto_mapping: true
            metadata_cache_driver: apc
            query_cache_driver:  
                type: service   
                id: my_doctrine_common_cache_service
            result_cache_driver:
                type: memcache
                host: localhost
                port: 11211
                instance_class: Memcache

Configuration des Correspondances
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Avoir des définitions explicites pour toutes les correspondances d'entités
est l'unique configuration nécessaire pour l'ORM et il y a plusieurs options
de configuration que vous pouvez contrôler. Les options de configuration
suivantes existent pour les correspondances d'entités :

* ``type`` Une valeur parmi ``annotation``, ``xml``, ``yml``, ``php``
  ou ``staticphp``.
  Cela spécifie quel type de métadonnées vos correspondances utilisent.

* ``dir`` Chemin vers les correspondances ou fichiers d'entités (dépend du
  « driver »). Si ce chemin est relatif, alors on assume qu'il est relatif
  par rapport à la racine du bundle. Si vous voulez utiliser cette option
  pour spécifier des chemins absolus, vous devriez préfixer le chemin avec
  les paramètres du « kernel » qui existent dans le DIC (par exemple :
  %kernel.root_dir%).

* ``prefix`` Un préfixe commun d'espace de noms que toutes les entités de
  cette correspondance partagent. Le préfixe ne doit jamais être en
  conflit avec des préfixes d'autres correspondances d'entités définies
  sinon, certaines de vos entités ne seront pas trouvées par Doctrine. La
  valeur par défaut de cette option est l'espace de noms du bundle + ``Entity`` ;
  par exemple pour un bundle applicatif nommé ``AcmeHelloBundle``, le préfixe
  devrait être ``Acme\HelloBundle\Entity``.

* ``alias`` Doctrine offre une façon de créer des alias pour les espaces de
  noms afin d'avoir des noms plus simples et plus courts à utiliser dans les
  requêtes DQL ou dans les accès à un Repository. Quand vous utilisez un bundle,
  l'alias par défaut est le nom du bundle.

* ``is_bundle`` Cette option est une valeur dérivée de ``dir`` et est par
  défaut définie comme « true » si « dir » est prouvé comme existant grâce
  à une vérification via ``file_exists()`` qui retourne « false ». Cela est
  « false » si la vérification de présence retourne « true ». Dans ce cas,
  un chemin absolu est spécifié et les fichiers de métadonnées sont très
  certainement dans un répertoire en dehors de celui du bundle.

.. index::
    single: Configuration; DBAL Doctrine
    single: Doctrine; Configuration du DBAL

.. _`reference-dbal-configuration`:

Configuration du DBAL Doctrine
------------------------------

.. note::

    Le DoctrineBundle supporte tous les paramètres que les drivers Doctrine
    acceptent par défaut, convertis en XML ou YML selon les standards de
    nommage que Symfony force à utiliser. Voir la `Documentation DBAL`_ de
    Doctrine pour plus d'informations.

A côté des options de Doctrine par défaut, il y en a quelques unes liées à
Symfony que vous pouvez configurer. Le bloc suivant montre toutes les clés
de configuration possibles :

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                dbname:               database
                host:                 localhost
                port:                 1234
                user:                 user
                password:             secret
                driver:               pdo_mysql
                driver_class:         MyNamespace\MyDriverImpl
                options:
                    foo: bar
                path:                 %kernel.data_dir%/data.sqlite
                memory:               true
                unix_socket:          /tmp/mysql.sock
                wrapper_class:        MyDoctrineDbalConnectionWrapper
                charset:              UTF8
                logging:              %kernel.debug%
                platform_service:     MyOwnDatabasePlatformService
                mapping_types:
                    enum: string
                types:
                    custom: Acme\HelloBundle\MyCustomType

    .. code-block:: xml

        <!-- xmlns:doctrine="http://symfony.com/schema/dic/doctrine" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd"> -->

        <doctrine:config>
            <doctrine:dbal
                name="default"
                dbname="database"
                host="localhost"
                port="1234"
                user="user"
                password="secret"
                driver="pdo_mysql"
                driver-class="MyNamespace\MyDriverImpl"
                path="%kernel.data_dir%/data.sqlite"
                memory="true"
                unix-socket="/tmp/mysql.sock"
                wrapper-class="MyDoctrineDbalConnectionWrapper"
                charset="UTF8"
                logging="%kernel.debug%"
                platform-service="MyOwnDatabasePlatformService"
            >
                <doctrine:option key="foo">bar</doctrine:option>
                <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                <doctrine:type name="custom">Acme\HelloBundle\MyCustomType</doctrine:type>
            </doctrine:dbal>
        </doctrine:config>

Si vous voulez configurer plusieurs connexions en YAML, mettez-les sous la
clé ``connections`` et donnez leurs un nom unique :

.. code-block:: yaml

    doctrine:
        dbal:
            default_connection:       default
            connections:
                default:
                    dbname:           Symfony2
                    user:             root
                    password:         null
                    host:             localhost
                customer:
                    dbname:           customer
                    user:             root
                    password:         null
                    host:             localhost

Le service ``database_connection`` réfère toujours à la connexion *default*,
qui est la première définie ou celle configurée via le paramètre ``default_connection``.

Chaque connexion est aussi accessible via le service ``doctrine.dbal.[name]_connection``
où ``[name]`` est le nom de la connexion.

.. _Documentation DBAL: http://docs.doctrine-project.org/projects/doctrine-dbal/en/latest/index.html