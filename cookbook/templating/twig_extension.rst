.. index::
   single: Extensions Twig
   
Comment écrire une Extension Twig personnalisée
===============================================

La motivation principale d'écrire une extension est de déplacer du code
souvent utilisé dans une classe réutilisable, comme par exemple ajouter le support
pour l'internationalisation.
Une extension peut définir des tags, des filtres, des tests, des opérateurs,
des variables globales, des fonctions et des noeuds visiteurs.

Créer une extension permet aussi une meilleure séparation du code qui est
exécuté au moment de la compilation et du code nécessaire lors de l'exécution.
De ce fait, cela rend votre code plus rapide.

.. tip::

    Avant d'écrire vos propres extensions, jetez un oeil au `dépôt officiel des extensions Twig`_.

Créer la Classe Extension
-------------------------

Pour avoir votre fonctionnalité personnalisée, vous devez créer en premier lieu
une classe Extension Twig. En tant qu'exemple, nous allons créer un filtre « prix »
afin de formatter un nombre donné en un prix::

    // src/Acme/DemoBundle/Twig/AcmeExtension.php
    namespace Acme\DemoBundle\Twig;

    class AcmeExtension extends \Twig_Extension
    {
        public function getFilters()
        {
            return array(
                'price' => new \Twig_Filter_Method($this, 'priceFilter'),
            );
        }
        
        public function priceFilter($number, $decimals = 0, $decPoint = '.', $thousandsSep = ',')
        {
            $price = number_format($number, $decimals, $decPoint, $thousandsSep);
            $price = '$' . $price;

            return $price;
        }

        public function getName()
        {
            return 'acme_extension';
        }
    }
    
.. tip::

    Dans le même style que les filtres personnalisés, vous pouvez aussi ajouter des `fonctions`
    personnalisées et définir des `variables globales`.

Définir une Extension en tant que Service
-----------------------------------------

Maintenant, vous devez informer le Conteneur de Service de l'existence de votre
Extension Twig nouvellement créée :

.. configuration-block::

    .. code-block:: xml
        
        <!-- src/Acme/DemoBundle/Resources/config/services.xml -->
        <services>
            <service id="acme.twig.acme_extension" class="Acme\DemoBundle\Twig\AcmeExtension">
                <tag name="twig.extension" />
            </service>
        </services>

    .. code-block:: yaml
        
        # src/Acme/DemoBundle/Resources/config/services.yml
        services:
            acme.twig.acme_extension:
                class: Acme\DemoBundle\Twig\AcmeExtension
                tags:
                    - { name: twig.extension }

    .. code-block:: php

        // src/Acme/DemoBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $acmeDefinition = new Definition('\Acme\DemoBundle\Twig\AcmeExtension');
        $acmeDefinition->addTag('twig.extension');
        $container->setDefinition('acme.twig.acme_extension', $acmeDefinition);
         
.. note::

   Gardez en mémoire que les Extensions Twig ne sont pas chargées de manière
   paresseuse (« lazy loading » en anglais). Cela signifie qu'il y a de grandes
   chances que vous obteniez une **CircularReferenceException** ou une
   **ScopeWideningInjectionException** si quelconques services (ou votre
   Extension Twig dans ce cas) sont dépendants du service de requête.
   Pour plus d'informations, jetez un oeil sur
   :doc:`/cookbook/service_container/scopes`.
                
Utiliser l'Extension personnalisée
----------------------------------

Utiliser votre Extension Twig nouvellement créée n'est en rien différent
des autres :

.. code-block:: jinja

    {# affiche $5,500.00 #}
    {{ '5500'|price }}
    
Passez d'autres arguments à votre filtre :

.. code-block:: jinja
    
    {# affiche $5500,2516 #}
    {{ '5500.25155'|price(4, ',', '') }}
    
En savoir plus
--------------

Pour étudier le sujet des Extensions Twig plus en détail, veuillez jeter un coup d'oeil à
la `documentation des extensions Twig`_.

.. _`dépôt officiel des extensions Twig`: https://github.com/fabpot/Twig-extensions
.. _`documentation des extensions Twig`: http://twig.sensiolabs.org/doc/advanced.html#creating-an-extension
.. _`variables globales`: http://twig.sensiolabs.org/doc/advanced.html#id1
.. _`fonctions`: http://twig.sensiolabs.org/doc/advanced.html#id2
