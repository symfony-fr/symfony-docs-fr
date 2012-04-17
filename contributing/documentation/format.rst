Documentation Format
====================

La documentation Symfony2 utilise comme format d'écriture `reStructuredText`_ 
et comme constructeur `Sphinx`_ pour produire les documents dans d'autres 
formats (HTML, PDF, ...).

reStructuredText
----------------

reStructuredText "is an easy-to-read, what-you-see-is-what-you-get plaintext
markup syntax and parser system".

Vous pouvez apprendre la syntaxe rst en lisant la documentation existante
Symfony2 `documents`_ ou en lisant `reStructuredText Primer`_ sur le site web de
Sphinx.

Si vous êtes familier avec Markdown, soyez attentif à certaines tournures
d'apparences similaires mais dont le comportement diffèrent:

* Les listes commencent au début de la ligne (aucune indentation n'est permise);

* Les lignes de codes utilisent des double ticks (````like this````).

Sphinx
------

Sphinx est le systeme de construction ajoutant certaines fonctionnalités
agréables permettant de créer une documentation à partir de documents 
reStructuredText. Il ajoute de nouvelles directives et des fonctions comme le
standard reST `markup`_.

Syntaxe Colorée
~~~~~~~~~~~~~~~

Tout example de code utilise PHP comme schéma de coloration par défaut. Vous
pouvez changer cela à l'aide de la directive ``code-block`` :

.. code-block:: rst

    .. code-block:: yaml

        { foo: bar, bar: { foo: bar, bar: baz } }

Si votre code PHP débute par ``<?php``, vous devrez utiliser ``html+php`` comme
pseudo-language de coloration:

.. code-block:: rst

    .. code-block:: html+php

        <?php echo $this->foobar(); ?>

.. note::

    Une liste des langages supportés ainsi que leurs noms respectifs est
	disponible sur le site web `Pygments`_.

Blocs de configurations
~~~~~~~~~~~~~~~~~~~~~~~

Si vous affichez une configuration, vous devez utiliser la directive
``configuration-block`` dans tous les formats de configuration reconnus
(``PHP``, ``YAML``, et ``XML``)

.. code-block:: rst

    .. configuration-block::

        .. code-block:: yaml

            # Configuration in YAML

        .. code-block:: xml

            <!-- Configuration in XML //-->

        .. code-block:: php

            // Configuration in PHP

Le code précédent sera rendu comme cela:

.. configuration-block::

    .. code-block:: yaml

        # Configuration in YAML

    .. code-block:: xml

        <!-- Configuration in XML //-->

    .. code-block:: php

        // Configuration in PHP

La liste des formats supportés:

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

Tester une Documentation
~~~~~~~~~~~~~~~~~~~~~~~~

Afin de tester une documentation avant de la proposer:

* Installer `Sphinx`_;

* Exécuter l'installateur `Sphinx quick setup`_;

* Installer l'extension concernantes les blocs de configuration (voir plus bas);

* Executez ``make html`` et contrôlez le code HTML généré dans le répertoire
  ``build``.

Installer l'extension concernant les blocs de configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* Télécharger l'extension depuis le dépot `configuration-block source`_

* Copier ``configurationblock.py`` vers le dossier ``_exts`` dans le répertoire
  racine (où se trouve ``conf.py``)

* Ajouter les indications suivantes au fichier ``conf.py``:

.. code-block:: py
    
    # ...
    sys.path.append(os.path.abspath('_exts'))
    
    # ...
    # add configurationblock to the list of extensions
    extensions = ['configurationblock']

.. _reStructuredText:           http://docutils.sf.net/rst.html
.. _Sphinx:                     http://sphinx.pocoo.org/
.. _documents:                  http://github.com/symfony/symfony-docs
.. _reStructuredText Primer:    http://sphinx.pocoo.org/rest.html
.. _markup:                     http://sphinx.pocoo.org/markup/
.. _Pygments:                   http://pygments.org/languages/
.. _configuration-block source: https://github.com/fabpot/sphinx-php
.. _Sphinx quick setup:         http://sphinx.pocoo.org/tutorial.html#setting-up-the-documentation-sources
