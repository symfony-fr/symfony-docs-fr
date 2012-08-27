G�n�rer un squelette pour un nouveau bundle
===========================================

Utilisation
-----------

La commande ``generate:bundle`` g�n�re une nouvelle structure de bundle et l'active
automatiquement dans votre application.

Par d�faut, la commande est ex�cut�e en mode interactif et vous pose des questions
pour d�finir le nom du bundle, son emplacement, son format de configuration et sa
structure par d�faut :

.. code-block:: bash

    php app/console generate:bundle

Pour d�sactiver le mode interactif, utilisez l'option `--no-interaction` mais il
vous faudra alors penser � passer toutes les options obligatoires :

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/Bundle/BlogBundle --no-interaction

Options disponibles
-------------------

* ``--namespace``: L'espace de nom du bundle � cr�er. L'espace de nom devrait commencer
  avec un nom � commercial � comme le nom de votre entreprise, le nom de votre projet
  ou le nom de votre client, suivi par un ou plusieurs sous-espace de nom facultatifs,
  et devrait �tre termin� par le nom du bundle lui-m�me (qui doit avoir Bundle comme
  suffixe) :

  .. code-block:: bash

        php app/console generate:bundle --namespace=Acme/Bundle/BlogBundle

* ``--bundle-name``: Le nom du bundle facultatif. Ce doit �tre une chaine de
  caract�res termin�e par le suffixe ``Bundle`` :

    .. code-block:: bash

        php app/console generate:bundle --bundle-name=AcmeBlogBundle

* ``--dir``: Le r�pertoire dans lequel stocker le bundle. Par convention,
  la commande d�tecte et utilise le r�pertoire ``src/`` de l'application :

    .. code-block:: bash

        php app/console generate:bundle --dir=/var/www/myproject/src

* ``--format``: (**annotation**) [valeurs: yml, xml, php ou annotation]
  D�termine le format � utiliser pour les fichiers de configuration g�n�r�
  comme le routage. Par d�faut, la commande utilise le format ``annotation``.
  Choisir le format ``annotation`` implique que le ``SensioFrameworkExtraBundle``
  soit d�j� install� :

    .. code-block:: bash

        php app/console generate:bundle --format=annotation

* ``--structure``: (**no**) [valeurs: yes|no] G�nerer ou non la structure
  de r�pertoire compl�te, incluant les r�pertoires publics pour la documentation
  et les ressources web ainsi que les dictionnaires de traductions :

    .. code-block:: bash

        php app/console generate:bundle --structure=yes
