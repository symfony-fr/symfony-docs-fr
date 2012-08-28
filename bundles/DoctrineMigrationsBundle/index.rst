DoctrineMigrationsBundle
========================

La fonctionnalité permettant des migrations de base de données est une extension
de la couche d'abstraction et vous permet de déployer programmatiquement de nouvelles
versions de votre schéma de base de données de manière simple, sécurisée et standardisée.

.. tip::

    Vous pouvez en lire plus sur les migrations de base de données Doctrine sur
    la `documentation`_ du projet.

Installation
------------

Les migrations Doctrine pour Symfony sont maintenues dans le `DoctrineMigrationsBundle`_.
Le bundle utilise la librairie externe `Doctrine Database Migrations`_.

Suivez ces étapes pour installer le bundle et la librairie dans l'Edition
Standard de Symfony. Ajoutez le code suivant à votre fichier ``composer.json`` :

.. code-block:: json

    {
        "require": {
            "doctrine/doctrine-migrations-bundle": "dev-master"
        }
    }

Mettez à jour vos librairies « vendor » :

.. code-block:: bash

    $ php composer.phar update

Si tout s'est bien passé, le ``DoctrineMigrationsBundle`` peut maintenant
être trouvé dans le répertoire ``vendor/doctrine/doctrine-migrations-bundle``.

.. note::

    ``DoctrineMigrationsBundle`` installe la librairie `Doctrine Database Migrations`_.
    La librairie peut être trouvée dans le répertoire ``vendor/doctrine/migrations``.

Enfin, assurez-vous d'avoir activé le bundle dans le fichier ``AppKernel.php`` en
ajoutant le code suivant :

.. code-block:: php

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            //...
            new Doctrine\Bundle\MigrationsBundle\DoctrineMigrationsBundle(),
        );
    }

Utilisation
-----------

Toutes les fonctionnalités liées aux migrations sont contenues dans quelques
commandes :

.. code-block:: bash

    doctrine:migrations
      :diff     Génère une migration en comparant votre base actuelle et vos informations de « mapping ».
      :execute  Exécute une seule migration manuelle de version (vers une version supérieure ou inférieure).
      :generate Génère une classe de migration vide.
      :migrate  Exécute une migration jusqu'à la version spécifiée, ou jusqu'à la dernière version.
      :status   Affiche le statut d'un ensemble de migrations.
      :version  Ajoute ou supprime manuellement des versions du tableau de versions.

Commencez par récupérer le statut des migrations de votre application en exécutant
la commande ``status`` :

.. code-block:: bash

    php app/console doctrine:migrations:status

     == Configuration

        >> Name:                                               Application Migrations
        >> Configuration Source:                               manually configured
        >> Version Table Name:                                 migration_versions
        >> Migrations Namespace:                               Application\Migrations
        >> Migrations Directory:                               /path/to/project/app/DoctrineMigrations
        >> Current Version:                                    0
        >> Latest Version:                                     0
        >> Executed Migrations:                                0
        >> Available Migrations:                               0
        >> New Migrations:                                     0

Maintenant, vous pouvez commencer à travailler avec les migrations en générant
une nouvelle classe de migration vide. Plus tard, vous apprendrez comment Doctrine
peut générer des migrations automatiquement à votre place.

.. code-block:: bash

    php app/console doctrine:migrations:generate
    Nouvelle classe de migration générée dans "/path/to/project/app/DoctrineMigrations/Version20100621140655.php"

Jetez un oeil à la classe de migration nouvellement créée et vous verrez quelque
chose qui ressemble à ceci::

    namespace Application\Migrations;

    use Doctrine\DBAL\Migrations\AbstractMigration,
        Doctrine\DBAL\Schema\Schema;

    class Version20100621140655 extends AbstractMigration
    {
        public function up(Schema $schema)
        {

        }

        public function down(Schema $schema)
        {

        }
    }

Si vous exécutez la commande ``status``, elle vous indiquera que vous avez une
nouvelle migration à exécuter :

.. code-block:: bash

    php app/console doctrine:migrations:status

     == Configuration

       >> Name:                                               Application Migrations
       >> Configuration Source:                               manually configured
       >> Version Table Name:                                 migration_versions
       >> Migrations Namespace:                               Application\Migrations
       >> Migrations Directory:                               /path/to/project/app/DoctrineMigrations
       >> Current Version:                                    0
       >> Latest Version:                                     2010-06-21 14:06:55 (20100621140655)
       >> Executed Migrations:                                0
       >> Available Migrations:                               1
       >> New Migrations:                                     1

    == Migration Versions

       >> 2010-06-21 14:06:55 (20100621140655)                not migrated

Vous pouvez maintenant ajouter du code de migration dans les méthodes ``up()`` et ``down()``
et migrez ensuite lorsque vous serez prêt :

.. code-block:: bash

    php app/console doctrine:migrations:migrate

Pour plus d'informations sur comment écrire les migrations elles-mêmes
(c'est à dire comment remplir les méthodes ``up()`` et ``down()``), lisez la
`documentation`_ officielle sur les Migrations Doctrine.

Exécuter les migrations pendant le déploiement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Bien sûr, le but final des migrations est d'être capable de les utiliser
afin de mettre à jour de façon fiable la structure de votre base de données lorsque
vous déployez votre application. En exécutant les migrations localement (ou
sur un serveur de test), vous pouvez vous assurer qu'elles fonctionnent comme
vous le voulez.

Lorsque vous déployez enfin votre application, vous devez juste vous rappeler
d'exécuter la commande ``doctrine:migrations:migrate``. En interne, Doctrine
crée une table ``migration_versions`` dans votre base de données et surveille
quelles migrations ont été exécutées. En conséquence, peu importe combien de
migrations vous avez créé et exécuté localement, lorsque vous exécutez la commande
durant le déploiement, Doctrine sait exactement quelles migrations n'ont pas encore
été exécutées en regardant dans la table ``migration_versions`` de votre base de
données de production. Indépendamment du serveur sur lequel vous vous trouvez,
vous pouvez toujours exécuter cette commande en toute sécurité pour exécuter
les migrations qui n'ont pas encore été exécutées sur *cette* base de données
en particulier.

Générer les migrations automatiquement
--------------------------------------

En réalité, vous devrez rarement avoir besoin d'écrire les migrations manuellement,
puisque la librairie peut générer les classes de migration automatiquement en
comparant vos informations de « mapping » Doctrine (c'est à dire ce à quoi votre
base de données *devrait* ressembler) avec la structure de votre base de données
actuelle.

Par exemple, supposons que vous créiez une nouvelle entité ``User`` et que vous
ajoutiez les informations de « mapping » pour l'ORM Doctrine :

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/HelloBundle/Entity/User.php
        namespace Acme\HelloBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity
         * @ORM\Table(name="hello_user")
         */
        class User
        {
            /**
             * @ORM\Id
             * @ORM\Column(type="integer")
             * @ORM\GeneratedValue(strategy="AUTO")
             */
            protected $id;

            /**
             * @ORM\Column(type="string", length="255")
             */
            protected $name;
        }

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/doctrine/User.orm.yml
        Acme\HelloBundle\Entity\User:
            type: entity
            table: hello_user
            id:
                id:
                    type: integer
                    generator:
                        strategy: AUTO
            fields:
                name:
                    type: string
                    length: 255

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/doctrine/User.orm.xml -->
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                            http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="Acme\HelloBundle\Entity\User" table="hello_user">
                <id name="id" type="integer" column="id">
                    <generator strategy="AUTO"/>
                </id>
                <field name="name" column="name" type="string" length="255" />
            </entity>

        </doctrine-mapping>

Avec ces informations, Doctrine est maintenant prêt à vous aider à persister
votre nouvel objet ``User`` vers et depuis la table ``hello_user``. Bien sûr,
cette table n'existe pas encore ! Générez automatiquement une nouvelle migration
pour cette table en exécutant la commande suivante :

.. code-block:: bash

    php app/console doctrine:migrations:diff

Vous devriez voir un message indiquant qu'une nouvelle classe de migration
a été générée en se basant sur les différences du schéma. Si vous ouvrez ce
fichier, vous y trouverez le code SQL nécessaire à la création de la table
``hello_user``. Ensuite, exécutez la migration pour ajouter la table à votre
base de données :

.. code-block:: bash

    php app/console doctrine:migrations:migrate

La morale de l'histoire est la suivante : après chaque changement que vous
faites dans votre « mapping » Doctrine, exécutez la commande ``doctrine:migrations:diff``
pour générer automatiquement vos classes de migration.

Si vous faites cela dès le début de votre projet (c'est à dire dès que les premières
tables ont été chargées via une classe de migration), vous serez toujours capable
de créer une base de données fraîche et d'exécuter les migrations dans l'ordre
afin d'avoir votre schéma de base de données complètement à jour. En fait, c'est
une manière de travailler simple et fiable pour votre projet.

.. _documentation: http://docs.doctrine-project.org/projects/doctrine-migrations/en/latest/index.html
.. _DoctrineMigrationsBundle: https://github.com/doctrine/DoctrineMigrationsBundle
.. _`Doctrine Database Migrations`: https://github.com/doctrine/migrations