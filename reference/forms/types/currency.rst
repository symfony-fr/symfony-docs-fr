.. index::
    single: Forms; Fields; currency

Type de champ currency
======================

Le type  ``currency`` est un sous-ensemble du
:doc:`type choice</reference/forms/types/choice>` qui permet à l'utilisateur
de faire un choix parmis une liste complète de devises au format `3-lettres ISO 4217`_.

Contrairement au type ``choice``, vous n'avez pas besoin de spécifier les options
``choices`` ou ``choice_list`` puisque le type retourne automatiquement un large
choix de devises. Vous *pouvez* spécifier ces options manuellement, mais alors
vous devriez plutôt utiliser directement le type ``choice``.

+-------------+------------------------------------------------------------------------+
| Rendu comme | peut être différentes balises (voir :ref:`forms-reference-choice-tags`)|
+-------------+------------------------------------------------------------------------+
| Options     | - `choices`_                                                           |
| surchargées |                                                                        |
+-------------+------------------------------------------------------------------------+
| Options     | - `multiple`_                                                          |
| héritées    | - `expanded`_                                                          |
|             | - `preferred_choices`_                                                 |
|             | - `empty_value`_                                                       |
|             | - `error_bubbling`_                                                    |
|             | - `required`_                                                          |
|             | - `label`_                                                             |
|             | - `read_only`_                                                         |
|             | - `disabled`_                                                          |
|             | - `mapped`_                                                            |
+-------------+------------------------------------------------------------------------+
| Type parent | :doc:`choice </reference/forms/types/choice>`                          |
+-------------+------------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CurrencyType` |
+-------------+------------------------------------------------------------------------+

Options surchargées
-------------------

choices
~~~~~~~

**default**: ``Symfony\Component\Intl\Intl::getCurrencyBundle()->getCurrencyNames()``

La valeur par défaut de l'option ``choice`` est la liste des devises

Options héritées
----------------

Ces options héritent du type :doc:`choice</reference/forms/types/choice>` :

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

Ces options héritent du type :doc:`field</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. _`3-lettres ISO 4217`: http://fr.wikipedia.org/wiki/ISO_4217