.. index::
   single: Console; Activer le logging

Comment activer les logs dans la commande console
=================================================

Le composant Console ne fournit aucune solution out of the box. Normallement,
vous lancez les commandes console manuellement et observez la sortie, c'est
pourquoi le système de log n'est pas fourni. Cependant, il y a quelques cas
où vous devriez en avoir besoin. Par example, si vous lancez une commande console
sans surveillance, comme des cron ou des scripts de déploiements, il serait plus
facile d'utiliser les fonctionnalités de log de Symfony, au lieu de configurer
d'autres outils pour rassembler la sortie console et la traiter. Ceci peut être
particulièrement pratique si vous avez déjà quelques paramètres existants pour
aggréger et analyser les logs Symfony.

Il y a fondamentalement deux cas de log dont vous auriez besoin :
 * Logguer manuellement quelques informations depuis votre commande;
 * Logguer les exceptions non catchées.

Logguer manuellement depuis la commande console
----------------------------------------------

Cette solution est vraiment simple. Lorsque vous créez une commande console
dans le framework complet comme décrit dans ":doc:`/cookbook/console/console_command`",
votre commande étend :class:`Symfony\\Bundle\\FrameworkBundle\\Command\\ContainerAwareCommand`.
Cela signifie que vous pouvez simplement accéder au service logger standard à travers
le conteneur de services et utilisez le pour logguer::

    // src/Acme/DemoBundle/Command/GreetCommand.php
    namespace Acme\DemoBundle\Command;

    use Symfony\Bundle\FrameworkBundle\Command\ContainerAwareCommand;
    use Symfony\Component\Console\Input\InputArgument;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;
    use Psr\Log\LoggerInterface;

    class GreetCommand extends ContainerAwareCommand
    {
        // ...

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            /** @var $logger LoggerInterface */
            $logger = $this->getContainer()->get('logger');

            $name = $input->getArgument('name');
            if ($name) {
                $text = 'Hello '.$name;
            } else {
                $text = 'Hello';
            }

            if ($input->getOption('yell')) {
                $text = strtoupper($text);
                $logger->warning('Yelled: '.$text);
            } else {
                $logger->info('Greeted: '.$text);
            }

            $output->writeln($text);
        }
    }

Selon l'environnement dans lequel vous lancez votre commande (et votre paramétrage de logging),
vous devriez voir les entrées logguées dans le fichier ``app/logs/dev.log`` ou `app/logs/prod.log``.

Activer le logging automatique des Exceptions
---------------------------------------------

Pour faire que votre application console récupére les logs des exceptions
manquées automatiquement pour toutes vos commande, vous pouvez utiliser
:doc:`console events</components/console/events>`.

.. versionadded:: 2.3
    Les évènements console ont été ajoutés en Symfony 2.3.

Dans un premier temps, configurez un écouteur (listener) pour les évènements exception
de la console dans le conteneur de service :

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            kernel.listener.command_dispatch:
                class: Acme\DemoBundle\EventListener\ConsoleExceptionListener
                arguments:
                    logger: "@logger"
                tags:
                    - { name: kernel.event_listener, event: console.exception }

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
                   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                   xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <parameters>
                <parameter key="console_exception_listener.class">Acme\DemoBundle\EventListener\ConsoleExceptionListener</parameter>
            </parameters>

            <services>
                <service id="kernel.listener.command_dispatch" class="%console_exception_listener.class%">
                    <argument type="service" id="logger"/>
                    <tag name="kernel.event_listener" event="console.exception" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->setParameter(
            'console_exception_listener.class',
            'Acme\DemoBundle\EventListener\ConsoleExceptionListener'
        );
        $definitionConsoleExceptionListener = new Definition(
            '%console_exception_listener.class%',
            array(new Reference('logger'))
        );
        $definitionConsoleExceptionListener->addTag(
            'kernel.event_listener',
            array('event' => 'console.exception')
        );
        $container->setDefinition(
            'kernel.listener.command_dispatch',
            $definitionConsoleExceptionListener
        );

Puis implémentez l'écouteur (listener)::

    // src/Acme/DemoBundle/EventListener/ConsoleExceptionListener.php
    namespace Acme\DemoBundle\EventListener;

    use Symfony\Component\Console\Event\ConsoleExceptionEvent;
    use Psr\Log\LoggerInterface;

    class ConsoleExceptionListener
    {
        private $logger;

        public function __construct(LoggerInterface $logger)
        {
            $this->logger = $logger;
        }

        public function onConsoleException(ConsoleExceptionEvent $event)
        {
            $command = $event->getCommand();
            $exception = $event->getException();

            $message = sprintf(
                '%s: %s (uncaught exception) at %s line %s while running console command `%s`',
                get_class($exception),
                $exception->getMessage(),
                $exception->getFile(),
                $exception->getLine(),
                $command->getName()
            );

            $this->logger->error($message);
        }
    }

Dans le code ci-dessus, lorsque l'une des commandes lance une exception, le
listener recevera un évènement. Vous pouvez simplement logguer en passant
le service logger via la configuration du service. Votre méthode reçoit un
objet :class:`Symfony\\Component\\Console\\Event\\ConsoleExceptionEvent`,
qui a une méthode pour récupérer les informations concernant l'évènement et
l'exception.

Logguer les statuts "non-0 exit"
--------------------------------

L'utilisation du logger de la console peut être poussé plus loin en logguant
les statuts "non-0 exit". De cette façon, vous saurez si une commande comporte
des erreurs, même si une aucune exception n'a été levée.

Dans un premier temps, configurez un écouteur pour l'évènement console.termine dans
le conteneur de services :

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            kernel.listener.command_dispatch:
                class: Acme\DemoBundle\EventListener\ConsoleTerminateListener
                arguments:
                    logger: "@logger"
                tags:
                    - { name: kernel.event_listener, event: console.terminate }

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
                   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                   xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <parameters>
                <parameter key="console_terminate_listener.class">Acme\DemoBundle\EventListener\ConsoleExceptionListener</parameter>
            </parameters>

            <services>
                <service id="kernel.listener.command_dispatch" class="%console_terminate_listener.class%">
                    <argument type="service" id="logger"/>
                    <tag name="kernel.event_listener" event="console.terminate" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->setParameter(
            'console_terminate_listener.class',
            'Acme\DemoBundle\EventListener\ConsoleExceptionListener'
        );
        $definitionConsoleExceptionListener = new Definition(
            '%console_terminate_listener.class%',
            array(new Reference('logger'))
        );
        $definitionConsoleExceptionListener->addTag(
            'kernel.event_listener',
            array('event' => 'console.terminate')
        );
        $container->setDefinition(
            'kernel.listener.command_dispatch',
            $definitionConsoleExceptionListener
        );

Puis implémentez l'écouteur (listener)::

    // src/Acme/DemoBundle/EventListener/ConsoleExceptionListener.php
    namespace Acme\DemoBundle\EventListener;

    use Symfony\Component\Console\Event\ConsoleTerminateEvent;
    use Psr\Log\LoggerInterface;

    class ConsoleTerminateListener
    {
        private $logger;

        public function __construct(LoggerInterface $logger)
        {
            $this->logger = $logger;
        }

        public function onConsoleTerminate(ConsoleTerminateEvent $event)
        {
            $statusCode = $event->getExitCode();
            $command = $event->getCommand();

            if ($statusCode === 0) {
                return;
            }

            if ($statusCode > 255) {
                $statusCode = 255;
                $event->setExitCode($statusCode);
            }

            $this->logger->warning(sprintf(
                'Command `%s` exited with status code %d',
                $command->getName(),
                $statusCode
            ));
        }
    }
