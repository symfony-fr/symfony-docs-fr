.. index::
   single: Sessions, dossier de sessions

Configurer le Dossier où les Fichiers pour les Sessions sont Enregistrés
========================================================================

Par défaut, la Symfony Standard Edition utilise les valeurs par défaut de
``php.ini`` pour ``session.save_handler`` et ``session.save_path`` pour
déterminer l'endroit où persister les données de session. Cela est possible
grâce à la configuration suivante :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                # handler_id fixé à null utilisera le session handler de php.ini
                handler_id: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd"
        >
            <framework:config>
                <!-- handler_id fixé à null utilisera le session handler de php.ini -->
                <framework:session handler-id="null" />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array(
                // handler_id fixé à null utilisera le session handler de php.ini
                'handler_id' => null,
            ),
        ));

Avec cette configuration, c'est à vous de changer le fichier `php.ini`` pour changer
*l'endroit où* les metadata de votre session seront stockées.

Néanmoins, si vous disposez de la configuration suivante, Symfony stockera les
données de la session dans des fichiers contenus dans le dossier de cache
``%kernel.cache_dir%/sessions``. Cela signifie que lorsque vous purgez le cache,
toutes les sessions courantes seront également effacées :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd"
        >
            <framework:config>
                <framework:session />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array(),
        ));

Utiliser un dossier différent pour enregistrer les données de session
est l'une des méthodes permettant d'assurer que les sessions courantes
ne seront pas perdues lorsque le vous nettoierez le cache Symfony.

.. tip::

    Une excellente méthode (la plus complexe) de gestion de session avec Symfony
    est d'utiliser un handler d'enregistrement de session différent. Consultez
    :doc:`/components/http_foundation/session_configuration` pour aller plus
    loin avec les handlers de sauvegarde de session. Il existe également un
    cookbook pour le stockage de sessions en
    :doc:`base de données </cookbook/configuration/pdo_session_storage>`.

Pour modifier le dossier dans lequel Symfony enregistre les données de session,
vous avez uniquement besoin de changer la configuration du framework. Dans
cet exemple, vous allez changer le dossier de session pour ``app/sessions`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                handler_id: session.handler.native_file
                save_path: "%kernel.root_dir%/sessions"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd"
        >
            <framework:config>
                <framework:session handler-id="session.handler.native_file"
                    save-path="%kernel.root_dir%/sessions"
                />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array(
                'handler_id' => 'session.handler.native_file',
                'save_path'  => '%kernel.root_dir%/sessions',
            ),
        ));
