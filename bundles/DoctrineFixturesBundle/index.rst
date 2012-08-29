DoctrineFixturesBundle
======================

Les fixtures sont utilisées pour charger un ensemble de données dans une base
de données. Ces données peuvent être utilisées pour les tests ou peuvent être
les données initiales nécessaires au bon fonctionnement de l'application.
Symfony2 n'a pas été conçu pour gérer les fixtures, mais Doctrine2 possède une
librairie qui peut vous aider à écrire des fictures pour l':doc:`ORM</book/doctrine>`
ou l':doc:`ODM</bundles/DoctrineMongoDBBundle/index>` Doctrine.

Installation et configuration
-----------------------------

Les fixtures Doctrine pour Symfony sont maintenues dans le `DoctrineFixturesBundle`_.
Le bundle utilise la librairie externe `Doctrine Data Fixtures`_.

Suivez ces étapes pour installer le bundle et la librairie dans l'Édition
Standard de Symfony. Ajoutez le code suivant à votre fichier ``composer.json`` :

.. code-block:: json

    {
        "require": {
            "doctrine/doctrine-fixtures-bundle": "dev-master"
        }
    }

Mettez à jour les librairies vendor :

.. code-block:: bash

    $ php composer.phar update

Si tout s'est bien passé, le ``DoctrineFixturesBundle`` peut maintenant
être trouvé dans le répertoire ``vendor/doctrine/doctrine-fixtures-bundle``.

.. note::

    ``DoctrineFixturesBundle`` installe la librairie `Doctrine Data Fixtures`_.
	La librairie peut être trouvée dans le répertoire ``vendor/doctrine/data-fixtures``.

Finalement, enregistrez le bundle ``DoctrineFixturesBundle`` dans le fichier ``app/AppKernel.php``.

.. code-block:: php

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Doctrine\Bundle\FixturesBundle\DoctrineFixturesBundle(),
            // ...
        );
        // ...
    }

Écrire des fixtures simples
---------------------------

Les fixtures Doctrine2 sont des classes PHP dans lesquelles vous pouvez
créer des objets et les persister en base de données. Comme toutes les classes
dans Symfony2, les fixtures doivent se trouver dans l'un des bundles de votre
application.

Pour un bundle qui se situe dans ``src/Acme/HelloBundle``, les classes fixture
devraient se trouver dans les répertoires ``src/Acme/HelloBundle/DataFixtures/ORM``
ou ``src/Acme/HelloBundle/DataFixtures/MongoDB`` respectivement pour l'ORM et l'ODM.
Ce tutoriel suppose que vous utilisez l'ORM, mais les fixtures peuvent être ajoutées
tout aussi facilement si vous utilisez l'ODM.

Imaginez que vous havez une classe ``User``, et que vous aimeriez charger
une entrée ``User`` :

.. code-block:: php

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadUserData.php

    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use Acme\HelloBundle\Entity\User;

    class LoadUserData implements FixtureInterface
    {
        /**
         * {@inheritDoc}
         */
        public function load(ObjectManager $manager)
        {
            $userAdmin = new User();
            $userAdmin->setUsername('admin');
            $userAdmin->setPassword('test');

            $manager->persist($userAdmin);
            $manager->flush();
        }
    }

Dans Doctrine2, les fixtures sont juste des objets où vous chargez les données
en interagissant avec vos entités comme vous le faites habituellement. Cela vous
permet de créer précisément les fixtures dont vous avez besoin pour votre application.

La plus grande limitation est que vous ne pouvez pas partager d'objets entre les
fixtures. Plus tard, vous verrez comment contourner cette limitation.

Exécuter des fixtures
---------------------

Une fois que vos fixtures ont été écrites, vous pouvez les charger via
la ligne de commande en utilisant la commande ``doctrine:fixtures:load`` :

.. code-block:: bash

    php app/console doctrine:fixtures:load

Si vous utilisez l'ODM, utilisez plutôt la commande ``doctrine:mongodb:fixtures:load`` :

.. code-block:: bash

    php app/console doctrine:mongodb:fixtures:load

La tâche regardera dans le répertoire ``DataFixtures/ORM`` (ou ``DataFixtures/MongoDB``
pour l'ODM) de chaque bundle et exécuter chaque classe qui implémente la
``FixtureInterface``.

Les deux commandes sont fournies avec quelques options :

* ``--fixtures=/path/to/fixture`` - Utilisez cette option pour spécifier manuellement
  le répertoire où les fixtures doivent être chargées;

* ``--append`` - Utilisez ce flag pour ajouter les données au lieu de supprimer les
  données avant de les charger (les supprimer en premier est le comportement par défaut);

* ``--em=manager_name`` - Spécifie manuellement le gestionnaire d'entité à utiliser pour
  charger les données.

.. note::

   Si vous utilisez la tâche ``doctrine:mongodb:fixtures:load``, remplacez l'option
   ``--em=`` par ``--dm=`` pour spécifier manuellement le gestionnaire de document.

Un exemple d'utilisation complet pourrait ressembler à :

.. code-block:: bash

   php app/console doctrine:fixtures:load --fixtures=/path/to/fixture1 --fixtures=/path/to/fixture2 --append --em=foo_manager

Partager des objets entre les fixtures
--------------------------------------

Écrire une fixture basique est très simple. Mais que se passerait-il si vous
avez plusieurs classes de fixtures et que vous voulez être capable de faire
référence à des données chargées dans d'autres classes de fixture ?
Par exemple, comment feriez vous si vous chargez un objet ``User`` dans une fixture,
et qu'ensuite vous voulez y faire référence dans une fixture différente pour assigner
l'utilisateur à un groupe particulier?

La librairie de fixtures Doctrine gère cela très facilement en vous permettant
de spécifier l'ordre dans lequel les fixtures sont chargées.

.. code-block:: php

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadUserData.php
    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\DataFixtures\OrderedFixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use Acme\HelloBundle\Entity\User;

    class LoadUserData extends AbstractFixture implements OrderedFixtureInterface
    {
        /**
         * {@inheritDoc}
         */
        public function load(ObjectManager $manager)
        {
            $userAdmin = new User();
            $userAdmin->setUsername('admin');
            $userAdmin->setPassword('test');

            $manager->persist($userAdmin);
            $manager->flush();

            $this->addReference('admin-user', $userAdmin);
        }

        /**
         * {@inheritDoc}
         */
        public function getOrder()
        {
            return 1; // l'ordre dans lequel les fichiers sont chargées
        }
    }

La classe de fixture implémente maintenant l'interface ``OrderedFixtureInterface``,
qui spécifie à Doctrine que vous voulez choisir l'ordre de vos fixtures. Créez
une autre classe de fixture et faites la charter après ``LoadUserData`` en
retournant un ordre 2 :

.. code-block:: php

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadGroupData.php

    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\DataFixtures\OrderedFixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use Acme\HelloBundle\Entity\Group;

    class LoadGroupData extends AbstractFixture implements OrderedFixtureInterface
    {
        /**
         * {@inheritDoc}
         */
        public function load(ObjectManager $manager)
        {
            $groupAdmin = new Group();
            $groupAdmin->setGroupName('admin');

            $manager->persist($groupAdmin);
            $manager->flush();

            $this->addReference('admin-group', $groupAdmin);
        }

        /**
         * {@inheritDoc}
         */
        public function getOrder()
        {
            return 2; // l'ordre dans lequel les fichiers sont chargées
        }
    }

Les deux classes de fixtures étendent ``AbstractFixture``, qui vous permet
de créer des objets et de les définir comme référence pour qu'ils puissent
être réutilisés ensuite dans d'autres fixtures. Par exemple, les objets
``$userAdmin`` et ``$groupAdmin`` peuvent être référencés plus tard via les
références ``admin-user`` et ``admin-group`` :

.. code-block:: php

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadUserGroupData.php

    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\DataFixtures\OrderedFixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use Acme\HelloBundle\Entity\UserGroup;

    class LoadUserGroupData extends AbstractFixture implements OrderedFixtureInterface
    {
        /**
         * {@inheritDoc}
         */
        public function load(ObjectManager $manager)
        {
            $userGroupAdmin = new UserGroup();
            $userGroupAdmin->setUser($manager->merge($this->getReference('admin-user')));
            $userGroupAdmin->setGroup($manager->merge($this->getReference('admin-group')));

            $manager->persist($userGroupAdmin);
            $manager->flush();
        }

        /**
         * {@inheritDoc}
         */
        public function getOrder()
        {
            return 3;
        }
    }

Les fixtures seront maintenant exécutées dans l'ordre ascendant des valeurs
retournées par ``getOrder()``. Tout objet qui est défini avec la méthode
``setReference()`` est accessible via ``getReference()`` dans les classes de
fixtures qui ont un ordre plus grand.

Les fixtures vous permettent de créer tout type de données dont vous avez
besoin via l'interface PHP normale pour créer et persister des objets. En
contrôlant l'ordre des fixtures et la définition des références, presque
tout peut être géré par les fixtures.

Utiliser le Conteneur dans les fixtures
---------------------------------------

Dans certains cas, vous pourriez avoir besoin d'accéder à des services pour
charger les fixtures. Symfony2 rend cela très facile : le conteneur sera
injecté dans toutes les classes de fixtures qui implémentent l'interface
:class:`Symfony\\Component\\DependencyInjection\\ContainerAwareInterface`.

Réécrivons la première fixture pour encoder le mot de passe avant de le
stocker dans la base de données (une très bonne pratique). Cela utilisera
l'encodeur (encoder_factory) pour encoder le mot de passe et s'assurer qu'il
est bien encodé de la manière utilisée par le composant de sécurité :

.. code-block:: php

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadUserData.php

    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Symfony\Component\DependencyInjection\ContainerAwareInterface;
    use Symfony\Component\DependencyInjection\ContainerInterface;
    use Acme\HelloBundle\Entity\User;

    class LoadUserData implements FixtureInterface, ContainerAwareInterface
    {
        /**
         * @var ContainerInterface
         */
        private $container;

        /**
         * {@inheritDoc}
         */
        public function setContainer(ContainerInterface $container = null)
        {
            $this->container = $container;
        }

        /**
         * {@inheritDoc}
         */
        public function load(ObjectManager $manager)
        {
            $user = new User();
            $user->setUsername('admin');
            $user->setSalt(md5(uniqid()));

            $encoder = $this->container
                ->get('security.encoder_factory')
                ->getEncoder($user)
            ;
            $user->setPassword($encoder->encodePassword('secret', $user->getSalt()));

            $manager->persist($user);
            $manager->flush();
        }
    }

Comme vous pouvez le voir, tout ce dont vous avez besoin est d'ajouter
:class:`Symfony\\Component\\DependencyInjection\\ContainerAwareInterface` à la classe puis
créer une nouvelle méthode :method:`Symfony\\Component\\DependencyInjection\\ContainerInterface::setContainer`
qui implémente l'interface. Avant que la fixture ne soit exécutée, Symfony appellera
la méthode :method:`Symfony\\Component\\DependencyInjection\\ContainerInterface::setContainer`
automatiquement. Tant que vous stockez le conteneur dans une propriété de la
classe (comme expliqué ci-dessus), vous pouvez y accéder via la méthode ``load()``.

.. note::

	Si vous êtes trop fénéant pour implémenter la méthode obligatoire
	:method:`Symfony\\Component\\DependencyInjection\\ContainerInterface::setContainer`, vous pouvez
	étendre votre classe avec :class:`Symfony\\Component\\DependencyInjection\\ContainerAware`.

.. _DoctrineFixturesBundle: https://github.com/doctrine/DoctrineFixturesBundle
.. _`Doctrine Data Fixtures`: https://github.com/doctrine/data-fixtures
