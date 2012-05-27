Participer à la Documentation
=============================

La documentation est aussi importante que le code. Elle suit exactement les 
mêmes principes:
DRY, testée, facile à maintenir, extensible, optimisée, et factorisée dans un
soucis de concision.

Contribuer
----------

Avant de contribuer, vous devez devenir familier avec le language utilisé
:doc:`markup rest<format>` dans la documentation.

La documentation Symfony2 est hébergé sur GitHub:

.. code-block:: text

    https://github.com/symfony/symfony-docs

Si vous voulez soumetter un correctif, `forker`_ le dépot offciel sur GitHub et
cloner votre dépot:

.. code-block:: bash

    $ git clone git://github.com/YOURUSERNAME/symfony-docs.git

A moins que vous ne documentiez une fonctionnalité nouvelle, vos changements
doivent être basé sur la branche 2.0 plutôt que sur la branche master. Pour
effectuer ceci ``checkouter`` la branche 2.0:

.. code-block:: bash

    $ git checkout 2.0


Ensuite, créez une branche dédiée pour vos changement:

.. code-block:: bash

    $ git checkout -b improving_foo_and_bar

Vous pouvez maintenant appliquer vos changements directement à cette branche et 
les transmettre (commit). Ensuite, envoyer cette branche à *votre* dépot et
inititier une requête (pull request). Celle-ci devra être entre votre branche
``improving_foo_and_bar`` et la branche symfony-docs ``master``.

.. image:: /images/docs-pull-request.png
   :align: center

Si vous avez basé vos changement sur la branche 2.0 vous devez suivre le lien
 de commit et changer la branche de base vers @2.0 :

.. image:: /images/docs-pull-request-change-base.png
   :align: center

GitHub traite en détail les requêtes de mise à jour ou `pull requests`_ .

.. note::

    La documentation Symfony2 est sous licence Creative Commons
    Attribution-Share Alike 3.0 Unported :doc:`Licence <license>`.

.. tip::
		
		Vos changements apparaissent sur le site symfony.com moins de 15 minutes
		après que l'équipe de documentation a mergé votre pull request. Vous pouvez
		vérifier si vos changements ont introduit des erreurs de syntaxe en allant
		sur la page `Erreurs de génération de la documentation`_ (elle est mise à
		jour chaque nuit à 3h du matin quand le serveur génère la documentation).


Signaler une erreur
-------------------

La contribution la plus facile que vous pouvez effectuer est de signaler une 
erreur: une typo, une grammaire imparfaite, un example de code erroné, une 
explication manquante...

Etapes:

* Soumettez un bogue dans le gestionnaire de bogues;

* *(optionel)* Proposer un correctif.

Traductions
-----------

Lisez la documentation dédiée :doc:`traductions <translations>`.

.. _`forker`: http://help.github.com/fork-a-repo/
.. _`pull requests`: http://help.github.com/pull-requests/
.. _`Erreurs de génération de la documentation`: http://symfony.com/doc/build_errors