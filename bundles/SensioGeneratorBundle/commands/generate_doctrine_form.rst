Générer une nouvelle classe de type de formulaire basée sur une entité Doctrine
===============================================================================

Utilisation
-----------

la commande ``generate:doctrine:form`` génère une classe de type de formulaire
basique en utilisant les métadonnées de mapping d'une classe entité donnée :

.. code-block:: bash

    php app/console generate:doctrine:form AcmeBlogBundle:Post

Arguments obligatoires
----------------------

* ``entity``: Le nom de l'entité donné en notation raccourcie, contenant le nom
  du bundle dans lequel l'entité est localisée, ainsi que le nom de l'entité.
  Par exemple, ``AcmeBlogBundle:Post`` :

    .. code-block:: bash

        php app/console generate:doctrine:form AcmeBlogBundle:Post
