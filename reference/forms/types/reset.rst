.. index::
   single: Forms; Fields; reset

Type de champ reset
================

.. versionadded:: 2.3
    Le type ``reset`` a été ajouté à la version Symfony 2.3

Un bouton qui remets à zéro tous les champs à leurs valeurs d'origine.

+----------------------+---------------------------------------------------------------------+
| Rendu comme          | ``input`` ``reset`` tag                                             |
+----------------------+---------------------------------------------------------------------+
| Options              | - `attr`_                                                           |
| héritées             | - `disabled`_                                                       |
|                      | - `label`_                                                          |
|                      | - `translation_domain`_                                             |
+----------------------+---------------------------------------------------------------------+
| Type parent          | :doc:`button</reference/forms/types/button>`                        |
+----------------------+---------------------------------------------------------------------+
| Classe                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\ResetType` |
+----------------------+---------------------------------------------------------------------+

Inherited options
-----------------

.. include:: /reference/forms/types/options/button_attr.rst.inc

.. include:: /reference/forms/types/options/button_disabled.rst.inc

.. include:: /reference/forms/types/options/button_label.rst.inc

.. include:: /reference/forms/types/options/button_translation_domain.rst.inc
