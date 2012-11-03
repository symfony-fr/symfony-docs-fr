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

Types de noeud
~~~~~~~~~~~~~~

Il est possible de valider le type d'une valeur fournie en utilisant la
définition de noeud appropriée. Les types de noeud disponibles sont :
  
* scalar
* boolean
* array
* enum (new in 2.1)
* variable (pas de validation)
  
et sont créés avec ``node($name, $type)`` ou leurs méthodes raccourcies associées
``xxxxNode($name)``.

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

Sections facultatives
---------------------

.. versionadded:: 2.1
    Les méthodes ``canBeEnabled`` et ``canBeDisabled`` sont une nouveauté de Symfony 2.2

Si vous avez des sections entières qui sont facultatives et qui peuvent être activées ou
désactivées, vous pouvez profiter des avantages des méthodes raccourci
:method:`Symfony\\Component\\Config\\Definition\\Builder\\ArrayNodeDefinition::canBeEnabled` et
:method:`Symfony\\Component\\Config\\Definition\\Builder\\ArrayNodeDefinition::canBeDisabled`::

    $arrayNode
        ->canBeEnabled()
    ;

    // est équivalent à

    $arrayNode
        ->treatFalseLike(array('enabled' => false))
        ->treatTrueLike(array('enabled' => true))
        ->treatNullLike(array('enabled' => true))
        ->children()
            ->booleanNode('enabled')
                ->defaultFalse()
    ;

La méthode ``canBeDisabled`` est quasiment identique à ceci près que la section sera
activée par défaut.

Options de fusion
-----------------

Des options supplémentaires concernant le processus de fusion peuvent être
fournies. Pour les tableaux :

``performNoDeepMerging()``
    Lorsque la valeur est aussi définie dans un second tableau de configuration,
    n'essaye pas de fusionner un tableau, mais le surcharge entièrement

Pour tout les noeuds :

``cannotBeOverwritten()``
    Ne laisse pas les autres tableaux de configuration surcharger une
    valeur existante pour ce noeud

Ajouter des sections
--------------------

Si vous devez valider une configuration complexe, alors l'arbre peut devenir
très long et vous voudrez surement le découper en plusieurs sections. Vous
pouvez faire cela en définissant une section dans un noeud séparé et en ajoutant
ce noeud à l'arbre principal avec ``append()``::

    public function getConfigTreeBuilder()
    {
        $treeBuilder = new TreeBuilder();
        $rootNode = $treeBuilder->root('database');

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
                ->append($this->addParametersNode())
            ->end()
        ;

        return $treeBuilder;
    }

    public function addParametersNode()
    {
        $builder = new TreeBuilder();
        $node = $builder->root('parameters');

        $node
            ->isRequired()
            ->requiresAtLeastOneElement()
            ->useAttributeAsKey('name')
            ->prototype('array')
                ->children()
                    ->scalarNode('name')->isRequired()->end()
                    ->scalarNode('value')->isRequired()->end()
                ->end()
            ->end()
        ;

        return $node;
    }

C'est aussi très utile pour vous aider à ne pas vous répeter si vous avez
des sections de configuration qui sont identiques en plusieurs endroits.

Normalisation
-------------

Lorsque les fichiers de configuration sont traités, ils sont d'abord normalisés.
Ensuite, ils sont mergés puis l'arbre est utilisé pour valider le tableau qui a été
généré. La normalisation consiste à supprimer certaines des différences issues des différents
formats de configuration, principalement des différences entre Yaml et XML.

Typiquement, le séparateur de clés utilisé en Yaml est ``_`` et ``-`` en XML.
Par exemple, ``auto_connect`` en Yaml deviendrait ``auto-connect`` en XML. La
normalisation les transforme tout les deux en ``auto_connect``.

Une autre différence en Yaml et Xml est la manière dont les tableaux de valeurs
sont représentés. En Yaml, cela ressemble à :

.. code-block:: yaml

    twig:
        extensions: ['twig.extension.foo', 'twig.extension.bar']

et en XML à :

.. code-block:: xml

    <twig:config>
        <twig:extension>twig.extension.foo</twig:extension>
        <twig:extension>twig.extension.bar</twig:extension>
    </twig:config>

Cette différence peut être supprimée à la normalisation en pluralisant
la clé utilisée en XML. Vous pouvez indiquer que vous voulez qu'une clé soit
pluralisée de cette manière avec ``fixXmlConfig()``::

    $rootNode
        ->fixXmlConfig('extension')
        ->children()
            ->arrayNode('extensions')
                ->prototype('scalar')->end()
            ->end()
        ->end()
    ;

S'il s'agit d'un pluriel irrégulier, vous pouvez le spécifier comme second
argument::

    $rootNode
        ->fixXmlConfig('child', 'children')
        ->children()
            ->arrayNode('children')
        ->end()
    ;

En parallèle, ``fixXmlConfig`` garantit que chaque élément xml sera toujours
intégré dans un tableau. Vous pouvez avoir :

.. code-block:: xml

    <connection>default</connection>
    <connection>extra</connection>

et parfois seulement :

.. code-block:: xml

    <connection>default</connection>

Par défaut, ``connection`` sera un tableau dans le premier cas et une chaine
de caractères dans le second cas, ce qui rendrait la validation difficile.
Vous pouvez vous assurer qu'il s'agisse toujours d'un tableau avec ``fixXmlConfig``.

Si nécessaire, vous pouvez contrôler encore plus le processus de normalisation. Par
exemple, vous pouvez autoriser qu'une chaine de caractères soit définie et utilisée
comme clé particulière, ou que plusieurs clés soit définies explicitement. Pour cela,
si tout est facultatif dans votre config sauf l'id:

.. code-block:: yaml

    connection:
        name: my_mysql_connection
        host: localhost
        driver: mysql
        username: user
        password: pass

vous pouvez agalement autoriser ce qui suit :

.. code-block:: yaml

    connection: my_mysql_connection

en changeant la valeur d'une chaine de caractère en tableau associatif avec
``name`` comme clé::
    $rootNode
        ->arrayNode('connection')
           ->beforeNormalization()
               ->ifString()
               ->then(function($v) { return array('name'=> $v); })
           ->end()
           ->scalarValue('name')->isRequired()
           // ...
        ->end()
    ;    

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
    $processedConfiguration = $processor->processConfiguration(
        $configuration,
        $configs)
    ;
