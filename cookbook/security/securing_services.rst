.. index::
   single: Security; Securing any service
   single: Security; Securing any method

Comment sécuriser n'importe quel service ou méthode de votre application
========================================================================

Dans le chapitre sur la sécurité, vous pouvez voir comment
:ref:`sécuriser un contrôleur<book-security-securing-controller>` en
récupérant le service ``security.context`` depuis le Conteneur de
Service et en vérifiant le rôle actuel de l'utilisateur::

    // ...
    use Symfony\Component\Security\Core\Exception\AccessDeniedException;

    public function helloAction($name)
    {
        if (false === $this->get('security.context')->isGranted('ROLE_ADMIN')) {
            throw new AccessDeniedException();
        }

        // ...
    }

Vous pouvez aussi sécuriser *n'importe quel* service d'une manière similaire en lui injectant
le service ``security.context``. Pour une introduction générale sur l'injection
de dépendances dans un service, voyez le chapitre :doc:`/book/service_container` du
book. Par exemple, supposons que vous ayez une classe ``NewsletterManager`` qui envoie
des emails et que vous souhaitiez restreindre son utilisation aux utilisateurs ayant
un rôle nommé ``ROLE_NEWSLETTER_ADMIN`` uniquement. Avant l'ajout de cette sécurité,
la classe ressemble à ceci :

.. code-block:: php

    // src/Acme/HelloBundle/Newsletter/NewsletterManager.php
    namespace Acme\HelloBundle\Newsletter;

    class NewsletterManager
    {

        public function sendNewsletter()
        {
            // c'est ici que vous effectuez le travail
        }

        // ...
    }

Votre but est de vérifier le rôle de l'utilisateur lorsque la méthode
``sendNewsletter()`` est appelée. La première étape pour y parvenir est d'injecter
le service ``security.context`` dans l'objet. Sachant qu'il serait assez risqué
de *ne pas* effectuer de vérification de sécurité, nous avons ici un candidat
idéal pour l'injection via le constructeur, qui garantit que l'objet du contexte
de sécurité sera disponible dans la classe ``NewsletterManager``::

    namespace Acme\HelloBundle\Newsletter;

    use Symfony\Component\Security\Core\SecurityContextInterface;

    class NewsletterManager
    {
        protected $securityContext;

        public function __construct(SecurityContextInterface $securityContext)
        {
            $this->securityContext = $securityContext;
        }

        // ...
    }

Puis, dans votre configuration de service, vous pouvez injecter le service :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager

        services:
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                arguments: [@security.context]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
        </parameters>

        <services>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <argument type="service" id="security.context"/>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');

        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(new Reference('security.context'))
        ));

Le service injecté peut dès lors être utilisé pour effectuer la vérification
de sécurité lorsque la méthode ``sendNewsletter()`` est appelée::

    namespace Acme\HelloBundle\Newsletter;

    use Symfony\Component\Security\Core\Exception\AccessDeniedException;
    use Symfony\Component\Security\Core\SecurityContextInterface;
    // ...

    class NewsletterManager
    {
        protected $securityContext;

        public function __construct(SecurityContextInterface $securityContext)
        {
            $this->securityContext = $securityContext;
        }

        public function sendNewsletter()
        {
            if (false === $this->securityContext->isGranted('ROLE_NEWSLETTER_ADMIN')) {
                throw new AccessDeniedException();
            }

            //...
        }

        // ...
    }

Si l'utilisateur actuel ne possède pas le rôle ``ROLE_NEWSLETTER_ADMIN``,
il lui sera demandé de se connecter.

Sécuriser des méthodes en utilisant des annotations
---------------------------------------------------

Vous pouvez aussi sécuriser des appels de méthodes dans n'importe quel service avec
des annotations en utilisant le bundle facultatif `JMSSecurityExtraBundle`_. Ce
bundle n'est pas inclus dans la Distribution Standard de Symfony2, mais vous pouvez
choisir de l'installer.

Pour activer la fonctionnalité des annotations, :ref:`taggez<book-service-container-tags>`
le service que vous voulez sécuriser avec le tag ``security.secure_service``
(vous pouvez aussi activer automatiquement cette fonctionnalité pour tous
les services, voir :ref:`l'encadré<securing-services-annotations-sidebar>`
ci-dessous) :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        # ...

        services:
            newsletter_manager:
                # ...
                tags:
                    -  { name: security.secure_service }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <!-- ... -->

        <services>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <!-- ... -->
                <tag name="security.secure_service" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $definition = new Definition(
            '%newsletter_manager.class%',
            array(new Reference('security.context'))
        ));
        $definition->addTag('security.secure_service');
        $container->setDefinition('newsletter_manager', $definition);

Vous pouvez ainsi parvenir aux mêmes résultats que ci-dessus en utilisant
une annotation::

    namespace Acme\HelloBundle\Newsletter;

    use JMS\SecurityExtraBundle\Annotation\Secure;
    // ...

    class NewsletterManager
    {

        /**
         * @Secure(roles="ROLE_NEWSLETTER_ADMIN")
         */
        public function sendNewsletter()
        {
            //...
        }

        // ...
    }

.. note::

    Les annotations fonctionnent car une classe proxy est créée pour votre
    classe qui effectue les vérifications de sécurité. Cela signifie que vous
    pouvez utiliser les annotations sur des méthodes « public » ou « protected »,
    mais que vous ne pouvez pas les utiliser avec des méthodes « private » ou
    avec des méthodes marquées comme « final »

Le ``JMSSecurityExtraBundle`` vous permet aussi de sécuriser les paramètres et
les valeurs retournées par les méthodes. Pour plus d'informations, lisez la
documentation du `JMSSecurityExtraBundle`_.

.. _securing-services-annotations-sidebar:

.. sidebar:: Activer la Fonctionnalité des Annotations pour tous les Services

    Quand vous sécurisez la méthode d'un service (comme expliqué ci-dessus), vous
    pouvez soit tagger chaque service individuellement, ou activer la
    fonctionnalité pour *tous* les services en une seule fois. Pour ce faire,
    définissez l'option de configuration ``secure_all_services`` à « true » :

    .. configuration-block::

        .. code-block:: yaml

            # app/config/config.yml
            jms_security_extra:
                # ...
                secure_all_services: true

        .. code-block:: xml

            <!-- app/config/config.xml -->
            <srv:container xmlns="http://symfony.com/schema/dic/security"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xmlns:srv="http://symfony.com/schema/dic/services"
                xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

                <jms_security_extra secure_controllers="true" secure_all_services="true" />

            </srv:container>

        .. code-block:: php

            // app/config/config.php
            $container->loadFromExtension('jms_security_extra', array(
                ...,
                'secure_all_services' => true,
            ));

    L'inconvénient de cette méthode est que, si elle est activée, le chargement
    initial de la page pourrait être très lent selon le nombre de services que
    vous avez défini.

.. _`JMSSecurityExtraBundle`: https://github.com/schmittjoh/JMSSecurityExtraBundle
