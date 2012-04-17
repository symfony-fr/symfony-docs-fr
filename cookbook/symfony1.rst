En quoi Symfony2 diffère de Symfony1
==================================

Le framework Symfony2 correspond à une évolution majeure si on le compare à la
première version du framework. Heureusement, comme il est basé sur l'architecture MVC,
les qualités utilisées pour maîtriser un projet symfony1 continuent d'être
pertinentes pour développer avec Symfony2. Bien entendu, ``app.yml`` 
n'existe plus mais le routage, les contrôleurs et les templates sont toujours
présents.

Dans ce chapitre, nous allons parcourir les différences entre symfony1 et
Symfony2. Comme vous allez le voir, de nombreuses opérations sont effectuées de
manière légèrement différente. Vous apprécierez ces différences mineures car
elles permettent des comportements stables, sans surprises, testables et une
séparation claire des logiques utilisées au sein de vos applications Symfony2.

Ainsi, asseyez vous et relaxez vous afin que nous puissions vous transporter du « passé »
au « présent ».

Arborescence des répertoires
----------------------------

Quand vous observez un projet Symfony2 - par exemple, l'édition
`Standard Symfony2`_ - vous pouvez noter une structure de répertoire très
différente de celle présente dans symfony1. Les differences sont cependant 
superficielles.

Le dossier ``app/``
~~~~~~~~~~~~~~~~~~~

Dans symfony1, vos projets avaient une ou plusieurs applications, et chacune
se situait dans le répertoire ``apps/`` (ex. ``apps/frontend``). Par
défaut, dans Symfony2, vous avez une seule application représentée par le 
dossier ``app/``. Comme dans symfony1, le dossier ``app/`` contient une 
configuration spécifique à l'application. Il contient aussi le cache, les logs et
les templates spécifiques à l'application ainsi qu'une classe ``Kernel`` (``AppKernel``),
qui est l'objet de base représentant l'application.

A la différence de symfony1, très peu de code PHP se trouve dans le dossier
``app/``. Ce répertoire n'est pas destiné à contenir les modules maison ou les 
fichiers des bibliothèques comme il le faisait dans symfony1. Il correspond au
répertoire où se situent les fichiers de configuration et autres ressources générales
(templates, fichiers de traduction).

Le dossier ``src/``
~~~~~~~~~~~~~~~~~~~

En résumé, votre code se trouve ici. Dans Symfony2, tout le code applicatif 
se trouve dans un bundle (plus ou moins équivalent aux plugins de symfony1) et, par 
défaut, chaque bundle se place dans le dossier ``src``. Sur cet aspect, le
répertoire ``src`` est un peu comme le dossier ``plugins`` dans symfony1, tout
en comportant beaucoup plus de flexibilité. De plus, pendant que *vos bundles*
sont dans le répertoire ``src/``, les bundles de bibliothèques tierces se situeront
dans le répertoire ``vendor/``.

Afin d'avoir une idée plus précise du répertoire ``src/`` , pensez d'abord à une
application symfony1. Premièrement, certaines parties de votre code sont
situées dans une ou  plusieurs applications. Ces applications incluent, le plus souvent,
des modules mais peuvent également inclure n'importe quelle classe PHP. Vous
avez probablement créé un fichier ``schema.yml`` dans le répertoire ``config`` de votre
projet et généré de nombreux fichiers de modèles. Finalement, afin d'utiliser
certaines fonctionnalités communes, vous avez utilisé de nombreuses
bibliothèques externes présentes dans le répertoire ``plugins/``. En d'autres
termes, le code qui fait fonctionner votre application se trouve à plusieurs endroits.

Dans Symfony2, la vie est plus simple car *tout* le code Symfony2 doit être
placé dans un bundle. Dans les projets symfony1, tout le code applicatif
*pourrait* être déplacé dans un ou plusieurs plugins (ce qui est une bonne
pratique en fait). Supposons que tous les modules, les classes PHP, les schémas, les
configurations des routes, etc... soient déplacées dans un plugin, alors le
dossier symfony1 ``plugins/`` serait très similaire au dossier Symfony2
``src/``.

Résumons de manière simple, le dossier ``src/`` est l'endroit où placer tout le
code, les ressources, les templates et tous les outils spécifiques à votre projet.

Le dossier ``vendor/``
~~~~~~~~~~~~~~~~~~~~~~

Le dossier ``vendor/`` est grossièrement l'équivalent du dossier ``lib/vendor/``
présent dans symfony1, qui était le dossier conventionnel pour toutes les 
bibliothèques et les bundles externes. Par défaut, vous trouverez les fichiers
de la bibliothèque Symfony2 dans ce répertoire, ainsi que de nombreuses autres
bibliothèques comme Doctrine2, Twig et Swiftmailer. Les bundles tierces
intégrés à Symfony2 se situent quelque part dans le répertoire ``vendor/``.

Le dossier ``web/``
~~~~~~~~~~~~~~~~~~~

Peu de choses ont changé dans le dossier ``web/``. Les différences les plus
notables sont l'absence des dossiers ``css/``, ``js/`` et ``images/``. C'est
intentionnel. Tout comme le code PHP, les ressources devraient également se
placer dans un bundle. Avec l'aide des commandes de la console, le
dossier ``Resources/public/`` de chaque bundle est copié ou symboliquement lié
(ln -s) au dossier ``web/bundles/``. Cela permet de conserver vos ressources
organisées dans votre bundle, tout en permettant de les rendre publiques. Afin
de vous assurer que tous les bundles soient disponibles, lancez la commande suivante::

    php app/console assets:install web

.. note::

   Cette commande est l'équivalent Symfony2 de la commande symfony1
   ``plugin:publish-assets``.

Auto-chargement
---------------

Un des avantages d'un framework moderne est de ne jamais s'occuper des imports
de fichiers. En utilisant un autoloader (chargeur automatique), vous pouvez
faire référence à n'importe quelle classe de votre project et ainsi être certain
qu'elle est disponible. L'autoloader a changé dans Symfony2 afin d'être plus
universel, plus rapide, et moins dépendant du nettoyage du cache.

Dans symfony1, le chargement automatique était réalisé en recherchant, dans tout le
projet, la présence de classes PHP et en mettant en cache cette information dans un
gigantesque tableau. Ce tableau disait à symfony1 les correspondances exactes entre les
fichiers et les classes. Dans l'environnement de production, cela impliquait de nettoyer le
cache lorsque des classes étaient ajoutées ou déplacées.

Dans Symfony2, une nouvelle classe - ``UniversalClassLoader`` - effectue ce
travail. L'idée derrière l'autoloadeur est simple : le nom de votre classe
(incluant l'espace de nom) doit correspondre avec le chemin du fichier contenant
la classe. Prenez le ``FrameworkExtraBundle`` faisant partie de l'édition
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
la classe. Plus précisemment, l'espace de nom ``Sensio\Bundle\FrameworkExtraBundle``,
correspond au répertoire où le fichier doit être trouvé
(``vendor/sensio/framework-extra-bundle/Sensio/Bundle/FrameworkExtraBundle/``).
Cela s'explique par le fait que dans le fichier ``app/autoload.php``, vous avec
configuré Symfony pour qu'il recherche l'espace de nom ``Sensio`` dans le répertoire
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
correpondent pas. Plus simplement, Symfony2 recherche cette classe dans à un
emplacement précis, mais cet emplacement n'existe pas (ou contient une classe
différente). Pour qu'une classe soit chargée automatiquement, vous **n'avez
jamais besoin de nettoyer le cache** dans Symfony2.

Comme mentionné précédemment, pour que le chaargement automatique fontionne, il
a besoin de savoir que l'espace de nom ``Sensio`` se trouve dans le dossier
``vendor/bundles`` et que, par exemple, l'espace de nom ``Doctrine`` se trouve
dans le dossier ``vendor/doctrine/orm/lib/``. Cette association est entièrement
sous votre contrôle via le fichier ``app/autoload.php``.

Si vous observez le contrôleur ``HelloController`` de l'édition Standard de
Symfony2 vous remarquerez qu'il est placé dans l'espace de nom
``Acme\DemoBundle\Controller``. Cependant, l'espace de nom ``Acme`` n'est pas
défini dans le fichier ``app/autoload.php``. En effet, par défaut vous n'avez
pas à définir explicitement l'emplacement de vos bundles présents à l'intérieur
du répertoire ``src/``. L'``UniversalClassLoader`` est configuré pour rechercher
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

Dans un projet symfony1, il est commun d'avoir plusieurs applications : une 
pour la partie front(frontend) et une pour la partie administrative (backend)
par exemple.

Dans un projet Symfony2, vous n'avez besoin de créer qu'une application (un blog, une
application intranet, ...). Le plus souvent, si vous voulez créer une seconde application,
vous devriez plutôt créer un autre projet et partager certains bundles entre eux.

Et si vous avez besoin de séparer la partie frontend de la partie backend de certains 
bundles, vous pouvez créer des sous-espaces de noms pour les contrôleurs, des 
sous-répertoires pour les templates, différentes configurations sémantiques,
séparer les configurations de routages, et bien plus encore.

Bien sur, il n'y a rien de mal à avoir plusieurs applications dans votre
projet, c'est à vous de décider. Une deuxième application impliquerait un
nouveau répertoire, par exemple ``my_app/``, avec la même configuration que le
répertoire ``app/``.

.. tip::

    Vous pouvez lire à ce sujet la définition des termes :term:`Projet`,
    :term:`Application`, et :term:`Bundle` dans le glossaire.

Bundles et Plugins
------------------

Dans un projet symfony1, un plugin pouvait contenir de la configuration, des
modules, des bibliothèques PHP, des ressources ou tout autre fichier en relation
avec votre projet. Dans Symfony2, l'idée de plugin est remplacée par celle de
« bundle ». Un bundle est encore plus puissant qu'un plugin, la preuve le coeur
du framework Symfony2 est composé d'une série de bundles. Dans Symfony2, les
bundles sont les citoyens de première classe si flexibles que même le coeur de
Symfony2 est lui-même un bundle.

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
étaient automatiquement chargés depuis un plugin. Dans Symfony2, les
configurations de routages et d'applications inclues dans un bundle doivent
être chargées manuellement. Par exemple, pour inclure un fichier de routage à partir
d'un bundle appelé ``AcmeDemoBundle``, vous devez faire::

    # app/config/routing.yml
    _hello:
        resource: "@AcmeDemoBundle/Resources/config/routing.yml"

Cela chargera automatiquement les routes trouvées dans le fichier
``Resources/config/routing.yml`` du bundle ``AcmeDemoBundle``. La convention
`@AcmeDemoBundle`` est un raccourci qui, en interne, est remplacé par le chemin
complet du bundle.

Vous pouvez utiliser la même stratégie pour charger une configuration provenant
d'un bundle:

.. code-block:: yaml

    # app/config/config.yml
    imports:
        - { resource: "@AcmeDemoBundle/Resources/config/config.yml" }

Dans Symfony2, la configuration ressemble au ``app.yml`` présent dans symfony1,
excepté qu'elle est mieux encadrée. Dans ``app.yml``, vous pouviez créer toutes
les clefs dont vous aviez besoin. Par défaut, ces entrées étaient dénuées de sens
et dépendaient entièrement de comment vous les utilisiez dans votre application :

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

Vous pouvez maintenant accèder à cette valeur depuis votre contrôleur, par
exemple::

    public function helloAction($name)
    {
        $fromAddress = $this->container->getParameter('email.from_address');
    }

En réalité, la configuration de Symfony2 est beaucoup plus puissante et est utilisée
principalement pour configurer les objets que vous pouvez utiliser. Pour plus 
d'informations, consultez le chapitre intitulé « :doc:`/book/service_container` ».

.. _`Standard Symfony2`: https://github.com/symfony/symfony-standard