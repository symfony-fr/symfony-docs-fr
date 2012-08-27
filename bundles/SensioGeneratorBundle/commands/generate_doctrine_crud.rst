G�n�rer un contr�leur CRUD bas� sur une entit� Doctrine
=======================================================

Utilisation
-----------

la commande ``generate:doctrine:crud`` g�n�re un contr�leur basique pour une
entit� donn�e, localis� dans un bundle donn�. Ce contr�leur vous permet
d'effectuer les cinq op�rations de base sur un mod�le.

* Afficher les enregistrements,
* Afficher un seul enregistrement en se basant sur sa cl� primaire,
* Cr�er un nouvel enregistrement,
* Modifier un enregistrement existant,
* Supprimer un enregistrement existant.

Par d�faut, la commande est ex�cut�e en mode interactif et vous pose des questions
pour d�finir le nom de l'entit�, le pr�fixe de la route et s'il faut ou non g�n�rer
les actions :

.. code-block:: bash

    php app/console generate:doctrine:crud

Pour d�sactiver le mode interactif, utilisez l'option `--no-interaction` mais il
vous faudra alors penser � passer toutes les options obligatoires :

.. code-block:: bash

    php app/console generate:doctrine:crud --entity=AcmeBlogBundle:Post --format=annotation --with-write --no-interaction

Options disponibles
-------------------

* ``--entity``: Le nom de l'entit� donn� en notation raccourcie, contenant le nom
  du bundle dans lequel l'entit� est localis�e, ainsi que le nom de l'entit�.
  Par exemple, ``AcmeBlogBundle:Post`` :

    .. code-block:: bash

        php app/console generate:doctrine:crud --entity=AcmeBlogBundle:Post

* ``--route-prefix``: Le pr�fixe � utiliser pour chaque route associ�e � une
  action :

    .. code-block:: bash

        php app/console generate:doctrine:crud --route-prefix=acme_post

* ``--with-write``: (**no**) [valeurs: yes|no] Sp�cifie s'il faut g�n�rer ou non
   les actions `new`, `create`, `edit`, `update` et `delete` :

    .. code-block:: bash

        php app/console generate:doctrine:crud --with-write

* ``--format``: (**annotation**) [valeurs: yml, xml, php ou annotation]
  D�termine le format � utiliser pour les fichiers de configuration g�n�r�
  comme le routage. Par d�faut, la commande utilise le format ``annotation``.
  Choisir le format ``annotation`` implique que le ``SensioFrameworkExtraBundle``
  soit d�j� install� :

    .. code-block:: bash

        php app/console generate:doctrine:crud --format=annotation
