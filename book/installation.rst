.. index::
   single: Installation

Installer et Configurer Symfony
===============================

Le but de ce chapitre est de vous permettre de démarrer avec une application
construite avec Symfony. Heureusement, Symfony propose un système de « distributions ».
Ce sont des projets Symfony fonctionnels « de départ » que vous pouvez télécharger
et qui vous permettent de développer immédiatement.

.. tip::
    Si vous cherchez des instructions sur la meilleure façon de créer un nouveau
    projet et de le stocker dans un gestionnaire de versions, lisez `Utiliser un Gestionnaire de Versions`_.

Télécharger une Distribution Symfony2 
-------------------------------------

.. tip::
    Premièrement, vérifiez que vous avez installé et configuré un serveur web
    (comme Apache) avec PHP 5.3.4 ou supérieur. Pour plus d'informations sur les
    prérequis Symfony2, lisez le chapitre :doc:`pré-requis</reference/requirements>`.
    Pour plus d'informations sur la manière de configurer la racine web de votre serveur,
    lisez les documentations suivantes : `Apache`_ | `Nginx`_ .

Les « distributions » Symfony2 sont des applications entièrement fonctionnelles
qui incluent les librairies du coeur de Symfony2, une sélection de bundles utiles,
une arborescence pratique et une configuration par défaut. Quand vous téléchargez
une distribution Symfony2, vous téléchargez un squelette d'application qui peut
être immédiatement utilisé pour commencer à développer votre application.

Commencez par visiter la page de téléchargement de Symfony2 à `http://symfony.com/download`_.
Sur cette page, vous verrez la *Symfony Standard Edition*, qui est la principale
distribution Symfony2. Vous devrez alors faire deux choix :

* Télécharger l'archive au format ``.tgz`` ou ``.zip``. Les deux sont équivalentes
  donc téléchargez celle avec laquelle vous vous sentez le plus à l'aise.

* Téléchargez la distribution avec ou sans vendors. Si vous avez installé `Git`_
  sur votre ordinateur, vous devriez télécharger Symfony2 « sans vendors ». Cela
  vous donnera plus de flexibilité quand vous incluerez des librairies tierces.

Téléchargez l'une des archives quelque part dans le dossier racine de votre serveur
web et extrayez là. Depuis une interface de commande UNIX, cela peut être fait
avec l'une des commandes suivantes (remplacez ``###`` par le nom du fichier) :

.. code-block:: bash

    # pour l'archive .tgz
    tar zxvf Symfony_Standard_Vendors_2.0.###.tgz

    # pour l'archive .zip
    unzip Symfony_Standard_Vendors_2.0.###.zip

Lorsque vous aurez fini, vous devriez avoir un répertoire ``Symfony/`` qui
ressemble à ceci :

.. code-block:: text

    www/ <- votre dossier racine
        Symfony/ <- l'archive extraite
            app/
                cache/
                config/
                logs/
            src/
                ...
            vendor/
                ...
            web/
                app.php
                ...

Mettre à jour les Vendors
~~~~~~~~~~~~~~~~~~~~~~~~~

Step 1: Téléchargez `Composer`_ (Le nouveau système de package PHP)

.. code-block:: bash

    curl -s http://getcomposer.org/installer | php

Assurez vous d'avoir téléchargé ``composer.phar`` dans le même répertoire
que celui où se situe le fichier ``composer.json`` (par défaut à la racine
de votre projet Symfony).

Step 2: Install vendors

.. code-block:: bash

    php composer.phar install

Cette commande télécharge toutes les librairies nécessaires - incluant
Symfony elle-même - dans le répertoire ``vendor/``.

.. note::

    Si vous n'avez pas installé ``curl``, vous pouvez juste télécharger le fichier ``installer``
    manuellement à cette adresse http://getcomposer.org/installer. Placez ce fichier dans votre
    projet puis lancez les commandes :

    .. code-block:: bash

        php installer
        php composer.phar install

.. tip::

  Lorsque vous exécutez ``php composer.phar install`` ou ``php composer.phar update``,le Composer
  exécutera les commandes install/update pour vider le cache et installer les ressources (assets).
  Par défaut, les ressources seront copiées dans le répertoire ``web``. Pour créer des liens
  symboliques plutôt que de copier les ressources, vous pouvez ajouter une entrée dans le noeud
  ``extra`` du fichier composer.json avec la clé `symfony-assets-install`` et la valeur ``symlink`` :

  .. code-block:: json
 
      "extra": {
          "symfony-app-dir": "app", 
          "symfony-web-dir": "web", 
          "symfony-assets-install": "symlink" 
      }
  
  Si vous définissez ``relative`` au lieu de ``symlink`` pour symfony-assets-install, la commande
  génèrera des liens symboliques relatifs.

Configuration et installation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant, toutes les librairies tierces nécessaires sont dans le répertoire
``vendor/``. Vous avez également une application par défaut installée dans le
répertoire ``app/`` et un exemple de code dans le répertoire ``src/``.

Symfony2 est livré avec un testeur de configuration de votre serveur afin de
vérifier que votre serveur web et PHP sont bien configurés pour utiliser Symfony.
Utilisez l'URL suivante pour vérifier votre configuration :

.. code-block:: text

    http://localhost/config.php

S'il y a des problèmes, corrigez les maintenant avant de poursuivre.

.. sidebar:: Définir les permissions

    Un des problèmes les plus fréquents et que les répertoires ``app/cache`` et
    ``app/logs`` ne sont pas accessibles en écriture par le serveur web et par
    l'utilisateur de ligne de commande. Sur un système UNIX, si votre utilisateur
    de ligne de commande est différent de celui du serveur web, vous pouvez lancer
    les commandes suivantes une fois dans votre projet pour vous assurer que les
    permissions sont correctement définies. Changez l'utilisateur du serveur web
    pour ``www-data`` et celui de la ligne de commande pour ``yourname`` :

    **1. Utiliser l'ACL sur un système qui supporte chmod +a**

    Beaucoup de systèmes autorisent l'usage de la commande ``chmod +a``.
    Essayez d'abord cela, et si vous avez une erreur, essayez la méthode suivante.

    .. code-block:: bash

        rm -rf app/cache/*
        rm -rf app/logs/*

        sudo chmod +a "www-data allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs
        sudo chmod +a "yourname allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs

    **2. Utiliser l'ACL sur un système qui ne supporte pas chmod +a**

    Certains systèmes ne supportent pas la commande``chmod +a``, 
    mais supportent un autre utilitaire appelé ``setfacl``. Vous devrez sans doute
    `activer le support ACL`_ sur votre partition et installer setfacl avant de
    pouvoir l'utiliser (comme c'est le cas avec Ubuntu), de cette façon :

    .. code-block:: bash

        sudo setfacl -R -m u:www-data:rwx -m u:`whoami`:rwx app/cache app/logs
        sudo setfacl -dR -m u:www-data:rwx -m u:`whoami`:rwx app/cache app/logs

    Notez que tout les serveurs web n'utilisent pas l'utilisateur ``www-data``. Vous devez
    vérifier quel utilisateur votre serveur web utilise et le remplacer dans la commande
    ci-dessus si besoin. Cela peut être fait en vérifiant la liste des processus pour voir
    quel utilisateur éxécute votre serveur web.

    **3. Sans utiliser l'ACL**

    Si vous n'avez pas les droits de changer les accès aux répertoires, vous aurez
    besoin de changer le umask pour que les répertoires cache et log soit accessibles
    en écriture au groupe ou aux autres (cela dépend si l'utilisateur serveur web
    et l'utilisateur de ligne de commande sont dans le même groupe ou non). Pour
    faire ceci, ajoutez la ligne suivante au début des fichiers ``app/console``,
    ``web/app.php`` et ``web/app_dev.php`` :

    .. code-block:: php

        umask(0002); // Définit une permission 0775

        // ou

        umask(0000); // Définit une permission 0777

    Notez que utiliser l'ALC est recommandé si vous y avez accès sur votre serveur
    car changer le umask n'est pas sûr.

Lorsque tout est bon, cliquez sur « Go to the Welcome page » pour afficher votre
première « vraie » page Symfony2 :

.. code-block:: text

    http://localhost/app_dev.php/

Symfony2 devrait vous accueillir et vous féliciter pour tout le travail accompli
jusqu'ici !

.. image:: /images/quick_tour/welcome.jpg

Commencer à développer
----------------------

Maintenant que vous avez une application Symfony2 fonctionnelle, vous pouvez
commencer à développer ! Votre distribution devrait contenir un exemple de code.
Vérifiez le fichier ``README.rst`` inclu avec la distribution (ouvrez le en tant
que fichier texte) pour savoir quel exemple de code est inclu avec votre distribution
et savoir comment le supprimer par la suite.

Si vous découvrez Symfony, jetez un oeil au chapitre « :doc:`page_creation` », où
vous apprendrez comment créer des pages, changer la configuration  et faire tout
ce que vous aurez besoin de faire dans votre nouvelle application.

Utiliser un Gestionnaire de Versions
------------------------------------

Si vous utilisez un système de contrôle de version comme ``Git`` ou ``Subversion``,
vous pouvez le configurer et commencer à commiter votre projet normalement. La
Symfony Standard edition *est* le point de départ de votre nouveau projet.

Pour des instructions spécifiques sur la meilleur façon de gérer votre projet avec git, 
lisez le chapitre :doc:`/cookbook/workflow/new_project_git`.

Ignorer le répertoire ``vendor/`` 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous avez téléchargé l'archive *sans vendors*, vous pouvez ignorer tout le 
répertoire ``vendor/`` en toute sécurité et ne pas le commiter. Avec ``Git``,
cela se fait en créant le fichier ``.gitignore`` et en y ajoutant la ligne suivante:

.. code-block:: text

    vendor/

Maintenant, le répertoire vendor ne sera pas commité sur votre système de gestion
de code. C'est plutôt bien (en fait c'est génial !) car lorsque quelqu'un clone ou
récupère le projet, il lui suffit de lancer la commande ``php bin/vendors install``
pour récupérer toutes les librairies nécessaires.

.. _`activer le support ACL`: https://help.ubuntu.com/community/FilePermissionsACLs
.. _`http://symfony.com/download`: http://symfony.com/download
.. _`Git`: http://git-scm.com/
.. _`GitHub`: http://help.github.com/set-up-git-redirect
.. _`Composer`: http://getcomposer.org/
.. _`Apache`: http://httpd.apache.org/docs/current/mod/core.html#documentroot
.. _`Nginx`: http://wiki.nginx.org/Symfony
