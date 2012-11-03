.. index::
   pair: Doctrine; DBAL

Comment utiliser la couche DBAL de Doctrine
===========================================

.. note::

    Cet article traite de la couche DBAL de Doctrine. Typiquement, vous
    allez travailler avec le haut niveau de la couche ORM de Doctrine,
    qui utilise le DBAL en arrière-plan pour effectivement communiquer
    avec la base de données. Pour en lire plus à propos de l'ORM Doctrine,
    voir « :doc:`/book/doctrine` ».

La couche d'abstraction de la base de données (DBAL pour DataBase Abstraction Layer)
de `Doctrine`_ se situe au plus haut niveau de `PDO`_ et offre une API intuitive et flexible
pour communiquer avec les bases de données relationnelles les plus populaires.
En d'autres termes, la bibliothèque DBAL rend facile l'exécution de requêtes et autres
actions sur la base de données.

.. tip::

    Lisez la `Documentation DBAL`_ officielle de Doctrine pour apprendre tous
    les détails et capacités de la bibliothèque DBAL de Doctrine.

Pour démarrer, configurez les paramètres de connexion de la base de données :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                driver:   pdo_mysql
                dbname:   Symfony2
                user:     root
                password: null
                charset:  UTF8

    .. code-block:: xml

        // app/config/config.xml
        <doctrine:config>
            <doctrine:dbal
                name="default"
                dbname="Symfony2"
                user="root"
                password="null"
                driver="pdo_mysql"
            />
        </doctrine:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'driver'    => 'pdo_mysql',
                'dbname'    => 'Symfony2',
                'user'      => 'root',
                'password'  => null,
            ),
        ));

Pour une liste complète des options de configuration de DBAL, voir
:ref:`reference-dbal-configuration`.

Vous pouvez alors accéder à la connexion DBAL de Doctrine en utilisant
le service ``database_connection`` :

.. code-block:: php

    class UserController extends Controller
    {
        public function indexAction()
        {
            $conn = $this->get('database_connection');
            $users = $conn->fetchAll('SELECT * FROM users');

            // ...
        }
    }

Déclarer des Types de Correspondance Personnalisés
--------------------------------------------------

Vous pouvez déclarer des types de correspondance personnalisés via la configuration
de Symfony. Ils seront ajoutés à toutes les connexions configurées. Pour plus
d'informations sur les types de correspondances personnalisés, lisez la section
`types de correspondances personnalisés`_ de la documentation de Doctrine.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                types:
                    custom_first: Acme\HelloBundle\Type\CustomFirst
                    custom_second: Acme\HelloBundle\Type\CustomSecond

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal>
                    <doctrine:type name="custom_first" class="Acme\HelloBundle\Type\CustomFirst" />
                    <doctrine:type name="custom_second" class="Acme\HelloBundle\Type\CustomSecond" />
                </doctrine:dbal>
            </doctrine:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'types' => array(
                    'custom_first'  => 'Acme\HelloBundle\Type\CustomFirst',
                    'custom_second' => 'Acme\HelloBundle\Type\CustomSecond',
                 ),
            ),
        ));

Déclarer des Types de Correspondance Personnalisés via le SchemaTool
--------------------------------------------------------------------

Le SchemaTool est utilisé pour inspecter la base de données afin d'en comparer
le schéma. Pour effectuer cette tâche, il a besoin de connaître quel type de
correspondance utiliser pour chaque type de base de données. En déclarer de
nouveaux peut être effectué grâce à la configuration.

Faisons correspondre le type ENUM (non-supporté par DBAL par défaut) à un type
``string`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                connections:
                    default:
                        // Autres paramètres de connexion
                        mapping_types:
                            enum: string

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection>
                        <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                    </doctrine:connection>
                </doctrine:dbal>
            </doctrine:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'connections' => array(
                    'default' => array(
                        'mapping_types' => array(
                            'enum'  => 'string',
                        ),
                    ),
                ),
            ),
        ));

.. _`PDO`:           http://www.php.net/pdo
.. _`Doctrine`:      http://www.doctrine-project.org
.. _`Documentation DBAL`: http://docs.doctrine-project.org/projects/doctrine-dbal/en/latest/index.html
.. _`types de correspondances personnalisés`: http://docs.doctrine-project.org/projects/doctrine-dbal/en/latest/reference/types.html#custom-mapping-types