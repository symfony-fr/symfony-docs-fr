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
|             | - `error_bubbling`_                                                  |
+-------------+----------------------------------------------------------------------+
| Type parent | :doc:`field</reference/forms/types/field>`                           |
+-------------+----------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\NumberType` |
+-------------+----------------------------------------------------------------------+

Options du champ
----------------

precision
~~~~~~~~~

**type**: ``integer`` **default**: Locale-specific (usually around ``3``)

Cette option spécifie combien de décimales seront autorisées après que le champ
aura arrondi la valeur soumise (via ``rounding_mode``). Par exemple, si ``precision``
est définie à ``2``, une valeur soumise de ``20.123`` sera arrondie, par exemple,
``20.12`` (cela dépendra de votre ``rounding_mode``).

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

Ces options héritent du type :doc:`field</reference/forms/types/field>` :

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
