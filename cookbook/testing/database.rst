.. index::
   single: Tests; Database

Comment tester du code interagisant avec une base de données
============================================================

Si votre code interagit avec une base de données, par exemple si elle lit ou
enregistre des données, vous devez ajuster vos tests pour prendre en compte
ceci en compte. Il y a de nombreuses façons de gérer ce cas. Dans un test unitaire,
vous pouvez créer un bouchon (``mock`` en anglais) pour un ``Repository`` et l'utiliser
pour retourner les objets attendus. Dans un test fonctionnel, vous devriez avoir
besoin de préparer une base de données de test avec des valeurs prédéfinies pour
assurer que votre test tfonctionne toujours avec les mêmes données.

.. note::

    Si vous voulez tester directement vos requêtes,
    consultez :doc:`/cookbook/testing/doctrine`.

Mocker le ``Repository`` dans un test unitaire
----------------------------------------------

Si vous souhaitez tester du code dépendant d'un repository Doctrine tout
en l'isolant, vous aurez besoin de mocker ce ``Repository``. Normalement,
vous injectez l' ``EntityManager`` dans votre classe et l'utilisez pour
récupérer le repository. Cela rend les choses un peu plus difficile puisque
vous avez besoin de mocker et l' ``EntityManager`` et votre classe de repository.

.. tip::

    Il est possible (et c'est une bonne idée) d'injecter votre repository directement
    en l'enregistrant comme un :doc:`service factory </components/dependency_injection/factories>`.
    Cela demande un peu plus de travail pour le paramétrage, mais cela rend les tests plus faciles
    puisque vous n'aurez besoin de mocker que le repository.

Supposons que la classe que vous voulez tester resemble à ceci::

    namespace Acme\DemoBundle\Salary;

    use Doctrine\Common\Persistence\ObjectManager;

    class SalaryCalculator
    {
        private $entityManager;

        public function __construct(ObjectManager $entityManager)
        {
            $this->entityManager = $entityManager;
        }

        public function calculateTotalSalary($id)
        {
            $employeeRepository = $this->entityManager->getRepository('AcmeDemoBundle::Employee');
            $employee = $employeeRepository->find($id);

            return $employee->getSalary() + $employee->getBonus();
        }
    }

Puisque l' ``ObjectManager`` est injecté dans la classe à travers le constructeur,
il est facile de passer un objet mock dans un test ::

    use Acme\DemoBundle\Salary\SalaryCalculator;

    class SalaryCalculatorTest extends \PHPUnit_Framework_TestCase
    {
        public function testCalculateTotalSalary()
        {
            // Premièrement, mockez l'objet qui va être utilisé dans le test
            $employee = $this->getMock('\Acme\DemoBundle\Entity\Employee');
            $employee->expects($this->once())
                ->method('getSalary')
                ->will($this->returnValue(1000));
            $employee->expects($this->once())
                ->method('getBonus')
                ->will($this->returnValue(1100));

            // Maintenant, mocker le repository pour qu'il retourne un mock de l'objet emloyee
            $employeeRepository = $this->getMockBuilder('\Doctrine\ORM\EntityRepository')
                ->disableOriginalConstructor()
                ->getMock();
            $employeeRepository->expects($this->once())
                ->method('find')
                ->will($this->returnValue($employee));

            // Et enfin, mockez l'EntityManager pour qu'il retourne un mock du repository
            $entityManager = $this->getMockBuilder('\Doctrine\Common\Persistence\ObjectManager')
                ->disableOriginalConstructor()
                ->getMock();
            $entityManager->expects($this->once())
                ->method('getRepository')
                ->will($this->returnValue($employeeRepository));

            $salaryCalculator = new SalaryCalculator($entityManager);
            $this->assertEquals(2100, $salaryCalculator->calculateTotalSalary(1));
        }
    }

Dans cet exemple, vous construisez les mocks de l'intérieur vers l'extérieur, en
créant premièrement l'employee qui est retourné par le ``Repository``, qui lui-même
est retourné par l' ``entityManager``. De cette façon, aucune vraie classe n'est
impliquée dans le test.

Changer le paramétrage de la base de données pour les tests fonctionnels
------------------------------------------------------------------------

Si vous avez des tests fonctionnels, vous souhaitez qu'ils interagissent avec une
vraie base de données. La plupart du temps, vous souhaitez une base de données dédiée
durant le développement de l'application ainsi qu'être capable de nettoyer la base de
données avant chaque test.

Pour faire cela, il vous est possible de spécifier la configuration de la base de
données, qui remplacera la configuration par défaut:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        doctrine:
            # ...
            dbal:
                host: localhost
                dbname: testdb
                user: testdb
                password: testdb

    .. code-block:: xml

        <!-- app/config/config_test.xml -->
        <doctrine:config>
            <doctrine:dbal
                host="localhost"
                dbname="testdb"
                user="testdb"
                password="testdb"
            />
        </doctrine:config>

    .. code-block:: php

        // app/config/config_test.php
        $configuration->loadFromExtension('doctrine', array(
            'dbal' => array(
                'host'     => 'localhost',
                'dbname'   => 'testdb',
                'user'     => 'testdb',
                'password' => 'testdb',
            ),
        ));

Assurez vous que votre base de données tourne sur localhost, qu'elle a une base
de données définie et que les droits utilisateurs sont configurés.
