Comment travailler avec plusieurs gestionnaires d'entité
========================================================

Vous pouvez utiliser plusieurs gestionnaires d'entité dans une application
Symfony2. Cela est nécessaire si vous utilisez différentes bases de données
ou même des « vendors » avec des ensembles d'entités entièrement différents.
En d'autres termes, un gestionnaire d'entité qui se connecte à une base de
données va gérer quelques entités alors qu'un autre gestionnaire d'entité
qui se connecte à une autre base de données pourrait gérer le reste.

.. note::

    Utiliser plusieurs gestionnaires d'entité est assez facile, mais plus
    avancé et généralement non requis. Soyez sûr que vous nécessitiez plusieurs
    gestionnaires d'entité avant d'ajouter cette couche de complexité.

Le code de configuration suivant montre comment vous pouvez configurer deux
gestionnaires d'entité :

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            orm:
                default_entity_manager:   default
                entity_managers:
                    default:
                        connection:       default
                        mappings:
                            AcmeDemoBundle: ~
                            AcmeStoreBundle: ~
                    customer:
                        connection:       customer
                        mappings:
                            AcmeCustomerBundle: ~

Dans ce cas, vous avez défini deux gestionnaires d'entité et les avez
appelé ``default`` et ``customer``. Le gestionnaire d'entité ``default``
gère les entités des bundles ``AcmeDemoBundle`` et ``AcmeStoreBundle``,
alors que le gestionnaire d'entité ``customer`` gère les entités du bundle
``AcmeCustomerBundle``.

Lorsque vous travaillez avec plusieurs gestionnaires d'entité, vous devriez
être explicite quant au gestionnaire d'entité que vous voulez. Si vous
*omettez* le nom du gestionnaire d'entité quand vous le demandez, le
gestionnaire d'entité par défaut (i.e. ``default``) est retourné::

    class UserController extends Controller
    {
        public function indexAction()
        {
            // les deux retournent le gestionnaire d'entité "default"
            $em = $this->get('doctrine')->getEntityManager();
            $em = $this->get('doctrine')->getEntityManager('default');
            
            $customerEm =  $this->get('doctrine')->getEntityManager('customer');
        }
    }

Vous pouvez maintenant utiliser Doctrine comme vous le faisiez avant - en
utilisant le gestionnaire d'entité ``default`` pour persister et aller chercher
les entités qu'il gère et le gestionnaire d'entité ``customer`` pour persister
et aller chercher ses entités.
