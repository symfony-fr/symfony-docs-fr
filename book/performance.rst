.. index::
   single: Tests

Performance
===========

Symfony2 est rapide, sans aucune configuration. Bien entendu, si vous voulez
que ça soit plus rapide, il y a de nombreuses façons de rendre Symfony
encore plus rapide. Dans ce chapitre, vous explorerez les méthodes les plus courantes
et les plus puissantes pour rendre votre application Symfony encore plus rapide.

.. index::
   single: Performance; Byte code cache

Utiliser un cache de byte code (ex APC)
---------------------------------------

L'une des meilleures choses (et des plus simples) que vous devriez faire pour
augmenter la performance est d'utiliser un cache de byte code (« byte code cache »).
L'idée d'un cache de byte code est de ne plus avoir à recompiler constamment
le code source PHP. Il existe un certain nombre de `caches de byte code`, dont certains
sont open source. Le cache de byte code le plus largement utilisé est 
probablement `APC`_

Il n'y a aucun effet négatif à utiliser un cache de byte code, et Symfony2 
a été conçu pour être vraiment performant dans ce type d'environnement.

D'autres optimisations
~~~~~~~~~~~~~~~~~~~~~~

Le cache de byte code surveille normalement tout changement de code source.
Cela permet de recompiler automatiquement le byte code en cas de changement
d'un fichier source. C'est très pratique, mais cela a évidemment un coût.

Pour cette raison, certains caches de byte code offrent l'option de désactiver
la vérification, ce sera donc à l'administrateur système de vider le cache 
à chaque changement de fichier. Sinon, les mises à jour ne seront pas visibles.

Par exemple, pour désactiver cette vérification avec APC, ajoutez simplement
``apc.stat=0`` à votre fichier de configuration php.ini.

.. index::
   single: Performance; Autoloader

Utiliser un chargeur automatique qui met en cache (par exemple ``ApcUniversalClassLoader``)
-------------------------------------------------------------------------------------------

Par défaut, la distribution Symfony standard utilise ``UniversalClassLoader``
dans le fichier `autoloader.php`_. Ce chargeur automatique (autoloader) est facile
à utiliser, car il va automatiquement trouver toute nouvelle classe que vous aurez
placée dans les répertoires enregistrés.

Malheureusement, cela a un coût, et le chargeur itère à travers tous les espaces de noms
(namespaces) pour trouver un fichier en particulier, en appelant ``file_exists`` 
jusqu'à ce qu'il trouve le fichier qu'il recherchait.

La solution la plus simple est de mettre en cache l'emplacement de chaque classe
après qu'elles ont été trouvées la première fois. Symfony est fourni avec une classe
- :class:`Symfony\\Component\\ClassLoader\\ApcClassLoader` - qui fait exactement cela.
Pour l'utiliser, il vous suffit d'adapter votre contrôleur frontal. Si vous utilisez la
distribution standard, le code devrait déjà être disponible dans ce fichier en commentaires::

    // app.php
    // ...

    $loader = require_once __DIR__.'/../app/bootstrap.php.cache';

    // Use APC for autoloading to improve performance
    // Change 'sf2' by the prefix you want in order to prevent key conflict with another application
    /*
    $loader = new ApcClassLoader('sf2', $loader);
    $loader->register(true);
    */

    // ...

.. note::

    Lorsque vous utilisez le chargeur automatique APC, si vous avez de nouvelles
    classes, elles vont être trouvées automatiquement et tout fonctionnera comme
    avant (donc aucune raison de vider le cache). Par contre, si vous changez
    l'emplacement d'un espace de noms ou d'un préfixe, il va falloir que vous vidiez le
    cache APC. Sinon, le chargeur automatique verra toujours l'ancien emplacement 
    des classes à l'intérieur de ce namespace.

.. index::
   single: Performance; Bootstrap files


Utiliser des fichiers d'amorçage
--------------------------------

Afin d'assurer une flexibilité maximale et de favoriser la réutilisation du code,
les applications Symfony2 profitent de nombreuses classes et composants externes.
Mais charger toutes les classes depuis des fichiers séparés à chaque requête peut
entraîner des coûts. Afin de réduire ces coûts, la distribution Symfony standard
fournit un script qui génère ce qu'on appelle un `fichier d'amorçage`_
(« bootstrap file »), qui consiste en une multitude de définitions de classes
en un seul fichier. En incluant ce fichier (qui contient une copie de 
nombreux fichiers du core), Symfony n'a plus besoin d'inclure les fichiers 
sources individuels qui contiennent ces classes. Cela va réduire un peu 
les lectures/écritures sur le disque.

Si vous utilisez l'édition Symfony standard, alors vous utilisez probablement 
déjà le fichier d'amorçage. Pour vous en assurer, ouvrez votre contrôleur frontal
(généralement ``app.php``) et vérifiez que la ligne suivante est présente :

.. code-block:: php

    require_once __DIR__.'/../app/bootstrap.php.cache';

Veuillez noter qu'il y a deux inconvénients à utiliser un fichier d'amorçage :

* le fichier nécessite d'être régénéré à chaque fois que les fichiers sources 
  originaux changent (à savoir quand vous mettez à jour le code source de Symfony2
  ou une bibliothèque tierce),

* lors du débogage, vous devrez placer des points d'arrêt (breakpoints) dans ce fichier
  d'amorçage.

Si vous utilisez l'édition Symfony2 standard, les fichiers d'amorçage sont automatiquement
régénérés après avoir mis à jour les bibliothèques tierces (« vendors »)
grâce à la commande ``php bin/vendors install``.

Fichiers d'amorçage et caches de byte code
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Même en utilisant un cache de byte code, la performance sera améliorée si vous utilisez
un fichier d'amorçage, car il y aura moins de fichiers dont il faut surveiller 
les changements. Bien sûr, si vous désactivez la surveillance des changements de fichiers
(par exemple ``apc.stat=0`` in APC), il n'y a plus de raison d'utiliser un fichier
d'amorçage.

.. _`caches de byte code`: http://en.wikipedia.org/wiki/List_of_PHP_accelerators
.. _`APC`: http://php.net/manual/en/book.apc.php
.. _`autoloader.php`: https://github.com/symfony/symfony-standard/blob/2.0/app/autoload.php
.. _`fichier d'amorçage`: https://github.com/sensio/SensioDistributionBundle/blob/master/Composer/ScriptHandler.php
