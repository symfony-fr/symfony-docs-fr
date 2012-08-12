.. index::
   single: Doctrine; Common extensions

Extensions Doctrine: Timestampable, Sluggable, Translatable, etc.
=================================================================

Doctrine2 est très flexible, et la communauté a déjà créé une série d'extensions
Doctrine très pratiques afin de vous aider avec les tâches usuelles liées aux
entités.

Une librairie en particulier - la librairie `DoctrineExtensions`_ - fournit
l'intégration de fonctionnalités pour les comportements (Behaviors) `Sluggable`_,
`Translatable`_, `Timestampable`_, `Loggable`_, `Tree`_ et `Sortable`_

L'utilisation de chacune de ces extensions est expliquée dans son dépôt.

Toutefois, pour installer/activer chaque extension, vous devez enregistrer
et activer un :doc:`Ecouteur d'évènement (Event Listener)</cookbook/doctrine/event_listeners_subscribers>`.
Pour faire cela, vous avez deux possibilités :

#. Utiliser le bundle `StofDoctrineExtensionsBundle`_, qui intègre la librairie ci-dessus.

#. Implémenter ces services directement en suivant la documentation pour l'intégration dans
   Symfony2 : `Installer les extensions Gedmo Doctrine2 dans Symfony2`_

.. _`DoctrineExtensions`: https://github.com/l3pp4rd/DoctrineExtensions
.. _`StofDoctrineExtensionsBundle`: https://github.com/stof/StofDoctrineExtensionsBundle
.. _`Sluggable`: https://github.com/l3pp4rd/DoctrineExtensions/blob/master/doc/sluggable.md
.. _`Translatable`: https://github.com/l3pp4rd/DoctrineExtensions/blob/master/doc/translatable.md
.. _`Timestampable`: https://github.com/l3pp4rd/DoctrineExtensions/blob/master/doc/timestampable.md
.. _`Loggable`: https://github.com/l3pp4rd/DoctrineExtensions/blob/master/doc/loggable.md
.. _`Tree`: https://github.com/l3pp4rd/DoctrineExtensions/blob/master/doc/tree.md
.. _`Sortable`: https://github.com/l3pp4rd/DoctrineExtensions/blob/master/doc/sortable.md
.. _`Installer les extensions Gedmo Doctrine2 dans Symfony2`: https://github.com/l3pp4rd/DoctrineExtensions/blob/master/doc/symfony2.md