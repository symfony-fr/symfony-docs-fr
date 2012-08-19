.. index::
   single: PHP templates

Comment utiliser PHP plutôt que Twig dans les templates
=======================================================

Même si Symfony2 utilise Twig en tant que moteur de rendu par défaut, vous
pouvez utiliser du code PHP si vous le désirez. Ces deux moteurs de rendu
sont en effet supportés à part égal au sein de Symfony2. Symfony2 ajoute
même à PHP quelques possibilités utiles permettant d'écrire des modèles
encore plus puissants.

Rendu des templates PHP
-----------------------

Si vous voulez utiliser le moteur de rendu PHP, il vous faut d'abord vous
assurer d'activer celui-ci dans le fichier de configuration de votre
application:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating:    { engines: ['twig', 'php'] }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config ...>
            <!-- ... -->
            <framework:templating ...>
                <framework:engine id="twig" />
                <framework:engine id="php" />
            </framework:templating>
        </framework:config>

    .. code-block:: php

        $container->loadFromExtension('framework', array(
            ...,
            'templating'      => array(
                'engines' => array('twig', 'php'),
            ),
        ));

Vous pouvez maintenant utiliser le moteur de rendu en utilisant des templates PHP
plutôt que des templates Twig simplement en utilisant l'extension ``.php`` dans
le nom de vos templates à la place de l'extension ``.twig``.

Le contrôleur suivant délivre ainsi le template ``index.html.php`` ::

    // src/Acme/HelloBundle/Controller/HelloController.php

    // ...

    public function indexAction($name)
    {
        return $this->render('AcmeHelloBundle:Hello:index.html.php', array('name' => $name));
    }

.. index::
  single: Templating; Layout
  single: Layout

Templates de décorations
------------------------

Très souvent, les templates au sein d'un même projet partagent des composants communs
comme l'en-tête bien connu ou le pied de page. Chez Symfony, nous aimons penser à ce
problème différemment : un template peut être décoré par un autre.

Le template ``index.html.php`` est décoré par ``layout.html.php``, grâce à l'appel
de la méthode ``extend()`` :

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php $view->extend('AcmeHelloBundle::layout.html.php') ?>

    Hello <?php echo $name ?>!

La notation ``AcmeHelloBundle::layout.html.php`` vous parait peut être familière ;
c'est en effet la même notation qui est utilisée pour référencer un template à
l'intérieur d'un contrôleur. La partie ``::`` s'expliquant simplement par l'absence
d'un sous-dossier correspondant habituellement au contrôleur et qui sera donc
cherché directement à la racine du dossier ``views/``.

Maintenant, regardons d'un peu plus près le fichier ``layout.html.php`` :

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/layout.html.php -->
    <?php $view->extend('::base.html.php') ?>

    <h1>Hello Application</h1>

    <?php $view['slots']->output('_content') ?>

Le décorateur ou layout est lui-même décoré par un autre (``::base.html.php``).
Symfony2 supporte en effet de multiples niveaux de décoration : un décorateur
peut lui-même être décoré par un autre, et celà indéfinimment. Quand la partie
bundle du nom du template est vide, les vues sont recherchées dans le dossier
``app/Resources/views/``. Ce dossier contient donc les vues globales utilisées
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
est remplacée par le contenu du template fils, respectivement ``index.html.php`` et
``layout.html.php`` (voir la prochaine section sur les slots).

Comme vous pouvez le voir, Symfony2 fourni des méthodes sur l'objet ``$view``. Dans un
template, la variable ``$view`` est toujours disponible et réfère à un objet fournissant
un ensemble de méthodes rendant le moteur de rendu puissant.

.. index::
   single: Templating; Slot
   single: Slot

Travailler avec les slots
-------------------------

Un slot est un bout de code défini dans un template et réutilisable dans tous les
décorateurs de ce template. Ainsi dans le template ``index.html.php`` un slot
``title`` correspond à :

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php $view->extend('AcmeHelloBundle::layout.html.php') ?>

    <?php $view['slots']->set('title', 'Hello World Application') ?>

    Hello <?php echo $name ?>!

Le décorateur de base a déjà le code pour afficher le titre dans le header html :

.. code-block:: html+php

    <!-- app/Resources/views/base.html.php -->
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title><?php $view['slots']->output('title', 'Hello Application') ?></title>
    </head>

La méthode ``output()`` insert le contenu d'un slot et optionnellement prend une
valeur par défaut si le slot n'est pas défini. ``_content`` est quand à lui un
slot special qui contient le rendu du template enfant.

Pour les slots plus longs, il existe aussi une syntaxe étendue :

.. code-block:: html+php

    <?php $view['slots']->start('title') ?>
        Du code html sur de nombreuses lignes
    <?php $view['slots']->stop() ?>

.. index::
   single: Templating; Include

Inclure d'autres templates
--------------------------

La meilleure façon de partager une partie d'un template est de définir un template qui
pourra être inclus dans d'autres.

Créez un template ``hello.html.php`` :

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/hello.html.php -->
    Hello <?php echo $name ?>!

Et changez le template ``index.html.php`` pour qu'il comporte :

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php $view->extend('AcmeHelloBundle::layout.html.php') ?>

    <?php echo $view->render('AcmeHelloBundle:Hello:hello.html.php', array('name' => $name)) ?>

La méthode ``render()`` évalue et retourne le contenu d'un autre template (c'est
exactement la même méthode que celle utilisée dans le contrôleur).

.. index::
   single: Templating; Embedding pages

Intégrer d'autre contrôleurs
----------------------------

Intégrer le résultat d'un contrôleur dans un template peut être très utile afin de
factoriser certaines partie de l'application, en particulier lors de traitements
Ajax, ou quand les templates intégrés ont besoin de certaines variables non-incluses
dans le template principal.

Si vous créez une action nommée ``fancy``, et que vous voulez l'inclure dans le template
``index.html.php``, utilisez simplement le code suivant :

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php echo $view['actions']->render('AcmeHelloBundle:Hello:fancy', array('name' => $name, 'color' => 'green')) ?>

Ici, la chaîne de caractères ``AcmeHelloBundle:Hello:fancy`` fait référence à l'action
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

Mais où est défini le tableau d'éléments ``$view['actions']`` ? Comme ``$view['slots']``,
c'est un template « helper » et la section suivante vous en apprendra plus à son propos.

.. index::
   single: Templating; Helpers

Utiliser les templates « helpers »
----------------------------------

Le système de rendu par template utilisé par Symfony peut être étendu facilement
grace à des « helpers ». Les « helpers » sont des objets PHP qui fournissent des
possibilités utiles dans le contexte des templates. ``actions`` et ``slots``
sont ainsi deux des nombreux « helpers » intégrés dans Symfony2.

Créer des liens entre les pages
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A l'intérieur d'une application web, créez des liens entre les pages nécessite
d'utiliser des méthode propres à l'application si l'on souhaite conserver une
évolutivité et une maintenabilité sans failles. Ainsi l'utilisation d'un « helper »
``router`` à l'intérieur des template permet de générer des URLs basées sur la
configuration du routage. De cette façon, toutes les URLs peuvent facilement être
mises à jour directement en changeant simplement la configuration:

.. code-block:: html+php

    <a href="<?php echo $view['router']->generate('hello', array('name' => 'Thomas')) ?>">
        Greet Thomas!
    </a>

La méthode ``generate()``  prend comme arguments le nom de la route et un tableau
de paramètres. Le nom de la route est la clé principale sous laquelle celle-ci
est définie, les paramètres sont des valeurs remplaçant les paramètres inclus
dans celle-ci :

.. code-block:: yaml

    # src/Acme/HelloBundle/Resources/config/routing.yml
    hello: # The route name
        pattern:  /hello/{name}
        defaults: { _controller: AcmeHelloBundle:Hello:index }

Utiliser des « assets » : images, JavaScripts, et feuilles de style
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Que serait Internet sans images, sans JavaScript ou sans feuille de style ?
Symfony2 fourni le tag ``assets`` pour les utiliser facilement :

.. code-block:: html+php

    <link href="<?php echo $view['assets']->getUrl('css/blog.css') ?>" rel="stylesheet" type="text/css" />

    <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" />

Les « helpers » ``assets`` ont pour but principal de rendre votre application plus
portable. Grâce à ceux-ci, vous pouvez déplacer le répertoire principal de votre
application où vous le souhaitez à l'intérieur d'un dossier web sans changer
quoique ce soit dans le code de vos templates.

Echappement des variables de sortie (« Output Escaping » en anglais)
--------------------------------------------------------------------

Quand vous utilisez des templates, les variables peuvent être conservées tant qu'elles ne
sont pas affichées à l'utilisateur::

    <?php echo $view->escape($var) ?>

Par défaut, la méthode ``escape()`` assume que la variable est affichée dans un contexte
HTML. Le second argument vous permet de définir le contexte. Par exemple, pour afficher
cette variable dans un script JavaScript, il est possible d'utiliser le contexte ``js``::

    <?php echo $view->escape($var, 'js') ?>
