@Template
=========

Utilisation
-----------

L'annotation ``@Template`` associe un contrôleur à un nom de template::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    /**
     * @Template("SensioBlogBundle:Post:show")
     */
    public function showAction($id)
    {
        // récupère l'objet Post
        $post = ...;

        return array('post' => $post);
    }

Lorsque vous utilisez l'annotation ``@Template``, le contrôleur devrait retourner
un tableau de paramètres à passer à la vue plutôt que de retourner un objet ``Response``.

.. tip::

   Si l'action retourne un objet ``Response``, l'annotation ``@Template`` 
   sera simplement ignorée.

Si le template est nommé d'après les noms du contrôleur et de l'action, ce qui
est le cas dans l'exemple ci-dessus, vous pouvez même éviter de spécifier la valeur
de l'annotation::

    /**
     * @Template
     */
    public function showAction($id)
    {
        // récupère l'objet Post
        $post = ...;

        return array('post' => $post);
    }

.. note::
    
    Si vous utilisez PHP comme moteur de template, vous devrez le spécifier::

        /**
         * @Template(engine="php")
         */
        public function showAction($id)
        {
            // ...
        }

Et si les seuls paramètres à passer au template sont les arguments de méthodes,
vous pouvez utiliser l'attribut ``vars`` au lieu de retourner un tableau. C'est très
utile en association avec l':doc:`annotation <converters>` ``@ParamConverter``::

    /**
     * @ParamConverter("post", class="SensioBlogBundle:Post")
     * @Template("SensioBlogBundle:Post:show", vars={"post"})
     */
    public function showAction(Post $post)
    {
    }

Ce qui, grâce aux conventions, est équivalent à la configuration suivante::

    /**
     * @Template(vars={"post"})
     */
    public function showAction(Post $post)
    {
    }

Vous pouvez même la rendre encore plus concise car tout les arguments de méthodes
sont automatiquement passés au template si la méthode retourne ``null`` et que l'attribut
``vars`` n'est pas défini::

    /**
     * @Template
     */
    public function showAction(Post $post)
    {
    }
