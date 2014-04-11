.. index::
   single: Emails; Spooling

Comment utiliser le « Spool » d'Email
=====================================

Quand vous utilisez le ``SwiftmailerBundle`` pour envoyer un email depuis une
application Symfony2, il va par défaut l'envoyer immédiatement. Vous pourriez,
cependant, vouloir éviter d'avoir un hit de performance entre le ``Swiftmailer``
et le transport de l'email, qui pourrait avoir pour conséquence que l'utilisateur
ait à attendre que la page suivante se charge pendant que l'email est envoyé. Cela
peut être évité en choisissant d'envoyer les emails en mode « spool » au lieu
de les envoyer directement. Cela signifie que ``Swiftmailer`` n'essaie pas d'envoyer
l'email mais à la place sauvegarde le message quelque part comme par exemple
dans un fichier. Un autre processus peut alors lire depuis le « spool » et
prendre en charge l'envoi des emails contenus dans ce dernier. Pour le moment,
seul le « spooling » via un fichier est supporté par ``Swiftmailer``.

Afin d'utiliser le « spool », utilisez la configuration suivante :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer:
            # ...
            spool:
                type: file
                path: /path/to/spool

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!--
            xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
            http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config>
             <swiftmailer:spool
                 type="file"
                 path="/path/to/spool" />
        </swiftmailer:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('swiftmailer', array(
            ...,
            'spool' => array(
                'type' => 'file',
                'path' => '/path/to/spool',
            )
        ));

.. tip::

    Si vous voulez stocker le « spool » quelque part dans votre répertoire
    de projet, rappelez-vous que vous pouvez utiliser le paramètre
    `%kernel.root_dir%` pour référencer la racine du projet :

    .. code-block:: yaml

        path: "%kernel.root_dir%/spool"

Maintenant, quand votre application envoie un email, il ne sera en fait pas
envoyé mais ajouté au « spool » à la place. L'envoi des messages depuis le
« spool » est effectué séparément. Il y a une commande de la console pour
envoyer les messages qui sont dans le « spool » :

.. code-block:: bash

    $ php app/console swiftmailer:spool:send --env=prod

Cette commande possède une option pour limiter le nombre de messages
devant être envoyés :

.. code-block:: bash

    $ php app/console swiftmailer:spool:send --message-limit=10 --env=prod

Vous pouvez aussi définir la limite de temps en secondes :

.. code-block:: bash

    $ php app/console swiftmailer:spool:send --time-limit=10 --env=prod

Bien sûr, vous n'avez pas besoin de lancer cette tâche manuellement.
A la place, la commande de la console devrait être lancée par une tâche cron
ou une tâche planifiée et exécutée à intervalle régulier.
