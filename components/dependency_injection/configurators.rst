.. index::
   single: Dependency Injection; Service configurators

Configurer des services avec un configurateur de service
========================================================

Le configurateur de service est une fonctionnalité du containeur d'injection de
dépendances qui permet d'utiliser une fonction de rappel (callable) pour configurer
un service après son instanciation.

Vous pouvez spécifier une méthode d'un autre service, une fonction PHP ou une
méthode statique d'une classe. L'instance du service est passé à une fonction
de rappel permettant au configurateur d'effectuer ce qui est nécessaire à la
configuration du service après sa création.

Le configurateur de service peut être utiliser, par exemple, quand vous avez
un service qui nécessite une configuration complexe basée sur des paramètres de
configuration provenant de différentes sources/services. En utilisant un configurateur
externe, vous pouvez maintenir l'implementation du service proprement et le garder
découplée des autres objets qui fournissent la configuration.

Un autre cas d'utilisation intéressant, c'est lorsque vous avez plusieurs objets
qui partagent une configuration commune ou qui doivent être configurés de
manière similaire à l'exécution.

Par exemple, supposons que vous ayez une application où vous envoyez différents
types d'e-mails aux utilisateurs. Les e-mails sont transmis à travers différents
formateurs qui pourraient être activée ou non en fonction de certains paramètres
dynamiques de l'application.
Vous commencez la définition d'une classe ``NewsletterManager`` comme ceci::

    class NewsletterManager implements EmailFormatterAwareInterface
    {
        protected $mailer;
        protected $enabledFormatters;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setEnabledFormatters(array $enabledFormatters)
        {
            $this->enabledFormatters = $enabledFormatters;
        }

        // ...
    }

et ensuite, une classe ``GreetingCardManager``::

    class GreetingCardManager implements EmailFormatterAwareInterface
    {
        protected $mailer;
        protected $enabledFormatters;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setEnabledFormatters(array $enabledFormatters)
        {
            $this->enabledFormatters = $enabledFormatters;
        }

        // ...
    }

Comme mentionné précédemment, l'objectif est de définir les formateurs lors de
l'exécution en fonction des paramètres de l'application. Pour ce faire, vous avez
également une classe ``EmailFormatterManager`` qui est responsable du chargement
et de la validation du formatage permis par l'application::

    class EmailFormatterManager
    {
        protected $enabledFormatters;

        public function loadFormatters()
        {
            // code pour configurer comment les formateurs sont utilisés
            $enabledFormatters = array(...);
            // ...

            $this->enabledFormatters = $enabledFormatters;
        }

        public function getEnabledFormatters()
        {
            return $this->enabledFormatters;
        }

        // ...
    }

Si votre objectif est d'éviter d'avoir à coupler ``NewsletterManager`` et
``GreetingCardManager`` avec ``EmailFormatterManager``, alors vous voudrez
peut-être créer une classe configurateur pour configurer ces instances::

    class EmailConfigurator
    {
        private $formatterManager;

        public function __construct(EmailFormatterManager $formatterManager)
        {
            $this->formatterManager = $formatterManager;
        }

        public function configure(EmailFormatterAwareInterface $emailManager)
        {
            $emailManager->setEnabledFormatters(
                $this->formatterManager->getEnabledFormatters()
            );
        }

        // ...
    }

Le travail de ``EmailConfigurator`` consiste à injecter les filtres activés dans
``NewsletterManager`` et ``GreetingCardManager``  parce qu'ils ne sont pas conscients
de l'endroit d'où proviennent les filtres. En revanche, la classe ``EmailFormatterManager``
connaît les formateurs activés et la façon de les charger, en gardant la seule
responsabilité.

La configuration d'un configurateur de service
----------------------------------------------

La configuration pour les classes ci-dessus pourraient ressembler à ceci:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_mailer:
                # ...

            email_formatter_manager:
                class:     EmailFormatterManager
                # ...

            email_configurator:
                class:     EmailConfigurator
                arguments: ["@email_formatter_manager"]
                # ...

            newsletter_manager:
                class:     NewsletterManager
                calls:
                    - [setMailer, ["@my_mailer"]]
                configurator: ["@email_configurator", configure]

            greeting_card_manager:
                class:     GreetingCardManager
                calls:
                    - [setMailer, ["@my_mailer"]]
                configurator: ["@email_configurator", configure]


    .. code-block:: xml

        <services>
            <service id="my_mailer" ...>
              <!-- ... -->
            </service>
            <service id="email_formatter_manager" class="EmailFormatterManager">
              <!-- ... -->
            </service>
            <service id="email_configurator" class="EmailConfigurator">
                <argument type="service" id="email_formatter_manager" />
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="NewsletterManager">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
                <configurator service="email_configurator" method="configure" />
            </service>
            <service id="greeting_card_manager" class="GreetingCardManager">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
                <configurator service="email_configurator" method="configure" />
            </service>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setDefinition('my_mailer', ...);
        $container->setDefinition('email_formatter_manager', new Definition(
            'EmailFormatterManager'
        ));
        $container->setDefinition('email_configurator', new Definition(
            'EmailConfigurator'
        ));
        $container->setDefinition('newsletter_manager', new Definition(
            'NewsletterManager'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer'),
        ))->setConfigurator(array(
            new Reference('email_configurator'),
            'configure',
        )));
        $container->setDefinition('greeting_card_manager', new Definition(
            'GreetingCardManager'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer'),
        ))->setConfigurator(array(
            new Reference('email_configurator'),
            'configure',
        )));
