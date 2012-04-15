Soumettre un correctif
======================

Les patches représentent la meilleur façon de fournir une correction de bug ou 
de proposer des améliorations à Symfony2.

Check-Liste
-----------

Le propos de cet check-liste est de s'assurer que les contributions seront 
examinées sans émettre des remarques purement formelles entrainant un
ralentissement du processus d'intégration de nouvelles fonctionnalités ou
d'élimination des erreurs, permettant ainsi une évolution rapide du framework 
Symfony2.

Il est conseillé de réaliser une requête en anglais, si cependant un mot ou une
expression ne trouvait pas de traduction, nous vous conseillons, d'écrire en
anglais la majorité de la requête et d'inclure en français les seuls passages 
intraduisibles.

Le titre de la requête (pull request) doit être préfixé avec le nom du composant
ou du bundle en relation.

.. code-block:: text

    [Composant] Courte description.

Un example de titre pourrait ressembler à ceci:

.. code-block:: text

    [Form] Add selectbox field type.

.. tip::

    Utiliser le préfixe "[WIP]" afin de mentionner une soumission incomplète ou
    dont les tests n'ont pas encore été effectués.

Toute requete devrait inclure le template suivant dans sa description:

.. code-block:: text

    Bug fix: [yes|no]
    Feature addition: [yes|no]
    Backwards compatibility break: [yes|no]
    Symfony2 tests pass: [yes|no]
    Fixes the following tickets: [comma separated list of tickets fixed by the PR]
    Todo: [list of todos pending]
    
Un example de champ descriptif pourrait donc ressembler à ceci:

.. code-block:: text

   Bug fix: no
   Feature addition: yes
   Backwards compatibility break: no
   Symfony2 tests pass: yes
   Fixes the following tickets: -
   Todo: -

Merci d'inclure ce template à chacune de vos soumissions!

.. tip::

   Tout ajout de nouvelles fonctionnalités doit être envoyé à la branche master,
   tandis les corrections de bug doivent être envoyées à la branche active 
   la plus ancienne (sauf bien entendu si cette correction ne concernent qu'une
   version    particulière). De plus toute soumission devra conserver la
   compatibilité ascendante du framework.

.. tip::

   Afin que l'ajout d'une fonctionalité puisse être testé automatiquement, sur 
   la branche destinatrice, vous pouvez ajouter votre "fork" à `travis-ci.org`_.
   Identifié vous en utilisant votre compte github.com et ensuite actionnez un
   simple bouton afin d'activer les tests automatisés. Ainsi à la place d'
   indiquer, dans votre requête "*Symfony2 tests pass: [yes|no]*", vous pouvez 
   ajouter un liens vers `travis-ci.org status icon`_. Pour plus de détails, 
   consulté `travis-ci.org Getting Started Guide`_.

Un environnement de travail agréable
------------------------------------

Avant de travailler sur Symfony2, configurer un environnement de travail
agréable à l'aide des logiciels suivant :

* Git;
* PHP version 5.3.2 or above;
* PHPUnit 3.6.4 or above.

Configurer votre compte git à l'aide des commandes suivantes

.. code-block:: bash

    $ git config --global user.name "Your Name"
    $ git config --global user.email you@example.com

.. tip::

    Si vous avez commencé à apprendre git, nous vous recommendons chaudement de 
    lire l'excellent et gratuit `ProGit`_.

.. tip::

    Utilisateurs windows: quand vous installer git, il vous est demander comment
    git doit traiter les fins de ligne (line ending), le choix par défaut est de
    remplacer tous les Lf par des CRLF. Si vous voulez contribuer à Symfony, il
    vous faudra sélectionner la méthod as-is afin que git convertissent vos 
    contributions dans votre dépot. Si vous avez déjà installé git, vous pouvez
    vérifier votre réglage grace à la commande:

    .. code-block:: bash

        $ git config core.autocrlf

    Cela retournant "false", "input" ou "true", "input" est la valeur attendu.
    Si ce n'était pas la valeur renvoyée, vous pouvez la modifier grâce à cette
    commande:

    .. code-block:: bash

        $ git config --global core.autocrlf input

    Remplacer --global par --local si vous désirez n'activer cette option que
    dans votre dépot actuel.

Obtenir le code source de Symfony2:

* Créer un compte `GitHub`_ et connectez vous;
* Forker le `Symfony2 repository`_ (cliquer sur le bouton "Fork");
* Après que l'action "hardcore forking" soit terminée, clonez votre fork 
  localement (cela créera un répertoire `symfony`):

.. code-block:: bash

      $ git clone git@github.com:USERNAME/symfony.git

* Ajouter un dépot distant à l'aide de la commande:

.. code-block:: bash

      $ cd symfony
      $ git remote add upstream git://github.com/symfony/symfony.git

Maintenant que Symfony2 est installé, vérifiez que tous les tests unitaires 
réussissent dans votre environnement comme c'est expliqué dans la documentation  
dédiée :doc:`document <tests>`.

Travailler sur un patch
-----------------------

Chaque fois que vous voulez travailler sur un patch pour une correction de bug
ou une amélioration, vous aurez besoin de créer une branche particulière.

Cette branche doit être basée sur la branche master pour une nouvelle 
fonctionnalité, sur la branche active la plus ancienne dans laquel le bug est
présent pour une correction de bug (ex. `2.0`).

Pour un ajout, créez la branche nommée à l'aide la commande suivante:

.. code-block:: bash

    $ git checkout -b BRANCH_NAME master

Où, si vous voulez fournir une correction de bug pour la branche 2.0, vous devez
d'abord télécharger la branche distance `2.0` localement:

.. code-block:: bash

    $ git checkout -t origin/2.0

Avant de créer une nouvelle branche à partir de celle-ci:

.. code-block:: bash

    $ git checkout -b BRANCH_NAME 2.0

.. tip::

   Utiliser un nom descriptif pour votre branche (`ticket_XXX` où `XXX` est
   le numéro indiqué dans la déclaration de bug est une convention efficace).

Les commandes précédentes vous positionnent automatiquement dans la branche
créée (vérifier la branche active à l'aide de `git branch`).

Travailler sur le code autant que vous le désirez and commiter aussi souvent que
vous le voulez mais garder à l'esprit de:

* Respecter les :doc:`standards <standards>` (utiliser `git diff --check` pour
  vérifier les espaces en bout de ligne);
* Ajouter des test unitaires afin de prouver que le bug est corrigé ou que l'
  ajout est fonctionnel;
* Réaliser des "commits" automique et séparer logiquement ceux-ci (utiliser le 
  pouvoir du `git rebase` afin d'éclaircir votre historique);
* Ecriver des messages parlant pour chacun des commits.

.. tip::

   Un bon message de commit est composé d'un résumé succint (la première ligne),
   suivi optionnellement par une ligne vide et par une description détaillée. 
   le résumé commence avec le composant sur lequel vous êtes en train de
   travailler entre crochets (``[DependencyInjection]``, ``[FrameworkBundle]``,
   ...). Utiliser un verbe (``fixed ...``, ``added ...``, ...) pour commencer
   votre résumé et n'ajouter pas de point.

Soumettre une requête
---------------------

Avant de soumettre un correctif, mettez à jour votre branche (requis si vous 
avez pris du temps à écrire votre correctif):

.. code-block:: bash

   $ git checkout master
   $ git fetch upstream
   $ git merge upstream/master
   $ git checkout BRANCH_NAME
   $ git rebase master

.. tip::

   Remplacer `master` par la version cible (ex. `2.0`) si vous travailler sur
   un correctif de bug.

Quand vous effectuez la commande ``rebase``, vous pouvez avoir besoin de fixer
des conflit de fusion. La commande ``git status`` vous montrera les fichiers 
non fusionnés *unmerged*. Résolvez tous les conflits et continuez le rebase:

.. code-block:: bash

    $ git add ... # Ajouter les fichiers dont le conflit est résolus
    $ git rebase --continue

Vérifiez à nouveau que tous les tests fonctionnent et envoyez (push) les
modifications effectuées sur votre branche:

.. code-block:: bash

    $ git push origin BRANCH_NAME

Vous pouvez maintenant entamer une discussion à propos de votre correctif sur la 
`liste de diffusion dédiée aux développements`_ ou effectuer une requête d'ajout
(pull request). (sur le dépot ``symfony/symfony``). Afin de faciliter le travail
de l'équipe centrale, incluez toujours les composants modifiés dans votre
requête de cette manière:

.. code-block:: text

    [Yaml] foo bar
    [Form] [Validator] [FrameworkBundle] foo bar

.. tip::

    Prenez soin d'indiquer comme cible (range) ``symfony:2.0`` si vous émettez
    un correctif de bug basé sur la branche 2.0.

Si vous envoyez un mail à la mailing liste, n'oubliez pas d'indiquer l'URL de 
référence de votre branche (ex. ``https://github.com/USERNAME/symfony.git/BRANCH_NAME``)
ou l'URL de votre requête.

En vous appuyant sur les retours de la liste de diffusion ou via les
commentaires de votre requête, vous pourrez être amené à corriger votre patch.
Avant de soumettre à nouveau celui-ci, pensez à ``rebaser`` votre dépot avec
upstream/master ou upstream/2.0, ne fusionner pas, et forcé l'envoi (push) vers 
``origin``:

.. code-block:: bash

    $ git rebase -f upstream/master
    $ git push -f origin BRANCH_NAME

.. note::

    quand vous effectuer un envoi avec l'option -f (ou --force), préciser
    toujours le nom de la branche explicitement pour éviter de cibler une autre 
    branches du dépot (--force réalise des actions sans controle, utilisez le
    avec attention).

Souvent, les modérateurs vous demanderons de "squasher" vos ``commits``. Cela
implique en fait de convertir de nombreux commits en une seule et unique
modification. Afin d'effectuer ceci, utiliser la commande rebase:

.. code-block:: bash

    $ git rebase -i head~3
    $ git push -f origin BRANCH_NAME

Le nombre 3 correspond au nombre de commits effectué sur votre branche. Après 
cette commande, un éditeur s'ouvrira vous montrant une liste de commits:

.. code-block:: text

    pick 1a31be6 first commit
    pick 7fc64b4 second commit
    pick 7d33018 third commit

Pour fusionner l'ensemble des commits en un seul, supprimer le mot "pick" avant 
le second et le dernier commit, et replacé le par le mot "squash" ou juste par
"s". Une fois sauvegarder, git commencera le ``rebasing``, et si celui-ci est 
réussi, vous demandera d'éditer le message de commit, qui par défaut est un 
listing de tous les messages précédents. Une fois terminé, executer la commande
push.

.. note::

    Tous les correctifs que vous produirez devront être réaliser sous la license
    MIT license, à moins que vous ne le précisiez explicitement dans votre code.

Tous les correctifs intégrés à la branche 2.0 seront fusionnés dans les branches 
de maintenance plus récentes. Par exemple, si vous soumettez une correction
pour la branche `2.0`, celui-ci sera aussi appliqué par l'équipe centrale sur la
branche `master`.

.. _ProGit:                                       http://progit.org/
.. _GitHub:                                       https://github.com/signup/free
.. _Symfony2 repository:                          https://github.com/symfony/symfony
.. _liste de diffusion dédiée aux développements: http://groups.google.com/group/symfony-devs
.. _travis-ci.org:                                http://travis-ci.org
.. _`travis-ci.org status icon`:                  http://about.travis-ci.org/docs/user/status-images/
.. _`travis-ci.org Getting Started Guide`:        http://about.travis-ci.org/docs/user/getting-started/