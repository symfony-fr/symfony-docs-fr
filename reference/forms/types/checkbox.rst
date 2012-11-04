.. index::
   single: Forms; Fields; checkbox

Type de champ Checkbox
======================

Crée un unique champ de type input checkbox. Cela devrait toujours ête utilisé pour
un champ qui a une valeur booléenne : si la checkbox est cochée, le champ sera
défini à true. Si la checkbox n'est pas cochée, le champ sera défini à false.

+-------------+------------------------------------------------------------------------+
| Rendu comme | Champ ``input`` ``checkbox``                                           |
+-------------+------------------------------------------------------------------------+
| Options     | - `value`_                                                             |
+-------------+------------------------------------------------------------------------+
| Options     | - `required`_                                                          |
| héritées    | - `label`_                                                             |
|             | - `read_only`_                                                         |
|             | - `error_bubbling`_                                                    |
+-------------+------------------------------------------------------------------------+
| Type parent | :doc:`field</reference/forms/types/field>`                             |
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

Ces options héritent du type :doc:`field</reference/forms/types/field>` :

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc