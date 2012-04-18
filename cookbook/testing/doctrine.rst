.. index::
   single: Tests; Doctrine

Comment tester les dépôts doctrines
===================================

Les tests unitaires des dépôts Doctrines à l’intérieur d'un projet Symfony
est un tache aisée qui demande cependant une préparation minutieuse. Ainsi
vous devez charger le dépot pour vos entités, un gestionnaire d'entité
(entity manager) ainsi que différents objets comme entre autre une connexion.

Pour tester votre dépôt, vous avez deux options:

1) **Test fonctionnel**: Cela inclus l'utilisation d'un base de donnée réelle.
   C'est simple à réaliser, vous permet de tester de tout tester mais est en
   contrepartie relativement lent à l’exécution.
   See :ref:`cookbook-doctrine-repo-functional-test`.

2) **Test unitaire**: Les test unitaires sont plus rapides et précis. Ils requièrent
   une configuration plus approfondie, que nous détaillerons dans ce document. Ils
   peuvent tester les méthodes créant les requêtes sans la possibilités d’exécuter
   celles-ci.

Test Unitaires
--------------

Symfony et Doctrine partageant le même framework de test, il est facile d'implémenter
des tests unitaires dans vos projets. L'ORM est fourni avec son propre jeu d'outils
facilitant les test unitaires vous permettant de vous abstraire de l'obligation de 
créer une connexion, un gestionnaire d'entité, etc. En utilisant les composants 
fournis par Doctrine - et une configuration simple - vous pouvez utiliser les outils
Doctrine's pour réaliser des tests unitaires sur vos dépôts.

Garder à l'esprit que si vous voulez tester l’exécution de requêtes sur vos bases de
données, vous devez réaliser des tests fonctionnels (voir la fin de ce cookbook ainsi
que :ref:`cookbook-doctrine-repo-functional-test`).

Configuration
~~~~~~~~~~~~~

Premièrement vous devez ajouter l'espace de nom ``Doctrine\Tests`` à votre autoloader::

    // app/autoload.php
    $loader->registerNamespaces(array(
        // ...
        'Doctrine\\Tests'                => __DIR__.'/../vendor/doctrine/orm/tests',
    ));

Ensuite, configurez un gestionnaire d'entité dans chaque test afin que Doctrine soit 
capable de charger entités et dépôts.

Par défaut, Doctrine ne chargera pas les méta données d'annotation pour vos entités,
vous devez donc configurer le lecteur d'annotations  si vous les utiliser::

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


            // provide the namespace of the entities you want to tests
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

* La classe hérite de ``\Doctrine\Tests\OrmTestCase`` qui fourni de nombreuses 
  méthodes utiles au test unitaires.

* Vous avez inclus le gestionnaire d'annotation ``AnnotationReader`` afin
  qu'il réalise l'analyse syntaxique de vos entités.

* Vous avez créer un gestionnaire d'entité en appelant ``_getTestEntityManager``.
  Cette méthode vous fourni un gestionnaire virtuel n'ayant pas besoin de connection.

Voilà! Vous êtes maintenant prêt à écrire les tests unitaires de vos dépôts Doctrine.

Ecrire vos test unitaires
~~~~~~~~~~~~~~~~~~~~~~~~~

Rappelez vous que les méthodes des dépôts Doctrine ne peuvent être testées qui si 
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

Dans ce test vous analyser l'objet ``QueryBuilder``, et vérifier chacune des parties
dont le résultats . Si vous ajoutez des paramètres au constructeur de requête (query
builder), vous pourrez vérifier les modifications sur chacuns des ensembles DQL suivant:
``select``, ``from``, ``join``, ``set``, ``groupBy``, ``having``, or ``orderBy``.

Si vous voulez vérifier l'exactitude de la syntaxe d'une requête complète ou la requête
actuelle vous pouvez tester la chaîne de caractère générée en une requête DQL directement::

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
