.. index::
   single: Session; Database Storage

Comment stocker les sessions dans la base de données grâce à PdoSessionStorage
==============================================================================
.. caution::

    Symfony 2.6 entraine une rupture de la rétro-compatibilité : le modèle de données a légèrement changé. Voir :ref:`Symfony 2.6 Changes <pdo-session-handle-26-changes>`
    pour plus d'informations.

Le gestionnaire de sessions de Symfony stocke par défaut les sessions dans des fichiers. La plupart des sites
de moyenne et grande envergure vont cependant vouloir stocker les sessions dans la base de données
plutôt que dans des fichiers, car l'usage des bases de données permet plus facilement la
gestion de la montée en charge dans un environnement multi-serveurs.

Symfony incorpore une solution de stockage de sessions dans la base de données appelée
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\PdoSessionHandler`.
Pour l'utiliser, il vous suffit de changer quelques paramètres dans ``config.yml``
(ou le format de configuration de votre choix):

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                # ...
                handler_id: session.handler.pdo

        services:
            session.handler.pdo:
                class:     Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler
                public:    false
                arguments:
                    - "mysql:dbname=mydatabase"
                    - { db_username: myuser, db_password: mypassword }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session handler-id="session.handler.pdo" cookie-lifetime="3600" auto-start="true"/>
        </framework:config>

        <services>
            <service id="session.handler.pdo" class="Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler" public="false">
                <argument>mysql:dbname=mydatabase</agruement>
                <argument type="collection">
                    <argument key="db_username">myuser</argument>
                    <argument key="db_password">mypassword</argument>
                </argument>
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->loadFromExtension('framework', array(
            ...,
            'session' => array(
                // ...,
                'handler_id' => 'session.handler.pdo',
            ),
        ));

        $storageDefinition = new Definition('Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler', array(
            'mysql:dbname=mydatabase',
            array('db_username' => 'myuser', 'db_password' => 'mypassword')
        ));
        $container->setDefinition('session.handler.pdo', $storageDefinition);

Configurer la Table et les noms des colonnes
--------------------------------------------

Le nom de la table, ainsi que le nom de ses colonnes, peut être configuré en 
passant un deuxième argument au ``PdoSessionHandler``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            # ...
            session.handler.pdo:
                class:     Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler
                public:    false
                arguments:
                    - "mysql:dbname=mydatabase"
                    - { db_table: sessions, db_username: myuser, db_password: mypassword }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
            <service id="session.handler.pdo" class="Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler" public="false">
                <argument>mysql:dbname=mydatabase</agruement>
                <argument type="collection">
                    <argument key="db_table">sessions</argument>
                    <argument key="db_username">myuser</argument>
                    <argument key="db_password">mypassword</argument>
                </argument>
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php

        use Symfony\Component\DependencyInjection\Definition;
        // ...

        $storageDefinition = new Definition('Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler', array(
            'mysql:dbname=mydatabase',
            array('db_table' => 'sessions', 'db_username' => 'myuser', 'db_password' => 'mypassword')
        ));
        $container->setDefinition('session.handler.pdo', $storageDefinition);

.. versionadded:: 2.6
    La colonne  ``db_lifetime_col`` a été ajoutée dans Symfony 2.6. Avant la version 2.6,
    cette colonne n'existait pas.

Il est possible de configurer les informations suivantes:

* ``db_table``: (par défaut: ``sessions``) Le nom de la table de session dans votre base de données;
* ``db_id_col``: (par défaut: ``sess_id``) Le nom de la colonne id dans 
  votre table de session (VARCHAR(128));
* ``db_data_col``: (par défaut: ``sess_data``) Le nom de la colonne value dans
  votre table de session (BLOB);
* ``db_time_col``: (par défaut: ``sess_time``) Le nom de la colonne time dans
  votre table de session (INTEGER);
* ``db_lifetime_col``: (par défaut: ``sess_lifetime``) Le nom de la colonne lifetime
  votre table de session (INTEGER).

Partager les informations de connection à la base de données
------------------------------------------------------------

Avec cette configuration, les paramètres de connexion à la base de données ne concernent
que le stockage des sessions. Ceci peut fonctionner si vous dédiez une base de données aux sessions.

Mais si vous désirez stocker les informations de session dans la même base de données
que le reste des données du projet, vous pouvez réutiliser les paramètres de connexion définis dans
dans ``parameters.yml`` en référençant lesdits paramètres :

.. configuration-block::

    .. code-block:: yaml

        services:
            session.handler.pdo:
                class:     Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler
                public:    false
                arguments:
                    - "mysql:host=%database_host%;port=%database_port%;dbname=%database_name%"
                    - { db_username: %database_user%, db_password: %database_password% }

    .. code-block:: xml

        <service id="session.handler.pdo" class="Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler" public="false">
            <argument>mysql:host=%database_host%;port=%database_port%;dbname=%database_name%</agruement>
            <argument type="collection">
                <argument key="db_username">%database_user%</argument>
                <argument key="db_password">%database_password%</argument>
            </argument>
        </service>

    .. code-block:: php

        $storageDefinition = new Definition('Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler', array(
            'mysql:host=%database_host%;port=%database_port%;dbname=%database_name%',
            array('db_username' => '%database_user%', 'db_password' => '%database_password%')
        ));

Exemple d'instruction SQL
-------------------------

.. _pdo-session-handle-26-changes:

.. sidebar:: Mise à jour de la base de données nécésessaire lors du passage à Symfony 2.6

    Lors du passage à Symfony 2.6, en cas d'utilisation de ``PdoSessionHandler``, une
    mise à jour de la table session sera nécessaire:

    * Ajout de la colonne session lifetime (``sess_lifetime`` par défaut) de type integer
      devra être ajoutée;
    * La colonne data (``sess_data`` par défaut) devra passer en type BLOB.

    Se référer aux instructions SQL ci-dessous pour plus d'informations.

    Pour garder un fonctionnement identique aux versions 2.5 et précédentes de Symfony, il suffit de modifier la classe  
    ``LegacyPdoSessionHandler`` au lieu de ``PdoSessionHandler`` (cette
    classe a été ajoutée dans la version Symfony 2.6.2).

MySQL
~~~~~

L'instruction SQL pour la création d'une table de sessions sera probablement proche de :

.. code-block:: sql

    CREATE TABLE `sessions` (
        `sess_id` VARBINARY(128) NOT NULL PRIMARY KEY,
        `sess_data` BLOB NOT NULL,
        `sess_time` INTEGER UNSIGNED NOT NULL,
        `sess_lifetime` MEDIUMINT NOT NULL
    ) COLLATE utf8_bin, ENGINE = InnoDB;

.. note::

    La colonne ``BLOB`` ne peut stocker que 64 kb. Si les données stockées dans
    une session utilisateur dépasse cette taille, une exception pourrait être levée
    ou la session utilisateur pourrait être réinitialisée. 
    Il est possible d'utiliser le type  ``MEDIUMBLOB`` si plus d'espace est nécessaire.

PostgreSQL
~~~~~~~~~~

Pour PostgreSQL, ce sera plutôt:

.. code-block:: sql

    CREATE TABLE sessions (
        sess_id VARCHAR(128) NOT NULL PRIMARY KEY,
        sess_data BYTEA NOT NULL,
        sess_time INTEGER NOT NULL,
        sess_lifetime INTEGER NOT NULL
    );

Microsoft SQL Server
~~~~~~~~~~~~~~~~~~~~

Pour MSSQL:

.. code-block:: sql

    CREATE TABLE [dbo].[sessions](
        [sess_id] [nvarchar](255) NOT NULL,
        [sess_data] [ntext] NOT NULL,
        [sess_time] [int] NOT NULL,
        [sess_lifetime] [int] NOT NULL,
        PRIMARY KEY CLUSTERED(
            [sess_id] ASC
        ) WITH (
            PAD_INDEX  = OFF,
            STATISTICS_NORECOMPUTE  = OFF,
            IGNORE_DUP_KEY = OFF,
            ALLOW_ROW_LOCKS  = ON,
            ALLOW_PAGE_LOCKS  = ON
        ) ON [PRIMARY]
    ) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

