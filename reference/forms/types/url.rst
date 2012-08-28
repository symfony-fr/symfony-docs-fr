.. index::
   single: Forms; Fields; url

Type de champ Url
=================

Le champ ``url`` est un champ texte qui préfixe la valeur soumise par un protocole
donné (ex ``http://``) si la valeur n'a pas déjà un protocole.

+-------------+-------------------------------------------------------------------+
| Rendu comme | Champ ``input url``                                               |
+-------------+-------------------------------------------------------------------+
| Options     | - `default_protocol`_                                             |
+-------------+-------------------------------------------------------------------+
| Options     | - `max_length`_                                                   |
| héritées    | - `required`_                                                     |
|             | - `label`_                                                        |
|             | - `trim`_                                                         |
|             | - `read_only`_                                                    |
|             | - `error_bubbling`_                                               |
+-------------+-------------------------------------------------------------------+
| Type parent | :doc:`text</reference/forms/types/text>`                          |
+-------------+-------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\UrlType` |
+-------------+-------------------------------------------------------------------+

Options du champ
----------------

default_protocol
~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``http``

Si une valeur soumise ne commence pas un protocole (ex ``http://``,
``ftp://``, etc), ce protocole sera ajouté au début de la chaîne de caractères
lorsque les données seront associées au formulaire.

Options héritées
----------------

Ces options héritent du type :doc:`field</reference/forms/types/field>` :

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc