.. index::
   single: Console; Single command application

Comment construire une application qui est une commande unique
==============================================================

Lorsque vous construisez un outil de ligne de commande, vous n'avez peut
être pas besoin de fournir plusieurs commandes. Dans ce cas, devoir passer
plusieurs noms de commande à chaque fois peut être fastidieux. Heureusement,
il est possible de supprimer cette nécessité en étendant l'application::

    namespace Acme\Tool;

    use Symfony\Component\Console\Application;
    use Symfony\Component\Console\Input\InputInterface;

    class MyApplication extends Application
    {
        /**
         * Récupère le nom de la commande saisie.
         *
         * @param InputInterface $input L'interface de saisie
         *
         * @return string Le nom de la commande
         */
        protected function getCommandName(InputInterface $input)
        {
            // Cela devrait retourner le nom de votre commande.
            return 'my_command';
        }

        /**
         * Récupère les commandes par défaut qui sont toujours disponibles.
         *
         * @return array Un tableau d'instances de commandes par défaut
         */
        protected function getDefaultCommands()
        {
            // Conserve les commandes par défaut du noyau pour avoir la
            // commande HelpCommand en utilisant l'option --help
            $defaultCommands = parent::getDefaultCommands()

            $defaultCommands[] = new MyCommand();

            return $defaultCommands;
        }
    }

Lorsque vous appelez votre script de console, la commande `MyCommand`
sera maintenant toujours utilisée, sans avoir à saisir son nom.