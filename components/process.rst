.. index::
   single: Process
   single: Components; Process

Le Composant « Process »
========================

    Le Composant « Process » exécute des commandes dans des sous-processus.

Installation
------------

Vous pouvez installer le composant de différentes manières :

* Utilisez le dépôt Git officiel (https://github.com/symfony/Process) ;
* Installez le via Composer (``symfony/process`` sur `Packagist`_).

Utilisation
-----------

La classe :class:`Symfony\\Component\\Process\\Process` vous permet d'exécuter
une commande dans un sous-processus::

    use Symfony\Component\Process\Process;

    $process = new Process('ls -lsa');
    $process->setTimeout(3600);
    $process->run();
    if (!$process->isSuccessful()) {
        throw new \RuntimeException($process->getErrorOutput());
    }

    print $process->getOutput();

La méthode :method:`Symfony\\Component\\Process\\Process::run` se charge
des différences subtiles entre les différentes plateformes lors de
l'exécution d'une commande.

Lorsque vous exécutez une commande durant un certain temps (comme effectuer un
rsync de fichiers vers un serveur distant), vous pouvez donner un retour
à l'utilisateur final en temps réel en passant une fonction anonyme à la
méthode :method:`Symfony\\Component\\Process\\Process::run`::

    use Symfony\Component\Process\Process;

    $process = new Process('ls -lsa');
    $process->run(function ($type, $buffer) {
        if ('err' === $type) {
            echo 'ERR > '.$buffer;
        } else {
            echo 'OUT > '.$buffer;
        }
    });

Si vous voulez exécuter du code PHP de manière isolée, utilisez plutôt
le ``PhpProcess`` à la place::

    use Symfony\Component\Process\PhpProcess;

    $process = new PhpProcess(<<<EOF
        <?php echo 'Hello World'; ?>
    EOF
    );
    $process->run();

.. versionadded:: 2.1
    La classe ``ProcessBuilder`` a été ajoutée avec la version 2.1.

Pour que votre code fonctionne mieux sur toutes les plateformes, vous
pourriez vouloir utiliser la classe
:class:`Symfony\\Component\\Process\\ProcessBuilder` à la place::

    use Symfony\Component\Process\ProcessBuilder;

    $builder = new ProcessBuilder(array('ls', '-lsa'));
    $builder->getProcess()->run();

.. _Packagist: https://packagist.org/packages/symfony/process