.. index::
   single: Environments; External parameters

Comment configurer les paramètres externes dans le conteneur de services
========================================================================

Dans le chapitre doc:`/cookbook/configuration/environments`, Vous avez vu
comment gérer la configuration de votre application. Parfois on aura cependant besoin
de stocker certaines données hors du code du projet, par exemple des mots de passe, ou des
paramètres de configuration d'une base de données.
La flexibilité du conteneur de services Symfony vous le permet.

Variables d'environnement
-------------------------

Symfony va repérer toute variable d'environnement préfixée de ``SYMFONY__`` et
la stocker en tant que paramètre dans le conteneur de services. les doubles tirets bas sont remplacés
par un point, le point n'étant pas un caractère permis dans un nom de variable d'environnement.

Par exemple, si vous utilisez Apache, les variables d'environnement peuvent être définies
par la configuration ``VirtualHost`` suivante:

.. code-block:: apache

    <VirtualHost *:80>
        ServerName      Symfony2
        DocumentRoot    "/path/to/symfony_2_app/web"
        DirectoryIndex  index.php index.html
        SetEnv          SYMFONY__DATABASE__USER user
        SetEnv          SYMFONY__DATABASE__PASSWORD secret

        <Directory "/path/to/symfony_2_app/web">
            AllowOverride All
            Allow from All
        </Directory>
    </VirtualHost>

.. note::

    L'exemple de configuration ci-dessus concerne Apache, à l'aide de la directive
    `SetEnv`_. Cependant, ceci fonctionnera pour tout serveur permettant la définition
    de variables d'environnement.

    De même, afin de permettre l'usage de variables d'environnement par la console (sans serveur),
    il est nécessaire de définir lesdites variables comme des variables shell.

    Sur un système Unix:

    .. code-block:: bash

        $ export SYMFONY__DATABASE__USER=user
        $ export SYMFONY__DATABASE__PASSWORD=secret

Maintenant que les variables d'environnement ont été déclarées, elles seront présentes
dans la variable globale ``$_SERVER`` de PHP. Symfony va donc automatiquement recopier
les valeurs des variables ``$_SERVER`` préfixées de ``SYMFONY__`` en tant que paramètres
du conteneur de services.

Vous pourrez ainsi faire référence à ces paramètres si nécessaire.

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                driver    pdo_mysql
                dbname:   symfony2_project
                user:     %database.user%
                password: %database.password%

    .. code-block:: xml

        <!-- xmlns:doctrine="http://symfony.com/schema/dic/doctrine" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd"> -->

        <doctrine:config>
            <doctrine:dbal
                driver="pdo_mysql"
                dbname="symfony2_project"
                user="%database.user%"
                password="%database.password%"
            />
        </doctrine:config>

    .. code-block:: php

        $container->loadFromExtension('doctrine', array('dbal' => array(
            'driver'   => 'pdo_mysql',
            'dbname'   => 'symfony2_project',
            'user'     => '%database.user%',
            'password' => '%database.password%',
        ));

Constantes
----------

Le conteneur de services permet également la définition de constantes PHP comme paramètres.
Il suffit de faire correspondre le nom de votre constante à une clé de paramètre
et de préciser le type ``constant``.

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8"?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <parameters>
                <parameter key="global.constant.value" type="constant">GLOBAL_CONSTANT</parameter>
                <parameter key="my_class.constant.value" type="constant">My_Class::CONSTANT_NAME</parameter>
            </parameters>
        </container>

.. note::

    Ceci ne fonctionne qu'avec une configuration XML. Si vous *n'utilisez pas* XML
    pour la configuration, importez un fichier XML pour pouvoir le faire:

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: parameters.xml }

Diverses considérations
-----------------------

La directive ``imports`` peut être utilisée pour récupérer des paramètres stockés ailleurs.
L'import d'un fichier PHP vous permet un maximum de flexibilité dans le conteneur.
Le code suivant importe un fichier ``parameters.php``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: parameters.php }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <import resource="parameters.php" />
        </imports>

    .. code-block:: php

        // app/config/config.php
        $loader->import('parameters.php');

.. note::

    Un fichier de ressource peut être de plusieurs types. La directive ``imports`` accepte
    des ressources de type PHP, XML, YAML, INI, et closure.

Dans le fichier ``parameters.php``, vous allez indiquer au conteneur de services les paramètres
que vous désirez définir. Ceci est notamment utile lorsque d'importants éléments de configuration
sont disponibles dans un format non standard. L'exemple ci-dessous importe des paramètres de configuration
de base de données pour Drupal dans le conteneur de services.

.. code-block:: php

    // app/config/parameters.php

    include_once('/path/to/drupal/sites/default/settings.php');
    $container->setParameter('drupal.database.url', $db_url);

.. _`SetEnv`: http://httpd.apache.org/docs/current/env.html
