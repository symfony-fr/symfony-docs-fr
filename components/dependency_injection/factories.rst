.. index::
   single: Dependency Injection; Factories

Utiliser une « Factory » pour créer des services
================================================

Le Conteneur de Service de Symfony2 fournit une manière puissante de contrôler
la création d'objets, vous permettant de spécifier les arguments passés
au constructeur ainsi que d'appeler des méthodes et de définir des paramètres.
Parfois, cependant, ceci ne vous fournira pas tout ce dont vous avez
besoin pour construire vos objets. Dans cette situation, vous pouvez
utiliser une « factory » pour créer l'objet et informer le conteneur de
service d'appeler une méthode de la « factory » plutôt que d'instancier
l'objet directement.

Supposons que vous ayez une « factory » qui configure et retourne un nouvel
objet ``NewsletterManager``::

    class NewsletterFactory
    {
        public function get()
        {
            $newsletterManager = new NewsletterManager();
            
            // ...
            
            return $newsletterManager;
        }
    }

Pour rendre l'objet ``NewsletterManager`` disponible en tant que service,
vous pouvez configurer le conteneur de service afin qu'il utilise la classe
« factory » ``NewsletterFactory`` :

.. configuration-block::

    .. code-block:: yaml

        parameters:
            # ...
            newsletter_manager.class: NewsletterManager
            newsletter_factory.class: NewsletterFactory
        services:
            newsletter_manager:
                class:          "%newsletter_manager.class%"
                factory_class:  "%newsletter_factory.class%"
                factory_method: get 

    .. code-block:: xml

        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">NewsletterManager</parameter>
            <parameter key="newsletter_factory.class">NewsletterFactory</parameter>
        </parameters>

        <services>
            <service id="newsletter_manager" 
                     class="%newsletter_manager.class%"
                     factory-class="%newsletter_factory.class%"
                     factory-method="get"
            />
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->setParameter('newsletter_manager.class', 'NewsletterManager');
        $container->setParameter('newsletter_factory.class', 'NewsletterFactory');

        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->setFactoryClass(
            '%newsletter_factory.class%'
        )->setFactoryMethod(
            'get'
        );

Lorsque vous spécifiez la classe à utiliser pour la « factory » (via ``factory_class``)
la méthode sera appelée de manière statique. Si la « factory » elle-même doit
être instanciée et la méthode de l'objet résultant appelée (comme dans cet exemple),
configurez la « factory » elle-même comme un service :

.. configuration-block::

    .. code-block:: yaml

        parameters:
            # ...
            newsletter_manager.class: NewsletterManager
            newsletter_factory.class: NewsletterFactory
        services:
            newsletter_factory:
                class:            "%newsletter_factory.class%"
            newsletter_manager:
                class:            "%newsletter_manager.class%"
                factory_service:  newsletter_factory
                factory_method:   get 

    .. code-block:: xml

        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">NewsletterManager</parameter>
            <parameter key="newsletter_factory.class">NewsletterFactory</parameter>
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

        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->setParameter('newsletter_manager.class', 'NewsletterManager');
        $container->setParameter('newsletter_factory.class', 'NewsletterFactory');

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

    Le service « factory » est spécifié par son ID de nom et non pas par
    une référence au service lui-même. Donc vous ne devez pas utiliser la
    syntaxe « @ ».

Passer des Arguments à la Méthode de la « Factory »
---------------------------------------------------

Si vous devez passer des arguments à la méthode de la « factory », vous pouvez
utiliser l'option ``arguments`` du conteneur de service. Par exemple, supposons
que la méthode ``get`` de l'exemple précédent prenne le service ``templating``
en tant qu'argument :

.. configuration-block::

    .. code-block:: yaml

        parameters:
            # ...
            newsletter_manager.class: NewsletterManager
            newsletter_factory.class: NewsletterFactory
        services:
            newsletter_factory:
                class:            "%newsletter_factory.class%"
            newsletter_manager:
                class:            "%newsletter_manager.class%"
                factory_service:  newsletter_factory
                factory_method:   get
                arguments:
                    -             @templating

    .. code-block:: xml

        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">NewsletterManager</parameter>
            <parameter key="newsletter_factory.class">NewsletterFactory</parameter>
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

        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->setParameter('newsletter_manager.class', 'NewsletterManager');
        $container->setParameter('newsletter_factory.class', 'NewsletterFactory');

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
