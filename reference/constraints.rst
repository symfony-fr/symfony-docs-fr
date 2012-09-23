Contraintes de validation de référence
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
   constraints/Length
   constraints/Url
   constraints/Regex
   constraints/Ip

   constraints/Max
   constraints/Min
   constraints/Range

   constraints/Date
   constraints/DateTime
   constraints/Time

   constraints/Choice
   constraints/Collection
   constraints/Count
   constraints/UniqueEntity
   constraints/Language
   constraints/Locale
   constraints/Country

   constraints/File
   constraints/Image

   constraints/Luhn

   constraints/Callback
   constraints/All
   constraints/UserPassword
   constraints/Valid
   
Le validateur est conçu pour valider les objets selon des *contraintes*.
Dans la vie réelle, une contrainte pourrait être : « Le gâteau ne doit
pas être brûlé ». Dans Symfony2, les contraintes sont similaires : ce sont
des assertions qu'une condition est vérifiée.

Contraintes supportées
----------------------

Les contraintes suivantes sont nativement supportées par Symfony2 :

.. include:: /reference/constraints/map.rst.inc