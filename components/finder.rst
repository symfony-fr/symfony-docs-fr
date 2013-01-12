.. index::
   single: Finder
   single: Components; Finder

Le Composant Finder
===================

    Le Composant Finder trouve des fichiers et des répertoires via une interface
    intuitive.

Installation
------------

Vous pouvez installer le composant de différentes manières :

* Utilisez le dépôt Git officiel (https://github.com/symfony/Finder) ;
* Installez le via Composer (``symfony/finder`` sur `Packagist`_).

Utilisation
-----------

La classe :class:`Symfony\\Component\\Finder\\Finder` trouve des fichiers
et/ou des répertoires::

    use Symfony\Component\Finder\Finder;

    $finder = new Finder();
    $finder->files()->in(__DIR__);

    foreach ($finder as $file) {
        // affiche le chemin absolu
        print $file->getRealpath()."\n";
        // affiche le chemin relatif d'un fichier, sans le nom du fichier
        print $file->getRelativePath()."\n";
        // affiche le chemin relatif du fichier
        print $file->getRelativePathname()."\n";
    }

``$file`` est une instance de :class:`Symfony\\Component\\Finder\\SplFileInfo`
qui étend :phpclass:`SplFileInfo` afin de fournir des méthodes permettant de
travailler avec les chemins relatifs.

Le code ci-dessus affiche les noms de tous les fichiers dans le répertoire courant
de manière récursive. La classe « Finder » utilise une interface intuitive, et de ce fait
toutes les méthodes retournent une instance de « Finder ».

.. tip::

    Une instance de « Finder » est un `Iterator`_ PHP. Donc, au lieu d'itérer
    sur le « Finder » avec un ``foreach``, vous pouvez aussi le convertir en
    un tableau avec la méthode :phpfunction:`iterator_to_array`, ou récupérer
    le nombre d'éléments grâce à :phpfunction:`iterator_count`.

Critères
--------

Le chemin
~~~~~~~~~

Le chemin (c-a-d l'endroit où se trouve le fichier) est le seul critère
obligatoire. Il informe le « finder » du répertoire à utiliser pour
la recherche::

    $finder->in(__DIR__);

Vous pouvez effectuer une recherche dans plusieurs répertoires/chemins en
chaînant les appels de la méthode
:method:`Symfony\\Component\\Finder\\Finder::in`::

    $finder->files()->in(__DIR__)->in('/elsewhere');

Vous pouvez exclure des répertoires de votre recherche grâce à la
méthode :method:`Symfony\\Component\\Finder\\Finder::exclude`::

    $finder->in(__DIR__)->exclude('ruby');

Comme le « Finder » utilise des itérateurs PHP, vous pouvez passer
n'importe quelle URL ayant un `protocole`_ supporté::

    $finder->in('ftp://example.com/pub/');

Et cela fonctionne aussi avec les flux définis par l'utilisateur::

    use Symfony\Component\Finder\Finder;

    $s3 = new \Zend_Service_Amazon_S3($key, $secret);
    $s3->registerStreamWrapper("s3");

    $finder = new Finder();
    $finder->name('photos*')->size('< 100K')->date('since 1 hour ago');
    foreach ($finder->in('s3://bucket-name') as $file) {
        // faites quelque chose

        print $file->getFilename()."\n";
    }

.. note::

    Lisez la documentation sur les `Flux`_ pour apprendre comment créer vos
    propres flux.

Fichiers et répertoires
~~~~~~~~~~~~~~~~~~~~~~~

Par défaut, le « Finder » retourne des fichiers et des répertoires ; mais les
méthodes :method:`Symfony\\Component\\Finder\\Finder::files` et
:method:`Symfony\\Component\\Finder\\Finder::directories` contrôlent cela::

    $finder->files();

    $finder->directories();

Si vous souhaitez suivre des liens, utilisez la méthode ``followLinks()``::

    $finder->files()->followLinks();

Par défaut, l'itérateur ignore les fichiers VCS dont le type est populaire.
Cela peut être modifié grâce à la méthode ``ignoreVCS()``::

    $finder->ignoreVCS(false);

Triage
~~~~~~

Triez les résultats par nom ou par type (répertoires en premier, ensuite les
fichiers)::

    $finder->sortByName();

    $finder->sortByType();

.. note::

    Notez que les méthodes ``sort*`` ont besoin de récupérer tous les éléments
    correspondants à la recherche pour effectuer leur travail. Pour des
    itérateurs de grande taille, cela est lent.

Vous pouvez aussi définir votre propre algorithme de triage via la méthode ``sort()``::

    $sort = function (\SplFileInfo $a, \SplFileInfo $b)
    {
        return strcmp($a->getRealpath(), $b->getRealpath());
    };

    $finder->sort($sort);

Nom de Fichier
~~~~~~~~~~~~~~

Restreignez les fichiers par leur nom grâce à la méthode
:method:`Symfony\\Component\\Finder\\Finder::name`::

    $finder->files()->name('*.php');

La méthode ``name()`` accepte des « globs », des chaînes de caractères,
ou des expressions régulières::

    $finder->files()->name('/\.php$/');

La méthode ``notName()`` exclut les fichiers correspondant à un pattern::

    $finder->files()->notName('*.rb');

Contenus de fichier
~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    Les méthodes ``contains()`` et ``notContains()`` ont été introduites
    dans la version 2.1.

Restreignez les fichiers par leur contenu grâce à la méthode
:method:`Symfony\\Component\\Finder\\Finder::contains`::

    $finder->files()->contains('lorem ipsum');

La méthode ``contains()`` accepte des chaînes de caractères ou des
expressions régulières::

    $finder->files()->contains('/lorem\s+ipsum$/i');

La méthode ``notContains()`` exclut les fichiers correspondant à un pattern
donné::

    $finder->files()->notContains('dolor sit amet');

Path
~~~~

.. versionadded:: 2.2
   Les méthodes ``path()`` et ``notPath()`` ont été ajoutées dans la version 2.2.

Vous pouvez restreindre les fichiers et les répertoire en fonction
de leur chemin avec la méthode :method:`Symfony\\Component\\Finder\\Finder::path`::

    $finder->path('some/special/dir');

Le slash (c-a-d ``/``) devrait être utilisé comme séparateur de dossiers sur
toutes les plateformes.

La méthode `path()`` accepte une chaine de caractères ou une expression
régulière::

    $finder->path('foo/bar');
    $finder->path('/^foo\/bar/');

En interne, les chaines de caractères sont converties en expressions régulières
en échappant les slashes et en ajoutant des délimiteurs :

.. code-block:: text

    dirname    ===>    /dirname/
    a/b/c      ===>    /a\/b\/c/

La méthode :method:`Symfony\\Component\\Finder\\Finder::notPath` exclut des fichiers
en fonction de leur chemin::

    $finder->notPath('other/dir');

Taille de Fichier
~~~~~~~~~~~~~~~~~

Restreignez les fichiers par leur taille grâce à la méthode
:method:`Symfony\\Component\\Finder\\Finder::size`::

    $finder->files()->size('< 1.5K');

Restreignez par un intervalle de taille en chaînant les appels::

    $finder->files()->size('>= 1K')->size('<= 2K');

L'opérateur de comparaison peut être l'un des suivants : ``>``, ``>=``, ``<``, ``<=``,
``==``, ``!=``.

.. versionadded:: 2.1
    L'opérateur ``!=`` a été ajouté dans la version 2.1.

La valeur cible peut utiliser les unités suivantes : kilo-octets (``k``, ``ki``), mega-octets
(``m``, ``mi``), ou giga-octets (``g``, ``gi``). Celles suffixées avec un ``i`` utilisent
la version appropriée ``2**n`` en accord avec le `standard IEC`_.

Date de fichier
~~~~~~~~~~~~~~~

Restreignez les fichiers par leur date de dernière modification grâce à la
méthode :method:`Symfony\\Component\\Finder\\Finder::date`::

    $finder->date('since yesterday');

L'opérateur de comparaison peut être l'un des suivants : ``>``, ``>=``, ``<``,
'<=', '=='. Vous pouvez aussi utiliser ``since`` (« depuis » en français) ou
``after`` (« après » en français) en tant qu'alias de ``>``, et ``until``
(« jusqu'à » en français) ou ``before`` (« avant » en français) en tant qu'alias
de ``<``.

La valeur cible peut être n'importe quelle date supportée par la fonction
`strtotime`_.

Profondeur de répertoire
~~~~~~~~~~~~~~~~~~~~~~~~

Par défaut, le « Finder » parcourt les répertoires récursivement. Restreignez
la profondeur de navigation grâce à la méthode
:method:`Symfony\\Component\\Finder\\Finder::depth`::

    $finder->depth('== 0');
    $finder->depth('< 3');

Filtrage personnalisé
~~~~~~~~~~~~~~~~~~~~~

Pour restreindre les fichiers correspondants à votre propre stratégie,
utilisez la méthode :method:`Symfony\\Component\\Finder\\Finder::filter`::

    $filter = function (\SplFileInfo $file)
    {
        if (strlen($file) > 10) {
            return false;
        }
    };

    $finder->files()->filter($filter);

La méthode ``filter()`` prend une Closure en argument. Pour chaque fichier qui
correspond, cette dernière est appelée avec le fichier en tant qu'instance de
:class:`Symfony\\Component\\Finder\\SplFileInfo`. Le fichier est exclu de
l'ensemble des résultats si la Closure retourne ``false``.

Lire le contenu des fichiers retournés
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
   La méthode ``getContents()`` a été ajoutée dans la version 2.1.

Les contenus des fichiers retournés peuvent être lus avec
:method:`Symfony\\Component\\Finder\\SplFileInfo::getContents`::

    use Symfony\Component\Finder\Finder;

    $finder = new Finder();
    $finder->files()->in(__DIR__);

    foreach ($finder as $file) {
        $contents = $file->getContents();
        ...
    }

.. _strtotime:   http://www.php.net/manual/fr/datetime.formats.php
.. _Iterator:     http://www.php.net/manual/fr/spl.iterators.php
.. _protocole:     http://www.php.net/manual/fr/wrappers.php
.. _Flux:      http://www.php.net/streams
.. _standard IEC: http://physics.nist.gov/cuu/Units/binary.html
.. _Packagist:    https://packagist.org/packages/symfony/finder