.. index::
    single: Console; Events

Utilisez les événements
=======================

.. versionadded:: 2.3
    Les évenements de Console ont été ajouté à Symfony 2.3.

La classe d'Application du composant Console vous permet en option de vous brancher
sur le cycle de vie de l'application. Au lieu de réinventer la roue, il utilise
le composant EventDispatcher de Symfony pour faire le boulot::

    use Symfony\Component\Console\Application;
    use Symfony\Component\EventDispatcher\EventDispatcher;

    $application = new Application();
    $application->setDispatcher($dispatcher);
    $application->run();

L'événement ``ConsoleEvents::COMMAND``
--------------------------------------

**Utilisation courante**: Faire quelque chose avant que n'importe quelle commande
soit lancé( comme logger ce que la commande est train d' exécuter), ou afficher
certaines choses sur l'événement en cours d'exécution.

Avant l'exécution de toute commande, l'événement ``ConsoleEvents::COMMAND`` est
envoyé. Les écouteurs reçoivent un événement de classe
:class:`Symfony\\Component\\Console\\Event\\ConsoleCommandEvent`::

    use Symfony\Component\Console\Event\ConsoleCommandEvent;
    use Symfony\Component\Console\ConsoleEvents;

    $dispatcher->addListener(ConsoleEvents::COMMAND, function (ConsoleCommandEvent $event) {
        // récupérer l'instance de la saisie
        $input = $event->getInput();

        // récupérer la sortie
        $output = $event->getOutput();

        // récupérer la commande exécutée
        $command = $event->getCommand();

        // Ecrire à propos de la commande
        $output->writeln(sprintf('Avant le début de l\'exécution de la commande <info>%s</info>', $command->getName()));

        // récupérer l'application
        $application = $command->getApplication();
    });

L'événement ``ConsoleEvents::TERMINATE``
----------------------------------------

**Utilisation courante**: Réaliser certains nettoyages après que la commande
soit exécutée.

Après que la commande soit terminée, l'événement ``ConsoleEvents::TERMINATE`` est envoyé.
Il peut être utiliser pour faire certaines
actions à exécuter pour toutes les commandes ou pour nettoyer ce qui a été initié avec l'écouteur
``ConsoleEvents::COMMAND``( comme envoyer des logs, fermer une connection à la
base de données, envoyer des emails ...). Un écouteur peut aussi modifier le
code de sortie.

Les écouteurs reçoivent un événement de classe
:class:`Symfony\\Component\\Console\\Event\\ConsoleTerminateEvent` ::

    use Symfony\Component\Console\Event\ConsoleTerminateEvent;
    use Symfony\Component\Console\ConsoleEvents;

    $dispatcher->addListener(ConsoleEvents::TERMINATE, function (ConsoleTerminateEvent $event) {
        // récupérer la sortie
        $output = $event->getOutput();

        // récupérer la commande exécutée
        $command = $event->getCommand();

        // afficher des informations
        $output->writeln(sprintf('Après l\'exécution de la commande <info>%s</info>', $command->getName()));

        // Changer le code de sortie
        $event->setExitCode(128);
    });

.. tip::

    Cet événement est aussi envoyé quand une exception est lancée par une
    commande. Il est envoyé juste avant l'événement ``ConsoleEvents::EXCEPTION``.
    Le code de sortie reçu dans ce cas est le code d'exception.

L'événement ``ConsoleEvents::EXCEPTION``
----------------------------------------

**Utilisation courante**: Récupérer les exceptions lever lors de l'exécution de
la commande.

Quand une exception est levée par une commande, l'événement ``ConsoleEvents::EXCEPTION``
est envoyé. Un écouteur peut couvrir ou changer l'exception ou changer certaines
choses avant que l'exception soit lever par l'application.

Les écouteurs reçoivent un événement de classe
:class:`Symfony\\Component\\Console\\Event\\ConsoleForExceptionEvent` ::

    use Symfony\Component\Console\Event\ConsoleForExceptionEvent;
    use Symfony\Component\Console\ConsoleEvents;

    $dispatcher->addListener(ConsoleEvents::EXCEPTION, function (ConsoleForExceptionEvent $event) {
        $output = $event->getOutput();

        $command = $event->getCommand();

        $output->writeln(sprintf('Oops, L\'exception levée par la commande en cours <info>%s</info>', $command->getName()));

        // Récupérer le code de sortie (Le code de l'exception ou de sortie défini par l'événement ConsoleEvents::TERMINATE)
        $exitCode = $event->getExitCode();

        // change le type d'exception
        $event->setException(new \LogicException('Caught exception', $exitCode, $event->getException()));
    });

