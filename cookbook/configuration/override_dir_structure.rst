.. index::
   single: Override Symfony

Comment surcharger la structure de répertoires par défaut de Symfony
====================================================================

Symfony est fourni automatiquement avec une structure de répertoires par
défaut. Vous pouvez facilement surcharger cette structure de répertoires
et créer la vôtre. La structure par défaut est la suivante :

.. code-block:: text

    app/
        cache/
        config/
        logs/
        ...
    src/
        ...
    vendor/
        ...
    web/
        app.php
        ...

Surcharger le répertoire ``cache``
----------------------------------

Vous pouvez surcharger le répertoire de cache en surchargeant la méthode
``getCacheDir`` dans la classe ``AppKernel`` de votre application::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function getCacheDir()
        {
            return $this->rootDir.'/'.$this->environment.'/cache/';
        }
    }

``$this->rootDir`` est le chemin absolu vers le répertoire ``app`` et
``$this->environment`` est l'environnement actuel (c-a-d ``dev``). Dans
ce cas, vous avez changé l'emplacement du répertoire cache pour qu'il
devienne ``app/{environment}/cache``.

.. caution::

    Vous devriez avoir un répertoire ``cache`` différent pour chaque environnement,
    sinon certains effets de bord pourraient survenir. Chaque environnement génère
    ses propres fichiers de configuration en cache, et donc, chacun a besoin de son
    propre répertoire pour stocker ces fichiers.

Surcharger le répertoire ``logs``
---------------------------------

Pour surcharger le répertoire ``logs``, procédez de la même manière que pour
le répertoire ``cache``. La seule différence est que vous devez surcharger la
méthode ``getLogDir``::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function getLogDir()
        {
            return $this->rootDir.'/'.$this->environment.'/logs/';
        }
    }

Ici, vous avez changé l'emplacement du répertoire pour ``app/{environment}/logs``.

Surcharger le répertoire ``web``
--------------------------------

Si vous avez besoin de renommer ou de déplacer le répertoire ``web``, la seule
chose dont vous devez vous assurer et que le chemin vers le répertoire ``app``
est toujours correct dans vos contrôleurs frontaux ``app.php`` et ``app_dev.php``.
Si vous renommez simplement le répertoire, alors tout est déjà bon. Mais si vous
le déplacez, vous aurez besoin de modifier les chemins dans ces fichiers::

    require_once __DIR__.'/../Symfony/app/bootstrap.php.cache';
    require_once __DIR__.'/../Symfony/app/AppKernel.php';

.. tip::

    Certains serveurs mutualisés ont un répertoire racine ``public_html``.
    Renommer votre répertoire de ``web`` pour ``public_html`` est une manière
    de faire fonctionner votre projet Symfony sur votre serveur mutualisé. Une
    autre manière serait déployer votre application en dehors de la racine web,
    supprimer le répertoire ``public_html`` puis le remplacer par une lien symbolique
    vers le répertoire ``web`` de votre projet.

.. note::
   
    Si vous utilisez le bundle AsseticBundle, vous devrez le configurer pour qu'il
    utilise le bon répertoire ``web`` :

    .. code-block:: yaml

        # app/config/config.yml

        # ...
        assetic:
            # ...
            read_from: "%kernel.root_dir%/../../public_html"

    Maintenant, vous devez juste exporter vos ressources pour que votre application puisse
    fonctionner :

    .. code-block:: bash

        $ php app/console assetic:dump --env=prod --no-debug
