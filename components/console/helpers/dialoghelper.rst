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


Valider la réponse
------------------

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