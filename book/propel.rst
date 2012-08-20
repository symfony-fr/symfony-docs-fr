.. index::
   single: Propel

Propel et les bases de données
==============================

Regardons la vérité en face, l'une des tâches les plus communes et difficiles
pour quelconque application implique de devoir persister et lire de l'information
dans et depuis une base de données. Symfony2 ne vient pas avec un ORM intégré mais
l'intégration de Propel reste néanmoins aisée. Pour commencer, lisez
`Travailler Avec Symfony2`_.

Un Exemple Simple : Un Produit
------------------------------

Dans cette section, vous allez configurer votre base de données, créer un
objet ``Product``, le persister dans la base de données et ensuite le
récupérer.

.. sidebar:: Codez en même temps que vous lisez cet exemple

    Si vous voulez codez en même temps que vous parcourez l'exemple de ce
    chapitre, créez un ``AcmeStoreBundle`` via : 

    .. code-block:: bash

        $ php app/console generate:bundle --namespace=Acme/StoreBundle

Configurer la Base de Données
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Avant de pouvoir démarrer, vous allez avoir besoin de configurer les informations
de connexion de votre base de données. Par convention, ces informations sont
généralement configurées dans un fichier ``app/config/parameters.yml`` :

.. code-block:: yaml

    # app/config/parameters.yml
    parameters:
        database_driver:   mysql
        database_host:     localhost
        database_name:     test_project
        database_user:     root
        database_password: password
        database_charset:  UTF8

.. note::

    Définir la configuration via ``parameters.yml`` n'est qu'une convention. Les
    paramètres définis dans ce fichier sont référencés par le fichier de la
    configuration principale lorsque vous définissez Propel :

    .. code-block:: yaml

        propel:
            dbal:
                driver:     %database_driver%
                user:       %database_user%
                password:   %database_password%
                dsn:        %database_driver%:host=%database_host%;dbname=%database_name%;charset=%database_charset%

Maintenant que Propel connaît votre base de données, Symfony2 peut créer cette dernière
pour vous :

.. code-block:: bash

    $ php app/console propel:database:create

.. note::

    Dans cet exemple, vous avez une connexion configurée, nommée ``default``.
    Si vous voulez configurer plus d'une connexion, lisez la partie sur la
    configuration <Travailler avec Symfony2 - Configuration>`_.

Créer une Classe Modèle
~~~~~~~~~~~~~~~~~~~~~~~

Dans le monde de Propel, les classes ActiveRecord sont connues en tant que
**modèles** car les classes générées par Propel contiennent un peu de logique
métier.

.. note::

    Pour les gens qui utilisent Symfony2 avec Doctrine2, les **modèles** sont
    équivalents aux **entités**.

Supposons que vous construisiez une application dans laquelle des produits
doivent être affichés. Tout d'abord, créez un fichier ``schema.xml`` dans le
répertoire ``Resources/config`` de votre ``AcmeStoreBundle`` :

.. code-block:: xml

    <?xml version="1.0" encoding="UTF-8"?>
    <database name="default" namespace="Acme\StoreBundle\Model" defaultIdMethod="native">
        <table name="product">
            <column name="id" type="integer" required="true" primaryKey="true" autoIncrement="true" />
            <column name="name" type="varchar" primaryString="true" size="100" />
            <column name="price" type="decimal" />
            <column name="description" type="longvarchar" />
        </table>
    </database>

Construire le Modèle
~~~~~~~~~~~~~~~~~~~~

Après avoir créé votre ``schema.xml``, générez votre modèle à partir de ce
dernier en exécutant :

.. code-block:: bash

    $ php app/console propel:model:build

Cela va générer chaque classe modèle afin que vous puissiez développer
rapidement votre application dans le répertoire ``Model/`` de votre
bundle ``AcmeStoreBundle``.

Créer les Tables et le Schéma de la Base de Données
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant, vous avez une classe ``Product`` utilisable et tout ce dont vous
avez besoin pour persister un produit. Bien sûr, pour le moment, vous ne disposez pas
de la table ``product`` correspondante dans votre base de données. Heureusement,
Propel peut automatiquement créer toutes les tables de base de données nécessaires
à chaque modèle connu de votre application. Pour effectuer cela, exécutez :

.. code-block:: bash

    $ php app/console propel:sql:build
    $ php app/console propel:sql:insert --force

Votre base de données possède désormais une table ``product`` entièrement
fonctionnelle avec des colonnes qui correspondent au schéma que vous avez
spécifié.

.. tip::

    Vous pouvez exécuter les trois dernières commandes de manière combinée
    en utilisant la commande suivante : ``php app/console propel:build --insert-sql``.

Persister des Objets dans la Base de Données
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant que vous avez un objet ``Product`` et une table ``product``
correspondante, vous êtes prêt à persister des données dans la base de données.
Depuis un contrôleur, cela est assez facile. Ajoutez la méthode suivante au
``DefaultController`` du bundle créé plus haut::

    // src/Acme/StoreBundle/Controller/DefaultController.php
    use Acme\StoreBundle\Model\Product;
    use Symfony\Component\HttpFoundation\Response;
    // ...

    public function createAction()
    {
        $product = new Product();
        $product->setName('A Foo Bar');
        $product->setPrice(19.99);
        $product->setDescription('Lorem ipsum dolor');

        $product->save();

        return new Response('Created product id '.$product->getId());
    }

Dans ce bout de code, vous instanciez et travaillez avec l'objet ``product``.
Lorsque vous appelez la méthode ``save()`` sur ce dernier, vous persistez le
produit dans la base de données. Pas besoin d'utiliser d'autres services,
l'objet sait comment se persister lui-même.

.. note::

    Si vous codez tout en lisant cet exemple, vous allez avoir besoin de
    créer une :doc:`route <routing>` qui pointe vers cette action pour
    la voir fonctionner.

Récupérer des Objets depuis la Base de Données
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Récupérer un objet depuis la base de données est encore plus simple. Par exemple,
supposons que vous ayez configuré une route pour afficher un ``Produit`` spécifique
basé sur la valeur de son ``id``::
    
    use Acme\StoreBundle\Model\ProductQuery;
    
    public function showAction($id)
    {
        $product = ProductQuery::create()
            ->findPk($id);
    
        if (!$product) {
            throw $this->createNotFoundException('No product found for id '.$id);
        }
    
        // faites quelque chose, comme passer l'objet $product à un template
    }

Mettre à jour un Objet
~~~~~~~~~~~~~~~~~~~~~~

Une fois que vous avez récupéré un objet depuis Propel, le mettre à jour est
facile. Supposons que vous ayez une route qui fasse correspondre un « id » de
produit à une action de mise à jour dans un contrôleur::

    use Acme\StoreBundle\Model\ProductQuery;
    
    public function updateAction($id)
    {
        $product = ProductQuery::create()
            ->findPk($id);
    
        if (!$product) {
            throw $this->createNotFoundException('No product found for id '.$id);
        }
    
        $product->setName('New product name!');
        $product->save();
    
        return $this->redirect($this->generateUrl('homepage'));
    }

Mettre à jour un objet implique seulement trois étapes :

#. récupérer l'objet depuis Propel ;
#. modifier l'objet ;
#. le sauvegarder.

Supprimer un Objet
~~~~~~~~~~~~~~~~~~

Supprimer un objet est très similaire, excepté que cela requiert un appel à la
méthode ``delete()`` sur l'objet::

    $product->delete();

Effectuer des requêtes pour récupérer des Objets
------------------------------------------------

Propel fournit des classes ``Query`` générées pour exécuter aussi bien des requêtes
basiques que des requêtes complexes, et cela sans aucun effort de votre part::
    
    \Acme\StoreBundle\Model\ProductQuery::create()->findPk($id);
    
    \Acme\StoreBundle\Model\ProductQuery::create()
        ->filterByName('Foo')
        ->findOne();

Imaginez que vous souhaitiez effectuer une requête sur des produits coûtant plus
de 19.99, ordonnés du moins cher au plus cher. Depuis l'un de vos contrôleurs,
faites ce qui suit::

    $products = \Acme\StoreBundle\Model\ProductQuery::create()
        ->filterByPrice(array('min' => 19.99))
        ->orderByPrice()
        ->find();

Très facilement, vous obtenez vos produits en construisant une requête de manière 
orientée objet.
Aucun besoin de perdre du temps à écrire du SQL ou quoi que ce soit d'autre,
Symfony2 offre une manière de programmer totalement orientée objet et Propel
respecte la même philosophie en fournissant une couche d'abstraction ingénieuse.

Si vous voulez réutiliser certaines requêtes, vous pouvez ajouter vos
propres méthodes à la classe ``ProductQuery``::

    // src/Acme/StoreBundle/Model/ProductQuery.php
    
    class ProductQuery extends BaseProductQuery
    {
        public function filterByExpensivePrice()
        {
            return $this
                ->filterByPrice(array('min' => 1000))
        }
    }

Mais notez que Propel génère beaucoup de méthodes pour vous et qu'une
simple méthode ``findAllOrderedByName()`` peut être écrite sans aucun
effort::

    \Acme\StoreBundle\Model\ProductQuery::create()
        ->orderByName()
        ->find();

Relations/Associations
----------------------

Supposez que les produits de votre application appartiennent tous à une seule
« catégorie ». Dans ce cas, vous aurez besoin d'un objet ``Category`` et d'une
manière de lier un objet ``Product`` à un objet ``Category``.

Commencez par ajouter la définition de ``category`` dans votre ``schema.xml`` :


.. code-block:: xml

    <database name="default" namespace="Acme\StoreBundle\Model" defaultIdMethod="native">
        <table name="product">
            <column name="id" type="integer" required="true" primaryKey="true" autoIncrement="true" />
            <column name="name" type="varchar" primaryString="true" size="100" />
            <column name="price" type="decimal" />
            <column name="description" type="longvarchar" />
    
            <column name="category_id" type="integer" />
            <foreign-key foreignTable="category">
                <reference local="category_id" foreign="id" />
            </foreign-key>
        </table>
    
        <table name="category">
            <column name="id" type="integer" required="true" primaryKey="true" autoIncrement="true" />
            <column name="name" type="varchar" primaryString="true" size="100" />
       </table>
    </database>

Créez les classes :

.. code-block:: bash

    $ php app/console propel:model:build

Assumons que vous ayez des produits dans votre base de données, vous ne souhaitez
pas les perdre. Grâce aux migrations, Propel va être capable de mettre à jour votre
base de données sans perdre aucune données.

.. code-block:: bash

    $ php app/console propel:migration:generate-diff
    $ php app/console propel:migration:migrate

Votre base de données a été mise à jour, vous pouvez continuer à écrire
votre application.

Sauvegarder des Objets Liés
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant, voyons le code en action. Imaginez que vous soyez dans un
contrôleur::

    // ...
    use Acme\StoreBundle\Model\Category;
    use Acme\StoreBundle\Model\Product;
    use Symfony\Component\HttpFoundation\Response;
    // ...
    
    class DefaultController extends Controller
    {
        public function createProductAction()
        {
            $category = new Category();
            $category->setName('Main Products');
    
            $product = new Product();
            $product->setName('Foo');
            $product->setPrice(19.99);
            // lie ce produit à la catégorie définie plus haut
            $product->setCategory($category);
    
            // sauvegarde l'ensemble
            $product->save();
    
            return new Response(
                'Created product id: '.$product->getId().' and category id: '.$category->getId()
            );
        }
    }

Maintenant, une seule ligne est ajoutée aux deux tables ``category`` et ``product``.
La colonne ``product.category_id`` pour le nouveau produit est définie avec l'id
de la nouvelle catégorie. Propel gère la persistance de cette relation pour vous.

Récupérer des Objets Liés
~~~~~~~~~~~~~~~~~~~~~~~~~

Lorsque vous avez besoin de récupérer des objets liés, votre processus ressemble 
à la récupération d'un attribut dans l'exemple précédent. Tout d'abord, 
récupérez un objet ``$product`` et ensuite accédez à sa ``Category`` liée::

    // ...
    use Acme\StoreBundle\Model\ProductQuery;
    
    public function showAction($id)
    {
        $product = ProductQuery::create()
            ->joinWithCategory()
            ->findPk($id);
    
        $categoryName = $product->getCategory()->getName();
    
        // ...
    }

Notez que dans l'exemple ci-dessus, seulement une requête a été effectuée.

Plus d'informations sur les Associations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous trouverez plus d'informations sur les relations en lisant le chapitre
dédié sur les `Relations`_.

Callbacks et Cycles de Vie
--------------------------

De temps en temps, vous avez besoin d'effectuer une action juste avant ou
juste après qu'un objet soit inséré, mis à jour, ou supprimé. Ces types d'actions
sont connus en tant que callbacks de « cycles de vie » ou « hooks », comme ce sont
des méthodes callbacks que vous devez exécuter à différentes étapes du cycle de vie
d'un objet (par exemple : l'objet est inséré, mis à jour, supprimé, etc).

Pour ajouter un « hook », ajoutez simplement une nouvelle méthode à la classe
de l'objet::

    // src/Acme/StoreBundle/Model/Product.php
    
    // ...
    
    class Product extends BaseProduct
    {
        public function preInsert(\PropelPDO $con = null)
        {
            // faites quelque chose avant que l'objet soit inséré
        }
    }

Propel fournit les « hooks » suivants :

* ``preInsert()`` code exécuté avant l'insertion d'un nouvel objet
* ``postInsert()`` code exécuté après l'insertion d'un nouvel objet
* ``preUpdate()`` code exécuté avant la mise à jour d'un objet existant
* ``postUpdate()`` code exécuté après la mise à jour d'un objet existant
* ``preSave()`` code exécuté avant la sauvegarde d'un objet (nouveau ou existant)
* ``postSave()`` code exécuté après la sauvegarde d'un objet (nouveau ou existant)
* ``preDelete()`` code exécuté avant la suppression d'un objet
* ``postDelete()`` code exécuté après la suppression d'un objet


Comportements
-------------

Tous les comportements fournis par Propel fonctionnent avec Symfony2. Pour avoir
plus d'informations sur comment utiliser les comportements Propel, jetez un oeil
à la `Section de Référence des Comportements`_.

Commandes
---------

Vous devriez lire la section dédiée aux `commandes Propel dans Symfony2`_.

.. _`Travailler Avec Symfony2`: http://www.propelorm.org/cookbook/symfony2/working-with-symfony2.html#installation
.. _`Travailler avec Symfony2 - Configuration`: http://www.propelorm.org/cookbook/symfony2/working-with-symfony2.html#configuration
.. _`Relations`: http://www.propelorm.org/documentation/04-relationships.html
.. _`Section de Référence des Comportements`: http://www.propelorm.org/documentation/#behaviors_reference
.. _`commandes Propel dans Symfony2`: http://www.propelorm.org/cookbook/symfony2/working-with-symfony2#the_commands
