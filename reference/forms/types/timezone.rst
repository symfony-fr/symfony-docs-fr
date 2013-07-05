.. index::
   single: Forms; Fields; timezone

Type de champ Timezone
======================

Le type ``timezone`` est un sous-ensemble du type ``ChoiceType`` qui permet à l'utilisateur
de choisir parmi les fuseaux horaires possibles.

La « valeur » de chaque fuseau horaire est le nom complet du fuseau, comme ``America/Chicago``
ou ``Europe/Istanbul``.

Contrairement au type ``choice``, vous n'avez pas besoin de spécifier les options ``choices`` ou
``choice_list`` puisque le type de champs utilise automatiquement la liste des fuseaux.
Vous *pouvez* spécifier l'une ou l'autre de ces options manuellement, mais vous devriez
alors utiliser directement le type ``choice``.

+-------------+------------------------------------------------------------------------+
| Rendu comme | peut être différentes balises (voir :ref:`forms-reference-choice-tags`)|
+-------------+------------------------------------------------------------------------+
| Options     | - `choice_list`_                                                       |
| surchargées |                                                                        |
+-------------+------------------------------------------------------------------------+
| Options     | - `multiple`_                                                          |
| héritées    | - `expanded`_                                                          |
|             | - `preferred_choices`_                                                 |
|             | - `empty_value`_                                                       |
|             | - `required`_                                                          |
|             | - `label`_                                                             |
|             | - `read_only`_                                                         |
|             | - `disabled`_                                                          |
|             | - `error_bubbling`_                                                    |
|             | - `error_mapping`_                                                     |
|             | - `mapped`_                                                            |
+-------------+------------------------------------------------------------------------+
| Type parent | :doc:`choice</reference/forms/types/choice>`                           |
+-------------+------------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TimezoneType` |
+-------------+------------------------------------------------------------------------+

Options surchargées
-------------------

choice_list
~~~~~~~~~~~

**default**: :class:`Symfony\\Component\\Form\\Extension\\Core\\ChoiceList\\TimezoneChoiceList`

L'option ``choice_list`` du type Timezone contient par défaut la liste des
fuseaux renvoyés par la méthode :phpmethod:`DateTimeZone::listIdentifiers`,
triés par continent.

Options héritées
----------------

Ces options héritent du type :doc:`choice</reference/forms/types/choice>` :

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

Ces options héritent du type :doc:`form</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc