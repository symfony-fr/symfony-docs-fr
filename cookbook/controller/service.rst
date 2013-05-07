.. index::
   single: Contrôleur; En tant que Services

Comment définir des contrôleurs en tant que Services
====================================================

Dans le Book, vous avez appris comment un contrôleur
peut facilement être utilisé lorsqu'il étend la classe de base
:class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`. Bien que
ceci fonctionne très bien, les contrôleurs peuvent aussi être définis
en tant que services.

Pour faire référence à un contrôleur qui est défini en tant que service, utilisez
la notation avec deux-points (:). Par exemple, supposons que vous ayez
défini un service nommé ``my_controller`` et que vous voulez le transmettre
à une méthode appelée ``indexAction()`` à l'intérieur du service::

    $this->forward('my_controller:indexAction', array('foo' => $bar));

Vous devez utiliser la même notation quand vous définissez la valeur de
la route ``_controller`` :

.. code-block:: yaml

    my_controller:
        pattern:   /
        defaults:  { _controller: my_controller:indexAction }

Pour utiliser un contrôleur de cette manière, il doit être défini dans la
configuration du conteneur de services. Pour plus d'informations, voir le
chapitre :doc:`Service Container</book/service_container>`.

Lorsque vous utilisez un contrôleur défini en tant que service, il ne va pas
étendre la classe de base ``Controller`` dans la plupart des cas. Au lieu de
se reposer sur ses méthodes « raccourcis », vous allez intéragir directement
avec les services dont vous avez besoin. Heureusement, cela est généralement
très facile et la classe de base ``Controller`` elle-même est une formidable
source d'inspiration quant à comment effectuer de nombreuses tâches usuelles.

.. note::

    Spécifier un contrôleur en tant que service demande un petit plus de
    travail. L'avantage premier est que le contrôleur en entier ou tout
    service passé au contrôleur peut être modifié via la configuration du
    conteneur de services. Cela est spécialement utile lorsque vous
    développez un bundle open-source ou tout autre bundle qui va être
    utilisé dans beaucoup de projets différents. Donc, même si vous ne
    spécifiez pas vos contrôleurs en tant que services, vous allez probablement
    voir ceci effectué dans quelques bundles Symfony2 open-source.

Utiliser les annotations de routage
-----------------------------------

Lorsque vous utilisez les annotations pour définir le routage dans un contrôleur
défini comme service, vous devrez spécifier votre service comme suit::

    /**
     * @Route("/blog", service="my_bundle.annot_controller")
     * @Cache(expires="tomorrow")
     */
    class AnnotController extends Controller
    {
    }

Dans cet exemple, ``my_bundle.annot_controller`` devrait l'id de l'instance
du ``AnnotController`` définie dans le conteneur de services. Cette partie
est documentée dans le chapitre :doc:`/bundles/SensioFrameworkExtraBundle/annotations/routing`.