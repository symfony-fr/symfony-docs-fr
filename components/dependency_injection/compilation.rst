﻿.. index::
   single: Dependency Injection; Compilation

Compiler le Conteneur
=====================

Le conteneur de service peut être compilé pour plusieurs raisons. Ces
raisons incluent les vérifications de tout les problèmes potentiels
comme par exemple les références circulaires, ainsi que le fait de rendre le conteneur plus
efficace en résolvant les paramètres et en supprimant les services qui ne sont pas
utilisés.

Le conteneur est compilé en exécutant::

    $container->compile();

La méthode « compile » utilise des *Passes de Compilation* pour la compilation. Le
composant d'*Injection de Dépendance* est fourni avec plusieurs passes qui sont
automatiquement enregistrées pour la compilation. Par exemple, la classe
:class:`Symfony\\Component\\DependencyInjection\\Compiler\\CheckDefinitionValidityPass`
vérifie les problèmes potentiels qu'il pourrait y avoir avec les définitions
qui ont été déclarées dans le conteneur. Après cette passe et d'autres qui se chargent
de vérifier la validité du conteneur, des passes de compilation supplémentaires
sont utilisées pour optimiser la configuration avant qu'elle soit mise en cache.
Par exemple, les services privés et les services abstraits sont supprimés, et les
alias sont résolus.

Créer une Passe de Compilateur
------------------------------

Vous pouvez aussi créer et enregistrer vos propres passes de compilateur dans
le conteneur. Pour créer une passe de compilateur, vous devez implémenter
l'interface :class:`Symfony\\Component\\DependencyInjection\\Compiler\\CompilerPassInterface`.
La passe de compilateur vous donne l'opportunité de manipuler les définitions
de service qui ont été compilées. Cela peut être très puissant, mais ce n'est
pas non plus quelque chose dont vous aurez besoin tous les jours.

La passe de compilateur doit avoir la méthode ``process`` qui est passée au
conteneur qui doit être compilé::

    class CustomCompilerPass
    {
        public function process(ContainerBuilder $container)
        {
           //--
        }
    }

Les paramètres et définitions du conteneur peuvent être manipulés en
utilisant les méthodes décrites dans la documentation que vous trouverez
ici :doc:`/components/dependency_injection/definitions`. Une chose courante
à faire dans une passe de compilateur est de rechercher tous les services
qui ont un certain tag afin de les traiter d'une certaine manière ou d'injecter
chacun d'entre eux dans un autre service de façon dynamique.

Enregistrer une Passe de Compilateur
------------------------------------

Vous devez enregistrer votre passe personnalisée dans votre conteneur. Sa
méthode « process » sera alors appelée lorsque le conteneur aura été compilé::

    use Symfony\Component\DependencyInjection\ContainerBuilder;

    $container = new ContainerBuilder();
    $container->addCompilerPass(new CustomCompilerPass);

Contrôler l'Ordre des Passes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les passes de compilateur par défaut sont groupées en des passes d'optimisation
et des passes de suppression. Les passes d'optimisation sont exécutées en premier
et incluent des tâches comme résoudre les références dans les définitions. Les
passes de suppression exécutent des tâches telles que la suppression des alias privés
et des services inutilisés. Vous pouvez choisir dans quel ordre de passage vous
souhaitez que vos passes personnalisées soient exécutées. Par défaut, elles vont
être exécutées avant les passes d'optimisation.

Vous pouvez utiliser les constantes suivantes en tant que second argument quand
vous enregistrez une passe dans le conteneur pour contrôler où elle sera placée
dans l'ordre de passage :

* ``PassConfig::TYPE_BEFORE_OPTIMIZATION``
* ``PassConfig::TYPE_OPTIMIZE``
* ``PassConfig::TYPE_BEFORE_REMOVING``
* ``PassConfig::TYPE_REMOVE``
* ``PassConfig::TYPE_AFTER_REMOVING``

Par exemple, pour exécuter votre passe personnalisée après que les passes de suppression
par défaut ont été exécutées, vous pouvez faire comme cela::

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Compiler\PassConfig;

    $container = new ContainerBuilder();
    $container->addCompilerPass(new CustomCompilerPass, PassConfig::TYPE_AFTER_REMOVING);


Gérer la Configuration avec des Extensions
------------------------------------------

Tout comme vous pouvez charger la configuration directement dans le conteneur,
comme c'est expliqué dans l':doc:`/components/dependency_injection/introduction`, vous
pouvez aussi la gérer en enregistrant des extensions dans le conteneur. Les
extensions doivent implémenter l'interface
:class:`Symfony\\Component\\DependencyInjection\\Extension\\ExtensionInterface` et
peuvent être enregistrées dans le conteneur avec::

    $container->registerExtension($extension);

Le travail principal d'une extension se déroule dans la méthode ``load``.
Dans cette dernière, vous pouvez charger votre configuration depuis un ou
plusieurs fichiers de configuration ainsi que manipuler les définitions du
conteneur en utilisant les méthodes montrées dans
:doc:`/components/dependency_injection/definitions`.

Un nouveau conteneur à définir est passé à la méthode ``load``, qui est
ensuite fusionné avec le conteneur avec lequel il est enregistré. Cela
vous permet d'avoir plusieurs extensions qui gèrent les définitions du
conteneur indépendemment. Les extensions n'ajoutent rien à la configuration
des conteneurs lorsqu'elles sont ajoutées mais sont traitées quand la méthode
``compile`` du conteneur est appelée.

.. note::

    Si vous devez manipuler la configuration chargée par une extension, alors
    vous ne pouvez pas le faire depuis une autre extension comme elle utilise
    un nouveau conteneur. Pour cela, vous devriez plutôt utiliser une passe de
    compilateur à la place qui fonctionne avec le conteneur complet après que
    les extensions ont été traitées.

« Dumper » la Configuration pour plus de Performance
----------------------------------------------------

Utiliser des fichiers de configuration pour gérer le conteneur de services
peut être beaucoup plus facile à comprendre que d'utiliser PHP une fois que
vous avez de nombreux services. Néanmoins, cette facilité a un prix quand on
commence à parler de performance car les fichiers de configuration ont besoin
d'être traités et ensuite la configuration en PHP a besoin d'être assemblée
à partir de ces derniers. Le processus de compilation rend le conteneur plus
efficace mais il prend du temps à être exécuté. Cependant, vous pouvez avoir
le meilleur des deux mondes en utilisant des fichiers de configuration que
vous « dumpez » et dont vous cachez la configuration résultante.
Le ``PhpDumper`` facilite le « dump » du conteneur compilé::

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Dumper\PhpDumper

    $file = __DIR__ .'/cache/container.php';

    if (file_exists($file)) {
        require_once $file;
        $container = new ProjectServiceContiner();
    } else {
        $container = new ContainerBuilder();
        //--
        $container->compile();

        $dumper = new PhpDumper($container);
        file_put_contents($file, $dumper->dump());
    }

``ProjectServiceContainer`` est le nom par défaut donné à la classe du conteneur
« dumpé », mais vous pouvez changer cela avec l'option ``class`` lorsque vous
la « dumpez »::

    // ...
    $file = __DIR__ .'/cache/container.php';

    if (file_exists($file)) {
        require_once $file;
        $container = new MyCachedContainer();
    } else {
        $container = new ContainerBuilder();
        //--
        $container->compile();

        $dumper = new PhpDumper($container);
        file_put_contents($file, $dumper->dump(array('class' => 'MyCachedContainer')));
    }

Vous allez maintenant profiter de la rapidité du conteneur PHP configuré tout en
conservant la facilité d'utilisation des fichiers de configuration. De plus, dumper
le conteneur de cette manière optimise encore la manière dont les services sont
créés par le conteneur.

Dans l'exemple ci-dessus, vous devrez supprimer le fichier du conteneur mis en cache
chaque fois que vous effectuerez des changements. Ajouter un contrôle sur une variable
qui détermine si vous êtes en mode débuggage vous permet de conserver la rapidité du conteneur
mis en cache en production mais aussi d'avoir une configuration toujours à jour lorsque vous
êtes en train de développer votre application::

    // ...

    // définir $isDebug en se basant sur une information provenant de votre projet

    $file = __DIR__ .'/cache/container.php';

    if (!$isDebug && file_exists($file)) {
        require_once $file;
        $container = new MyCachedContainer();
    } else {
        $container = new ContainerBuilder();
        //--
        $container->compile();

        if(!$isDebug) 
            $dumper = new PhpDumper($container);
            file_put_contents($file, $dumper->dump(array('class' => 'MyCachedContainer')));
        }
    }

Cela pourrait être encore amélioré en recompilant seulement le conteneur en mode
debug lorsque des changements ont été fait dans sa configuration plutôt qu'à
chaque requête. Ceci peut être fait en cachant les fichiers utilisés pour configurer
le conteneur de la manière décrite dans « :doc:`/components/config/caching` » dans
la documentation du composant Config.

Vous n'avez pas besoin de vous soucier des fichiers à mettre en cache car le contructeur
du conteneur garde une trâce de toute les ressources utilisées pour le configurer, pas
seulement les fichiers de configuration mais également les classes d'extension et les
passes de compilateur. Cela signifie que tout changement dans l'un de ces fichiers
invalidera le cache et déclenchera la regénération du conteneur. Vous avez juste besoin
de demander ces ressources au conteneur et les utiliser comme metadonnées pour le cache::

    // ...

    // définissez $isDebug en vous basant sur quelque chose dans votre projet

    $file = __DIR__ .'/cache/container.php';
    $containerConfigCache = new ConfigCache($file, $isDebug);

    if (!$containerConfigCache->isFresh()) {
        $containerBuilder = new ContainerBuilder();
        //--
        $containerBuilder->compile();

        $dumper = new PhpDumper($containerBuilder);
        $containerConfigCache->write(
            $dumper->dump(array('class' => 'MyCachedContainer')),
            $containerBuilder->getResources()
        );
    }

    require_once $file;
    $container = new MyCachedContainer();

Maintenant, le conteneur récupéré dans le cache est utilisé indépendamment du fait
que le mode debug est activé ou non. La différence est que le ``ConfigCache`` est
définit comme le debug mode (la valeur du mode debug lui est passé comme second
argument dans son constructeur). Lorsque le cache n'est pas en mode debug, le conteneur
mis en cache sera toujours utilisé s'il existe. En mode debug, un fichier de métadonnées
est écrit avec le timestamp de tout les fichiers de ressource. Ceci sont ensuite vérifiés
pour voir si les fichiers ont changé, et si c'est le cas, le cache sera considéré comme
périmé.