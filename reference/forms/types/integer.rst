.. index::
   single: Forms; Fields; integer

Type de champ Integer
=====================

Affiche un champ input « number ». De façon basique, c'est un champ texte qui sait bien 
gérer les données entières d'un formulaire. Le champ input ``number`` ressemble à
un champ texte, excepté que, si le navigateur supporte l'HTML5, il aura des fonctionnalités
supplémentaires.

Ce champ a différentes options permettant de définir comment gérer les valeurs qui
ne sont pas des entiers. Par défaut, toutes les valeurs non entières (ex: 6,78)
seront arrondies à l'entier inférieur (ex: 6).

+-------------+-----------------------------------------------------------------------+
| Rendu comme | Champ ``input`` ``text``                                              |
+-------------+-----------------------------------------------------------------------+
| Options     | - `rounding_mode`_                                                    |
|             | - `precision`_                                                        |
|             | - `grouping`_                                                         |
+-------------+-----------------------------------------------------------------------+
| Options     | - `empty_data`_                                                       |
| héritées    | - `required`_                                                         |
|             | - `label`_                                                            |
|             | - `label_attr`_                                                       |
|             | - `data`_                                                             |
|             | - `read_only`_                                                        |
|             | - `disabled`_                                                         |
|             | - `error_bubbling`_                                                   |
|             | - `error_mapping`_                                                    |
|             | - `invalid_message`_                                                  |
|             | - `invalid_message_parameters`_                                       |
|             | - `mapped`_                                                           |
+-------------+-----------------------------------------------------------------------+
| Type parent | :doc:`form</reference/forms/types/form>`                              |
+-------------+-----------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\IntegerType` |
+-------------+-----------------------------------------------------------------------+

Options du champ
----------------

rounding_mode
~~~~~~~~~~~~~

**type**: ``integer`` **default**: ``IntegerToLocalizedStringTransformer::ROUND_DOWN``

Par défaut, si l'utilisateur entre un nombre qui n'est pas entier, il sera arrondi
à l'entier inférieur. Il y a plusieurs autres méthodes pour arrondir, et chacune
est une constante de la classe :class:`Symfony\\Component\\Form\\Extension\\Core\\DataTransformer\\IntegerToLocalizedStringTransformer`:

*   ``IntegerToLocalizedStringTransformer::ROUND_DOWN`` Mode pour arrondir jusqu'à zéro.

*   ``IntegerToLocalizedStringTransformer::ROUND_FLOOR`` Mode pour arrondir jusqu'à
    l'infini négatif.

*   ``IntegerToLocalizedStringTransformer::ROUND_UP`` Mode pour arrondir en partant
    de zéro.

*   ``IntegerToLocalizedStringTransformer::ROUND_CEILING`` Mode pour arrondir jusqu'à
    l'infini positif.

.. include:: /reference/forms/types/options/grouping.rst.inc

Options héritées
----------------

Ces options héritent du type :doc:`form</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/empty_data.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc