Créer le projet
===============

Installer Symfony
-----------------

Il n'y a qu'une seule voie recommandée pour installer Symfony :

.. best-practice::

    Utilisez toujours `Composer`_ pour installer Symfony.

Composer est le gestionnaire de dépendances utilisé par les applications PHP modernes.
Ajoutez ou supprimez des prérequis à votre projet et mettez à jour les bibliothèques
tierces utilisées par votre code est un vrai bonheur avec Composer.

Gestion des dépendances avec Composer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Avant d'installer Symfony, vous devez être sûr que vous avez Composer d'installé
globalement. Ouvrez votre terminal (aussi appelé *console de commandes*) et exécutez
la commande suivante :

.. code-block:: bash

    $ composer --version
    Composer version 1e27ff5e22df81e3cd0cd36e5fdd4a3c5a031f4a 2014-08-11 15:46:48

Vous verrez probablement un identifiant de version différent. Ce n'est pas grave 
car Composer est mis à jour de manière continuelle et sa version spécifique n'a
pas d'importance.


Installer Composer globalement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans le cas où vous n'auriez pas Composer d'installer globalement, exécutez les
deux commandes suivantes si vous utilisez Linux ou Max OS X (la seconde commande
vous demandera votre mot de passe utilisateur) :

.. code-block:: bash

    $ curl -sS https://getcomposer.org/installer | php
    $ sudo mv composer.phar /usr/local/bin/composer

.. note::

    En fonction de votre distribution Linux, vous devrez exécuter la commande ``su``
    au lieu de ``sudo``.

Si vous utilisez un système Windows, téléchargez l'installateur depuis la 
`page de téléchargement de Composer`_ et suivez les étapes pour l'installer.

Créer l'application de blog
---------------------------

Maintenant que tout est correctement paramétré, vous pouvez créer un nouveau
projet basé sur Symfony. Dans votre console, allez dans un répertoire où vous
avez le droit de créer des fichiers et exécutez les commandes suivantes :

.. code-block:: bash

    $ cd projects/
    $ composer create-project symfony/framework-standard-edition blog/

Cette commande créera un nouveau répertoire appelé ``blog`` qui contiendra
un nouveau projet basé sur la version stable la plus récente de Symfony disponible.

Vérifier l'installation de Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Une fois l'installation terminée, allez dans le répertoire ``blog/`` et vérifiez
que Symfony est correctement installé en exécutant la commande suivante :

.. code-block:: bash

    $ cd blog/
    $ php app/console --version

    Symfony version 2.6.* - app/dev/debug

Si vous voyez la version de Symfony installée, tout fonctionne comme attendu. Sinon,
vous pouvez exécuter le *script* suivant pour vérifier ce qui empêche votre système
d'exécuter correctement des applications Symfony :

.. code-block:: bash

    $ php app/check.php

En fonction de votre système, vous pouvez voir jusqu'à deux listes différentes 
lors de l'exécution du script `check.php`. La première montre les prérequis 
obligatoires que votre système doit avoir pour pouvoir exécuter des applications
Symfony. La seconde liste montre les prérequis facultatifs suggérés pour une 
exécution optimal des applications Symfony :

.. code-block:: bash

    Symfony2 Requirements Checker
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    > PHP is using the following php.ini file:
      /usr/local/zend/etc/php.ini

    > Checking Symfony requirements:
      .....E.........................W.....

    [ERROR]
    Your system is not ready to run Symfony2 projects

    Fix the following mandatory requirements
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

     * date.timezone setting must be set
       > Set the "date.timezone" setting in php.ini* (like Europe/Paris).

    Optional recommendations to improve your setup
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

     * short_open_tag should be disabled in php.ini
       > Set short_open_tag to off in php.ini*.


.. tip::

    Les distributions de Symfony sont signées numériquement pour des raisons de sécurité. 
    Si vous souhaitez vérifier l'intégrité de votre installation Symfony, regardez le
    `dépôt public des sommes de contrôle`_ et suivez `ces étapes`_ pour vérifier les signatures.

Structurer l'application
------------------------

Après avoir créé l'application, allez dans le répertoire ``blog/`` et vous verrez un
certain nombre de fichiers et répertoires générés automatiquement :

.. code-block:: text

    blog/
    ├─ app/
    │  ├─ console
    │  ├─ cache/
    │  ├─ config/
    │  ├─ logs/
    │  └─ Resources/
    ├─ src/
    │  └─ AppBundle/
    ├─ vendor/
    └─ web/

Cette architecture de fichers et de répertoires est une convention proposée par
Symfony pour la structure de vos application. L'usage recommandé pour chaque
répertoire est le suivant :

* ``app/cache/``, stocke tous les fichiers de cache générés par l'application;
* ``app/config/``, stocke toute la configuration définie pour chaque environnement;
* ``app/logs/``, stocke tous les fichiers de journaux (logs) générés par l'application;
* ``app/Resources/``, stocke tous les fichiers de templates et de traduction pour l'application;
* ``src/AppBundle/``, stocke tout le code Symfony spécifique (contrôleurs et routes),
  votre code métier (ex: classes Doctrine) et toute votre logique métier;
* ``vendor/``, c'est le répertoire où Composer installe les dépendances de votre application
  et vous ne devez jamais modifier son contenu;
* ``web/``, stocke tous les fichiers des contrôleurs frontaux et toutes les ressources web, telles que
  les feuilles de style, les fichiers JavaScript et les images.

Application Bundles
~~~~~~~~~~~~~~~~~~~

Quand Symfony 2.0 est sorti, beaucoup de développeurs ont naturellement adopté
la voie de symfony 1.x en divisant leurs applications en modules logiques. C'est
pourquoi beaucoup d'applications Symfony utilisent les bundles pour diviser leur 
code en fonctionnalités logiques : ``UserBundle``, ``ProductBundle``, ``InvoiceBundle``, 
etc.

Mais un bundle *entend* être quelque chose pouvant être réutilisé comme un élément
de logiciel à part. Si ``UserBundle`` ne peut pas être réutilisé *"en l'état"* dans
une autre application Symfony, alors il ne devrait pas être son propre bundle. Tout 
comme si ``InvoiceBundle`` dépend de ``ProductBundle``, alors il n'y a pas d'avantage
à avoir deux bundles spérarés.

.. best-practice::

    Créez seulement un bundle appelé ``AppBundle`` pour votre application métier

Implémenter un simple bundle ``AppBundle`` dans vos projet rendra votre code plus
concis et plus simple à comprendre. À partir de Symfony 2.6, la documentation 
officielle de Symfony utilisera le nom ``AppBundle``.

.. note::

    Il n'est pas nécessaire de préfixer le ``AppBundle`` avec votre propre nom d'organisation
     (ex: ``AcmeAppBundle``), car ce bundle applicatif n'a aucune vocation à être partagé.

Au final, ceci est la structure typique d'une application Symfony suivant ces bonnes 
pratiques :

.. code-block:: text

    blog/
    ├─ app/
    │  ├─ console
    │  ├─ cache/
    │  ├─ config/
    │  ├─ logs/
    │  └─ Resources/
    ├─ src/
    │  └─ AppBundle/
    ├─ vendor/
    └─ web/
       ├─ app.php
       └─ app_dev.php

.. tip::

    Si vous utilisez Symfony 2.6 ou une version plus récente, le bundle ``AppBundle``
    est déjà généré pour vous. Si vous utilisez une ancienne version de Symfony, vous
    pouvez le générer à la main en exécutant cette commande :

    .. code-block:: bash

        $ php app/console generate:bundle --namespace=AppBundle --dir=src --format=annotation --no-interaction

Étendre la structure des répertoires
------------------------------------

Si vos projets ou votre infrastructure requiert quelques changement dans les 
répertoires par défaut de la structure de Symfony, vous pouvez 
`surcharger l'emplacement des répertoires principaux`_ :
``cache/``, ``logs/`` and ``web/``.

En plus, Symfony3 utilisera une structure de répertoire légèrement différentes
lorsqu'il sortira :

.. code-block:: text

    blog-symfony3/
    ├─ app/
    │  ├─ config/
    │  └─ Resources/
    ├─ bin/
    │  └─ console
    ├─ src/
    ├─ var/
    │  ├─ cache/
    │  └─ logs/
    ├─ vendor/
    └─ web/

Les changements sont vraiment superficiels, mais pour le moment, nous vous 
recommandons d'utiliser la structure de répertoire de Symfony2.

.. _`Composer`: https://getcomposer.org/
.. _`Get Started`: https://getcomposer.org/doc/00-intro.md
.. _`page de téléchargement de Composer`: https://getcomposer.org/download/
.. _`surcharger l'emplacement des répertoires principaux`: http://symfony.com/doc/current/cookbook/configuration/override_dir_structure.html
.. _`dépôt public des sommes de contrôle`: https://github.com/sensiolabs/checksums
.. _`ces étapes`: http://fabien.potencier.org/article/73/signing-project-releases
