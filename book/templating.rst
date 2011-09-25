.. index::
   single: Templating

Créer et utiliser les templates
===============================

Comme vous le savez, le :doc:`contrôleur </book/controller>` est responsable de
la gestion de toute les requêtes d'une application Symfony2. En réalité, le contrôleur
délègue le plus gros du travail à d'autres classes afin que le code puisse être
testé et réutilisé. Quand un contrôleur a besoin de générer du HTML, CSS ou tout
autre contenu,  il donne la main au moteur de template.
Dans ce chapitre, vous apprendrez comment écrire des templates puissants qui peuvent
être utilisés pour retourner du contenu à l'utilisateur, remplir le corps d'emails
et bien d'autres. Vous apprendrez des raccourcis, des méthodes ingénieuses pour
étendre les templates et surtout comment les réutiliser.

.. index::
   single: Templating; What is a template?

Templates
---------

Un template est un simple fichier texte qui peut génerer n'importe quel format
basé sur du texte (HTML, XML, CSV, LaTeX ...). Le type de template le plus connu
est le template *PHP* - un fichier texte interpreté par PHP qui contient du texte
et du code PHP::

    <!DOCTYPE html>
    <html>
        <head>
            <title>Welcome to Symfony!</title>
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
            <title>Welcome to Symfony!</title>
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
    service et de le tagger avec ``twig.extension`` :ref:`tag<book-service-container-tags>`.

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

.. sidebar:: Pourquoi Twig?

    Les templates Twig sont conçus pour être simple et ne traiteront
    aucun code PHP. De par sa conception, le système de template Twig
    s'occupe de la présentation, pas de la logique. Plus vous
    utiliserez Twig, plus vous apprécierez cette distinction et en
    bénéficierez. Et bien sûr, vous serez adoré par tous les web
    designers.

    Twig peut aussi faire des choses que PHP ne pourrait pas faire, comme du vrai
    héritage de templates (Twig compile les templates en classes PHP qui héritent
    les unes des autres), le contrôle d'espace, le bac à sable et l'inclusion de
    fonctions et de filtres personnalisés qui n'affectent que les templates. Twig
    contient de petites fonctionnalités qui rendent l'écriture de template plus
    facile et plus concise. Prenez l'exemple suivant, il combine une boucle avec
    l'instruction logique ``if`` :
    
    .. code-block:: html+jinja
    
        <ul>
            {% for user in users %}
                <li>{{ user.username }}</li>
            {% else %}
                <li>No users found</li>
            {% endfor %}
        </ul>

.. index::
   pair: Twig; Cache

Twig et la mise en cache
~~~~~~~~~~~~~~~~~~~~~~~~

Twig est rapide. Chaque template Twig est compilé en une classe PHP natif qui est
rendue à l'éxécution. Les classes compilées sont stockées dans le répertoire
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
regénéré. Souvenez vous bien de cela lorsque vous déploirez votre application.

.. index::
   single: Templating; Inheritance

L'héritage de template et layouts
---------------------------------

Bien souvent, les templates d'un projet partagent des éléments communs, comme les
entête, pied de page et menu latéraux. Dans Symfony2, nous abordons ce problème
différemment : un template peut être décoré par un autre. Cela fonctionne exactement
comme les classes PHP : l'héritage de template vous permet de batir un template
« layout » de base qui contient tous les éléments communs de votre site et de définir
des **blocks** (comprenez « classe PHP avec des méthodes de base »). Un template
enfant peut étendre le template layout et surcharger n'importe lequel de ses blocks
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
                    <?php if ($view['slots']->has('sidebar'): ?>
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
de deux colonnes. Dans cette exemplen, trois espaces ``{% block %}`` sont défnis
(``title``, ``sidebar`` et ``body``). Chacun de ces bloques peut être soit
surcharger dans un template enfant ou soit conserver leur code d'origine. Ce
template peut aussi être rendu directement. Dans ce cas, les bloques ``title``,
``sidebar`` et ``body`` conserveront simplement les valeurs par défauts
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

   Le template parent est identifié grâce à une chaine de caractères
   particulière (``::base.html.twig``) qui indique que ce template se trouve
   dans le dossier ``app/Resources/views`` du projet. Cette convention de
   nommage est complètement expliqués dans :ref:`template-naming-locations`.

La clé de l'héritage de template est la balise ``{% extends %}``. Elle indique
au moteur de template d'évaluer d'abord le template de base, qui configure le
layout et définit plusieurs bloques. Le template enfant est ensuite
rendu. Durant ce traitement les bloques parents ``title`` et ``body`` sont
remplacés par ceux de l'enfant. Dépendant de la valeur de ``blog_entries``, la
sortie peut ressembler à ceci : ::

    <!DOCTYPE html>
    <html>
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <title>My cool blog posts</title>
        </head>
        <body>
            <div id="sidebar">
                <ul>
                    <li><a href="/">Home</a></li>
                    <li><a href="/blog">Blog</a></li>
                </ul>
            </div>

            <div id="content">
                <h2>My first post</h2>
                <p>The body of the first post.</p>

                <h2>Another post</h2>
                <p>The body of the second post.</p>
            </div>
        </body>
    </html>

Remarquons que comme le template enfant n'a pas définit le block ``sidebar``, la
valeur du template parent est utilisé à la place. Le contenu d'une balise 
``{% block %}`` d'un template parent est toujours utilisé par défaut.

Vous pouvez utiliser autant de niveau d'héritage que vous souhaitez. Dans la
section suivante, un modèle commun d'héritage à trois niveaux sera expliqué,
ainsi que l'organisation des templates au sein d'un projet Symfony2.

Quand on travaille avec l'héritage de template, il est important de garder ces
astuces à l'esprit :

* Si vous utilisez ``{% extends %}`` dans un template, alors ce doit être la
  première balise de ce template.

* Plus vous utilisez les balises ``{% block %}`` dans les templates, le mieux
  c'est. Souvenez-vous, les templates enfants ne doivent pas obligatoirement
  définir tous les bloques parents, donc créez autant de bloques que vous
  désirez dans le template de base et attribuez leurs une configuration par
  défaut. Plus vous avez de bloques dans le template de base, plus le layout
  sera flexible.

* Si vous vous retrouvez à dupliquer du contenu dans plusieurs template, cela
  veut probablement dire que vous devriez déplacer ce contenu dans un 
  ``{% block  %}`` d'un template parent. Dans certain cas, la meilleur solution 
  peut être de déplacer le contenu dans un nouveau template et de l'``include`` 
  (voir :ref:`including-templates`).

* Si vous avez besoin de récupérer le contenu d'un bloque d'un template parent,
  vous pouvez utiliser la fonction ``{{ parent() }}``. C'est utile si on
  souhaite compléter le contenu du bloque parent au lieu de le réécrire
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
-----------------------------

Par défaut, les templates peuvent se trouver dans deux emplacements
différents :

* ``app/Resources/views/`` : Le dossier des ``views`` de l'application peut
  aussi bien contenir le template de base de l'application (i.e. le layout de
  l'application) ou les templates qui surchargent les templates des bundles
  (voir :ref:`overriding-bundle-templates`);

* ``path/to/bundle/Resources/views/`` : Chaque bundle place leurs
  templates dans leur dossier ``Resources/views`` (et sous dossiers). La
  plupart des templates résident au sein d'un bundle.

Symfony2 utilise une chaine de caractère au format
**bundle**:**controller**:**template** pour les templates.

* ``AcmeBlogBundle:Blog:index.html.twig``: Cette syntaxe est utilisée pour
  spécifier un template pour une page donnée. Les trois parties de la chaine de
  caractère, séparée par deux-points (``:``), signifie ceci :

    * ``AcmeBlogBundle``: (*bundle*) le template se trouve dans le 
      ``AcmeBlogBundle`` (``src/Acme/BlogBundle`` par exemple);

    * ``Blog``: (*controller*) indique que le template se trouve dans le
      sous-répertoire ``Blog`` de ``Resources/views``;

    * ``index.html.twig``: (*template*) le nom réel du fichier est 
      ``index.html.twig``.

  En supposant que le ``AcmeBlogBundle`` se trouve à ``src/Acme/BlogBundle``, le
  chemin final du layout serait ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``.

* ``AcmeBlogBundle::layout.html.twig``: Cette syntaxe fait référence à un
  template de base qui est spécifique au ``AcmeBlogBundle``. Donc la partie du
  milieu, "controller", est absente (``Blog`` par exemple), le template se
  trouve à ``Resources/views/layout.html.twig`` dans ``AcmeBlogBundle``.

* ``::base.html.twig``: Cette syntaxe fait référence à un template de base d'une
  application ou layout. Remarquez que la chaine de caractère commence par deux
  deux-points (``::``), ce qui signifie que les deux parties *bundle* et
  *controller* sont absente. Ce qui signifie que le template ne se trouve dans
  aucun bundle, mais directement dans le répertoire racine
  ``app/Resources/views/``.

Dans la section :ref:`overriding-bundle-templates`, vous verrez comment les
templates intéragissent avec ``AcmeBlogBundle``. Par exemple, il est possible de
surcharger un template en plaçant un template du même nom dans le répertoire
``app/Resources/AcmeBlogBundle/views/``. Cela offre la possibilité de surcharger
les templates fournis par n'importe quel vendor bundle.

.. tip::

    On espère que la syntaxe de nommage des templates vous parait familière -
    c'est la même convention de nommage qui est utiliser pour faire référence à
    :ref:`controller-string-syntax`.

Le Template Suffixe
~~~~~~~~~~~~~~~

Le format **bundle**:**controller**:**template** de chaque template spécifie
*où* se situe le fichier template. Chaque nom de template a aussi deux extension
qui spécifie le *format* et le *moteur* (engine) pour le template.

* **AcmeBlogBundle:Blog:index.html.twig** - HTML format, Twig engine

* **AcmeBlogBundle:Blog:index.html.php** - HTML format, PHP engine

* **AcmeBlogBundle:Blog:index.css.twig** - CSS format, Twig engine

Par défaut, tout template de Symfony2 peut être écrit soit en Twig ou en PHP, et
la dernière partie de l'extension (``.twig`` ou ``.php`` par exemple) spécifie
lequel de ces deux *moteurs* sera utilisé. La première partie de l'extension
(``.html``, ``.css`` par exemple) désigne le format final du template qui sera
généré. Contrairement au moteur, qui détermine comment Symfony2 parsera le
template, c'est simplement une tactique organisationnel qui est utilisé dans le
cas où la même ressource a besoin d'être rendue en HTML (``index.html.twig``),
en XML (``index.xml.twig``), ou tout autre format. Pour plus d'information,
lisez la section :ref:`template-formats`.

.. note::

   Les *moteurs* disponibles peuvent être configurés et d'autre moteur peuvent
   être ajouté. Voir :ref:`Templating Configuration<template-configuration>`
   pour plus de détails.

.. index::
   single: Templating; Tags and Helpers
   single: Templating; Helpers

Tags et Helpers
----------------

Vous avez maintenant compris les bases des templates, comment ils sont nommés et
comment utiliser l'héritage de template. Les parties les plus difficiles sont
dores et déjà derrière vous. Dans cette section, vous apprendrez à utiliser un
ensemble d'outil disponible pour aider à réaliser les tâche les plus communes
avec les templates comme l'inclusion de templates, faire des liens entre des
pages et l'inclusion d'image.

Symfony2 regroupe plusieurs paquets dont plusieurs spécialisés dans les balises
et fonctions Twig qui facilite le travaille du template designer. En PHP, le
système de template fournit un système de *helper* extensible. Ce système fournit des
propriétés utiles dans le contexte des templates.

Nous avons déjà vu quelques balises Twig (``{% block %}`` & ``{% extends %}``)
ainsi qu'un exemple de helper PHP (``$view['slots']``). Apprenons en un peu
plus.

.. index::
   single: Templating; Including other templates

.. _including-templates:

L'inclusion de templates
~~~~~~~~~~~~~~~~~~~~~~~~~

Vous voudrez souvent inclure le même template ou fragment de code dans
différentes pages. Par exemple, dans une application avec un espace "nouveaux
articles", le code du template affichant un article peut être utiliser sur la
page détaillant l'article, sur une page affichant les articles les plus
populaires, ou dans une liste des derniers articles.

Quand vous avez besoin de réutiliser une grand partie d'un code PHP, typiquement
vous déplacez le code dans une nouvelle classe PHP ou dans une fonction. La même
choses s'applique aussi aux templates. En déplaçant le code réutilisé dans son
propre template, il peut être inclu par tous les autres templates. D'abord,
créez le template que vous souhaiterez réutiliser.

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/articleDetails.html.twig #}
        <h1>{{ article.title }}</h1>
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

L'inclusion de ce template dans tout  autre template est simple :

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/Article/list.html.twig #}
        {% extends 'AcmeArticleBundle::layout.html.twig' %}

        {% block body %}
            <h1>Recent Articles<h1>

            {% for article in articles %}
                {% include 'AcmeArticleBundle:Article:articleDetails.html.twig' with {'article': article} %}
            {% endfor %}
        {% endblock %}

    .. code-block:: php

        <!-- src/Acme/ArticleBundle/Resources/Article/list.html.php -->
        <?php $view->extend('AcmeArticleBundle::layout.html.php') ?>

        <?php $view['slots']->start('body') ?>
            <h1>Recent Articles</h1>

            <?php foreach ($articles as $article): ?>
                <?php echo $view->render('AcmeArticleBundle:Article:articleDetails.html.php', array('article' => $article)) ?>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

Le template est inclu via l'utilisation de la balise ``{% include %}``. 
Remarquons que le nom du template suit la même convention habituelle. Le
template ``articleDetails.html.twig`` utilise une variable ``article``. Elle est
passée au template ``list.html.twig`` en utilisant la commande ``with``.

.. tip::

    La syntaxe ``{'article': article}`` est la syntaxe standard de Twig pour les
    tables de hachage (hash maps) (i.e. un tableau indicé par des chaines de
    caractères). Si nous souhaitons passer plusieurs elements, cela ressemblera
    à ceci : ``{'foo': foo, 'bar': bar}``.

.. index::
   single: Templating; Embedding action

.. _templating-embedding-controller:

Contrôleurs Embarqués
~~~~~~~~~~~~~~~~~~~~

Dans quelques cas, vous aurez besoin d'inclure plus qu'un simple
template. Supposons que vous avez un menu latéral dans votre layout qui contient
les trois articles les plus récents. La récupération des trois articles les plus
récents peut nécessité l'inclusion d'une requête vers une base de données et de
réaliser d'autres oprérations logiques qui ne peuvent pas être effectuées à partir
d'un template.

La solution consiste simplement à imbriquer les résultats d'un contrôleur à partir
du template. Dans un premier temps, créez un contrôleur qui retourne un certain
nombre d'articles récents :

.. code-block:: php

    // src/Acme/ArticleBundle/Controller/ArticleController.php

    class ArticleController extends Controller
    {
        public function recentArticlesAction($max = 3)
        {
            // make a database call or other logic to get the "$max" most recent articles
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
        <?php foreach ($articles in $article): ?>
            <a href="/article/<?php echo $article->getSlug() ?>">
                <?php echo $article->getTitle() ?>
            </a>
        <?php endforeach; ?>

.. note::

    Remarquons que nous avons triché et que nous avons codé en dur les URL des
    articles dans cet exemple (``/article/*slug*`` par exemple). Ce n'est pas
    une bonne pratique. Dans la section suivante, vous apprendrez comment le
    faire correctement.

Pour inclure le contrôleur, vous avez besoin de faire référence à ce-dernier en
utilisant la chaine de caractère standard pour les contrôleurs
(i.e. **bundle**:**controller**:**action**) :


.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        ...

        <div id="sidebar">
            {% render "AcmeArticleBundle:Article:recentArticles" with {'max': 3} %}
        </div>

    .. code-block:: php

        <!-- app/Resources/views/base.html.php -->
        ...

        <div id="sidebar">
            <?php echo $view['actions']->render('AcmeArticleBundle:Article:recentArticles', array('max' => 3)) ?>
        </div>

Au cas où vous pensez avoir besoin d'une variable ou de quzlquzs informations
auxquelles vous n'avez pas accès à partir d'un template, penser à rendre un
contrôleur. Les contrôleurs sont rapides à l'exécution et promouvoient une bonne
organisation et réutilisabilité du code.

.. index::
   single: Templating; Linking to pages

Le Lien vers des Pages
~~~~~~~~~~~~~~~~

La création de lien vers d'autres pages de votre projet est l'opération la plus
commune qui soit dans un template. Au lieu de coder en dure les URLs dans les
templates, utilisez la fonction ``path`` de Twig (ou le helper ``router`` en
PHP) pour genererles URLs basées sur la configuration des routes. Plus tard, si
vous désirez modifier l'URL d'une page particulière, tout ce que vous avez
besoin de faire c'est changer la configuration des routes; les templates
genereront automatiquement la nouvelle URL.

Dans un premier temps, configurons le lien vers la page "_welcome" qui est
accessible via la configuration de route suivant :

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

Comme prévu, ceci générera l'URL ``/``. Voyons comment cela fonctionne avec des
routes plus comliquées :

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

Dans ce cas, dans ce cas vous devrez spécifiez le nom de route
(``article_show``) et une valeur pour le paramètre ``{slug}``. En utilisant
cette route, revoyons la ``recentList`` de template de la section précédente, et
faisons le lien vers les articles correctement :

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
        <?php foreach ($articles in $article): ?>
            <a href="<?php echo $view['router']->generate('article_show', array('slug' => $article->getSlug()) ?>">
                <?php echo $article->getTitle() ?>
            </a>
        <?php endforeach; ?>

.. tip::

    Vous pouvez aussi générer l'URL absolue en utilisant la fonction ``url`` de Twig :

    .. code-block:: html+jinja

        <a href="{{ url('_welcome') }}">Home</a>

    La même chose peut être réaliser dans les templates en PHP en passant un
    troisième argument à la méthode ``generate()`` :

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('_welcome', array(), true) ?>">Home</a>

.. index::
   single: Templating; Linking to assets

Le Lien vers des Fichiers
~~~~~~~~~~~~~~~~~

Les templates font aussi très souvent référence à des images, du Javascript, des
feuilles de style et d'autres fichiers. Bien sûr vous pouvez coder en dur le chemin
vers ces fichiers (``/images/logo.png`` par exemple), mais Symfony2 fournit une
façon de faire plus souple via la fonction ``assets`` de Twig :

.. configuration-block::

    .. code-block:: html+jinja

        <img src="{{ asset('images/logo.png') }}" alt="Symfony!" />

        <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    .. code-block:: php

        <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" alt="Symfony!" />

        <link href="<?php echo $view['assets']->getUrl('css/blog.css') ?>" rel="stylesheet" type="text/css" />

Le principal objectif de la fonction ``asset`` est de rendre votre application
plus portable. Si votre application se trouve à la racince de votre hôte
(http://example.com par exemple), alors les chemins se référant à une icône sera
``/images/logo.png``. Mais si votre application se trouve dans un sous
répertoire (http://example.com/my_app par exemple), chaque chemin vers les
fichiers sera alors généré avec le sous répertoire (``/my_app/images/logo.png``
par exemple). La fonction ``asset`` fait attention à cela en déterminant comment
votre application est utilisé et en générant les chemins correctes.

.. index::
   single: Templating; Including stylesheets and Javascripts
   single: Stylesheets; Including stylesheets
   single: Javascripts; Including Javascripts

L'inclusion de Feuilles de style et de Javascripts avec Twig
---------------------------------------------

Aucun site n'est complet sans inclure des fichiers Javascript et des feuilles de
styles. Dans Symfony, l'inclusion de ces fichiers est gérée d'une façon élégante en
conservant les avantages du méchanisme d'héritage de template de Symfony.

.. tip::

    Cette section vous apprendra la philosophie qui existe derrière l'inclusion
    de feuille de style et de fichiers Javascript dans Symfony. Symfony contient
    aussi une autre bibliothèque, appelée assetic, qui suit la même philosophie
    mais vous permet de faire des choses plus intéressantes avec ces
    fichiers. Pour plus d'information sur le sujet voir
    :doc:`/cookbook/assetic/asset_management`.


Commencons par ajouté deux bloques à notre template de base qui inclura deux
fichiers : l'un s'appelle ``stylesheet``, inclut dans la balise ``head``, et
l'autre s'appelle ``javascript``, inclut juste avant que la base ``body`` se
referme. Ces bloques contiendront toutes les feuilles de style et tous les
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

That's easy enough! But what if you need to include an extra stylesheet or
Javascript from a child template? For example, suppose you have a contact
page and you need to include a ``contact.css`` stylesheet *just* on that
page. From inside that contact page's template, do the following:

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Contact/contact.html.twig #}
    {# extends '::base.html.twig' #}

    {% block stylesheets %}
        {{ parent() }}
        
        <link href="{{ asset('/css/contact.css') }}" type="text/css" rel="stylesheet" />
    {% endblock %}
    
    {# ... #}

In the child template, you simply override the ``stylesheets`` block and 
put your new stylesheet tag inside of that block. Of course, since you want
to add to the parent block's content (and not actually *replace* it), you
should use the ``parent()`` Twig function to include everything from the ``stylesheets``
block of the base template.

The end result is a page that includes both the ``main.css`` and ``contact.css``
stylesheets.

.. index::
   single: Templating; The templating service

Configuring and using the ``templating`` Service
------------------------------------------------

The heart of the template system in Symfony2 is the templating ``Engine``.
This special object is responsible for rendering templates and returning
their content. When you render a template in a controller, for example,
you're actually using the templating engine service. For example:

.. code-block:: php

    return $this->render('AcmeArticleBundle:Article:index.html.twig');

is equivalent to

.. code-block:: php

    $engine = $this->container->get('templating');
    $content = $engine->render('AcmeArticleBundle:Article:index.html.twig');

    return $response = new Response($content);

.. _template-configuration:

The templating engine (or "service") is preconfigured to work automatically
inside Symfony2. It can, of course, be configured further in the application
configuration file:

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

Several configuration options are available and are covered in the
:doc:`Configuration Appendix</reference/configuration/framework>`.

.. note::

   The ``twig`` engine is mandatory to use the webprofiler (as well as many
   third-party bundles).

.. index::
    single; Template; Overriding templates

.. _overriding-bundle-templates:

Overriding Bundle Templates
---------------------------

The Symfony2 community prides itself on creating and maintaining high quality
bundles (see `Symfony2Bundles.org`_) for a large number of different features.
Once you use a third-party bundle, you'll likely need to override and customize
one or more of its templates.

Suppose you've included the imaginary open-source ``AcmeBlogBundle`` in your
project (e.g. in the ``src/Acme/BlogBundle`` directory). And while you're
really happy with everything, you want to override the blog "list" page to
customize the markup specifically for your application. By digging into the
``Blog`` controller of the ``AcmeBlogBundle``, you find the following::

    public function indexAction()
    {
        $blogs = // some logic to retrieve the blogs

        $this->render('AcmeBlogBundle:Blog:index.html.twig', array('blogs' => $blogs));
    }

When the ``AcmeBlogBundle:Blog:index.html.twig`` is rendered, Symfony2 actually
looks in two different locations for the template:

#. ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``

To override the bundle template, just copy the ``index.html.twig`` template
from the bundle to ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
(the ``app/Resources/AcmeBlogBundle`` directory won't exist, so you'll need
to create it). You're now free to customize the template.

This logic also applies to base bundle templates. Suppose also that each
template in ``AcmeBlogBundle`` inherits from a base template called
``AcmeBlogBundle::layout.html.twig``. Just as before, Symfony2 will look in
the following two places for the template:

#. ``app/Resources/AcmeBlogBundle/views/layout.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/layout.html.twig``

Once again, to override the template, just copy it from the bundle to
``app/Resources/AcmeBlogBundle/views/layout.html.twig``. You're now free to
customize this copy as you see fit.

If you take a step back, you'll see that Symfony2 always starts by looking in
the ``app/Resources/{BUNDLE_NAME}/views/`` directory for a template. If the
template doesn't exist there, it continues by checking inside the
``Resources/views`` directory of the bundle itself. This means that all bundle
templates can be overridden by placing them in the correct ``app/Resources``
subdirectory.

.. _templating-overriding-core-templates:

.. index::
    single; Template; Overriding exception templates

Overriding Core Templates
~~~~~~~~~~~~~~~~~~~~~~~~~

Since the Symfony2 framework itself is just a bundle, core templates can be
overridden in the same way. For example, the core ``FrameworkBundle`` contains
a number of different "exception" and "error" templates that can be overridden
by copying each from the ``Resources/views/Exception`` directory of the
``FrameworkBundle`` to, you guessed it, the
``app/Resources/FrameworkBundle/views/Exception`` directory.

.. index::
   single: Templating; Three-level inheritance pattern

Three-level Inheritance
-----------------------

One common way to use inheritance is to use a three-level approach. This
method works perfectly with the three different types of templates we've just
covered:

* Create a ``app/Resources/views/base.html.twig`` file that contains the main
  layout for your application (like in the previous example). Internally, this
  template is called ``::base.html.twig``;

* Create a template for each "section" of your site. For example, an ``AcmeBlogBundle``,
  would have a template called ``AcmeBlogBundle::layout.html.twig`` that contains
  only blog section-specific elements;

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/layout.html.twig #}
        {% extends '::base.html.twig' %}

        {% block body %}
            <h1>Blog Application</h1>

            {% block content %}{% endblock %}
        {% endblock %}

* Create individual templates for each page and make each extend the appropriate
  section template. For example, the "index" page would be called something
  close to ``AcmeBlogBundle:Blog:index.html.twig`` and list the actual blog posts.

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
        {% extends 'AcmeBlogBundle::layout.html.twig' %}

        {% block content %}
            {% for entry in blog_entries %}
                <h2>{{ entry.title }}</h2>
                <p>{{ entry.body }}</p>
            {% endfor %}
        {% endblock %}

Notice that this template extends the section template -(``AcmeBlogBundle::layout.html.twig``)
which in-turn extends the base application layout (``::base.html.twig``).
This is the common three-level inheritance model.

When building your application, you may choose to follow this method or simply
make each page template extend the base application template directly
(e.g. ``{% extends '::base.html.twig' %}``). The three-template model is
a best-practice method used by vendor bundles so that the base template for
a bundle can be easily overridden to properly extend your application's base
layout.

.. index::
   single: Templating; Output escaping

Output Escaping
---------------

When generating HTML from a template, there is always a risk that a template
variable may output unintended HTML or dangerous client-side code. The result
is that dynamic content could break the HTML of the resulting page or allow
a malicious user to perform a `Cross Site Scripting`_ (XSS) attack. Consider
this classic example:

.. configuration-block::

    .. code-block:: jinja

        Hello {{ name }}

    .. code-block:: php

        Hello <?php echo $name ?>

Imagine that the user enters the following code as his/her name::

    <script>alert('hello!')</script>

Without any output escaping, the resulting template will cause a JavaScript
alert box to pop up::

    Hello <script>alert('hello!')</script>

And while this seems harmless, if a user can get this far, that same user
should also be able to write JavaScript that performs malicious actions
inside the secure area of an unknowing, legitimate user.

The answer to the problem is output escaping. With output escaping on, the
same template will render harmlessly, and literally print the ``script``
tag to the screen::

    Hello &lt;script&gt;alert(&#39;helloe&#39;)&lt;/script&gt;

The Twig and PHP templating systems approach the problem in different ways.
If you're using Twig, output escaping is on by default and you're protected.
In PHP, output escaping is not automatic, meaning you'll need to manually
escape where necessary.

Output Escaping in Twig
~~~~~~~~~~~~~~~~~~~~~~~

If you're using Twig templates, then output escaping is on by default. This
means that you're protected out-of-the-box from the unintentional consequences
of user-submitted code. By default, the output escaping assumes that content
is being escaped for HTML output.

In some cases, you'll need to disable output escaping when you're rendering
a variable that is trusted and contains markup that should not be escaped.
Suppose that administrative users are able to write articles that contain
HTML code. By default, Twig will escape the article body. To render it normally,
add the ``raw`` filter: ``{{ article.body | raw }}``.

You can also disable output escaping inside a ``{% block %}`` area or
for an entire template. For more information, see `Output Escaping`_ in
the Twig documentation.

Output Escaping in PHP
~~~~~~~~~~~~~~~~~~~~~~

Output escaping is not automatic when using PHP templates. This means that
unless you explicitly choose to escape a variable, you're not protected. To
use output escaping, use the special ``escape()`` view method::

    Hello <?php echo $view->escape($name) ?>

By default, the ``escape()`` method assumes that the variable is being rendered
within an HTML context (and thus the variable is escaped to be safe for HTML).
The second argument lets you change the context. For example, to output something
in a JavaScript string, use the ``js`` context:

.. code-block:: js

    var myMsg = 'Hello <?php echo $view->escape($name, 'js') ?>';

.. index::
   single: Templating; Formats

.. _template-formats:

Template Formats
----------------

Templates are a generic way to render content in *any* format. And while in
most cases you'll use templates to render HTML content, a template can just
as easily generate JavaScript, CSS, XML or any other format you can dream of.

For example, the same "resource" is often rendered in several different formats.
To render an article index page in XML, simply include the format in the
template name:

*XML template name*: ``AcmeArticleBundle:Article:index.xml.twig``
*XML template filename*: ``index.xml.twig``

In reality, this is nothing more than a naming convention and the template
isn't actually rendered differently based on its format.

In many cases, you may want to allow a single controller to render multiple
different formats based on the "request format". For that reason, a common
pattern is to do the following:

.. code-block:: php

    public function indexAction()
    {
        $format = $this->getRequest()->getRequestFormat();
    
        return $this->render('AcmeBlogBundle:Blog:index.'.$format.'.twig');
    }

The ``getRequestFormat`` on the ``Request`` object defaults to ``html``,
but can return any other format based on the format requested by the user.
The request format is most often managed by the routing, where a route can
be configured so that ``/contact`` sets the request format to ``html`` while
``/contact.xml`` sets the format to ``xml``. For more information, see the
:ref:`Advanced Example in the Routing chapter <advanced-routing-example>`.

To create links that include the format parameter, include a ``_format``
key in the parameter hash:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('article_show', {'id': 123, '_format': 'pdf'}) }}">
	        PDF Version
	    </a>

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate('article_show', array('id' => 123, '_format' => 'pdf')) ?>">
            PDF Version
        </a>

Final Thoughts
--------------

The templating engine in Symfony is a powerful tool that can be used each time
you need to generate presentational content in HTML, XML or any other format.
And though templates are a common way to generate content in a controller,
their use is not mandatory. The ``Response`` object returned by a controller
can be created with our without the use of a template:

.. code-block:: php

    // creates a Response object whose content is the rendered template
    $response = $this->render('AcmeArticleBundle:Article:index.html.twig');

    // creates a Response object whose content is simple text
    $response = new Response('response content');

Symfony's templating engine is very flexible and two different template
renderers are available by default: the traditional *PHP* templates and the
sleek and powerful *Twig* templates. Both support a template hierarchy and
come packaged with a rich set of helper functions capable of performing
the most common tasks.

Overall, the topic of templating should be thought of as a powerful tool
that's at your disposal. In some cases, you may not need to render a template,
and in Symfony2, that's absolutely fine.

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/error_pages`

.. _`Twig`: http://www.twig-project.org
.. _`Symfony2Bundles.org`: http://symfony2bundles.org
.. _`Cross Site Scripting`: http://en.wikipedia.org/wiki/Cross-site_scripting
.. _`Output Escaping`: http://www.twig-project.org
.. _`tags`: http://www.twig-project.org/doc/templates.html#list-of-control-structures
.. _`filtres`: http://www.twig-project.org/doc/templates.html#list-of-built-in-filters
.. _`ajouter vos propres extensions`: http://www.twig-project.org/doc/advanced.html
