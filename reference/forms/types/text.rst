.. index::
   single: Forms; Fields; text

Type de champ Text
==================

Le champ texte représente le plus basique des champs input texte.

+-------------+--------------------------------------------------------------------+
| Rendu comme | Champ ``input`` ``text``                                           |
+-------------+--------------------------------------------------------------------+
| Options     | - `max_length`_                                                    |
| héritées    | - `required`_                                                      |
|             | - `label`_                                                         |
|             | - `trim`_                                                          |
|             | - `read_only`_                                                     |
|             | - `disabled`_                                                      |
|             | - `error_bubbling`_                                                |
|             | - `error_mapping`_                                                 |
|             | - `mapped`_                                                        |
+-------------+--------------------------------------------------------------------+
| Type parent | :doc:`form</reference/forms/types/form>`                           |
+-------------+--------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TextType` |
+-------------+--------------------------------------------------------------------+


Options héritées
----------------

Ces options héritent du type :doc:`form</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc