.. index::
   single: Deployment

Déployer une application Symfony2
=================================

.. note::

    Le déploiement peut être une tâche complexe et variable en fonction de
    votre configuration et de vos besoins. Cet article n'essaie pas de répondre
    à tout, mais plutôt d'aborder les besoins les plus récurrents et d'apporter
    quelques idées lors du déploiement.

Bases du déploiement Symfony2
-----------------------------

Les étapes typiques à suivre lors du déploiement d'une application Symfony2
sont :

#. Uploader votre code à jour sur le serveur de production;
#. Mettre à jour vos dépendances Vendor (en général, c'est fait via
   Composer et cela peut être fait avant l'upload);
#. Lancer les migrations de base de données ou toute tâche similaire qui
   apporterait des changements de structure à votre base;
#. Vider (et peut être plus important encore, faire un "warm up") le cache.

Un déploiement peut aussi inclure d'autres étapes comme :

* Tagger une version particulière de votre code dans votre système de gestion de code;
* Créer un espace temporaire pour mettre certaines choses à jour hors ligne;
* Lancer les tests pour garantir la stabilité du code et/ou du serveur;
* Supprimer tout fichier inutile du répertoire ``web`` pour conserver votre
  environnement de production propre;
* Vider les systèmes de cache externes (comme `Memcached`_ ou `Redis`_).

Comment déployer une application Symfony2
-----------------------------------------

Il y a plusieurs manières de déployer une application Symfony2.

Commençons par les bases pour entrer dans les détails ensuite.

Transfert de fichier de base
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La manière la plus basique de déployer une application est de copier les
fichiers manuellement via ftp/scp (ou une méthode similaire). Cela a quelques
inconvénients puisque vous ne contrôlez pas tout, notamment le processus de mise
à jour. Cette méthode implique également de réaliser d'autres tâches manuellement
après le transfert de fichiers (voir `Tâches communes après le déploiement`_).

Utiliser un système de gestion de version 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous utilisez un système de gestion de version (ex git ou svn), vous
pouvez vous simplifier la vie en faisant en sorte que votre application en
production soit une copie de votre dépôt. Ainsi, lorsque vous êtes prêt à
mettre à jour votre code, il suffit juste de récupérer les dernières modifications
de votre dépôt.

Cela rend les mises à jour de vos fichiers *plus facile*, mais vous devrez
tout de même vous occuper manuellement d'autres étapes
(voir `Tâches communes après le déploiement`_).

Utiliser des scripts et d'autres outils
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il existe des outils de qualité pour faciliter le déploiement. Il y a même
certains outils qui ont spécialement été taillés pour les besoins de Symfony2,
et d'autres qui s'assurent que tout se passe bien avant, pendant et après le
déploiement.

Lisez `Les outils`_ pour une liste des outils qui peuvent vous aider à déployer.

Tâches communes après le déploiement
------------------------------------

Après avoir déployé votre code source, il y a un certain nombre de choses à
faire :

A) Configurer votre fichier ``app/config/parameters.ini``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ce fichier devrait être personnalisé sur chaque système. La méthode
que vous utilisez pour déployer votre code source de doit *pas* déployer
ce fichier. Vous devriez plutôt le définir manuellement (ou via un processus)
sur votre serveur.

B) Mettre à jour les vendors
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vos vendors peuvent être mis à jour avant de transférer votre code source
(mettez à jour votre répertoire ``vendor/`` puis transférez le avec le reste
de votre code source) ou après sur le serveur. Peu importe ce que vous choisissez,
mettez à jour vos vendors comme vous le faites d'habitude :

.. code-block:: bash

    $ php composer.phar install --optimize-autoloader

.. tip::

    L'option ``--optimize-autoloader`` rend l'autoloader de Composer plus performant
    en construisant une « map ».

C) Videz votre cache Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Assurez vous de vider (et faire un warm up) de votre cache :

.. code-block:: bash

    $ php app/console cache:clear --env=prod --no-debug

D) Dumpez vos ressources Assetic
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous utilisez Assetic, vous devrez aussi dumpez vos ressources :

.. code-block:: bash

    $ php app/console assetic:dump --env=prod --no-debug

E) Et bien d'autres !
~~~~~~~~~~~~~~~~~~~~~

Il y a encore bien d'autres choses que vous devrez peut être faire selon
votre configuration :

* Lancer vos migrations de base de données
* Vider votre cache APC
* Lancer ``assets:install`` (déjà dans ``composer.phar install``)
* Ajouter/éditer des tâches CRON
* Mettre vos ressources sur un CDN
* ...

Cycle de vie de l'application : intégration continue, qualité, ...
------------------------------------------------------------------

Alors que cet article couvre les aspects techniques du déploiement, le cycle
de vie complet du code depuis le développement jusqu'au serveur de production
peut contenir bien d'autres étapes (déploiement en préproduction, qualité, lancement
des tests, ...).

L'utilisation de la préproduction, des tests, de l 'assurance qualité, de l'intégration
continue, des migrations de base de données et la capacité de retour arrière en cas d'échec
sont fortement recommandés. Il existes des outils simples ou plus complexes qui vous
permettent de simplifier le déploiement.

N'oubliez pas que déployer votre application implique également de mettre à jour
vos dépendances (généralement avec Composer), mettre à jour votre base de données,
vider votre cache et de réaliser potentiellement d'autres chose comme mettre vos
ressources sur un CDN (voir `Tâches communes après le déploiement`_).


Les outils
----------

`Capifony`_:

    Cet outil fournit un ensemble d'outils spécialisés basés sur Capistrano et
    taillés spécifiquement pour les projets symfony et Symfony2.

`sf2debpkg`_:

    Cet outil aide à construire un paquet natif Debian pour vos projets Symfony2.

`Magallanes`_:

    Cet outil de déploiement semblable à Capistrano est construit en PHP et
    est peut être plus facile à étendre pour les développeurs PHP qui ont des
    besoins spécifiques.

Bundles:

    Il existe plusieurs 
    `bundles qui proposent des fonctionnalités liés au déploiement`_ directement
    dans votre console Symfony2.

Scripts de base:

    Vous pouvez bien sur utiliser le shell, `Ant`_, ou n'importe quel autre
    outil de script pour déployer vos projets.

Platform as a Service Providers:

    Paas est une manière relativement nouvelle de déployer votre application.
    Typiquement, une Paas utilisera un unique fichier de configuration à la
    racine de votre projet pour déterminer comment construire un environnement
    à la volée qui supporte votre logiciel.
    `PagodaBox`_ possède un excellent support de Symfony2.

.. tip::

    Vous voulez en savoir plus ? Discutez avec la communauté sur le `canal IRC Symfony`_
    #symfony (sur freenode) pour plus d'informations.

.. _`Capifony`: http://capifony.org/
.. _`sf2debpkg`: https://github.com/liip/sf2debpkg
.. _`Ant`: http://blog.sznapka.pl/deploying-symfony2-applications-with-ant
.. _`PagodaBox`: https://github.com/jmather/pagoda-symfony-sonata-distribution/blob/master/Boxfile
.. _`Magallanes`: https://github.com/andres-montanez/Magallanes
.. _`bundles qui proposent des fonctionnalités liés au déploiement`: http://knpbundles.com/search?q=deploy
.. _`canal IRC Symfony`: http://webchat.freenode.net/?channels=symfony
.. _`Memcached`: http://memcached.org/
.. _`Redis`: http://redis.io/