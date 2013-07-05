.. index::
   single: Forms; Fields; password

Type de champ Password
======================

Le champ ``password`` rend un input texte de type password.

+-------------+------------------------------------------------------------------------+
| Rendu comme | Champ ``input`` ``password``                                           |
+-------------+------------------------------------------------------------------------+
| Options     | - `always_empty`_                                                      |
+-------------+------------------------------------------------------------------------+
| Options     | - `max_length`_                                                        |
| héritées    | - `required`_                                                          |
|             | - `label`_                                                             |
|             | - `trim`_                                                              |
|             | - `read_only`_                                                         |
|             | - `disabled`_                                                          |
|             | - `error_bubbling`_                                                    |
|             | - `error_mapping`_                                                     |
|             | - `mapped`_                                                            |
+-------------+------------------------------------------------------------------------+
| Type parent | :doc:`text</reference/forms/types/text>`                               |
+-------------+------------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\PasswordType` |
+-------------+------------------------------------------------------------------------+

Options du champ
----------------

always_empty
~~~~~~~~~~~~

**type**: ``Boolean`` **default**: ``true``

Si cette option est définie à true, le champ sera *toujours* rendu vide, même si
le champ correspondant a une valeur. Si elle est définie à false, alors le champ
password sera rendu avec l'attribut ``value`` correctement rempli avec la vraie valeur.

Plus simplement, si pour une raison quelconque vous voulez afficher le champ password
*avec* sa valeur déjà préremplie, définissez cette option à false.

Options héritées
----------------

Ces options sont héritées du type :doc:`form</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc