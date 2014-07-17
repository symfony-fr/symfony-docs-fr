DoctrineMongoDBBundle
=====================

.. toctree::
   :hidden:

   config
   form

L'ODM (« Object Document Mapper ») `MongoDB`_  ressemble beaucoup à l'ORM Doctrine2
dans sa philosophie et son fonctionnement. En d'autres termes, comme l':doc:`ORM Doctrine2</book/doctrine>`,
avec l'ODM `MongoDB`_, vous traitez des objets PHP qui sont ensuite persistés de
façon transparente dans et depuis MongoDB.

.. tip::

    Vous pouvez en savoir plus sur l'ODM Doctrine MongoDB en lisant la `documentation`_
    du projet.

Il existe un bundle qui intègre l'ODM Doctrine MongoDB dans Symfony et le rend
facile à configurer et à utiliser.

.. note::

    Ce chapitre ressemble beaucoup au :doc:`chapitre sur l'ORM Doctrine2</book/doctrine>`,
    qui traite de la manière dont l'ORM Doctrine peut être utilisé pour persister
    les données dans une base de données relationnelle (par exemple : MySQL). C'est
    en ce sens, si vous persistez dans une base de données relationnelle via l'ORM
    ou dans MongoDB via l'ODM, que les philosophies sont proches.

Installation
------------

Pour utiliser l'ODM MongoDB, vous avez besoin de deux bibliothèques fournies
par Doctrine et d'un bundle qui les intègre dans Symfony.

Installer le bundle avec Composer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour installer DoctrineMongoDBBundle avec Composer, ajoutez simplement le code
suivant à votre fichier `composer.json`::

    {
        "require": {
            "doctrine/mongodb-odm": "1.0.*@dev",
            "doctrine/mongodb-odm-bundle": "3.0.*@dev"
        },
    }

L'instruction ``@dev`` est nécessaire tant qu'une version non-beta du bundle
ou de ODM n'est pas disponible. Selon les besoins de votre projet, d'autres valeurs peuvent etre utilisée.
Elle sont expliquées dans la `documentation de Composer`_ .

Ensuite, vous pouvez installer les nouvelles dépendances en exécutant la commande
``update`` de Composer depuis le répertoire dans lequel est situé le fichier
``composer.json`` :

.. code-block :: bash

    php composer.phar update

Maintenant, Composer téléchargera automatiquement tous les fichiers requis, et les
installera pour vous.

Activer les annotations et le bundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ensuite, activez la bibliothèque des annotations en ajoutant le code suivant au chargeur
automatique (ci-dessous la ligne ``AnnotationRegistry::registerLoader`` existante)::

    // app/autoload.php
    use Doctrine\ODM\MongoDB\Mapping\Driver\AnnotationDriver;
    AnnotationDriver::registerAnnotationClasses();

Tout ce qu'il reste à faire est de mettre à jour votre fichier ``AppKernel.php``,
et d'enregistrer le nouveau bundle::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Doctrine\Bundle\MongoDBBundle\DoctrineMongoDBBundle(),
        );

        // ...
    }

Félicitations ! Vous êtes prêt à travailler.

Configuration
-------------

Pour commencer, vous aurez besoin d'une configuration de base qui installe
le gestionnaire de documents. La manière la plus simple est d'activer l'``auto_mapping``,
qui activera l'ODM MongoDB dans votre application :

.. code-block:: yaml

    # app/config/config.yml
    doctrine_mongodb:
        connections:
            default:
                server: mongodb://localhost:27017
                options: {}
        default_database: test_database
        document_managers:
            default:
                auto_mapping: true

.. note::

    Bien sûr, vous aurez également besoin de vous assurer que le serveur MongoDB
    est démarré. Pour plus de détails, lisez le guide MongoDB `Quick Start`_.

Un exemple simple : un produit
------------------------------

La manière la plus facile de comprendre l'ODM Doctrine MongoDB est de le voir
en action. Dans cette section, vous allez suivre les étapes obligatoires pour
commencer à persister des documents depuis et dans MongoDB.

.. sidebar:: Coder les exemples en même temps

    Si vous souhaitez suivre les exemples au fur et à mesure, créez un
    ``AcmeStoreBundle`` à l'aide de la commande :

    .. code-block:: bash

        php app/console generate:bundle --namespace=Acme/StoreBundle

Créer une classe document
~~~~~~~~~~~~~~~~~~~~~~~~~

Supposons que vous créiez une application qui affiche des produits.
Sans même pensez à Doctrine ou MongoDB, vous savez déjà que
vous aurez besoin d'un objet ``Product`` représentant ces produits. Créez
cette classe dans le répertoire ``Document`` de votre bundle ``AcmeStoreBundle``::

    // src/Acme/StoreBundle/Document/Product.php
    namespace Acme\StoreBundle\Document;

    class Product
    {
        protected $name;

        protected $price;
    }

Cette classe, souvent appelée un « document », ce qui veut dire *une classe basique
qui contient des données* - est simple et remplit les besoins métiers des produits
dans votre application. Cette classe ne peut pas encore être persistée dans
MongoDB, c'est juste une simple classe PHP.

Ajouter des informations de « mapping »
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine vous permet de travailler avec MongoDB d'une manière beaucoup
plus intéressante que de simplement extraire les données comme vous le feriez
pour un tableau. Au lieu de ça, Doctrine vous permet de persister des *objets*
entiers dans MongoDB et d'extraire des objets entiers depuis MongoDB. Cela
fonctionne en associant une classe PHP et ses propriétés aux entrées d'une
collection MongoDB.

Pour que Doctrine soit capable de faire cela, vous avez juste besoin de
créer des « métadonnées », ou une configuration qui indique exactement à
Doctrine comment la classe ``Product`` et ses propriétés devraient être
*associées* à MongoDB. Ces métadonnées peuvent être spécifiées dans différents
formats incluant YAML, XML ou directement dans la classe ``Product`` via les
annotations :

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Document/Product.php
        namespace Acme\StoreBundle\Document;

        use Doctrine\ODM\MongoDB\Mapping\Annotations as MongoDB;

        /**
         * @MongoDB\Document
         */
        class Product
        {
            /**
             * @MongoDB\Id
             */
            protected $id;

            /**
             * @MongoDB\String
             */
            protected $name;

            /**
             * @MongoDB\Float
             */
            protected $price;
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.mongodb.yml
        Acme\StoreBundle\Document\Product:
            fields:
                id:
                    id:  true
                name:
                    type: string
                price:
                    type: float

    .. code-block:: xml

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.mongodb.xml -->
        <doctrine-mongo-mapping xmlns="http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping
                            http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping.xsd">

            <document name="Acme\StoreBundle\Document\Product">
                <field fieldName="id" id="true" />
                <field fieldName="name" type="string" />
                <field fieldName="price" type="float" />
            </document>
        </doctrine-mongo-mapping>

Doctrine vous permet de choisir parmi une très grande variété de types de champs
ayant chacun ses propres options. Pour obtenir des informations sur les types de champs
disponibles, reportez vous à la section :ref:`cookbook-mongodb-field-types`.


.. seealso::

    Vous pouvez aussi regarder la documentation sur les `Bases du Mapping`_ de Doctrine pour
    avoir tous les détails à propos des informations de « mapping ». Si vous utilisez
    les annotations, vous devrez préfixer toutes les annotations avec ``MongoDB\`` 
    (ex: ``MongoDB\String``), ce qui n'est pas montré dans la documentation de
    Doctrine. Vous devrez aussi inclure le morceau de code :
    ``use Doctrine\ODM\MongoDB\Mapping\Annotations as MongoDB;``, qui *importe*
    le préfixe ``MongoDB`` pour les annotations.

Générer les accesseurs et mutateurs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Même si Doctrine sait maintenant comment persister un objet ``Product`` dans
MongoDB, la classe elle-même n'est pas encore très utile. Comme ``Product``
est juste une simple classe PHP, vous devez créer des accesseurs et des mutateurs
(par exemple : ``getName()``, ``setName()``) pour pouvoir accéder à ces propriétés
(car elles sont ``protected``). Heureusement, Doctrine peut faire ça pour vous
en exécutant la commande :

.. code-block:: bash

    php app/console doctrine:mongodb:generate:documents AcmeStoreBundle

Cette commande s'assure que tous les accesseurs et mutateurs sont générés pour
la classe ``Product``. C'est une commande sûre ; vous pouvez l'exécuter
encore et encore : elle ne génèrera que les accesseurs et mutateurs qui n'existent
pas (c-à-d qu'elle ne remplace pas les méthodes existantes).

.. note::

    Doctrine se moque que vos propriétés soient ``protected`` ou ``private``, ou
    même que vous ayez un accesseur ou un mutateur pour une propriété.
    Les accesseurs et mutateurs sont générés ici seulement parce que vous en aurez besoin
    pour intéragir avec vos objets PHP.

Persister des objets dans MongoDB
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant que vous avez un document ``Product`` correspondant aux méthodes accesseurs et
mutateurs, vous êtes prêt à persister des données dans MongoDB. Dans un
contrôleur, c'est très facile. Ajoutez la méthode suivante au ``DefaultController``
du bundle :

.. code-block:: php
    :linenos:

    // src/Acme/StoreBundle/Controller/DefaultController.php
    use Acme\StoreBundle\Document\Product;
    use Symfony\Component\HttpFoundation\Response;
    // ...

    public function createAction()
    {
        $product = new Product();
        $product->setName('A Foo Bar');
        $product->setPrice('19.99');

        $dm = $this->get('doctrine_mongodb')->getManager();
        $dm->persist($product);
        $dm->flush();

        return new Response('Created product id '.$product->getId());
    }

.. note::

    Si vous suivez les exemples au fur et à mesure, vous aurez besoin de
    créer une route qui pointe vers cette action pour voir si elle fonctionne.

Décortiquons cet exemple :

* **lignes 8 à 10** Dans cette section, vous instanciez et travaillez avec l'objet
  ``product`` comme n'importe quel autre objet PHP normal ;

* **ligne 12** Cette ligne récupère un objet *gestionnaire de document* (document manager)
  de Doctrine, qui est chargé de gérer le processus de persistence et de récupération
  des objets vers et depuis MongoDB ;

* **ligne 13** La méthode ``persist()`` dit à Doctrine de « gérer » l'objet ``product``.
  Cela ne requiert pas vraiment qu'une requête soit faite à MongoDB (du moins pas encore) ;

* **ligne 14** Quand la méthode ``flush()`` est appelée, Doctrine regarde dans tous 
  les objets qu'il gère pour savoir s'ils ont besoin d'être persistés dans MongoDB.
  Dans cet exemple, l'objet ``$product`` n'a pas encore été persisté, donc
  le gestionnaire de document fait une requête à MongoDB, ce qui ajoute une nouvelle
  entrée.

.. note::

    En fait, comme Doctrine a connaissance de toutes vos objets gérés, lorsque
    vous appelez la méthode ``flush()``, il calcule un ensemble de changements
    global et éxecute la ou les requêtes les plus efficaces possibles.

Pour la création et la suppression d'objet, le fonctionnement est toujours le même. 
Dans la prochaine section, vous découvrirez que Doctrine est assez rusé pour
mettre à jour les entrées si elles existent déjà dans MongoDB.

.. tip::

    Doctrine fournit une bibliothèque qui vous permet de charger de manière 
    automatisée des données de test dans votre projet (des « fixtures »).
    Pour plus d'informations, lisez :doc:`/bundles/DoctrineFixturesBundle/index`.


Récupérer des objets de MongoDB
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Récupérer un objet depuis MongoDB est encore plus facile. Par exemple,
supposons que vous ayez configuré une route pour afficher un ``Product`` spécifique
en se basant sur la valeur de son ``id``::

    public function showAction($id)
    {
        $product = $this->get('doctrine_mongodb')
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);

        if (!$product) {
            throw $this->createNotFoundException('No product found for id '.$id);
        }

        // faire quelque chose comme envoyer l'objet $product à un template
    }

Lorsque vous effectuez une requête pour un type particulier d'objet, vous utiliserez
toujours ce qui est connu sous le nom de « dépôt » (ou « repository » en anglais).
Dites-vous qu'un dépôt est une classe PHP dont le seul travail est de vous aider
à récupérer des entités d'une certaine classe. Vous pouvez accéder au dépôt d'une
classe d'entités grâce à::

    $repository = $this->get('doctrine_mongodb')
        ->getManager()
        ->getRepository('AcmeStoreBundle:Product');

.. note::

    La chaîne ``AcmeStoreBundle:Product`` est un raccourci que vous pouvez utiliser
    n'importe où dans Doctrine au lieu du nom complet de la classe du document
    (c-à-d ``Acme\StoreBundle\Entity\Product``). Tant que vos documents sont disponibles
    sous l'espace de noms ``Entity`` de votre bundle, cela fonctionnera.

Une fois que vous disposez de votre dépôt, vous pouvez accéder à toute sorte
de méthodes utiles::

    // requête par clé primaire (souvent « id »)
    $product = $repository->find($id);

    /// noms de méthodes dynamiques en se basant sur un nom de colonne
    $product = $repository->findOneById($id);
    $product = $repository->findOneByName('foo');

    // trouver *tous* les produits
    $products = $repository->findAll();

    // trouver un groupe de produits en se basant sur une valeur de colonne
    $products = $repository->findByPrice(19.99);

.. note::

    Bien sûr, vous pouvez aussi générer des requêtes complexes, ce que vous apprendrez
    dans la section :ref:`book-doctrine-queries`.

Vous pouvez aussi profiter des méthodes utiles ``findBy`` et ``findOneBy`` pour
récupérer facilement des objets en se basant sur des conditions multiples::

    // requête pour un produit identifié par son nom et son prix
    $product = $repository->findOneBy(array('name' => 'foo', 'price' => 19.99));

    // requête pour tous les produits identifiés par leur nom, triés par prix
    $product = $repository->findBy(
        array('name' => 'foo'),
        array('price', 'ASC')
    );

Mettre à jour un objet
~~~~~~~~~~~~~~~~~~~~~~

Une fois que vous avez récupéré un objet depuis Doctrine, le mettre à jour est
facile. Supposons que vous ayez une route qui associe l'id d'un produit à
une action de mise à jour dans un contrôleur::

    public function updateAction($id)
    {
        $dm = $this->get('doctrine_mongodb')->getManager();
        $product = $dm->getRepository('AcmeStoreBundle:Product')->find($id);

        if (!$product) {
            throw $this->createNotFoundException('No product found for id '.$id);
        }

        $product->setName('New product name!');
        $dm->flush();

        return $this->redirect($this->generateUrl('homepage'));
    }

Mettre à jour l'objet ne nécessite que trois étapes :

1. Récupérer l'objet depuis Doctrine ;
2. Modifier l'objet ;
3. Apeller la méthode ``flush()`` du gestionnaire de document.

Notez qu'apeller ``$em->persist($product)`` n'est pas nécessaire. Souvenez-vous que
cette méthode dit simplement à Doctrine de gérer, ou « regarder » l'objet ``$product``.
Dans ce cas, comme vous avez récupéré l'objet ``$product`` depuis Doctrine,
il est déjà « surveillé ».

Supprimer un objet
~~~~~~~~~~~~~~~~~~

Supprimer un objet est très similaire, mais requiert un appel à la méthode
``remove()`` du gestionnaire de document::

    $dm->remove($product);
    $dm->flush();

Comme vous vous en doutez, la méthode ``remove()`` signale à Doctrine
que vous voulez supprimer le document de MongoDB. La véritable suppression,
cependant, n'est réellement exécutée que lorsque la méthode ``flush()``
est appelée.

Requêter des objets
-------------------

Comme vous l'avez vu plus haut, la classe de dépôt préconstruite vous permet
de faire des requêtes pour récupérer un ou plusieurs objets en vous basant
sur un certain nombre de différents paramètres. Lorsque c'est suffisant, c'est
la manière la plus simple de retrouver des documents. Bien sûr, vous pouvez
aussi créer des requêtes plus complexes.

Utiliser le constructeur de requêtes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'ODM Doctrine est fourni avec un objet « Constructeur » de requête, qui vous
permet de construire une requête qui retournera exactement les documents que vous
voulez. Si vous utilisez un environnement de développement, vous pourrez aussi
tirer profit de l'autocomplétion lorsque vous taperez les noms de méthode. Dans
un contrôleur::

    $products = $this->get('doctrine_mongodb')
        ->getManager()
        ->createQueryBuilder('AcmeStoreBundle:Product')
        ->field('name')->equals('foo')
        ->limit(10)
        ->sort('price', 'ASC')
        ->getQuery()
        ->execute()

Dans ce cas, la requête retourne dix produits dont le nom est « foo » et qui sont
triés par ordre de prix du plus petit au plus grand.

L'objet ``QueryBuilder`` contient toutes les méthodes nécessaires pour construire
votre requête. Pour plus d'informations sur le constructeur de requêtes de Doctrine,
consultez la documentation de Doctrine : `Query Builder`_. Pour obtenir une liste des
conditions que vous pouvez placer dans une requête, lisez la documentation spécifique
`Opérateurs Conditionnels`_.

Classes de dépôt personnalisées
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans la section précédente, vous avez commencé à construire et à utiliser des
requêtes plus complexes à l'intérieur de vos contrôleurs. Dans le but d'isoler,
de tester et de réutiliser ces requêtes, il est conseillé de créer des dépôts
personnalisés pour vos documents et d'y ajouter les méthodes contenant vos
requêtes.

Pour ce faire, ajoutez le nom de la classe dépôt à vos définitions de « mapping ».

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Document/Product.php
        namespace Acme\StoreBundle\Document;

        use Doctrine\ODM\MongoDB\Mapping\Annotations as MongoDB;

        /**
         * @MongoDB\Document(repositoryClass="Acme\StoreBundle\Repository\ProductRepository")
         */
        class Product
        {
            //...
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.mongodb.yml
        Acme\StoreBundle\Document\Product:
            repositoryClass: Acme\StoreBundle\Repository\ProductRepository
            # ...

    .. code-block:: xml

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.mongodb.xml -->
        <!-- ... -->
        <doctrine-mongo-mapping xmlns="http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping
                            http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping.xsd">

            <document name="Acme\StoreBundle\Document\Product"
                    repository-class="Acme\StoreBundle\Repository\ProductRepository">
                <!-- ... -->
            </document>

        </doctrine-mong-mapping>

Doctrine peut générer la classe de dépôt pour vous en exécutant la commande :

.. code-block:: bash

    php app/console doctrine:mongodb:generate:repositories AcmeStoreBundle

Ensuite, ajoutez une méthode ``findAllOrderedByName()`` à la classe de dépôt
fraîchement générée. Cette méthode requêtera tout les documents ``Product``, en les
classant par ordre alphabétique.

.. code-block:: php

    // src/Acme/StoreBundle/Repository/ProductRepository.php
    namespace Acme\StoreBundle\Repository;

    use Doctrine\ODM\MongoDB\DocumentRepository;

    class ProductRepository extends DocumentRepository
    {
        public function findAllOrderedByName()
        {
            return $this->createQueryBuilder()
                ->sort('name', 'ASC')
                ->getQuery()
                ->execute();
        }
    }

Vous pouvez utiliser cette nouvelle méthode exactement comme les autres méthodes
par défaut de type ``find`` du dépôt::

    $products = $this->get('doctrine_mongodb')
        ->getManager()
        ->getRepository('AcmeStoreBundle:Product')
        ->findAllOrderedByName();


.. note::

    Lorsque vous utilisez une classe de dépôt personnalisée, vous avez toujours
    accès aux méthodes par défaut de type ``find``, comme ``find()`` et ``findAll()``.

Les extensions de Doctrine: Timestampable, Sluggable, etc.
----------------------------------------------------------

Doctrine est très flexible, et il existe un certain nombre d'extensions tierces
qui permettent de faciliter les tâches courantes sur vos documents.
Elles incluent diverses choses comme *Sluggable*, *Timestampable*, *Loggable*,
*Translatable*, et *Tree*.

Pour plus d'informations sur comment trouver et utiliser ces extensions, lisez
l'article du cookbook à ce sujet : :doc:`Utiliser les extensions Doctrine</cookbook/doctrine/common_extensions>`.

.. _cookbook-mongodb-field-types:

Référence des types de champs de Doctrine
-----------------------------------------

Doctrine contient un grand nombre de types de champs. Chacun fait correspondre
un type de données PHP à un `type MongoDB`_ spécifique. Voici *quelques uns*
des types qui sont supportés par Doctrine :

* ``string``
* ``int``
* ``float``
* ``date``
* ``timestamp``
* ``boolean``
* ``file``

Pour plus d'informations, lisez la documentation Doctrine `Types de mapping Doctrine`_.

.. index::
   single: Doctrine; ODM Console Commands
   single: CLI; Doctrine ODM

Commandes de console
--------------------

L'intégration de l'ODM Doctrine2 offre plusieurs commandes de console sous l'espace
de noms ``doctrine:mongodb``. Pour voir la liste de ces commandes, vous pouvez
lancer la console sans aucun argument :

.. code-block:: bash

    php app/console

Une liste des commandes disponibles s'affichera, la plupart d'entre elles
commencent par le préfixe ``doctrine:mongodb``. Vous pouvez obtenir plus d'informations
sur n'importe laquelle de ces commandes (ou n'importe quelle commande Symfony)
en lançant la commande ``help``. Par exemple, pour obtenir des informations
sur la commande ``doctrine:mongodb:query``, lancez :

.. code-block:: bash

    php app/console help doctrine:mongodb:query

.. note::

   Pour pouvoir charger des données de test (« fixtures ») dans MongoDB, vous devrez
   installer le bundle ``DoctrineFixturesBundle``. Pour apprendre comment
   le faire, lisez la documentation : ":doc:`/bundles/DoctrineFixturesBundle/index`"

.. index::
   single: Configuration; Doctrine MongoDB ODM
   single: Doctrine; MongoDB ODM configuration

Configuration
-------------

Pour obtenir des informations détaillées sur les options de configuration disponibles
lorsque vous utilisez l'ODM Doctrine, lisez la section
:doc:`MongoDB Reference</bundles/DoctrineMongoDBBundle/config>`.

Enregistrer des abonnements et des écouteurs d'évènements
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine vous permet d'enregistrer des écouteurs et des abonnements qui sont
notifiés lorsque différents évènements surviennent dans l'ODM Doctrine. Pour
plus d'informations, lisez la `Documentation sur les évènements`_ de Doctrine.

.. tip::

    En plus des évènement de l'ODM, vous pouvez également écouter des évènements
    MongoDB de plus bas niveau, dont vous trouverez les définitions dans la classe
    ``Doctrine\MongoDB\Events``.

.. note::

    Chaque connexion dans Doctrine possède son propre gestionnaire d'évènements,
    qui est partagé entre tous les gestionnaires de document rattachés à cette
    connexion. Les écouteurs et les abonnements peuvent être enregistrés avec tous
    les gestionnaires d'évènements ou seulement un (en utilisant le nom de la
    connexion).

Dans Symfony, vous pouvez enregistrer un écouteur ou un abonnement en créant un
:term:`service` puis en le :ref:`taggant<book-service-container-tags>` avec un tag
spécifique.

*   **écouteur d'évènement**: Utilisez le tag ``doctrine_mongodb.odm.event_listener``
    pour enregistrer un écouteur. L'attribut ``event`` est obligatoire et devrait
    décrire l'évènement à écouter. Par défaut, les écouteurs seront enregistrés
    avec les gestionnaires d'évènement de toutes les connexions. Pour restreindre
    un écouteur à une seule connexeion, spécifiez son nom dans l'attribut ``connection``
    du tag.

    L'attribut ``priority``, dont la valeur par défaut est ``0`` s'il est absent,
    peut être utilisé pour contrôler l'ordre dans lequel les écouteurs sont enregistrés.
    Tout comme dans le :doc:`répartiteur d'évènements</components/event_dispatcher/introduction>` de Symfony2,
    un grand nombre signifie que l'écouteur sera exécuté en premier et les écouteurs
    avec la même priorité seront exécutés dans l'ordre où ils ont été enregistrés
    par le gestionnaire d'évènements.

    Enfin, l'attribut ``lazy``, dont la valeur par défaut est ``false`` s'il est
    absent, peut être utilisé pour demander à ce que l'écouteur soit chargé de
    manière « fainéante » par le gestionnaire d'évènements lorsque l'évènement est
    réparti.

    .. configuration-block::

        .. code-block:: yaml

            services:
                my_doctrine_listener:
                    class:   Acme\HelloBundle\Listener\MyDoctrineListener
                    # ...
                    tags:
                        -  { name: doctrine_mongodb.odm.event_listener, event: postPersist }

        .. code-block:: xml

            <service id="my_doctrine_listener" class="Acme\HelloBundle\Listener\MyDoctrineListener">
                <!-- ... -->
                <tag name="doctrine_mongodb.odm.event_listener" event="postPersist" />
            </service>.

        .. code-block:: php

            $definition = new Definition('Acme\HelloBundle\Listener\MyDoctrineListener');
            // ...
            $definition->addTag('doctrine_mongodb.odm.event_listener', array(
                'event' => 'postPersist',
            ));
            $container->setDefinition('my_doctrine_listener', $definition);

*   **Abonnement à un évènement**: Utilisez le tag ``doctrine_mongodb.odm.event_subscriber``
    pour enregistrer un abonnement. Les abonnements sont chargés d'implémenter
    ``Doctrine\Common\EventSubscriber`` et une méthode pour retourner les évènements
    qu'il surveille. Pour cette raison, ce tag n'a pas d'attribut ``event``, pourtant,
    les attributs ``connection``, ``priority`` et ``lazy`` sont disponibles.

.. note::

    Contrairement aux écouteurs d'évènement Symfony2, le gestionnaire d'évènements
    Doctrine s'attend à ce que chaque écouteur et chaque abonnement ait un nom de
    méthode correspondant à/aux évènement(s) observé(s). Pour cette raison, les tags
    mentionnés ci-dessus n'ont pas d'attribut ``method``.

Intégration du SecurityBundle
-----------------------------

Un fournisseur d'utilisateur est disponible pour vos projets MongoDB. Il fonctionne
exactement de la même façon que le fournisseur d'`entité` décrit dans :doc:`le Cookbook</cookbook/security/entity_provider>`.

.. configuration-block::

    .. code-block:: yaml

        security:
            providers:
                my_mongo_provider:
                    mongodb: {class: Acme\DemoBundle\Document\User, property: username}

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <provider name="my_mongo_provider">
                <mongodb class="Acme\DemoBundle\Document\User" property="username" />
            </provider>
        </config>

Résumé
------

Avec Doctrine, vous pouvez tout d'abord vous focaliser sur vos objets et sur 
leur utilité dans votre application, puis vous occuper de leur persistence dans
MongoDB ensuite. Vous pouvez faire cela car Doctrine vous permet d'utiliser n'importe
quel objet PHP pour stocker vos données et se fie aux métadonnées de « mapping »
pour faire correspondre les données d'un objet à une collection de MongoDB.

Et même si Doctrine tourne autour d'un simple concept, il est incroyablement
puissant, vous permettant de créer des requêtes complexes et de vous abonner
à des évènements qui vous permettent d'effectuer différentes actions au
cours du cycle de vie de vos objets.

Apprenez en plus grâce au Cookbook
----------------------------------

* :doc:`/bundles/DoctrineMongoDBBundle/form`

.. _`MongoDB`:          http://www.mongodb.org/
.. _`documentation`:    http://docs.doctrine-project.org/projects/doctrine-mongodb-odm/en/latest/
.. _`documentation de Composer`: http://getcomposer.org/doc/04-schema.md#minimum-stability
.. _`Quick Start`:      http://www.mongodb.org/display/DOCS/Quickstart
.. _`Bases du Mapping`: http://doctrine-mongodb-odm.readthedocs.org/en/latest/reference/basic-mapping.html
.. _`type MongoDB`: http://us.php.net/manual/en/mongo.types.php
.. _`Types de mapping Doctrine`: http://docs.doctrine-project.org/projects/doctrine-mongodb-odm/en/latest/reference/basic-mapping.html#doctrine-mapping-types
.. _`Query Builder`: http://www.doctrine-project.org/docs/mongodb_odm/1.0/en/reference/query-builder-api.html
.. _`Opérateurs Conditionnels`: http://www.doctrine-project.org/docs/mongodb_odm/1.0/en/reference/query-builder-api.html#conditional-operators
.. _`Documentation sur les évènements`: http://www.doctrine-project.org/docs/mongodb_odm/1.0/en/reference/events.html
