.. index::
   single: Templating

Créer et utiliser les templates
===============================

Comme vous le savez, le :doc:`contrôleur </book/controller>` est
responsable de la gestion de toutes les requêtes d'une application
Symfony2. En réalité, le contrôleur délègue le plus gros du travail à
d'autres classes afin que le code puisse être testé et
réutilisé. Quand un contrôleur a besoin de générer du HTML, CSS ou
tout autre contenu, il donne la main au moteur de template.  Dans ce
chapitre, vous apprendrez comment écrire des templates puissants qui
peuvent être utilisés pour retourner du contenu à l'utilisateur,
remplir le corps d'emails et bien d'autres. Vous apprendrez des
raccourcis, des méthodes ingénieuses pour étendre les templates et
surtout comment les réutiliser.

.. note::
    L'affichage des templates est abordé dans la page
    :ref:`contrôleur <controller-rendering-templates>` du Book.
    
.. index::
   single: Templating; What is a template?

Templates
---------

Un template est un simple fichier texte qui peut générer n'importe quel format
basé sur du texte (HTML, XML, CSV, LaTeX ...). Le type de template le plus connu
est le template *PHP* - un fichier texte interprété par PHP qui contient du texte
et du code PHP::

    <!DOCTYPE html>
    <html>
        <head>
            <title>Bienvenue sur Symfony !</title>
        </head>
        <body>
            <h1><?php echo $page_title ?></h1>

            <ul id="navigation">
                <?php foreach ($navigation as $item): ?>
                    <li>
                        <a href="<?php echo $item->getHref() ?>">
                            <?php echo $item->getCaption() ?>
                        </a>
                    </li>
                <?php endforeach; ?>
            </ul>
        </body>
    </html>

.. index:: Twig; Introduction

Mais Symfony2 contient un langage de template encore plus puissant appelé `Twig`_.
Twig vous permet d'écrire des templates simples et lisibles qui sont plus
compréhensibles par les web designers et, dans bien des aspects, plus puissants
que les templates PHP :

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
        <head>
            <title>Bienvenue sur Symfony !</title>
        </head>
        <body>
            <h1>{{ page_title }}</h1>

            <ul id="navigation">
                {% for item in navigation %}
                    <li><a href="{{ item.href }}">{{ item.caption }}</a></li>
                {% endfor %}
            </ul>
        </body>
    </html>

Twig définit deux types de syntaxe spéciale :

* ``{{ ... }}``: « Dit quelque chose »: écrit une variable ou le résultat d'une
  expression  dans le template ;

* ``{% ... %}``: « Fait quelque chose »: un **tag** qui contrôle la logique
  du template ; il est utilisé pour exécuter des instructions comme la boucle
  for par exemple.

.. note::

   Il y a une troisième syntaxe utilisée pour les commentaires : ``{# un commentaire #}``.
   Cette syntaxe peut être utilisée sur plusieurs lignes comme la syntaxe PHP
   ``/* commentaire */`` qui est équivalente.

Twig contient également des **filtres**, qui modifient le contenu avant de le rendre.
Le filtre suivant met la variable ``title`` en majuscule avant de la rendre :

.. code-block:: jinja

    {{ title | upper }}

Twig est fourni avec une longue liste de `tags`_ et de `filtres`_ qui sont disponibles
par défaut. Vous pouvez même `ajouter vos propres extensions`_ à Twig si besoin.

.. tip::

    Créer une nouvelle extension Twig est aussi simple que de créer un nouveau
    service et de le taguer avec ``twig.extension`` :ref:`tag<reference-dic-tags-twig-extension>`.

Comme vous le verrez tout au long de la documentation, Twig supporte aussi les
fonctions, et de nouvelles fonctions peuvent être ajoutées. Par exemple, la fonction
suivante utilise le tag standard ``for`` et la fonction ``cycle`` pour écrire dix
balises div en alternant les classes ``odd`` et ``even`` :

.. code-block:: html+jinja

    {% for i in 0..10 %}
      <div class="{{ cycle(['odd', 'even'], i) }}">
        <!-- some HTML here -->
      </div>
    {% endfor %}

Tout au long de ce chapitre, les exemples de templates seront donnés à la fois
avec Twig et PHP.

.. tip::

    Si vous choisissez de ne *pas* utiliser Twig et que vous le désactivez, vous
    devrez implémenter votre propre gestionnaire d'exceptions via l'évènement ``kernel.exception``.

.. sidebar:: Pourquoi Twig?

    Les templates Twig sont conçus pour être simples et ne traiteront
    aucun code PHP. De par sa conception, le système de template Twig
    s'occupe de la présentation, pas de la logique. Plus vous
    utiliserez Twig, plus vous apprécierez cette distinction et en
    bénéficierez. Et bien sûr, vous serez adoré par tous les web
    designers.

    Twig peut aussi faire des choses que PHP ne pourrait pas faire, comme le contrôle
    d'espaces blancs, le bac à sable, l'échappement de caractères automatique et
    contextuel et l'inclusion de fonctions et de filtres personnalisés qui n'affectent
    que les templates. Twig contient de petites fonctionnalités qui rendent
    l'écriture de template plus facile et plus concise. Prenez l'exemple suivant, il
    combine une boucle avec l'instruction logique ``if`` :
    
    .. code-block:: html+jinja
    
        <ul>
            {% for user in users if user.active %}
                <li>{{ user.username }}</li>
            {% else %}
                <li>Aucun utilisateur trouvé.</li>
            {% endfor %}
        </ul>

.. index::
   pair: Twig; Cache

Twig et la mise en cache
~~~~~~~~~~~~~~~~~~~~~~~~

Twig est rapide. Chaque template Twig est compilé en une classe PHP natif qui est
rendue à l'exécution. Les classes compilées sont stockées dans le répertoire
``app/cache/{environment}/twig`` (où ``{environment}`` est l'environnement, par
exemple ``dev`` ou ``prod``) et elles peuvent être utiles dans certains cas pour
débugguer. Lisez le chapitre :ref:`environments-summary` pour plus d'informations
sur les environnements.

Lorsque le mode ``debug`` est activé (par exemple en environnement de ``dev``), 
un template Twig sera automatiquement recompilé à chaque fois qu'un changement y
sera apporté. Cela signifie que durant le développement, vous pouvez effectuer des
modifications dans un template Twig et voir instantanément les changements sans vous
soucier de vider le cache.

Lorsque le mode ``debug`` est désactivé (par exemple en environnement de ``prod``),
en revanche, vous devrez vider le répertoire de cache Twig afin que le template soit
régénéré. Souvenez-vous bien de cela lorsque vous déploirez votre application.

.. index::
   single: Templating; Inheritance

L'héritage de template et layouts
---------------------------------

Bien souvent, les templates d'un projet partagent des éléments communs, comme les
entêtes, pieds de page et menus latéraux. Dans Symfony2, nous abordons ce problème
différemment : un template peut être décoré par un autre. Cela fonctionne exactement
comme les classes PHP : l'héritage de template vous permet de bâtir un template
« layout » de base qui contient tous les éléments communs de votre site et de définir
des **blocs** (comprenez « classe PHP avec des méthodes de base »). Un template
enfant peut étendre le template layout et surcharger n'importe lequel de ses blocs
(comprenez « une sous-classe PHP qui surcharge certaines méthodes de sa classe parente »).

Tout d'abord, construisez un fichier layout :

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block title %}Test Application{% endblock %}</title>
            </head>
            <body>
                <div id="sidebar">
                    {% block sidebar %}
                    <ul>
                        <li><a href="/">Home</a></li>
                        <li><a href="/blog">Blog</a></li>
                    </ul>
                    {% endblock %}
                </div>

                <div id="content">
                    {% block body %}{% endblock %}
                </div>
            </body>
        </html>

    .. code-block:: php

        <!-- app/Resources/views/base.html.php -->
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title><?php $view['slots']->output('title', 'Test Application') ?></title>
            </head>
            <body>
                <div id="sidebar">
                    <?php if ($view['slots']->has('sidebar')): ?>
                        <?php $view['slots']->output('sidebar') ?>
                    <?php else: ?>
                        <ul>
                            <li><a href="/">Home</a></li>
                            <li><a href="/blog">Blog</a></li>
                        </ul>
                    <?php endif; ?>
                </div>

                <div id="content">
                    <?php $view['slots']->output('body') ?>
                </div>
            </body>
        </html>

.. note::

    Bien que les explications sur l'héritage de template concernent Twig, la
    philosophie est la même pour les templates PHP.

Ce template définit le squelette HTML de base d'un document constitué simplement
de deux colonnes. Dans cette exemple, trois espaces ``{% block %}`` sont définis
(``title``, ``sidebar`` et ``body``). Chacun de ces blocs peut être soit
surchargé dans un template enfant ou soit conserver leur code d'origine. Ce
template peut aussi être rendu directement. Dans ce cas, les blocs ``title``,
``sidebar`` et ``body`` conserveront simplement les valeurs par défaut
utilisées dans ce template.

Un template enfant peut ressembler à cela : 

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
        {% extends '::base.html.twig' %}

        {% block title %}My cool blog posts{% endblock %}

        {% block body %}
            {% for entry in blog_entries %}
                <h2>{{ entry.title }}</h2>
                <p>{{ entry.body }}</p>
            {% endfor %}
        {% endblock %}

    .. code-block:: php

        <!-- src/Acme/BlogBundle/Resources/views/Blog/index.html.php -->
        <?php $view->extend('::base.html.php') ?>

        <?php $view['slots']->set('title', 'My cool blog posts') ?>

        <?php $view['slots']->start('body') ?>
            <?php foreach ($blog_entries as $entry): ?>
                <h2><?php echo $entry->getTitle() ?></h2>
                <p><?php echo $entry->getBody() ?></p>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

.. note::

   Le template parent est identifié grâce à une chaîne de caractères
   particulière (``::base.html.twig``) qui indique que ce template se trouve
   dans le dossier ``app/Resources/views`` du projet. Cette convention de
   nommage est complètement expliquée dans :ref:`template-naming-locations`.

La clé de l'héritage de template est la balise ``{% extends %}``. Elle indique
au moteur de template d'évaluer d'abord le template de base, qui configure le
layout et définit plusieurs blocs. Le template enfant est ensuite
rendu. Durant ce traitement les blocs parents ``title`` et ``body`` sont
remplacés par ceux de l'enfant. Dépendant de la valeur de ``blog_entries``, la
sortie peut ressembler à ceci :

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <title>Mes billets de blog cools</title>
        </head>
        <body>
            <div id="sidebar">
                <ul>
                    <li><a href="/">Accueil</a></li>
                    <li><a href="/blog">Blog</a></li>
                </ul>
            </div>

            <div id="content">
                <h2>Mon premier post</h2>
                <p>Le corps du premier post.</p>

                <h2>Un autre post</h2>
                <p>Le corps du deuxième post.</p>
            </div>
        </body>
    </html>

Remarquons que comme le template enfant n'a pas défini le bloc ``sidebar``, la
valeur du template parent est utilisée à la place. Le contenu d'une balise 
``{% block %}`` d'un template parent est toujours utilisé par défaut.

Vous pouvez utiliser autant de niveaux d'héritage que vous souhaitez. Dans la
section suivante, un modèle commun d'héritage à trois niveaux sera expliqué,
ainsi que l'organisation des templates au sein d'un projet Symfony2.

Quand on travaille avec l'héritage de templates, il est important de garder ces
astuces à l'esprit :

* Si vous utilisez ``{% extends %}`` dans un template, alors ce doit être la
  première balise de ce template.

* Plus vous utilisez les balises ``{% block %}`` dans les templates, mieux
  c'est. Souvenez-vous, les templates enfants ne doivent pas obligatoirement
  définir tous les blocs parents, donc créez autant de blocs que vous
  désirez dans le template de base et attribuez leurs une configuration par
  défaut. Plus vous avez de blocs dans le template de base, plus le layout
  sera flexible.

* Si vous vous retrouvez à dupliquer du contenu dans plusieurs templates, cela
  veut probablement dire que vous devriez déplacer ce contenu dans un 
  ``{% block  %}`` d'un template parent. Dans certain cas, la meilleure solution 
  peut être de déplacer le contenu dans un nouveau template et de l'``include`` 
  (voir :ref:`including-templates`).

* Si vous avez besoin de récupérer le contenu d'un bloc d'un template parent,
  vous pouvez utiliser la fonction ``{{ parent() }}``. C'est utile si on
  souhaite compléter le contenu du bloc parent au lieu de le réécrire
  totalement :

    .. code-block:: html+jinja

        {% block sidebar %}
            <h3>Table of Contents</h3>
            ...
            {{ parent() }}
        {% endblock %}

.. index::
   single: Templating; Naming Conventions
   single: Templating; File Locations

.. _template-naming-locations:

Nommage de template et Emplacements
-----------------------------------

Par défaut, les templates peuvent se trouver dans deux emplacements
différents :

* ``app/Resources/views/`` : Le dossier ``views`` de l'application peut
  aussi bien contenir le template de base de l'application (c-a-d le layout de
  l'application) ou les templates qui surchargent les templates des bundles
  (voir :ref:`overriding-bundle-templates`);

* ``path/to/bundle/Resources/views/`` : Chaque bundle place leurs
  templates dans leur dossier ``Resources/views`` (et sous dossiers). La
  plupart des templates résident au sein d'un bundle.

Symfony2 utilise une chaîne de caractères au format
**bundle**:**controller**:**template** pour les templates. Cela permet plusieurs
types de templates, chacun se situant à un endroit spécifique :

* ``AcmeBlogBundle:Blog:index.html.twig``: Cette syntaxe est utilisée pour
  spécifier un template pour une page donnée. Les trois parties de la chaîne de
  caractères, séparées par deux-points (``:``), signifient ceci :

    * ``AcmeBlogBundle``: (*bundle*) le template se trouve dans le 
      ``AcmeBlogBundle`` (``src/Acme/BlogBundle`` par exemple);

    * ``Blog``: (*controller*) indique que le template se trouve dans le
      sous-répertoire ``Blog`` de ``Resources/views``;

    * ``index.html.twig``: (*template*) le nom réel du fichier est 
      ``index.html.twig``.

  En supposant que le ``AcmeBlogBundle`` se trouve à ``src/Acme/BlogBundle``, le
  chemin final du layout serait ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``.

* ``AcmeBlogBundle::layout.html.twig``: Cette syntaxe fait référence à un
  template de base qui est spécifique au ``AcmeBlogBundle``. Puisque la partie du
  milieu, « controller », est absente (``Blog`` par exemple), le template se
  trouve à ``Resources/views/layout.html.twig`` dans ``AcmeBlogBundle``.

* ``::base.html.twig``: Cette syntaxe fait référence à un template de base d'une
  application ou layout. Remarquez que la chaîne de caractères commence par deux
  deux-points (``::``), ce qui signifie que les deux parties *bundle* et
  *controller* sont absentes. Ce qui signifie que le template ne se trouve dans
  aucun bundle, mais directement dans le répertoire racine
  ``app/Resources/views/``.

Dans la section :ref:`overriding-bundle-templates`, vous verrez comment les
templates interagissent avec ``AcmeBlogBundle``. Par exemple, il est possible de
surcharger un template en plaçant un template du même nom dans le répertoire
``app/Resources/AcmeBlogBundle/views/``. Cela offre la possibilité de surcharger
les templates fournis par n'importe quel vendor bundle.

.. tip::

    La syntaxe de nommage des templates doit vous paraître familière -
    c'est la même convention de nommage qui est utilisée pour faire référence à
    :ref:`controller-string-syntax`.

Les Suffixes de Template 
~~~~~~~~~~~~~~~~~~~~~~~~

Le format **bundle**:**controller**:**template** de chaque template spécifie
*où* se situe le fichier template. Chaque nom de template a aussi deux extensions
qui spécifient le *format* et le *moteur* (engine) pour le template.

* **AcmeBlogBundle:Blog:index.html.twig** - format HTML, moteur de template Twig

* **AcmeBlogBundle:Blog:index.html.php** - format HTML, moteur de template PHP

* **AcmeBlogBundle:Blog:index.css.twig** - format CSS, moteur de template Twig

Par défaut, tout template de Symfony2 peut être écrit soit en Twig ou en PHP, et
la dernière partie de l'extension (``.twig`` ou ``.php`` par exemple) spécifie
lequel de ces deux *moteurs* sera utilisé. La première partie de l'extension
(``.html``, ``.css`` par exemple) désigne le format final du template qui sera
généré. Contrairement au moteur, qui détermine comment Symfony2 parsera le
template, il s'agit là simplement une méthode organisationnelle qui est utilisée dans le
cas où la même ressource a besoin d'être rendue en HTML (``index.html.twig``),
en XML (``index.xml.twig``), ou tout autre format. Pour plus d'informations,
lisez la section :ref:`template-formats`.

.. note::

   Les *moteurs* disponibles peuvent être configurés et d'autres moteurs peuvent
   être ajoutés. Voir :ref:`Templating Configuration<template-configuration>`
   pour plus de détails.

.. index::
   single: Templating; Tags and helpers
   single: Templating; Helpers

Balises et Helpers
------------------

Vous avez maintenant compris les bases des templates, comment ils sont nommés et
comment utiliser l'héritage de templates. Les parties les plus difficiles sont
d'ores et déjà derrière vous. Dans cette section, vous apprendrez à utiliser un
ensemble d'outils disponibles pour aider à réaliser les tâches les plus communes
avec les templates comme l'inclusion de templates, faire des liens entre des
pages et l'inclusion d'images.

Symfony2 regroupe plusieurs paquets dont plusieurs spécialisés dans les balises
et fonctions Twig qui facilitent le travail du web designer. En PHP, le
système de templates fournit un système de *helper* extensible. Ce système fournit des
propriétés utiles dans le contexte des templates.

Nous avons déjà vu quelques balises Twig (``{% block %}`` & ``{% extends %}``)
ainsi qu'un exemple de helper PHP (``$view['slots']``). Apprenons-en un peu
plus.

.. index::
   single: Templating; Including other templates

.. _including-templates:

L'inclusion de Templates
~~~~~~~~~~~~~~~~~~~~~~~~

Vous voudrez souvent inclure le même template ou fragment de code dans
différentes pages. Par exemple, dans une application avec un espace « nouveaux
articles », le code du template affichant un article peut être utilisé sur la
page détaillant l'article, sur une page affichant les articles les plus
populaires, ou dans une liste des derniers articles.

Quand vous avez besoin de réutiliser une grande partie d'un code PHP,
typiquement vous déplacez le code dans une nouvelle classe PHP ou dans
une fonction. La même chose s'applique aussi aux templates. En
déplaçant le code réutilisé dans son propre template, il peut être
inclus par tous les autres templates. D'abord, créez le template que
vous souhaiterez réutiliser.

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/articleDetails.html.twig #}
        <h2>{{ article.title }}</h2>
        <h3 class="byline">by {{ article.authorName }}</h3>

        <p>
          {{ article.body }}
        </p>

    .. code-block:: php

        <!-- src/Acme/ArticleBundle/Resources/views/Article/articleDetails.html.php -->
        <h2><?php echo $article->getTitle() ?></h2>
        <h3 class="byline">by <?php echo $article->getAuthorName() ?></h3>

        <p>
          <?php echo $article->getBody() ?>
        </p>

L'inclusion de ce template dans tout autre template est simple :

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/list.html.twig #}
        {% extends 'AcmeArticleBundle::layout.html.twig' %}

        {% block body %}
            <h1>Recent Articles<h1>

            {% for article in articles %}
                {{ include('AcmeArticleBundle:Article:articleDetails.html.twig', {'article': article}) }}
            {% endfor %}
        {% endblock %}

    .. code-block:: php

        <!-- src/Acme/ArticleBundle/Resources/views/Article/list.html.php -->
        <?php $view->extend('AcmeArticleBundle::layout.html.php') ?>

        <?php $view['slots']->start('body') ?>
            <h1>Recent Articles</h1>

            <?php foreach ($articles as $article): ?>
                <?php echo $view->render('AcmeArticleBundle:Article:articleDetails.html.php', array('article' => $article)) ?>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

Le template est inclus via l'utilisation de la balise ``{{ include() }}``.
Remarquons que le nom du template suit la même convention habituelle. Le
template ``articleDetails.html.twig`` utilise une variable ``article``. Elle est
passée au template ``list.html.twig`` en utilisant la commande ``with``.

.. tip::

    La syntaxe ``{'article': article}`` est la syntaxe standard de Twig pour les
    tables de hachage (hash maps) (c-a-d un tableau clé-valeurs). Si vous souhaitez
    passer plusieurs elements, cela ressemblera à ceci : ``{'foo': foo, 'bar': bar}``.

.. index::
   single: Templating; Embedding action

.. _templating-embedding-controller:

Contrôleurs imbriqués
~~~~~~~~~~~~~~~~~~~~~

Dans certains cas, vous aurez besoin d'inclure plus qu'un simple
template. Supposons que vous avez un menu latéral dans votre layout qui contient
les trois articles les plus récents. La récupération des trois articles les plus
récents peut nécessiter l'inclusion d'une requête vers une base de données et de
réaliser d'autres opérations logiques qui ne peuvent pas être effectuées dans
un template.

La solution consiste simplement à imbriquer les résultats d'un contrôleur dans un
template. Dans un premier temps, créez un contrôleur qui retourne un certain
nombre d'articles récents::

    // src/Acme/ArticleBundle/Controller/ArticleController.php

    class ArticleController extends Controller
    {
        public function recentArticlesAction($max = 3)
        {
            // un appel en base de données ou n'importe quoi qui retourne les "$max" plus récents articles
            $articles = ...;

            return $this->render('AcmeArticleBundle:Article:recentList.html.twig', array('articles' => $articles));
        }
    }

Le template ``recentList`` est simplement le suivant :

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}
        {% for article in articles %}
          <a href="/article/{{ article.slug }}">
              {{ article.title }}
          </a>
        {% endfor %}

    .. code-block:: php

        <!-- src/Acme/ArticleBundle/Resources/views/Article/recentList.html.php -->
        <?php foreach ($articles as $article): ?>
            <a href="/article/<?php echo $article->getSlug() ?>">
                <?php echo $article->getTitle() ?>
            </a>
        <?php endforeach; ?>

.. note::

    Notez que dans l'exemple de cet article, les URLs sont codées en dur
    (``/article/*slug*`` par exemple). Ce n'est pas une bonne pratique. Dans
    la section suivante, vous apprendrez comment le faire correctement.

Pour inclure le contrôleur, vous avez besoin de faire référence à ce dernier en
utilisant la chaîne de caractères standard pour les contrôleurs
(c-a-d **bundle**:**controller**:**action**) :

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        ...

        <div id="sidebar">
            {{ render(controller('AcmeArticleBundle:Article:recentArticles', { 'max': 3 })) }}
        </div>

    .. code-block:: php

        <!-- app/Resources/views/base.html.php -->
        ...

        <div id="sidebar">
            <?php echo $view['actions']->render('AcmeArticleBundle:Article:recentArticles', array('max' => 3)) ?>
        </div>

A chaque fois que vous pensez avoir besoin d'une variable ou de quelques informations
auxquelles vous n'avez pas accès à partir d'un template, penser à rendre un
contrôleur. Les contrôleurs sont rapides à l'exécution et favorisent une bonne
organisation et réutilisabilité du code.


Contenu asynchrone avec hinclude.js
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1 
    hinclude.js support was added in Symfony 2.1

Les contrôleurs peuvent être imbriqués de façon asynchrone avec la bibliothèque
javascript hinclude.js_.
Comme le contenu imbriqué vient d'une autre page (un d'un autre contrôleur),
Symfony2 utilise le helper standard ``render`` pour configurer les tags ``hinclude``:
 
.. configuration-block::

    .. code-block:: jinja
 
        {% render '...:news' with {}, {'standalone': 'js'} %}
  
    .. code-block:: php
 
        <?php echo $view['actions']->render('...:news', array(), array('standalone' => 'js')) ?>

.. note::

   hinclude.js_ doit être inclus dans votre page pour fonctionner.

Le contenu par défaut (pendant le chargement ou si javascript n'est pas activé) peut
être défini de manière globale dans la configuration de votre application :
 
.. configuration-block::
 
    .. code-block:: yaml
 
        # app/config/config.yml
        framework:
            # ...
            templating:
                hinclude_default_template: AcmeDemoBundle::hinclude.html.twig

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:templating hinclude-default-template="AcmeDemoBundle::hinclude.html.twig" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'hinclude_default_template' => array('AcmeDemoBundle::hinclude.html.twig'),
            ),
        ));

.. index::
   single: Templating; Linking to pages

.. _book-templating-pages:

Liens vers des Pages
~~~~~~~~~~~~~~~~~~~~

La création de liens vers d'autres pages de votre projet est l'opération la plus
commune qui soit dans un template. Au lieu de coder en dur les URLs dans les
templates, utilisez la fonction ``path`` de Twig (ou le helper ``router`` en
PHP) pour générer les URLs basées sur la configuration des routes. Plus tard, si
vous désirez modifier l'URL d'une page particulière, tout ce que vous avez
besoin de faire c'est changer la configuration des routes; les templates
génèreront automatiquement la nouvelle URL.

Dans un premier temps, configurons le lien vers la page « _welcome » qui est
accessible via la configuration de route suivante :

.. configuration-block::

    .. code-block:: yaml

        _welcome:
            pattern:  /
            defaults: { _controller: AcmeDemoBundle:Welcome:index }

    .. code-block:: xml

        <route id="_welcome" pattern="/">
            <default key="_controller">AcmeDemoBundle:Welcome:index</default>
        </route>

    .. code-block:: php

        $collection = new RouteCollection();
        $collection->add('_welcome', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Welcome:index',
        )));

        return $collection;

Pour faire un lien vers cette page, utilisons simplement la fonction ``path`` de
Twig en faisant référence à cette route :

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('_welcome') }}">Home</a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('_welcome') ?>">Home</a>

Comme prévu, ceci génèrera l'URL ``/``. Voyons comment cela fonctionne avec des
routes plus compliquées :

.. configuration-block::

    .. code-block:: yaml

        article_show:
            pattern:  /article/{slug}
            defaults: { _controller: AcmeArticleBundle:Article:show }

    .. code-block:: xml

        <route id="article_show" pattern="/article/{slug}">
            <default key="_controller">AcmeArticleBundle:Article:show</default>
        </route>

    .. code-block:: php

        $collection = new RouteCollection();
        $collection->add('article_show', new Route('/article/{slug}', array(
            '_controller' => 'AcmeArticleBundle:Article:show',
        )));

        return $collection;

Dans ce cas, vous devrez spécifier le nom de route (``article_show``) et une 
valeur pour le paramètre ``{slug}``. En utilisant cette route, revoyons le
template ``recentList`` de la section précédente, et
faisons les liens vers les articles correctement :

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}
        {% for article in articles %}
          <a href="{{ path('article_show', { 'slug': article.slug }) }}">
              {{ article.title }}
          </a>
        {% endfor %}

    .. code-block:: php

        <!-- src/Acme/ArticleBundle/Resources/views/Article/recentList.html.php -->
        <?php foreach ($articles as $article): ?>
            <a href="<?php echo $view['router']->generate('article_show', array('slug' => $article->getSlug()) ?>">
                <?php echo $article->getTitle() ?>
            </a>
        <?php endforeach; ?>

.. tip::

    Vous pouvez aussi générer l'URL absolue en utilisant la fonction ``url`` de Twig :

    .. code-block:: html+jinja

        <a href="{{ url('_welcome') }}">Home</a>

    La même chose peut être réalisée dans les templates en PHP en passant un
    troisième argument à la méthode ``generate()`` :

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('_welcome', array(), true) ?>">Home</a>

.. index::
   single: Templating; Linking to assets

.. _book-templating-assets:

Liens vers des Fichiers
~~~~~~~~~~~~~~~~~~~~~~~

Les templates font aussi très souvent référence à des images, du Javascript, des
feuilles de style et d'autres fichiers. Bien sûr vous pouvez coder en dur le chemin
vers ces fichiers (``/images/logo.png`` par exemple), mais Symfony2 fournit une
façon de faire plus souple via la fonction ``asset`` de Twig :

.. configuration-block::

    .. code-block:: html+jinja

        <img src="{{ asset('images/logo.png') }}" alt="Symfony!" />

        <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    .. code-block:: php

        <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" alt="Symfony!" />

        <link href="<?php echo $view['assets']->getUrl('css/blog.css') ?>" rel="stylesheet" type="text/css" />

Le principal objectif de la fonction ``asset`` est de rendre votre application
plus portable. Si votre application se trouve à la racine de votre hôte
(http://example.com par exemple), alors les chemins se retourné sera
``/images/logo.png``. Mais si votre application se trouve dans un sous
répertoire (http://example.com/my_app par exemple), chaque chemin vers les
fichiers sera alors généré avec le sous répertoire (``/my_app/images/logo.png``
par exemple). La fonction ``asset`` fait attention à cela en déterminant comment
votre application est utilisée et en générant les chemins corrects.

De plus, si vous utilisez la fonction ``asset``, Symfony peut automatiquement ajouter
une chaîne de caractères afin de garantir que la ressource statique mise à jour ne
sera pas mise en cache lors de son déploiement. Par exemple, ``/images/logo.png`` 
pourrait ressembler à ``/images/logo.png?v2``. Pour plus d'informations, lisez
la documentation de l'option de configuration :ref:`ref-framework-assets-version`.

.. index::
   single: Templating; Including stylesheets and Javascripts
   single: Stylesheets; Including stylesheets
   single: Javascript; Including Javascripts

L'inclusion de Feuilles de style et de Javascripts avec Twig
------------------------------------------------------------

Aucun site n'est complet sans inclure des fichiers Javascript et des feuilles de
styles. Dans Symfony, l'inclusion de ces fichiers est gérée d'une façon élégante en
conservant les avantages du mécanisme d'héritage de templates de Symfony.

.. tip::

    Cette section vous apprendra la philosophie qui existe derrière l'inclusion
    de feuilles de style et de fichiers Javascript dans Symfony. Symfony contient
    aussi une autre bibliothèque, appelée Assetic, qui suit la même philosophie,
    mais vous permet de faire des choses plus intéressantes avec ces
    fichiers. Pour plus d'informations sur le sujet voir
    :doc:`/cookbook/assetic/asset_management`.


Commençons par ajouter deux blocs à notre template de base qui incluront deux
fichiers : l'un s'appelle ``stylesheet`` et est inclus dans la balise ``head``, et
l'autre s'appelle ``javascript`` et est inclus juste avant que la balise ``body`` ne se
referme. Ces blocs contiendront toutes les feuilles de style et tous les
fichiers javascript dont vous aurez besoin pour votre site :

.. code-block:: html+jinja

    {# 'app/Resources/views/base.html.twig' #}
    <html>
        <head>
            {# ... #}

            {% block stylesheets %}
                <link href="{{ asset('/css/main.css') }}" type="text/css" rel="stylesheet" />
            {% endblock %}
        </head>
        <body>
            {# ... #}

            {% block javascripts %}
                <script src="{{ asset('/js/main.js') }}" type="text/javascript"></script>
            {% endblock %}
        </body>
    </html>

C'est assez simple. Mais comment faire si vous avez besoin d'inclure une feuille
de style supplémentaire ou un autre fichier javascript à partir d'un template
enfant ? Par exemple, supposons que vous avez une page contact et que vous avez
besoin d'inclure une feuille de style ``contact.css`` uniquement sur cette
page. Au sein du template de la page contact, faites comme ce qui suit :

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Contact/contact.html.twig #}
    {% extends '::base.html.twig' %}

    {% block stylesheets %}
        {{ parent() }}
        
        <link href="{{ asset('/css/contact.css') }}" type="text/css" rel="stylesheet" />
    {% endblock %}
    
    {# ... #}

Dans le template enfant, nous surchargeons simplement le bloc ``stylesheets``
en ajoutant une nouvelle balise de feuille de style dans ce bloc. Bien sûr,
puisque nous voulons ajouter du contenu au bloc parent (et non le *remplacer*),
nous devons utiliser la fonction Twig ``parent()`` pour inclure le bloc
``stylesheets`` du template de base.

Vous pouvez aussi inclure des ressources situées dans le dossier ``Resources/public``
de vos bundles. Vous devrez lancer la commande ``php app/console assets:install target [--symlink]`` 
pour placer les fichiers dans le bon répertoire ("web" par défaut)

.. code-block:: html+jinja

   <link href="{{ asset('bundles/acmedemo/css/contact.css') }}" type="text/css" rel="stylesheet" />

Le résultat final est une page qui inclut à la fois la feuille de style
``main.css`` et ``contact.css``.

.. index::
   single: Templating; The templating service

Configuration et Utilisation du Service ``templating``
------------------------------------------------------

Le coeur du système de template dans Symfony2 est le ``moteur`` de template (``Engine``). Cet
objet spécial est responsable de rendre des templates et de retourner leur
contenu. Quand vous effectuez le rendu d'un template à travers un contrôleur
par exemple, vous utilisez en effet le service de moteur de template. Par
exemple::

    return $this->render('AcmeArticleBundle:Article:index.html.twig');

est équivalent à::

    $engine = $this->container->get('templating');
    $content = $engine->render('AcmeArticleBundle:Article:index.html.twig');

    return $response = new Response($content);

.. _template-configuration:

Le moteur de template (ou « service ») est préconfiguré pour fonctionner
automatiquement dans Symfony2. Il peut, bien sûr, être configuré grâce au
fichier de configuration de l'application :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating: { engines: ['twig'] }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:templating>
            <framework:engine id="twig" />
        </framework:templating>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'engines' => array('twig'),
            ),
        ));

Plusieurs options de configuration sont disponibles et sont détaillées dans le
:doc:`Configuration Appendix</reference/configuration/framework>`.

.. note::

   Le moteur ``twig`` est nécessaire pour utiliser un webprofiler (de même que
   beaucoup de bundles tiers).

.. index::
    single: Template; Overriding templates

.. _overriding-bundle-templates:

La Surcharge de templates de Bundle
-----------------------------------

La communauté Symfony2 est fière de créer et de maintenir des bundles de haute
qualité (voir `KnpBundles.com`_) concernant un grand nombre de fonctionnalités.
Une fois que vous utilisez un tel bundle, vous aimeriez sûrement surcharger et
personnaliser un ou plusieurs de ses templates.

Supposons que vous utilisiez un imaginaire ``AcmeBlogBundle`` open source dans
votre projet (dans le répertoire ``src/Acme/blogBundle`` par exemple). Même si vous
êtes très content de ce bundle, vous voudriez probablement surcharger la page « liste » du
blog pour la personnaliser et l'adapter spécialement à votre application. En
cherchant un peu dans le contrôleur ``Blog`` du ``AcmeBlogBundle``, vous trouvez ceci :

.. code-block:: php

    public function indexAction()
    {
        $blogs = // logique qui récupère les blogs

        $this->render('AcmeBlogBundle:Blog:index.html.twig', array('blogs' => $blogs));
    }

Quand le ``AcmeBlogBundle:Blog:index.html.twig`` est rendu, Symfony2 regarde en
fait dans deux emplacements pour le template :

#. ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``

Pour surcharger le template du bundle, il suffit de copier le template
``index.html.twig`` du bundle vers
``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig`` (le répertoire
``app/Resources/AcmeBlogBundle`` n'existe pas, nous vous laissons le soin
de le créer). Vous êtes maintenant à même de personnaliser le template.

.. caution::

    Si vous ajoutez un template à un nouvel endroit, vous *pourriez* avoir
    besoin de vider votre cache (``php app/console cache:clear``), même
    si vous êtes en mode debug.

Cette logique s'applique aussi au template layout. Supposons maintenant que chaque
template dans ``AcmeBlogBundle`` hérite d'un template de base appelé
``AcmeBlogBundle::layout.html.twig``. Comme précédemment, Symfony2 regardera
dans les deux emplacements suivants :

#. ``app/Resources/AcmeBlogBundle/views/layout.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/layout.html.twig``

Encore une fois, pour surcharger le template, il suffit de le copier du bundle
vers ``app/Resources/AcmeBlogBundle/views/layout.html.twig``. Vous êtes
maintenant libre de personnaliser cette copie comme il vous plaira.

Si vous prenez du recul, vous vous apercevrez que Symfony2 commence toujours
par regarder dans le répertoire ``app/Resources/{BUNDLE_NAME}/views/`` pour un
template. Si le template n'existe pas là, il continue sa recherche dans le
répertoire ``Resources/views`` du bundle lui-même. Ce qui signifie que tous les
templates d'un bundle peuvent être surchargés à condition de les placer dans le
bon sous-répertoire de ``app/Resources``.

.. note::
    
    Vous pouvez également surcharger les templates provenant d'un bundle grâce
    à l'héritage de bundle. Pour plus d'informations, voir :doc:`/cookbook/bundles/inheritance`.

.. _templating-overriding-core-templates:

.. index::
    single: Template; Overriding exception templates

La Surcharge des Core Templates
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Puisque le framework Symfony2 lui-même est juste un bundle, les templates du noyau
peuvent être surchargés de la même façon. Par exemple, le bundle noyau
``TwigBundle`` contient un certain nombre de templates relatifs aux
« exceptions » et aux « erreurs » qui peuvent être surchargés en copiant chacun d'eux
du répertoire ``Ressources/views/Exception`` du ``TwigBundle`` vers, vous
l'avez deviné, le répertoire ``app/Resources/TwigBundle/views/Exception``.

.. index::
   single: Templating; Three-level inheritance pattern

Trois niveaux d'héritages
-------------------------

Une façon commune d'utiliser l'héritage est d'utiliser l'approche à trois
niveaux. Cette méthode fonctionne parfaitement avec trois différents types
de templates détaillés :

* Créez un fichier ``app/Resources/views/base.html.twig`` qui contient le
  principal layout pour votre application (comme dans l'exemple précédent). En
  interne, ce template est appelé ``::base.html.twig``;

* Créez un template pour chaque « section » de votre site. Par exemple, un
  ``AcmeBlogBundle`` devrait avoir un template appelé
  ``AcmeBlogBundle::layout.html.twig`` qui contient uniquement les éléments de
  section spécifiques au blog;

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/layout.html.twig #}
        {% extends '::base.html.twig' %}

        {% block body %}
            <h1>Blog Application</h1>

            {% block content %}{% endblock %}
        {% endblock %}

* Créez un template individuel pour chaque page et faites en sorte que chacun
  étende (extend) le template approprié à la section. Par exemple, la page
  « index » sera appelée à peu de chose près
  ``AcmeBlogBundle:Blog:index.html.twig`` et listera les billets du blog.

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
        {% extends 'AcmeBlogBundle::layout.html.twig' %}

        {% block content %}
            {% for entry in blog_entries %}
                <h2>{{ entry.title }}</h2>
                <p>{{ entry.body }}</p>
            {% endfor %}
        {% endblock %}

Remarquons que ce template étend la section (``AcmeBlogBundle::layout.html.twig``)
qui à son tour étend le layout de base de l'application (``::base.html.twig``).
C'est le modèle commun d'héritage à trois niveaux.

Quand vous construisez votre application, vous pouvez choisir de suivre cette
méthode ou simplement faire que chaque template de page étende le layout de
l'application directement (``{% extends '::base.html.twig' %}`` par exemple). Le
modèle des trois templates est une meilleure pratique, utilisée par les bundles
vendor. De ce fait, le layout d'un bundle peut facilement être surchargé pour
étendre proprement le layout de base de votre application.

.. index::
   single: Templating; Output escaping

L'Échappement
-------------

Lors de la génération HTML d'un template, il y a toujours un risque qu'une
variable du template affiche du code HTML non désiré ou du code dangereux côté
client. Le résultat est que le contenu dynamique peut casser le code HTML de la
page résultante ou permettre à un utilisateur malicieux de réaliser une attaque
`Cross Site Scripting`_ (XSS). Considérons cet exemple classique :

.. configuration-block::

    .. code-block:: jinja

        Hello {{ name }}

    .. code-block:: php

        Hello <?php echo $name ?>

Imaginons que l'utilisateur ait rentré le code suivant comme son nom :

.. code-block:: text

    <script>alert('hello!')</script>

Sans échappement du rendu, le template résultant provoquera l'affichage d'une boîte
d'alert Javascript :

.. code-block:: text

    Hello <script>alert('hello!')</script>

Et bien que cela semble inoffensif, si un utilisateur peut aller aussi loin, ce
même utilisateur peut aussi écrire un Javascript qui réalise des actions
malicieuses dans un espace sécurisé d'un inconnu et légitime utilisateur.

La réponse à ce problème est l'échappement (output escaping). En activant l'échappement,
le même template sera rendu de façon inoffensive, et affichera
littéralement la balise ``script`` à l'écran :

.. code-block:: text

    Hello &lt;script&gt;alert(&#39;helloe&#39;)&lt;/script&gt;

Les systèmes de template Twig et PHP abordent le problème différemment. Si
vous utilisez Twig, l'échappement est activé par défaut et vous êtes protégé. En
PHP, l'échappement n'est pas automatique, ce qui signifie que vous aurez besoin
d'échapper manuellement là où c'est nécessaire.

L'échappement avec Twig
~~~~~~~~~~~~~~~~~~~~~~~

Si vous utilisez les templates Twig, alors l'échappement est activé par
défaut. Ce qui signifie que vous êtes protégé immédiatement des conséquences
non intentionnelles du code soumis par l'utilisateur. Par défaut, l'échappement
suppose que le contenu est bien échappé pour un affichage HTML.

Dans certains cas, vous aurez besoin de désactiver l'échappement de la sortie
lors du rendu d'une variable qui est sure et qui contient des décorations qui ne
doivent pas être échappées. Supposons que des utilisateurs administrateurs sont
capables décrire des articles qui contiennent du code HTML. Par défaut, Twig
échappera le corps de l'article. Pour le rendre normalement, il suffit
d'ajouter le filtre ``raw`` : ``{{ article.body | raw }}``.

Vous pouvez aussi désactiver l'échappement au sein d'un ``{% block %}`` ou pour
un template entier. Pour plus d'informations, voir `Output Escaping`_ dans la
documentation de Twig.

L'échappement en PHP
~~~~~~~~~~~~~~~~~~~~

L'échappement n'est pas automatique lorsque vous utilisez des templates PHP. Ce
qui signifie que, à moins que vous ne choisissiez explicitement d'échapper une
variable, vous n'êtes pas protégé. Pour utiliser l'échappement, utilisez la
méthode spéciale ``escape()`` de view : :

.. code-block:: php

    Hello <?php echo $view->escape($name) ?>

Par défaut, la méthode ``escape()`` suppose que la variable est rendue dans un
contexte HTML (et donc que la variable est échappée pour être sans danger pour
l'HTML). Le second argument vous permet de changer de contexte. Par exemple,
pour afficher quelque chose dans une chaîne de caractères JavaScript, utilisez le
context ``js`` :

.. code-block:: js

    var myMsg = 'Hello <?php echo $view->escape($name, 'js') ?>';

.. index::
   single: Templating; Formats

.. _template-formats:

Debugguer
---------

.. versionadded:: 2.0.9
    Cette fonctionnalité est disponible depuis Twig ``1.5.x``, qui a été fourni pour
    la première fois avec Symfony 2.0.9.

Lorsque vous utilisez PHP, vous pouvez utiliser ``var_dump()`` si vous avez
besoin de trouver rapidement la valeur d'une variable. C'est utile, par exemple,
dans un contrôleur. La même chose peut être faite lorsque vous utilisez Twig en
utilisant l'extension de debug. Elle a besoin d'être activée dans la configuration :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            acme_hello.twig.extension.debug:
                class:        Twig_Extension_Debug
                tags:
                     - { name: 'twig.extension' }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
            <service id="acme_hello.twig.extension.debug" class="Twig_Extension_Debug">
                <tag name="twig.extension" />
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('Twig_Extension_Debug');
        $definition->addTag('twig.extension');
        $container->setDefinition('acme_hello.twig.extension.debug', $definition);

Les paramètres de template peuvent être affichés en utilisant la fonction ``dump`` :

.. code-block:: html+jinja

    {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}
    {{ dump(articles) }}

    {% for article in articles %}
        <a href="/article/{{ article.slug }}">
            {{ article.title }}
        </a>
    {% endfor %}

Les variables ne seront affichées que si l'option ``debug`` de Twig (dans ``config.yml``)
est à ``true``. Cela signifie que par défaut, les variables seront affichées en environnement
de ``dev`` mais pas en environnement de ``prod``.

Vérification de la syntaxe
--------------------------

.. versionadded:: 2.1
    La commande ``twig:lint`` a été ajoutée dans Symfony 2.1

Vous pouvez vérifier les éventuelles erreurs de syntaxe dans les templates
Twig en utilisant la commande ``twig:lint`` :

.. code-block:: bash

    # Vous pouvez vérifier par nom de fichier :
    $ php app/console twig:lint src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig

    # ou par répertoire :
    $ php app/console twig:lint src/Acme/ArticleBundle/Resources/views

    # ou en utilisant le nom du bundle :
    $ php app/console twig:lint @AcmeArticleBundle

Les Formats de Template
-----------------------

Les templates sont une façon générique de rendre un contenu dans *n'importe
quel* format. Et bien que dans la plupart des cas vous utiliserez les templates
pour rendre du contenu HTML, un template peut tout aussi facilement générer du
JavaScript, du CSS, du XML ou tout autre format dont vous pouvez rêver.

Par exemple, la même « ressource » est souvent rendue dans plusieurs formats
différents. Pour rendre la page index d'un article en XML, incluez simplement le
format dans le nom du template :

* *nom du template XML*: ``AcmeArticleBundle:Article:index.xml.twig``
* *nom de fichier du template XML*: ``index.xml.twig``

En réalité, ce n'est rien de plus qu'une convention de nommage et le template
n'est pas rendu différemment en se basant sur ce format.

Dans beaucoup de cas, vous pourriez vouloir autoriser un simple contrôleur à
rendre plusieurs formats en se basant sur le « request format ». Pour cette
raison, un pattern commun est de procéder comme cela::

    public function indexAction()
    {
        $format = $this->getRequest()->getRequestFormat();
    
        return $this->render('AcmeBlogBundle:Blog:index.'.$format.'.twig');
    }

Le ``getRequestFormat`` sur l'objet ``Request`` retourne par défaut ``html``,
mais peut aussi retourner n'importe quel autre format basé sur le format demandé
par l'utilisateur. Le format demandé est le plus souvent géré par le système de
route, où une route peut être configurée telle que ``/contact`` définit le format
demandé à ``html`` alors que ``/contact.xml`` définit le format à ``xml``. Pour
plus d'informations, voir l':ref:`Exemple avancé du chapitre routing
<advanced-routing-example>`.

Pour créer des liens qui incluent le paramètre de format, incluez une clé
``_format`` dans le tableau de paramètres :

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('article_show', {'id': 123, '_format': 'pdf'}) }}">
	    PDF Version
	</a>

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate('article_show', array('id' => 123, '_format' => 'pdf')) ?>">
            PDF Version
        </a>

Réflexions Finales
------------------

Le moteur de template dans Symfony est un outil puissant qui peut être utilisé
chaque fois que vous avez besoin de générer du contenu de représentation en
HTML, XML ou tout autre format. Et bien que les templates soient un moyen commun
de générer du contenu dans un contrôleur, leur utilisation n'est pas
systématique. L'objet ``Response`` retourné par un contrôleur peut être créé
avec ou sans utilisation de template::

    // création d'un objet Response qui contient le rendu d'un template
    $response = $this->render('AcmeArticleBundle:Article:index.html.twig');

    // création d'un objet Response qui contient un texte simple
    $response = new Response('response content');

Le moteur de templates de Symfony est très flexible et deux outils de restitution
sont disponibles par défaut : les traditionnels templates *PHP* et les élégants
et puissants templates *Twig*. Ils supportent tous les deux une hiérarchie des
template et sont fournis avec un ensemble riche de fonctions capables de
réaliser la plupart des tâches.

Dans l'ensemble, le système de templates doit être pensé comme étant un outil
puissant qui est à votre disposition. Dans certains cas, vous n'aurez pas besoin
de rendre un template, et dans Symfony2, c'est tout à fait envisageable.

En savoir plus grâce au Cookbook
--------------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/templating/twig_extension`

.. _`Twig`: http://twig.sensiolabs.org
.. _`KnpBundles.com`: http://knpbundles.com
.. _`Cross Site Scripting`: http://en.wikipedia.org/wiki/Cross-site_scripting
.. _`Output Escaping`: http://twig.sensiolabs.org/doc/api.html#escaper-extension
.. _`tags`: http://twig.sensiolabs.org/doc/tags/index.html
.. _`filtres`: http://twig.sensiolabs.org/doc/filters/index.html
.. _`ajouter vos propres extensions`: http://twig.sensiolabs.org/doc/advanced.html#creating-an-extension
.. _`hinclude.js`: http://mnot.github.com/hinclude/
