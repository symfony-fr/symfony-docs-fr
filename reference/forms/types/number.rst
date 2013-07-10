.. index::
   single: Forms; Fields; number

Type de champ Number
====================

Rend un champ input texte spécialisé dans la gestion des nombres. Ce type propose
différentes options pour gérer la précision, les arrondis, et les regroupements
que vous pouvez utiliser pour vos nombres.

+-------------+----------------------------------------------------------------------+
| Rendu comme | Champ ``input`` ``text``                                             |
+-------------+----------------------------------------------------------------------+
| Options     | - `rounding_mode`_                                                   |
|             | - `precision`_                                                       |
|             | - `grouping`_                                                        |
+-------------+----------------------------------------------------------------------+
| Options     | - `required`_                                                        |
| héritées    | - `label`_                                                           |
|             | - `read_only`_                                                       |
|             | - `disabled`_                                                        |
|             | - `error_bubbling`_                                                  |
|             | - `error_mapping`_                                                   |
|             | - `invalid_message`_                                                 |
|             | - `invalid_message_parameters`_                                      |
|             | - `mapped`_                                                          |
+-------------+----------------------------------------------------------------------+
| Type parent | :doc:`form</reference/forms/types/form>`                             |
+-------------+----------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\NumberType` |
+-------------+----------------------------------------------------------------------+

Options du champ
----------------

.. include:: /reference/forms/types/options/precision.rst.inc

rounding_mode
~~~~~~~~~~~~~

**type**: ``integer`` **default**: ``IntegerToLocalizedStringTransformer::ROUND_HALFUP``

Si le nombre soumis a besoin d'être arrondi (et en se basant sur l'option ``precision``),
vous avez plusieurs options configurables pour gérer cet arrondi. Chaque option est
une constante de la classe :class:`Symfony\\Component\\Form\\Extension\\Core\\DataTransformer\\IntegerToLocalizedStringTransformer`:
    
*   ``IntegerToLocalizedStringTransformer::ROUND_DOWN`` Mode pour arrondir jusqu'à
    zéro.    

*   ``IntegerToLocalizedStringTransformer::ROUND_FLOOR`` Mode pour arrondir jusqu'à
    l'infini négatif.

*   ``IntegerToLocalizedStringTransformer::ROUND_UP`` Mode pour arrondir en partant
    de zéro.

*   ``IntegerToLocalizedStringTransformer::ROUND_CEILING`` Mode pour arrondir jusqu'à
    l'infini positif.

*   ``IntegerToLocalizedStringTransformer::ROUND_HALFDOWN`` Mode pour arrondir
    au « voisin le plus proche ». Si les deux voisins sont équidistants, alors c'est
    arrondi au voisin inférieur.

*   ``IntegerToLocalizedStringTransformer::ROUND_HALFEVEN`` Mode pour arrondir
    au « voisin le plus proche ». Si les deux voisins sont équidistants, alors c'est
    arrondi au voisin pair.

*   ``IntegerToLocalizedStringTransformer::ROUND_HALFUP`` Mode pour arrondir
    au « voisin le plus proche ». Si les deux voisins sont équidistants, alors c'est
    arrondi au voisin supérieur.

.. include:: /reference/forms/types/options/grouping.rst.inc

Option héritées
---------------

Ces options héritent du type :doc:`form</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc