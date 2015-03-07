.. index::
   pair: Autoloader; Configuration
   single: Components; ClassLoader

Le Composant ClassLoader
========================

    Le Composant ClassLoader charge les classes de votre projet automatiquement si
    elles suivent quelques conventions standards de PHP.

Chaque fois que vous utilisez une classe non-définie, PHP utilise le mécanisme
de chargement automatique (« Autoloading » en anglais) afin qu'il s'occupe
de charger un fichier définissant la classe. Symfony2 fournit un « autoloader
universel », qui est capable de charger des classes depuis des fichiers qui
implémentent l'une des conventions suivantes :

* Les `standards`_ d'intéropérabilité technique pour les espaces de nom de PHP
  5.3 et les noms de classe ;

* La convention de nommage `PEAR`_ pour les classes.

Si vos classes et les bibliothèques tierces que vous utilisez pour votre
projet suivent ces standards, l'« autoloader » de Symfony2 est l'unique
« autoloader » dont vous aurez besoin.

Installation
------------

Vous pouvez installer le composant de plusieurs manières :

* Utilisez le dépôt officiel Git (https://github.com/symfony/ClassLoader) ;
* Installez le via Composer (``symfony/class-loader`` sur `Packagist`_).

Utilisation
-----------

.. versionadded:: 2.1
   La méthode ``useIncludePath`` a été ajoutée dans Symfony 2.1.

Enregistrer l'« autoloader » :class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader`
est très facile::

    require_once '/path/to/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';

    use Symfony\Component\ClassLoader\UniversalClassLoader;

    $loader = new UniversalClassLoader();
    
    // enregistrez les espaces de noms et préfixes ici (voir ci-dessous)

    // vous pouvez rechercher dans l'« include_path » en dernier recours.
    $loader->useIncludePath(true);

    $loader->register();

Pour des gains de performance mineurs, les chemins vers les classes peuvent être
cachés en mémoire en utilisant APC en enregistrant la classe
:class:`Symfony\\Component\\ClassLoader\\ApcUniversalClassLoader`::

    require_once '/path/to/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';
    require_once '/path/to/src/Symfony/Component/ClassLoader/ApcUniversalClassLoader.php';

    use Symfony\Component\ClassLoader\ApcUniversalClassLoader;

    $loader = new ApcUniversalClassLoader('apc.prefix.');
    $loader->register();

L'« autoloader » est utile uniquement si vous ajoutez des bibliothèques à charger
automatiquement.

.. note::

    L'« autoloader » est automatiquement enregistré dans une application
    Symfony2 (voir ``app/autoload.php``).

Si les classes à charger automatiquement utilisent les espaces de noms, utilisez la
méthode :method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerNamespace`
ou
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerNamespaces`::

    $loader->registerNamespace('Symfony', __DIR__.'/vendor/symfony/symfony/src');

    $loader->registerNamespaces(array(
        'Symfony' => __DIR__.'/../vendor/symfony/symfony/src',
        'Monolog' => __DIR__.'/../vendor/monolog/monolog/src',
    ));

    $loader->register();

Pour les classes qui suivent la convention de nommage PEAR, utilisez la méthode
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerPrefix`
ou
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerPrefixes`::

    $loader->registerPrefix('Twig_', __DIR__.'/vendor/twig/twig/lib');

    $loader->registerPrefixes(array(
        'Swift_' => __DIR__.'/vendor/swiftmailer/swiftmailer/lib/classes',
        'Twig_'  => __DIR__.'/vendor/twig/twig/lib',
    ));

    $loader->register();

.. note::

    Certaines bibliothèques requièrent que la racine de leur chemin soit définie
    dans le « include path » PHP (``set_include_path()``).

Les classes provenant d'un sous-espace de nom ou d'une sous-hiérarchie de classes
PEAR peuvent être recherchées dans une liste de chemins afin de faciliter la séparation
de sous-ensembles de bibliothèques pour les grands projets::

    $loader->registerNamespaces(array(
        'Doctrine\\Common'           => __DIR__.'/vendor/doctrine/common/lib',
        'Doctrine\\DBAL\\Migrations' => __DIR__.'/vendor/doctrine/migrations/lib',
        'Doctrine\\DBAL'             => __DIR__.'/vendor/doctrine/dbal/lib',
        'Doctrine'                   => __DIR__.'/vendor/doctrine/orm/lib',
    ));

    $loader->register();

Dans cet exemple, si vous essayez d'utiliser une classe de l'espace de noms
``Doctrine\Common`` ou de l'un de ses enfants, l'« autoloader » va d'abord rechercher
la classe dans le répertoire ``doctrine-common``, et il va ensuite rechercher dans
le répertoire ``Doctrine`` (le dernier configuré) s'il ne la trouve pas, avant
d'abandonner. L'ordre des enregistrements est significatif dans ce cas.

.. _standards: http://www.php-fig.org/psr/psr-0/fr/
.. _PEAR:      http://pear.php.net/manual/en/standards.php
.. _Packagist: https://packagist.org/packages/symfony/class-loader
