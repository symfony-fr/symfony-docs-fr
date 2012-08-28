Générer un squelette pour un nouveau bundle
===========================================

Utilisation
-----------

La commande ``generate:bundle`` génère une nouvelle structure de bundle et l'active
automatiquement dans votre application.

Par défaut, la commande est exécutée en mode interactif et vous pose des questions
pour définir le nom du bundle, son emplacement, son format de configuration et sa
structure par défaut :

.. code-block:: bash

    php app/console generate:bundle

Pour désactiver le mode interactif, utilisez l'option `--no-interaction` mais il
vous faudra alors penser à passer toutes les options obligatoires :

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/Bundle/BlogBundle --no-interaction

Options disponibles
-------------------

* ``--namespace``: L'espace de nom du bundle à créer. L'espace de nom devrait commencer
  avec un nom « commercial » comme le nom de votre entreprise, le nom de votre projet
  ou le nom de votre client, suivi par un ou plusieurs sous-espace(s) de nom facultatifs,
  et devrait être terminé par le nom du bundle lui-même (qui doit avoir Bundle comme
  suffixe) :

  .. code-block:: bash

        php app/console generate:bundle --namespace=Acme/Bundle/BlogBundle

* ``--bundle-name``: Le nom du bundle facultatif. Ce doit être une chaîne de
  caractères terminée par le suffixe ``Bundle`` :

    .. code-block:: bash

        php app/console generate:bundle --bundle-name=AcmeBlogBundle

* ``--dir``: Le répertoire dans lequel stocker le bundle. Par convention,
  la commande détecte et utilise le répertoire ``src/`` de l'application :

    .. code-block:: bash

        php app/console generate:bundle --dir=/var/www/myproject/src

* ``--format``: (**annotation**) [valeurs: yml, xml, php ou annotation]
  Détermine le format à utiliser pour les fichiers de configuration générés
  comme le routage. Par défaut, la commande utilise le format ``annotation``.
  Choisir le format ``annotation`` implique que le ``SensioFrameworkExtraBundle``
  soit déjà installé :

    .. code-block:: bash

        php app/console generate:bundle --format=annotation

* ``--structure``: (**no**) [valeurs: yes|no] Spécifie s'il faut génerer ou
  non la structure de répertoire complète, incluant les répertoires publics
  pour la documentation et les ressources web ainsi que les dictionnaires de
  traductions :

    .. code-block:: bash

        php app/console generate:bundle --structure=yes
