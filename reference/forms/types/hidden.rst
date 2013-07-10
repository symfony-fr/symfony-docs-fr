.. index::
   single: Forms; Fields; hidden

Type de champ Hidden
====================

Le type hidden représente un champ input hidden.

+-------------+----------------------------------------------------------------------+
| Rendu comme | Champ ``input`` ``hidden``                                           |
+-------------+----------------------------------------------------------------------+
| Options     | - `required`_                                                        |
| surchargées | - `error_bubbling`_                                                  |
+-------------+----------------------------------------------------------------------+
| Options     | - `data`_                                                            |
| héritées    | - `property_path`_                                                   |
|             | - `mapped`_                                                          |
|             | - `error_mapping`_                                                   |
+-------------+----------------------------------------------------------------------+
| Type parent | :doc:`form</reference/forms/types/form>`                             |
+-------------+----------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\HiddenType` |
+-------------+----------------------------------------------------------------------+

Options surchargées
-------------------

required
~~~~~~~~

**default**: ``false``

Les champs cachés ne peuvent pas être obligatoires.

error_bubbling
~~~~~~~~~~~~~~

**default**: ``true``

Transfères les erreurs à la racine du formulaire, sinon, elles ne seraient
pas visibles.

Options héritées
----------------

Ces options héritent du type :doc:`form</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/property_path.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc