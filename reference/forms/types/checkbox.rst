.. index::
   single: Forms; Fields; checkbox

Type de champ Checkbox
======================

Crée un unique champ de type input checkbox. Cela devrait toujours être utilisé pour
un champ qui a une valeur booléenne : si la case est cochée, le champ sera
défini à true. Si la case n'est pas cochée, le champ sera défini à false.

+-------------+------------------------------------------------------------------------+
| Rendu comme | Champ ``input`` ``checkbox``                                           |
+-------------+------------------------------------------------------------------------+
| Options     | - `value`_                                                             |
+-------------+------------------------------------------------------------------------+
| Options     | - `data`_                                                              |
| héritées    | - `required`_                                                          |
|             | - `label`_                                                             |
|             | - `label_attr`_                                                        |
|             | - `read_only`_                                                         |
|             | - `disabled`_                                                          |
|             | - `error_bubbling`_                                                    |
|             | - `error_mapping`_                                                     |
|             | - `mapped`_                                                            |
+-------------+------------------------------------------------------------------------+
| Type parent | :doc:`form</reference/forms/types/form>`                               |
+-------------+------------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CheckboxType` |
+-------------+------------------------------------------------------------------------+

Exemple d'utilisation
---------------------

.. code-block:: php

    $builder->add('public', 'checkbox', array(
        'label'     => 'Afficher publiquement ?',
        'required'  => false,
    ));

Options du champ
----------------

value
~~~~~

**type**: ``mixed`` **default**: ``1``

La valeur qui est effectivement utilisée comme valeur de la checkbox. Cela n'affecte
pas la valeur qui est définie sur votre objet.

Options héritées
----------------

Ces options héritent du type :doc:`field</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc
