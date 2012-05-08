.. index::
   single: Bundle; Héritage

Comment surcharger n'importe quelle partie d'un bundle
======================================================

Ce document est une référence succinte sur comment surcharger différentes
partie d'un bundle tiers.

Templates
---------

Pour des informations sur la surcharge de templates, lisez
* :ref:`overriding-bundle-templates`.
* :doc:`/cookbook/bundles/inheritance`

Routage
-------

Le routage n'est jamais importé automatiquement dans Symfony2. Si vous voulez
inclure les routes d'un bundle, alors elles doivent être importées manuellement
à un endroit de votre application (ex ``app/config/routing.yml``).

La manière la plus simple de « surcharger » les routes d'un bundle est de
ne pas les importer du tout. Plutôt que d'importer les routes d'un bundle tiers,
copiez simplement le fichier de routage dans votre application, modifiez le, et
importez le.

Contrôleurs
-----------

En partant du principe que les bundles tiers n'utisent pas de contrôleurs qui
ne soient pas des services (ce qui est presque toujours le cas), vous pouvez
facilement surcharger les contrôleurs grâce à l'héritage de bundle. Pour plus
d'informations, lisez :doc:`/cookbook/bundles/inheritance`.

Services & Configuration
------------------------

En cours...

Entités et mapping
------------------

En cours...

Formulaires
-----------

En cours...

Métadonnées de Validation
-------------------------

En cours...

Traductions
-----------

En cours...