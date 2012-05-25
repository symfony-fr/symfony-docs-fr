.. index::
   single: Forms; Fields; money

Type de champ Money
===================

Rend un champ input texte spécialisé dans la gestion des données monétaires.

Ce type de champ vous permet de spécifier une devise, dont le symbole sera affiché
à côté du champ texte. Il y a plusieurs autres options pour personnaliser la façon
dont les données en entrée et en sortie seront prises en charge.

+-------------+---------------------------------------------------------------------+
| Rendu comme | Champ ``input`` ``text``                                            |
+-------------+---------------------------------------------------------------------+
| Options     | - `currency`_                                                       |
|             | - `divisor`_                                                        |
|             | - `precision`_                                                      |
|             | - `grouping`_                                                       |
+-------------+---------------------------------------------------------------------+
| Options     | - `required`_                                                       |
| héritées    | - `label`_                                                          |
|             | - `read_only`_                                                      |
|             | - `error_bubbling`_                                                 |
|             | - `invalid_message`_                                                |
|             | - `invalid_message_parameters`_                                     |
+-------------+---------------------------------------------------------------------+
| Type parent | :doc:`field</reference/forms/types/field>`                          |
+-------------+---------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\MoneyType` |
+-------------+---------------------------------------------------------------------+

Options du champ
----------------

currency
~~~~~~~~

**type**: ``string`` **default**: ``EUR``

Définit la devise dans laquelle la somme est spécifiée. Cela détermine quel symbole
monétaire sera affiché dans le champ texte. Selon la devise choisie, le symbole
s'affichera avant ou après la donnée dans le champ texte.

Cette option peut aussi être définie à false pour cacher le symbole monétaire.

divisor
~~~~~~~

**type**: ``integer`` **default**: ``1``

Si, pour une raison quelconque, vous avez besoin de diviser votre valeur de départ par un
nombre avant de le rendre à l'utilisateur, vous pouvez utiliser l'option ``divisor``.
Par exemple::

    $builder->add('price', 'money', array(
        'divisor' => 100,
    ));

Dans ce cas, si le champ ``price`` est défini à ``9900``, alors c'est en fait la valeur
``99`` qui sera affichée à l'utilisateur. Lorsque l'utilisateur soumettra la valeur
``99``, elle sera automatiquement multipliée par ``100`` et ``9900`` sera la valeur
finalement stockée dans votre projet.

precision
~~~~~~~~~

**type**: ``integer`` **default**: ``2``

Si pour une raison quelconque vous avez besoin d'une autre précision que 2 décimales,
vous pouvez modifier cette option. Vous n'aurez probablement pas besoin de le faire
à moins, par exemple, que vous ne vouliez arrondir au dollar le plus proche (dans
ce cas, définissez la précision à ``0``).

.. include:: /reference/forms/types/options/grouping.rst.inc

Options héritées
----------------

Ces options héritent du type :doc:`field</reference/forms/types/field>` :

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc