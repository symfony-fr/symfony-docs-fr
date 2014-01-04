.. index::
    single: Console; CLI
    single: Components; Console

Le Composant Console
====================

    Le composant Console facilite la création d'interfaces en ligne de commandes,
    belles et testables.

Le composant Console vous permet de créer des commandes de ligne de commandes.
Vos commandes de console peuvent être utilisées pour n'importe quelle tâche récurrente,
comme des tâches cron, des imports, ou d'autres processus à exécuter par lots.

Installation
------------

Vous pouvez installer le composant de deux manières différentes :

* Utilisez le dépôt Git officiel (https://github.com/symfony/Console) ;
* :doc:`Installez le via Composer </components/using_components>` (``symfony/config`` à `Packagist`_).

.. note::

    Windows ne supporte pas les couleurs ANSI par défaut, donc le composant Console
    le détecte et désactive les couleurs où Windows n'apporte pas de support.
    Cependant, si Windows n'est pas configuré avec driver ANSI et que les commandes
    console appelle d'autres scripts qui envoient des séquences de couleurs ANSI,
    ils seront affichées en caractères bruts.

    Pour activer les couleurs ANSI dans Windows, installez `ANSICON`_.

Créer une Commande basique
--------------------------

Pour faire une commande de console qui vous accueille depuis la ligne de commandes, créez
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
                ->setDescription('Saluez quelqu\'un')
                ->addArgument(
                    'name',
                    InputArgument::OPTIONAL,
                    'Qui voulez vous saluez?'
                )
                ->addOption(
                   'yell',
                   null,
                   InputOption::VALUE_NONE,
                   'Si défini, la réponse est rendue en majuscules'
                )
            ;
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $name = $input->getArgument('name');
            if ($name) {
                $text = 'Salut, '.$name;
            } else {
                $text = 'Salut';
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
    <?php
    // app/console

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

    Salut, Fabien

Vous pouvez aussi utiliser l'option ``--yell`` pour afficher tout en majuscules :

.. code-block:: bash

    $ app/console demo:greet Fabien --yell

Cela affiche:

.. code-block:: bash

    SALUT, FABIEN

.. _components-console-coloring:

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

You can also set these colors and options inside the tagname::

    // green text
    $output->writeln('<fg=green>foo</fg=green>');

    // black text on a cyan background
    $output->writeln('<fg=black;bg=cyan>foo</fg=black;bg=cyan>');

    // bold text on a yellow background
    $output->writeln('<bg=yellow;options=bold>foo</bg=yellow;options=bold>');

Niveau de verbosité
~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    Les constantes ``VERBOSITY_VERY_VERBOSE`` et ``VERBOSITY_DEBUG`` ont été
    introduites avec la version 2.3.

la console a 5 niveau de verbosité. Ils sont définis dans la classe
:class:`Symfony\\Component\\Console\\Output\\OutputInterface`:

=======================================  =========================================
Mode                                     Valeur
=======================================  =========================================
OutputInterface::VERBOSITY_QUIET         N'affiche pas les messages
OutputInterface::VERBOSITY_NORMAL        Le niveau par défaut
OutputInterface::VERBOSITY_VERBOSE       Augmente la verbosité des messages
OutputInterface::VERBOSITY_VERY_VERBOSE  Les messages d'information non essentiels
OutputInterface::VERBOSITY_DEBUG         Les messages de de débuggage
=======================================  =========================================

Vous pouvez spécifié le niveau de verbosité silencieuse avec l'option ``--quiet``
ou ``-q``. L'option ``--verbose`` ou ``-v`` est utilisé quand vous voulez
augmenter le niveau de verbosité.

.. tip::

    La stacktrace de l'exception complète est affichée si le niveau
    ``VERBOSITY_VERBOSE`` ou supérieur est utilisé.

Il est possible d'afficher un message dans une commande pour un niveau spécifique
de verbosité. Par exemple::

    if (OutputInterface::VERBOSITY_VERBOSE <= $output->getVerbosity()) {
        $output->writeln(...);
    }

Quand le niveau ``quiet`` est utilisé, tous les affichages sont supprimés. la
méthode
:method:`Symfony\Component\Console\Output::write<Symfony\\Component\\Console\\Output::write>`
 retourne sans l'affichage actuel.

Utiliser des arguments de commande
----------------------------------

La partie la plus intéressante des commandes sont les arguments et options que
vous pouvez rendre disponibles. Les arguments sont les chaînes de caractères -
séparées par des espaces - qui viennent après le nom de la commande lui-même.
Ils sont ordonnés, et peuvent être optionnels ou obligatoires. Par exemple, ajoutez
un argument optionnel ``last_name`` à la commande et faites en sorte que l'argument
``name`` soit obligatoire::

     $this
        // ...
        ->addArgument(
            'name',
            InputArgument::REQUIRED,
            'Qui voulez vous saluer ?'
        )
        ->addArgument(
            'last_name',
            InputArgument::OPTIONAL,
            'Votre nom de famille ?'
        );

Vous avez maintenant accès à l'argument ``last_name`` depuis votre commande::

    if ($lastName = $input->getArgument('last_name')) {
        $text .= ' '.$lastName;
    }

La commande peut maintenant être utilisée de l'une des façons suivantes :

.. code-block:: bash

    $ app/console demo:greet Fabien
    $ app/console demo:greet Fabien Potencier

Il est aussi possible de passer une liste de valeur en argument (imaginez que
vous vouliez saluer tous vos amis). Pour effectuer ceci, vous devez le spécifier
à la fin de la liste d'arguments::

    $this
        // ...
        ->addArgument(
            'names',
            InputArgument::IS_ARRAY,
            'Qui voulez vous saluez (séparer les noms par des espaces)?'
        );

Pour l'utiliser, spécifiez combien de noms vous voulez:

.. code-block:: bash

    $ app/console demo:greet Fabien Ryan Bernhard

Vous accédez à l'argument ``names`` comme un tableau::

    if ($names = $input->getArgument('names')) {
        $text .= ' '.implode(', ', $names);
    }

Il y a 3 différents arguments que vous pouvez utiliser.:

===========================  ======================================================================================================
Mode                         Valeur
===========================  ======================================================================================================
InputArgument::REQUIRED      L'argument est requis
InputArgument::OPTIONAL      L'argument est optionnel et peut être omis
InputArgument::IS_ARRAY      L'argument peut contenir une infinité d'arguments et doit être utilisé à la fin de liste des arguments
===========================  ======================================================================================================

Vous pouvez combiner ``IS_ARRAY`` avec ``REQUIRED`` et ``OPTIONAL`` comme ceci::

    $this
        // ...
        ->addArgument(
            'names',
            InputArgument::IS_ARRAY | InputArgument::REQUIRED,
            'Qui voulez vous saluez (séparer les noms par des espaces)?'
        );

Utiliser des options de commande
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
        ->addOption(
            'iterations',
            null,
            InputOption::VALUE_REQUIRED,
            'Combien de fois voulez vous afficher le message ?',
            1
        );

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

Vous pouvez combiner ``VALUE_IS_ARRAY`` avec ``VALUE_REQUIRED`` ou ``VALUE_OPTIONAL`` de la manière suivante ::

    $this
        // ...
        ->addOption(
            'iterations',
            null,
            InputOption::VALUE_REQUIRED | InputOption::VALUE_IS_ARRAY,
            'Combien de fois voulez vous afficher le message ?',
            1
        );

Aides pour la Console
---------------

Le composant de console contient également un jeu d'outils ("helpers") de tailles différentes afin de vous aider dans différentes tâches:

* :doc:`/components/console/helpers/dialoghelper`: demande de question à l'utilisateur de manière interactif
* :doc:`/components/console/helpers/formatterhelper`: personnalisation de la coloration de sortie
* :doc:`/components/console/helpers/progresshelper`: affichage d'une barre de progression
* :doc:`/components/console/helpers/tablehelper`: affichage de données tabulaires comme une table

Tester les commandes
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
retourne ce qui aurait été rendu dans l'appel normal de la commande via la console.

Vous pouvez tester les arguments et options envoyés à la commande, en les passant
dans un tableau à la méthode :method:`Symfony\\Component\\Console\\Tester\\CommandTester::execute` ::

    use Symfony\Component\Console\Application;
    use Symfony\Component\Console\Tester\CommandTester;
    use Acme\DemoBundle\Command\GreetCommand;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        // ...

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

Appeler une commande existante
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

Ensuite, vous devez créer une nouvelle classe :class:`Symfony\\Component\\Console\\Input\\ArrayInput`
avec les arguments et options que vous souhaitez passer à la commande.

Éventuellement, vous pouvez appelez la méthode ``run()`` qui va exécuter la commande
et retourner le code retourné par le commande (retourne la valeur de la méthode
``execute()`` de la commande).

.. note::

    La plupart du temps, appeler une commande depuis du code qui n'est pas
    exécuté depuis la ligne de commandes n'est pas une bonne idée pour plusieurs
    raisons. Mais le plus important, c'est que vous compreniez qu'il faut voir une
    commande comme un contrôleur ; il devrait utiliser le modèle pour faire quelque
    chose et afficher le retour à l'utilisateur. Donc, plutôt que d'appeler une commande
    depuis le Web, revoyez votre code et déplacez la logique dans une nouvelle classe.

En savoir plus !
----------------

* :doc:`/components/console/usage`
* :doc:`/components/console/single_command_tool`

.. _Packagist: https://packagist.org/packages/symfony/console
.. _ANSICON: http://adoxa.3eeweb.com/ansicon/
