Format de Documentation 
=======================

La documentation Symfony2 utilise `reStructuredText`_ comme format d'écriture  
et `Sphinx`_ pour produire les documents dans d'autres formats (HTML, PDF, ...).

reStructuredText
----------------

reStructuredText « est une syntaxe ce-que-vous-voyez-est-ce-que-vous-obtenez (wysiwyg) facile à lire 
et un analyseur (parser) ».

Vous pouvez apprendre la syntaxe rst en lisant la `documentation`_ Symfony2
existante ou en lisant `les bases de reStructuredText`_ sur le site de
Sphinx.

Si vous êtes familier avec Markdown, soyez attentif à certains concepts
qui semblent très similaires mais sont différents :

* Les listes commencent au début de la ligne (aucune indentation n'est permise);

* Les lignes de codes utilisent des double ticks (````comme ceci````).

Sphinx
------

Sphinx est un système de génération qui ajoute certaines fonctionnalités
sympas permettant de créer une documentation à partir de documents 
reStructuredText. Il ajoute de nouvelles directives et des fonctions comme le
standard reST `markup`_.

Coloration syntaxique
~~~~~~~~~~~~~~~~~~~~~

Tout exemple de code utilise PHP comme schéma de coloration syntaxique par défaut. Vous
pouvez changer cela à l'aide de la directive ``code-block`` :

.. code-block:: rst

    .. code-block:: yaml

        { foo: bar, bar: { foo: bar, bar: baz } }

Si votre code PHP débute par ``<?php``, vous devrez utiliser ``html+php`` comme
pseudo-language de coloration :

.. code-block:: rst

    .. code-block:: html+php

        <?php echo $this->foobar(); ?>

.. note::

    Une liste des langages supportés ainsi que leurs noms respectifs est
    disponible sur le site web `Pygments`_.

Blocs de configurations
~~~~~~~~~~~~~~~~~~~~~~~

Si vous affichez une configuration, vous devez utiliser la directive
``configuration-block`` dans tous les formats de configuration supportés
(``PHP``, ``YAML``, et ``XML``)

.. code-block:: rst

    .. configuration-block::

        .. code-block:: yaml

            # Configuration in YAML

        .. code-block:: xml

            <!-- Configuration in XML //-->

        .. code-block:: php

            // Configuration in PHP

Le code reST précédent sera affiché comme cela :

.. configuration-block::

    .. code-block:: yaml

        # Configuration in YAML

    .. code-block:: xml

        <!-- Configuration in XML //-->

    .. code-block:: php

        // Configuration in PHP

La liste des formats supportés :

+-----------------------+-------------+
| Identifiant du Format | Affichage   |
+=======================+=============+
| html                  | HTML        |
+-----------------------+-------------+
| xml                   | XML         |
+-----------------------+-------------+
| php                   | PHP         |
+-----------------------+-------------+
| yaml                  | YAML        |
+-----------------------+-------------+
| jinja                 | Twig        |
+-----------------------+-------------+
| html+jinja            | Twig        |
+-----------------------+-------------+
| jinja+html            | Twig        |
+-----------------------+-------------+
| php+html              | PHP         |
+-----------------------+-------------+
| html+php              | PHP         |
+-----------------------+-------------+
| ini                   | INI         |
+-----------------------+-------------+
| php-annotations       | Annotations |
+-----------------------+-------------+

Ajouter des liens
~~~~~~~~~~~~~~~~~

Pour ajouter des liens à d'autres pages dans les documents, utilisez la
syntaxe suivante :

.. code-block:: rst

    :doc:`/path/to/page`

En utilisant le chemin et le nom du fichier de la page sans l'extension,
par exemple :

.. code-block:: rst

    :doc:`/book/controller`

    :doc:`/components/event_dispatcher/introduction`

    :doc:`/cookbook/configuration/environments`

Le texte du lien sera le titre principal du document vers lequel pointe le
lien. Vous pouvez aussi spécifier un autre texte pour le lien :

.. code-block:: rst

    :doc:`Spooling Email</cookbook/email/spool>`

Vous pouvez aussi ajouter des liens vers la documentation de l'API :

.. code-block:: rst

    :namespace:`Symfony\\Component\\BrowserKit`

    :class:`Symfony\\Component\\Routing\\Matcher\\ApacheUrlMatcher`

    :method:`Symfony\\Component\\HttpKernel\\Bundle\\Bundle::build`

et vers la documentation de PHP :

.. code-block:: rst

    :phpclass:`SimpleXMLElement`

    :phpmethod:`DateTime::createFromFormat`

    :phpfunction:`iterator_to_array`

Tester une Documentation
~~~~~~~~~~~~~~~~~~~~~~~~

Afin de tester une documentation avant de la proposer :

* Installez `Sphinx`_;

* Exécutez l'installeur `Installation rapide de Sphinx`_;

* Installez les extensions Sphinx (voir plus bas);

* Executez ``make html`` et relisez le code HTML généré dans le répertoire
  ``build``.

Installer les extensions Sphinx
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* Télécharger l'extension depuis le dépôt `source`_

* Copiez le répertoire ``sensio`` vers le dossier ``_exts`` dans le répertoire
  racine (où se trouve ``conf.py``)

* Ajouter les instructions suivantes au fichier ``conf.py`` :

.. code-block:: py
    
    # ...
    sys.path.append(os.path.abspath('_exts'))

    # ajouter PhpLexer 
    from sphinx.highlighting import lexers 
    from pygments.lexers.web import PhpLexer
    
    # ...
    # ajoute les extensions à la liste des extensions
    extensions = [..., 'sensio.sphinx.refinclude', 'sensio.sphinx.configurationblock', 'sensio.sphinx.phpcode']

    # active la coloration syntaxique par défaut pour le code PHP qui n'est pas entre ``<?php ... ?>``
    lexers['php'] = PhpLexer(startinline=True)
    lexers['php-annotations'] = PhpLexer(startinline=True)

    # ajoute PHP comme domaine principal
    primary_domain = 'php'
    
    # Ajoute les urls pour les liens vers l'API
    api_url = 'http://api.symfony.com/master/%s'

.. _reStructuredText:           http://docutils.sourceforge.net/rst.html
.. _Sphinx:                     http://sphinx-doc.org/
.. _documentation:                  https://github.com/symfony/symfony-docs
.. _les bases de reStructuredText:    http://sphinx-doc.org/rest.html
.. _markup:                     http://sphinx-doc.org/markup/
.. _Pygments:                   http://pygments.org/languages/
.. _source: https://github.com/fabpot/sphinx-php
.. _Installation rapide de Sphinx:         http://sphinx-doc.org/tutorial.html#setting-up-the-documentation-sources
