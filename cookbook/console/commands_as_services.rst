.. index::
    single: Console; Commands as Services

Comment définir des commandes en tant que services
==================================================

Par défaut, Symfony regarde dans le dossier ``Command`` de chaque bundle
et enregistre automatiquement vos commandes. Si une commande étend la classe
:class:`Symfony\\Bundle\\FrameworkBundle\\Command\\ContainerAwareCommand`,
Symfony va y injecter le conteneur de services.
Même si cela facilite la vie, il y a un certain nombre de limitation :

* Votre commande doit être présente dans le dossier ``Command``;
* Il n'existe pas de solution pour enregistrer votre service en fonction de
  l'environnement ou de la disponibilité de certaines dépendances;
* Vous ne pouvez pas accéder au conteneur dans la méthode ``configure()`` 
  (parce que la méthode ``setContainer`` n'a pas encore été appelée);
* Vous ne pouvez pas utiliser la même classe pour créer plusieurs commandes
  (c'est-à-dire chacune avec des configurations différentes).

Pour résoudre ces problèmes, vous pouvez enregistrer votre commande en tant 
que service et la taguer avec ``console.command``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            app.command.my_command:
                class: AppBundle\Command\MyCommand
                tags:
                    -  { name: console.command }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
            http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="app.command.my_command"
                    class="AppBundle\Command\MyCommand">
                    <tag name="console.command" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container
            ->register(
                'app.command.my_command',
                'AppBundle\Command\MyCommand'
            )
            ->addTag('console.command')
        ;

Utiliser les dépendances et les paramètres pour définir des valeurs par défaut aux options
------------------------------------------------------------------------------------------

Imaginez que vous voulez définir une valeur par défaut à l'option ``name``.
Vous pouvez passer l'une des solutions ci-dessous au 5ème argument de 
la méthode ``addOption()``:

* une chaine de caractère codée en dur;
* un paramètre du conteneur (par exemple quelque chose provenant de ``parameters.yml``);
* une valeur calculée par un service (par exemple un dépôt).

En étendant ``ContainerAwareCommand``, seule la première solution est possible car
vous ne pouvez pas accéder au conteneur à l'intérieur de la méthode ``configure()``.
Au lieu de cela, injectez les paramètres ou les services dont vous avez besoin dans 
le constructeur. Par exemple, supposez que vous stockez la valeur par défaut 
dans un paramètre ``%command.default_name%``::

    // src/AppBundle/Command/GreetCommand.php
    namespace AppBundle\Command;

    use Symfony\Component\Console\Command\Command;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;

    class GreetCommand extends Command
    {
        protected $defaultName;

        public function __construct($defaultName)
        {
            $this->defaultName = $defaultName;
            
            parent::__construct();
        }

        protected function configure()
        {
            // essayez d'éviter de faire des traitements ici (par ex : une requête en base de données)
            // cette méthode est *toujours* appelée - voir l'avertissement ci-dessous
            $defaultName = $this->defaultName;

            $this
                ->setName('demo:greet')
                ->setDescription('Saluer quelqu\'un')
                ->addOption(
                    'name',
                    '-n',
                    InputOption::VALUE_REQUIRED,
                    'Qui voulez-vous saluer ?',
                    $defaultName
                )
            ;
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $name = $input->getOption('name');

            $output->writeln($name);
        }
    }

Maintenant, vous avez juste à mettre à jour les arguments de la configuration de votre 
service comme habituellement, en y injectant le paramètre ``command.default_name``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            command.default_name: Javier

        services:
            app.command.my_command:
                class: AppBundle\Command\MyCommand
                arguments: ["%command.default_name%"]
                tags:
                    -  { name: console.command }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
            http://symfony.com/schema/dic/services/services-1.0.xsd">

            <parameters>
                <parameter key="command.default_name">Javier</parameter>
            </parameters>

            <services>
                <service id="app.command.my_command"
                    class="AppBundle\Command\MyCommand">
                    <argument>%command.default_name%</argument>
                    <tag name="console.command" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->setParameter('command.default_name', 'Javier');

        $container
            ->register(
                'app.command.my_command',
                'AppBundle\Command\MyCommand',
            )
            ->setArguments(array('%command.default_name%'))
            ->addTag('console.command')
        ;

Super, vous avez maintenant une valeur par défaut dynamique !

.. caution::

    Faites attention à ne pas faire de traitements dans la méthode ``configure``
    (par exemple faire une requête en base de données), car votre code sera exécuté 
    même si vous utilisez la console pour lancer une commande différente.
