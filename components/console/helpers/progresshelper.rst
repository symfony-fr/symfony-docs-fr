.. index::
    single: Console Helpers; Progress Helper
    
Progress Helper
===============

.. versionadded:: 2.2
    Le Helper ``progress`` a été ajouté à Symfony 2.2.

.. versionadded:: 2.3
    La méthode ``setCurrent`` a été ajouté à Symfony 2.3.

Quand vous exécutez une commande avec une longue execution, il peut être utile
d'afficher la progression de celle-ci:

.. image:: /images/components/console/progress.png

Pour afficher les détails de la progression, utilisez la classe :class:`Symfony\\Component\\Console\\Helper\\ProgressHelper`,
passez lui le nombre total d'unités, et avancer la progression au fur et à mesure
que la commande s'execute::

    $progress = $this->getHelperSet()->get('progress');

    $progress->start($output, 50);
    $i = 0;
    while ($i++ < 50) {
        // ... Le code à exécuter

        // avance la barre de progression d'une unité
        $progress->advance();
    }

    $progress->finish();

.. tip::


    Vous pouvez aussi définir la progression en cours en appelant la méthode
    :method:`Symfony\\Component\\Console\\Helper\\ProgressHelper::setCurrent`.

L'apparence de la barre de progression peut aussi être personnalisée, avec de nombreux
niveaux de verbosité. Chacun d'eux affiche différents éléments possibles - comme
le pourcentage d'achèvement, une barre de progression mobile ou l'information
en cours/total (ex. 10/50)::

    $progress->setFormat(ProgressHelper::FORMAT_QUIET);
    $progress->setFormat(ProgressHelper::FORMAT_NORMAL);
    $progress->setFormat(ProgressHelper::FORMAT_VERBOSE);
    $progress->setFormat(ProgressHelper::FORMAT_QUIET_NOMAX);
    // la valeur par défaut
    $progress->setFormat(ProgressHelper::FORMAT_NORMAL_NOMAX);
    $progress->setFormat(ProgressHelper::FORMAT_VERBOSE_NOMAX);

En plus, vous pouvez contrôler les différents caractères et la largeur utilisés
pour la barre de progression::

    // la dernière partie de la barre
    $progress->setBarCharacter('<comment>=</comment>');
    // la partie en cours de la barre
    $progress->setEmptyBarCharacter(' ');
    $progress->setProgressCharacter('|');
    $progress->setBarWidth(50);

Pour voir toutes les options possibles, regarder la documentation de l'API à la
classe :class:`Symfony\\Component\\Console\\Helper\\ProgressHelper`.

.. caution::

    Pour des questions de performances, faites attention si vous définissez un
    grand nombre total d'étapes. Par exemple, si vous itérez à travers un grand
    nombre d'éléments, envisager de définir la fréquence de rafraîchissement à
    une valeur plus haute en appelant la méthode :method:`Symfony\\Component\\Console\\Helper\\ProgressHelper::setRedrawFrequency`,
    afin de ne mettre à jour qu'à certaines iterations::

        $progress->start($output, 50000);

        // mis à jour que tous les 100 itérations
        $progress->setRedrawFrequency(100);

        $i = 0;
        while ($i++ < 50000) {
            // ... Le code à exécuter

            $progress->advance();
        }

