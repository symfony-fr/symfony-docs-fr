.. index::
    single: Console; Changing the Default Command

Modifier la commande par défault
================================

.. versionadded:: 2.5
    La méthode :method:`Symfony\\Component\\Console\\Application::setDefaultCommand`
    a été ajouté dans le version 2.5.

``app/console`` lancera toujours ``ListCommand`` lorsqu'aucune commande n'est passée.
Pour changer la commande par défaut vous n'avez simplement qu'à passer le nom de la
commande que vous souhaitez lancer par défaut dans la méthode ``setDefaultCommand`` ::

    namespace Acme\Console\Command;

    use Symfony\Component\Console\Command\Command;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Output\OutputInterface;

    class HelloWorldCommand extends Command
    {
        protected function configure()
        {
            $this->setName('hello:world')
                ->setDescription('Outputs \'Hello World\'');
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $output->writeln('Hello World');
        }
    }

Exécuter l'application et changer la commande par défaut ::

    // application.php

    use Acme\Console\Command\HelloWorldCommand;
    use Symfony\Component\Console\Application;

    $command = new HelloWorldCommand();
    $application = new Application();
    $application->add($command);
    $application->setDefaultCommand($command->getName());
    $application->run();

Testez la nouvelle commande console par défaut en lançant la
commande suivante :

.. code-block:: bash

    $ php application.php

Cela affichera ce qui suit dans votre ligne de commande :

.. code-block:: text

    Hello Fabien

.. tip::

    Cette fonctionnalité a une limitation : il est impossible de l'utiliser
    avec des arguments.

Apprenez-en plus !
------------------

* :doc:`/components/console/single_command_tool`
