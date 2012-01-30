.. index::
   single: Process

The Process Component
=====================

    The Process Component executes commands in sub-processes.

Installation
------------

You can install the component in many different ways:

* Use the official Git repository (https://github.com/symfony/Process);
* Install it via PEAR ( `pear.symfony.com/Process`);
* Install it via Composer (`symfony/process` on Packagist).

Usage
-----

<<<<<<< HEAD
The :class:`Symfony\\Component\\Process\\Process` class allows you to execute
=======
The `:class:Symfony\\Component\\Process\\Process` class allows you to execute
>>>>>>> d6023ef546934b47429ac6e71c35c7a010ba6b68
a command in a sub-process::

    use Symfony\Component\Process\Process;

    $process = new Process('ls -lsa');
    $process->setTimeout(3600);
    $process->run();
    if (!$process->isSuccessful()) {
        throw new RuntimeException($process->getErrorOutput());
    }

    print $process->getOutput();

<<<<<<< HEAD
The :method:`Symfony\\Component\\Process\\Process::run` method takes care
=======
The ``:method::Symfony\\Component\\Process\\Process:run()`` method takes care
>>>>>>> d6023ef546934b47429ac6e71c35c7a010ba6b68
of the subtle differences between the different platforms when executing the
command.

When executing a long running command (like rsync-ing files to a remote
server), you can give feedback to the end user in real-time by passing an
anonymous function to the
<<<<<<< HEAD
:method:`Symfony\\Component\\Process\\Process::run` method::
=======
``:method::Symfony\\Component\\Process\\Process:run()`` method::
>>>>>>> d6023ef546934b47429ac6e71c35c7a010ba6b68

    use Symfony\Component\Process\Process;

    $process = new Process('ls -lsa');
    $process->run(function ($type, $buffer) {
        if ('err' === $type) {
            echo 'ERR > '.$buffer;
        } else {
            echo 'OUT > '.$buffer;
        }
    });

If you want to execute some PHP code in isolation, use the ``PhpProcess``
instead::

    use Symfony\Component\Process\PhpProcess;

    $process = new PhpProcess(<<<EOF
        <?php echo 'Hello World'; ?>
    EOF);
    $process->run();

.. versionadded:: 2.1
    The ``ProcessBuilder`` class has been as of 2.1.

To make your code work better on all platforms, you might want to use the
<<<<<<< HEAD
:class:`Symfony\\Component\\Process\\ProcessBuilder` class instead::
=======
``:class:Symfony\Component\Process\ProcessBuilder`` class instead::
>>>>>>> d6023ef546934b47429ac6e71c35c7a010ba6b68

    use Symfony\Component\Process\ProcessBuilder;

    $builder = new ProcessBuilder(array('ls', '-lsa'));
    $builder->getProcess()->run();
