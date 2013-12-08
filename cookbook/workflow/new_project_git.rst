.. index::
   single: Workflow; Git

Comment créer et stocker un projet Symfony2 dans git
====================================================

.. tip::

    Bien que cet article soit spécifique à git, les mêmes principes génériques
    s'appliqueront si vous stockez votre projet dans Subversion.

Une fois que vous aurez lu :doc:`/book/page_creation` et que vous deviendrez
familier avec l'usage de Symfony, vous serez sans aucun doute prêt à démarrer
votre propre projet. Dans cet article du Cookbook, vous allez apprendre la
meilleure façon qui soit de démarrer un nouveau projet Symfony2 stocké dans
le système de gestion de contrôle de source `git`_.

Configuration Initiale du Projet
--------------------------------

Pour démarrer, vous aurez besoin de télécharger Symfony et d'initialiser
votre dépôt local git :

1. Télécharger `Symfony2 Standard Edition`_ sans les « vendors ».

2. Dézippez/détarez la distribution. Cela va créer un dossier nommé Symfony
   avec votre nouvelle structure de projet, les fichiers de configuration, etc.
   Renommez-le en ce que vous voulez.

3. Créez un nouveau fichier nommé ``.gitignore`` à la racine de votre nouveau
   projet (par exemple : à côté du fichier ``deps``) et coller ce qui suit dedans.
   Les fichiers correspondants à ces patterns seront ignorés par git :

   .. code-block:: text

        /web/bundles/
        /app/bootstrap*
        /app/cache/*
        /app/logs/*
        /vendor/  
        /app/config/parameters.yml

.. tip::
    Vous pouvez aussi avoir un fichier .gitignore qui peut être utilisé
    sur tout votre système, dans ce cas, vous pourrez trouver plus d'informations
    ici : `Github .gitignore`_.
    De cette manière, vous pouvez exclure les fichiers/dossiers souvent utilisés par
    votre IDE pour l'ensemble de vos projets.

4. Copiez ``app/config/parameters.yml`` vers ``app/config/parameters.yml.dist``.
   Le fichier ``parameters.yml`` est ignoré par git (voir ci-dessus) afin que les
   paramètres spécifiques à la machine comme les mots de passe de base de données
   ne soient pas committés. En créant le fichier ``parameters.yml.dist``, les
   nouveaux développeurs peuvent rapidement cloner le projet, copier ce fichier
   vers ``parameters.yml``, l'adapter, et commencer à développer.

5. Initialisez votre dépôt git :

   .. code-block:: bash

        $ git init

6. Ajoutez tous les fichiers initiaux dans git :

   .. code-block:: bash

        $ git add .

7. Créez un commit initial avec votre nouveau projet :

   .. code-block:: bash

        $ git commit -m "Initial commit"

8. Finalement, téléchargez toutes les bibliothèques tierces en utilisant composer.
vous pourrez trouver plus d'informations sur :ref:`installation-updating-vendors`.

A ce point, vous disposez d'un projet Symfony2 totalement fonctionnel qui est
correctement committé sous git. Vous pouvez immédiatement commencer à
développer, en committant les nouveaux changements dans votre dépôt git.

Vous pouvez continuer en lisant le chapitre :doc:`/book/page_creation` pour en
apprendre davantage sur comment configurer et développer votre application en interne.

.. tip::

    L'Edition Standard Symfony2 est fournie avec des exemples d'utilisation. Pour
    supprimer le code de démonstration, suivez les instructions contenues dans 
    l'article `Readme de la Standard Edition`_.

.. _cookbook-managing-vendor-libraries:

.. include:: _vendor_deps.rst.inc

Vendors et Submodules
~~~~~~~~~~~~~~~~~~~~~~

Au lieu d'utiliser le système ``deps`` et ``bin/vendors`` pour gérer les
bibliothèques vendor, vous pourriez choisir à la place le système natif
`git submodules`_. Il n'y a rien d'incorrect dans cette approche, bien que le
système ``deps`` soit la manière officielle de résoudre ce problème et qu'il peut
être parfois difficile de travailler avec les git submodules.

Stocker votre projet sur un serveur distant
-------------------------------------------

Vous disposez maintenant d'un projet totalement fonctionnel stocké dans git.
Cependant, dans la plupart des cas, vous voudrez aussi stocker votre projet
sur un serveur distant que ce soit pour des raisons de sauvegardes mais aussi
afin que d'autres développeurs puissent collaborer au projet.

La façon la plus facile de stocker votre projet sur un serveur distant est via
`GitHub`_. Les dépôts publics sont gratuits, cependant vous devrez payer un
abonnement mensuel pour héberger des dépôts privés.

Une autre alternative est de stocker votre dépôt git sur n'importe quel serveur
en créant un `dépôt barebones`_ et en « pushant » ce dernier. Une bibliothèque
qui aide à gérer ceci est `Gitolite`_.

.. _`git`: http://git-scm.com/
.. _`Symfony2 Standard Edition`: http://symfony.com/download
.. _`Readme de la Standard Edition`: https://github.com/symfony/symfony-standard/blob/master/README.md
.. _`git submodules`: http://git-scm.com/book/en/Git-Tools-Submodules
.. _`GitHub`: https://github.com/
.. _`dépôt barebones`: http://git-scm.com/book/en/Git-Basics-Getting-a-Git-Repository
.. _`Gitolite`: https://github.com/sitaramc/gitolite
.. _`Github .gitignore`: https://help.github.com/articles/ignoring-files
