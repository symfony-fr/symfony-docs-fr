.. index::
   single: Forms; Fields; search

Type de champ Search
====================

Cela retourne un champ ``<input type="search" />`` qui est un champ texte 
avec une fonctionnalité spéciale supportée par certains navigateurs.

Apprenez en plus sur le champ input Search sur `DiveIntoHTML5.info`_

+-------------+----------------------------------------------------------------------+
| Rendu comme | Champ ``input search``                                               |
+-------------+----------------------------------------------------------------------+
| Options     | - `max_length`_                                                      |
| héritées    | - `required`_                                                        |
|             | - `label`_                                                           |
|             | - `trim`_                                                            |
|             | - `read_only`_                                                       |
|             | - `disabled`_                                                        |
|             | - `error_bubbling`_                                                  |
|             | - `error_mapping`_                                                   |
|             | - `mapped`_                                                          |
+-------------+----------------------------------------------------------------------+
| Type parent | :doc:`text</reference/forms/types/text>`                             |
+-------------+----------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\SearchType` |
+-------------+----------------------------------------------------------------------+

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

.. _`DiveIntoHTML5.info`: http://diveintohtml5.info/forms.html#type-search