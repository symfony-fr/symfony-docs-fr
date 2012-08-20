.. index::
   single: Doctrine; Multiple entity managers

Comment travailler avec plusieurs gestionnaires d'entités
=========================================================

Vous pouvez utiliser plusieurs gestionnaires d'entités dans une application
Symfony2. Cela est nécessaire si vous utilisez différentes bases de données
ou même des « vendors » avec des ensembles d'entités entièrement différents.
En d'autres termes, un gestionnaire d'entités qui se connecte à une base de
données va gérer quelques entités alors qu'un autre gestionnaire d'entités
qui se connecte à une autre base de données pourrait gérer le reste.

.. note::

    Utiliser plusieurs gestionnaires d'entités est assez facile, mais plus
    avancé et généralement non requis. Soyez sûr que vous nécessitiez plusieurs
    gestionnaires d'entités avant d'ajouter cette couche de complexité.

Le code de configuration suivant montre comment vous pouvez configurer deux
gestionnaires d'entités :

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

Dans ce cas, vous avez défini deux gestionnaires d'entités et les avez
appelé ``default`` et ``customer``. Le gestionnaire d'entités ``default``
gère les entités des bundles ``AcmeDemoBundle`` et ``AcmeStoreBundle``,
alors que le gestionnaire d'entités ``customer`` gère les entités du bundle
``AcmeCustomerBundle``.

Lorsque vous travaillez avec plusieurs gestionnaires d'entités, vous devriez
être explicite quant au gestionnaire d'entités que vous voulez. Si vous
*omettez* le nom du gestionnaire d'entité quand vous mettez à jour votre schema,
le gestionnaire par défaut (c-a-d ``default``) est utilisé::

    # Utilise le gestionnaire « default »
    php app/console doctrine:schema:update --force

    # tilise le gestionnaire « customer »
    php app/console doctrine:schema:update --force --em=customer

Si vous *omettez* le nom du gestionnaire d'entité quand vous le demandez, le
gestionnaire d'entités par défaut (c'est-à-dire ``default``) est retourné::

    class UserController extends Controller
    {
        public function indexAction()
        {
            // les deux retournent le gestionnaire d'entités "default"
            $em = $this->get('doctrine')->getManager();
            $em = $this->get('doctrine')->getManager('default');
            
            $customerEm =  $this->get('doctrine')->getManager('customer');
        }
    }

Vous pouvez maintenant utiliser Doctrine comme vous le faisiez avant - en
utilisant le gestionnaire d'entités ``default`` pour persister et aller chercher
les entités qu'il gère et le gestionnaire d'entités ``customer`` pour persister
et aller chercher ses entités.

La même chose s'applique aux appels de repository::

    class UserController extends Controller
    {
        public function indexAction()
        {
            // Retourne un repository géré par le gestionnaire « default »
            $products = $this->get('doctrine')
                             ->getRepository('AcmeStoreBundle:Product')
                             ->findAll();

            // MAnière explicite de traiter avec le gestionnaire « default »
            $products = $this->get('doctrine')
                             ->getRepository('AcmeStoreBundle:Product', 'default')
                             ->findAll();

            // Retourne un repository géré par le gestionnaire « customer »
            $customers = $this->get('doctrine')
                              ->getRepository('AcmeCustomerBundle:Customer', 'customer')
                              ->findAll();
        }
    }