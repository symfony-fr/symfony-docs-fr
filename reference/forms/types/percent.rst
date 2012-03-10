.. index::
   single: Forms; Fields; percent

Type de champ Percent
=====================

Le type ``percent`` rend un champ input texte spécialisé dans la gestion de pourcentages.
Si votre pourcentage est stocké comme décimale (ex ``0,95``),
vous pouvez utiliser ce champ directement. Si vous stockez votre données comme nombre
(ex ``95``), vous devriez définir l'option ``type`` à ``integer``.

Ce champ ajoute le signe pourcentage "``%``" après l'input.

+-------------+-----------------------------------------------------------------------+
| Rendu comme | Champ ``input`` ``text``                                              |
+-------------+-----------------------------------------------------------------------+
| Options     | - `type`_                                                             |
|             | - `precision`_                                                        |
+-------------+-----------------------------------------------------------------------+
| Options     | - `required`_                                                         |
| héritées    | - `label`_                                                            |
|             | - `read_only`_                                                        |
|             | - `error_bubbling`_                                                   |
+-------------+-----------------------------------------------------------------------+
| Type parent | :doc:`field</reference/forms/types/field>`                            |
+-------------+-----------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\PercentType` |
+-------------+-----------------------------------------------------------------------+

Options
-------

type
~~~~

**type**: ``string`` **default**: ``fractional``

Cette option contrôle la façon dont vos données sont stockées dans l'objet. Par exemple,
un pourcentage correspondant à « 55% », peut être stocké comme ``0,55`` ou ``55`` dans votre
objet. Les deux « types » gèrent ces deux cas :
    
*   ``fractional``
    Si votre donnée est stockée au format décimal (ex ``0,55``), utilisez ce type.
    La donnée sera multipliée par ``100`` avant d'être affichée à l'utilisateur (ex ``55``).
	La valeur soumise sera divisée par ``100`` lors de la soumission du formulaire pour que
	la valeur soit stockée au format décimal (``0,55``);

*   ``integer``
    Si votre donnée est stockée comme integer (ex 55), utilisez cette
	option. La valeur brute (``55``) est affichée à l'utilisateur et stockée dans votre objet.
	Notez que cela ne fonctionne que pour les valeurs entières.

precision
~~~~~~~~~

**type**: ``integer`` **default**: ``0``

Par défaut, les nombres sont arrondis. Pour autoriser les décimales, utilisez cette option.

Options héritées
----------------

Ces options héritent du type :doc:`field</reference/forms/types/field>` :

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

