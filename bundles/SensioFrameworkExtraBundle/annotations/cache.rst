@Cache
======

Utilisation
-----------

L'annotation ``@Cache`` facile la définition du cache HTTP::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Cache;

    /**
     * @Cache(expires="tomorrow")
     */
    public function indexAction()
    {
    }

Vous pouvez également définir l'annotation sur une classe pour définir le
cache pour toutes ses méthodes::

    /**
     * @Cache(expires="tomorrow")
     */
    class BlogController extends Controller
    {
    }

Lorsqu'il y a un conflit entre la configuration de la classe et la configuration d'une méthode,
la dernière surcharge la première::

    /**
     * @Cache(expires="tomorrow")
     */
    class BlogController extends Controller
    {
        /**
         * @Cache(expires="+2 days")
         */
        public function indexAction()
        {
        }
    }

Attributs
---------

Voici une liste des attributs valides, ainsi que les entêtes
HTTP correspondants :

============================== ================
Annotation                     Méthode Response
============================== ================
``@Cache(expires="tomorrow")`` ``$response->setExpires()``
``@Cache(smaxage="15")``       ``$response->setSharedMaxAge()``
``@Cache(maxage="15")``        ``$response->setMaxAge()``
============================== ================

.. note::

   L'attribut ``expires`` accepte toute date valide qui est comprise par
   la fonction PHP ``strtotime()``.

