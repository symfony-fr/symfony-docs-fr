.. index::
   single: Doctrine

Doctrine et les bases de données (« Le Modèle »)
================================================

Voyons les choses en face, les tâches les plus complexes pour n'importe quelle 
application impliquent de lire et de faire persister des informations dans une base 
de données. Heureusement, Symfony intègre `Doctrine`_, une bibliothèque dont
le seul but est de vous fournir des outils puissants afin de vous rendre
la tâche facile. Dans ce chapitre, vous apprendrez les bases de la philosophie
de Doctrine et verrez à quel point il peut être facile de travailler
avec une base de données.

.. note::

    Doctrine est totalement découplé de Symfony et son utilisation est optionelle.
    Ce chapitre est entièrement consacré à l'ORM Doctrine, dont l'objectif est de
    mapper vos objets avec une base de donnée relationnelle (comme *MySQL*, *PostGesSQL*
    ou *Microsoft SQL*). Si vous préferez utiliser des requêtes SQL brutes, 
    c'est facile, et expliqué dans l'article « :doc:`/cookbook/doctrine/dbal` » du cookbook

    Vous pouvez aussi persister vos données à l'aide de `MongoDB`_ en utilisant la
    bibliothèque ODM de Doctrine. Pour plus d'informations, lisez l'article 
    « :doc:`/bundles/DoctrineMongoDBBundle/index`" du cookbook

Un simple exemple : un produit
------------------------------

La manière la plus facile de comprendre comment Doctrine fonctionne est de le voir
en action.

Dans cette section, vous allez configurer votre base de données, créer un objet
``Product``, le faire persister dans la base de données et le récupérer.

.. sidebar:: Coder les exemples en même temps

    Si vous souhaitez suivre les exemples au fur et à mesure, créer un
    ``AcmeStoreBundle`` à l'aide de la commande :
    
    .. code-block:: bash
    
        php app/console generate:bundle --namespace=Acme/StoreBundle

Configurer la base de données
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Avant que vous ne soyez réellement prêt, vous devez configurer les paramètres
de connexion à votre base de données. Par convention, ces paramètres sont
habituellement placés dans le fichier ``app/config/parameters.yml`:

.. code-block:: yaml

    #app/config/parameters.yml
    [parameters]
        database_driver   = pdo_mysql
        database_host     = localhost
        database_name     = test_project
        database_user     = root
        database_password = password

.. note::

    Définir la configuration dans ``parameters.yml`` est juste une convention.
    Les paramètres definis dans ce fichiers sont référencés dans le fichier de
    configuration principal au moment de configurer Doctrine :
    
    .. code-block:: yaml
    
        doctrine:
            dbal:
                driver:   %database_driver%
                host:     %database_host%
                dbname:   %database_name%
                user:     %database_user%
                password: %database_password%
    
    En gardant ces paramètres de connexion dans un fichier séparé, vous pouvez
    facilement garder différentes versions de ce fichier sur chaque serveur.
    Vous pouvez aussi stocker la configuration de la base de données (ou n'importe
    quelle information sensible) en dehors de votre projet, comme par exemple
    dans votre configuration Apache. Pour plus d'informations, consultez
    l'article :doc:`/cookbook/configuration/external_parameters`.

Maintenant que Doctrine connaît vos paramètres de connexion, vous pouvez lui
demander de créer votre base de données :

.. code-block:: bash

    php app/console doctrine:database:create

Créer une classe entité
~~~~~~~~~~~~~~~~~~~~~~~

Supposons que vous créiez une application affichant des produits.

Sans même pensez à Doctrine ou à votre base de données, vous savez déjà que
vous aurez besoin d'un objet ``Product`` représentant ces derniers. Créez
cette classe dans le répertoire ``Entity`` de votre bundle ``AcmeStoreBundle`` :

.. code-block:: php

    // src/Acme/StoreBundle/Entity/Product.php    
    namespace Acme\StoreBundle\Entity;

    class Product
    {
        protected $name;

        protected $price;

        protected $description;
    }

Cette classe - souvent apellée une « entité », ce qui veut dire *une classe basique
qui contient des données* - est simple et remplit les besoins métiers des produits
dans votre application. Cette classe ne peut pas encore être persistée dans une
base de données - c'est juste une simple classe PHP.

.. tip::

    Une fois que vous connaissez les concepts derrière Doctrine, vous pouvez l'utiliser
    pour créer ces classes entité pour vous :
    
    .. code-block:: bash
    
        php app/console doctrine:generate:entity --entity="AcmeStoreBundle:Product" --fields="name:string(255) price:float description:text"

.. index::
    single: Doctrine; Adding mapping metadata

.. _book-doctrine-adding-mapping:

Ajouter des informations de mapping
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine vous permet de travailler avec des bases de données d'une manière beaucoup
plus intéréssante que de récupérer des lignes basées sur les colonnes de vos tables
dans des tableaux. A la place, Doctrine vous permet de persister des *objets* entiers
dans votre base de données et récupérer ces objets depuis votre base de données. Ce système
fonctionne en associant vos classes PHP avec des tables de votre base,
et les propriétés de ces classes PHP avec des colonnes de la table, c'est ce que l'on
appelle le mapping :

.. image:: /images/book/doctrine_image_1.png
   :align: center

Pour que Doctrine soit capable de faire ça, vous n'avez qu'à créer des « métadonnées »,
ou configurations qui expliquent à Doctrine exactement comment la classe ``Product``
et ses propriétés doivent être mappées avec la base de données. Ces métadonnées
peuvent être spécifiées dans de nombreux formats incluant le YAML, XML ou directement
dans la classe ``Product`` avec les annotations :

.. note::

    Un bundle ne peut accepter qu'un format de définition des métadonnées. Par 
    exemple, il n'est pas possible de mélanger des définitions au format YAML
    avec des entités annotées dans les classes PHP.

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Entity/Product.php
        namespace Acme\StoreBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity
         * @ORM\Table(name="product")
         */
        class Product
        {
            /**
             * @ORM\Id
             * @ORM\Column(type="integer")
             * @ORM\GeneratedValue(strategy="AUTO")
             */
            protected $id;

            /**
             * @ORM\Column(type="string", length=100)
             */
            protected $name;

            /**
             * @ORM\Column(type="decimal", scale=2)
             */
            protected $price;

            /**
             * @ORM\Column(type="text")
             */
            protected $description;
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            table: product
            id:
                id:
                    type: integer
                    generator: { strategy: AUTO }
            fields:
                name:
                    type: string
                    length: 100
                price:
                    type: decimal
                    scale: 2
                description:
                    type: text

    .. code-block:: xml

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                            http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="Acme\StoreBundle\Entity\Product" table="product">
                <id name="id" type="integer" column="id">
                    <generator strategy="AUTO" />
                </id>
                <field name="name" column="name" type="string" length="100" />
                <field name="price" column="price" type="decimal" scale="2" />
                <field name="description" column="description" type="text" />
            </entity>
        </doctrine-mapping>

.. tip::

    Le nom de la table est optionel et si il est omis il sera détérminé automatiquement
    en se basant sur le nom de la classe de l'entité.


Doctrine vous permet de choisir parmi une très grande variété de types de champs
chacun avec ses propres options. Pour obtenir des informations sur les types de champs
disponibles, reportez vous à la section :ref:`book-doctrine-field-types`.

.. seealso::

    Vous pouvez aussi regarder la documentation sur les`Bases du Mapping`_ de Doctrine pour
    avoir tout les détails à propos des informations de mapping. Si vous utilisez 
    les annotations, vous devrez préfixer toutes les annotations avec ``ORM\`` 
    (ex: ``ORM\Column(..)``), ce qui n'est pas montré dans la documentation de
    Doctrine. Vous devez aussi inclure le morceau de code :
    ``use Doctrine\ORM\Mapping as ORM;``, qui *importe* le préfixe ``ORM``
    pour les annotations.

.. caution::

    Faites bien attention que vos noms de classe et de propriétés ne soient pas
    mappés avec des mots-clés SQL (comme ``group`` ou ``user``). Par exemple, si
    le nom de la classe de votre entité est ``Group``, alors, par défaut, le nom
    de la table correspondante sera ``group``, ce qui causera des problèmes SQL
    avec certains moteurs. Lisez la documentation sur les `Mots-clé SQL réservés`_ de
    Doctrine pour savoir comment échapper ces noms.

.. note::

    Si vous utilisez une autre bibliothèque ou un programme (comme Doxygen) qui
    utilise les annotations, vous devrez placer une annotation ``@IgnoreAnnotation``
    sur votre classe pour indiquer à Symfony quelles annotations il devra ignorer.

    Par exemple, pour empêcher l'annotation ``@fn`` de lancer une exception,
    ajouter le code suivant::

        /**
         * @IgnoreAnnotation("fn")
         */
        class Product

Générer les getters et setters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Même si Doctrine sait maintenant comment persister un objet ``Product`` vers la
base de données, la classe elle-même n'est pas encore très utile. Comme ``Product``
est juste une simple classe PHP, vous devez créer des getters et des setters
(ex: ``getName()``, ``setName()``) pour pouvoir accéder à ces propriétés (car elles
sont ``protected``). Heureusement, Doctrine peut faire ça pour vous en lançant :

.. code-block:: bash

    php app/console doctrine:generate:entities Acme/StoreBundle/Entity/Product


Cette commande s'assure que tous les getters et les setters sont générés pour
la classe ``Product``. C'est une commande sure - vous pouvez la lancer
encore et encore : elle ne génèrera que les getters et les setters qui n'existent
pas (c.à.d qu'elle ne remplace pas les méthodes existantes)

.. caution::

    La commande ``doctrine:generate:entities`` fait une sauvegarde de ``Product.php``
    appelée ``Product.php~``. Dans certains cas, la présence de ce fichier peut
    créer l'erreur « Cannot redeclare class ». Vous pouvez supprimer ce fichier en
    toute sécurité

Vous pouvez également générer toutes les entitées connues (c.à.d toute classe PHP
qui contient des informations de mapping Doctrine) d'un bundle ou d'un namespace :

.. code-block:: bash
	
    php app/console doctrine:generate:entities AcmeStoreBundle
    php app/console doctrine:generate:entities Acme

.. note::

    Doctrine se moque que vos propriétés soient ``protected`` ou ``private``, ou
    même que vous ayez un getter ou un setter pour une propriété.
    Les getters et setters sont générés ici seulement parce que vous en aurez besoin
    pour intéragir avec vos objets PHP.

Créer les Tables et le Schema
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous avez maintenant une classe ``Product`` utilisable avec des informations de
mapping permettant à Doctrine de savoir exactement comment le faire persister. Bien sûr,
vous n'avez toujours pas la table ``product`` correspondante dans votre base de données.
Heureusement, Doctrine peut créer automatiquement toute les tables de la base de données
nécéssaires aux entités connues dans votre application. Pour ce faire, lancez :

.. code-block:: bash

    php app/console doctrine:schema:update --force

.. tip::

    En fait, cette commande est incroyablement puissante. Elle compare ce à quoi
    votre base de données *devrait* ressembler (en se basant sur le mapping de vos 
    entités) à ce à quoi elle ressemble *vraiment*, et génère le code SQL nécéssaire
    pour *mettre à jour* la base de données vers ce qu'elle doit être. En d'autre termes,
    si vous ajoutez une nouvelle propriété avec des métadonnées mappées sur 
    ``Product`` et relancez cette tâche, elle vous génerera une requête « alter table »
    nécéssaire pour ajouter cette nouvelle colonne à la table ``products`` existante.

    Une façon encore meilleure de profiter de cette fonctionnalité est d'utiliser
    les :doc:`migrations</bundles/DoctrineMigrationsBundle/index>`, qui vous permettent de
    générer ces requêtes SQL et de les stocker dans des classes de migration
    qui peuvent être lancées systématiquement sur vos serveurs de production
    dans le but de traquer et de migrer vos schémas de base de données de manière
    sure et fiable.

Votre base de données a maintenant une table ``product`` totalement fonctionnelle
avec des colonnes qui correspondent aux métadonnées que vous avez spécifiées.

Persister des objets dans la base de données
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant que vous avez mappé l'entité ``Product`` avec la table ``product``
correspondante, vous êtes prêt à faire persister des données dans la base
de données. Depuis un contrôleur, c'est très facile. Ajoutez la méthode 
suivante au ``DefaultController`` du bundle :

.. code-block:: php
    :linenos:

    // src/Acme/StoreBundle/Controller/DefaultController.php
    use Acme\StoreBundle\Entity\Product;
    use Symfony\Component\HttpFoundation\Response;
    // ...
    
    public function createAction()
    {
        $product = new Product();
        $product->setName('A Foo Bar');
        $product->setPrice('19.99');
        $product->setDescription('Lorem ipsum dolor');

        $em = $this->getDoctrine()->getEntityManager();
        $em->persist($product);
        $em->flush();

        return new Response('Produit créé avec id '.$product->getId());
    }

.. note::

    Si vous suivez les exemples au fur et à mesure, vous aurez besoin de
    créer une route qui pointe vers cette action pour voir si elle fonctionne.

Décortiquons cet exemple :

* **lignes 8 à 11** Dans cette section, vous instanciez et travaillez avec l'objet
  ``product`` comme n'importe quel autre objet PHP normal;

* **ligne 13** Cette ligne récupère un objet *gestionnaire d'entités* (entity manager)
  de Doctrine, qui est responsable de la gestion du processus de persistence et de récupération
  des objets vers et depuis la base de données;

* **ligne 14** La méthode ``persist()`` dit à Doctrine de « gérer » l'objet ``product``.
  Cela ne crée pas vraiment de requête dans la base de données (du moins pas encore).

* **ligne 15** Quand la méthode ``flush()`` est appelée, Doctrine regarde dans tous 
  les objets qu'il gère pour savoir si ils ont besoin d'être persistés dans la base
  de données. Dans cet exemple, l'objet ``$product`` n'a pas encore été persisté,
  le gestionnaire d'entités éxecute donc une requête ``INSERT`` et une ligne est créée dans
  la table ``product``

.. note::

  En fait, comme Doctrine a connaissance de toutes vos entités gérées, lorsque
  vous appelez la méthode ``flush()``, il calcule un ensemble de changements
  global et éxecute la ou les requêtes les plus efficaces possibles. Par exemple,
  si vous persistez un total de 100 objets ``Product`` et que vous appelez ensuite
  la méthode ``flush()``, Doctrine créera une *unique* requête préparée et la
  réutilisera pour chaque insertion. Ce concept est nommé *Unité de travail*, et
  est utilisé pour sa rapidité et son efficacité.

Pour la création et la suppression d'objet, le fonctionnement est le même. 
Dans la prochaine section, vous découvrirez que Doctrine est assez rusée pour
générer une requête ``UPDATE`` si l'enregistrement est déjà présent dans la base
de données.

.. tip::

    Doctrine fournit une bibliothèque qui vous permet de charger de manière 
    automatisée des données de test dans votre projet (des « fixtures »).
    Pour plus d'informations, voir :doc:`/bundles/DoctrineFixturesBundle/index`.

Récupérer des objets de la base de données
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Récupérer un objet depuis la base de données est encore plus facile. Par exemple,
supposons que vous avez configuré une route pour afficher un ``Product`` spécifique
en se basant sur la valeur de son ``id`` :

.. code-block:: php

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);
        
        if (!$product) {
            throw $this->createNotFoundException('Produit non trouvé avec id '.$id);
        }

        // faire quelque chose comme envoyer l'objet $product à un template
    }

Lorsque vous requêtez pour un type particulier d'objet, vous utiliserez toujours
ce qui est connu sous le nom de « dépôt » (ou « repository »). Dites vous qu'un
dépôt est une classe PHP dont le seul travail est de vous aider à récupérer 
des entités d'une certaine classe. Vous pouvez accéder au dépôt d'une classe
d'entités avec :

.. code-block:: php

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

.. note::

    La chaîne ``AcmeStoreBundle:Product`` est un raccourci que vous pouvez utiliser
    n'importe ou dans Doctrine au lieu du nom complet de la classe de l'entité
    (c.à.d ``Acme\StoreBundle\Entity\Product``). Tant que vos entités sont disponibles
    sous l'espace de nom ``Entity`` de votre bundle, cela marchera.

Une fois que vous disposez de votre dépôt, vous pouvez accéder à toute sorte de méthodes utiles :

.. code-block:: php

    // requête par clé primaire (souvent "id")
    $product = $repository->find($id);

    // Noms de méthodes dynamique en se basant sur un nom de colonne
    $product = $repository->findOneById($id);
    $product = $repository->findOneByName('foo');

    // trouver *tout* les produits
    $products = $repository->findAll();

    // trouver un groupe de produits en se basant sur une valeur de colonne
    $products = $repository->findByPrice(19.99);

.. note::

    Bien sûr, vous pouvez aussi générer des requêtes complexes, ce que vous apprendrez
    dans la section :ref:`book-doctrine-queries`.

Vous pouvez aussi profiter des méthodes utiles ``findBy`` et ``findOneBy`` pour
récupérer facilement des objets en se basant sur des conditions multiples :

.. code-block:: php

    // query for one product matching be name and price
    $product = $repository->findOneBy(array('name' => 'foo', 'price' => 19.99));

    // query for all products matching the name, ordered by price
    $product = $repository->findBy(
        array('name' => 'foo'),
        array('price' => 'ASC')
    );

.. tip::

    Lorsque vous effectuez le rendu d'une page, vous pouvez voir combien de
    requêtes sont faites dans le coin en bas à droite de votre barre d'outils
    de débuggage.

    .. image:: /images/book/doctrine_web_debug_toolbar.png
       :align: center
       :scale: 50
       :width: 350

    Si vous cliquez sur l'icône, le profileur s'ouvrira, vous montrant les
    requêtes exactes qui ont été faites.

Mettre un objet à jour
~~~~~~~~~~~~~~~~~~~~~~

Une fois que vous avez récupéré un objet depuis Doctrine, le mettre à jour est
facile. Supposons que vous avez une route qui mappe l'id d'un produit vers
une action de mise à jour dans un contrôleur :

.. code-block:: php

    public function updateAction($id)
    {
        $em = $this->getDoctrine()->getEntityManager();
        $product = $em->getRepository('AcmeStoreBundle:Product')->find($id);

        if (!$product) {
            throw $this->createNotFoundException('No product found for id '.$id);
        }

        $product->setName('New product name!');
        $em->flush();

        return $this->redirect($this->generateUrl('homepage'));
    }

Mettre à jour l'objet ne nécessite que trois étapes :

1. Récupérer l'objet depuis Doctrine;
2. Modifier l'objet;
3. Apeller la méthode ``flush()`` du gestionnaire d'entités

Notez qu'apeller ``$em->persist($product)`` n'est pas nécessaire. Rappeler
cette méthode dit simplement à Doctrine de gérer, ou « regarder » l'objet ``$product``.
Dans ce cas, comme vous avez récupéré l'objet ``$product`` depuis Doctrine,
il est déjà surveillé.

Supprimer un objet
~~~~~~~~~~~~~~~~~~

Supprimer un objet est très similaire, mais requiert un appel à la méthode
``remove()`` du gestionnaire d'entités :

.. code-block:: php

    $em->remove($product);
    $em->flush();

Comme vous vous en doutez, la méthode ``remove()`` signale à Doctrine
que vous voulez supprimer l'entité de la base de données. La vraie requête
``DELETE``, cependant, n'est réellement executée que lorsque la méthode ``flush()``
est appelée.

.. _`book-doctrine-queries`:

Requêter des objets
-------------------

Vous avez déja vu comment les objets dépôts vous permettaient de lancer des
requêtes basiques sans aucun travail :

.. code-block:: php

    $repository->find($id);
    
    $repository->findOneByName('Foo');

Bien sûr, Doctrine vous permet également d'écrire des requêtes plus complexes
en utilisant le Doctrine Query Language (DQL). Le DQL est très ressemblant au
SQL excepté que vous devez imaginer que vous requêtez un ou plusieurs objets
d'une classe d'entité (ex: ``Product``) au lieu de requêter des lignes dans
une table (ex: ``product``).

Lorsque vous effectuez une requête à l'aide de Doctrine, deux options s'offrent
à vous : écrire une requête Doctrine pure ou utilisez le constructeur de requête.

Requêter des objets avec DQL
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Imaginons que vous souhaitez récupérer tous les produits dont le prix est supérieur
à ``19.99``, triés du moins cher au plus cher. Depuis un contrôleur, vous pouvez faire :

.. code-block:: php

    $em = $this->getDoctrine()->getEntityManager();
    $query = $em->createQuery(
        'SELECT p FROM AcmeStoreBundle:Product p WHERE p.price > :price ORDER BY p.price ASC'
    )->setParameter('price', '19.99');
    
    $products = $query->getResult();

Si vous êtes à l'aise avec SQL, DQL devrait vous sembler très naturel. La plus grosse
différence est que vous devez penser en terme d'« objets » au lieu de lignes dans une
base de données. Pour cette raison, vous effectuez une sélection *depuis* ``AcmeStoreBundle:Product``
et lui donnez ``p`` pour alias.

La méthode ``getResult()`` retourne un tableau de résultats. Si vous ne souhaitez
obtenir qu'un seul objet, vous pouvez utiliser la méthode ``getSingleResult()`` à
la place :

.. code-block:: php

    $product = $query->getSingleResult();

.. caution::

    La méthode ``getSingleResult()`` lève une exception ``Doctrine\ORM\NoResultException``
    si aucun résultat n'est retourné et une exception ``Doctrine\ORM\NonUniqueResultException``
    si *plus* d'un résultat est retourné. Si vous utilisez cette méthode, vous voudrez
    sans doute l'entourer d'un block try-catch pour vous assurer que seul un résultat
    est retourné (si vous requêtez quelque chose qui pourrait retourner plus d'un résultat) :
    
    .. code-block:: php

        $query = $em->createQuery('SELECT ....')
            ->setMaxResults(1);
        
        try {
            $product = $query->getSingleResult();
        } catch (\Doctrine\Orm\NoResultException $e) {
            $product = null;
        }
        // ...

La syntaxe du DQL est incroyablement puissante, vous permettant d'effectuer simplement
des jointures entre vos entités (le sujet des :ref:`relations<book-doctrine-relations>` sera
abordé plus tard), regrouper, etc. Pour plus d'informations, reportez vous à la documentation
officielle de Doctrine : `Doctrine Query Language`.

.. sidebar:: Définir des paramètres

    Notez la présence de la méthode ``setParameter()``. En travaillant avec Doctrine,
    la bonne pratique est de définir toutes les valeurs externes en tant que
    « emplacements », ce qui a été fait dans la requête ci-dessus :
    
    .. code-block:: text

        ... WHERE p.price > :price ...

    Vous pouvez alors définir la valeur de l'emplacement ``price`` en appelant la méthode
    ``setParameter()`` :

    .. code-block:: php

        ->setParameter('price', '19.99')

    Utiliser des paramètres au lieu de placer les valeurs directement dans la chaîne
    constituant la requête permet de se prémunir des attaques de type injections de SQL
    et devrait *toujours* être fait. Si vous utilisez plusieurs paramètres, vous
    pouvez alors définir leurs valeurs d'un seul coup en utilisant la méthode 
    ``setParameters()`` :

    .. code-block:: php

        ->setParameters(array(
            'price' => '19.99',
            'name'  => 'Foo',
        ))

Utiliser le constructeur de requêtes de Doctrine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Au lieu d'écrire des requêtes directement, vous pouvez alternativement utiliser
le ``QueryBuilder`` (constructeur de requêtes) de Doctrine pour faire le même
travail en utilisant une jolie interface orientée-objet.
Si vous utilisez un IDE, vous pourrez aussi profiter de l'auto-complétion
en tapant le nom des méthodes. De l'intérieur d'un contrôleur :

.. code-block:: php

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

    $query = $repository->createQueryBuilder('p')
        ->where('p.price > :price')
        ->setParameter('price', '19.99')
        ->orderBy('p.price', 'ASC')
        ->getQuery();
    
    $products = $query->getResult();

L'objet ``QueryBuilder`` contient toutes les méthodes nécessaires pour construire
votre requête. En appelant la méthode ``getQuery()``, le constructeur de requêtes
retourne un objet standard ``Query``, qui est identique à celui que vous avez
construit dans la section précédente.

Pour plus d'informations sur le constructeur de requêtes de Doctrine, consultez
la documentation de Doctrine: `Query Builder`_

Classes de dépôt personnalisées
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans les sections précédentes, vous avez commencé à construire et utiliser des
requêtes plus complexes à l'intérieur de vos contrôleurs. Dans le but d'isoler,
de tester et de réutiliser ces requêtes, il est conseillé de créer des dépôts
personnalisés pour vos entités et d'y ajouter les méthodes contenant vos
requêtes.

Pour ce faire, ajouter le nom de la classe dépôt à vos informations de mapping.

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Entity/Product.php
        namespace Acme\StoreBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity(repositoryClass="Acme\StoreBundle\Repository\ProductRepository")
         */
        class Product
        {
            //...
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            repositoryClass: Acme\StoreBundle\Repository\ProductRepository
            # ...

    .. code-block:: xml

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->
        <!-- ... -->
        <doctrine-mapping>

            <entity name="Acme\StoreBundle\Entity\Product"
                    repository-class="Acme\StoreBundle\Repository\ProductRepository">
                    <!-- ... -->
            </entity>
        </doctrine-mapping>

Doctrine peut générer la classe de dépôt pour vous en lançant la même commande
que celle utilisée précédemment pour générer les getters et setters. 

.. code-block:: bash

    php app/console doctrine:generate:entities Acme

Ensuite, ajoutez une méthode - ``findAllOrderedByName()`` - à la classe fraîchement
générée. Cette méthode requêtera les entités ``Product``, en les classant par
ordre alphabétique.

.. code-block:: php

    // src/Acme/StoreBundle/Repository/ProductRepository.php
    namespace Acme\StoreBundle\Repository;

    use Doctrine\ORM\EntityRepository;

    class ProductRepository extends EntityRepository
    {
        public function findAllOrderedByName()
        {
            return $this->getEntityManager()
                ->createQuery('SELECT p FROM AcmeStoreBundle:Product p ORDER BY p.name ASC')
                ->getResult();
        }
    }

.. tip::

    Vous pouvez accéder au gestionnaire d'entités par ``$this->getEntityManager()`` à
    l'intérieur du dépôt.

Vous pouvez alors utiliser cette nouvelle méthode comme les méthodes par défaut du dépôt :

.. code-block:: php

    $em = $this->getDoctrine()->getEntityManager();
    $products = $em->getRepository('AcmeStoreBundle:Product')
                ->findAllOrderedByName();

.. note::

    En utilisant un dépôt personnalisé, vous avez toujours accès aux méthodes
    par défaut telles que ``find()`` et ``findAll()``.

.. _`book-doctrine-relations`:

Relations et associations entre les entités
-------------------------------------------

Supposons que les produits de votre application appartiennent tous à exactement une
« catégorie ». Dans ce cas, vous aurez besoin d'un objet ``Category`` et d'une manière
de rattacher un objet ``Product`` à un objet ``Category``. Commencez par créer l'entité
``Category``. Puisque vous savez que vous aurez besoin que Doctrine persiste votre
classe, vous pouvez le laisser générer la classe pour vous.

.. code-block:: bash

    php app/console doctrine:generate:entity --entity="AcmeStoreBundle:Category" --fields="name:string(255)"

Cette commande génère l'entité ``Category`` pour vous, avec un champ ``id``,
un champ ``name`` et les méthodes getter et setter associées.

Métadonnées de mapping de relations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour relier les entités ``Category`` et ``Product``, commencez par créer une
propriété ``products`` dans la classe ``Category`` :

.. configuration-block::

    .. code-block:: php-annotations
	
        // src/Acme/StoreBundle/Entity/Category.php
        // ...

        use Doctrine\Common\Collections\ArrayCollection;

	class Category
        {
            // ...

            /**
             * @ORM\OneToMany(targetEntity="Product", mappedBy="category")
             */
            protected $products;

            public function __construct()
            {
                $this->products = new ArrayCollection();
            }
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Category.orm.yml
        Acme\StoreBundle\Entity\Category:
            type: entity
            # ...
            oneToMany:
                products:
                    targetEntity: Product
                    mappedBy: category
            # n'oubliez pas d'initialiser la collection dans la méthode __construct() de l'entité


Tout d'abord, comme un objet ``Category`` sera relié à plusieurs objets
``Product``, une propriété tableau ``products`` est ajoutée pour stocker
ces objets ``Product``.
Encore une fois, nous ne faisons pas cela parce que Doctrine en a besoin,
mais plutôt parce qu'il est cohérent dans l'application que chaque ``Category``
contiennent un tableau d'objets ``Product``.

.. note::

    Le code de la méthode ``__construct()`` est important car Doctrine requiert
    que la propriété ``$products`` soit un objet de type ``ArrayCollection``.
    Cet objet ressemble et se comporte *exactement* comme un tableau, mais
    avec quelque flexibilités supplémentaires. Si ça vous dérange, ne vous
    inquiétez pas. Imaginez juste que c'est un ``array`` et vous vous porterez
    bien.


.. tip::

    La valeur targetEntity utilisée plus haut peut faire référence à n'importe
    quelle entitée avec un espace de nom valide, et pas seulement les entitées
    définies dans la même classe. Pour lier une entitée définie dans une autre
    classe ou un autre bundle, entrez l'espace de nom complet dans targetEntity.

Ensuite, comme chaque classe ``Product`` est reliée exactement à un objet ``Category``,
il serait bon d'ajouter une propriété ``$category`` à la classe ``Product`` :


.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Entity/Product.php
         // ...

        class Product
        {
            // ...

            /**
             * @ORM\ManyToOne(targetEntity="Category", inversedBy="products")
             * @ORM\JoinColumn(name="category_id", referencedColumnName="id")
             */
            protected $category;
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            # ...
            manyToOne:
                category:
                    targetEntity: Category
                    inversedBy: products
                    joinColumn:
                        name: category_id
                        referencedColumnName: id

Finallement, maintenant que vous avez ajouté une nouvelle propriété aux classes
``Category`` et ``Product``, dites à Doctrine de regénérer les getters et setters
manquants pour vous :

.. code-block:: bash

    php app/console doctrine:generate:entities Acme

Ignorez les métadonnées de Doctrine pour un moment. Vous avez maintenant deux
classes - ``Category`` et ``Product`` avec une relation naturelle one-to-many.
La classe ``Category`` peut contenir un tableau de ``Product`` et l'objet ``Product``
peut contenir un objet ``Category``. En d'autre termes, vous avez construit vos 
classes de manière à ce qu'elles aient un sens pour répondre à vos besoins. Le fait
que les données aient besoin d'être persistées dans une base de données est
toujours secondaire.

Maintenant, regardez les métadonnées au dessus de la propriété ``$category``
dans la classe ``Product``. Les informations ici disent à Doctrine que la classe
associée est ``Category`` et qu'il devrait stocker l'``id`` de la catégorie
dans un champ ``category_id`` présent dans la table ``product``. En d'autre
termes, l'objet ``Category`` associé sera stocké dans la propriété ``$category``,
mais dans les coulisses, Doctrine persistera la relation en stockant la valeur
de l'id de la catégorie dans la colonne ``category_id`` de la table ``product``.

.. image:: /images/book/doctrine_image_2.png
   :align: center

Les métadonnées de la propriété ``$products`` de l'objet ``Category``
sont moins importantes, et disent simplement à Doctrine de regarder la propriété
``Product.category`` pour comprendre comment l'association est mappée.

Avant que vous ne continuiez, assurez vous que Doctrine ajoute la nouvelle
table ``category``, et la colonne ``product.category_id``, ainsi que la
nouvelle clé étrangère :

.. code-block:: bash

    php app/console doctrine:schema:update --force

.. note::

    Cette tâche ne devrait être réalisée en pratique que lors du développement.
    Pour une façon plus robuste de mettre à jour systématiquement les bases de
    données de production, lisez l'article suivant: :doc:`Doctrine migrations</bundles/DoctrineFixturesBundle/index>`.

Sauver les entités associées
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant, regardons le code en action. Imaginez que vous êtes dans un contrôleur :

.. code-block:: php

    // ...
    use Acme\StoreBundle\Entity\Category;
    use Acme\StoreBundle\Entity\Product;
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
            // relate this product to the category
            $product->setCategory($category);
            
            $em = $this->getDoctrine()->getEntityManager();
            $em->persist($category);
            $em->persist($product);
            $em->flush();
            
            return new Response(
                'Created product id: '.$product->getId().' and category id: '.$category->getId()
            );
        }
    }

Maintenant, une simple ligne est ajoutée aux tables ``category`` et ``product``.
La colonne ``product.category_id`` du nouveau produit est définie comme
la valeur de l'``id`` de la nouvelle catégorie. Doctrine gèrera la persistence 
de cette relation pour vous.

Récupérer des objets associés
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Lorsque vous récupérez des objets associés, le processus que vous employez
ressemble exactement à celui employé auparavant. Tout d'abord, récupérez
un objet ``$product`` et accéder alors à sa ``Category`` associée :

.. code-block:: php

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);

        $categoryName = $product->getCategory()->getName();
        
        // ...
    }

Dans cet exemple, vous requêtez tout d'abord un objet ``Product`` en vous basant
sur l'``id`` du produit. Cela produit une requête *uniquement* pour les
données du produit et hydrate l'objet ``$product`` avec ces données. Plus tard,
lorsque vous appelez ``$product->getCategory()->getName()``, Doctrine effectue
une seconde requête silencieusement pour trouver la ``Category`` qui est associé
à ce ``Product``. Il prépare l'objet ``$category`` et vous le renvoie.

.. image:: /images/book/doctrine_image_3.png
   :align: center

Ce qui est important est le fait que vous ayez un accès facile à la catégorie
associée au produit, mais que les données de cette catégorie ne sont réellement
récupérées que lorsque vous demandez la catégorie (on parle alors de chargement
fainéant ou « lazy loading »).

Vous pouvez aussi faire cette requête dans l'autre sens :

.. code-block:: php

    public function showProductAction($id)
    {
        $category = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Category')
            ->find($id);

        $products = $category->getProducts();
    
        // ...
    }

Dans ce cas, la même chose se produit : vous requêtez tout d'abord un simple
objet ``Category``, et Doctrine effectue alors une seconde requête pour récupérer
les objets ``Product`` associés, mais uniquement une fois que/si vous les demandez
(c.à.d si vous appelez ``->getProducts()``).
La variable ``$products`` est un tableau de tous les objets ``Product`` associés
à l'objet ``Category`` donnés via leur valeurs ``category_id``.

.. sidebar:: Associations et classes mandataires

    Ce mécanisme de « chargement fainéant » est possible car, quand c'est nécessaire,
    Doctrine retourne un objet « mandataire » (proxy) au lieu des vrais objets.
    Regardez de plus près l'exemple ci-dessus :

    .. code-block:: php

        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);

        $category = $product->getCategory();

        // affiche "Proxies\AcmeStoreBundleEntityCategoryProxy"
        echo get_class($category);

    Cet objet mandataire étend le vrai objet ``Category``, et à l'air de
    se comporter exactement de la même manière. La différence est que, en 
    utilisant un objet mandataire, Doctrine peut retarder le requêtage
    des vraies données de la ``Category`` jusqu'a ce que vous en ayez
    réellement besoin (en appelant par exemple ``$category->getName()``).

    Les classes mandataires sont générées par Doctrine et stockées dans
    le répertoire du cache. Même si vous ne remarquerez probablement jamais
    que votre objet ``$category`` est en fait un objet mandataire, il
    est important de le garder à l'esprit.

    Dans la prochaine section, lorsque vous récupérerez les données du produit
    et de la catégorie d'un seul coup (via un *join*), Doctrine retournera
    un *vrai* objet ``Category``, car rien ne sera chargé de manière fainéante.

Faire des jointures avec des enregistrements associés
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans les exemples ci-dessus, deux requêtes ont été faites - une pour l'objet
original (par exemple, une ``Category``), et une pour le(s) objet(s) associé(s)
(par exemple, les objets ``Product``)

.. tip::

    N'oubliez pas que vous pouvez voir toutes les requêtes effectuées en
    utilisant la barre d'outils de débuggage.

Bien sûr, si vous savez dès le début que vous aurez besoin d'accéder aux deux
objets, vous pouvez éviter de produire une deuxième requête en ajoutant
une jointure dans la requête originale. Ajouter le code suivant à la classe
``ProductRepository`` :

.. code-block:: php

    // src/Acme/StoreBundle/Repository/ProductRepository.php
    
    public function findOneByIdJoinedToCategory($id)
    {
        $query = $this->getEntityManager()
            ->createQuery('
                SELECT p, c FROM AcmeStoreBundle:Product p
                JOIN p.category c
                WHERE p.id = :id'
            )->setParameter('id', $id);
        
        try {
            return $query->getSingleResult();
        } catch (\Doctrine\ORM\NoResultException $e) {
            return null;
        }
    }

Maintenant, vous pouvez utiliser cette méthode dans votre contrôleur pour
requêter un objet ``Product`` et sa ``Category`` associée avec une seule requête :

.. code-block:: php

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->findOneByIdJoinedToCategory($id);

        $category = $product->getCategory();
    
        // ...
    }    

Plus d'informations sur les associations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cette section a introduit le type le plus commun d'associations entre les
entités, la relation one-to-many. Pour plus de détails et d'exemples avancés
sur comment utiliser les autre types de relations (comme ``one-to-one``, ou ``many-to-many``),
consultez la documentation de Doctrine: `Association Mapping Documentation`_.

.. note::

    Si vous utilisez les annotations, vous devrez préfixer les annotations avec ``ORM\``
    (par exemple: ``ORM\OneToMany``), ce qui n'est pas spécifié dans la documentation
    de Doctrine. Vous aurez aussi besoin d'inclure la ligne ``use Doctrine\ORM\Mapping as ORM;``
    pour *importer* le préfixe d'annotation ``ORM``.

Configuration
-------------

Doctrine est hautement configurable, même si vous n'aurez sans doute jamais besoin
de vous embêter avec la plupart de ses options. Pour obtenir des informations
sur la configuration de Doctrine, rendez-vous dans la section : :doc:`reference manual</reference/configuration/doctrine>`.

Callbacks et cycle de vie
-------------------------

Parfois, vous voudrez effectuer des actions juste avant ou après qu'une entité 
ait été inserée, mise à jour ou supprimée. Ces actions sont connues sous le nom
de callbacks du « cycle de vie » (lifecycle), car il s'agit de callbacks (méthodes)
qui peuvent être appelées à divers moment du cycle de vie de votre entité (par exemple lorsque
l'entité est inserée, mise à jour, supprimée, etc.).

Si vous utilisez des annotations pour vos métadonnées, commencez par activer
les callbacks du cycle de vie. Si vous utilisez YAML ou XML pour votre mapping,
ce n'est pas nécéssaire :

.. code-block:: php-annotations

    /**
     * @ORM\Entity()
     * @ORM\HasLifecycleCallbacks()
     */
    class Product
    {
        // ...
    }

Désormais, vous pouvez dire à Doctrine d'éxecutez une méthode à n'importe
quel événement du cycle de vie. Par exemple, supposons que vous souhaitez
définir une date ``created`` à la date courante, uniquement lorsque l'entité
est persistée (c.à.d insérée) :

.. configuration-block::

    .. code-block:: php-annotations

        /**
         * @ORM\prePersist
         */
        public function setCreatedValue()
        {
            $this->created = new \DateTime();
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            # ...
            lifecycleCallbacks:
                prePersist: [ setCreatedValue ]

    .. code-block:: xml

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->
        <!-- ... -->
        <doctrine-mapping>

            <entity name="Acme\StoreBundle\Entity\Product">
                    <!-- ... -->
                    <lifecycle-callbacks>
                        <lifecycle-callback type="prePersist" method="setCreatedValue" />
                    </lifecycle-callbacks>
            </entity>
        </doctrine-mapping>

.. note::

    L'exemple ci-dessus suppose que vous avez créé et mappé une propriété
    ``created`` (qui n'est pas montrée ici).

Maintenant, juste avant que l'entité soit initialement persistée, Doctrine
appelera automatiquement la méthode et le champ ``created`` sera défini
à la date courante.

Vous pouvez procéder ainsi pour n'importe quel autre événement du cycle de
vie, ce qui inclut :

* ``preRemove``
* ``postRemove``
* ``prePersist``
* ``postPersist``
* ``preUpdate``
* ``postUpdate``
* ``postLoad``
* ``loadClassMetadata``

Pour plus d'informations sur la signification de ces événements du cycle de vie
et sur leurs callbacks en général, réferrez vous à la documentation de 
Doctrine: `Lifecycle Events documentation`_.

.. sidebar:: Callbacks du cycle de vie et traitants d'événements

    Notez que la méthode ``setCreatedValue()`` ne prend pas d'argument.
    C'est toujours le cas des callbacks du cycle de vie, et c'est intentionnel :
    ces callbacks doivent être de simple méthodes et contiennent des
    transformations de données internes à l'entité (ex: définir un champ
    créé ou mis à jour, générer une valeur de slug...).

    Si vous souhaitez faire des montages plus lourds - comme une identification ou
    envoyer un mail - vous devez écrire une classe externe et l'enregistrer
    pour écouter ou s'abonner aux évenements, puis lui donner les accès
    à toutes les ressources dont vous aurez besoin. Pour plus d'informations,
    voir :doc:`/cookbook/doctrine/event_listeners_subscribers`.

Les extensions de Doctrine: Timestampable, Sluggable, etc.
----------------------------------------------------------

Doctrine est très flexible, et il existe un certain nombre d'extensions tierces
qui permettent de faciliter les tâches courantes sur vos entités.
Elles incluent diverses choses comme *Sluggable*, *Timestampable*, *Loggable*,
*Translatable*, et *Tree*.

Pour plus d'informations sur comment trouver et utiliser ces extensions, regardez
l'article du cookbook à ce sujet : :doc:`using common Doctrine extensions</cookbook/doctrine/common_extensions>`.

.. _book-doctrine-field-types:

Référence des types de champs de Doctrine
----------------------------------------

Doctrine contient un grand nombre de types de champs. Chacun mappe un type
de données PHP vers un type de colonne spécifique à la base de données que 
vous utilisez. Les types suivants sont supportés par Doctrine :

* **Chaînes de caractères**

  * ``string`` (utilisé pour des chaînes courtes)
  * ``text`` (utilisé pour des chaînes longues)

* **Nombres**

  * ``integer``
  * ``smallint``
  * ``bigint``
  * ``decimal``
  * ``float``

* **Dates et heures** (ces champs utilisent un objet PHP `DateTime`_)

  * ``date``
  * ``time``
  * ``datetime``

* **Autre types**

  * ``boolean``
  * ``object`` (serialisé et stocké dans un champ ``CLOB``)
  * ``array`` (serialisé et stocké dans un champ ``CLOB``)

Pour plus d'informations, lisez la documentation Doctrine `Types de mapping Doctrine`_.

Options des champs
~~~~~~~~~~~~~~~~~~

Un ensemble d'options peut être appliqué à chaque champ. Les options
disponibles incluent ``type`` (valant ``string`` par défaut), ``name``,
``length``, ``unique`` et ``nullable``. Regardons quelques exemples :

.. configuration-block::

    .. code-block:: php-annotations
        /**
        * Une chaîne de caractères de longueur 255 qui ne peut pas être nulle
        * (refletant les valeurs par défaut des options "type", "length" et *nullable);
        * 
        * @ORM\Column()
        */
        protected $name;

        /**
        * Une chaîne de longueur 150 qui sera persistée vers une colonne "email_address"
        * et a un index unique.
        *
        * @ORM\Column(name="email_address", unique="true", length="150")
        */
        protected $email;

    .. code-block:: yaml
 
        fields:
            # Une chaîne de caractères de longueur 255 qui ne peut pas être nulle
            # (refletant les valeurs par défaut des options "type", "length" et *nullable);
            # l'attribut type est nécessaire dans une configuration en yaml
            name:
                type: string

            # Une chaîne de longueur 150 qui sera persistée vers une colonne "email_address"
            # et a un index unique.
            email:
                type: string
                column: email_address
                length: 150
                unique: true


.. note::

    Il existe d'autre options qui ne sont pas listées ici. Pour plus de détails,
    voir `Property Mapping documentation`_.


.. index::
   single: Doctrine; ORM Console Commands
   single: CLI; Doctrine ORM

Commandes en console
--------------------

L'intégration de l'ORM Doctrine2 offre plusieurs commandes en console
sous l'espace de nom ``doctrine``. Pour voir la liste de ces commandes,
vous pouvez lancer la console sans aucun argument :

.. code-block:: bash

    php app/console

Une liste des commandes disponibles s'affichera, la plupart d'entre elles
commencent par le préfixe ``doctrine:``. Vous pouvez obtenir plus d'informations
sur n'importe laquelle de ces commandes (ou n'importe quelle commande Symfony)
en lançant la commande ``help``. Par exemple, pour obtenir des informations
sur la commande ``doctrine:database:create``, lancez :

.. code-block:: bash

    php app/console help doctrine:database:create

Quelques commandes notables ou intéréssantes incluent :

* ``doctrine:ensure-production-settings`` - teste si l'environnement actuel
  est efficacement configuré pour la production. Cela devrait toujours être
  lancé dans un environement `prod` :
  
  .. code-block:: bash
  
    php app/console doctrine:ensure-production-settings --env=prod

* ``doctrine:mapping:import`` - permet à Doctrine d'introspecter une
  base de données existante pour créer les informations de mapping.
  Pour plus d'informations, voir :doc:`/cookbook/doctrine/reverse_engineering`.

* ``doctrine:mapping:info`` - vous donne toutes les entités dont Doctrine a
  connaisance et s'il existe des erreurs basiques dans leur mapping.

* ``doctrine:query:dql`` et ``doctrine:query:sql`` - vous permet d'effectuer
  des commandes DQL ou SQL directement en ligne de commande.

.. note::

    Pour pouvoir charger des données d'installation (fixtures), vous devrez 
    installer le bundle ``DoctrineFixtureBundle``. Pour apprendre comment
    le faire, lisez le chapitre du Cookbook : ":doc:`/bundles/DoctrineFixturesBundle/index`"

Résumé
------

Avec Doctrine, vous pouvez tout d'abord vous focaliser sur vos objets et sur 
leur utilité dans votre application, puis vous occuper de leur persistence
ensuite. Vous pouvez faire cela car Doctrine vous permet d'utiliser n'importe
quel objet PHP pour stocker vos données et se fie aux métadonnées de mapping
pour faire correspondre les données d'un objet à une table particulière de
la base de données.

Et même si Doctrine tourne autour d'un simple concept, il est incroyablement
puissant, vous permettant de créer des requêtes complexes et de vous abonner
à des événements qui vous permettent d'effectuer différentes actions au
cours du cycle de vie de vos objets.

Pour plus d'informations sur Doctrine, lisez la section *Doctrine* du 
Cookbook: :doc:`cookbook</cookbook/index>`, qui inclut les articles 
suivant :

* :doc:`/bundles/DoctrineFixturesBundle/index`
* :doc:`/cookbook/doctrine/common_extensions`

.. _`Doctrine`: http://www.doctrine-project.org/
.. _`MongoDB`: http://www.mongodb.org/
.. _`Bases du Mapping`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html
.. _`Query Builder`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/query-builder.html
.. _`Doctrine Query Language`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/dql-doctrine-query-language.html
.. _`Association Mapping Documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/association-mapping.html
.. _`DateTime`: http://php.net/manual/en/class.datetime.php
.. _`Types de mapping Doctrine`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html#doctrine-mapping-types
.. _`Property Mapping documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html#property-mapping
.. _`Lifecycle Events documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/events.html#lifecycle-events
.. _`Mots-clé SQL réservés`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html#quoting-reserved-words
