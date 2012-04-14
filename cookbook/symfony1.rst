En quoi Symfony2 diffère de Symfony1
==================================

Le framework Symfony2 correspond à une évolution majeure si on le compare à la
première version du framework. Heureusement, intégrant l'architecture MVC en son
sein, les qualités utilisées pour manipuler un projet symfony1 continuent d'être
pertinentes dans le développement dans Symfony2. Bien entendu, ``app.yml`` 
n'existe plus, mais le routage, les contrôleurs et les templates sont toujours
présents.

Dans ce chapitre, nous allons parcourir les différences entre symfony1 et
Symfony2. Comme vous allez le voir, de nombreuses opérations sont effectuées de
manière légèrement différentes. Vous apprécierez ces différences mineures car
elles permettent des comportement stables, prédictibles, testables et une
séparation claire des logiques utilisées au sein de vos applications Symfony2.

Ainsi, asseyez vous et relaxez afin que nous puissions vous transporter du passé
au présent.

Arborescence des répertoires
----------------------------

Quand vous observez un projet Symfony2 - par exemple, l'édition
`Symfony2 Standard`_ - vous pouvez noté une structure de répertoire très
différente de celle présente dans symfony1. Les differences sont cependant 
superficielles.

Le dossier ``app/``
~~~~~~~~~~~~~~~~~~~

Dans symfony1, vos projets avait une ou plusieurs applications, et chacunes
vivait à l'intérieur du répertoire ``apps/`` (ex. ``apps/frontend``). Par
défaut, dans Symfony2, vous avez une seule application représentée par le 
dossier ``app/``. Comme dans symfony1, le dossier ``app/`` contient une 
configuration spécifique à l'application. Il contient aussi caches, log et
templates ainsi qu'une classe ``Kernel`` (``AppKernel``), qui est l'objet de
base représentant l'application.

A la différence de symfony1, très peu de code PHP est présent dans le dossier
``app/``. Ce répertoire n'est pas destiné à contenir les modules maison ou les 
fichiers des bibliothèques comme il le faisait dans symfony1. Il correspond à la
résidence des fichiers de configuration et autres ressources générales
(templates, fichiers de tranduction).

Le dossier ``src/``
~~~~~~~~~~~~~~~~~~~

Placer simplement, votre code ici. Dans Symfony2, tous le code applicatif vit à
travers un bundle (un équivalent approchant des plugins dans symfony1) et, par 
défaut, chaque bundle se place dans le dossier ``src``. De cet façon, le
répertoire ``src`` est un peu comme le dossier ``plugins`` dans symfony1, tout
en comportant beaucoup plus de flexibilité. De plus, pendant que *vos bundles*
vivent dans le répertoire ``src/``, les bundles de bibliothèques tierces vivront
dans le répertoire ``vendor/``.

Afin d'avoir une idée plus précise du répertoire ``src/`` , pensez d'abord à une
application symfony1. Premièrement, certaines partie de votre code sont
utilisées à l'intérieur d'une ou de plusieurs applications. Les parties les plus
communes sont intégrées en tant que modules, ou en tant que bibliothèques. Vous
avez créé un fichier ``schema.yml`` dans le répertoire ``config`` de votre
projet et construit de nombreux fichiers de modèles. Finalement, afin de
permettre certaines fonctionnalités, vous avez utilisez de nombreuses
bibliothèques externes présentes dans le répertoire ``plugins/``. En d'autres
mots, le code qui conduit votre application est présent en de nombreux endroits.

Dans Symfony2, la vie est plus simple car *tout* le code Symfony2 doit être
placer dans un bundle. Dans les projets symfony1, tout le code applicatif
*pouvait* être déplacé dans un ou plusieurs plugins (Ce qui est une bonne
pratique). Supposons que tous les modules, les classes PHP, les schémas, les
configurations des routes, etc... soient déplacer dans un plugin, alors le
dossier symfony1 ``plugins/`` serait très similaire au dossier Symfony2
``src/``.

Résumons de manière simple, le dossier ``src/`` est l'endroit où placer tout le
code, les assets, les templates et tous les outils spécifiques à vos projets.

Le dossier ``vendor/``
~~~~~~~~~~~~~~~~~~~~~~

Le dossier ``vendor/`` est basiquement l'équivalent du dossier ``lib/vendor/``
présent dans symfony1, qui était le dossier conventionnel pour toutes les 
bibliothèques et les bundles externes. Par défaut, vous trouverez les fichiers
de la bibliothèque Symfony2 dans ce répertoire, ainsi que de nombreuses autres
bibliothèques comme Doctrine2, Twig et Swiftmailer. Les bundles de tierce partie
intégrés à Symfony2 sont quelque part dans le répertoire ``vendor/``.

Le dossier ``web/``
~~~~~~~~~~~~~~~~~~~

Peu de choses ont changé dans le dossier ``web/``. Les différences les plus
notables sont l'absence des dossiers ``css/``, ``js/`` et ``images/``. C'est
intentionnel. Comme tout le code PHP, tous les assets devrait eux aussi être
présente à l'intérieur d'un bundle. Avec l'aide des commandes de la console, le
dossier ``Resources/public/`` de chaque bundle est copié ou symboliquement lié
(ln -s) au dossier ``web/bundles/``. Cela permet de conserver ceux-ci organisés
au sein même de votre bundle, tout en permettant de les rendre disponibles
publiquement. Afin de vous assurer que tous les bundles sont disponibles lancez
la commande suivante::

    php app/console assets:install web

.. note::

   Cette commande est l'équivalent Symfony2 de la commande symfony1
   ``plugin:publish-assets``.

Auto-chargement
---------------

Un des avantages d'un framework moderne est de ne jamais s'occuper de l'appel du
chargement des fichiers.  Par l'utilisation d'un autoloader, vous pouvez
référencer toute classe de votre project et ainsi être certain qui soit mis à
disposition. L'auto-chargement a changé dans Symfony2 afin d'être plus
universel, plus rapide, et indépendant des besoins de clarification du cache.

Dans symfony1, l'auto-chargement était réalisé par la recherche dans le projet
entier de la présence d'un fichier particulier et par la mise en cache de cette
information dans un gigantesque tableau (array). Le tableau apprenait à symfony1
les correspondances exactes entre fichiers et classes. Dans un environnement de
production, cela implique de nettoyer le cache quand certaines classes sont
ajoutées ou déplacées.

Dans Symfony2, une nouvelle classe - ``UniversalClassLoader`` - effectue ce
travail. L'idée derrière l'autoloadeur est simple: le nom de votre classe
(incluant l'espace de nom) doit correspondre avec le chemin du fichier contenant
la classe. Prenez le ``FrameworkExtraBundle`` faisant parti de l'édition
standard Symfony2 comme exemple::

    namespace Sensio\Bundle\FrameworkExtraBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;
    // ...

    class SensioFrameworkExtraBundle extends Bundle
    {
        // ...

Le fichier lui même est présent dans 
``vendor/sensio/framework-extra-bundle/Sensio/Bundle/FrameworkExtraBundle/SensioFrameworkExtraBundle.php``.
Comme vous pouvez le voir, l'emplacement de ce fichier suit l'espace de nom de
la classe. Plus précisemment, l'espace de nom,
``Sensio\Bundle\FrameworkExtraBundle``, correspond au répertoire où le fichier
doit être trouvé
(``vendor/sensio/framework-extra-bundle/Sensio/Bundle/FrameworkExtraBundle/``).
Cela s'explique, dans le fichier ``app/autoload.php``, vous configurer Symfony
pour qu'il recherche l'espace de nom ``Sensio`` dans le répertoire
``vendor/sensio``:

.. code-block:: php

    // app/autoload.php

    // ...
    $loader->registerNamespaces(array(
        // ...
        'Sensio'           => __DIR__.'/../vendor/sensio/framework-extra-bundle',
    ));

Si ce fichier ne se trouve *pas* à cette position exacte, vous recevrez une
erreur ``Class "Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBundle"
does not exist.``. Dans Symfony2, une erreur "class does not exist" implique que
l'espace de nom de la classe incriminée et son emplacement physique ne
correpondent pas. Basiquement, Symfony2 recherche dans un emplacement exact pour
cette classe, mais cet emplacement n'existe pas (ou contient une classe
différente). Pour qu'un classe soit charger automatiquement, vous **n'avez
jamais besoin de nettoyer le cache** dans Symfony2.

Comme mentionné précédemment, pour que l'autochargement fontionne, il est
indispensable que le système soient informer sur l'emplacement de l'espace de
nom ainsi l'espace de nom ``Sensio`` se trouve dans le dossier
``vendor/bundles`` et l'espace de nom ``Doctrine`` est placé dans le répertoire
``vendor/doctrine/orm/lib/``. Cette cartograpie est entièrement sous votre
contrôle via le fichier ``app/autoload.php``.

Si vous observez le contrôleur ``HelloController`` de l'édition standard de
Symfony2 vous remarquerez qu'il est placé dans l'espace de nom
``Acme\DemoBundle\Controller``. Cependant, l'espace de nom ``Acme`` n'est pas
défini dans le fichier ``app/autoload.php``. En effet, par défaut vous n'avez
pas à définir explicitement l'emplacement de vos bundles présents à l'intérieur
du répertoire ``src/``. L'``UniversalClassLoader`` est configuré pour recherché
par défaut dans le répertoire ``src/`` en utilisant la méthode
``registerNamespaceFallbacks``:

.. code-block:: php

    // app/autoload.php

    // ...
    $loader->registerNamespaceFallbacks(array(
        __DIR__.'/../src',
    ));

Utilisation de la console
-------------------------

Dans symfony1, la console est dans le répertoire racine de votre projet et est 
appelée ``symfony``:

.. code-block:: text

    php symfony

Dans Symfony2, la console est maintenant dans le sous-dossier app et est appelée
``console``:

.. code-block:: text

    php app/console

Applications
------------

Dans une projet symfony1, il est commun d'avoir plusieurs applications: une 
pour la partie frontal(backend) et une pour la partie administrative (backend).

Dans un projet Symfony2, vous ne créez qu'une application (un blog, une
application intranet, ...). Si vous voulez créer une seconde application, vous
devez créer un autre projet et partager certains bundles entre eux.

Et si vous avez besoin de séparer le frontal de l'administratif de certains 
bundles, vous pouvez créer des espaces de noms inclus pour les contrôleurs, des 
répertoires internes pour les templates, différentes configurations sémantiques,
séparer les configurations de routages, et bien plus encore.

Bien sur, il n'y a rien de faux à avoir de multiples application dans votre
projet, c'est à vous de décider. Une deuxième application voudrait dire un
nouveau répertoire, ex. ``my_app/``, avec la même configuration que le
répertoire ``app/``.

.. tip::

    Vous pouvez lire à ce sujet la définition des termes :term:`Project`,
	:term:`Application`, et :term:`Bundle` dans le glossaire.

Bundles et Plugins
------------------

Dans un projet symfony1, un plugin pouvait contenir configurations, modules, 
bibliothèques PHP, assets et tout autre fichier en relation avec votre projet.
Dans Symfony2, l'idée de plugin est remplacer par celle de "bundle". Un bundle
est plus puissant qu'un plugin, la preuve le coeur du framework Symfony2 est
composé de série de bundles. Dans Symfony2, les bundles sont les citoyens de
première classe si flexible que même le coeur de Symfony2 est lui-même un
bundle.

Dans symfony1, un plugin doit être activé à l'intérieur de la classe
``ProjectConfiguration``::

    // config/ProjectConfiguration.class.php
    public function setup()
    {
        $this->enableAllPluginsExcept(array(/* some plugins here */));
    }

Dans Symfony2, les bundles sont activés à l'intérieur du noyau applicatif::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            new Symfony\Bundle\FrameworkBundle\FrameworkBundle(),
            new Symfony\Bundle\TwigBundle\TwigBundle(),
            // ...
            new Acme\DemoBundle\AcmeDemoBundle(),
        );

        return $bundles;
    }

Routage (``routing.yml``) et Configuration (``config.yml``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans symfony1, les fichiers de configurations ``routing.yml`` et ``app.yml``
étaient automatiquement charger à l'intérieur des plugins. Dans Symfony2, les
configurations de routages et d'applications incluent dans un bundle doivent
être charger manuellement. Par exemple, pour inclure un ressource route à partir
d'un bundle appelé ``AcmeDemoBundle``, vous devez faire::

    # app/config/routing.yml
    _hello:
        resource: "@AcmeDemoBundle/Resources/config/routing.yml"

Cela chargera automatiquement les routes trouvés dans le fichier
``Resources/config/routing.yml`` du bundle ``AcmeDemoBundle``. La convention
`@AcmeDemoBundle`` est un raccourci qui, en interne, est résolu par le chemin
complet du bundle.

Vous pouvez utilisez la même stratégie pour charger une configuration provenant
d'un bundle:

.. code-block:: yaml

    # app/config/config.yml
    imports:
        - { resource: "@AcmeDemoBundle/Resources/config/config.yml" }

Dans Symfony2, la configuration ressemble au ``app.yml`` présent dans symfony1,
excepté qu'elle intègre maintenant un vocabulaire beaucoup conséquent. Avec
``app.yml``, vous pouvez créer les clefs dont vous avez besoin.
Par défaut, ces entrées sont dénués de sens et dépendent entièrement de comment
vous les utiliserez dans votre application:

.. code-block:: yaml

    # un fichier app.yml provenant de symfony1
    all:
      email:
        from_address:  foo.bar@example.com

Dans Symfony2, vous pouvez aussi créer des clefs arbitraires à l'intérieur de la
clef ``parameters`` de votre configuration:

.. code-block:: yaml

    parameters:
        email.from_address: foo.bar@example.com

Vous pouvez maintenant accèder à cette valeur depuis votre controller, par
example::

    public function helloAction($name)
    {
        $fromAddress = $this->container->getParameter('email.from_address');
    }

En réalité, la configuration de Symfony2 est beaucoup plus puissante et est utilisée
principalement pour configurer les objets que vous pouvez utiliser. Pour plus 
d'informations, consultez, le chapitre intitulé ":doc:`/book/service_container`".

.. _`Symfony2 Standard`: https://github.com/symfony/symfony-standard