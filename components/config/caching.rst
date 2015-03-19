.. index::
   single: Config; Caching based on resources

Mécanisme de cache basé sur les ressources
==========================================

Lorsque toutes les ressources de configuration sont chargées, vous pourriez
vouloir traiter les valeurs de configuration et les combiner toutes en
un seul fichier. Ce fichier agit en tant que cache. Son contenu n'a pas
besoin d'être regénéré chaque fois que l'application est exécutée mais seulement
quand les ressources de configuration sont modifiées.

Par exemple, le composant de Routage de Symfony vous permet de charger
toutes les routes, et d'afficher une correspondance d'URL ou une URL générée
basée sur ces routes. Dans ce cas, lorsqu'une des ressources est modifiée
(et que vous travaillez dans un environnement de développement), le fichier
généré devrait être invalidé ou regénéré. Cela peut être effectué en utilisant
la classe :class:`Symfony\\Component\\Config\\ConfigCache`.

L'exemple ci-dessous vous montre comment collecter des ressources, puis
générer du code basé sur les ressources qui ont été chargées, et écrire
ce code dans le cache. Le cache reçoit aussi la collection de ressources
qui ont été utilisées pour générer le code. En regardant le timestamp de
la date de dernière modification de ces ressources, le cache peut dire
s'il contient toujours la dernière version ou si son contenu devrait
être regénéré::

    use Symfony\Component\Config\ConfigCache;
    use Symfony\Component\Config\Resource\FileResource;

    $cachePath = __DIR__.'/cache/appUserMatcher.php';

    // le second argument indique si vous voulez utiliser le mode debug ou non
    $userMatcherCache = new ConfigCache($cachePath, true);

    if (!$userMatcherCache->isFresh()) {
        // remplissez cela avec un tableau contenant les chemins des fichiers
        // « users.yml »
        $yamlUserFiles = ...;

        $resources = array();

        foreach ($yamlUserFiles as $yamlUserFile) {
            // Lisez l'article précédent « Chargement des ressources »
            // pour voir d'où provient $delegatingLoader
            $delegatingLoader->load($yamlUserFile);
            $resources[] = new FileResource($yamlUserFile);
        }

        // le code pour le « UserMatcher » est généré quelque part d'autre
        $code = ...;

        $userMatcherCache->write($code, $resources);
    }

    // vous pourriez vouloir inclure le code caché :
    require $cachePath;

En mode débuggage, un fichier ``.meta`` sera créé dans le même répertoire
que le fichier de cache lui-même. Ce fichier ``.meta`` contient les ressources
sérialisées, dont les timestamps sont utilisées pour déterminer si le
cache contient toujours la dernière version. Lorsque vous n'êtes pas en
mode débuggage, le cache est considéré comme contenant la dernière version
dès qu'il existe, et donc, aucun fichier ``.meta`` ne sera généré.
