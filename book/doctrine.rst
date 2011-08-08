.. index::
   single: Doctrine

Doctrine et les bases de données ("Le Modèle")
====================================

Voyons les choses en face, les tâches les plus complexes pour n'importe quelle 
application impliquent de lire et de faire persister des informations dans une base 
de données. Heureusement, Symfony intègre `Doctrine`_, une bibliothèque dont
le seul but est de vous fournir des outils puissants afin de vous rendre
la tâche facile. Dans ce chapitre, vous apprendez les bases de la philosophie
de Doctrine et verrez à quel point il peut être facile de travailler
avec une base de données.

.. note::

    Doctrine est totalement découplé de Symfony et son utilistion est optionelle.
    Ce chapitre est entièrement consacré à l'ORM Doctrine, dont l'objectif est de
    mapper vos objets avec une base de donnée relationelle (comme *MySQL*, *PostGesSQL*
    ou *Microsoft SQL*). Si vous preferez utiliser des requêtes SQL brutes, 
    c'est facile, et expliqué dans l'article ":doc:`/cookbook/doctrine/dbal`" du cookbook

    Vous pouvez aussi persister vos données à l'aide de `MongoDB`_ an utilisant la
    bibliothèque ODM de Doctrine. Pour plus d'informations, lisez l'article 
    ":doc:`/cookbook/doctrine/mongodb`" du cookbook

Un simple exemple: un produit
---------------------------~~

La manière la plus facile de comprendre comment Doctrine fonctionne est de le voir
en action.

Dans cette section, vous allez configurer votre base de données, créer un objet
``Product``, le faire persister dans la base de donnée et le récupérer.

.. sidebar:: Coder les exemples en même temps

    Si vous souhaitez suivre les exemples au fur et à mesure, créer un
    ``AcmeStoreBundle`` à l'aide de la commande:
    
    .. code-block:: bash
    
        php app/console generate:bundle --namespace=Acme/StoreBundle

Configurer la base de donnés
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Avant que vous ne soyez réellement prêts, vous devez configurer les paramètres
de connexion à votre base de données. Par convention, ces paramètres sont
habituellement placés dans le fichier ``app/config/parameters.ini`:

.. code-block:: ini

    ;app/config/parameters.ini
    [parameters]
        database_driver   = pdo_mysql
        database_host     = localhost
        database_name     = test_project
        database_user     = root
        database_password = password

.. note::

    Définir la configuration dans ``parametres.ini`` est juste une convention.
    Les paramètres definis dans ce fichiers sont référencés dans le fichier de
    configuration principale au moment de configurer Doctrine:
    
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
~~~~~~~~~~~~~~~~~~~~~~~~

Supposons que vous créez une application affichant des produits

Sans même pensez à Doctrine ou à votre base de données, vous savez déjà que
vous aurez bsoin d'un objet ``Product`` représentant ces derniers. Créez
cette classe dans le répertoire ``Entity`` de votre bundle ``AcmeStoreBundle``::

    // src/Acme/StoreBundle/Entity/Product.php    
    namespace Acme\StoreBundle\Entity;

    class Product
    {
        protected $name;

        protected $price;

        protected $description;
    }

Cette classe - souvent apellée une "entité", ce qui veut dire *une classe basique
qui contient des données* - est simple et remplit les beoins métiers des produits
dans votre application. Cette classe ne peut pas encore être persistée dans une
base de donnée - c'est juste une simple classe PHP.

.. tip::

    Une fois que vous conaissez les concepts derrière Doctrine, vous pouvez l'utiliser
    pour créer ces classes entité pour vous :
    
    .. code-block:: bash
    
        php app/console doctrine:generate:entity AcmeStoreBundle:Product "nom:string(255) prix:float description:text"

.. index::
    single: Doctrine; Adding mapping metadata

.. _book-doctrine-adding-mapping:

Ajouter des information de mapping
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine vous permet de travailler avec des base de données d'une manière beaucoup
plus intéréssantes que de récupérer des lignes basées sur les colonnes de vos tables
dans des tableaux. A la place, Doctrine vous permet de persister des *objets* entiers
dans votre base de données et récupérer ces objets depuis votre base de données. Ce système
fonctionne en associant vos classes PHP avec des table de votre base de données,
et les propriétés de ces classes PHP avec des colonnes de la table, c'est ce que l'on
apelle le mapping :

.. image:: /images/book/doctrine_image_1.png
   :align: center

Pour que Doctrine soit capable de faire ça, vous n'avez qu'à créer des "métadonnées",
ou configurations qui expliquent à Doctrine exactement comment la classe ``Product``
et ses propriétés doivent être mappés avec la base de données. Ces métadonnées
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

    Vous pouvez aussi regarder la `Basic Mapping Documentation`_ de Doctrine pour
    avoir tout les détails à propos des informations de mapping. Si vous utilisez 
    les annotations, vous devrez préfixer toutes les annotations avec ``ORM\`` 
    (ex: ``ORM\Column(..)``), ce qui n'est pas montré dans la documentation de
    Doctrine. Vous devez aussi inclure le morceau de code:
    ``use Doctrine\ORM\Mapping as ORM;``, qui *importe* le préfixe ``ORM``
    pour les annotations.

.. caution::

    Faites bien attention que vos noms de classe et de propriétés ne soient pas
    mappés avec des mots clés SQL (comme ``group`` ou ``user``). Par exemple, si
    le nom de la classe de votre entité est ``Group``, alors, par défaut, le nom
    de la table correspondante sera ``group``, ce qui causera des problèmes SQL
    davec certains moteurs. Lisez la `Reserved SQL keywords documentation`_ de
    Doctrine pour savoir comment échapper ces noms.

.. note::

    Si vous utilisez une autre bibliothèque ou un programme (comme Doxygen) qui
    utilise les annotations, vous devrez placer une annotation ``@IgnoreAnnotation``
    sur votre classe pour indiquer à Symfony quelles annotations il devra ignorer.

    Par exemple, pour empêcher l'annotation ``@fn`` de lancer une exception,
    ajouter le code suivant:

        /**
         * @IgnoreAnnotation("fn")
         */
        class Product

Générer les getters et setters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Même si Doctrine sait maintenant comment persister un objet ``Product`` vers la
base de données, la classe elle même n'est pas encore très utile. Comme ``Product``
est juste une simple classe PHP, vous devez créer des getters et des setters
(ex: ``getName()``, ``setName()``) pour pouvoir accéder à ces propriétés (car elles
sont ``protected``). Heureusement, Doctrine peut faire ça pour vous en lançant :

.. code-block:: bash

    php app/console doctrine:generate:entities Acme/StoreBundle/Entity/Product


Cette commande s'assure que tous les getters et les setters sont générés pour
la classe ``Product``. C'est une commande sure - vous pouvez la lancer
encore et encore : elle ne génèrera que les getters et les setters qui n'existent
pas (c.à.d qu'elle ne remplace pas les méthodes existantes)

.. note::

    Doctrine se moque que vos propriétés soient ``protected`` ou ``private``, ou
    même que vous ayez un getter ou un setter pour une propriété.
    Les getters et setters sont générés ici seulement parce que vous en aurez besoin
    pour intéragir avec vos objets PHP.

.. tip::

    Vous pouvez aussi générer toutes les entités connues (c.à.d n'importe quelle 
    classe PHP ayant des informations de mapping pour Doctrine) d'un bundle ou
    d'un espace de nom entier.

    .. code-block:: bash

        php app/console doctrine:generate:entities AcmeStoreBundle
        php app/console doctrine:generate:entities Acme

Creating the Database Tables/Schema
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous avez maintenant une classe ``Product`` utilisable avec des informations de
mapping permettant à Doctrine de savoir exactement comment le faire persister. Bien sûr,
vous n'avez toujours pas la table ``product`` correspondante dans votre base de données.
Heureusement, Doctrine peut créer automatiquement toute les tables de la base de données
nécéssaires aux entités connues dans votre application. Pour ce faire, lancez :

.. code-block:: bash

    php app/console doctrine:schema:update --force

.. tip::

    En fait, cette commande est incroyablement puissante. Elle compare ce à quoi
    votre base de donnée *devrait* ressembler (en se basant sur le mapping de vos 
    entités) à ce à quoi elle ressemble *vraiment*, et génère le code SQL nécéssaire
    pour *mettre à jour* la base de données vers ce qu'elle doit être. En d'autre termes,
    si vous ajoutez une nouvelle propriété avec des métadonnées mappées sur 
    ``Product`` et relancez cette tâche, elle vous génerera une requête "alter table"
    nécéssaire pour ajouter cette nouvelle colonne à la table ``products`` existante.

    Une façon encore meilleure de profiter de cette fonctionnalité est d'utiliser
    les :doc:`migrations</cookbook/doctrine/migrations>`, qui vous permettent de
    générer ces requêtes SQL et de les stocker dans des classes de migrations
    qui peuvent être lancées systématiquement sur vos serveurs de production
    dans le but de traquer et de migrer vos schémas de base de données de manière
    sure et fiable.

Votre base de donnée a maintenant une table ``product`` totalement fonctionelle
avec des colonnes qui correspondent aux métadonnées que vous avez spécifiées.

Persister des objets dans la base de données
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant que vous avez mappés l'entité ``Product`` avec la table ``product``
correspondante, vous êtes prêts à faire persister des données dans la base
de données. Depuis in contrôleur, c'est très facile. Ajoutez la méthode 
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

        return new Response('Created product id '.$product->getId());
    }

.. note::

    Si vous suivez les exemples au fur et à mesure, vous aurez besoin de
    créer une route qui pointe vers cette action pour voir si elle fonctionne.

Décortiquons cet exemple :

* **lignes 8 à 11** Dans cette section, vous instantiez et travaillez avec l'objet
  ``product`` comme n'importe quel autre objet PHP normal;

* **ligne 13** Cette ligne obtient un objet *gestionnaire d'entités* (entity manager)
  de Doctrine, qui est responsable de la gestion du processus de persistence et de récupération
  des objets vers et depuis la base de données;

* **ligne 14** La méthode ``persist()`` dis à Doctrine de "gérer" l'objet ``product``.
  Cela n'engendre pas vraiment une requête dans la base de données (du moins pas encore).

* **ligne 15** Quand la méthode ``flush()`` est apellée, Doctrine regarde dans tous 
  les objets qu'il gère pour savoir si ils ont besoin d'être persistés dans la base
  de données. Dans cet exemple, l'objet ``$product`` n'a pas encore été persisté,
  le gestionnaire d'entités éxecute donc une requête ``INSERT`` et une ligne est créée dans
  la table ``product``

.. note::

  En fait, comme Doctrine a connaissance de toutes vos entités gérées, lorsque
  vous apellez la méthode ``flush()``, il calcule un ensemble de changement
  global et execute la ou les requêtes les plus efficace possible. Par exemple,
  si vous persistez 100 ``Product`` et que vous apellez ``persist()``, Doctrine
  créera une *unique* requête préparée et la réutilisera pour chaque insertion.
  Ce concept est nommé *Unité de travail*, et est utilisé pour sa rapidité
  et son efficacité.

Pour la création et la suppression d'objet, le fonctionnement le même. 
Dans la prochaine section, vous découvrirez que Doctrine est assez rusé pour
générer une requête ``UPDATE`` si l'enregistrement est déjà présent dans la base
de données.

.. tip::

    Doctrine fournit une bibliothèque qui vous permet de charger de manière 
    automatisée des données de test dans votre projet (c.à.d, des "données d'installation").
    Pour plus d'informations, voir :doc:`/cookbook/doctrine/doctrine_fixtures`.

Récupérer des objets de la base de données
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Récupérer un objet depuis la base de données est encore plus facile. Par exemple,
supposons que vous avez configuré une route pour afficher un ``Product`` spécific
en se basant sur la valeur de son ``id`` ::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);
        
        if (!$product) {
            throw $this->createNotFoundException('No product found for id '.$id);
        }

        // do something, like pass the $product object into a template
    }

Lorsque vous requêtez pour un type particulier d'objet, vous utiliserez toujours
ce qui est connu sous le nom de "dépôt" (ou "repository"). Dites vous qu'un
dépôt est une classe PHP dont le seul travail est de vous aider à récupérer 
des entités d'une certaine classe. Vous pouvez accéder au dépôt d'une classe
d'entités avec ::

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

.. note::

    La chaîne ``AcmeStoreBundle:Product`` est un raccourci que vous pouvez utiliser
    n'importe ou dans Doctrine au lieu du nom complet de la classe de l'entité
    (c.à.d ``Acme\StoreBundle\Entity\Product``). Tant que vos entités sont disponible
    sous l'espace de nom ``Entity`` de votre bundle, cela marchera.

Une fois que vous disposez de votre dépôt, vous pouvez accéder à toute sorte de méthode utiles ::

    // query by the primary key (usually "id")
    $product = $repository->find($id);

    // dynamic method names to find based on a column value
    $product = $repository->findOneById($id);
    $product = $repository->findOneByName('foo');

    // find *all* products
    $products = $repository->findAll();

    // find a group of products based on an arbitrary column value
    $products = $repository->findByPrice(19.99);

.. note::

    Bien sûr, vous pouvez aussi générer des requêtes complexes, ce que vous apprendez
    dans la section :ref:`book-doctrine-queries`.

Vous pouvez aussi profiter des méthodes utiles ``findBy`` et ``findOneBy`` pour
récupérer facilement des méthodes en se basant sur des conditions multiples ::

    // query for one product matching be name and price
    $product = $repository->findOneBy(array('name' => 'foo', 'price' => 19.99));

    // query for all products matching the name, ordered by price
    $product = $repository->findBy(
        array('name' => 'foo'),
        array('price' => 'ASC')
    );

.. tip::

    Lorsque vous effectuez le rendu d'une page, vous pouvez voir combien de
    requêtes sont faites dans le coin bas-droit de votre barre d'oûtils
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
facile. Supposons que vous avez une route qui map l'id d'un produit vers
une action de mise à jour dans un contrôleur ::

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

Mettre à jour l'objet ne nécéssite que trois étapes :

1. Récupérer l'objet depuis Doctrine;
2. Modifier l'objet;
3. apeller la méthode ``flush()`` du gestionnaire d'entités

Notez qu'apeller ``$em->persist($product)`` n'est pas nécéssaire. Rapellez
cette méthode dit simplement à Doctrine de gérer, ou "regarder" l'objet ``$product``.
Dans ce cas, comme vous avez récupéré l'objet ``$product`` depuis Doctrine,
il est déjà surveillé.

Supprimer un objet
~~~~~~~~~~~~~~~~~~

Supprimer un objet est très similaire, mais requiert un appel à la méthode
``remove()`` du gestionnaire d'entités::

    $em->remove($product);
    $em->flush();

Comme vous vous en doutez, la méthode ``remove()`` signale à Doctrine
que vous voulez ssupprimer l'entité de la base de données. La vraie requête
``DELETE``, cependent, n'est réellement executée que lorsque la méthode ``flush()``
est apellée.

.. _`book-doctrine-queries`:

Requêter des objets
-------------------

Vous avez déja vu comment les objets dépôts vous permettaient de lancer des
requêtes basiques sans aucun travail ::

    $repository->find($id);
    
    $repository->findOneByName('Foo');

Bien sûr, Doctrine vous permet également d'écrire des requêtes plus complexes
en utilisant le Doctrine Query Language (DQL). Le DQL est très ressemblant au
SQL excepté que vous devez imaginer que vous requêtez un ou plusieurs objets
d'une classe d'entité (ex: ``Product``) au lieu de requêter des lignes dans
une table (ex: ``product``).

Lorsque vous effectuez une requête à l'aide de Doctrine, deux options s'offrent
à vous : écrire une requête Doctrine pure ou utilisez le constructeur de requête.

Requêter des objets avecDQL
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Imaginont que vous souhaitez récupérer tous les produits dont le prix est supérieur
à ``19.99``, triés du moins au plus cher. Depuis un contrôleur, vous pouvez faire ::

    $em = $this->getDoctrine()->getEntityManager();
    $query = $em->createQuery(
        'SELECT p FROM AcmeStoreBundle:Product p WHERE p.price > :price ORDER BY p.price ASC'
    )->setParameter('price', '19.99');
    
    $products = $query->getResult();

Si vous êtes à l'aide avec SQL, DQL devrait vous sembler très naturel. La plus grosse
différence est que vous devez penser en terme d'"objets" au lieu de lignes dans une
base de données. Pour cette raison, vous effectuez une selection *depuis* ``AcmeStoreBundle:Product``
et l'aliassez par ``p``.

La méthode ``getResult()`` retourne un tableau de résultats. Si vous ne souhaitez
obtenir qu'un seul objet, vous pouvez utiliser la méthode ``getSingleResult)`` à
la place ::

    $product = $query->getSingleResult();

.. caution::

    La méthode ``getSingleResult()`` lève une exception ``Doctrine\ORM\NoResultException``
    si aucun résultat n'est retourné et une exception ``Doctrine\ORM\NonUniqueResultException``
    si *plus* d'un résultat est retourné. Si vous utilisez cette méthode, vous voudrez
    sans doute l'entourer d'un block try-catch pour vous assurer que seul un résultat
    est retourné (si vous requêtez quelque chose qui pourrait retourner plus d'un résutlat) ::
    
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
officielle de Doctrine: `Doctrine Query Language`.

.. sidebar:: Définir des paramètres

    Notez la présence de la méthode ``setParameter()``. En travaillant avec Doctrine,
    la bonne pratique est de définir toutes les valeurs externes en tant que
    "emplacements", ce qui a été fait dans la requête ci-dessus :
    
    .. code-block:: text

        ... WHERE p.price > :price ...

    Vous pouvez alors définir la valeur de l'emplacement ``price`` en apellant la méthode
    ``setParameter()`` ::

        ->setParameter('price', '19.99')

    Utiliser des paramètres au lieu de placer les valeurs directement dans la chaîne
    constituant la requête permet de se prémunir des attaques de type injections SQL
    et devrait *toujours* être fait. Si vous utilisez plusieurs paramètres, vous
    pouvez alors définir leur valeurs d'un seul coup en utilisant la méthode 
    ``setParameters()`` ::

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
en tapant le nom des méthodes. De l'intérieur d'un contrôleur ::

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

    $query = $repository->createQueryBuilder('p')
        ->where('p.price > :price')
        ->setParameter('price', '19.99')
        ->orderBy('p.price', 'ASC')
        ->getQuery();
    
    $products = $query->getResult();

L'objet ``QueryBuilder`` contient toutes les méthodes nécéssaires pour construire
votre requête. En apellant la méthode ``getQuery()``, le constructeur de requêtes
retourne un objet standard ``Query``, qui est identique à celui que vous avez
construit dans la section précédente.

Pour plus d'informations sur le constructeur de requêtes de Doctrine, consultez
la documentation de Doctrine: `Query Builder`_

Classes de dépôt personnalisés
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
que celle utilisez précédemment pour générer les getter et setter 

.. code-block:: bash

    php app/console doctrine:generate:entities Acme

Ensuite, ajoutez une méthode - ``findAllOrderedByName()`` - à la classe fraîchement
générée. Cette méthode requêtera les entités ``Product``, en les classant
dans l'ordre alphabétique.

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

    Le gestionnaire d'entités peut être accédé par ``$this->getEntityManager()`` de
    l'intérieur du dépôt.

Vous pouvez alors utiliser cette nouvelle méthode comme les méthodes par défaut du dépôt ::

    $em = $this->getDoctrine()->getEntityManager();
    $products = $em->getRepository('AcmeStoreBundle:Product')
                ->findAllOrderedByName();

.. note::

    En utilisant un dépôt personnalisé, vous avez toujours accès aux méthodes
    par défaut telles que ``find()`` et ``findAll()``.

.. _`book-doctrine-relations`:

Relations et associations entre les entités
---------------------------------

Supposons que les produits de votre application appartiennent tous à exactement une
"catégorie". Dans ce cas, vous aurez besoin d'un objet ``Category`` et d'une manière
de rattacher un objet ``Product`` à un objet ``Category``. Commencez par créer l'entité
``Category``. Puisque vous savez que vous aurez besoin que Doctrine persist votre
classe, vous pouvez le laisser générer la classe pour vous :

.. code-block:: bash

    php app/console doctrine:generate:entity AcmeStoreBundle:Category "name:string(255)" --mapping-type=yml

Cette commande génère l'entité ``Category`` pour vous, avec un champ ``id``,
un champ ``name`` et les méthodes getter et setter associées.

Métadonnées de mapping de relations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour relier les entités ``Category`` et ``Product``, commencez par créer une
propriété ``products`` dans la classe ``Category`` ::

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

Tout d'abord, comme un objet ``Category`` sera relié à plusieurs objets
``Product``, une propriété tableau ``products`` est ajouté pour stocker
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

Ensuite, comme chaque classe ``Product`` est reliée exactement à un objet ``Category``,
il serait bon d'ajouter une propriété ``$category`` à la classe ``Product`` ::

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

Finallement, maintenant que vous avez ajouté une nouvelle propriété aux classes
``Category`` et ``Product``, dites à Doctrine de regénérer les getter et setter
manquants pour vous :

.. code-block:: bash

    php app/console doctrine:generate:entities Acme

Ignorez les métadonnées de Doctrine pour un moment. Vous avez maintenant deux
classes - ``Category`` et ``Product`` avec une relation naturelle un-vers-plusieurs.
La classe ``Category`` peut contenir un tableau de ``Product`` et l'objet ``Product``
peut contenir un objet ``Category``. En d'autre mots - vous avez construit vos 
classes de manière à ce qu'elles aient un sens pour répondre à vos besoins. Le fait
que les données aient besoin d'être persistées dans une base de données est
toujours secondaire.

Maintenant, regardez les métadonnées au dessus de la propriété ``$category``
dans la classe ``Product``. Les informations ici disent à Doctrine que la classe
associée est ``Category`` et qu'il devrait stocker l'``id`` de la catégorie
dans un champs ``category_id`` présent dans la table ``product``. En d'autre
mots, l'objet ``Category`` associé sera stocké dans la propriété ``$category``,
mais dans les coulisses, Doctrine persistera la relations en stockant la valeur
de l'id de la catégorie dans la colonne ``category_id`` de la table ``product``.

.. image:: /images/book/doctrine_image_2.png
   :align: center

Les métadonnées au dessus de la propriété ``$products`` de l'objet ``Category``
sont moins importantes, et disent simplement à Doctrine de regarder la propriété
``Product.category`` pour comprendre comment l'association est mappée.

Avant que vous ne continuiez, assurez vous que Doctrine ajoute la nouvelle
table ``category``, et la colonne ``product.category_id``, ainsi que la
nouvelle clé étrangère :

.. code-block:: bash

    php app/console doctrine:schema:update --force

.. note::

    Cette tâche ne devrait être réalisé en pratique que lors du développement.
    Pour une méthode plus robuste de mettre à jour systématiquement les base de
    données de production, lisez l'article suivant: :doc:`Doctrine migrations</cookbook/doctrine/migrations>`.

Sauver les entités associées
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant, regardons le code en action. Imaginez que vous êtes dans un contrôleur ::

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
La colonne ``product.category_id`` du nouveau produit est défini à ce que sera
la valeur de l'``id`` de la nouvelle catégorie. Doctrine gèrera la persistence 
de cette relation pour vous.

Récupérer des objets associés
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Lorsque vous récupérez des objets associés, le processus que vous employez
ressemble exactement à celui employé auparavent. Tout d'abord, récupérez
un objet ``$product`` et accéder alors à sa ``Category`` associée ::

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
lorsque vous apellez ``$product->getCategory()->getName()``, Doctrine effectue
une seconde requête silencieusement pour trouver la ``Category`` qui est associé
à ce ``Product``. Il prépare l'objet ``$category`` et vous le renvoie.

.. image:: /images/book/doctrine_image_3.png
   :align: center

Ce qui est important est le fait que vous ayez un accès facile à la catégorie
associée au produit, mais que les données de cette catégorie ne sont réellement
récupérées que lorsque vous demandez la catégorie (on parle alors d'évaluation
fainéante).

Vous pouvez aussi faire cette requête dans l'autre sens ::

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
(c.à.d si vous apellez ``->getProducts()``).
La variable ``$products`` est un tableau de tous les objets ``Product`` associés
à l'objet ``Category`` donné via leur valeurs ``category_id``.

.. sidebar:: Associations et classes mandataires

    Ce mécanisme de "chargement fainéant" est possible car, quand nécéssaire,
    Doctrine retourne un objet "mandataire" (proxy) au lieu des vrais objets.
    Regardez de plus près l'exemple ci-dessus::

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
    réellement besoin (en apellant par exemple ``$category->getName()``).

    Les classes mandataires sont générées par Doctrine et stockées dans
    le répértoire du cache. Même si vous ne remarquerez probablement jamais
    que votre objet ``$category`` est en fait un objet mandataire, il
    est important de le garder à l'esprit.

    Dans la prochaine section, lorsque vous récupérerez les données du produit
    et de la catégorie d'un seul coup (via un *join*), Doctrine retournera la
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
``ProductRepository`` ::

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
requêter un objet ``Product`` et sa ``Category`` associée avec une seule requête ::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->findOneByIdJoinedToCategory($id);

        $category = $product->getCategory();
    
        // ...
    }    

Plus d'informations sur les associations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cette section a introduituit le type le plus commun d'associations entre les
entités, la relation un-ves-plusieurs. Pour plus de détails et d'exemples avancés
sur comment utiliser les autre types de relations (comme ``un-vers-un``, ou ``plusieurs-vers-plusieurs``),
consultez la documentation de Doctrine: `Association Mapping Documentation`_.

.. note::

    Si vous utilisez les annotations, vous devrez préfixer les annotations avec ``ORM\``
    (par exemple: ``ORM\OneToMany``), ce qui n'est pas reflété dans la documentation
    de Doctrine. Vous aurez aussi besoin d'inclure la ligne ``use Doctrine\ORM\Mapping as ORM;``
    pour *importer* le préfixe d'annotation ``ORM``.

Configuration
-------------

Doctrine est hautement configurable, même si vous n'aurez sans doute jamais besoin
de vous embêter avec la plupart de ses options. Pour obtenir des informations
sur la configuration de Doctrine, rendez-vous dans la section : :doc:`reference manual</reference/configuration/doctrine>`.

Callbacks et cycle de vie
-------------------------

Défois, vous voudrez effectuer des actions juste avant ou après qu'une entité 
est inserée, mise à jour ou supprimée. Ce actions sont connues sous le nom
de callbacks du "cycle de vie" (lifecycle), car il s'agit de callbacks (méthodes)
appellables à divers moment du cycle de vie de votre entité (par exemple lorsque
l'entité est inserée, mise à jour, supprimmée, etc.).

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

Désormais, vous pouvez dire à Doctrine d'éxécutez une méthode à n'importe
quel événement du cycle de vie. Par exemple, supposons que vous souhaitez
définit une date ``created`` à la date courrante, uniquement lorsque l'entité
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
apellera automatiqquement la méthode et le champ ``created`` sera défini
en la date courrante.

Vous pouvez faire ainsi pour n'importe quelle autre événement du cycle de
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
et sur leurs callbacks en général, référrez vous à la documentation de 
Doctrine: `Lifecycle Events documentation`_.

.. sidebar:: Callbacks du cycle de vie et traitants d'événements

    Notez que la méthode ``setCreatedValue()`` ne prend pas d'arguments.
    C'est toujours le cas des callbacks du cycle de vie, et c'est intentionnel :
    ces callbacks doivent être de simple méthodes et contiennent des
    transformations de données internes à l'entités (ex: définir un champ
    créé ou mis à jours, générer une valeur de slug...).

    Si vous souhaitez faire des montages plus lourds - comme journaliser ou
    envoyer un mail - vous devez écrire une classe externe et l'enregistrer
    pour écouter ou s'abonner aux évenements, puis lui donner les accès
    à toute les ressources dont vous aurez besoin. Pour plus d'informations,
    voir :doc:`/cookbook/doctrine/event_listeners_subscribers`.

Les extensions de Doctrine: Timestampable, Sluggable, etc.
----------------------------------------------------------

Doctrine est très flexible, et il existe un certain nombre d'extensions tierces
qui permettent de faciliter les tâches courantes sur vos entités.
Elles incluent diverses choses comme *Sluggable*, *Timestampable*, *Loggable*,
*Translatable*, et *Tree*.

Pour plus d'informations sur comment trouver et utiliser ces extensions, regarder
l'article du cookbook à ce sujet : :doc:`using common Doctrine extensions</cookbook/doctrine/common_extensions>`.

.. _book-doctrine-field-types:

Référence des types de champs de Doctrine
----------------------------------------

Doctrine contient un grand nombre de types de champs. Chacun est mappe un type
de données PHP vers un type de colonne spécifique à la base de données que 
vous utilisez.. Les types suivants sont supportés par Doctrine :

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

For more information, see Doctrine's `Mapping Types documentation`_.

Options des champs
~~~~~~~~~~~~~~~~~~

Un ensemble d'option peut être appliqué à chaque champ. Les options
disponibles includent ``type`` (valant ``string`` par défaut), ``name``,
``length``, ``unique`` et ``nullable``. Regardons quelques annotations
en guise d'exemple :

.. code-block:: php-annotations

    /**
     * Une chaîne de caractère de longueur 255 qui ne peut pas être nul
     * (refletant les valeurs par défaut des options "type", "length" et *nullable);
     * 
     * @ORM\Column()
     */
    protected $name;

    /**
     * Une chaîne de longueur 150 qui sera persisté vers une colonne "email_address"
     * et a un index unique.
     *
     * @ORM\Column(name="email_address", unique="true", length="150")
     */
    protected $email;

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
vous pouvez lancer la console sans aucun argumen:

.. code-block:: bash

    php app/console

Une liste des commandes disponibles s'affichera, la plupart d'entre elles
commencent par le préfixe ``doctrine:``. Vous pouvez obtenir plus d'information
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

* ``doctrine:mapping:info`` - vous donne toute les entités dont Doctrine a
  connaisance et si il existe des erreurs basiques dans leur mapping.

* ``doctrine:query:dql`` et ``doctrine:query:sql`` - vous permet d'effectuer
  des commandes DQL ou SQL directement en ligne de commande.

.. note::

    Pour pouvoir charger des données d'installation (fixture), vous devrez 
    installer le bundle ``DoctrineFixtureBundle``. Pour apprendre comment
    le faire, lisez l'entrée suivante du Cookbook : ":doc:`/cookbook/doctrine/doctrine_fixtures`"

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

* :doc:`/cookbook/doctrine/doctrine_fixtures`
* :doc:`/cookbook/doctrine/migrations`
* :doc:`/cookbook/doctrine/mongodb`
* :doc:`/cookbook/doctrine/common_extensions`

.. _`Doctrine`: http://www.doctrine-project.org/
.. _`MongoDB`: http://www.mongodb.org/
.. _`Basic Mapping Documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html
.. _`Query Builder`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/query-builder.html
.. _`Doctrine Query Language`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/dql-doctrine-query-language.html
.. _`Association Mapping Documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/association-mapping.html
.. _`DateTime`: http://php.net/manual/en/class.datetime.php
.. _`Mapping Types Documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html#doctrine-mapping-types
.. _`Property Mapping documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html#property-mapping
.. _`Lifecycle Events documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/events.html#lifecycle-events
.. _`Reserved SQL keywords documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html#quoting-reserved-words
