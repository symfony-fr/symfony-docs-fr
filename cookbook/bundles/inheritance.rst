.. index::
   single: Bundle; Héritage

Comment utiliser l'héritage de bundle pour surcharger certaines parties d'un bundle
===================================================================================

Lorsque vous travaillerez avec des bundles tiers, vous allez probablement rencontrer une
situation où vous voudrez surcharger un fichier de ce bundle tiers en le remplacant
par un fichier de l'un de vos propres bundles. Symfony vous fournit une manière très
pratique de surcharger des fichiers comme des contrôleurs, des templates et d'autres
fichiers présents dans le dossier ``Resources/`` d'un bundle.

Par exemple, supposons que vous installiez le `FOSUserBundle`_, mais que vous souhaitez
surcharger son template de base ``layout.html.twig``, ainsi que l'un de ses contrôleurs.
Supposons aussi que vous ayez votre propre ``AcmeUserBundle`` où vous voulez avoir les
fichiers de substitution. Commencez par déclarer le ``FOSUserBundle`` comme parent de
votre bundle::

    // src/Acme/UserBundle/AcmeUserBundle.php
    namespace Acme\UserBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeUserBundle extends Bundle
    {
        public function getParent()
        {
            return 'FOSUserBundle';
        }
    }

En effectuant ce simple changement, vous pouvez désormais surcharger plusieurs parties
du ``FOSUserBundle`` en créant simplement un fichier ayant le même nom.

.. note::

    Malgré le nom de la méthode, il n'y a pas de relation parent/enfant entre
    les bundles. Il s'agit juste d'une manière d'étendre et de surcharger un
    bundle existant.

Surcharger des contrôleurs
~~~~~~~~~~~~~~~~~~~~~~~~~~

Supposons que vous vouliez ajouter de la fonctionnalité à ``registerAction``
du ``RegistrationController`` résidant dans le ``FOSUserBundle``. Pour faire
cela, créez simplement votre propre fichier ``RegistrationController.php``,
surcharger la méthode originale du bundle, et changez sa fonctionnalité::

    // src/Acme/UserBundle/Controller/RegistrationController.php
    namespace Acme\UserBundle\Controller;

    use FOS\UserBundle\Controller\RegistrationController as BaseController;

    class RegistrationController extends BaseController
    {
        public function registerAction()
        {
            $response = parent::registerAction();
            
            // do custom stuff
            
            return $response;
        }
    }

.. tip::

    Suivant le degré de changement de fonctionnalité dont vous avez besoin,
    vous pourriez appeler ``parent::registerAction()`` ou alors remplacer
    complètement sa logique par la vôtre.

.. note::

    Surcharger des contrôleurs de cette façon fonctionne uniquement si le
    bundle réfère au contrôleur en utilisant la syntaxe standard
    ``FOSUserBundle:Registration:register`` dans les routes et templates.
    Ceci est la bonne pratique.

Surcharger des ressources : templates, routage, validation, etc.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La plupart des ressources peuvent aussi être surchargées en créant simplement un
fichier au même emplacement que dans votre bundle parent.

Par exemple, il est très courant d'avoir besoin de surcharger le template
``layout.html.twig`` du ``FOSUserBundle`` afin qu'il utilise le layout de
base de votre application. Comme le fichier réside à l'emplacement
``Resources/views/layout.html.twig`` dans le ``FOSUserBundle``, vous pouvez
créer votre propre fichier au même endroit dans le ``AcmeUserBundle``.
Symfony va complètement ignorer le fichier étant dans le ``FOSUserBundle``,
et utiliser le vôtre à la place.

Il en va de même pour les fichiers de routage, de configuration de la validation
et pour les autres ressources.

.. note::

    Surcharger des ressources fonctionne uniquement lorsque vous référez à
    des ressources via la méthode ``@FosUserBundle/Resources/config/routing/security.xml``.
    Si vous référez à des ressources sans utiliser le raccourci @NomDuBundle,
    ces dernières ne peuvent alors pas être surchargées.

.. caution::

   Les fichiers de traduction ne fonctionnent pas de la même manière que
   celle décrite ci-dessus. Tous les fichiers de traduction sont accumulés dans
   un ensemble de « groupements » (un pour chaque domaine). Symfony charge
   les fichiers de traduction des bundles en premier (dans l'ordre dans
   lequel les bundles sont initialisés) et ensuite ceux de votre répertoire
   ``app/Resources``. Si la même traduction est spéficiée dans deux ressources,
   c'est la traduction venant de la ressource chargée en dernier qui gagnera.

.. _`FOSUserBundle`: https://github.com/friendsofsymfony/fosuserbundle

