.. index::
   pair: Twig; Configuration Reference

Configuration de référence du TwigBundle
========================================

.. configuration-block::

    .. code-block:: yaml

        twig:
            exception_controller:  Symfony\Bundle\TwigBundle\Controller\ExceptionController::showAction
            form:
                resources:

                    # Par défaut:
                    - form_div_layout.html.twig

                    # Exemple:
                    - MyBundle::form.html.twig
            globals:

                # Exemples:
                foo:                 "@bar"
                pi:                  3.14

                # Exemples d'options, mais la manière la plus simple est celle ci-dessus
                some_variable_name:
                    # un id de service qui peut être la valeur
                    id:                   ~
                    # définissez à "service" ou laissez vide
                    type:                 ~
                    value:                ~
            autoescape:           ~
            base_template_class:  ~ # Example: Twig_Template
            cache:                "%kernel.cache_dir%/twig"
            charset:              "%kernel.charset%"
            debug:                "%kernel.debug%"
            strict_variables:     ~
            auto_reload:          ~
            optimizations:        ~

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:twig="http://symfony.com/schema/dic/twig"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/twig http://symfony.com/schema/dic/doctrine/twig-1.0.xsd">

            <twig:config auto-reload="%kernel.debug%" autoescape="true" base-template-class="Twig_Template" cache="%kernel.cache_dir%/twig" charset="%kernel.charset%" debug="%kernel.debug%" strict-variables="false">
                <twig:form>
                    <twig:resource>MyBundle::form.html.twig</twig:resource>
                </twig:form>
                <twig:global key="foo" id="bar" type="service" />
                <twig:global key="pi">3.14</twig:global>
            </twig:config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('twig', array(
            'form' => array(
                'resources' => array(
                    'MyBundle::form.html.twig',
                )
             ),
             'globals' => array(
                 'foo' => '@bar',
                 'pi'  => 3.14,
             ),
             'auto_reload'         => '%kernel.debug%',
             'autoescape'          => true,
             'base_template_class' => 'Twig_Template',
             'cache'               => '%kernel.cache_dir%/twig',
             'charset'             => '%kernel.charset%',
             'debug'               => '%kernel.debug%',
             'strict_variables'    => false,
        ));

Configuration
-------------

.. _config-twig-exception-controller:

exception_controller
....................

**type**: ``string`` **default**: ``Symfony\\Bundle\\TwigBundle\\Controller\\ExceptionController::showAction``

C'est le contrôleur qui est activé après qu'une exception est lancée dans votre
application. Le contrôleur par défaut 
(:class:`Symfony\\Bundle\\TwigBundle\\Controller\\ExceptionController`)
est responsable de l'affichage de templates spécifiques en cas d'erreurs
(voir :doc:`/cookbook/controller/error_pages`). Modifier cette option est
assez compliqué. Si vous avez besoin de personnaliser une page d'erreur,
vous devriez utiliser le lien ci-dessus. Si vous avez besoin d'ajouter un
comportement sur une exception, vous devriez ajouter un écouteur sur l'évènement
``kernel.exception`` (voir :ref:`dic-tags-kernel-event-listener`).
