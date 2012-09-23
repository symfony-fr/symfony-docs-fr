.. index::
   single: Config; Define and process configuration values

Définir et traiter les valeurs de configuration
===============================================

Valider les valeurs de configuration
------------------------------------

Après avoir chargé les valeurs de configuration depuis toute sorte de ressources,
les valeurs et leur structure peuvent être validées en utilisant la partie
« Definition » du Composant Config. On s'attend généralement à ce que les
valeurs de configuration possèdent une certaine hiérarchie. Aussi, les
valeurs devraient être d'un certain type, restreintes en nombre ou faire
partie d'un ensemble de valeurs donné. Par exemple, la configuration suivante
(en YAML) montre une hiérarchie claire et quelques règles de validation
qui devraient lui être appliquées (par exemple : « la valeur de ``auto_connect``
doit être une valeur booléenne ») :

.. code-block:: yaml

    auto_connect: true
    default_connection: mysql
    connections:
        mysql:
            host: localhost
            driver: mysql
            username: user
            password: pass
        sqlite:
            host: localhost
            driver: sqlite
            memory: true
            username: user
            password: pass

Lorsque vous chargez plusieurs fichiers de configuration, il devrait être
possible de fusionner et de surcharger quelques valeurs. D'autres valeurs
ne devraient pas être fusionnées et rester comme elles étaient lorsque
vous les avez rencontrées pour la première fois. Aussi, certaines clés
sont seulement disponibles quand une autre clé possède une valeur spécifique
(dans l'exemple de configuration ci-dessus : la clé ``memory`` n'a de
sens que si le ``driver`` est ``sqlite``).

Définir une hiérarchie des valeurs de configuration en utilisant le « TreeBuilder »
-----------------------------------------------------------------------------------

Toutes les règles concernant les valeurs de configuration peuvent être
définies en utilisant le :class:`Symfony\\Component\\Config\\Definition\\Builder\\TreeBuilder`
(« constructeur d'arbre » en français).

Une instance de :class:`Symfony\\Component\\Config\\Definition\\Builder\\TreeBuilder`
devrait être retournée depuis une classe ``Configuration`` personnalisée
qui implémente :class:`Symfony\\Component\\Config\\Definition\\ConfigurationInterface`::

    namespace Acme\DatabaseConfiguration;

    use Symfony\Component\Config\Definition\ConfigurationInterface;
    use Symfony\Component\Config\Definition\Builder\TreeBuilder;

    class DatabaseConfiguration implements ConfigurationInterface
    {
        public function getConfigTreeBuilder()
        {
            $treeBuilder = new TreeBuilder();
            $rootNode = $treeBuilder->root('database');

            // ... ajoute des définitions de noeud à la racine de l'arbre

            return $treeBuilder;
        }
    }

Ajouter des définitions de noeuds à l'arbre
-------------------------------------------

Noeuds variable
~~~~~~~~~~~~~~~

Un arbre contient des définitions de noeuds qui peuvent être décrites d'une
manière sémantique. Cela signifie qu'en utilisant l'indentation et une
notation fluide, il est possible de refléter la structure réelle des valeurs
de configuration::

    $rootNode
        ->children()
            ->booleanNode('auto_connect')
                ->defaultTrue()
            ->end()
            ->scalarNode('default_connection')
                ->defaultValue('default')
            ->end()
        ->end()
    ;

Le noeud racine lui-même est un tableau de noeuds, et possède des enfants,
comme le noeud booléen ``auto_connect`` et le noeud scalaire ``default_connection``.
En général : après avoir défini un noeud, un appel à la méthode ``end()``
vous fait remonter d'un niveau dans la hiérarchie.

Noeuds tableau
~~~~~~~~~~~~~~

Il est possible d'ajouter un niveau plus profond à la hiérarchie en ajoutant
un noeud tableau. Le noeud tableau lui-même peut avoir un ensemble prédéfini
de noeuds variable :

.. code-block:: php

    $rootNode
        ->arrayNode('connection')
            ->scalarNode('driver')->end()
            ->scalarNode('host')->end()
            ->scalarNode('username')->end()
            ->scalarNode('password')->end()
        ->end()
    ;

Ou vous pouvez définir un prototype pour chaque noeud à l'intérieur d'un
noeud tableau :

.. code-block:: php

    $rootNode
        ->arrayNode('connections')
            ->prototype('array')
                ->children()
                    ->scalarNode('driver')->end()
                    ->scalarNode('host')->end()
                    ->scalarNode('username')->end()
                    ->scalarNode('password')->end()
                ->end()
            ->end()
        ->end()
    ;

Un prototype peut être utilisé pour ajouter une définition qui peut être
répétée un grand nombre de fois à l'intérieur du noeud courant. Selon la
définition du prototype dans l'exemple ci-dessus, il est possible d'avoir
plusieurs tableaux de connexion (contenant un ``driver``, ``host``, etc.).

Options de noeud tableau
~~~~~~~~~~~~~~~~~~~~~~~~

Avant de définir les enfants d'un noeud tableau, vous pouvez fournir des
options comme :

``useAttributeAsKey()``
    Fournit le nom d'un noeud enfant, dont la valeur devrait être utilisée
    en tant que clé dans le tableau résultant
``requiresAtLeastOneElement()``
    Il devrait y avoir au moins un élément dans le tableau (fonctionne
    seulement quand ``isRequired`` est aussi appelé).

Un exemple de cela :

.. code-block:: php

    $rootNode
        ->arrayNode('parameters')
            ->isRequired()
            ->requiresAtLeastOneElement()
            ->useAttributeAsKey('name')
            ->prototype('array')
                ->children()
                    ->scalarNode('name')->isRequired()->end()
                    ->scalarNode('value')->isRequired()->end()
                ->end()
            ->end()
        ->end()
    ;

Valeurs par défaut et valeurs requises
--------------------------------------

Pour tous les types de noeud, il est possible de définir des valeurs par
défaut et des valeurs de remplacement dans le cas où un noeud possède une
certaine valeur :

``defaultValue()``
    Définit une valeur par défaut
``isRequired()``
    Doit être défini (mais peut être vide)
``cannotBeEmpty()``
    Ne peut pas contenir de valeur vide
``default*()``
    (``null``, ``true``, ``false``), raccourci pour ``defaultValue()``
``treat*Like()``
    (``null``, ``true``, ``false``), fournit une valeur de remplacement
    dans le cas où la valeur est ``*.``

.. code-block:: php

    $rootNode
        ->arrayNode('connection')
            ->children()
                ->scalarNode('driver')
                    ->isRequired()
                    ->cannotBeEmpty()
                ->end()
                ->scalarNode('host')
                    ->defaultValue('localhost')
                ->end()
                ->scalarNode('username')->end()
                ->scalarNode('password')->end()
                ->booleanNode('memory')
                    ->defaultFalse()
                ->end()
            ->end()
        ->end()
    ;

Options de fusion
-----------------

Des options supplémentaires concernant le processus de fusion peuvent être
fournies. Pour les tableaux :

``performNoDeepMerging()``
    Lorsque la valeur est aussi définie dans un second tableau de configuration,
    n'essaye pas de fusionner un tableau, mais le surcharge entièrement

Pour tous les noeuds :

``cannotBeOverwritten()``
    Ne laisse pas les autres tableaux de configuration surcharger une
    valeur existante pour ce noeud

Règles de validation
--------------------

Des règles de validation plus avancées peuvent être fournies en utilisant
le :class:`Symfony\\Component\\Config\\Definition\\Builder\\ExprBuilder`
(« constructeur d'expression » en français). Ce constructeur implémente
une interface fluide pour une structure de contrôle bien connue. Le constructeur
est utilisé pour ajouter des règles de validation avancées aux définitions
de noeud, comme::

    $rootNode
        ->arrayNode('connection')
            ->children()
                ->scalarNode('driver')
                    ->isRequired()
                    ->validate()
                        ->ifNotInArray(array('mysql', 'sqlite', 'mssql'))
                        ->thenInvalid('Invalid database driver "%s"')
                    ->end()
                ->end()
            ->end()
        ->end()
    ;

Une règle de validation a toujours une partie « if ». Vous pouvez spécifier
cette partie grâce aux manières suivantes :

- ``ifTrue()``
- ``ifString()``
- ``ifNull()``
- ``ifArray()``
- ``ifInArray()``
- ``ifNotInArray()``
- ``always()``

Une règle de validation requiert aussi une partie « then » :

- ``then()``
- ``thenEmptyArray()``
- ``thenInvalid()``
- ``thenUnset()``

Généralement, « then » est une closure. Sa valeur de retour sera utilisée
en tant que nouvelle valeur pour le noeud, à la place de la valeur originale
de ce dernier.

Traiter les valeurs de configuration
------------------------------------

La classe :class:`Symfony\\Component\\Config\\Definition\\Processor` utilise
l'arbre comme s'il était construit en utilisant le :class:`Symfony\\Component\\Config\\Definition\\Builder\\TreeBuilder`
pour traiter plusieurs tableaux de valeurs de configuration qui devraient
être fusionnés. Si une valeur quelconque n'est pas du type attendu, est obligatoire
et pas encore définie, ou n'a pas pu être validée d'une façon ou d'une
autre, une exception sera lancée. Sinon, le résultat est un tableau contenant
les valeurs de configuration::

    use Symfony\Component\Yaml\Yaml;
    use Symfony\Component\Config\Definition\Processor;
    use Acme\DatabaseConfiguration;

    $config1 = Yaml::parse(__DIR__.'/src/Matthias/config/config.yml');
    $config2 = Yaml::parse(__DIR__.'/src/Matthias/config/config_extra.yml');

    $configs = array($config1, $config2);

    $processor = new Processor();
    $configuration = new DatabaseConfiguration;
    $processedConfiguration = $processor->processConfiguration($configuration, $configs);
