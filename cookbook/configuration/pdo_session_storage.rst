.. index::
   single: Session; Database Storage

Comment stocker les sessions dans la base de données grâce à PdoSessionStorage
==============================================================================

Par défaut, Symfony2 stocke les sessions dans des fichiers. La plupart des sites
de moyenne et grande envergure vont cependant vouloir stocker les sessions dans la base de données
plutôt que dans des fichiers, car l'usage des bases de données permet plus facilement la
gestion de la montée en charge dans un environnement multi-serveurs.

Symfony2 incorpore une solution de stockage de sessions dans la base de données appelée
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\PdoSessionHandler`.
Pour l'utiliser, il vous suffit de changer quelques paramètres dans ``config.yml``
(ou le format de configuration de votre choix):

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                # ...
                handler_id:     session.handler.pdo

        parameters:
            pdo.db_options:
                db_table:        session
                db_id_col:       session_id
                db_data_col:     session_data
                db_time_col:     session_time
                db_lifetime_col: session_lifetime

        services:
            pdo:
                class: PDO
                arguments:
                    dsn:      "mysql:dbname=mydatabase"
                    user:     myuser
                    password: mypassword
                calls:
                    - [setAttribute, [3, 2]] # \PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION

            session.handler.pdo:
                class:     Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler
                arguments: [@pdo, %pdo.db_options%]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session handler-id="session.handler.pdo" lifetime="3600" auto-start="true"/>
        </framework:config>

        <parameters>
            <parameter key="pdo.db_options" type="collection">
                <parameter key="db_table">session</parameter>
                <parameter key="db_id_col">session_id</parameter>
                <parameter key="db_data_col">session_data</parameter>
                <parameter key="db_time_col">session_time</parameter>
                <parameter key="db_lifetime_col">session_lifetime</parameter>
            </parameter>
        </parameters>

        <services>
            <service id="pdo" class="PDO">
                <argument>mysql:dbname=mydatabase</argument>
                <argument>myuser</argument>
                <argument>mypassword</argument>
                <call method="setAttribute">
                    <argument type="constant">PDO::ATTR_ERRMODE</argument>
                    <argument type="constant">PDO::ERRMODE_EXCEPTION</argument>
                </call>
            </service>

            <service id="session.handler.pdo" class="Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler">
                <argument type="service" id="pdo" />
                <argument>%pdo.db_options%</argument>
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->loadFromExtension('framework', array(
            // ...
            'session' => array(
                ...,
                'handler_id' => 'session.handler.pdo',
            ),
        ));

        $container->setParameter('pdo.db_options', array(
            'db_table'        => 'session',
            'db_id_col'       => 'session_id',
            'db_data_col'     => 'session_data',
            'db_time_col'     => 'session_time',
            'db_lifetime_col' => 'session_lifetime',
        ));

        $pdoDefinition = new Definition('PDO', array(
            'mysql:dbname=mydatabase',
            'myuser',
            'mypassword',
        ));
        $pdoDefinition->addMethodCall('setAttribute', array(\PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION));
        $container->setDefinition('pdo', $pdoDefinition);

        $storageDefinition = new Definition('Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler', array(
            new Reference('pdo'),
            '%pdo.db_options%',
        ));
        $container->setDefinition('session.handler.pdo', $storageDefinition);

* ``db_table`` : Nom de la table des sessions dans votre base de données
* ``db_id_col`` : Nom de la colonne identifiant dans la table des sessions (VARCHAR(128))
* ``db_data_col`` : Nom de la colonne des valeurs dans la table des sessions (BLOB)
* ``db_time_col`` : Nom de la colonne temps dans la table des sessions (INTEGER)
* ``db_lifetime_col`` : Nom de la colonne durée de vie dans la table des sessions (INTEGER)

Partager les informations de connection à la base de données
------------------------------------------------------------

Avec cette configuration, les paramètres de connexion à la base de données ne concernent
que le stockage des sessions. Ceci peut fonctionner si vous dédiez une base de données aux sessions.

Mais si vous désirez stocker les informations de session dans la même base de données
que le reste des données du projet, vous pouvez réutiliser les paramètres de connexion définis
dans ``parameter.ini`` en référençant lesdits paramètres :

.. configuration-block::

    .. code-block:: yaml

        pdo:
            class: PDO
            arguments:
                - "mysql:host=%database_host%;port=%database_port%;dbname=%database_name%"
                - %database_user%
                - %database_password%

    .. code-block:: xml

        <service id="pdo" class="PDO">
            <argument>mysql:host=%database_host%;port=%database_port%;dbname=%database_name%</argument>
            <argument>%database_user%</argument>
            <argument>%database_password%</argument>
        </service>

    .. code-block:: php

        $pdoDefinition = new Definition('PDO', array(
            'mysql:host=%database_host%;port=%database_port%;dbname=%database_name%',
            '%database_user%',
            '%database_password%',
        ));

Exemple d'instruction SQL
-------------------------

MySQL
~~~~~

L'instruction SQL pour la création d'une table de sessions sera probablement proche de :

.. code-block:: sql

    CREATE TABLE `session` (
        `session_id` VARBINARY(128) NOT NULL PRIMARY KEY,
        `session_data` BLOB NOT NULL,
        `session_time` INTEGER UNSIGNED NOT NULL,
        `session_lifetime` MEDIUMINT NOT NULL
    ) COLLATE utf8_bin, ENGINE = InnoDB;

PostgreSQL
~~~~~~~~~~

Pour PostgreSQL, ce sera plutôt :

.. code-block:: sql

    CREATE TABLE session (
        session_id VARCHAR(128) NOT NULL PRIMARY KEY,
        session_data BYTEA NOT NULL,
        session_time INTEGER NOT NULL,
        session_lifetime INTEGER NOT NULL
    );
    
Microsoft SQL Server
~~~~~~~~~~~~~~~~~~~~

Pour MSSQL, ce sera plutôt :

.. code-block:: sql

    CREATE TABLE [dbo].[session](
        [session_id] [nvarchar](255) NOT NULL,
        [session_data] [ntext] NOT NULL,
        [session_time] [int] NOT NULL,
        [session_lifetime] [int] NOT NULL,
        PRIMARY KEY CLUSTERED(
            [session_id] ASC
        ) WITH (
            PAD_INDEX  = OFF,
            STATISTICS_NORECOMPUTE  = OFF,
            IGNORE_DUP_KEY = OFF,
            ALLOW_ROW_LOCKS  = ON,
            ALLOW_PAGE_LOCKS  = ON
        ) ON [PRIMARY]
    ) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
