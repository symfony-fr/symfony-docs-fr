.. index::
   single: Console; Create commands

Comment créer une commande pour la Console
==========================================

La page Console de la partie Components (:doc:`/components/console/introduction`) décrit
comment créer une commande. Cet article du Cookbook aborde les différences
lorsque vous créez des commandes pour la console avec le framework Symfony2.

Enregistrement automatique des commandes
----------------------------------------

Pour que les commandes soient automatiquement disponibles dans Symfony2, créez
un répertoire ``Command`` dans votre bundle et créez un fichier php se terminant
par ``Command.php`` pour chaque commande que vous voulez créer. Par exemple, si
vous voulez étendre le bundle ``AcmeDemoBundle`` (disponible dans la Standard
Edition de Symfony2) pour vous saluer en ligne de commande, créez le fichier
``GreetCommand.php`` et insérez y le contenu suivant::

    // src/Acme/DemoBundle/Command/GreetCommand.php
    namespace Acme\DemoBundle\Command;

    use Symfony\Bundle\FrameworkBundle\Command\ContainerAwareCommand;
    use Symfony\Component\Console\Input\InputArgument;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;

    class GreetCommand extends ContainerAwareCommand
    {
        protected function configure()
        {
            $this
                ->setName('demo:greet')
                ->setDescription('Saluer une personne')
                ->addArgument('name', InputArgument::OPTIONAL, 'Qui voulez vous saluer??')
                ->addOption('yell', null, InputOption::VALUE_NONE, 'Si définie, la tâche criera en majuscules')
            ;
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $name = $input->getArgument('name');
            if ($name) {
                $text = 'Bonjour '.$name;
            } else {
                $text = 'Bonjour';
            }

            if ($input->getOption('yell')) {
                $text = strtoupper($text);
            }

            $output->writeln($text);
        }
    }

Cette commande sera maintenant automatiquement prête à être exécutée :

.. code-block:: bash

    $ app/console demo:greet Fabien

Tester les commandes
--------------------

Pour tester les commandes utilisées dans le cadre du framework, la classe
:class:`Symfony\\Bundle\\FrameworkBundle\\Console\\Application <Symfony\\Bundle\\FrameworkBundle\\Console\\Application>` devrait être
utilisée au lieu de la classe :class:`Symfony\\Component\\Console\\Application <Symfony\\Component\\Console\\Application>`::

    use Symfony\Component\Console\Tester\CommandTester;
    use Symfony\Bundle\FrameworkBundle\Console\Application;
    use Acme\DemoBundle\Command\GreetCommand;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        public function testExecute()
        {
            // mockez le Kernel ou créez en un selon vos besoins
            $application = new Application($kernel);
            $application->add(new GreetCommand());

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(array('command' => $command->getName()));

            $this->assertRegExp('/.../', $commandTester->getDisplay());

            // ...
        }
    }

Récupérer des services du Conteneur de services
-----------------------------------------------

En utilisant :class:`Symfony\\Bundle\\FrameworkBundle\\Command\\ContainerAwareCommand`
comme classe parente de la commande (au lieu de la classe basique
:class:`Symfony\\Component\\Console\\Command\\Command`), vous avez accès au conteneur
de services. En d'autres termes, vous avez accès à tous les services configurés.
Par exemple, vous pouvez facilement étendre la tâche pour gérer les traductions::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $name = $input->getArgument('name');
        $translator = $this->getContainer()->get('translator');
        if ($name) {
            $output->writeln($translator->trans('Hello %name%!', array('%name%' => $name)));
        } else {
            $output->writeln($translator->trans('Hello!'));
        }
    }
