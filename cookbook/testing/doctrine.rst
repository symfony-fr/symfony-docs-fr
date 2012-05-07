.. index::
   single: Tests; Doctrine

Comment tester les dépôts Doctrine
==================================

Test de manière unitaire les dépôts Doctrine à l’intérieur d'un projet Symfony
n'est pas une tâche aisée et demande une préparation minutieuse. Ainsi, pour charger
un dépôt vous devez charger vos entités, un gestionnaire d'entité ainsi que différents
objets comme entre autres une connexion.

Pour tester votre dépôt, vous avez deux options :

1) **Test fonctionnel** : Cela inclut l'utilisation d'une connexion à une base de
   donnée réelle avec des objets réels. C'est simple à réaliser et vous permet de
   tout tester mais est en contrepartie relativement lent à l’exécution.
   Voir :ref:`cookbook-doctrine-repo-functional-test`.

2) **Test unitaire**: Tester de manière unitaire est plus rapide à exécuter et plus précis
   quant à la manière dont vous effectuez vos tests. Cela requiert une configuration
   plus approfondie, que nous détaillerons dans ce document. Cette façon permer de tester
   uniquement les méthodes qui, par exemple, créent des requêtes sans devoir exécuter les
   méthodes qui les appellent pour autant.

Tests Unitaires
---------------

Symfony et Doctrine partageant le même framework de test, il est facile d'implémenter
des tests unitaires dans vos projets. L'ORM est fourni avec son propre ensemble d'outils
facilitant les test unitaires vous permettant de vous abstraire de l'obligation de
créer une connexion, un gestionnaire d'entité, etc. En utilisant les composants
fournis par Doctrine - et une configuration simple - vous pouvez utiliser les outils
Doctrine pour réaliser des tests unitaires sur vos dépôts.

Gardez à l'esprit que si vous voulez tester l’exécution de requêtes sur vos bases de
données, vous devez réaliser des tests fonctionnels (voir la fin de ce cookbook ainsi
que :ref:`cookbook-doctrine-repo-functional-test`).
Tester de manière unitaire est seulement possible lorsque vous testez une méthode qui
construit une requête.

Configuration
~~~~~~~~~~~~~

Premièrement vous devez ajouter l'espace de noms ``Doctrine\Tests`` à votre autoloader::

    // app/autoload.php
    $loader->registerNamespaces(array(
        // ...
        'Doctrine\\Tests'                => __DIR__.'/../vendor/doctrine/orm/tests',
    ));

Ensuite, configurez un gestionnaire d'entité dans chaque test afin que Doctrine soit
capable de charger entités et dépôts.

Par défaut, Doctrine ne chargera pas les métadonnées d'annotations pour vos entités,
vous devez donc configurer le lecteur d'annotations afin de pouvoir analyser et charger
les entités::

    // src/Acme/ProductBundle/Tests/Entity/ProductRepositoryTest.php

    namespace Acme\ProductBundle\Tests\Entity;

    use Doctrine\Tests\OrmTestCase;
    use Doctrine\Common\Annotations\AnnotationReader;
    use Doctrine\ORM\Mapping\Driver\DriverChain;
    use Doctrine\ORM\Mapping\Driver\AnnotationDriver;

    class ProductRepositoryTest extends OrmTestCase
    {

         /**
         * @var \Doctrine\Tests\Mocks\EntityManagerMock
         */
        private $em;

        /**
         * {@inheritDoc}
         */
        protected function setUp()
        {
            $reader = new AnnotationReader();
            $reader->setIgnoreNotImportedAnnotations(true);
            $reader->setEnableParsePhpImports(true);


            // fournit l'espace de noms des entités que vous voulez tester
            $metadataDriver = new AnnotationDriver($reader, 'Acme\\StoreBundle\\Entity');

            $this->em = $this->_getTestEntityManager();

            $this->em->getConfiguration()->setMetadataDriverImpl($metadataDriver);

            // vous permet d'utiliser la syntaxe AcmeStoreBundle:Product
            $this->em->getConfiguration()->setEntityNamespaces(array(
                'AcmeStoreBundle' => 'Acme\\StoreBundle\\Entity'
            ));
        }
    }

En observant ces lignes, vous pouvez remarquer que:

* La classe hérite de ``\Doctrine\Tests\OrmTestCase`` qui fournit de nombreuses
  méthodes utiles aux tests unitaires.

* Vous avez inclus le gestionnaire d'annotation ``AnnotationReader`` afin
  qu'il réalise l'analyse syntaxique de vos entités.

* Vous avez créé un gestionnaire d'entité en appelant ``_getTestEntityManager``.
  Cette méthode vous fournit un gestionnaire virtuel n'ayant pas besoin de connexion.

Voilà! Vous êtes maintenant prêt à écrire les tests unitaires de vos dépôts Doctrine.

Ecrire vos tests unitaires
~~~~~~~~~~~~~~~~~~~~~~~~~

Rappelez-vous que les méthodes des dépôts Doctrine ne peuvent être testées qui si
les dépots ont été construits et que les tests retournent des requêtes sans les
exécuter. Prenez l’exemple suivant::

    // src/Acme/StoreBundle/Entity/ProductRepository.php

    namespace Acme\StoreBundle\Entity;

    use Doctrine\ORM\EntityRepository;

    class ProductRepository extends EntityRepository
    {

        /**
         * @param  string $name
         * @return \Doctrine\ORM\QueryBuilder
         */
        public function createSearchByNameQueryBuilder($name)
        {
            return $this
                ->createQueryBuilder('p')
                ->where('p.name LIKE :name')
                ->setParameter('name', $name)
            ;
        }
    }

Ici, la méthode retourne une instance de ``QueryBuilder``. Vous pouvez tester le
résultat par différents moyens::


    // src/Acme/StoreBundle/Tests/Entity/ProductRepositoryTest.php

    /* ... */

    class ProductRepositoryTest extends OrmTestCase
    {
        /* ... */

        public function testCreateSearchByNameQueryBuilder()
        {
            $queryBuilder = $this->em
                ->getRepository('AcmeStoreBundle:Product')
                ->createSearchByNameQueryBuilder('foo')
            ;

            $this->assertEquals('p.name LIKE :name', (string) $queryBuilder->getDqlPart('where'));
            $this->assertEquals(array('name' => 'foo'), $queryBuilder->getParameters());
        }
     }

Dans ce test, vous analysez l'objet ``QueryBuilder``, et vérifiez que chacune des parties
correspond au résultat attendu. Si vous ajoutez des paramètres au constructeur de requête,
vous pourrez vérifier les modifications sur chacuns des ensembles DQL suivants:
``select``, ``from``, ``join``, ``set``, ``groupBy``, ``having``, or ``orderBy``.

Si vous voulez vérifier l'exactitude de la syntaxe d'une requête complète ou la requête
actuelle, vous pouvez tester la chaîne de caractères générée en une requête DQL directement::

    public function testCreateSearchByNameQueryBuilder()
    {

        $queryBuilder = $this->em
            ->getRepository('AcmeStoreBundle:Product')
            ->createSearchByNameQueryBuilder('foo')
        ;

        $dql = $queryBuilder->getQuery()->getDql();

        // test DQL
        $this->assertEquals(
            'SELECT p FROM Acme\StoreBundle\Entity\Product p WHERE p.name LIKE :name',
            $dql
        );
    }

.. _cookbook-doctrine-repo-functional-test:

Tests fonctionnels
------------------

Si vous avez besoin de tester l’exécution d'une requête, vous devez démarrer le kernel
afin d'obtenir une connexion valide. Dans ce cas, votre classe doit hériter de ``WebTestCase``,
une classe qui simplifiera les processus de test::

    // src/Acme/StoreBundle/Tests/Entity/ProductRepositoryFunctionalTest.php

    namespace Acme\StoreBundle\Tests\Entity;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;

    class ProductRepositoryFunctionalTest extends WebTestCase
    {
        /**
         * @var \Doctrine\ORM\EntityManager
         */
        private $em;

        public function setUp()
        {
            $kernel = static::createKernel();
            $kernel->boot();
            $this->em = $kernel->getContainer()->get('doctrine.orm.entity_manager');
        }

        public function testProductByCategoryName()
        {
            $results = $this->em
                ->getRepository('AcmeStoreBundle:Product')
                ->searchProductsByNameQuery('foo')
                ->getResult()
            ;

            $this->assertCount(1, $results);
        }
    }
