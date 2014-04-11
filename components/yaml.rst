.. index::
   single: Yaml
   single: Components; Yaml
   
Le Composant YAML
=================

    Le Composant YAML charge et effectue un « dump » de fichiers YAML.

Qu'est ce que c'est ?
---------------------

Le Composant YAML de Symfony2 analyse des chaînes de caractères YAML afin de les
convertir en tableaux PHP.
Il est aussi capable de convertir des tableaux PHP en chaînes de caractères YAML.

`YAML`_, *YAML Ain't Markup Language* (litéralement : « YAML n'est pas un langage
de balises » en français), est un standard de sérialisation de données facilement
compréhensible par l'humain pour tous les langages de programmation. YAML est un format
génial pour vos fichiers de configuration. Les fichiers YAML sont aussi parlant que
des fichiers XML et aussi lisibles que des fichiers INI.

Le Composant YAML de Symfony2 implémente la version 1.2 de la spécification YAML.

Installation
------------

Vous pouvez installer le composant de différentes manières :

* Utilisez le dépôt Git officiel (https://github.com/symfony/Yaml) ;
* Installez le via Composer (``symfony/yaml`` sur `Packagist`_).

Pourquoi ?
----------

Rapide
~~~~~~

L'un des buts de Symfony YAML est de trouver le bon équilibre entre vitesse et
fonctionnalités. Il supporte juste les fonctionnalités nécessaires à la gestion
des fichiers de configuration.

Un analyseur réel
~~~~~~~~~~~~~~~~~

Il arbore un analyseur réel et est capable d'analyser (« parser ») un sous-ensemble
important de la spécification YAML, pour tous vos besoins de configuration. Cela signifie
aussi que l'analyseur est particulièrement robuste, facile à comprendre, et
suffisamment simple à étendre.

Des messages d'erreur clairs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Chaque fois que vous avez un problème de syntaxe avec vos fichiers YAML, la
bibliothèque affiche un message utile avec le nom du fichier et le numéro de
la ligne où le problème est apparu. Cela facilite énormément le débuggage.

Support du « Dump »
~~~~~~~~~~~~~~~~~~~

Le composant est aussi capable d'afficher (« dump » en anglais) des tableaux PHP
en YAML avec le support d'objet et de configuration directement dans le code
afin d'avoir un « bel » affichage.

Support des Types
~~~~~~~~~~~~~~~~~

Il supporte la plupart des types YAML intégrés comme les dates, les entiers,
les octaux, les booléens, et plus encore...

Support complet de la fusion par clé
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Support complet des références, des alias, et de la fusion par clé. Ne
vous répétez pas en référençant les bouts de configuration communs.

Utiliser le Composant YAML de Symfony2
--------------------------------------

Le Composant YAML de Symfony2 est très simple et consiste en deux classes
principales : l'une analyse les chaînes de caractères YAML
(:class:`Symfony\\Component\\Yaml\\Parser`), et l'autre affiche une chaîne de
caractères YAML à partir d'un tableau PHP (:class:`Symfony\\Component\\Yaml\\Dumper`).

Au-dessus de ces deux classes, la classe :class:`Symfony\\Component\\Yaml\\Yaml`
agit comme une fine couche supplémentaire qui simplifie les usages communs.

Lire des fichiers YAML
~~~~~~~~~~~~~~~~~~~~~~

La méthode :method:`Symfony\\Component\\Yaml\\Parser::parse` analyse une
chaîne de caractères YAML et la convertit en un tableau PHP :

.. code-block:: php

    use Symfony\Component\Yaml\Parser;

    $yaml = new Parser();

    $value = $yaml->parse(file_get_contents('/path/to/file.yml'));

Si une erreur se produit durant l'analyse, l'analyseur lance une exception
:class:`Symfony\\Component\\Yaml\\Exception\\ParseException` indiquant le
type d'erreur et la ligne de la chaîne de caractères YAML où l'erreur est
apparue :

.. code-block:: php

    use Symfony\Component\Yaml\Exception\ParseException;

    try {
        $value = $yaml->parse(file_get_contents('/path/to/file.yml'));
    } catch (ParseException $e) {
        printf("Unable to parse the YAML string: %s", $e->getMessage());
    }

.. tip::

    Comme l'analyseur est « réutilisable », vous pouvez utiliser le même
    objet analyseur pour charger différentes chaînes de caractères YAML.

Lorsque vous chargez un fichier YAML, il est parfois mieux d'utiliser la
méthode de surcouche :method:`Symfony\\Component\\Yaml\\Yaml::parse` :

.. code-block:: php

    use Symfony\Component\Yaml\Yaml;

    $yaml = Yaml::parse('/path/to/file.yml');

La méthode statique :method:`Symfony\\Component\\Yaml\\Yaml::parse` prend une
chaîne de caractères YAML ou un fichier contenant du YAML. En interne, elle appelle
la méthode :method:`Symfony\\Component\\Yaml\\Parser::parse`, mais elle met en valeur
les erreurs si quelque chose se passe mal en ajoutant le nom du fichier au message

Exécuter du PHP dans les fichiers YAML
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    La méthode ``Yaml::enablePhpParsing()`` est une nouveauté de Symfony 2.1. Avant
    la version 2.1, le PHP était *toujours* exécuté lors d'un appel à la fonction ``parse()``.

Par défaut, si vous incluez du PHP dans un fichier YAML, il ne sera pas analysé.
Si vous voulez que le PHP soit analysé, vous devez appeler ``Yaml::enablePhpParsing()``
avant d'analyser le fichier pour activer ce mode. Si vous ne voulez autoriser le code
PHP que dans un seul fichier YAML, assurez vous de désactiver l'analyse du PHP après
avoir analysé votre fichier en appelant ``Yaml::$enablePhpParsing = false;`` (``$enablePhpParsing``
est une propriété publique).

Écrire des fichiers YAML
~~~~~~~~~~~~~~~~~~~~~~~~

La méthode :method:`Symfony\\Component\\Yaml\\Dumper::dump` affiche n'importe
quel tableau PHP en sa représentation YAML :

.. code-block:: php

    use Symfony\Component\Yaml\Dumper;

    $array = array('foo' => 'bar', 'bar' => array('foo' => 'bar', 'bar' => 'baz'));

    $dumper = new Dumper();

    $yaml = $dumper->dump($array);

    file_put_contents('/path/to/file.yml', $yaml);

.. note::

    Bien sûr, le « dumper » YAML de Symfony2 n'est pas capable d'afficher
    des ressources. Aussi, même si le « dumper » est capable d'afficher des
    objets PHP, cela est considéré comme une fonctionnalité non supportée.

Si une erreur intervient durant l'affichage, l'analyseur lance une exception
:class:`Symfony\\Component\\Yaml\\Exception\\DumpException`.

Si vous avez seulement besoin d'afficher un tableau, vous pouvez utiliser
le raccourci de la méthode statique :method:`Symfony\\Component\\Yaml\\Yaml::dump` :

.. code-block:: php

    use Symfony\Component\Yaml\Yaml;

    $yaml = Yaml::dump($array, $inline);

Le format YAML supporte deux sortes de représentation pour les tableaux : celle
étendue, et celle « en ligne » (« inline » en anglais). Par défaut, l'afficheur
utilise la représentation « en ligne » :

.. code-block:: yaml

    { foo: bar, bar: { foo: bar, bar: baz } }

Le second argument de la méthode :method:`Symfony\\Component\\Yaml\\Dumper::dump`
personnalise le niveau à partir duquel le rendu passe de la représentation
étendue à celle « en ligne » :

.. code-block:: php

    echo $dumper->dump($array, 1);

.. code-block:: yaml

    foo: bar
    bar: { foo: bar, bar: baz }

.. code-block:: php

    echo $dumper->dump($array, 2);

.. code-block:: yaml

    foo: bar
    bar:
        foo: bar
        bar: baz

Le Format YAML
--------------

Conformément au site web officiel `YAML`_, YAML est « un standard de
sérialisation de données compréhensible facilement par l'humain pour
tous les langages de programmation ».

Même si le format YAML peut décrire une structure de données imbriquée
complexe, ce chapitre décrit uniquement l'ensemble de fonctionnalités
minimal nécessaire pour utiliser YAML en tant que format de fichier de
configuration.

YAML est un langage simple qui décrit des données. Comme PHP, il possède une
syntaxe pour les types simples comme les chaînes de caractères, les booléens,
les nombres à virgule flottante, ou les entiers. Mais contrairement à PHP, il
fait une différence entre les tableaux (séquences) et les « hashes »
(correspondances clé-valeur).

Scalaire
~~~~~~~~

La syntaxe pour les scalaires est similaire à la syntaxe PHP.

Chaînes de caractères
.....................

.. code-block:: yaml

    Une chaîne de caractères en YAML

.. code-block:: yaml

    'Une chaîne de caractères entourée par des guillemets simples en YAML'

.. tip::

    Dans une chaîne de caractères entourée par des guillemets simples, un guillemet
    simple ``'`` doit être doublé :

    .. code-block:: yaml

        'Un guillemet simple '' dans une chaîne de caractères entourée par des guillemets simples'

.. code-block:: yaml

    "Une chaîne de caractères entourée par des guillemets doubles en YAML\n"

Les styles de citation avec guillemets sont utiles lorsqu'une chaîne de caractères commence
ou se termine avec un ou plusieurs espaces significatifs.

.. tip::

    Le style de citation avec des guillemets doubles fournit une manière
    d'exprimer des chaînes de caractères arbitraires, en utilisant des
    séquences d'échappement ``\``. Ceci est très utile lorsque vous avez
    besoin d'intégrer un ``\n`` ou un caractère unicode dans une chaîne de
    caractères.

Lorsqu'une chaîne de caractères contient un retour à la ligne, vous pouvez
utiliser le style littéral, indiqué par un séparateur vertical (``|``), pour
indiquer que la chaîne de caractères va s'étendre sur plusieurs lignes. Avec
le style de citation littéral, les nouvelles lignes sont préservées :

.. code-block:: yaml

    |
      \/ /| |\/| |
      / / | |  | |__

Autrement, les chaînes de caractères peuvent être écrites avec le style de citation
dit « plié », indiqué par le caractère ``>``, où chaque retour à la ligne est
remplacé par un espace :

.. code-block:: yaml

    >
      This is a very long sentence
      that spans several lines in the YAML
      but which will be rendered as a string
      without carriage returns.

.. note::

    Notez les deux espaces avant chaque ligne dans l'exemple précédent.
    Ils ne vont pas apparaître dans les chaînes de caractères PHP résultantes.

Nombres
.......

.. code-block:: yaml

    # un entier
    12

.. code-block:: yaml

    # un octal
    014

.. code-block:: yaml

    # un hexadécimal
    0xC

.. code-block:: yaml

    # un nombre à virgule flottante
    13.4

.. code-block:: yaml

    # un nombre exponentiel
    1.2e+34

.. code-block:: yaml

    # l'infini
    .inf

Nuls
....

Les valeurs nulles en YAML peuvent être exprimées grâce à
``null`` ou à ``~``.

Booléens
........

Les booléens en YAML sont exprimés via ``true`` et ``false``.

Dates
.....

YAML utilise le standard ISO-8601 pour exprimer les dates :

.. code-block:: yaml

    2001-12-14t21:59:43.10-05:00

.. code-block:: yaml

    # une date simple
    2002-12-14

Collections
~~~~~~~~~~~

Un fichier YAML est rarement utilisé pour décrire un simple scalaire. La
plupart du temps, il décrit une collection. Une collection peut être une
séquence ou une correspondance d'éléments. Les deux sont converties en
tableaux PHP.

Les séquences utilisent un tiret suivi d'un espace :

.. code-block:: yaml

    - PHP
    - Perl
    - Python

Le fichier YAML précédent est équivalent au code PHP suivant :

.. code-block:: php

    array('PHP', 'Perl', 'Python');

Les correspondances utilisent un deux-points (``:`` ) suivi d'un espace pour
marquer chaque paire clé/valeur :

.. code-block:: yaml

    PHP: 5.2
    MySQL: 5.1
    Apache: 2.2.20

qui est équivalent au code PHP suivant :

.. code-block:: php

    array('PHP' => 5.2, 'MySQL' => 5.1, 'Apache' => '2.2.20');

.. note::

    Dans une correspondance, une clé peut être n'importe quel scalaire valide.

Le nombre d'espaces entre les deux-points et la valeur n'a pas d'importance :

.. code-block:: yaml

    PHP:    5.2
    MySQL:  5.1
    Apache: 2.2.20

YAML utilise l'indentation avec un ou plusieurs espaces pour décrire les
collections imbriquées :

.. code-block:: yaml

    "symfony 1.0":
      PHP:    5.0
      Propel: 1.2
    "symfony 1.2":
      PHP:    5.2
      Propel: 1.3

Le YAML précédent est équivalent au code PHP suivant :

.. code-block:: php

    array(
      'symfony 1.0' => array(
        'PHP'    => 5.0,
        'Propel' => 1.2,
      ),
      'symfony 1.2' => array(
        'PHP'    => 5.2,
        'Propel' => 1.3,
      ),
    );

Il y a une chose importante que vous devez vous rappeler lorsque vous utilisez
l'indentation dans un fichier YAML : *L'indentation doit être faite avec un ou
plusieurs espaces, mais jamais avec des tabulations*.

Vous pouvez imbriquer des séquences et des correspondances comme vous le voulez :

.. code-block:: yaml

    'Chapter 1':
      - Introduction
      - Event Types
    'Chapter 2':
      - Introduction
      - Helpers

YAML peut aussi utiliser les styles dit de « flot » pour les collections,
en utilisant des indicateurs explicites plutôt que l'indentation pour
représenter la portée.

Une séquence peut être écrite comme une liste séparée par des virgules, le
tout entre crochets (``[]``) :

.. code-block:: yaml

    [PHP, Perl, Python]

Une correspondance peut être écrite comme une liste de clés/valeurs séparée
par des virgules, le tout entre accolades (``{}``) :

.. code-block:: yaml

    { PHP: 5.2, MySQL: 5.1, Apache: 2.2.20 }

Vous pouvez mélanger et faire correspondre les styles afin d'obtenir une
meilleure lisibilité :

.. code-block:: yaml

    'Chapter 1': [Introduction, Event Types]
    'Chapter 2': [Introduction, Helpers]

.. code-block:: yaml

    "symfony 1.0": { PHP: 5.0, Propel: 1.2 }
    "symfony 1.2": { PHP: 5.2, Propel: 1.3 }

Commentaires
~~~~~~~~~~~~

Les commentaires peuvent être ajoutés en YAML en les préfixant avec un
symbole dièse (``#``) :

.. code-block:: yaml

    # commentaire sur une ligne
    "symfony 1.0": { PHP: 5.0, Propel: 1.2 } # commentaire à la fin d'une ligne
    "symfony 1.2": { PHP: 5.2, Propel: 1.3 }

.. note::

    Les commentaires sont simplement ignorés par l'analyseur YAML et ne
    doivent pas être indentés par rapport au niveau courant d'imbrication
    dans une collection.

.. _YAML: http://yaml.org/
.. _Packagist: https://packagist.org/packages/symfony/yaml