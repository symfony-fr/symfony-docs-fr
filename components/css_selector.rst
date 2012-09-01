.. index::
   single: CSS Selector
   single: Components; CssSelector

Le Composant CssSelector
========================

    Le Composant CssSelector convertit des sélecteurs CSS en expressions XPath.

Installation
------------

Vous pouvez installer le composant de différentes manières :

* Utilisez le dépôt Git officiel (https://github.com/symfony/CssSelector) ;
* Installez le via PEAR ( `pear.symfony.com/CssSelector`) ;
* Installez le via Composer (`symfony/css-selector` dans Packagist).

Utilisation
-----------

Pourquoi utiliser des sélecteurs CSS ?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Lorsque vous parcourez un document HTML ou XML, la méthode la plus puissante
est de loin d'utiliser XPath.

Les expressions XPath sont incroyablement flexibles, ce qui fait qu'il y a
presque toujours une expression XPath qui va trouver l'élément dont vous avez
besoin. Malheureusement, ces expressions peuvent aussi devenir très compliquées,
et la courbe d'apprentissage est raide. Même les opérations communes (comme
trouver un élément ayant une classe particulière) peuvent requérir des
expressions longues et difficile à manier.

Beaucoup de développeurs -- en particulier les développeurs web -- sont
plus à l'aise avec l'utilisation de sélecteurs CSS pour trouver des éléments.
En plus de fonctionner dans les feuilles de styles, les sélecteurs CSS sont
aussi utilisés en Javascript avec la fonction ``querySelectorAll`` et dans des
bibliothèques Javascript populaires telles jQuery, Prototype et MooTools.

Les sélecteurs CSS sont moins puissants que XPath, mais sont beaucoup plus
facile à écrire, lire et comprendre. Comme ils sont moins puissants, quasiment
tous les sélecteurs CSS peuvent être convertis en un équivalent XPath. Cette
expression XPath peut dès lors être utilisée avec d'autres fonctions et classes
qui utilisent XPath pour trouver des éléments dans un document.

Le composant ``CssSelector``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le seul objectif de ce composant est de convertir des sélecteurs CSS
en leurs équivalents XPath::

    use Symfony\Component\CssSelector\CssSelector;

    print CssSelector::toXPath('div.item > h4 > a');

Cela vous affiche ce qui suit :

.. code-block:: text

    descendant-or-self::div[contains(concat(' ',normalize-space(@class), ' '), ' item ')]/h4/a

Vous pouvez utiliser cette expression avec, par exemple, :phpclass:`DOMXPath`
ou avec :phpclass:`SimpleXMLElement` afin de trouver des éléments dans un document.

.. tip::

    La méthode :method:`Crawler::filter()<Symfony\\Component\\DomCrawler\\Crawler::filter>`
    utilise le composant ``CssSelector`` pour trouver des éléments en se basant sur une
    chaîne de caractères représentant un sélecteur CSS. Lisez :doc:`/components/dom_crawler`
    pour plus de détails.

Limitations du composant CssSelector
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Tous les sélecteurs CSS ne peuvent pas être convertis en un équivalent XPath.

Il y a plusieurs sélecteurs CSS qui n'ont de sens que dans le contexte
d'un navigateur web.

* sélecteurs basés sur le statut d'un lien : ``:link``, ``:visited``, ``:target``
* sélecteurs basés sur l'action d'un utilisateur : ``:hover``, ``:focus``, ``:active``
* sélecteurs basés sur l'état de l'« UI » : ``:enabled``, ``:disabled``, ``:indeterminate``
  (cependant, ``:checked`` et ``:unchecked`` sont disponibles)

Les pseudo-éléments (``:before``, ``:after``, ``:first-line``, ``:first-letter``)
ne sont pas supportés car ils sélectionnent des portions de texte plutôt que
des éléments.

Plusieurs pseudo-classes ne sont pas encore supportées :

* ``:lang(language)``
* ``root``
* ``*:first-of-type``, ``*:last-of-type``, ``*:nth-of-type``,
  ``*:nth-last-of-type``, ``*:only-of-type``. (Ceux-ci fonctionnent avec un
  nom d'élément (par exemple : ``li:first-of-type``) mais pas avec ``*``.
