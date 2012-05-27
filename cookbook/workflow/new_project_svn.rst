Comment créer et stocker un projet Symfony2 dans Subversion
===========================================================

.. tip::

    Cette article est spécifique à Subversion et basé sur des principes que
    vous trouverez dans l'article :doc:`/cookbook/workflow/new_project_git`.

Une fois que vous avez lu :doc:`/book/page_creation` et que vous êtes devenu
familier avec l'usage de Symfony, vous serez sans aucun doute prêt à démarrer
votre propre projet. La méthode préférée pour gérer des projets Symfony2 est
d'utiliser `git`_ mais certains préfèrent utiliser `Subversion`_, ce qui ne pose
aucun problème! Dans cet article du Cookbook, vous allez apprendre comment gérer
votre projet en utilisant `svn`_ de la même manière que si vous l'aviez fait avec
`git`_.

.. tip::

    Ceci est **une** méthode parmi tant d'autres de stocker votre projet
    Symfony2 dans un dépôt Subversion. Il y a plusieurs manières de faire
    cela et celle-ci en est simplement une qui fonctionne.

Le Dépôt Subversion
-------------------

Pour cet article, nous supposerons que votre schéma de dépôt suit la structure
standard et répandue :

.. code-block:: text

    myproject/
        branches/
        tags/
        trunk/

.. tip::

    La plupart des hébergements subversion devraient suivre cette pratique standard.
    C'est le schéma recommandé par `Contrôle de version avec Subversion`_ et utilisé
    par la plupart des hébergements gratuits (voir :ref:`svn-hosting`).

Configuration Initiale du Projet
--------------------------------

Pour démarrer, vous aurez besoin de télécharger Symfony2 et d'effectuer la
configuration basique de Subversion :

1. Téléchargez `Symfony2 Standard Edition`_ avec ou sans les « vendors ».

2. Dézippez/détarez la distribution. Cela va créer un dossier nommé Symfony
   avec votre nouvelle structure de projet, les fichiers de configuration, etc.
   Renommez-le en ce que vous voulez.

3. Effectuez un « checkout » du dépôt Subversion qui va héberger ce projet. Disons
   qu'il est hébergé sur `Google code`_ et nommé ``myproject`` :

  .. code-block:: bash
    
        $ svn checkout http://myproject.googlecode.com/svn/trunk myproject

4. Copiez les fichiers du projet Symfony2 dans le dossier subversion :

  .. code-block:: bash

        $ mv Symfony/* myproject/

5. Ecrivons maintenant les patterns pour les fichiers à ignorer. Tout ne doit pas
   être stocké dans votre dépôt subversion. Quelques fichiers (comme le cache) sont
   générés et d'autres (comme la configuration de la base de données) sont destinés
   à être adaptés sur chaque machine. Ainsi, nous utilisons la propriété
   ``svn:ignore`` afin de pouvoir ignorer ces fichiers spécifiques.

  .. code-block:: bash

        $ cd myproject/
        $ svn add --depth=empty app app/cache app/logs app/config web

        $ svn propset svn:ignore "vendor" .
        $ svn propset svn:ignore "bootstrap*" app/
        $ svn propset svn:ignore "parameters.yml" app/config/
        $ svn propset svn:ignore "*" app/cache/
        $ svn propset svn:ignore "*" app/logs/

        $ svn propset svn:ignore "bundles" web

        $ svn ci -m "commit basic symfony ignore list (vendor, app/bootstrap*, app/config/parameters.yml, app/cache/*, app/logs/*, web/bundles)"

6. Le reste des fichiers peut maintenant être ajouté et committé dans le projet :

  .. code-block:: bash

        $ svn add --force .
        $ svn ci -m "add basic Symfony Standard 2.X.Y"

7. Copiez ``app/config/parameters.yml`` vers ``app/config/parameters.yml.dist``.
   Le fichier ``parameters.yml`` est ignoré par svn (voir ci-dessus) afin que les
   paramètres spécifiques à la machine comme les mots de passe de base de données ne
   soient pas committés. En créant le fichier ``parameters.yml.dist``, les
   nouveaux développeurs peuvent rapidement cloner le projet, copier ce fichier
   vers ``parameters.yml``, l'adapter, et commencer à développer.

8. Finalement, téléchargez toutes les bibliothèques tierces :

  .. code-block:: bash
    
        $ php bin/vendors install

.. tip::

    `git`_ doit être installé pour pouvoir exécuter la commande ``bin/vendors`` ;
    c'est le protocole utilisé pour aller récupérer les bibliothèques vendor.
    Cela signifie seulement que ``git`` est utilisé comme outil pour aider au
    téléchargement des bibliothèques dans le répertoire ``vendor/``.

A ce point, vous avez un projet Symfony2 entièrement fonctionnel stocké dans votre
dépôt Subversion. Le développement peut démarrer avec des commits dans ce dernier.

Vous pouvez continuer en lisant le chapitre :doc:`/book/page_creation` pour en
apprendre plus sur comment configurer et développer votre application en interne.

.. tip::

    L'Edition Standard Symfony2 vient avec des exemples de fonctionnalités. Pour
    supprimer le code de démonstration, suivez les instructions du fichier
    `Readme de la Standard Edition`_.

.. include:: _vendor_deps.rst.inc

.. _svn-hosting:

Solutions d'hébergement Subversion
----------------------------------

La plus grosse différence entre `git`_ et `svn`_ est que Subversion *a besoin*
d'un dépôt central pour fonctionner. Vous avez donc plusieurs solutions :

- Hébergement par vos soins : créez votre propre dépôt et accédez-y soit grâce au
  système de fichiers, soit via le réseau. Pour vous aider dans cette tâche, vous
  pouvez lire `Contrôle de version avec Subversion`_.

- Hébergement via une entité tierce : il y a beaucoup de solutions d'hébergement
  gratuites et sérieuses disponibles comme `GitHub`_, `Google code`_, `SourceForge`_
  ou `Gna`_. Certaines d'entre elles offrent aussi des possibilités d'hébergement git.

.. _`git`: http://git-scm.com/
.. _`svn`: http://subversion.apache.org/
.. _`Subversion`: http://subversion.apache.org/
.. _`Symfony2 Standard Edition`: http://symfony.com/download
.. _`Readme de la Standard Edition`: https://github.com/symfony/symfony-standard/blob/master/README.md
.. _`Contrôle de version avec Subversion`: http://svnbook.red-bean.com/
.. _`GitHub`: http://github.com/
.. _`Google code`: http://code.google.com/hosting/
.. _`SourceForge`: http://sourceforge.net/
.. _`Gna`: http://gna.org/
