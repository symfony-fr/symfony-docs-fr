@Route et @Method
=================

Utilisation
-----------

L'annotation @Route associe un schéma de route à un contrôleur::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

    class PostController extends Controller
    {
        /**
         * @Route("/")
         */
        public function indexAction()
        {
            // ...
        }
    }

L'action ``index`` du contrôleur ``Post`` est maintenant associée à l'URL ``/``.
C'est équivalent à la configuration YAML suivante :

.. code-block:: yaml

    blog_home:
        pattern:  /
        defaults: { _controller: SensioBlogBundle:Post:index }

Comme tout schéma de route, vous pouvez définir des valeurs par défaut, des valeurs
obligatoires et des valeurs de substitution (« placeholder » en anglais)::

    /**
     * @Route("/{id}", requirements={"id" = "\d+"}, defaults={"foo" = "bar"})
     */
    public function showAction($id)
    {
    }

Vous pouvez également faire correspondre la route à plusieurs URLs en définissant
plusieurs annotations ``@Route``::

    /**
     * @Route("/", defaults={"id" = 1})
     * @Route("/{id}")
     */
    public function showAction($id)
    {
    }

Activation
----------

Les routes doivent être importées pour être actives, comme toute ressource de routage
(notez le type ``annotation``) :

.. code-block:: yaml

    # app/config/routing.yml

    # importe les routes d'une classe Contrôleur
    post:
        resource: "@SensioBlogBundle/Controller/PostController.php"
        type:     annotation

Vous pouvez également importer tout un répertoire :

.. code-block:: yaml

    # importe les routes d'un répertoire Controller
    blog:
        resource: "@SensioBlogBundle/Controller"
        type:     annotation

Comme pour les autres ressources, vous pouvez préfixer les routes :

.. code-block:: yaml

    post:
        resource: "@SensioBlogBundle/Controller/PostController.php"
        prefix:   /blog
        type:     annotation

Nom de route
------------

Une route définie avec l'annotation ``@Route`` reçoit un nom prédéfini, composé du
nom du bundle, du nom du contrôleur et du nom de l'action. Dans l'exemple précédent,
le nom est ``sensio_blog_post_index``.

L'attribut ``name`` peut être utilisé pour surcharger le nom de route par défaut::

    /**
     * @Route("/", name="blog_home")
     */
    public function indexAction()
    {
        // ...
    }

Préfixe de route
----------------

Une annotation ``@Route`` d'une classe contrôleur définit un préfixe pour toutes les routes
des actions::

    /**
     * @Route("/blog")
     */
    class PostController extends Controller
    {
        /**
         * @Route("/{id}")
         */
        public function showAction($id)
        {
        }
    }

L'action ``show`` est associée au schéma ``/blog/{id}``.

Méthode de la route
-------------------

Il existe une annotation ``@Method`` pour spécifier les méthodes HTTP autorisées pour
la route. Pour l'utiliser, importez l'espace de nom ``Method``::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Method;

    /**
     * @Route("/blog")
     */
    class PostController extends Controller
    {
        /**
         * @Route("/edit/{id}")
         * @Method({"GET", "POST"})
         */
        public function editAction($id)
        {
        }
    }

L'action ``edit`` est associée au schema ``/blog/edit/{id}`` si la méthode utilisée
est GET ou POST.

L'annotation ``@Method`` n'est prise en compte que si l'action est annotée avec ``@Route``.

Contrôleur en tant que service
------------------------------

L'annotation ``@Route`` sur une classe contrôleur peut également être utilisée pour
assigner la classe contrôleur à un service, de telle sorte que le contrôleur sera
recherché dans le conteneur d'injection de dépendance et sera instancié au lieu
d'être créé par un appel à la méthode ``new PostController()`` elle-même::

    /**
     * @Route(service="my_post_controller_service")
     */
    class PostController extends Controller
    {
        // ...
    }
