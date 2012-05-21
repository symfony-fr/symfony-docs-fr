.. index::
   single: Forms; Fields; country

Type de champ Country
=====================

Le type ``country`` est un sous-ensemble de ``ChoiceType`` qui affiche la liste
des pays du monde. Et en bonus, les noms des pays sont affichés dans la langue de
l'utilisateur.

La « valeur » de chaque pays est son code en 2 lettres.

.. note::

   La locale de l'utilisateur est retournée par la méthode `Locale::getDefault()`_

Contrairement au type ``choice``, vous n'avez pas besoin de spécifier les options
``choices`` ou ``choice_list`` puisque le type retourne automatiquement la liste
de tous les pays du monde. Vous *pouvez* spécifier ces options manuellement, mais alors
vous devriez plutôt utiliser directement le type ``choice``.

+-------------+------------------------------------------------------------------------+
| Rendu comme | peut être différentes balises (voir :ref:`forms-reference-choice-tags`)|
+-------------+------------------------------------------------------------------------+
| Options     | - `multiple`_                                                          |
| héritées    | - `expanded`_                                                          |
|             | - `preferred_choices`_                                                 |
|             | - `empty_value`_                                                       |
|             | - `error_bubbling`_                                                    |
|             | - `required`_                                                          |
|             | - `label`_                                                             |
|             | - `read_only`_                                                         |
+-------------+------------------------------------------------------------------------+
| Type parent | :doc:`choice</reference/forms/types/choice>`                           |
+-------------+------------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CountryType`  |
+-------------+------------------------------------------------------------------------+

Options héritées
----------------

Ces options héritent du type :doc:`choice</reference/forms/types/choice>` :

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

Ces options héritent du type :doc:`field</reference/forms/types/field>` :

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. _`Locale::getDefault()`: http://php.net/manual/en/locale.getdefault.php