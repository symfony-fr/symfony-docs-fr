G�n�rer une nouvelle entit� Doctrine
====================================

Utilisation
-----------

La commande ``generate:doctrine:entity`` g�n�re une nouvelle entit� Doctrine
incluant la d�finition du mapping ainsi que les propri�t�, getters et setters
de la classe.

Par d�faut, la commande est ex�cut�e en mode interactif et vous pose des questions
pour d�finir le nom de l'entit�, son emplacement, son format de configuration et sa
structure par d�faut :

.. code-block:: bash

    php app/console generate:doctrine:entity

	
Pour d�sactiver le mode interactif, utilisez l'option `--no-interaction` mais il
vous faudra alors penser � passer toutes les options obligatoires :

.. code-block:: bash

    php app/console generate:doctrine:entity --non-interaction --entity=AcmeBlogBundle:Post --fields="title:string(100) body:text" --format=xml

Options disponibles
-----------------

* ``--entity``: Le nom de l'entit� donn� en notation raccourcie, contenant le nom
  du bundle dans lequel l'entit� est localis�e, ainsi que le nom de l'entit�.
  Par exemple, ``AcmeBlogBundle:Post`` :

    .. code-block:: bash

        php app/console generate:doctrine:entity --entity=AcmeBlogBundle:Post

* ``--fields``: La liste des champs � g�n�rer dans la classe entit� :

    .. code-block:: bash

        php app/console generate:doctrine:entity --fields="title:string(100) body:text"

* ``--format``: (**annotation**) [valeurs: yml, xml, php ou annotation]
  D�termine le format � utiliser pour les fichiers de configuration g�n�r�
  comme le routage. Par d�faut, la commande utilise le format ``annotation``.
  Choisir le format ``annotation`` implique que le ``SensioFrameworkExtraBundle``
  soit d�j� install� :

    .. code-block:: bash

        php app/console generate:doctrine:entity --format=annotation

* ``--with-repository``: Cette option indique de cr�er ou non la classe
  Doctrine `EntityRepository` associ�e :

    .. code-block:: bash

        php app/console generate:doctrine:entity --with-repository
