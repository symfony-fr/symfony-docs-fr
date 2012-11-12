.. index::
   single: Dependency Injection; Workflow

Processus de construction du Conteneur
======================================

Dans les pages précédentes, nous n'avons que peu abordé l'emplacement où
devrait se situer les divers fichiers et classes. C'est parce que cela dépend
de l'application, des bibliothèques ou du framework dans lequel vous voulez
utiliser le Conteneur. Comprendre comment le Conteneur est configuré et construit
dans le framework full stack Symfony2 vous aidera à voir comment tout cela
s'assemble, que vous utilisiez le framework full stack ou que vous cherchiez à
utiliser les services du conteneur dans une autre application.

Le framework full stack utilise le composant ``HttpKernel`` pour gérer le
chargement de la configuration du conteneur de services depuis les bundles
et l'application et prend également en charge la compilation et la mise en cache.
Même si vous n'utilisez pas ``HttpKernel``, cela devrait vous donner une idée de
la manière dont vous devriez organiser votre configuration dans une application
modulaire.

Travailler avec le Conteneur en cache
-------------------------------------

Avant de le construire, le noyau (kernel) vérifie s'il existe une version en
cache du conteneur. Le ``HttpKernel`` possède un paramètre « debug » et s'il est
à false, alors la version en cache est utilisée si elle existe. Si « debug » est
à true, alors le noyau :doc:`vérifie si la configuration est à jour</components/config/caching>`
et si c'est le cas, alors la version en cache est utilisée. Sinon, alors le conteneur
est construit à partir de la configuration au niveau de l'application ainsi que de
la configuration des bundles.

Lisez :ref:`Dumper la configuration pour plus de performance<components-dependency-injection-dumping>`
pour plus de détails sur le sujet.

Configuration au niveau de l'application
----------------------------------------

La configuration de l'application est chargée depuis le répertoire ``app/config``.
Plusieurs fichiers sont chargés et sont ensuite mergés selon leurs extensions.
Cela permet d'avoir différentes configuration pour différents environnements,
par exemple dev et prod.

Ces fichiers contiennent des paramètres et des services qui sont chargés directement
dans le conteneur grâce à
:ref:`l'initialisation du Conteneur avec des fichiers de configuration<components-dependency-injection-loading-config>`.
Ils contiennent également la configuration qui est traitée selon l'extension, comme c'est
expliqué dans :ref:`Gérer la configuration avec les extensions<components-dependency-injection-extension>`.
Ils sont considérés comme de la configuration de bundle car chaque bundle contient
une classe Extension.

Configuration au niveau du bundle avec les Extensions
-----------------------------------------------------

Par convention, chaque bundle contient une classe Extension qui se situe
dans le répertoire ``DependencyInjection`` du bundle. Elles sont enregistrées
avec le ``ContainerBuilder`` lorsque le noyau est initialisé. Lorsque le 
``ContainerBuilder`` est :doc:`compilé</components/dependency_injection/compilation>`,
la configuration de l'application qui correspond à l'extension du bundle est passée
à l'Extension qui charge également ses propres fichiers de configuration, généralement
depuis le répertoire ``Resources/config`` du bundle. La configuration niveau application
est généralement traitée avec un :doc:`objet Configuration</components/config/definition>`
qui est également situé dans le répertoire ``DependencyInjection`` du bundle.

Passes de compilateur pour autoriser les interactions entre bundles
-------------------------------------------------------------------

:ref:`Les passes de compilateur<components-dependency-injection-compiler-passes>`
sont utilisées pour permettre des interactions entre différents bundles puisqu'ils
ne peuvent pas agir sur la configuration des autres bundles dans leur classe Extension.
L'un des principaux usages est de traiter les services taggés, ce qui permet
aux bundles d'enregistrer des services d'autres bundles, comme les loggers Monolog,
les extensions Twig et les Collecteurs de Données du Web Profiler. Les passes de
compilateur sont généralement placées dans le répertoire ``DependencyInjection/Compiler``
du bundle.

Compilation et mise en cache
----------------------------

Après que le processus de compilation a chargé les services depuis la configuration,
les extensions et les passes de compilateur, elle est dumpée pour que le cache puisse
être utilisé la prochaine fois. La version dumpée est ensuite utilisée par les
sous-requêtes, ce qui est plus efficace.