.. index::
   single: Templating

Créer et utiliser les templates
===============================

Comme vous le savez, le :doc:`contrôleur </book/controller>` est
responsable de la gestion de toute les requêtes d'une application
Symfony2. En réalité, le contrôleur délègue le plus gros du travail à
d'autres classes afin que le code puisse être testé et
réutilisé. Quand un contrôleur a besoin de générer du HTML, CSS ou
tout autre contenu, il donne la main au moteur de template.  Dans ce
chapitre, vous apprendrez comment écrire des templates puissants qui
peuvent être utilisés pour retourner du contenu à l'utilisateur,
remplir le corps d'emails et bien d'autres. Vous apprendrez des
raccourcis, des méthodes ingénieuses pour étendre les templates et
surtout comment les réutiliser.

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
-----------------------------------

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
  milieu, «controller», est absente (``Blog`` par exemple), le template se
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
~~~~~~~~~~~~~~~~~~~

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

Balises et Helpers
------------------

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

L'inclusion de Templates
~~~~~~~~~~~~~~~~~~~~~~~~

Vous voudrez souvent inclure le même template ou fragment de code dans
différentes pages. Par exemple, dans une application avec un espace «nouveaux
articles», le code du template affichant un article peut être utiliser sur la
page détaillant l'article, sur une page affichant les articles les plus
populaires, ou dans une liste des derniers articles.

Quand vous avez besoin de réutiliser une grand partie d'un code PHP,
typiquement vous déplacez le code dans une nouvelle classe PHP ou dans
une fonction. La même choses s'applique aussi aux templates. En
déplaçant le code réutilisé dans son propre template, il peut être
inclu par tous les autres templates. D'abord, créez le template que
vous souhaiterez réutiliser.

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
~~~~~~~~~~~~~~~~~~~~~

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
~~~~~~~~~~~~~~~~~~~~~~

La création de lien vers d'autres pages de votre projet est l'opération la plus
commune qui soit dans un template. Au lieu de coder en dure les URLs dans les
templates, utilisez la fonction ``path`` de Twig (ou le helper ``router`` en
PHP) pour genererles URLs basées sur la configuration des routes. Plus tard, si
vous désirez modifier l'URL d'une page particulière, tout ce que vous avez
besoin de faire c'est changer la configuration des routes; les templates
genereront automatiquement la nouvelle URL.

Dans un premier temps, configurons le lien vers la page «_welcome» qui est
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
~~~~~~~~~~~~~~~~~~~~~~~~~

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
------------------------------------------------------------

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

C'est assez simple. Mais comment faire si vous avez besoin d'inclure une feuille
de style supplémentaire ou un autre fichier javascript à partir d'un template
enfant ? Par exemple, supposons que vous avez une page contact et que vous avez
besoin d'inclure une feuille de style ``contact.css`` uniquement sur cette
page. Au sein du template de la page contact, faites comme ce qui suit :

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Contact/contact.html.twig #}
    {# extends '::base.html.twig' #}

    {% block stylesheets %}
        {{ parent() }}
        
        <link href="{{ asset('/css/contact.css') }}" type="text/css" rel="stylesheet" />
    {% endblock %}
    
    {# ... #}

Dans le template enfant, nous surchargeons simplement le bloque ``stylesheets``
en ajoutant une nouvelle balise de feuille de style dans ce bloque. Bien sûr,
puisque nous voulons ajouter du contenu au bloque parent (et non le *remplacé*),
nous devont utiliser la fonction Twig ``parent()`` pour inclure le bloque
``stylesheets`` du template de base.

Le résultat final est une page qui inclut à la fois la feuille de style
``main.css`` et ``contact.css``.

.. index::
   single: Templating; The templating service

Configuration et Utilisation du Service ``templating``
------------------------------------------------------

Le coeur du système de template dans Symfony2 est le ``Engine`` de template. Cet
objet spécial est responsable du rendement des templates et de retourner leur
contenu. Quand vous effectuez le rendu d'un template à travers un controlleur
par exemple, vous utilisez en effet le service de moteur de template. Par
exemple :

.. code-block:: php

    return $this->render('AcmeArticleBundle:Article:index.html.twig');

est équivalent à

.. code-block:: php

    $engine = $this->container->get('templating');
    $content = $engine->render('AcmeArticleBundle:Article:index.html.twig');

    return $response = new Response($content);

.. _template-configuration:

Le moteur de template (ou «service») est préconfiguré pour travailler
automatiquement dans Symfony2. Il peut, bien sur, être configuré grâce au
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

Plusieurs options de configuration sont disponibles et sont détaillé dans le
:doc:`Configuration Appendix</reference/configuration/framework>`.

.. note::

   Le moteur ``twig`` est mandaté pour utiliser un webprofiler (de même que
   beaucoup de bundles de tier-partie).

.. index::
    single; Template; Overriding templates

.. _overriding-bundle-templates:

La Surcharge de templates de Bundle
-----------------------------------

La communaté Symfony2 est fier de créer et de maintenir des bundles de haute
qualité (voir `Symfony2Bundles.org`_) pour un grand nombre d'aspects. A un fois
que vous utilisez un bundle d'un tier-partie, vous aurez surement besoin de
surcharger et personnaliser un ou plusieurs de ces templates.

Supposons que vous utilisiez un imaginaire ``AcmeBlogBundle`` open-source dans
votre projet (dans le répertoire ``src/Acme/blogBundle`` par exemple). Et bien
que vous soyez content de tout cela, vous voudriez surchargé le page «list» du
blog pour la personnaliser ces décorations spécialement pour votre
application. En cherchant un peu dans le contrôleur ``Blog`` du
``AcmeBlogBundle``, vous trouvez ceci : ::

    public function indexAction()
    {
        $blogs = // some logic to retrieve the blogs

        $this->render('AcmeBlogBundle:Blog:index.html.twig', array('blogs' => $blogs));
    }

Quand le ``AcmeBlogBundle:Blog:index.html.twig`` est rendu, Symfony2 regarde en
fait dans deux emplacements pour le template :

#. ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``

Pour surcharger le template du bundle, il suffit de copier le template
``index.html.twig`` du bundle vers
``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig`` (le répertoire
``app/Resources/AcmeBlogBundle`` n'existe pas, nous laissons le soin au lecteur
de le créer). Vous êtes maintenant à même de personnaliser le template.

Cet logique s'applique aussi au template layout. Supposons maintenant que chaque
template dans ``AcmeBloqBundle`` hérite d'un template de base appelé
``AcmeBlogBundle::layout.html.twig``. Comme précédemment, Symfony2 regardera
dans les deux emplacements suivants :

#. ``app/Resources/AcmeBlogBundle/views/layout.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/layout.html.twig``

Encore une fois, pour surchargé le template, il suffit de le copier du bundle
vers ``app/Resources/AcmeBlogBundle/views/layout.html.twig``. Vous êtes
maintenant libre de personnaliser cette copie comme il vous plaira.

Si vous prenez du recule, vous vous apercevrez que Symfony2 commence toujours
par regarder dans le répertoire ``app/Resources/{BUNDLE_NAME}/views/`` pour un
template. Si le template n'existe pas là, il continue sa recherche dans le
répertoire ``Resources/views`` du bundle le-même. Ce qui signifie que tous les
templates d'un bundle peuvent être surchargés à condition de les placer dans le
bon sous-répertoire de ``app/Resources``.

.. _templating-overriding-core-templates:

.. index::
    single; Template; Overriding exception templates

La Surcharge des Core Templates
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Puisque le framework Symfony2 lui même est juste un bundle, les core templates
peuvent être surchargés de la même façon. Par exemple, le core
``FrameworkBundle`` contient un certain nombre de templates relatives aux
«exception» et aux «erreurs» qui peuvent être surchargés en copiant chacun d'eux
du répertoire ``Ressources/views/Exception`` de ``FrameworkBundle`` vers, vous
le devenez, le répertoire ``app/Resources/FrameworkBundle/views/Exception``.

.. index::
   single: Templating; Three-level inheritance pattern

Trois niveaux d'héritages
-------------------------

Une façon commune d'utiliser l'héritage est d'utiliser l'approche à trois
niveaux. Cette méthode fonctionnement parfaitement avec trois différents types
de templates que nous détaillons :

* Créer un fichier ``app/Resources/views/base.html.twig`` qui contient le
  principal layout pour votre application (comme dans l'exemple précédent). En
  interne, ce template est appelé ``::base.html.twig``;

* Créer un template pour chaque «section» de votre site. Par exemple, un
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

* Créer un template individuel pour chaque page et faites en sorte que chacun
  étende (extend) la template approprié à la section. Par exemple, la page
  «index» sera appelé à peu de chose prés
  ``AcmeBlogBundle:Blog:index.html.twig`` et listera les postes du blog.

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
        {% extends 'AcmeBlogBundle::layout.html.twig' %}

        {% block content %}
            {% for entry in blog_entries %}
                <h2>{{ entry.title }}</h2>
                <p>{{ entry.body }}</p>
            {% endfor %}
        {% endblock %}

Remarquons que ce template étend la section
-(``AcmeBlogBundle::layout.html.twig``) qui à son tour étend le layout de base
de l'application (``::base.html.twig``). C'est le modéle commun d'héritage à
trois niveaux.

Quand vous fabriquez votre application, vous pouvez choisir de suivre cette
méthode ou simplement faire que chaque template de page étende le layout de
l'application directement (``{% extends '::base.html.twig' %}`` par exemple). Le
modèle des trois templates est une meilleure pratique, utilisée par les bundles
vendor. De ce fait, la layout d'un bundle peut facilement être surchargée pour
étendre proprement le layout de base de votre application.

.. index::
   single: Templating; Output escaping

L'Echappée
----------

Lors de la génération HTML d'un template, il y a toujours un risque qu'une
variable du template affiche du code HTML non désiré ou du code dangeux côté
client. Le résultat est que le contenu dynamique peut casser le code HTML de la
page résultante ou peremttre un utilisateur malicieux de réaliser une attaque
`Cross Site Scripting`_ (XSS). Considérons cet exemple classique :

.. configuration-block::

    .. code-block:: jinja

        Hello {{ name }}

    .. code-block:: php

        Hello <?php echo $name ?>

Imaginons que l'utilisateur ait rentré le code suivant comme son nom ::

    <script>alert('hello!')</script>

Sans sortie échappée, le template résultant provoquera l'affichage d'une boîte
d'alert Javascript ::

    Hello <script>alert('hello!')</script>

Et bien que cela semble inoffensif, si un utilisateur peut aller aussi loin, ce
même utilisateur peut aussi écrire un Javascript qui réalise des actions
malicieuses dans un espace sécurisé d'un inconnu et légitime utilisateur.

La réponse à ce problème est l'échappement de la sortie. Avec l'échappement de
la sortie à «on», le même template sera rendu de façon inoffensive, et affichera
littéralement la balise ``script`` à l'écran ::

    Hello &lt;script&gt;alert(&#39;helloe&#39;)&lt;/script&gt;

Le système de template Twig et PHP aborde le problème de façon différentes. Si
vous utilisez Twig, l'échappement est activé par défaut et vous êtes protégé. En
PHP, l'échappement n'est pas automatique, ce qui signifie que vous aurez besoin
d'échappé manuellement là où c'est nécessaire.

Léchappement avec Twig
~~~~~~~~~~~~~~~~~~~~~~

Si vous utilisez les templates de Twig, alors l'échappement est activé par
défaut. Ce qui signifie que vous êtes protéger immédiatement des conséquences
non intentionnels du code soumis par l'utilisateur. Par défaut, l'échappement
suppose que le contenu est bien échappé pour un affichage HTML.

Dans certains cas, vous aurez besoin de désactiver l'échappement de la sortie
lors du rendu d'une variable qui est sûr et qui contient des décorations qui ne
doivent pas être échappées. Supposans que des utilisateurs administrateurs sont
capable décrire des articles qui contienne du code HTML. Par défaut, Twig
échappera le corps de l'article. Pour le rendre normallement, il suffit
d'ajouter le filtre ``raw`` : ``{{ article.body | raw }}``.

Vous pouvez aussi désactiver l'échappement au sein d'un ``{% block %}`` ou pour
un template entier, Pour plus d'information, voir `Output Escaping`_ dans la
documentation de Twig.

L'échappement en PHP
~~~~~~~~~~~~~~~~~~~~

L'échappement n'est pas automatique lorsque vous utilisez des templates PHP. Ce
qui signife que à moins que vous choissiez explicitement d'échapper une
variable, vous n'êtes pas protéger. Pour utiliser l'échappement, utilisez la
méthode spéciale ``escape()`` de view : ::

    Hello <?php echo $view->escape($name) ?>

Par défaut, la méthode ``escape()`` suppose que la variable est rendu dans un
contexte HTML (et donc que la variable est échappée pour être sans danger pour
l'HTML). Le second argument vous permet de changer de contexte. Par exemple,
pour afficher quelque chose dans un chaine de caractère JavaScript, utilisez le
context ``js`` :

.. code-block:: js

    var myMsg = 'Hello <?php echo $view->escape($name, 'js') ?>';

.. index::
   single: Templating; Formats

.. _template-formats:

Les Formats de Template
-----------------------

Les templates sont une façon générique de rendre un contenu dans *n'importe
quel* format. Et bien que dans la plupart des cas vous utiliserez les templates
pour rendre du contenu HTML, un template peut tout aussi facilement générer du
JavaScript, du CSS, du XML ou tout autre format dont vous pouvez rêver.

Par exemple, la même «ressource» est souvent rendu dans plusieurs différents
formats. Pour rendre la page index d'un article en XML, incluez simplement le
format dans le nom du template :

*nom du template XML*: ``AcmeArticleBundle:Article:index.xml.twig``
*nom de fichier du template XML*: ``index.xml.twig``

En réalité, ce n'est rien de plus qu'une convention de nommage et le template
n'est pas rendu différemment en se basant sur ce format.

Dans beaucoup de cas, vous pourriez vouloir autoriser un simple contrôleur à
rendre plusieurs formats en se basant sur le «request format». Pour cette
raison, un pattern commun est de procéder comme cela :

.. code-block:: php

    public function indexAction()
    {
        $format = $this->getRequest()->getRequestFormat();
    
        return $this->render('AcmeBlogBundle:Blog:index.'.$format.'.twig');
    }

Le ``getRequestFormat`` sur l'objet ``Request`` retourne par défaut ``html``,
mais peut aussi retourner n'importe quel autre format basé sur le format demandé
par l'utilisateur. Le format demandé est le plus souvent géré par le système de
route, où une route peut être configuré tel que ``/contact`` définit le format
demandé à ``html`` alors que ``/contact.xml`` définit le format à ``xml``. Pour
plus d'information, voir le :ref:`Advanced Example in the Routing chapter
<advanced-routing-example>`.

Pour créer des liens qui inclus le paramétre de format, incluez une clé
``_format`` dans le paramètre de hashage :

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

Le moteur de template dans Symfony est un outil puissant qui peut être utiliser
chaque fois que vous avez besoin de générer du contenu de répresentation en
HTML, XML ou tout autre format. Et bien que les templates soient un moyen commun
de générer du contenu dans un contrôleur, leur utilisation n'est pas
systèmatique. L'objet ``Response`` retourné par un contrôleur peut être créé
avec ou sans utilisation de template :

.. code-block:: php

    // création d'un objet Response qui contient le rendu d'un template
    $response = $this->render('AcmeArticleBundle:Article:index.html.twig');

    // création d'un objet Response qui contient un texte simple
    $response = new Response('response content');

Le moteur de template de Symfony est très flexible et deux outils de restition
sont disponibles par défaut : les traditionnels templates *PHP* et les élégants
et puissants templates *Twig*. Ils supportent tout les deux une hiérarchie des
template et sont fournis avec un ensemble riche de fonctions helper capablent de
réaliser la plupart des tâches.

Dans l'ensemble, le système de templates doit être pensé comme étant un outil
puissant qui est à votre disposition. Dans certains cas, vous n'aurez pas besoin
de rendre un template, et dans Symfony2, c'est tout à fait envisageasble.

En savoir plus grâce au Cookbook
--------------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/error_pages`

.. _`Twig`: http://www.twig-project.org
.. _`Symfony2Bundles.org`: http://symfony2bundles.org
.. _`Cross Site Scripting`: http://en.wikipedia.org/wiki/Cross-site_scripting
.. _`Output Escaping`: http://www.twig-project.org
.. _`tags`: http://www.twig-project.org/doc/templates.html#list-of-control-structures
.. _`filtres`: http://www.twig-project.org/doc/templates.html#list-of-built-in-filters
.. _`ajouter vos propres extensions`: http://www.twig-project.org/doc/advanced.html
