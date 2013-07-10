.. index::
   single: Forms; Fields; locale

Type de champ Locale
====================

Le type ``locale`` est un sous-ensemble de ``ChoiceType`` qui permet à l'utilisateur
de choisir une locale (langue + pays) dans une liste déroulante. En bonus, les noms des
locales sont affichés dans la langue de l'utilisateur.


La « valeur » de chaque locale est soit le code *langue* ISO639-1 en deux lettres
(ex ``fr``), soit le code langue suivi d'un underscore  (``_``), puis le code
*pays* ISO3166 (ex ``fr_FR`` pour Français/France).

.. note::

   La locale de votre utilisateur est devinée en utilisant :phpmethod:`Locale::getDefault`.

Contrairement au type ``choice``, vous n'avez pas besoin de spécifier les options
``choices`` ou ``choice_list`` puisque ce type de champ utilise automatiquement
la liste des locales. Vous *pouvez* spécifier l'une ou l'autre de ces options manuellement,
mais alors vous devriez plutôt utiliser directement le type ``choice``.

+-------------+------------------------------------------------------------------------+
| Rendu comme | Peut être différentes balises (voir :ref:`forms-reference-choice-tags`)|
+-------------+------------------------------------------------------------------------+
| Options     | - `choices`_                                                           |
| surchargées |                                                                        |
+-------------+------------------------------------------------------------------------+
| Options     | - `multiple`_                                                          |
| héritées    | - `expanded`_                                                          |
|             | - `preferred_choices`_                                                 |
|             | - `empty_value`_                                                       |
|             | - `error_bubbling`_                                                    |
|             | - `error_mapping`_                                                     |
|             | - `required`_                                                          |
|             | - `label`_                                                             |
|             | - `read_only`_                                                         |
|             | - `disabled`_                                                          |
|             | - `mapped`_                                                            |
+-------------+------------------------------------------------------------------------+
| Type parent | :doc:`choice</reference/forms/types/choice>`                           |
+-------------+------------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\LanguageType` |
+-------------+------------------------------------------------------------------------+

Options surchargées
------------------

choices
~~~~~~~

**default**: ``Symfony\Component\Intl\Intl::getLocaleBundle()->getLocaleNames()``

L'option ``choices`` est définie par défaut comme la liste de toutes les locales.
Elle utilise la locale par défaut pour spécifier la langue.

Options héritées
----------------

Ces options sont héritées du type :doc:`choice</reference/forms/types/choice>` :

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

Ces options sont héritées du type :doc:`form</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc