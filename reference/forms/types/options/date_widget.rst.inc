widget
~~~~~~

**type**: ``string`` **default**: ``choice``

Désigne la manière dont le champ doit être affiché. Cette option accepte les
cas suivants :

* ``choice``: Affiche trois listes déroulantes. L'ordre des listes est défini
  par l'option `format`_.

* ``text``: Affiche trois champs textes (mois, jour, année).

* ``single_text``: Affiche un unique champ texte(text en version 2.0 ). La saisie
de l'utilisateur sera validé en se basant sur l'option `format`_.

.. include:: /reference/forms/types/options/_date_limitation.rst.inc
