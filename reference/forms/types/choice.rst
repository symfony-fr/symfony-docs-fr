.. index::
   single: Forms; Fields; choice

Type de champ Choice
====================

Un champ multi-usage pour permettre à l'utilisateur de "choisir" une ou plusieurs
options. Il peut être affiché avec des balises ``select``, des boutons radio, ou
des checkboxes.

Pour utiliser ce champ, vous devez spécifier *soit l'option* ``choice_list``, *soit* ``choices``.

+-------------+-----------------------------------------------------------------------------+
| Rendu comme | peut être différentes balises (voir ci-dessous)                             |
+-------------+-----------------------------------------------------------------------------+
| Options     | - `choices`_                                                                |
|             | - `choice_list`_                                                            |
|             | - `multiple`_                                                               |
|             | - `expanded`_                                                               |
|             | - `preferred_choices`_                                                      |
|             | - `empty_value`_                                                            |
|             | - `empty_data`_                                                             |
+-------------+-----------------------------------------------------------------------------+
| Options     | - `required`_                                                               |
| héritées    | - `label`_                                                                  |
|             | - `read_only`_                                                              |
|             | - `error_bubbling`_                                                         |
+-------------+-----------------------------------------------------------------------------+
| Type parent | :doc:`form</reference/forms/types/form>` (if expanded), ``field`` otherwise |
+-------------+-----------------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\ChoiceType`        |
+-------------+-----------------------------------------------------------------------------+

Exemple d'utilisation
---------------------

La manière la plus facile d'utiliser ce champ est de spécifier directement les
choix possibles via l'option ``choices``. L'index du tableau deviendra la valeur
qui sera effectivement définie dans votre objet final (ex: ``m``), alors que la valeur
est ce que verra l'utilisateur dans le formulaire (ex: ``Masculin``).

.. code-block:: php

    $builder->add('gender', 'choice', array(
        'choices'   => array('m' => 'Masculin', 'f' => 'Féminin'),
        'required'  => false,
    ));

En définissant l'option ``multiple`` à true, vous pouvez autoriser l'utilisateur à 
choisir plusieurs valeurs. Le widget sera rendu comme une balide``select`` multiple, ou
comme une série de checkboxes en fonction de l'option ``expanded`` :

.. code-block:: php

    $builder->add('availability', 'choice', array(
        'choices'   => array(
            'matin'   => 'Matin',
            'apresmidi' => 'Après-midi',
            'soir'   => 'Soir',
        ),
        'multiple'  => true,
    ));

Vous pouvez aussi utiliser l'option ``choice_list``, qui prend un objet comme argument
pour spécifier les choix de votre widget.

.. _forms-reference-choice-tags:

.. include:: /reference/forms/types/options/select_how_rendered.rst.inc

Options du champ
----------------

choices
~~~~~~~

**type**: ``array`` **default**: ``array()``

C'est la façon la plus simple de spécifier les choix qui pourront être choisis 
dans le champ. L'option ``choices`` est un tableay, où les index sont les valeurs
des items, et les valeurs du tableau sont les labels des items :

.. code-block:: php

    $builder->add('gender', 'choice', array(
        'choices' => array('m' => 'Masculin', 'f' => 'Féminin')
    ));

choice_list
~~~~~~~~~~~

**type**: ``Symfony\Component\Form\Extension\Core\ChoiceList\ChoiceListInterface``

C'est une façon de spécifier les options de champ à utiliser. L'option ``choice_list``
doit être une instance de ``ChoiceListInterface``.
Pour des cas plus avancés, une classe personnalisée qui implémente l'interface peut
être créée pour définir les choix.

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc

Options héritées
----------------

Ces options héritent du type :doc:`field</reference/forms/types/field>` :

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
