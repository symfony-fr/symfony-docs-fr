.. index::
   single: Forms; Fields; email

Type de champ Email
===================

Le champ ``email`` est un champ texte qui est rendu en utilisant la balise HTML5
``<input type="email" />``.

+-------------+---------------------------------------------------------------------+
| Rendu comme | Champ ``input`` ``email`` (balise texte)                            |
+-------------+---------------------------------------------------------------------+
| Options     | - `max_length`_                                                     |
| héritées    | - `empty_data`_                                                     |
|             | - `required`_                                                       |
|             | - `label`_                                                          |
|             | - `label_attr`_                                                     |
|             | - `data`_                                                           |
|             | - `trim`_                                                           |
|             | - `read_only`_                                                      |
|             | - `disabled`_                                                       |
|             | - `error_bubbling`_                                                 |
|             | - `error_mapping`_                                                  |
|             | - `mapped`_                                                         |
+-------------+---------------------------------------------------------------------+
| Type parent | :doc:`field</reference/forms/types/form>`                           |
+-------------+---------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\EmailType` |
+-------------+---------------------------------------------------------------------+

Options héritées
----------------

Ces options sont héritées du type :doc:`field</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc