Effectuer les tests Symfony2
============================

Avant de soumettre un :doc:`patch <patches>` pour inclusion, vous avez besoin de
lancer la suite de test intégrée dans Symfony2 afin de vérifier que tout est 
fonctionnel.

PHPUnit
-------

Afin de lancer la suite de test Symfony2, `installer`_ PHPUnit 3.6.4 ou plus
récent:

.. code-block:: bash

    $ pear channel-discover pear.phpunit.de
    $ pear channel-discover components.ez.no
    $ pear channel-discover pear.symfony-project.com
    $ pear install phpunit/PHPUnit

Dépendences optionnelles
------------------------

Pour lancer la suite de tests entière, incluant les tests dépendants de
bibliothèques externes, Symfony2 doit pouvoir auto-charger celles-ci. Par
défaut, elles sont auto-chargés depuis le répertoire `vendor/` dans le dossier
racine (vérifier `autoload.php.dist` et `autoload.php`).

La suite de test à besoin des bibliothèques tierces suivantes:

* Doctrine
* Swiftmailer
* Twig
* Monolog

Pour les installer, lancer le script `vendors` :

.. code-block:: bash

    $ php vendors.php install

.. note::

    Noter que les tests prennent un certain temps pour s'effectuer.

Après l'installation, vous pouvez mettre à jour les fournisseurs externes
(vendors) avec la commande suivante:

.. code-block:: bash

    $ php vendors.php update

Lancement
---------

Premièrement, mettez à jour les fournisseurs (voir précédemment).

Ensuite, lancer la suite de tests depuis la racine de Symfony2 à l'aide de la
commande suivante:

.. code-block:: bash

    $ phpunit

La sortie devrait indiquée `OK`. Si ce n'était pas les cas, vous devez étudier
les résultats afin de comprendre en quoi vos modifications ont altéré les 
résultats des tests.

.. tip::

    Lancer la suite de tests avant d'appliquer vos modifications afin de
    vérifier qu'ils fonctionnent sur votre configuration.

Couverture du code
------------------

Si vous ajouter de nouvelles fonctionnalités, vous devez également vérifier que
le code ajouté est couvert en utilisant:

.. code-block:: bash

    $ phpunit --coverage-html=cov/

Vérifier la couverture de code en ouvrant la page générée `cov/index.html` dans
un navigateur web.

.. tip::

    La couverture de code ne fonctionne que si vous avez activé XDebug et
    installé toutes les dépendances.

.. _installer: http://www.phpunit.de/manual/current/en/installation.html