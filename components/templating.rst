.. index::
   single: Templating
   single: Components; Templating

Le Composant Templating
=======================

    Le Composant Templating fournit tous les outils nécessaires pour construire
    n'importe quel système de templates.

    Il fournit une infrastructure qui charge des fichiers de template et qui,
    optionnellement, surveille s'ils changent. Il apporte aussi une implémentation
    concrète d'un moteur de template utilisant PHP avec des outils additionnels
    pour séparer les templates en blocs et couches.

Installation
------------

Vous pouvez installer le composant de différentes manières :

* Utilisez le dépôt Git officiel (https://github.com/symfony/Templating) ;
* Installez le via Composer (``symfony/templating`` dans `Packagist`_).

Utilisation
-----------

La classe :class:`Symfony\\Component\\Templating\\PhpEngine` est le point
d'entrée du composant. Elle a besoin d'un analyseur de nom de template
(:class:`Symfony\\Component\\Templating\\TemplateNameParserInterface`) pour
convertir un nom de template en une référence de template et d'un chargeur
de template (:class:`Symfony\\Component\\Templating\\Loader\\LoaderInterface`)
pour trouver le template associé à une référence::

    use Symfony\Component\Templating\PhpEngine;
    use Symfony\Component\Templating\TemplateNameParser;
    use Symfony\Component\Templating\Loader\FilesystemLoader;

    $loader = new FilesystemLoader(__DIR__ . '/views/%name%');

    $view = new PhpEngine(new TemplateNameParser(), $loader);

    echo $view->render('hello.php', array('firstname' => 'Fabien'));

La méthode :method:`Symfony\\Component\\Templating\\PhpEngine::render` exécute
le fichier `views/hello.php` et retourne le texte de sortie.

.. code-block:: html+php

    <!-- views/hello.php -->
    Hello, <?php echo $firstname ?>!

Héritage de template avec les slots
-----------------------------------

L'héritage de template est structuré de manière à partager les couches avec
de nombreux templates.

.. code-block:: html+php

    <!-- views/layout.php -->
    <html>
        <head>
            <title><?php $view['slots']->output('title', 'Default title') ?></title>
        </head>
        <body>
            <?php $view['slots']->output('_content') ?>
        </body>
    </html>

La méthode :method:`Symfony\\Component\\Templating\\PhpEngine::extend` est appelée dans
le sous-template pour définir son template parent.

.. code-block:: html+php

    <!-- views/page.php -->
    <?php $view->extend('layout.php') ?>

    <?php $view['slots']->set('title', $page->title) ?>

    <h1>
        <?php echo $page->title ?>
    </h1>
    <p>
        <?php echo $page->body ?>
    </p>

Pour utiliser l'héritage de template, la classe
:class:`Symfony\\Component\\Templating\\Helper\\SlotsHelper` doit être
déclarée::

    use Symfony\Component\Templating\Helper\SlotsHelper;

    $view->set(new SlotsHelper());

    // récupère l'objet $page

    echo $view->render('page.php', array('page' => $page));

.. note::

    Avoir de multiples niveaux d'héritage est possible : une couche peut étendre
    une autre couche.

Échappement en sortie
---------------------

Cette partie de la documentation est toujours en cours d'écriture.

La classe d'Aide « Asset »
--------------------------

Cette partie de la documentation est toujours en cours d'écriture.

.. _Packagist: https://packagist.org/packages/symfony/templating