Gérer les bibliothèques vendors avec bin/vendors et deps
-----------------------------------------------------

### Comment ça marche?

Chaque projet Symfony utilise des bibliothèques tiers regroupées sous l'appellation
"vendors". D'une façon ou d'une autre, le but est de vous permettre de télécharger
ces bibliothèques dans le répertoire  ``vendor/`` et de pouvoir en obtenir la version
désirée pour chacune.

Par défaut, ces bibliothèques sont téléchargées en exécutant le script de téléchargement
``php bin/vendors install``. Ce script utilise le fichier `deps`` situé à la racine
de votre projet. Ce fichier au format INI contient la liste des bibliothèques nécessaires
à votre projet, ainsi que le répertoire cible où chacune doit être téléchargée,
et (de manière optionnelle) la version à télécharger. Le script `bin/vendors``
utilise ``git`` pour ce téléchargement, car la plupart des bibliothèques
sont gérées et disponibles via git. Le script ``bin/vendors`` se base également sur
le fichier ``deps.lock``, lequel vous permet d'arrêter la version à une révision précise.

Nota bene: ces bibliothèques "vendor" *ne sont pas* gérées dans votre *dépôt*;
ce sont de simples fichiers installés (et ignorés du dépôt git) dans le répertoire
``vendor/`` par le script ``bin/vendors``. Or, puisque toute l'information nécessaire
pour télécharger ces fichiers est disponible dans ``deps`` et ``deps.lock``
(qui, eux, *sont* gérés par votre dépôt), tout autre développeur peut utiliser votre
projet, lancer ``php bin/vendors install``, et obtenir les mêmes bibliothèques (et version).
Vous contrôlez donc exactement le contenu de chaque bibliothèque, sans avoir besoin
de le versionner dans votre dépôt.

Ainsi, lorsqu'un développeur veut travailler sur votre projet, il lui suffit de
lancer le script ``php bin/vendors install`` pour s'assurer que toutes les bibliothèques
nécessaires sont téléchargées.

.. sidebar:: Mettre à jour Symfony

    Puisque Symfony est une bibliothèque tiers de votre projet et qu'elle est
    donc gérée entièrement au travers de ``deps`` et``deps.lock``,
    mettre à jour Symfony signifie simplement mettre à jour
    ces deux fichiers afin qu'ils correspondent à la dernière version
    de l'édition standard de Symfony.

    Bien sûr, si vous avez ajouté des entrées aux fichiers ``deps`` ou ``deps.lock``,
    veillez à ne remplacer que les parties originales afin de ne pas supprimer les entrées
    supplémentaires.

.. ::

    La command ``php bin/vendors update`` existe mais n'est pas utilisée pour la mise
    à jour de votre projet. Vous ne l'utiliserez en fait pratiquement jamais.
    Cette commande permet de bloquer la version de toutes vos bibliothèques vendors
    en les mettant à jour selon la version spécifiée dans ``deps`` et en inscrivant la révision
    correspondante dans le fichier `deps.lock``.

### Hacker les bibliothèques vendor

Parfois, il est nécessaire de télécharger une version précise (branche, tag) d'une bibliothèque.
Vous pouvez le faire directement dans le fichier ``deps`` :

.. code-block:: text

    [AcmeAwesomeBundle]
        git=http://github.com/johndoe/Acme/AwesomeBundle.git
        target=/bundles/Acme/AwesomeBundle
        version=ma-version-trop-bien

* L'option ``git`` définit l'URL de la bibliothèque. Elle peut utiliser divers protocoles,
  comme ``http://`` ou ``git://``.

* L'option ``target`` définit le répertoire cible, le répertoire dans lequel la bibliothèque va être téléchargée.
  Par convention, les bundles devraient aller dans le répertoire ``vendor/bundles/Acme``, les autres
  bibliothèques iront dans ``vendor/ma-biblio-trop-bien``, le répertoire ``vendor`` est
  également le répertoire cible par défaut.

* L'option ``version`` vous permet de définir une révision spécifique. Vous pouvez lui attribuer
  un nom de tag (``version=origin/0.42``) ou de branche (``refs/remotes/origin/ma-branche-trop-bien``).
  Version par défaut: ``origin/HEAD``.

### Flux de mise à jour

lors de l'éxécution de ``php bin/vendors install``, le script vérifie d'abord si le répertoire cible
existe pour chaque bibliothèque. Si ce n'est pas le cas, il effectue un ``git clone``.

Puis, il effectue un ``git fetch origin``, puis ``git reset --hard ma-version-trop-bien``.

Le répertoire sera donc cloné une seule et unique fois. Pour faire des changements liés au dépôt distant
il est nécessaire d'effacer le répertoire cible lui-même, et non seulement son contenu.
