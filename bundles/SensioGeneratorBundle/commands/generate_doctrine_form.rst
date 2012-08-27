G�n�rer une nouvelle classe de type de formulaire bas�e sur une entit� Doctrine
===============================================================================

Utilisation
-----------

la commande ``generate:doctrine:form`` g�n�re une classe de type de formulaire
basique en utilisant les m�tadonn�es de mapping d'une classe entit� donn�e :

.. code-block:: bash

    php app/console generate:doctrine:form AcmeBlogBundle:Post

Arguments obligatoires
----------------------

* ``entity``: Le nom de l'entit� donn� en notation raccourcie, contenant le nom
  du bundle dans lequel l'entit� est localis�e, ainsi que le nom de l'entit�.
  Par exemple, ``AcmeBlogBundle:Post`` :

    .. code-block:: bash

        php app/console generate:doctrine:form AcmeBlogBundle:Post
