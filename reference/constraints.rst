Contraintes de Validation de Référence
======================================

.. toctree::
   :maxdepth: 1
   :hidden:

   constraints/NotBlank
   constraints/Blank
   constraints/NotNull
   constraints/Null
   constraints/True
   constraints/False
   constraints/Type

   constraints/Email
   constraints/MinLength
   constraints/MaxLength
   constraints/Url
   constraints/Regex
   constraints/Ip

   constraints/Max
   constraints/Min

   constraints/Date
   constraints/DateTime
   constraints/Time

   constraints/Choice
   constraints/Collection
   constraints/UniqueEntity
   constraints/Language
   constraints/Locale
   constraints/Country

   constraints/File
   constraints/Image

   constraints/Callback
   constraints/All
   constraints/UserPassword
   constraints/Valid

Le Validator est conçu pour valider des objets selon des *contraintes*.
Par exemple, une contrainte pourrait être : « Le gâteau ne doit pas être brulé».
Dans Symfony2, les contraintes sont similaires : ce sont des affirmations qui
font qu'une condition est vraie.

Contraintes supportées
----------------------

Les contraintes suivantes sont nativement supportées dans Symfony2 :

.. include:: /reference/constraints/map.rst.inc
