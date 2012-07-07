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

    Si votre IDE crée les fichiers de configuration dans le répertoire de votre
    projet, vous pouvez utiliser le fichier global ``.gitignore`` (pour tout les
    projets) ou le fichier ``.git/info/exclude`` (par projet) pour les ignorer.
    Lisez la [documentation Github] (https://help.github.com/articles/ignoring-files)

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

La description de la Pull Request doit inclure la check list suivante afin de
s'assurer que les contributions peuvent être relues et vérifiées sans échanges
inutiles avec vous, et que vos contributions peuvent être inclues dans Symfony2
aussi vite que possible :

.. code-block:: text

    Bug fix: [yes|no]
    Feature addition: [yes|no]
    Backwards compatibility break: [yes|no]
    Symfony2 tests pass: [yes|no]
    Fixes the following tickets: [liste de tickets corrigés séparés par une virgule]
    Todo: [liste de choses restantes à faire]
    License of the code: MIT
    Documentation PR: [Référence de la documentation de la PR s'il y en a]

Un exemple de soumission pourrait ressembler à ça :

.. code-block:: text

    Bug fix: no
    Feature addition: yes
    Backwards compatibility break: no
    Symfony2 tests pass: yes
    Fixes the following tickets: #12, #43
    Todo: -
    License of the code: MIT
    Documentation PR: symfony/symfony-docs#123

Dans la description de la Pull Request, donnez autant de détails que possible sur
les changements (n'hésitez pas à donner des exemples de code pour illustrer vos
propos). Si votre Pull Request concerne l'ajout ou la modification d'une
fonctionnalité existante, expliquez les raisons de ce changement. La description
d'une Pull Resquest est utile lors de la revue de code est fait office de référence
lorsque le code est mergé (la description de la Pull Request et tout les commentaires
qui y sont associés font partie du message de commit du merge).

En plus de cette Pull Request de « code », vous devez également envoyer une Pull Request
au `dépôt de la documentation`_ pour mettre à jour la documentation si besoin.

Retravaillez votre patch
~~~~~~~~~~~~~~~~~~~~~~~~

Selon les retours que vous aurez sur votre Pull Request, vous pourriez avoir
besoin de retravailler votre patch. Avant de le re-soumettre, faites un rebase
avec ``upstream/master`` ou ``upstream/2.0``, ne mergez pas; et forcez le push
vers origin :

.. code-block:: bash

    $ git rebase -f upstream/master
    $ git push -f origin NOM_BRANCHE

.. note::

    lorsque vous faites un ``push --force``, spécifiez toujours explicitement le
    nom de la branche pour éviter de mettre la pagaille dans votre dépôt (``--force``
    dit à git que vous voulez vraiment traiter des choses, donc faites attention).

Souvent, les modérateurs vous demanderont « d'aplanir » vos commits. Cela signifie que vous
devrez fusionner plusieurs commits en un seul. Pour faire cela, utilisez la commande rebase :

.. code-block:: bash

    $ git rebase -i HEAD~3
    $ git push -f origin NOM_BRANCHE

Ici, le nombre 3 doit être égal au nombre de commit de votre branche. Après que
vous avez tapé cette commande, un éditeur vous affichera une liste de commits :

.. code-block:: text

    pick 1a31be6 premier commit
    pick 7fc64b4 second commit
    pick 7d33018 troisième commit

Pour fusionner tous les commits dans le premier, supprimez le mot « pick »
avant le second et les derniers commits, et remplacez le par le mot « squash »
ou juste « s ». Quand vous enregistrez, fit commencera le rebase et, quand il aura
terminé, vous demandera de modifier le message de commit qui est, par défaut, une
liste des messages de chaque commit. Lorsque vous aurez terminé, lancez la commande push.

.. tip::

    Pour que votre branche soit automatiquement testée, vous pouvez ajouter votre
    fork à `travis-ci.org`_. Identifiez vous simplement en utilisant votre compte
    github.com et appuyez le bouton pour activer les tests automatiques. Dans votre
    Pull Request, plutôt que de spécifier « *Symfony2 tests pass: [yes|no]* », vous
    pouvez faire un lien vers l'`icone de status travis-ci.org`_. Pour plus de détails,
    lisez le `Guide de démarrage travis-ci.org`_. Cela peut être fait simplement en
    cliquant sur l'icone de la page de build de Travis. Sélectionnez votre branche
    puis copiez le contenu dans la description de la Pull Request.


.. _ProGit:                                       http://progit.org/
.. _GitHub:                                       https://github.com/signup/free
.. _dépôt Symfony2:                          https://github.com/symfony/symfony
.. _liste de diffusion dédiée aux développements: http://groups.google.com/group/symfony-devs
.. _travis-ci.org:                                http://travis-ci.org
.. _`icone de status travis-ci.org`:                  http://about.travis-ci.org/docs/user/status-images/
.. _`Guide de démarrage travis-ci.org`: http://about.travis-ci.org/docs/user/getting-started/
.. _`dépôt de la documentation`: https://github.com/symfony-fr/symfony-docs-fr


