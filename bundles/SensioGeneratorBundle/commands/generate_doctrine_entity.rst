Générer une nouvelle entité Doctrine
====================================

Utilisation
-----------

La commande ``generate:doctrine:entity`` génère une nouvelle entité Doctrine
incluant la définition du mapping ainsi que les propriété, getters et setters
de la classe.

Par défaut, la commande est exécutée en mode interactif et vous pose des questions
pour définir le nom de l'entité, son emplacement, son format de configuration et sa
structure par défaut :

.. code-block:: bash

    php app/console generate:doctrine:entity

	
Pour désactiver le mode interactif, utilisez l'option `--no-interaction` mais il
vous faudra alors penser à passer toutes les options obligatoires :

.. code-block:: bash

    php app/console generate:doctrine:entity --non-interaction --entity=AcmeBlogBundle:Post --fields="title:string(100) body:text" --format=xml

Options disponibles
-----------------

* ``--entity``: Le nom de l'entité donné en notation raccourcie, contenant le nom
  du bundle dans lequel l'entité est localisée, ainsi que le nom de l'entité.
  Par exemple, ``AcmeBlogBundle:Post`` :

    .. code-block:: bash

        php app/console generate:doctrine:entity --entity=AcmeBlogBundle:Post

* ``--fields``: La liste des champs à générer dans la classe entité :

    .. code-block:: bash

        php app/console generate:doctrine:entity --fields="title:string(100) body:text"

* ``--format``: (**annotation**) [valeurs: yml, xml, php ou annotation]
  Détermine le format à utiliser pour les fichiers de configuration généré
  comme le routage. Par défaut, la commande utilise le format ``annotation``.
  Choisir le format ``annotation`` implique que le ``SensioFrameworkExtraBundle``
  soit déjà installé :

    .. code-block:: bash

        php app/console generate:doctrine:entity --format=annotation

* ``--with-repository``: Cette option indique de créer ou non la classe
  Doctrine `EntityRepository` associée :

    .. code-block:: bash

        php app/console generate:doctrine:entity --with-repository
