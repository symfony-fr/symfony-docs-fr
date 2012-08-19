.. index::
   single: Dependency Injection; Service definitions

Travailler avec les paramètres et définitions du conteneur
==========================================================

Récupérer et Définir les Paramètres du Conteneur
------------------------------------------------

Travailler avec les paramètres du conteneur est très facile si vous utilisez
les méthodes d'accès du conteneur pour les paramètres. Vous pouvez contrôler
qu'un paramètre a été défini dans le conteneur avec::

     $container->hasParameter($name);

Vous pouvez récupérer des paramètres définis dans le conteneur avec::

    $container->getParameter($name);

et définir un paramètre dans le conteneur grâce à::

    $container->setParameter($name, $value);

Récupérer et définir les définitions de service
-----------------------------------------------

Il existe aussi des méthodes utiles pour travailler avec les
définitions de service.

Pour savoir si il y a une définition pour un certain ID de service::

    $container->hasDefinition($serviceId);

Cela est utile si vous voulez faire quelque chose uniquement si une définition
particulière existe.

Vous pouvez récupérer une définition avec::

    $container->getDefinition($serviceId);

ou::

    $container->findDefinition($serviceId);

qui contrairement à ``getDefinition()`` résoud aussi les alias donc si un
argument ``$serviceId`` est un alias, vous allez récupérer la définition
sous-jacente.

Les définitions de service elles-mêmes sont des objets donc si vous récupérez
une définition avec ces méthodes et effectuez des changements sur cette dernière,
ils seront répercutés sur le conteneur. Cependant, si vous créez une nouvelle
définition alors vous pouvez l'ajouter au conteneur en utilisant::

    $container->setDefinition($id, $definition);

Travailler avec une définition
------------------------------

Créer une nouvelle définition
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous devez créer une nouvelle définition plutôt que d'en manipuler une
récupérée depuis le conteneur, alors la classe de définition est
:class:`Symfony\\Component\\DependencyInjection\\Definition`.

Classe
~~~~~~

La classe de définition est la classe de l'objet retourné lorsque le
service est demandé depuis le conteneur.

Pour savoir quelle classe est définie pour une définition::

    $definition->getClass();

et pour définir une classe différente::

    $definition->setClass($class); // nom de classe entièrement qualifié en tant que chaîne de caractères

Arguments du constructeur
~~~~~~~~~~~~~~~~~~~~~~~~~

Pour récupérer un tableau contenant les arguments du constructeur pour une
définition, vous pouvez utiliser::

    $definition->getArguments();

ou pour récupérer un seul argument via sa position::

    $definition->getArgument($index); 
    // par exemple : $definition->getArguments(0) pour le premier argument

Vous pouvez ajouter un argument à la fin du tableau d'arguments en utilisant::

    $definition->addArgument($argument);

L'argument peut être une chaîne de caractères, un tableau, un paramètre de service en
utilisant ``%nom_de_paramètre%`` ou un ID de service en utilisant::

    use Symfony\Component\DependencyInjection\Reference;
  
    //--

    $definition->addArgument(new Reference('service_id'));

De façon similaire, vous pouvez remplacer un argument déjà défini via
son index en utilisant::

    $definition->replaceArgument($index, $argument);

Vous pouvez aussi remplacer tous les arguments (ou en définir quelques-uns
s'il n'y en a pas) par un tableau d'arguments::

    $definition->replaceArguments($arguments);

Appels de méthode
~~~~~~~~~~~~~~~~~

Si le service avec lequel vous travaillez utilise l'injection par mutateur (« setter »
en anglais), alors vous pouvez aussi manipuler n'importe quels appels de méthode dans
les définitions.

Vous pouvez récupérer un tableau de tous les appels de méthode avec::

    $definition->getMethodCalls();

Ajoutez un appel de méthode avec::

   $definition->addMethodCall($method, $arguments);

Où ``$method`` est le nom de la méthode et ``$arguments`` est un tableau d'arguments
à utiliser lors de l'appel de la méthode. Les arguments peuvent être des chaînes
de caractères, des tableaux, des paramètres ou des IDs de service tout comme pour
les arguments du constructeur.

Vous pouvez aussi remplacer n'importe quel appel de méthode par un tableau
de nouveaux appels grâce à la méthode::

    $definition->setMethodCalls($methodCalls);

