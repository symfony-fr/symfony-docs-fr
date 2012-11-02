.. index::
   single: Config; Loading resources

Chargement des ressources
=========================

Localisation des ressources
---------------------------

Le chargement de la configuration démarre normalement par une recherche
des ressources - dans la plupart des cas : des fichiers. Cela peut être
effectué à l'aide de la classe :class:`Symfony\\Component\\Config\\FileLocator`::

    use Symfony\Component\Config\FileLocator;

    $configDirectories = array(__DIR__.'/app/config');

    $locator = new FileLocator($configDirectories);
    $yamlUserFiles = $locator->locate('users.yml', null, false);

Le localisateur reçoit une collection de localisations où il doit rechercher
les fichiers. Le premier argument de ``locate()`` est le nom du fichier
à rechercher. Le second argument peut être le chemin courant, et lorsqu'il
est fourni, le localisateur doit rechercher en premier dans ce répertoire.
Le troisième argument indique si oui ou non le localisateur doit retourner
le premier fichier qu'il trouve, ou bien un tableau contenant tous les
fichiers trouvés.

Chargeurs de ressource
----------------------

Pour chaque type de ressource (Yaml, XML, annotations, etc.), un chargeur
doit être défini. Chaque chargeur doit implémenter :class:`Symfony\\Component\\Config\\Loader\\LoaderInterface`
ou étendre la classe abstraite :class:`Symfony\\Component\\Config\\Loader\\FileLoader`,
qui permet d'importer d'autres ressources de manière récursive::

    use Symfony\Component\Config\Loader\FileLoader;
    use Symfony\Component\Yaml\Yaml;

    class YamlUserLoader extends FileLoader
    {
        public function load($resource, $type = null)
        {
            $configValues = Yaml::parse($resource);

            // ... gérer les valeurs de configuration

            // importer peut-être d'autres ressources :

            // $this->import('extra_users.yml');
        }

        public function supports($resource, $type = null)
        {
            return is_string($resource) && 'yml' === pathinfo(
                $resource,
                PATHINFO_EXTENSION
            );
        }
    }

Trouver le bon chargeur
-----------------------

La classe :class:`Symfony\\Component\\Config\\Loader\\LoaderResolver` reçoit
en tant que premier argument de son constructeur une collection de chargeurs.
Quand une ressource (par exemple un fichier XML) doit être chargée, le
« LoaderResolver » va itérer sur cette collection de chargeurs et retourner
le chargeur qui supporte ce type de ressource en particulier.

La classe :class:`Symfony\\Component\\Config\\Loader\\DelegatingLoader`
utilise le :class:`Symfony\\Component\\Config\\Loader\\LoaderResolver`.
Lorsqu'on lui demande de charger une ressource, elle délègue cette question
au :class:`Symfony\\Component\\Config\\Loader\\LoaderResolver`. Dans le
cas où le résolveur trouve un chargeur approprié, ce dernier va être utilisé
pour charger la ressource::

    use Symfony\Component\Config\Loader\LoaderResolver;
    use Symfony\Component\Config\Loader\DelegatingLoader;

    $loaderResolver = new LoaderResolver(array(new YamlUserLoader($locator)));
    $delegatingLoader = new DelegatingLoader($loaderResolver);

    $delegatingLoader->load(__DIR__.'/users.yml');
    /*
    Le YamlUserLoader va être utilisé pour charger cette ressource puisqu'il
    supporte les fichiers ayant une extension « yml »
    */
