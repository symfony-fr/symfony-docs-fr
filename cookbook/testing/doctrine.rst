.. index::
   single: Tests; Doctrine

Comment tester les dépôts doctrines
===================================

Les tests unitaires des dépôts Doctrines à l’intérieur d'un projet Symfony
ne sont pas recommandés. Lorsque vous travaillez avec un dépôt, vous
travaillez avec quelque chose qui est réellement sensé être testé avec une
véritable connexion à une base de données.

Heureusement, vous pouvez facilement tester vos requêtes avec une vraie
base de données, comme décrit ci-dessous

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
