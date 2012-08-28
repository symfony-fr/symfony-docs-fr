Générer un contrôleur CRUD basé sur une entité Doctrine
=======================================================

Utilisation
-----------

la commande ``generate:doctrine:crud`` génère un contrôleur basique pour une
entité donnée, localisé dans un bundle donné. Ce contrôleur vous permet
d'effectuer les cinq opérations de base sur un modèle.

* Afficher les enregistrements,
* Afficher un seul enregistrement en se basant sur sa clé primaire,
* Créer un nouvel enregistrement,
* Modifier un enregistrement existant,
* Supprimer un enregistrement existant.

Par défaut, la commande est exécutée en mode interactif et vous pose des questions
pour définir le nom de l'entité, le préfixe de la route et s'il faut ou non générer
les actions :

.. code-block:: bash

    php app/console generate:doctrine:crud

Pour désactiver le mode interactif, utilisez l'option `--no-interaction` mais il
vous faudra alors penser à passer toutes les options obligatoires :

.. code-block:: bash

    php app/console generate:doctrine:crud --entity=AcmeBlogBundle:Post --format=annotation --with-write --no-interaction

Options disponibles
-------------------

* ``--entity``: Le nom de l'entité donné en notation raccourcie, contenant le nom
  du bundle dans lequel l'entité est localisée, ainsi que le nom de l'entité.
  Par exemple, ``AcmeBlogBundle:Post`` :

    .. code-block:: bash

        php app/console generate:doctrine:crud --entity=AcmeBlogBundle:Post

* ``--route-prefix``: Le préfixe à utiliser pour chaque route associée à une
  action :

    .. code-block:: bash

        php app/console generate:doctrine:crud --route-prefix=acme_post

* ``--with-write``: (**no**) [valeurs: yes|no] Spécifie s'il faut générer ou non
   les actions `new`, `create`, `edit`, `update` et `delete` :

    .. code-block:: bash

        php app/console generate:doctrine:crud --with-write

* ``--format``: (**annotation**) [valeurs: yml, xml, php ou annotation]
  Détermine le format à utiliser pour les fichiers de configuration généré
  comme le routage. Par défaut, la commande utilise le format ``annotation``.
  Choisir le format ``annotation`` implique que le ``SensioFrameworkExtraBundle``
  soit déjà installé :

    .. code-block:: bash

        php app/console generate:doctrine:crud --format=annotation
