SensioFrameworkExtraBundle
==========================

Le ``FrameworkBundle`` de Symfony2 implémente un framework MVC simple, mais
robuste et flexible. Le bundle `SensioFrameworkExtraBundle`_ l'étend et lui ajoute
des conventions et des annotations utiles. Il permet d'écrire des contrôleurs plus
simples.

Installation
------------

`Téléchargez`_ le bundle et placez-le sous l'espace de nom ``Sensio\Bundle\``.
Puis, comme n'importe quel autre bundle, incluez-le dans votre classe Kernel::

    public function registerBundles()
    {
        $bundles = array(
            ...

            new Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBundle(),
        );

        ...
    }

Configuration
-------------

Toutes les fonctionnalités fournies par le bundle sont activées par défaut
lorsque le bundle est enregistré dans votre classe Kernel.

La configuration par défaut est la suivante :

.. configuration-block::

    .. code-block:: yaml

        sensio_framework_extra:
            router:  { annotations: true }
            request: { converters: true }
            view:    { annotations: true }
            cache:   { annotations: true }

    .. code-block:: xml

        <!-- xmlns:sensio-framework-extra="http://symfony.com/schema/dic/symfony_extra" -->
        <sensio-framework-extra:config>
            <router annotations="true" />
            <request converters="true" />
            <view annotations="true" />
            <cache annotations="true" />
        </sensio-framework-extra:config>

    .. code-block:: php

        // charge le profileur
        $container->loadFromExtension('sensio_framework_extra', array(
            'router'  => array('annotations' => true),
            'request' => array('converters' => true),
            'view'    => array('annotations' => true),
            'cache'   => array('annotations' => true),
        ));

Vous pouvez désactiver certaines conventions ou certaines annotations en définissant
une ou plusieurs options à ``false``.

Annotations pour les contrôleurs
--------------------------------

Les annotations sont un super moyen de configurer facilement vos contrôleurs, depuis
les routes jusqu'à la configuration de mise en cache.

Même si les annotations ne sont pas une fonctionnalité native de PHP, elles ont tout
de même certains avantages sur les autres options de configuration de Symfony2 :

* Le code et la configuration sont au même endroit (la classe contrôleur) ;
* Simple à apprendre et à utiliser ;
* Facile à écrire ;
* Réduit la taille du contrôleur (puisque son seul rôle est de récupérer les données
  du Modèle).

.. tip::

   Si vous utilisez des classes pour vos vues, les annotations sont un excellent moyen
   d'éviter de devoir créer des classes pour vos vues pour les cas d'utilisations simples
   et classiques.

Les annotations suivantes sont définies par le bundle :

.. toctree::
   :maxdepth: 1

   annotations/routing
   annotations/converters
   annotations/view
   annotations/cache

L'exemple suivant montre comment utiliser chacune des annotations disponibles::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Cache;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Method;

    /**
     * @Route("/blog")
     * @Cache(expires="tomorrow")
     */
    class AnnotController extends Controller
    {
        /**
         * @Route("/")
         * @Template
         */
        public function indexAction()
        {
            $posts = ...;

            return array('posts' => $posts);
        }

        /**
         * @Route("/{id}")
         * @Method("GET")
         * @ParamConverter("post", class="SensioBlogBundle:Post")
         * @Template("SensioBlogBundle:Annot:post", vars={"post"})
         * @Cache(smaxage="15")
         */
        public function showAction(Post $post)
        {
        }
    }

Comme la méthode ``showAction`` suit certaines conventions, il est possible d'omettre
certaines annotations::

    /**
     * @Route("/{id}")
     * @Cache(smaxage="15")
     */
    public function showAction(Post $post)
    {
    }

.. _`SensioFrameworkExtraBundle`: https://github.com/sensio/SensioFrameworkExtraBundle
.. _`Téléchargez`: http://github.com/sensio/SensioFrameworkExtraBundle
