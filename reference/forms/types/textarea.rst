.. index::
   single: Forms; Fields; textarea

Type de champ Textarea
======================

Retourne un élément HTML ``textarea``. 

+-------------+------------------------------------------------------------------------+
| Rendu comme | Balise ``textarea``                                                    |
+-------------+------------------------------------------------------------------------+
| Options     | - `max_length`_                                                        |
| héritées    | - `required`_                                                          |
|             | - `label`_                                                             |
|             | - `trim`_                                                              |
|             | - `read_only`_                                                         |
|             | - `error_bubbling`_                                                    |
+-------------+------------------------------------------------------------------------+
| Type parent | :doc:`field</reference/forms/types/field>`                             |
+-------------+------------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TextareaType` |
+-------------+------------------------------------------------------------------------+

Options héritées
----------------

Ces options héritent du type :doc:`field</reference/forms/types/field>` :

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

