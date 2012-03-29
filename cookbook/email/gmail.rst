.. index::
   single: Emails; Gmail

Comment utiliser Gmail pour envoyer des Emails
==============================================

Durant le développement, au lieu d'utiliser un serveur SMTP ordinaire pour
envoyer des emails, vous pourriez trouver qu'utiliser Gmail soit plus
facile ou plus pratique. Le bundle Swiftmailer rend cela vraiment facile.

.. tip::

    Au lieu d'utiliser votre compte Gmail courant, il est bien sûr recommandé
    que vous créiez un compte spécial.

Dans le fichier de configuration de développement, changez le paramètre
``transport`` pour qu'il contienne la valeur ``gmail`` et définissez les
paramètres ``username`` et ``password`` avec les informations d'accès de
Google :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        swiftmailer:
            transport: gmail
            username:  your_gmail_username
            password:  your_gmail_password

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            transport="gmail"
            username="your_gmail_username"
            password="your_gmail_password" />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('swiftmailer', array(
            'transport' => "gmail",
            'username'  => "your_gmail_username",
            'password'  => "your_gmail_password",
        ));

Vous avez terminé !

.. note::

    Le transport ``gmail`` est simplement un raccourci qui utilise le transport
    ``smtp`` et définit ``encryption``, ``auth_mode`` et ``host`` afin que cela
    fonctionne avec Gmail.
