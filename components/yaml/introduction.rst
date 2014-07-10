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

.. tip::

    Apprenez-en plus sur le composant Yaml en lisant l'article
    :doc:`/components/yaml/yaml_format`.

Installation
------------

Vous pouvez l'installer de deux manières différentes:

* :doc:`Installez le via Composer </components/using_components>` (``symfony/yaml`` on `Packagist`_);
* Utilisez le dépôt Git officiel (https://github.com/symfony/Yaml).


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

Le composant est aussi capable de dumper des tableaux PHP
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

    $yaml = Yaml::parse(file_get_contents('/path/to/file.yml'));

La méthode statique :method:`Symfony\\Component\\Yaml\\Yaml::parse` prend une
chaîne de caractères YAML ou un fichier contenant du YAML. En interne, elle appelle
la méthode :method:`Symfony\\Component\\Yaml\\Parser::parse`, mais elle met en valeur
les erreurs si quelque chose se passe mal en ajoutant le nom du fichier au message.

.. caution::

    Parce qu'il est maintenant possible de passer un nom de fichier à cette méthode,
    vous devez d'abord valider la saisie. Passer un nom de fichier déprécié dans
    Symfony 2.2, et sera supprimé dans Symfony 3.0.

Écrire des fichiers YAML
~~~~~~~~~~~~~~~~~~~~~~~~

La méthode :method:`Symfony\\Component\\Yaml\\Dumper::dump` affiche n'importe
quel tableau PHP en sa représentation YAML :

.. code-block:: php

    use Symfony\Component\Yaml\Dumper;

    $array = array(
        'foo' => 'bar',
        'bar' => array('foo' => 'bar', 'bar' => 'baz'),
    );

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
la méthode statique :method:`Symfony\\Component\\Yaml\\Yaml::dump` :

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

.. _YAML: http://yaml.org/
.. _Packagist: https://packagist.org/packages/symfony/yaml