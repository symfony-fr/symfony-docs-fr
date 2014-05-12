.. index::
   single: Sessions

Combler une application legacy avec les sessions de Symfony
===========================================================

.. versionadded:: 2.3
    Cette intégration avec la session PHP legacy a été introduite en Symfony 2.3.

Si vous intégrez le framework Symfony full-stack dans une application legacy
qui démarre la session avec ``session_start()``, vous pouvez encore être capable
d'utiliser la gestion de session de Symfony en utilisant le PHP Bridge session.

Si l'application a fixé son propre handler PHP d'enregistrement vous pouvez
spécifier à null le paramètre ``handler_id`` :

.. configuration-block::

    .. code-block:: yaml

        framework:
            session:
                storage_id: session.storage.php_bridge
                handler_id: ~

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:framework="http://symfony.com/schema/dic/symfony">

            <framework:config>
                <framework:session storage-id="session.storage.php_bridge"
                    handler-id="null"
                />
            </framework:config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('framework', array(
            'session' => array(
                'storage_id' => 'session.storage.php_bridge',
                'handler_id' => null,
        ));

Autrement, si le problème est simplement que vous ne pouvez pas éviter
que l'application ne démarre la session avec ``session_start()``, vous
pouvez encore utiliser un handler de sauvegarde basé sur
Symfony en spécifiant le handler de sauvergarde comme dans l'exemple
qui suit :

.. configuration-block::

    .. code-block:: yaml

        framework:
            session:
                storage_id: session.storage.php_bridge
                handler_id: session.handler.native_file

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:framework="http://symfony.com/schema/dic/symfony">

            <framework:config>
                <framework:session storage-id="session.storage.php_bridge"
                    handler-id="session.storage.native_file"
                />
            </framework:config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('framework', array(
            'session' => array(
                'storage_id' => 'session.storage.php_bridge',
                'handler_id' => 'session.storage.native_file',
        ));

.. note::

    Si l'application legacy requiert son propose handler de sauvegarde de
    session, ne le surchargez pas. A la place, configurez ``handler_id: ~``.
    Notez qu'un handler de sauvegarde ne peut pas être changé une fois la
    session démarrée. Si l'application démarre la session avant que Symfony
    ne soit initialisé, le handler de sauvegarde sera déja fixé. Dans ce cas,
    vous aurez besoin de `handler_id: ~``.
    Surchargez le handler de sauvegarde uniquement si vous êtes sûr que l'application
    legacy peut utiliser le handler de sauvegarde de Symfony sans effets de
    bord et que la session n'ait pas été démarrée avant que Symfony n'ait été
    initialisé.

Pour plus de détails, consultez :doc:`/components/http_foundation/session_php_bridge`.
