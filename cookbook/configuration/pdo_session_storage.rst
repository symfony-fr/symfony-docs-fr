.. index::
   single: Session; Database Storage

Comment stocker les sessions dans la base de données grâce à PdoSessionStorage
==============================================================================

Par défaut, Symfony2 stocke les sessions dans des fichiers. La plupart des sites
de moyenne et grande envegure vont cependant vouloir stocker les sessions dans la base de données
plutôt que dans des fichiers, car l'usage des bases de données permet plus facilement la
gestion de la montée en charge dans un environnement multi-serveurs.

Symfony2 incorpore une solution de stockage de sessions dans la base de données appelée
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\PdoSessionStorage`.
Pour l'utiliser, il vous suffit de changer quelques paramètres dans ``config.yml``
(ou le format de configuration de votre choix):

.. versionadded:: 2.1
    Dans Symfony2.1 la classe et l'espace de noms ont évolué. Vous trouvez dorénavant
    la classe `PdoSessionStorage` dans l'espace `Session\\Storage` :
    ``Symfony\Component\HttpFoundation\Session\Storage\PdoSessionStorage``.
    De même, les second et troisième arguments du constructeur ont été permutés.
    Vous noterez ci-dessous que les arguments ``%session.storage.options%`` et ``%pdo.db_options%``
    ont échangé leurs positions.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                # ...
                handler_id:     session.storage.pdo

        parameters:
            pdo.db_options:
                db_table:    session
                db_id_col:   session_id
                db_data_col: session_value
                db_time_col: session_time

        services:
            pdo:
                class: PDO
                arguments:
                    dsn:      "mysql:dbname=mydatabase"
                    user:     myuser
                    password: mypassword

            session.storage.pdo:
                class:     Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler
                arguments: [@pdo, %pdo.db_options%, %session.storage.options%]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session handler-id="session.storage.pdo" lifetime="3600" auto-start="true"/>
        </framework:config>

        <parameters>
            <parameter key="pdo.db_options" type="collection">
                <parameter key="db_table">session</parameter>
                <parameter key="db_id_col">session_id</parameter>
                <parameter key="db_data_col">session_value</parameter>
                <parameter key="db_time_col">session_time</parameter>
            </parameter>
        </parameters>

        <services>
            <service id="pdo" class="PDO">
                <argument>mysql:dbname=mydatabase</argument>
                <argument>myuser</argument>
                <argument>mypassword</argument>
            </service>

            <service id="session.storage.pdo" class="Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler">
                <argument type="service" id="pdo" />
                <argument>%pdo.db_options%</argument>
                <argument>%session.storage.options%</argument>
            </service>
        </services>

    .. code-block:: php

        // app/config/config.yml
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->loadFromExtension('framework', array(
            // ...
            'session' => array(
                // ...
                'handler_id' => 'session.storage.pdo',
            ),
        ));

        $container->setParameter('pdo.db_options', array(
            'db_table'      => 'session',
            'db_id_col'     => 'session_id',
            'db_data_col'   => 'session_value',
            'db_time_col'   => 'session_time',
        ));

        $pdoDefinition = new Definition('PDO', array(
            'mysql:dbname=mydatabase',
            'myuser',
            'mypassword',
        ));
        $container->setDefinition('pdo', $pdoDefinition);

        $storageDefinition = new Definition('Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler', array(
            new Reference('pdo'),
            '%pdo.db_options%',
            '%session.storage.options%',
        ));
        $container->setDefinition('session.storage.pdo', $storageDefinition);

* ``db_table`` : Nom de la table des sessions dans votre base de données
* ``db_id_col`` : Nom de la colonne identifiant dans la table des sessions (de type VARCHAR(255) ou plus)
* ``db_data_col`` : Nom de la colonne des valeurs dans la table des sessions (de type TEXT ou CLOB)
* ``db_time_col`` : Nom de la colonne temps dans la table des sessions (INTEGER)

Partager les informations de connection à la base de données
------------------------------------------------------------

Avec cette configuration, les paramètres de connexion à la base de données ne concernent
que le stockage des sessions. Ceci peut fonctionner si vous dédiez une base de données aux sessions.

Mais si vous désirez stocker les informations de session dans la même base de données
que le reste des données du projet, vous pouvez réutiliser les paramètres de connexion définis dans
dans ``parameter.ini`` en référençant lesdits paramètres :

.. configuration-block::

    .. code-block:: yaml

        pdo:
            class: PDO
            arguments:
                - "mysql:dbname=%database_name%"
                - %database_user%
                - %database_password%

    .. code-block:: xml

        <service id="pdo" class="PDO">
            <argument>mysql:dbname=%database_name%</argument>
            <argument>%database_user%</argument>
            <argument>%database_password%</argument>
        </service>

    .. code-block:: php

        $pdoDefinition = new Definition('PDO', array(
            'mysql:dbname=%database_name%',
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
        `session_id` varchar(255) NOT NULL,
        `session_value` text NOT NULL,
        `session_time` int(11) NOT NULL,
        PRIMARY KEY (`session_id`)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;

PostgreSQL
~~~~~~~~~~

Pour PostgreSQL, ce sera plutôt :

.. code-block:: sql

    CREATE TABLE session (
        session_id character varying(255) NOT NULL,
        session_value text NOT NULL,
        session_time integer NOT NULL,
        CONSTRAINT session_pkey PRIMARY KEY (session_id)
    );
