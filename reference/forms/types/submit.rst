.. index::
   single: Forms; Fields; submit

Type de champ submit
====================

.. versionadded:: 2.3
    Le type ``submit`` a été ajouté à la versionSymfony 2.3

Un bouton submit.

+----------------------+----------------------------------------------------------------------+
| Rendu comme          | ``input`` ``submit`` tag                                             |
+----------------------+----------------------------------------------------------------------+
| Options              | - `attr`_                                                            |
| héritées             | - `disabled`_                                                        |
|                      | - `label`_                                                           |
|                      | - `translation_domain`_                                              |
+----------------------+----------------------------------------------------------------------+
| Type parent          | :doc:`button</reference/forms/types/button>`                         |
+----------------------+----------------------------------------------------------------------+
| Classe               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\SubmitType` |
+----------------------+----------------------------------------------------------------------+

Le bouton Submit est une :methode:`Symfony\\Component\\Form\\ClickableInterface::isClicked` supplémentaire qui permet.
de tester si ce bouton a été utilisé pour soumettre le formulaire. Ceci est particulièrement
utile quand  :ref:`un formulaire a plusieurs boutons de soumission <book-form-submitting-multiple-buttons>`::

    if ($form->get('sauvegardez')->isClicked()) {
        // ...
    }

Inherited options
-----------------

.. include:: /reference/forms/types/options/button_attr.rst.inc

.. include:: /reference/forms/types/options/button_disabled.rst.inc

.. include:: /reference/forms/types/options/button_label.rst.inc

.. include:: /reference/forms/types/options/button_translation_domain.rst.inc
