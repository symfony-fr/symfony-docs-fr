.. index::
   single: Forms; Fields; birthday

Type de champ Birthday
======================

Un champ :doc:`date</reference/forms/types/date>` qui est spécialisé dans la gestion
des dates de naissance.

Peut être rendu comme un champ texte unique, trois champs textes (mois, jour et année),
ou trois listes déroulantesS.

Ce type est très similaire au type :doc:`date</reference/forms/types/date>`, mais
avec des valeurs par défaut de l'option `years`_ plus appropriées. L'option `years`_
contient par défaut les 120 années précédant l'année courante.

+----------------------+------------------------------------------------------------------------------------------------------------------------+
| Type de données      | peut être ``DateTime``, ``string``, ``timestamp``, ou ``array`` (voir :ref:`option input <form-reference-date-input>`) |
+----------------------+------------------------------------------------------------------------------------------------------------------------+
| Rendu comme          | soit trois select, soit 1 ou 3 champs texte, basé sur l'option `widget`_                                               |
+----------------------+------------------------------------------------------------------------------------------------------------------------+
| Options              | - `years`_                                                                                                             |
+----------------------+------------------------------------------------------------------------------------------------------------------------+
| Options              | - `widget`_                                                                                                            |
| héritées             | - `input`_                                                                                                             |
|                      | - `months`_                                                                                                            |
|                      | - `days`_                                                                                                              |
|                      | - `format`_                                                                                                            |
|                      | - `pattern`_                                                                                                           |
|                      | - `data_timezone`_                                                                                                     |
|                      | - `user_timezone`_                                                                                                     |
|                      | - `invalid_message`_                                                                                                   |
|                      | - `invalid_message_parameters`_                                                                                        |
+----------------------+------------------------------------------------------------------------------------------------------------------------+
| Type parent          | :doc:`date</reference/forms/types/date>`                                                                               |
+----------------------+------------------------------------------------------------------------------------------------------------------------+
| Classe               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\BirthdayType`                                                 |
+----------------------+------------------------------------------------------------------------------------------------------------------------+

Options du champ
----------------

years
~~~~~

**type**: ``array`` **default**: 120 années précédant l'année courante

Liste des années disponibles pour le type de champ 'year'. Cette option n'est 
utile que si l'option ``widget`` est définie à ``choice``.

Options héritées
----------------

Ces options héritent du type :doc:`date</reference/forms/types/date>` :

.. include:: /reference/forms/types/options/date_widget.rst.inc
    
.. include:: /reference/forms/types/options/date_input.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/date_format.rst.inc
    
.. include:: /reference/forms/types/options/date_pattern.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc

Ces options héritent du type :doc:`date</reference/forms/types/field>` :

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc
