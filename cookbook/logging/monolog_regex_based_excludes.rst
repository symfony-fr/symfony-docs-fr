.. index::
   single: Logging
   single: Logging; Exclude 404 Errors
   single: Monolog; Exclude 404 Errors

Comment configurer Monolog pour exclure les erreurs 404 des logs
===========================================================

Quelquefois vos logs sont submergés d'erreurs HTTP 404 indésirables, 
par exemple, quand une personne malveillante teste certaines URLs bien 
connues sur votre application (`/phpmyadmin` par exemple). Lorsque vous 
utilisez le gestionnaire ``fingers_crossed``, vous pouvez exclure la mise 
en log de ces erreurs 404 via une expression régulière dans la config du 
MonologBundle

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        monolog:
            handlers:
                main:
                    # ...
                    type: fingers_crossed
                    handler: ...
                    excluded_404s:
                        - ^/phpmyadmin

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/monolog
                http://symfony.com/schema/dic/monolog/monolog-1.0.xsd"
        >
            <monolog:config>
                <monolog:handler type="fingers_crossed" name="main" handler="...">
                    <!-- ... -->
                    <monolog:excluded-404>^/phpmyadmin</monolog:excluded-404>
                </monolog:handler>
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('monolog', array(
            'handlers' => array(
                'main' => array(
                    // ...
                    'type'          => 'fingers_crossed',
                    'handler'       => ...,
                    'excluded_404s' => array(
                        '^/phpmyadmin',
                    ),
                ),
            ),
        ));