Comment utiliser une « Factory » (« Fabrique » en français) pour créer des Services
===================================================================================

Le conteneur de service de Symfony2 fournit une manière puissante de contrôler la
création d'objets. Il vous permet en effet de spécifier des arguments à passer au
contrôleur aussi bien que d'appeler des méthodes ou de configurer des paramètres.
Parfois, cependant, cela ne vous fournit pas tout ce dont vous avez besoin pour
construire vos objets. Dans une telle situation, vous pouvez alors utiliser une « factory »
pour créer l'objet et avertir le conteneur de service afin qu'il appelle
une méthode de la fabrique qui s'occupera de créer l'objet plutôt que de 
l'instancier directement.

Supposons que vous ayez une fabrique qui configure et retourne un nouvel objet 
NewsletterManager ::

    namespace Acme\HelloBundle\Newsletter;

    class NewsletterFactory
    {
        public function get()
        {
            $newsletterManager = new NewsletterManager();
            
            // ...
            
            return $newsletterManager;
        }
    }

Afin de créer le nouvel objet ``NewsletterManager`` et de le rendre disponible
en tant que service, vous pouvez configurer un conteneur de service pour qu'il
utilise la fabrique ``NewsletterFactory`` :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager
            newsletter_factory.class: Acme\HelloBundle\Newsletter\NewsletterFactory
        services:
            newsletter_manager:
                class:          %newsletter_manager.class%
                factory_class:  %newsletter_factory.class%
                factory_method: get 

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
            <parameter key="newsletter_factory.class">Acme\HelloBundle\Newsletter\NewsletterFactory</parameter>
        </parameters>

        <services>
            <service id="newsletter_manager" 
                     class="%newsletter_manager.class%"
                     factory-class="%newsletter_factory.class%"
                     factory-method="get"
            />
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');
        $container->setParameter('newsletter_factory.class', 'Acme\HelloBundle\Newsletter\NewsletterFactory');

        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->setFactoryClass(
            '%newsletter_factory.class%'
        )->setFactoryMethod(
            'get'
        );

Quand vous précisez la classe à utiliser comme « factory » (via ``factory_class``),
la méthode sera appelée de manière statique (ici ``get``). Si la « factory » elle-même
doit être instanciée, elle sera elle-aussi définie comme un service :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager
            newsletter_factory.class: Acme\HelloBundle\Newsletter\NewsletterFactory
        services:
            newsletter_factory:
                class:            %newsletter_factory.class%
            newsletter_manager:
                class:            %newsletter_manager.class%
                factory_service:  newsletter_factory
                factory_method:   get 

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
            <parameter key="newsletter_factory.class">Acme\HelloBundle\Newsletter\NewsletterFactory</parameter>
        </parameters>

        <services>
            <service id="newsletter_factory" class="%newsletter_factory.class%"/>
            <service id="newsletter_manager" 
                     class="%newsletter_manager.class%"
                     factory-service="newsletter_factory"
                     factory-method="get"
            />
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');
        $container->setParameter('newsletter_factory.class', 'Acme\HelloBundle\Newsletter\NewsletterFactory');

        $container->setDefinition('newsletter_factory', new Definition(
            '%newsletter_factory.class%'
        ))
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->setFactoryService(
            'newsletter_factory'
        )->setFactoryMethod(
            'get'
        );

.. note::

   Le service « factory » est précisé par son identifiant et non par une référence
   au service lui-même. Vous n'avez donc pas besoin d'utiliser la syntaxe utilisant
   l'arobase (« @ »).

Transmettre des arguments à la méthode de la « factory »
--------------------------------------------------------

Si vous avez besoin de transmettre des arguments à la méthode de la « factory »,
vous pouvez utiliser l'option ``arguments`` à l'intérieur du conteneur de
service. Ainsi, supposons que la méthode ``get`` de l’exemple précédent demande
le service ``templating`` comme argument :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager
            newsletter_factory.class: Acme\HelloBundle\Newsletter\NewsletterFactory
        services:
            newsletter_factory:
                class:            %newsletter_factory.class%
            newsletter_manager:
                class:            %newsletter_manager.class%
                factory_service:  newsletter_factory
                factory_method:   get
                arguments:
                    -             @templating

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
            <parameter key="newsletter_factory.class">Acme\HelloBundle\Newsletter\NewsletterFactory</parameter>
        </parameters>

        <services>
            <service id="newsletter_factory" class="%newsletter_factory.class%"/>
            <service id="newsletter_manager" 
                     class="%newsletter_manager.class%"
                     factory-service="newsletter_factory"
                     factory-method="get"
            >
                <argument type="service" id="templating" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');
        $container->setParameter('newsletter_factory.class', 'Acme\HelloBundle\Newsletter\NewsletterFactory');

        $container->setDefinition('newsletter_factory', new Definition(
            '%newsletter_factory.class%'
        ))
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(new Reference('templating'))
        ))->setFactoryService(
            'newsletter_factory'
        )->setFactoryMethod(
            'get'
        );
