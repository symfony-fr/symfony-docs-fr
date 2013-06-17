.. index::
    single: Console Helpers; Formatter Helper

Formatter Helper
================

Le Formatter Helper fournit des fonctions pour formater la sortie avec les couleurs.
Vous pouvez faire des choses plus avancées avec cette aide
:ref:`components-console-coloring`.

La méthode :class:`Symfony\\Component\\Console\\Helper\\FormatterHelper`fait partie 
des helpers par défaut, que vous pouvez obtenir en appelant la méthode
:method:`Symfony\\Component\\Console\\Command\\Command::getHelperSet`::

    $formatter = $this->getHelperSet()->get('formatter');

Les méthodes retournent une chaîne, ce que vous pouvez rendre directement avec
:method:`OutputInterface::writeln<Symfony\\Component\\Console\\Output\\OutputInterface::writeln>`.

Ecrire un message avec une section
----------------------------------

Symfony propose un style défini lors de l'affichage d'un message qui appartient à une certaine
«section». Elle affiche la section en couleur et avec des crochets autour d'elle et le
message réelle à sa droite. La couleur en moins, cela pourrait ressembler à ça:

.. code-block:: text

    [UneSection] Un message en rapport avec cette section

Pour reproduire ce style, vous pouvez utilisez la méthode 
:method:`Symfony\\Component\\Console\\Helper\\FormatterHelper::formatSection`::

    $formattedLine = $formatter->formatSection(
        'Unesection',
        'Un message en rapport avec cette section'
    );
    $output->writeln($formattedLine);

Ecrire un message dans un bloc
------------------------------

Certaines fois, vous souhaiteriez afficher un bloc de texte avec une couleur de fond.
Symfony l'utilise pour afficher des messages d'erreur.

Si vous écrivez vos messages d'erreur sur plus d'une ligne, vous pouvez
remarquerez que le fond est aussi long que chaque ligne. Pour générer un bloc
utilisez la méthode :method:`Symfony\\Component\\Console\\Helper\\FormatterHelper::formatBlock`::

    $errorMessages = array('Erreur!', 'Quelque chose ne vas pas');
    $formattedBlock = $formatter->formatBlock($errorMessages, 'error');
    $output->writeln($formattedBlock);
    
Comme vous pouvez le voir, on peux passer un tableau de messages à la méthode
:method:`Symfony\\Component\\Console\\Helper\\FormatterHelper::formatBlock`
pour créer la sortie désirée. Si vous passez ``true`` en 3eme paramètre,
le bloc est formaté avec plus de style (une ligne vide au dessus et en dessous des messages, 
ainsi que deux espaces à gauche et à droite).

Dans le cas précédent, vous utilisiez le style prédéfini ``error``,
mais vous pouvez créer votre propre style, regardez :ref:`components-console-coloring`.
