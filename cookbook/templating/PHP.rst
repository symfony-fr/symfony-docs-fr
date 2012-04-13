.. index::
   single: PHP Templates

Comment utiliser PHP plutôt que TWIG dans les modèles
=====================================================

Même si Symfony2 utilise Twig en tant que moteur de rendu par défaut, vous
pouvez utiliser du code PHP si vous le désirez. Ces deux moteurs de rendu
sont en effet supportés à part égal au sein de Symfony2. Symfony2 ajoute
même à PHP quelques possibilités agréables permettant d'écrire des modèles
encore plus puissants.

NdT: Un modèle se traduit en anglais par template.

Rendu des modèles PHP
---------------------

Si vous voulez utiliser le moteur de rendu PHP, il vous faut d'abord vous
assurer d'activer celui-ci dans votre le fichier de configuration de votre
application:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating:    { engines: ['twig', 'php'] }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config ... >
            <!-- ... -->
            <framework:templating ... >
                <framework:engine id="twig" />
                <framework:engine id="php" />
            </framework:templating>
        </framework:config>

    .. code-block:: php

        $container->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'engines' => array('twig', 'php'),
            ),
        ));

Vous pouvez maintenant utiliser le moteur de rendu utilisant des modèles PHP
plutôt que des modèles Twig simplement en utilisant l'extension ``.php`` dans
le nom de vos modèles à la place ide l'extension ``.twig``.

Le contrôleur suivant restitue ainsi le modèle ``index.html.php`` ::

    // src/Acme/HelloBundle/Controller/HelloController.php

    public function indexAction($name)
    {
        return $this->render('AcmeHelloBundle:Hello:index.html.php', array('name' => $name));
    }

.. index::
  single: Templating; Layout
  single: Layout

Modèles de décorations
----------------------

Très souvent les modèles au sein d'un même projets partagent des composants communs
comme le bien connu entête (header) ou le pied de page (footer). Chez Symfony, nous
aimons penser à ce problème différemment: un modèle peut être décoré par un autre.

Le modèle ``index.html.php`` est décoré par ``layout.html.php``, grace à l'appel
 ``extend()`` :

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php $view->extend('AcmeHelloBundle::layout.html.php') ?>

    Hello <?php echo $name ?>!

La notation ``AcmeHelloBundle::layout.html.php`` vous parait peut être familière,
c'est en effet la même notation qui est utilisée pour référencer un modèle à
l'intérieur d'un controleur. La partie ``::`` s'expliquant simplement par l'absence
d'un sous-dossier correspondant habituellement au contrôleur et qui sera donc
cherché directement à la racine du dossier ``views/``.

Maintenant regardons d'un peu plus prêt le fichier ``layout.html.php`` :

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/layout.html.php -->
    <?php $view->extend('::base.html.php') ?>

    <h1>Hello Application</h1>

    <?php $view['slots']->output('_content') ?>

Le décorateur ou layout est lui-même décoré par un autre (``::base.html.php``).
Symfony2 supporte en effet de multiples niveau de décoration: un décorateur
peut lui-même être décoré par un autre, et celà indéfinimment. Quand la partie
bundle du nom du modèle est vide, les vues sont recherchées dans le dossier
``app/Resources/views/``. Ce dossier enregistre donc les vues globales utilisées
dans tout le projet.

.. code-block:: html+php

    <!-- app/Resources/views/base.html.php -->
    <!DOCTYPE html>
    <html>
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <title><?php $view['slots']->output('title', 'Hello Application') ?></title>
        </head>
        <body>
            <?php $view['slots']->output('_content') ?>
        </body>
    </html>

Pour les deux décorateurs, l'expression ``$view['slots']->output('_content')``
est remplacé par le contenu du modèle fils, respectivement ``index.html.php`` et
``layout.html.php`` (voir la section prochaine sur les slots).

Comme vous pouvez le voir, Symfony2 fourni des méthodes sur l'objet ``$view``. Dans un
modèle, la variable ``$view`` est toujours disponible et réfère à un objet fournissant
une suite de méthodes rendant le moteur de rendu puissant.

.. index::
   single: Templating; Slot
   single: Slot

Travailler avec les slots
-------------------------

Un slot est un bout de code défini dans un modèle et réutilisable dans tous les
décorateurs de ce modèle. Ainsi dans le modèle ``index.html.php`` un slot
 ``title`` correspond à :

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php $view->extend('AcmeHelloBundle::layout.html.php') ?>

    <?php $view['slots']->set('title', 'Hello World Application') ?>

    Hello <?php echo $name ?>!

Le décorateur de base a déjà le code pour afficher le titre dans le header html:

.. code-block:: html+php

    <!-- app/Resources/views/base.html.php -->
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title><?php $view['slots']->output('title', 'Hello Application') ?></title>
    </head>

La méthode ``output()`` insert le contenu d'un slot and optionnellement prends une
valeur par défaut si le slot n'est pas défini. ``_content`` est quand à lui un
slot special qui contient le rendu du modèle enfant.

Pour les slots plus prolixes, il existe aussi une syntaxe étendue:

.. code-block:: html+php

    <?php $view['slots']->start('title') ?>
        Du code html sur de nombreuses lignes
    <?php $view['slots']->stop() ?>

.. index::
   single: Templating; Include

Inclure d'autres modèles
------------------------

La meilleure voie pour partager une part d'un modèle est de définir un modèle qui
pourra être inclus dans d'autres.

Créer un modèle ``hello.html.php`` :

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/hello.html.php -->
    Hello <?php echo $name ?>!

Et changer le modèle ``index.html.php`` pour qu'il comporte :

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php $view->extend('AcmeHelloBundle::layout.html.php') ?>

    <?php echo $view->render('AcmeHelloBundle:Hello:hello.html.php', array('name' => $name)) ?>

La méthode ``render()`` évalue et retourne le contenu d'un autre modèle (c'est
exactement la même méthode que celle utilisée dans le contrôleur).

.. index::
   single: Templating; Embedding Pages

Intégrer d'autre contrôleurs
----------------------------

Intégrer le résultat d'un contrôleur dans un modèle peut être très utile afin de
factoriser certaines partie de l'application, en particulier lors de traitements
Ajax, ou quand les modèles intégrés ont besoin de certaines variables non-incluses
dans le modèle principal.

Si vous créer une action nommé ``fancy``, et voulez l'inclure dans le modèle
``index.html.php``, utiliser simplement le code suivant:

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php echo $view['actions']->render('AcmeHelloBundle:Hello:fancy', array('name' => $name, 'color' => 'green')) ?>

Ici, la chaîne de caractère ``AcmeHelloBundle:Hello:fancy`` fait référence à l'action
``fancy`` du contrôleur ``Hello`` ::

    // src/Acme/HelloBundle/Controller/HelloController.php

    class HelloController extends Controller
    {
        public function fancyAction($name, $color)
        {
            // create some object, based on the $color variable
            $object = ...;

            return $this->render('AcmeHelloBundle:Hello:fancy.html.php', array('name' => $name, 'object' => $object));
        }

        // ...
    }

Mais où est défini le tableau d'éléments ``$view['actions']``? Comme ``$view['slots']``,
c'est un assitant modèle et la section suivantes vous en apprendra plus à son propos.

.. index::
   single: Templating; Helpers

Utiliser les assitants du modèle (ou template helper)
-----------------------------------------------------

Le système de rendu par modèle utiliser par Symfony peut être étendu facilement
grace à des assitants. Les assitants sont des objets PHP qui fournissent des
possibilités utiles dans le contextes des modèles. ``actions`` et ``slots``
sont ainsi deux des nombreux assitants intégrés à Symfony2.

Créer des liens entre les pages
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A l'intérieur d'une application web créer des liens entre les pages nécessite
d'utiliser des méthode propres à l'application si l'on souhaite conserver une
évolutivité et une maintenabilité sans faille. Ainsi l'utilisation d'un assitant
``router`` à l'intérieur des modèle permets de générer des URLs basées sur la
configuration du routage. De cette façon toutes les urls peuvent facilement être
mise à jour directement en changeant simplement la configuration:

.. code-block:: html+php

    <a href="<?php echo $view['router']->generate('hello', array('name' => 'Thomas')) ?>">
        Greet Thomas!
    </a>

La méthode ``generate()``  prends comme arguments le nom de la route et un tableau
de paramètres. Le nom de la route est la clef principal sous laquelle celle-ci
est défini, les paramètres sont des valeurs remplaçant les paramètres incluent
dans celle-ci:

.. code-block:: yaml

    # src/Acme/HelloBundle/Resources/config/routing.yml
    hello: # The route name
        pattern:  /hello/{name}
        defaults: { _controller: AcmeHelloBundle:Hello:index }

Utiliser des atouts (assets): images, JavaScripts, et feuilles de styles
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Que serait internet sans image, sans javascript ou sans feuille de style?
Symfony2 fourni le tag ``assets`` pour les utiliser facilement:

.. code-block:: html+php

    <link href="<?php echo $view['assets']->getUrl('css/blog.css') ?>" rel="stylesheet" type="text/css" />

    <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" />

Les assitants ``assets`` ont pour but principaux de rendre votre application plus
portable. Grâce à ceux-ci, vous pouvez déplacer le répertoire principal de votre
application où vous le souhaiter à l'intérieur d'un dossier web sans changer
quoique ce soit dans le code de vos modèles.

Sécurisation des sorties (échappement des variables)
----------------------------------------------------

Quand vous utiliser les modèles, les variables peuvent être conservée tant qu'elles ne
sont pas afficher à l'utilisateur::

    <?php echo $view->escape($var) ?>

Par défaut, la méthode ``escape()`` assumes que la variable est affichée dans un context
HTML. Le second argument vous permet de définir le contexte. Par exemple, pour afficher
cette variable dans un script JavaScript, il est possible d'utiliser le contexte ``js``::

    <?php echo $view->escape($var, 'js') ?>
