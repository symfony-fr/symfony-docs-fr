Soumettre un correctif
======================

Les patches représentent la meilleure manière de fournir une correction de bug ou 
de proposer des améliorations à Symfony2.

Step 1: Configurer votre environnement
--------------------------------------

Installer les logiciels
~~~~~~~~~~~~~~~~~~~~~~~

Avant de travailler sur Symfony2, installez un environnement avec les
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

Choisissez la bonne branche
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Avant de travailler sur un patch, vous devez déterminer sur quelle branche vous
devez travailler. La branche devrait être basée sur la branche `master` si vous
souhaitez ajouter une nouvelle fonctionnalité. Mais si vous voulez corriger un bug,
utiliser la dernière version maintenue de Symfony dans laquelle le bug apparait
(par exemple `2.0`).

.. note::
    
    Toute correction de bug mergée sur les branches de maintenance sera mergée
    sur une branche plus récente de manière régulière. Par exemple, si vous
    proposez une correction pour la branche `2.0`, elle sera également appliquée
    sur la branche `master` par notre équipe.

Créez une nouvelle branche
~~~~~~~~~~~~~~~~~~~~~~~~~~

Chaque fois que vous voulez travailler sur un patch lié à un bug ou à une nouvelle
fonctionnalité, créez une nouvelle branche :

.. code-block:: bash

    $ git checkout -b NOM_BRANCHE master

Ou, si vous voulez proposer un correctif pour un bug de la branche 2.0, récupérez
la branche `2.0` distante localement :

.. code-block:: bash

    $ git checkout -t origin/2.0

Puis créez une nouvelle branche basée sur la 2.0 pour travailler sur votre correctif :

.. code-block:: bash

    $ git checkout -b NOM_BRANCHE 2.0

.. tip::

    Utilisez un nom explicite pour votre branche (`ticket_XXX` où `XXX` est le numéro
    du ticket est une bonne convention pour les corrections de bugs).

La commande Checkout ci-dessus bascule automatiquement le code vers la branche
nouvellement créée (vérifiez la branche sur laquelle vous vous trouvez avec `git branch`).

Travaillez sur votre patch
~~~~~~~~~~~~~~~~~~~~~~~~~~

Travaillez sur le code autant que vous le désirez et commitez aussi souvent que
vous le voulez mais gardez bien à l'esprit de :

* Respecter les :doc:`standards <standards>` (utilisez `git diff --check` pour
  vérifier les espaces en bout de ligne, lisez aussi le conseil ci-dessous);

* Ajouter des test unitaires afin de prouver que le bug est corrigé ou que la
  nouvelle fonctionnalité est opérationnelle;

* Tâcher de ne pas casser la rétrocompatibilité (si c'est le cas, essayez de
  fournir une couche de compatibilité pour supporter l'ancienne version). Les
  patchs non rétrocompatibles ont moins de chance d'être mergés;

* Faire des commits bien (utilisez le pouvoir du `git rebase` afin d'avoir un
  historique clair et logique);

* Supprimer les commits non pertinents qui concernent les standards de code ou les
  corrections de fautes de frappe dans votre propre code;

* Ne jamais corriger les standards de code dans le code existant car cela rend la
  revue de code plus difficile;

* Ecrire des messages parlants pour chacun des commits (lisez le conseil ci-dessous).

.. tip::

    Vous pouvez vérifier les standards de code de votre patch grâce à
    [script](http://cs.sensiolabs.org/get/php-cs-fixer.phar) [src](https://github.com/fabpot/PHP-CS-Fixer):

    .. code-block:: bash

        $ cd /path/to/symfony/src
        $ php symfony-cs-fixer.phar fix . Symfony20Finder

.. tip::

   Un bon message de commit est composé d'un résumé succint (la première ligne),
   suivi optionnellement par une ligne vide et par une description plus détaillée. 
   Le résumé commence par le composant sur lequel vous êtes en train de
   travailler entre crochets (``[DependencyInjection]``, ``[FrameworkBundle]``,
   ...). Utilisez un verbe (``fixed ...``, ``added ...``, ...) pour commencer
   votre résumé et n'ajoutez pas de point à la fin.


Préparez votre patch avant de le soumettre
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si votre patch ne concerne pas une correction de bug (si vous ajoutez une nouvelle
fonctionnalité ou si vous en modifiez une existante par exemple), il doit également
inclure ce qui suit :

* Une explication des changements effectués dans le(s) fichier(s) CHANGELOG correspondant(s);

* Une explication sur comment mettre à jour une application existante dans le(s) fichier(s)
  UPGRADE correspondant(s), si vos changements ne sont pas rétrocompatibles.

Step 3: Soumettez votre patch
-----------------------------

Lorsque vous pensez que votre patch est prêt à être envoyé, suivez les étapes
suivantes.

Mettez à jour votre patch avec rebase
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Avant de soumettre votre patch, mettez à jour votre branche (cela est
nécessaire si vous avez mis du temp à terminer vos changements) :

.. code-block:: bash

    $ git checkout master
    $ git fetch upstream
    $ git merge upstream/master
    $ git checkout NOM_BRANCHE
    $ git rebase master

.. tip::

    Remplacez `master` par `2.0` si vous travaillez sur une correction de bug.

Lorsque vous utilisez la commande ``rebase``, vous pourriez avoir des conflits
lors du merge. ``git status`` affichera les fichiers *non mergés*. Résolvez tout
les conflits avant de continuer le rebase :

.. code-block:: bash

    $ git add ... # ajouter les fichiers traités
    $ git rebase --continue

Vérifiez que tout les tests passent toujours et pushez votre branche
sur le dépôt distant :

.. code-block:: bash

    $ git push origin NOM_BRANCHE

Faites une Pull Request
~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez maintenant faire une pull request sur le dépôt ``symfony/symfony``.

.. tip::

    Assurez vous de faire pointer votre pull request vers ``symfony:2.0`` si
    vous voulez que notre équipe récupère une correction de bug basée sur la
    branche 2.0.

Pour faciliter le travail de l'équipe, incluez toujours le nom du composant
modifié dans votre message de pull request, comme ceci :

.. code-block:: text

    [Yaml] fixed something
    [Form] [Validator] [FrameworkBundle] added something

.. tip::
    
    Veuillez utiliser le tag "[WIP]" dans le titre si la soumission n'est
    pas encore prête ou si les tests ne sont pas complet ou ne passent pas encore.

The pull request description must include the following check list to ensure
that contributions may be reviewed without needless feedback loops and that
your contributions can be included into Symfony2 as quickly as possible:

.. code-block:: text

    Bug fix: [yes|no]
    Feature addition: [yes|no]
    Backwards compatibility break: [yes|no]
    Symfony2 tests pass: [yes|no]
    Fixes the following tickets: [comma separated list of tickets fixed by the PR]
    Todo: [list of todos pending]
    License of the code: MIT
    Documentation PR: [The reference to the documentation PR if any]

An example submission could now look as follows:

.. code-block:: text

    Bug fix: no
    Feature addition: yes
    Backwards compatibility break: no
    Symfony2 tests pass: yes
    Fixes the following tickets: #12, #43
    Todo: -
    License of the code: MIT
    Documentation PR: symfony/symfony-docs#123

In the pull request description, give as much details as possible about your
changes (don't hesitate to give code examples to illustrate your points). If
your pull request is about adding a new feature or modifying an existing one,
explain the rationale for the changes. The pull request description helps the
code review and it serves as a reference when the code is merged (the pull
request description and all its associated comments are part of the merge
commit message).

In addition to this "code" pull request, you must also send a pull request to
the `documentation repository`_ to update the documentation when appropriate.

Rework your Patch
~~~~~~~~~~~~~~~~~

Based on the feedback on the pull request, you might need to rework your
patch. Before re-submitting the patch, rebase with ``upstream/master`` or
``upstream/2.0``, don't merge; and force the push to the origin:

.. code-block:: bash

    $ git rebase -f upstream/master
    $ git push -f origin BRANCH_NAME

.. note::

    when doing a ``push --force``, always specify the branch name explicitly
    to avoid messing other branches in the repo (``--force`` tells git that
    you really want to mess with things so do it carefully).

Often, moderators will ask you to "squash" your commits. This means you will
convert many commits to one commit. To do this, use the rebase command:

.. code-block:: bash

    $ git rebase -i HEAD~3
    $ git push -f origin BRANCH_NAME

The number 3 here must equal the amount of commits in your branch. After you
type this command, an editor will popup showing a list of commits:

.. code-block:: text

    pick 1a31be6 first commit
    pick 7fc64b4 second commit
    pick 7d33018 third commit

To squash all commits into the first one, remove the word "pick" before the
second and the last commits, and replace it by the word "squash" or just "s".
When you save, git will start rebasing, and if successful, will ask you to
edit the commit message, which by default is a listing of the commit messages
of all the commits. When you finish, execute the push command.

.. tip::

    To automatically get your feature branch tested, you can add your fork to
    `travis-ci.org`_. Just login using your github.com account and then simply
    flip a single switch to enable automated testing. In your pull request,
    instead of specifying "*Symfony2 tests pass: [yes|no]*", you can link to
    the `travis-ci.org status icon`_. For more details, see the
    `travis-ci.org Getting Started Guide`_. This could easily be done by clicking
    on the wrench icon on the build page of Travis. First select your feature
    branch and then copy the markdown to your PR description.

.. _ProGit:                                       http://progit.org/
.. _GitHub:                                       https://github.com/signup/free
.. _Symfony2 repository:                          https://github.com/symfony/symfony
.. _liste de diffusion dédiée aux développements: http://groups.google.com/group/symfony-devs
.. _travis-ci.org:                                http://travis-ci.org
.. _`travis-ci.org status icon`:                  http://about.travis-ci.org/docs/user/status-images/
.. _`travis-ci.org Getting Started Guide`: http://about.travis-ci.org/docs/user/getting-started/
.. _`dépôt de la documentation`https://github.com/symfony-fr/symfony-docs-fr




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

