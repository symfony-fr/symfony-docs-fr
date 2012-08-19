.. index::
    single: Console; CLI
    single: Components; Console

Le Composant Console
====================

    Le composant Console facilite la création d'interfaces de ligne de commandes,
    belles et testables.

Le composant Console vous permet de créer des commandes de ligne de commandes.
Vos commandes de console peuvent être utilisées pour quelconque tâche récurrente,
comme des cronjobs, des imports, ou d'autres processus à exécuter par lots.

Installation
------------

Vous pouvez installer le composant de différentes manières :

* Utilisez le dépôt Git officiel (https://github.com/symfony/Console) ;
* Installez le via PEAR (`pear.symfony.com/Console`) ;
* Installez le via Composer (`symfony/console` dans Packagist).

Créer une Commande basique
--------------------------

Pour faire une commande de console qui nous accueille depuis la ligne de commandes, créez
un fichier ``GreetCommand.php`` et ajoutez-lui ce qui suit::

    namespace Acme\DemoBundle\Command;

    use Symfony\Component\Console\Command\Command;
    use Symfony\Component\Console\Input\InputArgument;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;

    class GreetCommand extends Command
    {
        protected function configure()
        {
            $this
                ->setName('demo:greet')
                ->setDescription('Greet someone')
                ->addArgument('name', InputArgument::OPTIONAL, 'Who do you want to greet?')
                ->addOption('yell', null, InputOption::VALUE_NONE, 'If set, the task will yell in uppercase letters')
            ;
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $name = $input->getArgument('name');
            if ($name) {
                $text = 'Hello '.$name;
            } else {
                $text = 'Hello';
            }

            if ($input->getOption('yell')) {
                $text = strtoupper($text);
            }

            $output->writeln($text);
        }
    }

Vous devez aussi créer le fichier à exécuter en ligne de commandes qui crée
une ``Application`` et lui ajoute les commandes::

    #!/usr/bin/env php
    # app/console
    <?php 

    use Acme\DemoBundle\Command\GreetCommand;
    use Symfony\Component\Console\Application;

    $application = new Application();
    $application->add(new GreetCommand);
    $application->run();

Testez la nouvelle commande de console en exécutant ce qui suit :

.. code-block:: bash

    $ app/console demo:greet Fabien

Cela va afficher ce qui suit sur votre ligne de commandes :

.. code-block:: text

    Hello Fabien

Vous pouvez aussi utiliser l'option ``--yell`` pour afficher tout en majuscules :

.. code-block:: bash

    $ app/console demo:greet Fabien --yell

Cela affiche::

    HELLO FABIEN

Ajouter de la couleur à l'affichage
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A chaque fois que vous affichez du texte, vous pouvez entourer le texte avec
des balises afin d'ajouter de la couleur à l'affichage. Par exemple::

    // texte vert
    $output->writeln('<info>foo</info>');

    // texte jaune
    $output->writeln('<comment>foo</comment>');

    // texte noir sur fond cyan
    $output->writeln('<question>foo</question>');

    // texte blanc sur fond rouge
    $output->writeln('<error>foo</error>');

Il est possible de définir vos propres styles en utilisant la classe
:class:`Symfony\\Component\\Console\\Formatter\\OutputFormatterStyle`::

    $style = new OutputFormatterStyle('red', 'yellow', array('bold', 'blink'));
    $output->getFormatter()->setStyle('fire', $style);
    $output->writeln('<fire>foo</fire>');

Les couleurs d'écriture et de fond disponibles sont : ``black`` (« noir »), ``red``
(« rouge »), ``green`` (« vert »), ``yellow`` (« jaune »), ``blue`` (« bleu »),
``magenta`` (« magenta »), ``cyan`` (« cyan ») et ``white`` (« blanc »).

Et les options disponibles sont : ``bold`` (« gras »), ``underscore`` (« souligné »),
``blink`` (« clignotant »), ``reverse`` (« inversé ») et ``conceal`` (« masqué »).

Utiliser des Arguments de Commande
----------------------------------

La partie la plus intéressante des commandes sont les arguments et options que
vous pouvez rendre disponibles. Les arguments sont les chaînes de caractères -
séparées par des espaces - qui viennent après le nom de la commande lui-même.
Ils sont ordonnés, et peuvent être optionnels ou requis. Par exemple, ajoutez
un argument optionnel ``last_name`` à la commande et faites en sorte que l'argument
``name`` soit requis::

    $this
        // ...
        ->addArgument('name', InputArgument::REQUIRED, 'Who do you want to greet?')
        ->addArgument('last_name', InputArgument::OPTIONAL, 'Your last name?')
        // ...

Vous avez maintenant accès à l'argument ``last_name`` depuis votre commande::

    if ($lastName = $input->getArgument('last_name')) {
        $text .= ' '.$lastName;
    }

La commande peut maintenant être utilisée de l'une des façons suivantes :

.. code-block:: bash

    $ app/console demo:greet Fabien
    $ app/console demo:greet Fabien Potencier

Utiliser des Options de Commande
--------------------------------

Contrairement aux arguments, les options ne sont pas ordonnées (ce qui signifie
que vous pouvez les spécifier dans n'importe quel ordre) et sont spécifiées avec
deux tirets (par exemple : ``--yell`` - vous pouvez aussi déclarer un raccourci
d'une lettre que vous pouvez appeler avec un unique tiret comme ``-y``). Les
options sont *toujours* optionnelles, et peuvent être déclarées de manière à
accepter une valeur (par exemple : ``dir=src``) ou simplement en tant que
drapeau booléen sans valeur (par exemple : ``yell``).

.. tip::

    Il est aussi possible de faire qu'une option accepte *optionnellement* une
    valeur (qui ferait que ``--yell`` ou ``yell=loud`` fonctionnerait). Les
    options peuvent être configurées pour accepter un tableau de valeurs.

Par exemple, ajoutez une nouvelle option à la commande qui peut être utilisée
pour spécifier combien de fois le message devrait être affiché::

    $this
        // ...
        ->addOption('iterations', null, InputOption::VALUE_REQUIRED, 'How many times should the message be printed?', 1)

Ensuite, utilisez cette commande pour afficher le message plusieurs fois :

.. code-block:: php

    for ($i = 0; $i < $input->getOption('iterations'); $i++) {
        $output->writeln($text);
    }

Maintenant, lorsque vous exécutez la tâche, vous pouvez spécifier de manière
optionnelle un drapeau ``--iterations`` :

.. code-block:: bash

    $ app/console demo:greet Fabien

    $ app/console demo:greet Fabien --iterations=5

Le premier exemple va afficher le résultat une seule fois, puisque ``iterations``
est vide et que par défaut il vaut ``1`` (le dernier argument de ``addOption``).
Le second exemple va afficher le résultat cinq fois.

Rappelez-vous bien que ces options ne tiennent pas compte de leur ordre. Donc,
n'importe laquelle des deux commandes suivantes va fonctionner :

.. code-block:: bash

    $ app/console demo:greet Fabien --iterations=5 --yell
    $ app/console demo:greet Fabien --yell --iterations=5

Il y a 4 variantes d'options que vous pouvez utiliser :

===========================  =================================================================================================
Option                       Value
===========================  =================================================================================================
InputOption::VALUE_IS_ARRAY  Cette option accepte de multiples valeurs (par exemple : ``--dir=/foo --dir=/bar``)
InputOption::VALUE_NONE      N'accepte pas de valeur en entrée pour cette option (par exemple : ``--yell``)
InputOption::VALUE_REQUIRED  Cette valeur est requise (par exemple : ``--iterations=5``), l'option elle-même reste optionnelle
InputOption::VALUE_OPTIONAL  Cette option peut ou non avoir une valeur (par exemple : ``yell`` ou ``yell=loud``)
===========================  =================================================================================================

Vous pouvez combiner VALUE_IS_ARRAY avec VALUE_REQUIRED ou VALUE_OPTIONAL de la manière suivante :

.. code-block:: php

    $this
        // ...
        ->addOption('iterations', null, InputOption::VALUE_REQUIRED | InputOption::VALUE_IS_ARRAY, 'How many times should the message be printed?', 1)

Demander de l'Information à l'Utilisateur
-----------------------------------------

Lorsque vous créez des commandes, vous avez la possibilité de collecter plus
d'informations de la part de l'utilisateur en lui posant des questions. Par exemple, supposons
que vous souhaitiez confirmer une action avant de l'exécuter réellement. Ajoutez
ce qui suit à votre commande::

    $dialog = $this->getHelperSet()->get('dialog');
    if (!$dialog->askConfirmation($output, '<question>Continue with this action?</question>', false)) {
        return;
    }

Dans ce cas, l'utilisateur va être interrogé par : « Continuer avec cette action » ;
et à moins qu'il ne réponde par ``y``, la tâche va arrêter son exécution. Le
troisième argument de ``askConfirmation`` est la valeur par défaut à retourner
si l'utilisateur ne rentre aucune valeur.

Vous pouvez aussi poser des questions nécessitant plus qu'une simple réponse telle oui/non.
Par exemple, si vous aviez besoin de savoir le nom de quelque chose, vous pourriez
faire la chose suivante::

    $dialog = $this->getHelperSet()->get('dialog');
    $name = $dialog->ask($output, 'Please enter the name of the widget', 'foo');

Tester les Commandes
--------------------

Symfony2 fournit plusieurs outils pour vous aider à tester vos commandes. La
plus utile est la classe :class:`Symfony\\Component\\Console\\Tester\\CommandTester`.
Elle utilise des classes « d'entrée et de sortie » spécifiques permettant de
faciliter le « testing » sans avoir de console réelle::

    use Symfony\Component\Console\Application;
    use Symfony\Component\Console\Tester\CommandTester;
    use Acme\DemoBundle\Command\GreetCommand;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        public function testExecute()
        {
            $application = new Application();
            $application->add(new GreetCommand());

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(array('command' => $command->getName()));

            $this->assertRegExp('/.../', $commandTester->getDisplay());

            // ...
        }
    }

La méthode :method:`Symfony\\Component\\Console\\Tester\\CommandTester::getDisplay`
retourne ce qui aurait été retourné durant un appel normal depuis la console.

Vous pouvez tester l'envoi d'arguments et d'options à la commande en les passant
en tant que tableau à la méthode
:method:`Symfony\\Component\\Console\\Tester\\CommandTester::getDisplay`::

    use Symfony\Component\Console\Tester\CommandTester;
    use Symfony\Component\Console\Application;
    use Acme\DemoBundle\Command\GreetCommand;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {

        //--

        public function testNameIsOutput()
        {
            $application = new Application();
            $application->add(new GreetCommand());

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(
                array('command' => $command->getName(), 'name' => 'Fabien')
            );

            $this->assertRegExp('/Fabien/', $commandTester->getDisplay());
        }
    }

.. tip::

    Vous pouvez aussi tester une application console entière en utilisant
    :class:`Symfony\\Component\\Console\\Tester\\ApplicationTester`.

Appeler une Commande existante
------------------------------

Si une commande dépend d'une autre ayant été exécutée avant elle, plutôt que de
demander à l'utilisateur de se rappeler de l'ordre d'exécution, vous pouvez
l'appeler directement vous-même. Cela est aussi utile si vous souhaitez créer
une commande « méta » qui exécute juste un ensemble de commandes (par exemple,
toutes les commandes qui ont besoin d'être exécutées lorsque le code du projet
a été modifié sur les serveurs de production : effacer le cache, générer les
proxys Doctrine2, préparer les fichiers Assetic, ...).

Appeler une commande depuis une autre est très simple::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $command = $this->getApplication()->find('demo:greet');

        $arguments = array(
            'command' => 'demo:greet',
            'name'    => 'Fabien',
            '--yell'  => true,
        );

        $input = new ArrayInput($arguments);
        $returnCode = $command->run($input, $output);

        // ...
    }

D'abord, vous :method:`Symfony\\Component\\Console\\Application::find` (« trouvez »
en français) la commande que vous voulez exécuter en passant le nom de cette dernière.

Ensuite, vous devez créer un nouvel :class:`Symfony\\Component\\Console\\Input\\ArrayInput`
avec les arguments et options que vous souhaitez passer à la commande.

Eventuellement, vous pouvez appelez la méthode ``run()`` qui va exécuter la commande
et retourner le code retourné par le commande (retourne la valeur de la méthode
``execute()`` de la commande).

.. note::

    La plupart du temps, appeler une commande depuis du code qui n'est pas
    exécuté depuis la ligne de commandes n'est pas une bonne idée pour plusieurs
    raisons. Mais le plus important, c'est que vous compreniez qu'il faut voir une
    commande comme un contrôleur ; il devrait utiliser le modèle pour faire quelque
    chose et afficher le retour à l'utilisateur. Donc, plutôt que d'appeler une commande
    depuis le Web, revoyez votre code et déplacez la logique dans une nouvelle classe.
