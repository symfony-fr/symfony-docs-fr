.. _symfony2-versus-flat-php:

Symfony versus PHP pur
======================

**Pourquoi est-ce que Symfony est mieux que simplement ouvrir un fichier 
et écrire du PHP pur ?**

Si vous n'avez jamais utilisé un framework PHP, que vous n'êtes pas familier avec 
la philosophie MVC, ou simplement que vous vous demandez pourquoi il y a tant
d'*agitation* autour de Symfony, ce chapitre est pour vous. Au lieu de vous
*dire* que Symfony vous permet de développer plus rapidement de meilleures
applications qu'en PHP pur, vous allez le voir de vos propres yeux.

Dans ce chapitre, vous allez écrire une application simple en PHP, puis refactoriser
le code afin d'en améliorer l'organisation. Vous voyagerez dans le temps, en comprenant
les décisions qui ont fait évoluer le développement web au cours de ces dernières années.

D'ici la fin, vous verrez comment Symfony peut vous épargner les tâches banales et vous
permet de reprendre le contrôle de votre code.

Un simple blog en PHP
---------------------

Dans ce chapitre, vous bâtirez une application de blog en utilisant uniquement du 
code PHP.
Pour commencer, créez une page qui affiche les billets du blog qui ont été sauvegardés
dans la base de données. Écrire en pur PHP est « rapide et sale » :

.. code-block:: html+php

    <?php
    // index.php
    $link = mysql_connect('localhost', 'myuser', 'mypassword');
    mysql_select_db('blog_db', $link);

    $result = mysql_query('SELECT id, title FROM post', $link);
    ?>

    <!DOCTYPE html>
    <html>
        <head>
            <title>List of Posts</title>
        </head>
        <body>
            <h1>List of Posts</h1>
            <ul>
                <?php while ($row = mysql_fetch_assoc($result)): ?>
                <li>
                    <a href="/show.php?id=<?php echo $row['id'] ?>">
                        <?php echo $row['title'] ?>
                    </a>
                </li>
                <?php endwhile; ?>
            </ul>
        </body>
    </html>

    <?php
    mysql_close($link);
    ?>

C'est rapide à écrire, rapide à exécuter, et, comme votre application évoluera, 
impossible à maintenir. Il y a plusieurs problèmes qui doivent être résolus :

* **aucune gestion d'erreur** : Que se passe-t-il si la connexion à la base de 
  données échoue?

* **mauvaise organisation** : si votre application évolue, ce fichier deviendra de 
  moins en moins maintenable. Où devrez-vous mettre le code qui traite la soumission
  d'un formulaire? Comment pourrez-vous valider les données? Où devrez-vous mettre
  le code qui envoie des courriels ?

* **Difficulté de réutiliser du code** : puisque tout se trouve dans un seul 
  fichier, il est impossible de réutiliser des parties de l'application pour 
  d'autres pages du blog.

.. note::

    Un autre problème non mentionné ici est le fait que la base de données est 
    liée à MySQL. Même si le sujet n'est pas couvert ici, Symfony intègre `Doctrine`_,
    une bibliothèque dédiée à l'abstraction des base de données
    et au mapping objet-relationnel.
    
Retroussons-nous les manches et résolvons ces problèmes ainsi que d'autres.

Isoler la présentation
~~~~~~~~~~~~~~~~~~~~~~

Le code sera mieux structuré en séparant la logique « applicative » du code qui s'occupe
de la « présentation » HTML :

.. code-block:: html+php

    <?php
    // index.php
    $link = mysql_connect('localhost', 'myuser', 'mypassword');
    mysql_select_db('blog_db', $link);

    $result = mysql_query('SELECT id, title FROM post', $link);

    $posts = array();
    while ($row = mysql_fetch_assoc($result)) {
        $posts[] = $row;
    }

    mysql_close($link);

    // inclut le code de la présentation HTML
    require 'templates/list.php';

Le code HTML est maintenant dans un fichier séparé (``templates/list.php``), 
qui est essentiellement un fichier HTML qui utilise une syntaxe PHP de template :

.. code-block:: html+php

    <!DOCTYPE html>
    <html>
        <head>
            <title>List of Posts</title>
        </head>
        <body>
            <h1>List of Posts</h1>
            <ul>
                <?php foreach ($posts as $post): ?>
                <li>
                    <a href="/read?id=<?php echo $post['id'] ?>">
                        <?php echo $post['title'] ?>
                    </a>
                </li>
                <?php endforeach; ?>
            </ul>
        </body>
    </html>

Par convention, le fichier qui contient la logique applicative (``index.php``) 
est appelé « contrôleur ». Le terme :term:`contrôleur` est un mot que vous allez 
entendre souvent, quel que soit le langage ou le framework utilisé. Il fait
simplement référence à *votre* code qui traite les entrées de l'utilisateur
et prépare une réponse.

Dans notre cas, le contrôleur prépare les données de la base de données et inclut 
ensuite un template pour présenter ces données. Comme le contrôleur est isolé, 
vous pouvez facilement changer *uniquement* le fichier de template si vous désirez
afficher les billets du blog dans un autre format (par exemple ``list.json.php`` 
pour un format JSON).

Isoler la logique applicative
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour l'instant, l'application ne contient qu'une seule page. Mais que faire 
si une deuxième page a besoin d'utiliser la même connexion à la base de données,
ou le même tableau de billets du blog? Refactorisez le code pour que le comportement
principal et les fonctions d'accès aux données de l'application soient isolés
dans un nouveau fichier appelé ``model.php`` :

.. code-block:: html+php

    <?php
    // model.php
    function open_database_connection()
    {
        $link = mysql_connect('localhost', 'myuser', 'mypassword');
        mysql_select_db('blog_db', $link);

        return $link;
    }

    function close_database_connection($link)
    {
        mysql_close($link);
    }

    function get_all_posts()
    {
        $link = open_database_connection();

        $result = mysql_query('SELECT id, title FROM post', $link);
        $posts = array();
        while ($row = mysql_fetch_assoc($result)) {
            $posts[] = $row;
        }
        close_database_connection($link);

        return $posts;
    }

.. tip::

   Le nom du fichier ``model.php`` est utilisé, car la logique et l'accès aux données
   de l'application sont traditionnellement connus sous le nom de couche « modèle ».
   Dans une application bien structurée, la majorité du code représentant la logique
   métier devrait se trouver dans le modèle (plutôt que dans le contrôleur). Et
   contrairement à cet exemple, seulement une portion (ou aucune) du
   modèle est en fait concernée par l'accès à la base de données.

Le contrôleur (``index.php``) est maintenant très simple :

.. code-block:: html+php

    <?php
    require_once 'model.php';

    $posts = get_all_posts();

    require 'templates/list.php';

Maintenant, la seule responsabilité du contrôleur est de récupérer les données
de la couche modèle de l'application (le modèle) et d'appeler le template à afficher
ces données.
C'est un exemple très simple du patron de conception Modèle-Vue-Contrôleur.

Isoler le layout
~~~~~~~~~~~~~~~~

À ce point-ci, l'application a été refactorisée en trois parties distinctes, offrant
plusieurs avantages et l'opportunité de réutiliser pratiquement la totalité du code
pour d'autres pages.

La seule partie du code qui *ne peut pas* être réutilisée est le layout de la page.
Corrigez cela en créant un nouveau fichier ``layout.php`` :

.. code-block:: html+php

    <!-- templates/layout.php -->
    <!DOCTYPE html>
    <html>
        <head>
            <title><?php echo $title ?></title>
        </head>
        <body>
            <?php echo $content ?>
        </body>
    </html>

Le template (``templates/list.php``) peut maintenant simplement « hériter »
du layout :

.. code-block:: html+php

    <?php $title = 'List of Posts' ?>

    <?php ob_start() ?>
        <h1>List of Posts</h1>
        <ul>
            <?php foreach ($posts as $post): ?>
            <li>
                <a href="/read?id=<?php echo $post['id'] ?>">
                    <?php echo $post['title'] ?>
                </a>
            </li>
            <?php endforeach; ?>
        </ul>
    <?php $content = ob_get_clean() ?>

    <?php include 'layout.php' ?>

Vous avez maintenant une méthode qui permet la réutilisation du layout. 
Malheureusement, pour accomplir cela, vous êtes forcé d'utiliser des 
fonctions PHP (``ob_start()``, ``ob_get_clean()``) dans le template.
Symfony utilise un composant de ``Templates`` qui permet d'accomplir ce résultat
proprement et facilement. Vous le verrez en action bientôt.

Ajout d'une page pour afficher un billet
----------------------------------------

La page « liste » a maintenant été refactorisée afin que le code soit mieux organisé
et réutilisable. Pour le prouver, ajoutez une page qui va afficher un billet identifié
par un paramètre de requête ``id``.

Pour commencer, créez une fonction dans le fichier ``model.php`` qui récupère un seul 
billet du blog en fonction d'un id passé en paramètre::

    // model.php
    function get_post_by_id($id)
    {
        $link = open_database_connection();

        $id = intval($id);
        $query = 'SELECT date, title, body FROM post WHERE id = '.$id;
        $result = mysql_query($query);
        $row = mysql_fetch_assoc($result);

        close_database_connection($link);

        return $row;
    }

Puis créez un nouveau fichier appelé ``show.php`` - le contrôleur pour cette 
nouvelle page :

.. code-block:: html+php

    <?php
    require_once 'model.php';

    $post = get_post_by_id($_GET['id']);

    require 'templates/show.php';

Finalement, créez un nouveau fichier de template - ``templates/show.php`` - afin
d'afficher un billet du blog :

.. code-block:: html+php

    <?php $title = $post['title'] ?>

    <?php ob_start() ?>
        <h1><?php echo $post['title'] ?></h1>

        <div class="date"><?php echo $post['date'] ?></div>
        <div class="body">
            <?php echo $post['body'] ?>
        </div>
    <?php $content = ob_get_clean() ?>

    <?php include 'layout.php' ?>

Créer cette nouvelle page est vraiment facile et aucun code n'est dupliqué.
Malgré tout, cette page introduit des problèmes persistants qu'un framework peut
résoudre. Par exemple, un paramètre de requête ``id`` manquant ou invalide va
générer une erreur fatale. Il serait préférable de générer une erreur 404, mais
cela ne peut pas vraiment être fait facilement. Pire, comme vous avez oublié
de nettoyer le paramètre  ``id`` avec la fonction ``intval()``, votre base
de données est sujette à des attaques d'injection de SQL.

Un autre problème est que chaque fichier contrôleur doit inclure le fichier 
``model.php``. Que se passerait-il si chaque contrôleur devait soudainement
inclure un fichier additionnel ou effectuer une quelconque tâche globale (comme
renforcer la sécurité)? Dans l'état actuel, tout nouveau code devra être ajouté
à chaque fichier contrôleur. Si vous oubliez de modifier un fichier, il serait
bon que ce ne soit pas relié à la sécurité...

.. _book-from_flat_php-front-controller:

Un « contrôleur frontal » à la rescousse
----------------------------------------

La solution est d'utiliser un :term:`contrôleur frontal` (front controller):
un fichier PHP à travers lequel chaque requête sera traitée. Avec un contrôleur
frontal, les URIs de l'application changent un peu, mais elles sont plus flexibles :

.. code-block:: text

    Sans contrôleur frontal
    /index.php          => page de liste des billets (exécution de index.php)
    /show.php           => page d'affichage d'un billet (exécution de show.php)

    avec index.php comme contrôleur frontal
    /index.php          => page de liste des billets (exécution de index.php)
    /index.php/show     => page d'affichage d'un billet (exécution de index.php)

.. tip::
    La portion ``index.php`` de l'URI peut être enlevée en utilisant les règles
    de réécriture d'URI d'Apache (ou équivalent). Dans ce cas, l'URI de la
    page de détail d'un billet serait simplement ``/show``.

En utilisant un contrôleur frontal, un seul fichier PHP (``index.php`` dans notre cas)
traite *chaque* requête. Pour la page de détail d'un billet, ``/index.php/show``
va donc exécuter le fichier ``index.php``, qui est maintenant responsable de router
en interne les requêtes en fonction de l'URI complète. Comme vous allez le voir,
un contrôleur frontal est un outil très puissant.

Créer le contrôleur frontal
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous êtes sur le point de franchir une étape *importante* de l'application. 
Avec un seul fichier qui traite toutes les requêtes, vous pouvez centraliser des
fonctionnalités comme la gestion de la sécurité, le chargement de la configuration,
et le routage. Dans cette application, ``index.php`` doit être assez intelligent
pour afficher la page de liste des billets *ou* la page de détail d'un billet en
fonction de l'URI demandée :

.. code-block:: html+php

    <?php
    // index.php

    // charge et initialise les bibliothèques globales
    require_once 'model.php';
    require_once 'controllers.php';

    // route la requête en interne
    $uri = $_SERVER['REQUEST_URI'];
    if ('/index.php' == $uri) {
        list_action();
    } elseif ('/index.php/show' == $uri && isset($_GET['id'])) {
        show_action($_GET['id']);
    } else {
        header('Status: 404 Not Found');
        echo '<html><body><h1>Page Not Found</h1></body></html>';
    }

Pour des raisons d'organisation, les contrôleurs (initialement  ``index.php`` et ``show.php``)
sont maintenant des fonctions PHP et ont été placés dans le fichier ``controllers.php`` :

.. code-block:: php

    function list_action()
    {
        $posts = get_all_posts();
        require 'templates/list.php';
    }

    function show_action($id)
    {
        $post = get_post_by_id($id);
        require 'templates/show.php';
    }

En tant que contrôleur frontal, ``index.php`` assume un nouveau rôle, celui
d'inclure les bibliothèques principales et de router l'application pour que l'un des
deux contrôleurs (les fonctions ``list_action()`` et ``show_action()``) soit appelé.
En réalité, le contrôleur frontal commence à ressembler et à agir comme le mécanisme
de Symfony qui prend en charge et achemine les requêtes.

.. tip::

   Un autre avantage du contrôleur frontal est d'avoir des URIs flexibles.
   Veuillez noter que l'URL de la page d'affichage d'un billet peut être changée
   de ``/show`` à ``/read`` en changeant le code à un seul endroit. Sans le
   contrôleur frontal, il aurait fallu renommer un fichier. Avec Symfony, les
   URLs sont encore plus flexibles.

Jusqu'ici, l'application est passée d'un seul fichier PHP à une organisation qui
permet la réutilisation du code. Vous devriez être plus heureux, mais loin d'être
satisfait. Par exemple, le système de routing est inconstant, et ne reconnaîtrait
pas que la page de liste de billets (``/index.php``) devrait aussi être accessible
via ``/`` (si Apache rewrite est activé). Aussi, au lieu de développer une
application de blog, beaucoup de temps a été consacré à l'« architecture » du code
(par exemple le routing, l'appel aux contrôleurs, aux templates...). Plus de temps
devrait être consacré à la prise en charge des formulaires, la validation des champs, 
la journalisation et la sécurité. Pourquoi réinventer des solutions pour tout
ces problèmes ?

.. _add-a-touch-of-symfony2:

Ajoutez une touche de Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony à la rescousse. Avant d'utiliser Symfony, vous devez d'abord le télécharger.
Vous pouvez le faire en utilisant Composer, qui se chargera de télécharger la bonne
version ainsi que toutes ses dépendances, et qui chargera également les classes de
manière automatique (grâce à un autoloader). Un autoloader est un outil qui rend
possible l'utilisation de classes sans avoir à inclure les fichiers qui contiennent
chaque classe.

Dans votre répertoire racine, créez un fichier ``composer.json`` avec le contenu
suivant :

.. code-block:: json

    {
        "require": {
            "symfony/symfony": "2.7.*"
        },
        "autoload": {
            "files": ["model.php","controllers.php"]
        }
    }
    
Ensuite, `téléchargez Composer`_ puis exécutez la commande suivante, qui téléchargera
Symfony dans un répertoire /vendor :

.. code-block:: bash

    $ composer install

En plus de télécharger vos dépendances, Composer génère un fichier ``vendor/autoload.php``,
qui prendra en charge l'autoloading pour tous les fichiers du framework Symfony, de même
que les fichiers spécifiés dans la section autoload de votre ``composer.json``.

La philosophie de base de Symfony est que la principale activité d'une application
est d'interpréter chaque requête et de retourner une réponse. Pour cela, Symfony
fournit les classes :class:`Symfony\\Component\\HttpFoundation\\Request` et
:class:`Symfony\\Component\\HttpFoundation\\Response`. Ces classes sont des
représentations orientées-objet des requêtes HTTP brutes qui sont en train d'être
exécutées et d'une réponse HTTP qui est retournée. Utilisez-les pour améliorer le
blog :

.. code-block:: html+php

    <?php
    // index.php
    require_once 'vendor/autoload.php';

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;

    $request = Request::createFromGlobals();

    $uri = $request->getPathInfo();
    if ('/' == $uri) {
        $response = list_action();
    } elseif ('/show' == $uri && $request->query->has('id')) {
        $response = show_action($request->query->get('id'));
    } else {
        $html = '<html><body><h1>Page Not Found</h1></body></html>';
        $response = new Response($html, 404);
    }

    // affiche les entêtes et envoie la réponse
    $response->send();


Les contrôleurs sont maintenant chargés de retourner un objet ``Response``.
Pour simplifier les choses, vous pouvez ajouter une nouvelle fonction ``render_template()``, 
qui agit un peu comme le moteur de template de Symfony :

.. code-block:: php

    // controllers.php
    use Symfony\Component\HttpFoundation\Response;

    function list_action()
    {
        $posts = get_all_posts();
        $html = render_template('templates/list.php', array('posts' => $posts));

        return new Response($html);
    }

    function show_action($id)
    {
        $post = get_post_by_id($id);
        $html = render_template('templates/show.php', array('post' => $post));

        return new Response($html);
    }

    // fonction helper pour afficher un template
    function render_template($path, array $args)
    {
        extract($args);
        ob_start();
        require $path;
        $html = ob_get_clean();

        return $html;
    }

En intégrant une petite partie de Symfony, l'application est plus flexible
et fiable. La requête (``Request``) permet d'accéder aux informations d'une requête HTTP.
De manière plus spécifique, la méthode ``getPathInfo()`` retourne une URI épurée (retourne
toujours ``/show`` et jamais ``/index.php/show``). Donc, même si l'utilisateur va à 
``/index.php/show``, l'application est assez intelligente pour router la requête vers 
``show_action()``.

L'objet ``Response`` offre de la flexibilité pour construire une réponse HTTP, permettant
d'ajouter des entêtes HTTP et du contenu au travers d'une interface orientée objet.
Et même si les réponses de cette application sont simples, cette flexibilité sera un atout
lorsque l'application évoluera.

.. _the-sample-application-in-symfony2:

L'application exemple en Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le blog a *beaucoup* évolué, mais il contient beaucoup de code pour une si simple application.
Durant cette évolution, vous avez créé un mécanisme simple de routage et une méthode utilisant
``ob_start()`` et ``ob_get_clean()`` pour afficher les templates.
Si, pour une raison, vous deviez continuer à construire ce « framework », vous pourriez utiliser
les composants indépendants `Routing`_ et `Templating`_ de Symfony, qui apportent déjà une
solution à ces problèmes.

Au lieu de résoudre à nouveau des problèmes classiques, vous pouvez laisser Symfony s'en
occuper pour vous. Voici la même application, en utilisant cette fois-ci Symfony::

    // src/AppBundle/Controller/BlogController.php
    namespace AppBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function listAction()
        {
            $posts = $this->get('doctrine')
                ->getManager()
                ->createQuery('SELECT p FROM AcmeBlogBundle:Post p')
                ->execute();

            return $this->render('Blog/list.html.php', array('posts' => $posts));
        }

        public function showAction($id)
        {
            $post = $this->get('doctrine')
                ->getManager()
                ->getRepository('AppBundle:Post')
                ->find($id);

            if (!$post) {
                // cause the 404 page not found to be displayed
                throw $this->createNotFoundException();
            }

            return $this->render('Blog/show.html.php', array('post' => $post));
        }
    }

Les deux contrôleurs sont toujours simples. Chacun utilise la :doc:`librairie ORM Doctrine</book/doctrine>`
pour récupérer les objets de la base de données et le composant de ``Templating``
pour afficher les templates et retourner un objet ``Response``. Le template
qui affiche la liste est maintenant un peu plus simple :

.. code-block:: html+php

    <!-- app/Resources/views/Blog/list.html.php -->
    <?php $view->extend('layout.html.php') ?>

    <?php $view['slots']->set('title', 'List of Posts') ?>

    <h1>List of Posts</h1>
    <ul>
        <?php foreach ($posts as $post): ?>
        <li>
            <a href="<?php echo $view['router']->generate(
                'blog_show',
                array('id' => $post->getId())
            ) ?>">
                <?php echo $post->getTitle() ?>
            </a>
        </li>
        <?php endforeach; ?>
    </ul>

Le layout est à peu près identique :

.. code-block:: html+php

    <!-- app/Resources/views/layout.html.php -->
    <!DOCTYPE html>
    <html>
        <head>
            <title><?php echo $view['slots']->output(
                'title',
                'Default title'
            ) ?></title>
        </head>
        <body>
            <?php echo $view['slots']->output('_content') ?>
        </body>
    </html>

.. note::

    Le template d'affichage d'un billet est laissé comme exercice, cela devrait être
    assez simple si vous vous basez sur le template de liste.

Lorsque le moteur de Symfony (appelé ``Kernel``) démarre, il a besoin d'une table
qui définit quels contrôleurs exécuter en fonction des informations de la requête.
Une table de routage fournit cette information dans un format lisible :

.. code-block:: yaml

    # app/config/routing.yml
    blog_list:
        path:     /blog
        defaults: { _controller: AppBundle:Blog:list }

    blog_show:
        path:     /blog/show/{id}
        defaults: { _controller: AppBundle:Blog:show }

Maintenant que Symfony prend en charge toutes les taches banales, le contrôleur frontal
est extrêmement simple. Et comme il fait très peu de chose, vous n'aurez jamais à le modifier
une fois que vous l'aurez créé (et si vous utilisez une `distribution de Symfony`_, vous n'aurez
même pas à le créer !)::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->handle(Request::createFromGlobals())->send();

Le seul rôle du contrôleur frontal est d'initialiser le moteur Symfony (``Kernel``)
et de lui passer à un objet ``Request`` à traiter. Le coeur de Symfony utilise alors 
la table de routage pour déterminer quel contrôleur appeler. Comme précédemment, c'est à la
méthode du contrôleur de retourner un objet ``Response``.

Pour une représentation visuelle qui montre comment Symfony traite chaque requête,
voir :ref:`le diagramme de flux de contrôle d'une requête<request-flow-figure>`.

.. _where-symfony2-delivers:

En quoi Symfony tient ses promesses
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans les chapitres suivants, vous en apprendrez plus sur le fonctionnement 
chaque élément de Symfony et sur la structure recommandée d'un projet. Pour l'instant,
voyons en quoi la migration du blog depuis une version PHP en une version Symfony nous
simplifie la vie :

* Votre application est constituée de **code clair et bien organisé** (même si Symfony ne vous force
  pas à le faire). Cela encourage la **réutilisabilité** et permet aux nouveaux développeurs d'être 
  productifs sur votre projet plus rapidement;

* 100% du code que vous écrivez est pour *votre* application. Vous **n'avez pas à développer
  ou à maintenir des outils de bas niveau** tel que :ref:`le chargeur automatique<autoloading-introduction-sidebar>`,
  :doc:`le routage</book/routing>`, ou  :doc:`les contrôleurs</book/controller>`;

* Symfony vous donne **accès à des outils open source** comme Doctrine et le composants 
  de templates, de sécurité, de formulaires, de validation et de traduction 
  (pour n'en nommer que quelques-uns);

* L'application profite maintenant d'**URLs complètement flexibles**, grâce au 
  composant de routage (``Routing``);

* L'architecture centrée autour du protocole HTTP vous donne accès à des outils puissants tels que
  le **cache HTTP** permis par le **cache HTTP interne de Symfony** ou par d'autres outils 
  plus puissants tels que `Varnish`_. Ce point est couvert dans un chapitre consacré au
  :doc:`cache</book/http_cache>`.

Et peut-être le point le plus important, en utilisant Symfony, vous avez maintenant accès 
à un ensemble d'**outils de qualité open source développés par la communauté Symfony** ! 
Un large choix d'outils de la communauté Symfony se trouve sur `KnpBundles.com`_.


De meilleurs templates
----------------------

Si vous choisissez de l'utiliser, Symfony vient de facto avec un moteur de template appelé
`Twig`_ qui rend les templates plus rapides à écrire et plus faciles à lire.
Cela veut dire que l'application exemple pourrait contenir moins de code ! Prenez par exemple,
le template de liste de billets écrit avec Twig :

.. code-block:: html+jinja

    {# app/Resources/views/blog/list.html.twig #}
    {% extends "layout.html.twig" %}

    {% block title %}List of Posts{% endblock %}

    {% block body %}
        <h1>List of Posts</h1>
        <ul>
            {% for post in posts %}
            <li>
                <a href="{{ path('blog_show', {'id': post.id}) }}">
                    {{ post.title }}
                </a>
            </li>
            {% endfor %}
        </ul>
    {% endblock %}

Le template du layout associé ``layout.html.twig`` est encore plus simple à écrire :

.. code-block:: html+jinja

    {# app/Resources/views/layout.html.twig #}
    <!DOCTYPE html>
    <html>
        <head>
            <title>{% block title %}Default title{% endblock %}</title>
        </head>
        <body>
            {% block body %}{% endblock %}
        </body>
    </html>

Twig est très bien supporté par Symfony. Et bien que les templates PHP vont toujours
être supportés par Symfony, nous allons continuer à vanter les nombreux avantages de Twig.
Pour plus d'information, voir le :doc:`chapitre sur les templates</book/templating>`.

Apprenez en lisant le Cookbook
------------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/service`

.. _`Doctrine`: http://www.doctrine-project.org
.. _`téléchargez Composer`: http://getcomposer.org/download/
.. _`Routing`: https://github.com/symfony/Routing
.. _`Templating`: https://github.com/symfony/Templating
.. _`KnpBundles.com`: http://knpbundles.com/
.. _`Twig`: http://twig.sensiolabs.org
.. _`Varnish`: https://www.varnish-cache.org
.. _`PHPUnit`: http://www.phpunit.de
.. _`distribution de Symfony`: https://github.com/symfony/symfony-standard