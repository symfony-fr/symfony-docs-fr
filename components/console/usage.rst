.. index::
    single: Console; Usage

Utiliser les commandes de console, les raccourcis et les commandes préconstruites
=================================================================================

En plus des options que vous spécifiez dans vos commandes, il existe des
options ainsi que que des commandes préconstruites pour le composant Console.

.. note::

    Ces exemples supposent que vous avez ajouté un fichier ``app/console``
    pour exécutez l'interface de commande CLI::

        #!/usr/bin/env php
        # app/console
        <?php

        use Symfony\Component\Console\Application;

        $application = new Application();
        // ...
        $application->run();

Commandes préconstruites
~~~~~~~~~~~~~~~~~~~~~~~~

Il existe une commande ``list`` qui affiche la liste des options standard
et des commandes enregistrées :

.. code-block:: bash

    $ php app/console list

Vous pouvez également avoir le même affichage sans exécuter aucune commande

.. code-block:: bash

    $ php app/console

La commande d'aide liste les informations de la commande spécifiées. Par
exemple, pour avoir une aide sur la commande ``list``, exécutez :

.. code-block:: bash

    $ php app/console help list

Exécuter ``help`` sans spécifier aucune commande listera les options globales :

.. code-block:: bash

    $ php app/console help

Options globales
~~~~~~~~~~~~~~~~

Vous pouvez obtenir de l'aide sur n'importe quelle commande grâce à l'option ``--help``.
Pour obtenir de l'aide sur la commande list :

.. code-block:: bash

    $ php app/console list --help
    $ php app/console list -h

Vous pouvez supprimer l'affichage avec :

.. code-block:: bash

    $ php app/console list --quiet
    $ php app/console list -q

Vous pouvez obtenir des messages plus verbeux (si c'est supporté par la
commande) avec :

.. code-block:: bash

    $ php app/console list --verbose
    $ php app/console list -v

Si vous définissez des arguments facultatifs pour donner un nom et une version
à votre application::

    $application = new Application('Acme Console Application', '1.2');

alors vous pouvez utiliser :

.. code-block:: bash

    $ php app/console list --version
    $ php app/console list -V

pour obtenir l'affichage de ces informations :

.. code-block:: text

    Acme Console Application version 1.2

Si vous ne spécifiez pas les 2 arguments, alors cela affichera juste :

.. code-block:: text

    console tool

Vous pouvez forcer la coloration ANSI de l'affichage avec :

.. code-block:: bash

    $ php app/console list --ansi

ou la désactiver :

.. code-block:: bash

    $ php app/console list --no-ansi

Vous pouvez supprimer les questions interactives de la commande que vous
exécutez avec :

.. code-block:: bash

    $ php app/console list --no-interaction
    $ php app/console list -n

Syntaxe raccourcie
~~~~~~~~~~~~~~~~~~

Vous n'avez pas besoin de taper les noms de commande en entier. Vous pouvez
vous contenter de taper le nom raccourci non ambigu pour exécuter une commande.
En conséquence, s'il y a des commandes non conflictuelles, vous pouvez exécuter
``help`` comme ceci :

.. code-block:: bash

    $ php app/console h

Si vous avez des commandes qui utilisent ``:`` pour les espaces de noms, alors
il vous suffit juste de taper le texte raccourci non ambigu de chaque partie.
Si vous avez créé la commande ``demo:greet`` comme expliqué dans
:doc:`/components/console/introduction`, alors vous pouvez l'exécuter avec :

.. code-block:: bash

    $ php app/console d:g Fabien

Si vous tapez un raccourci de commande qui est ambigu (c-a-d si plusieurs
commandes correspondent), alors aucune commande ne sera exécutée et cela
affichera une liste de suggestion des commandes qu'il est possible de choisir.
