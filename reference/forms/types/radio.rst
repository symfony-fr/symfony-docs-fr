.. index::
   single: Forms; Fields; radio

Type de champ Radio
===================

Crée un simple bouton radio. Il devrait toujours être utilisé pour un champ
dont la valeur est booléenne. : si le bouton radio est sélectionné, le champ sera
défini à true, sinon, la valeur sera définie à false.

Le type ``radio`` n'est généralement pas utilisé directement. Le plus souvent, il 
est utilisé indirectement par d'autres types comme le type :doc:`Choice</reference/forms/types/choice>`.
Si vous voulez avoir un champ Booléen, utilisez :doc:`checkbox</reference/forms/types/checkbox>`.

+-------------+---------------------------------------------------------------------+
| Rendu comme | Champ ``input`` ``radio``                                           | 
+-------------+---------------------------------------------------------------------+
| Options     | - `value`_                                                          |
+-------------+---------------------------------------------------------------------+
| Options     | - `required`_                                                       |
| héritées    | - `label`_                                                          |
|             | - `read_only`_                                                      |
|             | - `disabled`_                                                       |
|             | - `error_bubbling`_                                                 |
|             | - `error_mapping`_                                                  |
|             | - `mapped`_                                                         |
+-------------+---------------------------------------------------------------------+
| Type parent | :doc:`form</reference/forms/types/form>`                            |
+-------------+---------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\RadioType` |
+-------------+---------------------------------------------------------------------+

Options du champ
----------------

value
~~~~~

**type**: ``mixed`` **default**: ``1``

La valeur qui est effectivement utilisée comme valeur pour le radio bouton. Cela
n'affecte pas la valeur qui est définie dans votre objet.

Options héritées
-----------------

Ces options héritent du type :doc:`form</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc