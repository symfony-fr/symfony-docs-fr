.. index::
   single: Installation

Installer et Configurer Symfony
===============================

Le but de ce chapitre est de vous permettre de démarrer avec une application
construite avec Symfony. Heureusement, Symfony propose un système de « distributions ».
Ce sont des projets Symfony fonctionnels « pour démarrer » que vous pouvez télécharger
et qui vous permettent de développer immédiatement.

.. tip::
    Si vous cherchez des instructions sur la meilleure façon de créer un nouveau
    projet et de le stocker dans un gestionnaire de versions, lisez `Utiliser un Gestionnaire de Versions`_.

Télécharger une Distribution Symfony2
-------------------------------------

.. tip::
    Premièrement, vérifiez que vous avez installé et configuré un serveur web
    (comme Apache) avec PHP. Pour plus d'informations sur les
    prérequis Symfony2, lisez le chapitre :doc:`prérequis</reference/requirements>`.

Les « distributions » Symfony2 sont des applications entièrement fonctionnelles
qui incluent les bibliothèques du coeur de Symfony2, une sélection de bundles utiles,
une arborescence pratique et une configuration par défaut. Quand vous téléchargez
une distribution Symfony2, vous téléchargez un squelette d'application qui peut
être immédiatement utilisé pour commencer à développer votre application.

Commencez par visiter la page de téléchargement de Symfony2 à `http://symfony.com/download`_.
Sur cette page, vous verrez la *Symfony Standard Edition*, qui est la principale
distribution Symfony2. Il y a deux façons de démarrer votre projet :

Option 1) « Composer »
~~~~~~~~~~~~~~~~~~~~~~

`Composer`_ est une bibliothèque de gestion de dépendances pour PHP, que vous
pouvez utiliser pour télécharger l'Édition Standard de Symfony2.

Commencez par `télécharger Composer`_ dans un quelconque répertoire sur votre
ordinateur. Si vous avez curl d'installé, cela est aussi facile que ce qui suit :

.. code-block:: bash

    curl -s https://getcomposer.org/installer | php

.. note::

    Si votre ordinateur n'est pas prêt à utiliser Composer, vous allez voir quelques
    recommandations lors de l'exécution de cette commande. Suivez ces recommandations
    afin que Composer puisse fonctionner correctement sur votre machine locale.

« Composer » est un fichier PHAR exécutable, que vous pouvez utiliser pour télécharger
la Distribution Standard :

.. code-block:: bash

    $ php composer.phar create-project symfony/framework-standard-edition /path/to/webroot/Symfony 2.3.0

.. tip::

    Pour une version exacte, remplacez `2.3.0` par la dernière version de
    Symfony. Pour plus de détails, lisez `Installation de Symfony`_

.. tip::

    Pour télécharger les « vendor » plus rapidement, ajoutez l'option
    "--prefer-dist" à la fin de la commande « Composer ».

Cette commande peut prendre plusieurs minutes pour s'exécuter, car « Composer »
télécharge la Distribution Standard ainsi que toutes les bibliothèques « vendor »
dont elle a besoin. Lorsque la commande a terminé son exécution, vous devriez
avoir un répertoire qui ressemble à quelque chose comme ça :

.. code-block:: text

    path/to/webroot/ <- votre répertoire racine
        Symfony/ <- le nouveau répertoire
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

Option 2) Télécharger une archive
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez aussi télécharger une archive contenant l'Édition Standard. Vous
devrez alors faire deux choix :

* Télécharger l'archive au format ``.tgz`` ou ``.zip``. Les deux sont équivalentes
  donc téléchargez celle avec laquelle vous vous sentez le plus à l'aise ;

* Téléchargez la distribution avec ou sans « vendors ». Si vous prévoyez d'utiliser
  d'autres bibliothèques tierces ou d'autres bundles et de les gérer via Composer,
  vous devriez probablement télécharger la distribution sans « vendors ».

Téléchargez l'une des archives quelque part dans le dossier racine de votre serveur
web et extrayez-la. Depuis une interface de commande UNIX, cela peut être fait
avec l'une des commandes suivantes (remplacez ``###`` par le nom du fichier) :

.. code-block:: bash

    # pour l'archive .tgz
    $ tar zxvf Symfony_Standard_Vendors_2.3.###.tgz

    # pour l'archive .zip
    $ unzip Symfony_Standard_Vendors_2.3.###.zip

Si vous avez téléchargé la distribution sans les « vendors », vous devez lire
la section suivante.

.. note::

    Vous pouvez facilement surcharger la structure de répertoires par défaut. Lisez
    :doc:`/cookbook/configuration/override_dir_structure` pour plus d'informations

Tous les fichiers publics et les contrôleurs frontaux qui prennent en charge les
requêtes entrantes d'une application Symfony2 se trouvent dans le répertoire
``Symfony/web/``. En conséquence, en supposant que vous avez extrait votre archive
à la racine de votre serveur web ou de votre virtual host, l'URL de votre
application commencera par ``http://localhost/Symfony/web/``. Pour avoir
des URLs plus courtes et plus agréables, vous devrez faire pointer la racine
de votre serveur web vers le répertoire ``Symfony/web/``. Bien que ce ne soit pas
nécessaire pour développer, c'est recommandé pour une application en production car
tous les fichiers système et liés à la configuration seront inaccessibles aux clients.
Pour plus d'informations sur la manière de configurer vos serveurs web, lisez les
documentations suivantes : `Apache`_ | `Nginx`_ .

.. note::

    Les exemples suivants supposent que vous n'avez pas touché la configuration
    de votre racine, donc toutes les urls commencent par ``http://localhost/Symfony/web/``

.. _installation-updating-vendors:

Mettre à jour les Vendors
~~~~~~~~~~~~~~~~~~~~~~~~~

A ce stade, vous avez téléchargé un projet Symfony entièrement fonctionnel
dans lequel vous allez commencer à développer votre propre application. Un projet
Symfony dépend d'un certain nombre de bibliothèques externes. Celles-ci sont
téléchargées dans le répertoire `vendor/` de votre projet via une bibliothèque
appelée `Composer`_.

Suivant la manière dont vous avez téléchargé Symfony, vous pourriez ou non avoir
besoin de mettre à jour vos « vendors » dès maintenant. Mais, mettre à jour vos
« vendors » est toujours sûr, et vous garantit d'avoir toutes les bibliothèques
dont vous avez besoin.

Étape 1: Téléchargez `Composer`_ (Le nouveau système de package PHP)

.. code-block:: bash

    curl -s http://getcomposer.org/installer | php

Assurez-vous d'avoir téléchargé ``composer.phar`` dans le même répertoire
que celui où se situe le fichier ``composer.json`` (par défaut à la racine
de votre projet Symfony).

Étape 2: Installer les « vendors »

.. code-block:: bash

    $ php composer.phar install

Cette commande télécharge toutes les bibliothèques nécessaires - incluant
Symfony elle-même - dans le répertoire ``vendor/``.

.. note::

    Si vous n'avez pas installé ``curl``, vous pouvez juste télécharger le fichier ``installer``
    manuellement à cette adresse http://getcomposer.org/installer. Placez ce fichier dans votre
    projet puis lancez les commandes :

    .. code-block:: bash

        php installer
        php composer.phar install

.. tip::

    Lorsque vous exécutez ``php composer.phar install`` ou ``php composer.phar update``,
    Composer va exécuter les commandes « install/update » pour vider le cache et
    installer les ressources (« assets » en anglais). Par défaut, les ressources
    seront copiées dans le répertoire ``web``.

    Au lieu de copier les ressources Symfony, vous pouvez créer des liens symboliques si
    votre système d'exploitation les supporte. Pour créer des liens symboliques, ajoutez
    une entrée dans le noeud ``extra`` de votre fichier composer.json avec la clé
    ``symfony-assets-install`` et la valeur ``symlink`` :

    .. code-block:: json

        "extra": {
            "symfony-app-dir": "app",
            "symfony-web-dir": "web",
            "symfony-assets-install": "symlink"
        }

    Si vous passez ``relative`` au lieu de ``symlink`` à la commande « symfony-assets-install »,
    cette dernière génèrera des liens symboliques relatifs.

Configuration et installation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant, toutes les bibliothèques tierces nécessaires sont dans le répertoire
``vendor/``. Vous avez également une application par défaut installée dans le
répertoire ``app/`` et un exemple de code dans le répertoire ``src/``.

Symfony2 est livré avec un testeur de configuration de votre serveur afin de
vérifier que votre serveur web et PHP sont bien configuré pour utiliser Symfony.
Utilisez l'URL suivante pour vérifier votre configuration :

.. code-block:: text

    http://localhost/config.php

S'il y a des problèmes, corrigez-les maintenant avant de poursuivre.

.. sidebar:: Définir les permissions

    Un des problèmes les plus fréquents est que les répertoires ``app/cache`` et
    ``app/logs`` ne sont pas accessibles en écriture par le serveur web et par
    l'utilisateur de ligne de commande. Sur un système UNIX, si votre utilisateur
    de ligne de commande est différent de celui du serveur web, vous pouvez lancer
    les commandes suivantes une fois dans votre projet pour vous assurer que les
    permissions sont correctement définies.

    **Notez que tous les serveurs web n'utilisent pas l'utilisateur** ``www-data``
    comme dans les exemples ci-dessous. Veuillez vérifier quel utilisateur *votre*
    serveur web utilise et utilisez-le à la place de ``www-data``.

    Sur un système UNIX, vous pouvez le faire grâce à une des commandes suivantes :

    .. code-block:: bash

        $ ps aux | grep httpd

    ou

    .. code-block:: bash

        $ ps aux | grep apache

    **1. Utiliser l'ACL sur un système qui supporte chmod +a**

    Beaucoup de systèmes autorisent l'usage de la commande ``chmod +a``.
    Essayez d'abord la première méthode, et si vous avez une erreur, essayez la seconde.
    Assurez-vous de bien remplacer ``www-data`` par l'utilisateur de votre serveur web
    dans la première commande ``chmod``:

    .. code-block:: bash

        $ rm -rf app/cache/*
        $ rm -rf app/logs/*

        $ sudo chmod +a "www-data allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs
        $ sudo chmod +a "`whoami` allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs

    **2. Utiliser l'ACL sur un système qui ne supporte pas chmod +a**

    Certains systèmes ne supportent pas la commande ``chmod +a``,
    mais supportent un autre utilitaire appelé ``setfacl``. Vous devrez sans doute
    `activer le support ACL`_ sur votre partition et installer setfacl avant de
    pouvoir l'utiliser (comme c'est le cas avec Ubuntu), de cette façon :

    .. code-block:: bash

        $ sudo setfacl -R -m u:www-data:rwX -m u:`whoami`:rwX app/cache app/logs
        $ sudo setfacl -dR -m u:www-data:rwx -m u:`whoami`:rwx app/cache app/logs

    **3. Sans utiliser l'ACL**

    Si vous n'avez pas les droits de changer les accès aux répertoires, vous aurez
    besoin de changer le umask pour que les répertoires cache et log soit accessibles
    en écriture au groupe ou aux autres (cela dépend si l'utilisateur serveur web
    et l'utilisateur de ligne de commande sont dans le même groupe ou non). Pour
    faire ceci, ajoutez la ligne suivante au début des fichiers ``app/console``,
    ``web/app.php`` et ``web/app_dev.php``::

        umask(0002); // Définit une permission 0775

        // ou

        umask(0000); // Définit une permission 0777

    Notez qu'utiliser l'ALC est recommandé si vous y avez accès sur votre serveur
    car changer le umask n'est pas sûr.

Lorsque tout est bon, cliquez sur « Go to the Welcome page » pour afficher votre
première « vraie » page Symfony2 :

.. code-block:: text

    http://localhost/app_dev.php/

Symfony2 devrait vous accueillir et vous féliciter pour tout le travail accompli
jusqu'ici !

.. image:: /images/quick_tour/welcome.png

Commencer à développer
----------------------

Maintenant que vous avez une application Symfony2 fonctionnelle, vous pouvez
commencer à développer ! Votre distribution devrait contenir un exemple de code.
Vérifiez le fichier ``README.md`` inclus avec la distribution (ouvrez-le en tant
que fichier texte) pour savoir quel exemple de code est inclus avec votre distribution
et savoir comment le supprimer par la suite.

Si vous découvrez Symfony, jetez un oeil au chapitre « :doc:`page_creation` », où
vous apprendrez comment créer des pages, changer la configuration  et faire tout
ce que vous aurez besoin de faire dans votre nouvelle application.

Assurez-vous aussi de consulter le :doc:`Cookbook</cookbook/index>`, qui contient
une grande variété d'articles expliquant comment solutionner des problèmes spécifiques
avec Symfony.

.. note::

    Si vous voulez supprimer le code d'exemple de votre distribution, jetez un oeil
    à cet article du Cookbook : ":doc:`/cookbook/bundles/remove`"

Utiliser un Gestionnaire de Versions
------------------------------------

Si vous utilisez un système de contrôle de version comme ``Git`` ou ``Subversion``,
vous pouvez le configurer et commencer à commiter votre projet normalement. La
Symfony Standard edition *est* le point de départ de votre nouveau projet.

Pour des instructions spécifiques sur la meilleure façon de gérer votre projet avec git,
lisez le chapitre :doc:`/cookbook/workflow/new_project_git`.

Ignorer le répertoire ``vendor/``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous avez téléchargé l'archive *sans vendors*, vous pouvez ignorer tout le
répertoire ``vendor/`` en toute sécurité et ne pas le commiter. Avec ``Git``,
cela se fait en créant le fichier ``.gitignore`` et en y ajoutant la ligne suivante:

.. code-block:: text

    /vendor/

Maintenant, le répertoire vendor ne sera pas commité sur votre système de gestion
de code. C'est plutôt bien (en fait c'est génial !) car lorsque quelqu'un clone ou
récupère le projet, il lui suffit de lancer la commande ``php composer.phar install``
pour récupérer toutes les bibliothèques nécessaires.

.. _`activer le support ACL`: https://help.ubuntu.com/community/FilePermissionsACLs
.. _`http://symfony.com/download`: http://symfony.com/download
.. _`Git`: http://git-scm.com/
.. _`GitHub`: http://help.github.com/set-up-git-redirect
.. _`télécharger Composer`: http://getcomposer.org/download/
.. _`Composer`: http://getcomposer.org/
.. _`Apache`: http://httpd.apache.org/docs/current/mod/core.html#documentroot
.. _`Nginx`: http://wiki.nginx.org/Symfony
.. _`Installation de Symfony`:    http://symfony.com/download
