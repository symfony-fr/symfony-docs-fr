Processus de sortie des versions
================================

Ce document explique le processus de sortie des versions de Symfony
(Symfony étant le code hébergé sur le `dépôt Git`_ symfony/symfony).

Symfony gère ses versions via un *modèle basé sur la date*; une nouvelle
version de Symfony sortira tout les *six mois*, une en *mai* et une en
*novembre*.

.. note::

    Ce processus a été adopté à partir de Symfony 2.2, et toutes les
    « règles » expliquées dans ce document devront être scrupuleusement
    respectées à partir de Symfony 2.4.

Développement
-------------

La période de six mois est divisées en deux phases :

* *Développement* : *Quatre mois* pour ajouter de nouvelles fonctionnalités
  et améliorer celles existantes;

* *Stabilisation*: *Deux mois* pour corriger les bugs, préparer la version et
  attendre que tout l'écosystème Symfony (bibliothèques tierces, bundles et
  projets qui utilisent Symfony) se mettent à jour.

Durant la phase de développement, toute nouvelle fonctionnalité peut être mise
de côté si elle n'est pas finalisée à temps, ou si elle n'est pas suffisamment
stable pour être intégrée à la version finale.

Maintenance
-----------

Chaque version de Symfony est maintenue pendant une période fixe, ce qui dépend
du type de version.

Version standard
~~~~~~~~~~~~~~~~~

Une version standard est maintenue pendant une période *huit mois*.

Version « Long Term Support »
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Tout les deux ans, une version Long Term Support (version LTS) est publiée.
Chaque version LTS est supportée pendant une période de *trois ans*.

.. note::

    Après la période de support de trois ans fourni par la communauté, un
    support payant est disponible auprès de `SensioLabs`_.

Planning
--------

Voici le planning des quelques premières version qui utilisent ce modèle :

.. image:: /images/release-process.jpg
   :align: center

* **Jaune** représente la phase de développement
* **Bleu** représente la phase de stabilisation
* **Vert** représente la phase de maintenance

Cela permet de connaitre précisément les dates de sortie et les périodes
de maintenance.

* *(special)* Symfony 2.2 sortira fin février 2013;
* *(special)* Symfony 2.3 (première LTS) sortira fin mai 2013;
* Symfony 2.4 sortira fin november 2013;
* Symfony 2.5 sortira fin mai 2014;
* ...

Rétrocompatibilité
------------------

Après la sortie de Symfony 2.3, la rétro compatibilité sera conservée
à tout prix. Si ce n'est pas possible, la fonctionnalité, l'amélioration ou
la correction de bug sera programmée pour la prochaine version majeure : Symfony 3.0.

.. note::

    Le travail sur Symfony 3.0 commencera dès qu'il y aura suffisamment de
    fonctionnalités majeures non rétrocompatibles en attente sur la todo liste.

Explications
------------

Ce processus a été adopté pour fournir plus de *visibilité* et de
*transparence*. Il a été établi selon les objectifs suivants :

* Raccourcir le cycle de sortie (permettre aux développeurs de bénéficier
  des nouvelles fonctionnalités plus rapidement);
* Donner plus de visibilité aux développeurs qui utilisent le framework ou
  les projets open-source qui utilisent Symfony;
* Améliorer l'expérience des contributeurs du coeur de Symfony: chacun
  sait quand une fonctionnalité sera disponible dans Symfony;
* Coordonner l'évolution de Symfony avec les projets PHP qui fonctionnent
  bien avec Symfony et les projets qui utilisent Symfony;
* Donner du temps à l'écosystème Symfony pour se mettre à jour avec les
  nouvelles versions (auteurs de bundles, contributeurs de documentation,
  traducteurs, ...). 

La période de six mois a été choisie pour que deux versions puissent sortir
par an. Cela permet également d'avoir pas mal de temps pour travailler sur de
nouvelles fonctionnalités, et cela permet également aux fonctionnalités qui
ne sont pas prêtes à temps de ne pas attendre trop longtemps avant d'être
intégrées dans la sortie du prochain cycle.

Le mode de maintenance dual a été choisi pour correspondre à chaque utilisateur
de Symfony. Ceux qui bougent vite et veulent travailler avec la meilleure et
la dernière version utiliseront les versions standard : une nouvelle version
est publiée tout les six mois et il y a une période de mise à jour de deux mois.
Les entreprises qui veulent plus de stabilité utiliseront les versions LTS :
une nouvelle version est publiée tout les deux ans et il y a une période de mise
à jour d'un an.

.. _dépôt Git: https://github.com/symfony/symfony
.. _SensioLabs:     http://sensiolabs.com/