Participer à la Documentation
=============================

La documentation est aussi importante que le code. Elle suit exactement les 
mêmes principes :
DRY, testée, facile à maintenir, extensible, optimisée, et factorisée dans un
soucis de concision.

Contribuer
----------

Avant de contribuer, vous devez maîtriser le language :doc:`markup rest<format>`
utilisé dans la documentation.

La documentation Symfony2 est hébergée sur GitHub:

.. code-block:: text

    https://github.com/symfony/symfony-docs

Si vous voulez soumettre un correctif, `forkez`_ le dépôt officiel sur GitHub et
clonez votre dépot :

.. code-block:: bash

    $ git clone git://github.com/YOURUSERNAME/symfony-docs.git

Conformément au code source de Symfony, le dépôt de la documentation est séparé en trois
branches : ``2.0`` pour la dernière version de Symfony 2.0.x, ``2.1`` pour la
dernière version de SYmfony 2.1.x et ``master`` qui est la branche de développement
des versions à venir.

A moins que vous ne documentiez une fonctionnalité nouvelle, vos changements
doivent toujours être basés sur la branche 2.0 plutôt que sur la branche master.
Pour effectuer ceci faites un ``checkout`` la branche 2.0 :

.. code-block:: bash

    $ git checkout 2.0


Ensuite, créez une branche dédiée pour vos changements :

.. code-block:: bash

    $ git checkout -b improving_foo_and_bar

Vous pouvez maintenant appliquer vos changements directement à cette branche et 
les commiter. Ensuite, envoyez cette branche à *votre* dépôt et
initiez une requête (pull request). Celle-ci devra être entre votre branche
``improving_foo_and_bar`` et la branche symfony-docs ``master``.

.. image:: /images/docs-pull-request.png
   :align: center

Si vous avez basé vos changement sur la branche 2.0 vous devez changer la branche
de base pour qu'elle soit 2.0 sur la page de prévisualisation :

.. image:: /images/docs-pull-request-change-base.png
   :align: center

.. note::
  
  Tout les changements faits sur la branche 2.0 seront mergés chaque semaine dans la
  branche 2.1 qui sera ensuite mergée sur la branche master pour la prochaine version.

GitHub explique en détail les requêtes de mise à jour ou `pull requests`_ .

.. note::

    La documentation Symfony2 est sous licence Creative Commons
    Attribution-Share Alike 3.0 Unported :doc:`Licence <license>`.

.. tip::

    Soyez patient, vos changements peuvent mettre de 15 minutes à plusieurs jours
    après que l'équipe de documentation a mergé votre pull request avant d'être visibles
    sur le site symfony.com. Vous pouvez vérifier si vos changements ont introduit des
    erreurs de syntaxe en allant sur la page `Erreurs de génération de la documentation`_
    (elle est mise à jour chaque nuit à 3h du matin quand le serveur génère la documentation).

Standards
---------

Dans le but d'aider le lecteur autant que possible, et de créer des exemples
de code qui semblent familiers, vous devriez suivre ces règles :

* Le code suit les :doc:`Standards de code Symfony</contributing/code/standards>`
  ainsi que les `Standards de code Twig`_;
* Chaque ligne devrait se terminer après le premier mot qui dépasse le 72ème caractère
  (ainsi, la plupart des lignes font environ 72 à 78 caractères);
* Lorsqu'on n'affiche pas une ou plusieurs lignes de code, on place ``...`` en commentaire à
  l'endroit ou les lignes sont censées se trouver. Ces commentaires sont : ``// ...`` (php),
  ``# ...`` (yaml/bash), ``{# ... #}`` (twig), ``<!-- ... -->`` (xml/html), ``; ...`` (ini),
  ``...`` (text);
* Lorsqu'on cache une partie d'une ligne, par exemple une variable, on place ``...`` (sans commentaire)
  à l'endroit ou elle est censée être;
* Description du code caché : (facultatif)
  Si on cache plusieurs lignes : la description peut être placée après les ``...``
  Si on ne cache qu'une partie de la ligne : la description peut être placée avant la ligne;
* Si c'est utile, un ``bloc de code`` devrait commencer par un commentaire indiquant le nom du
  fichier qui contient le code. Ne mettez pas de ligne vide après ce commentaire, à moins que
  la prochaine ligne ne soit également un commentaire;
* Vous devriez mettre un ``$`` devant chaque ligne de commande;
* Nous préférerons le raccourci ``::`` à ``.. code-block:: php`` pour commencer un block de PHP.

Un exemple::

    // src/Foo/Bar.php

    // ...
    class Bar
    {
        // ...

        public function foo($bar)
        {
            // définit foo avec la valeur de bar
            $foo = ...;

            // ... vérifie si $bar a la bonne valeur

            return $foo->baz($bar, ...);
        }
    }

.. note::

    * En Yaml vous devriez mettre un espace après ``{`` et avant ``}`` (ex ``{ _controller: ... }``),
      mais pas en Twig (ex ``{'hello' : 'value'}``).
    * Un élément de tableau est une partie de ligne, pas une ligne complète. Vous ne devriez donc pas
      utiliser ``// ...`` mais ``...,`` (la virgule est là pour les standards de code)::

        array(
            'une valeur',
            ...,
        )


Signaler une erreur
-------------------

La contribution la plus facile que vous pouvez effectuer est de signaler une 
erreur : une faute de frappe, une grammaire imparfaite, un exemple de code erroné, une 
explication manquante ...

Étapes:

* Soumettez un bogue dans le gestionnaire de bogues;

* *(optionnel)* Proposez un correctif.

Traductions
-----------

Lisez la documentation dédiée :doc:`traductions <translations>`.

.. _`forkez`: https://help.github.com/articles/fork-a-repo
.. _`pull requests`: https://help.github.com/articles/using-pull-requests
.. _`Erreurs de génération de la documentation`: http://symfony.com/doc/build_errors
.. _`Standards de code Twig`: http://twig.sensiolabs.org/doc/coding_standards.html