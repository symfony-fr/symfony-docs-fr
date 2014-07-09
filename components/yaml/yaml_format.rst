.. index::
    single: Yaml; YAML Format

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
~~~~~~~~~~~~~~~~~~~~~

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

Les guillemets doubles sont utiles lorsqu'une chaîne de caractères commence
ou se termine avec un ou plusieurs espaces significatifs.

.. tip::

    Le style avec doubles guillemets fournit une manière
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
      Voici une très longue phrase qui
      est découpée en plusieurs lignes en YAML
      mais qui sera affichée comme une chaine de
      caractères sans retours à la ligne.

.. note::

    Notez les deux espaces avant chaque ligne dans l'exemple précédent.
    Ils ne vont pas apparaître dans les chaînes de caractères PHP résultantes.

Nombres
~~~~~~~

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
~~~~

Les valeurs nulles en YAML peuvent être exprimées grâce à
``null`` ou à ``~``.

Booléens
~~~~~~~~

Les booléens en YAML sont exprimés via ``true`` et ``false``.

Dates
~~~~~

YAML utilise le standard ISO-8601 pour exprimer les dates :

.. code-block:: yaml

    2001-12-14t21:59:43.10-05:00

.. code-block:: yaml

    # une date simple
    2002-12-14

Collections
-----------

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
------------

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