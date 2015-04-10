.. index::
   single: Components; Installation
   single: Components; Usage

Comment installer et utiliser les composants de Symfony2
========================================================

Si vous commencez un nouveau projet (ou si vous en avez déjà un) qui doit
 utiliser un ou plusieurs composants, la manière la plus facile pour intégrer
 cela est `Composer`_. Composer est suffisamment intelligent pour télécharger
 le ou les composants dont vous avez besoin et prendre en charge l'autoloading,
 vous n'avez plus qu'à commencer à utiliser tout de suite ces librairies.

Cet article va vous guider à traver la documentation : :doc:`/components/finder`.
Cela pourra être utilisé pour tout autre composant.


Utiliser le composant Finder
----------------------------
**1.** Si vous créez un nouveau projet, créez le avec un répertoire vide.

**2.** Créé un fichier appelé ``composer.json`` et coller ce qui suit:

.. code-block:: json

    {
        "require": {
            "symfony/finder": "2.3.*"
        }
    }

Si vous avez déjà un fichier ``composer.json``, ajoutez juste les lignes ci-dessus.
Vous aurez peut-être besoin de modifié la version (exp. ``2.2.2`` ou ``2.3.*``).

Vous pouvez rechercher les noms et versions des composants à `packagist.org`_.

**3.** `Installez composer`_ s'il n'est pas déjà présent sur votre système:

**4.** Téléchargez les librairies tierces et générez fichier ``vendor/autoload.php``:

.. code-block:: bash

    $ php composer.phar install

**5.** Ecrivez votre code:

Une fois que Composer a téléchargé le ou les composants, tout ce dont vous avez
besoin est inclut dans le fichier ``vendor/autoload.php`` qui a été généré par
Composer. Ce fichier prend en compte l'autoloading pour toutes les librairies,
donc vous pouvez directement les utiliser::

        // Fichier: src/script.php

        // mettez jour ce chemin le répertoire "vendor/" en relation la position
        // de ce fichier
        require_once '../vendor/autoload.php';

        use Symfony\Component\Finder\Finder;

        $finder = new Finder();
        $finder->in('../data/');

        // ...

.. tip::

    Si vous voulez utiliser tous les composants Symfony2, au lieu de les ajouter
    tous un par un:

        .. code-block:: json

            {
                "require": {
                    "symfony/finder": "2.3.*",
                    "symfony/dom-crawler": "2.3.*",
                    "symfony/css-selector": "2.3.*"
                }
            }

    Vous pouvez faire ceci:

        .. code-block:: json

            {
                "require": {
                    "symfony/symfony": "2.3.*"
                }
            }

    Cela inclue les librairies Bundle et Bridge, que vous n'avez pas encore besoin.

Et maintenant ?
---------------

Le composant est installé et ``autochargé``, lisez la documentation du composant
pour en savoir sur comment l'utiliser.

Et maintenant, amusez vous bien !

.. _Composer: http://getcomposer.org
.. _Installez composer: http://getcomposer.org/download/
.. _packagist.org: https://packagist.org/
