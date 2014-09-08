.. index::
    single: Console Helpers; Dialog Helper

Helper Dialog
=============

La classe :class:`Symfony\\Component\\Console\\Helper\\DialogHelper` fournit
des fonctions qui demandent des informations à l'utilisateur. Elle est inclue
dans les helpers par défaut, que vous pouvez récupérer en appelant
:method:`Symfony\\Component\\Console\\Command\\Command::getHelperSet`::

    $dialog = $this->getHelperSet()->get('dialog');

Toutes les méthodes du Helper Dialog ont une 
:class:`Symfony\\Component\\Console\\Output\\OutputInterface` comme premier argument,
la question comme second argument et la valeur par défaut comme dernier argument.

Demander une confirmation à l'utilisateur
-----------------------------------------

Supposons que vous voulez confirmer une action avant de l'exécuter. AJouter
le code suivant à votre commande::

    // ...
    if (!$dialog->askConfirmation(
            $output,
            '<question>Continuer avec cette action ?</question>',
            false
        )) {
        return;
    }

Dans ce cas, la question « Continuer avec cette action ? » sera posée à l'utilisateur,
et elle retournera ``true`` si l'utilisateur répond ``y``, ou ``false`` dans les autres
cas. Le troisième argument de ``askConfirmation`` est la valeur par défaut à retourner
si l'utilisateur ne saisit rien.

Demander des informations à l'utilisateur
-----------------------------------------

Vous pouvez également poser une question dont la réponse serait plus qu'un simple
oui/non. Par exemple, si vous voulez connaitre le nom d'un bundle, vous pouvez
ajouter le code suivant à votre commande::

    // ...
    $bundle = $dialog->ask(
        $output,
        'Veuillez entrer le nom du bundle',
        'AcmeDemoBundle'
    );

La question « Veuillez entrer le nom du bundle » sera posée à l'utilisateur. Il
pourra taper un nom qui sera retourné à la méthode ``ask``. S'il n'entre aucun nom,
la valeur par défaut (``AcmeDemoBundle`` dans ce cas) sera retournée.

Autocompletion
~~~~~~~~~~~~~~

.. versionadded:: 2.2
    L'autocomplétion pour les questions a été ajoutée dans Symfony 2.2.

Vous pouvez aussi spécifier un tableau de réponses possibles pour une question
donnée. La réponse de l'utiliateur sera auto-complétée à mesure qu'il tape::

    $dialog = $this->getHelperSet()->get('dialog');
    $bundleNames = array('AcmeDemoBundle', 'AcmeBlogBundle', 'AcmeStoreBundle');
    $name = $dialog->ask(
        $output,
        'Please enter the name of a bundle',
        'FooBundle',
        $bundleNames
    );

Cacher la réponse de l'utilisateur
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.2
    La méthode ``askHiddenResponse`` a été ajoutée dans Symfony 2.2.
 
Vous pouvez également poser une question et cacher la réponse. Ceci
est particulièrement pratique pour les mots de passe::

    $dialog = $this->getHelperSet()->get('dialog');
    $password = $dialog->askHiddenResponse(
        $output,
        'Quel le mot de passe de la base de données ?',
        false
    );

.. caution::
   
    Lorsque vous demandez une réponse cachée, Symfony utilisera soit un binaire,
    soit il changera le mode stty, soit il utilisera autre chose pour cacher la
    réponse. Si aucun n'est disponible, il se rabattra sur une question classique
    à moins que vous n'ayez passé ``false`` comme troisième argument, comme dans
    l'exemple ci-dessus. Dans ce cas, une RuntimeException sera levée.

Poser une question et valider la réponse
----------------------------------------

Vous pouvez même valider la réponse. Par exemple, dans le dernier exemple, vous
avez demandé le nom d'un bundle. En suivant les conventions de nommage de Symfony2,
ce nom doit avoir ``Bundle`` comme suffixe. Vous pouvez valider cela en utilisant
la méthode :method:`Symfony\\Component\\Console\\Helper\\DialogHelper::askAndValidate`::

    // ...
    $bundle = $dialog->askAndValidate(
        $output,
        'Veuillez entrer le nom du bundle',
        function ($answer) {
            if ('Bundle' !== substr($answer, -6)) {
                throw new \RunTimeException(
                    'Le nom du bundle doit avoir \'Bundle\' comme suffixe.'
                );
            }
            return $answer;
        },
        false,
        'AcmeDemoBundle'
    );

Cette méthode a 2 nouveaux arguments, sa signature complète est::

    askAndValidate(
        OutputInterface $output, 
        string|array $question, 
        callback $validator, 
        integer $attempts = false, 
        string $default = null
    )

``$validator`` est un callback qui prend en charge la validation. Il devrait lever
une exception si quelque chose se passe mal. Le message de l'exception sera affiché
dans la console, donc c'est une bonne pratique d'y mettre des informations utiles.

Vous pouvez définir le nombre maximum de demandes dans l'argument ``$attempts``.
Si vous atteignez ce nombre, la valeur par défaut donnée en dernier argument sera
choisie. Utiliser ``false`` revient à définir un nombre d'essais illimité. La
demande sera faite à l'utilisateur jusqu'à ce qu'il propose une réponse valide.

Cacher la réponse de l'utilisateur
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.2
    La méthode ``askHiddenResponseAndValidate`` a été ajoutée dans Symfony 2.2.
   
Vous pouvez poser une question et valider une réponse cachée::

    $dialog = $this->getHelperSet()->get('dialog');

    $validator = function ($value) {
        if (trim($value) == '') {
            throw new \Exception('Le mot de passe ne peut pas être vide');
        }
        return $value;
    };

    $password = $dialog->askHiddenResponseAndValidate(
        $output,
        'Veuillez entrer le nom du widget',
        $validator,
        20,
        false
    );

Si vous voulez permettre qu'une réponse soit visible si elle ne peut pas être
cachée pour une raison quelconque, passez true comme cinquième argument.

Laisser l'utilisateur choisir parmi une liste de réponse
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.2
    La méthode :method:`Symfony\\Component\\Console\\Helper\\DialogHelper::select`
    a été ajouté depuis Symfony 2.2.

Si vous déterminez une liste de réponse dans laquelle l'utilisateur peut choisir,
vous pouvez utiliser la méthode ``ask`` citée précédemment, pour être sùr de la
réponse de l'utilisateur, la méthode ``askAndValidate``.Les deux ont le même
désavantage. Vous devez vous occuper de la gestion des valeurs incorrectes.

Au lieu de cela, vous pouvez utiliser la méthode
:method:`Symfony\\Component\\Console\\Helper\\DialogHelper::select` ,
qui permet de restreindre la saisie à la liste prédéfinie::

    $dialog = $app->getHelperSet()->get('dialog');
    $colors = array('rouge', 'bleu', 'jaune');

    $color = $dialog->select(
        $output,
        'Svp choisissez une couleur (par défaut rouge)',
        $colors,
        0
    );
    $output->writeln('Vous venez de sélectionner : '.$colors[$color]);

    // ... Utilisez la variable $color

L'option par défaut qui est utilisée, est fournie par le quatrième argument.
Si cet argument est à  ``null``, Cela signifie qu'il n'y a pas de valeur par
défaut.

Si l'utilisateur propose une valeur incorrecte, un message d'erreur est affiché
et il lui est demandé de faire à nouveau une proposition, jusqu'à ce que
l'utilisateur entre une valeur correcte ou que le nombre d'essais soit atteint
( que vous pouvez définir dans le cinquième argument). Le nombre d'essais est
par défaut à ``false``, ce qui signifie qu'il n'y a pas de limite d'essais.
Vous pouvez définir votre message d'erreur dans le sixième argument

.. versionadded:: 2.3
    le support de Multiselect a été ajouté à Symfony 2.3.

Multiple Choices
................

Certaines fois, de multiple réponses pourraient être valides. Le DialogHelper
permet cette fonctionnalité en utilisant des valeurs séparées par des virgules.
Cette possibilité est désactivée par défaut. Pour l'activer, définissez le
septième argument à ``true``::

    // ...

    $selected = $dialog->select(
        $output,
        'Sélectionnez votre couleur favorite (par défaut à rouge)',
        $colors,
        0,
        false,
        'La valeur "%s" est incorrecte',
        true // active l'option multiselect
    );

    $selectedColors = array_map(function($c) use ($colors) {
        return $colors[$c];
    }, $selected)

    $output->writeln('Vous venez de choisir: ' . implode(', ', $selectedColors));

Maintenant, quand les utilisateurs saisissent ``1,2``, Le résultat obtenu est :
 ``Vous venez de choisir: bleu, jaune``.

Tester une commande nécessitant une entrée
------------------------------------------

Si vous écrivez un test unitaire pour une commande qui nécessite la saisie dans
la ligne de commande, vous aurez besoin de surcharger le HelperSet utilisé par
la commande::

    use Symfony\Component\Console\Helper\DialogHelper;
    use Symfony\Component\Console\Helper\HelperSet;

    // ...
    public function testExecute()
    {
        // ...
        $commandTester = new CommandTester($command);

        $dialog = $command->getHelper('dialog');
        $dialog->setInputStream($this->getInputStream("Test\n"));
        // Equivalent à l'entrée par l'utilisateur de "Test" et appuie sur ENTER
        // Si vous avez besoin d'une confirmation, "yes\n" fonctionne.

        $commandTester->execute(array('command' => $command->getName()));

        // $this->assertRegExp('/.../', $commandTester->getDisplay());
    }

    protected function getInputStream($input)
    {
        $stream = fopen('php://memory', 'r+', false);
        fputs($stream, $input);
        rewind($stream);

        return $stream;
    }

En définissant le inputStream du ``DialogHelper``, vous imitez ce que fait la
console en interne avec tous les utilisateurs qui entrent des données via la
ligne de commande. De cette façon, vous pouvez tester toute interaction de
l'utilisateur (même complexes) en passant les bonnes valeurs.
