Effectuer les tests Symfony2
============================

Avant de soumettre un :doc:`patch <patches>` pour inclusion, vous devrez
lancer la suite de tests intégrée dans Symfony2 afin de vérifier que tout est 
fonctionnel.

PHPUnit
-------

Afin de lancer la suite de tests Symfony2, `installez`_ PHPUnit 3.6.4 ou plus
récent :

.. code-block:: bash

    $ pear channel-discover pear.phpunit.de
    $ pear channel-discover components.ez.no
    $ pear channel-discover pear.symfony-project.com
    $ pear install phpunit/PHPUnit

Dépendences (facultatif)
------------------------

Pour lancer la suite de tests complète, incluant les tests dépendants de
bibliothèques externes, Symfony2 doit pouvoir auto-charger celles-ci. Par
défaut, elles sont auto-chargées depuis le répertoire `vendor/` dans le dossier
racine (vérifier `autoload.php.dist`).

La suite de tests a besoin des bibliothèques tierces suivantes :

* Doctrine
* Swiftmailer
* Twig
* Monolog

Pour les installer, utilisez `Composer`_:

Etape 1: Récupérer `Composer`_

.. code-block:: bash

    curl -s http://getcomposer.org/installer | php

Assurez vous de télécharger ``composer.phar`` dans le même répertoire que
celui où est situé le fichier ``composer.json``.

Etape 2: Installer les vendors

.. code-block:: bash

    $ php composer.phar --dev install

.. note::

    Notez que le script peut prendre un certain temps avant de se terminer.

.. note::

    Si vous n'avez pas ``curl`` d'installé, vous pouvez aussi simplement télécharger le
    fichier ``installer`` manuellement à http://getcomposer.org/installer. Déplacer ce fichier
    dans votre projet puis lancez les commandes :

    .. code-block:: bash

        $ php installer
        $ php composer.phar --dev install

Après installation, vous pouvez mettre à jour les vendors dans leur version la
plus récente en lançant la commande suivante :

.. code-block:: bash

    $ php composer.phar --dev update

Lancement
---------

Premièrement, mettez à jour les vendors (voir ci-dessus).

Ensuite, lancez la suite de tests depuis la racine de Symfony2 à l'aide de la
commande suivante :

.. code-block:: bash

    $ phpunit

La sortie devrait afficher `OK`. Si ce n'est pas le cas, vous devez étudier
les résultats afin de comprendre en quoi vos modifications ont altéré les 
résultats des tests.

.. tip::

    Lancez la suite de tests avant d'appliquer vos modifications afin de
    vérifier qu'ils fonctionnent sur votre configuration.

Couverture du code
------------------

Si vous ajoutez de nouvelles fonctionnalités, vous devez également vérifier la
couverture de code grâce à l'option `coverage-html` :

.. code-block:: bash

    $ phpunit --coverage-html=cov/

Vérifiez la couverture de code en ouvrant la page générée `cov/index.html` dans
un navigateur web.

.. tip::

    La couverture de code ne fonctionne que si vous avez activé XDebug et
    installé toutes les dépendances.

.. _installer: http://www.phpunit.de/manual/current/fr/installation.html
.. _`Composer`: http://getcomposer.org/