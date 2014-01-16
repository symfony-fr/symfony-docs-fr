.. index::
    single: Console; Usage

Comment utiliser la Console
===========================

La page :doc:`/components/console/usage` de la documentation des Composants
traite des options de la console. Lorsque vous utilisez la console comme partie
intégrante du framework full-stack, des options globales supplémentaires sont
également disponibles.

Par défaut, les commandes de la console sont exécutées dans l'environnement de
``dev``, vous pourriez vouloir changer cela pour certaines commandes. Par exemple,
vous pourriez vouloir exécuter certaines commandes dans l'environnement de ``prod``
pour des raisons de performance. Le résultat de certaines commandes sera différent
selon l'environnement, par exemple, la commande ``cache:clear`` ne videra le cache
que de l'environnement spécifié. Pour vider le cache de ``prod``, vous devez exécuter :

.. code-block:: bash

    $ php app/console cache:clear --env=prod

ou son équivalent :
 
.. code-block:: bash

    $ php app/console cache:clear -e prod

En plus de changer l'environnement, vous pouvez également choisir de
désactiver le mode debug. Cela peut être utile lorsque vous voulez exécuter des
commandes dans l'environnement de ``dev`` mais éviter de dégrader les performances
en collectant des données de debug :

.. code-block:: bash

    $ php app/console list --no-debug

Il existe un shell interactive qui vous permet de taper des commandes sans devoir
spécifier ``php app/console`` à chaque fois, ce qui est très utile si vous devez saisir
plusieurs commandes. Pour entrer dans le shell, exécutez :

.. code-block:: bash

    $ php app/console --shell
    $ php app/console -s

Vous pouvez maintenant vous contenter de saisir simplement le nom de la commande :

.. code-block:: bash

    Symfony > list

Lorsque vous utilisez le shell, vous pouvez choisir d'exécuter chaque commande
dans un processus distinct :

.. code-block:: bash

    $ php app/console --shell --process-isolation
    $ php app/console -s --process-isolation

Lorsque vous faites cela, la sortie ne sera pas colorisée et l'interactivité
n'est pas supportée, vous devrez donc passer chaque paramètre explicitement.

.. note::
	  
	  A moins que vous n'utilisiez des processus séparés, vider le cache dans
	  le shell n'aura aucun effet sur les commandes que vous exécutez. Ceci est
	  dû au fait que les fichiers de cache originaux sont toujours utilisés.