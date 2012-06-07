Soumettre un correctif
======================

Les patches représentent la meilleure manière de fournir une correction de bug ou 
de proposer des améliorations à Symfony2.

Step 1: Configurer votre environnement
--------------------------------------

Installer les logiciels
~~~~~~~~~~~~~~~~~~~~~~~

Avant de travailler sur Symfony2, installer un environnement avec les
logiciels suivants :

* Git;
* PHP version 5.3.3 ou supérieur;
* PHPUnit 3.6.4 ou supérieur.

Configurer Git
~~~~~~~~~~~~~~

Spécifiez vos informations personnelles en définissant votre nom et une adresse
email fonctionnelle :

.. code-block:: bash

    $ git config --global user.name "Votre nom"
    $ git config --global user.email votre_email@exemple.com

.. tip::

    Si vous découvrez Git, nous vous recommandons de lire l'excellent livre
    gratuit `ProGit`_.

.. tip::

    Pour les utilisateurs Windows : en installant Git, l'installeur vous demandera
    quoi faire avec les fins de lignes, et vous suggérera de remplacer tout les
    saut de lignes (LF pour Line Feed) par des fins de lignes (CRLF pour Carriage
    Return Line Feed). Ce n'est pas la bonne configuration si vous souhaitez contribuer
    à Symfony ! Conserver la configuration par défaut est le meilleur choix à faire, puisque
    Git convertira vos sauts de ligne (Line Feed) conformément à ceux du dépôt. Si vous avez
    déjà installé Git, vous pouvez vérifier la valeur en tapant :

    .. code-block:: bash

        $ git config core.autocrlf

    Cela retournera soit « false », « input » ou « true », « true » et « false » étant
    les mauvaises valeurs. Changez la en tapant :

    .. code-block:: bash

        $ git config --global core.autocrlf input

    Remplacez --global par --local si vous ne voulez le changer que pour le dépôt actif.

Récupérez le code source de Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Récupérez le code source de Symfony2 :

* Créez un compte `GitHub`_ et authentifiez vous;

* Faites un Fork du `dépôt Symfony2`_ (cliquez sur le bouton « Fork »);

* Après que le « hardcore » est terminé, clonez votre fork
  localement (cela créera un dossier `symfony`) :

.. code-block:: bash

      $ git clone git@github.com:USERNAME/symfony.git

* Ajoutez le dépôt distant comme ``remote``:

.. code-block:: bash

      $ cd symfony
      $ git remote add upstream git://github.com/symfony/symfony.git

Vérifiez que les tests sont validés
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant que Symfony2 est installé, vérifiez que tout les tests unitaires
passent sur votre environnement comme cela est expliqué dans le :doc:`document <tests>`
dédié.

Step 2: Travaillez sur votre patch
----------------------------------

La License
~~~~~~~~~~

Avant de commencer, vous devez savoir que tout les patchs que vous soumettrez
devront être sous *license MIT*, à moins de le spécifier clairement dans vos
commits.

Choose the right Branch
~~~~~~~~~~~~~~~~~~~~~~~

Before working on a patch, you must determine on which branch you need to
work. The branch should be based on the `master` branch if you want to add a
new feature. But if you want to fix a bug, use the oldest but still maintained
version of Symfony where the bug happens (like `2.0`).

.. note::

    All bug fixes merged into maintenance branches are also merged into more
    recent branches on a regular basis. For instance, if you submit a patch
    for the `2.0` branch, the patch will also be applied by the core team on
    the `master` branch.







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
    License of the code: MIT
    
Un exemple de champ descriptif pourrait donc ressembler à ceci:

.. code-block:: text

   Bug fix: no
   Feature addition: yes
   Backwards compatibility break: no
   Symfony2 tests pass: yes
   Fixes the following tickets: #12, #43
   Todo: -

Merci d'inclure ce template à chacune de vos soumissions!

..note::

    Tout patch que vous allez soumettre doit être sous licence MIT, sauf
    si explicitement spécifié dans le code

.. tip::

   Tout ajout de nouvelles fonctionnalités doit être envoyé à la branche master,
   tandis les corrections de bug doivent être envoyées à la branche active 
   la plus ancienne (sauf bien entendu si cette correction ne concernent qu'une
   version    particulière). De plus toute soumission devra conserver la
   compatibilité ascendante du framework.

Lorsque votre pull request ne concerne pas une correction de bug (par exemple
si vous ajoutez une nouvelle fonctionnalité ou en changez une existante), votre
pull request doit également inclure ce qui suit :


* Un résumé justifiant les changements dans la description de la pull request;

* Une explication des changements dans le(s) bon(s) fichier(s) CHANGELOG;

* Si les changements ne sont pas rétrocompatibles, une explication sur comment migrer
  une application dans le(s) bon(s) fichier(s) UPGRADE.

En plus, vous devrez aussi envoyer une pull request vers le `dépôt de la documentation`_
pour mettre à jour la documentation.

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

.. note::
    
    Ne corrigez jamais les conventions de codage dans vos pull request car cela
    rend le travail de relecture de nos équipes plus difficile.

Vous pouvez maintenant entamer une discussion à propos de votre correctif sur la 
`liste de diffusion dédiée aux développements`_ ou effectuer une requête d'ajout
(pull request). (sur le dépot ``symfony/symfony``). Afin de faciliter le travail
de l'équipe centrale, incluez toujours les composants modifiés dans votre
requête de cette manière:

.. code-block:: text

    [Yaml] Correction de quelque chose
    [Form] [Validator] [FrameworkBundle] Ajout de quelque chose

.. tip::

    Prenez soin d'indiquer comme cible (range) ``symfony:2.0`` si vous émettez
    un correctif de bug basé sur la branche 2.0.

Si vous envoyez un mail à la mailing liste, n'oubliez pas d'indiquer l'URL de 
référence de votre branche (ex. ``https://github.com/USERNAME/symfony.git/BRANCH_NAME``)
ou l'URL de votre requête.

En vous appuyant sur les retours de la liste de diffusion ou via les
commentaires de votre requête, vous pourrez être amené à corriger votre patch.

Avant de soumettre à nouveau celui-ci, fusionnez tout les commits non pertinents
(voir ci-dessous) qui concernent les corrections de conventions de codage ou la
corrections d'erreurs d'inattention dans votre code, puis faites un rebase avec
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

    $ git rebase -i HEAD~3
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
.. _`travis-ci.org Getting Started Guide`: http://about.travis-ci.org/docs/user/getting-started/
.. _`dépôt de la documentation`: https://github.com/symfony/symfony-docs