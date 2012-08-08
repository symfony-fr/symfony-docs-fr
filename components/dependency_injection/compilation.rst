﻿.. index::
   single: Dependency Injection; Compilation

Compiler le Conteneur
=====================

Le conteneur de service peut être compilé pour plusieurs raisons. Ces
raisons incluent d'effectuer des vérifications de quelconques problèmes
potentiels comme des références circulaires et de rendre le conteneur plus
efficace en résolvant les paramètres et en supprimant les services n'étant pas
utilisés.

Le conteneur est compilé en exécutant::

    $container->compile();

La méthode « compile » utilise des *Passes de Compilation* pour la compilation. Le
composant d'*Injection de Dépendance* vient avec plusieurs passes qui sont
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
conteneur allant être compilé::

    class CustomCompilerPass
    {
        public function process(ContainerBuilder $container)
        {
           //--
        }
    }

Les paramètres et définitions du conteneur peuvent être manipulées en
utilisant les méthodes décrites dans la documentation que vous trouvez
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
passes de suppression exécutent des tâches telles la suppression des alias privés
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
par défaut aient été exécutées, vous pouvez faire comme cela::

For example, to run your custom pass after the default removal passes have been run::

    use Symfony\Component\DependencyInjection\ContainerBuilder;

    $container = new ContainerBuilder();
    $container->addCompilerPass(new CustomCompilerPass, PassConfig::TYPE_AFTER_REMOVING);


Gérer la Configuration avec des Extensions
------------------------------------------

Tout comme vous pouvez charger la configuration directement dans le conteneur
comment montré dans :doc:`/components/dependency_injection/introduction`, vous
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
    les extensions aient été traitées.

« Dumper » la Configuration pour plus de Performance
----------------------------------------------------

Utiliser des fichiers de configuration pour gérer le conteneur de services
peut être beaucoup plus facile à comprendre que d'utiliser PHP une fois que
vous avez de nombreux services. Néanmoins, cette facilité a un prix quand on
commence à parler de performance car les fichiers de configuration ont besoin
d'être traités et ensuite, la configuration en PHP a besoin d'être assemblée
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

``ProjectServiceContiner`` est le nom par défaut donné à la classe du conteneur
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

Vous allez maintenant profiter de la rapidité du conteneur configuré PHP tout en
conservant la facilité d'utilisation des fichiers de configuration. Dans l'exemple
ci-dessus, vous devrez supprimer le fichier du conteneur caché chaque fois que vous
effectuez des changements. Ajouter un contrôle sur une variable qui détermine si
vous êtes en mode débuggage vous permet de conserver la vitesse du conteneur caché
en production mais d'avoir une configuration toujours à jour lorsque vous êtes en
train de développer votre application::

    // ...

    // définir $isDebug basé sur une information provenant de votre projet

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

