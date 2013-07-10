.. index::
   single: Forms; Fields; date

Type de champ Date
==================

Un champ qui permet à l'utilisateur de modifier une date via différents éléments
HTML.

Les données utilisées par ce type de champ peuvent être un objet ``DateTime``,
une chaîne de caractères, un timestamp ou un tableau. Tant que l'option `input`_
est correctement définie, le champ s'occupera de tous les détails.

Le champ peut être rendu comme un simple champ texte, trois champs texte (mois,
jour et année) ou comme trois listes déroulantes (voyez l'option `widget_`).

+----------------------+-----------------------------------------------------------------------------+
| Type de données      | peut être ``DateTime``, une chaîne de caractères, un timestamp, ou un       |
|                      | tableau (voir l'option ``input``)                                           |
+----------------------+-----------------------------------------------------------------------------+
| Rendu comme          | champ texte unique, trois champs textes, ou trois listes déroulantes        |
+----------------------+-----------------------------------------------------------------------------+
| Options              | - `widget`_                                                                 |
|                      | - `input`_                                                                  |
|                      | - `empty_value`_                                                            |
|                      | - `years`_                                                                  |
|                      | - `months`_                                                                 |
|                      | - `days`_                                                                   |
|                      | - `format`_                                                                 |
|                      | - `model_timezone`_                                                         |
|                      | - `view_timezone`_                                                          |
+----------------------+-----------------------------------------------------------------------------+
| Options surchargées  | - `by_reference`_                                                           |
|                      | - `error_bubbling`_                                                         |
+----------------------+-----------------------------------------------------------------------------+
| Options              | - `invalid_message`_                                                        |
| héritées             | - `invalid_message_parameters`_                                             |
|                      | - `read_only`_                                                              |
|                      | - `disabled`_                                                               |
|                      | - `mapped`_                                                                 |
|                      | - `inherit_data`_                                                           |
|                      | - `virtual`_                                                                |
|                      | - `error_mapping`_                                                          |
+----------------------+-----------------------------------------------------------------------------+
| Type parent          | ``field`` (si texte), ``form`` sinon                                        |
+----------------------+-----------------------------------------------------------------------------+
| Classe               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\DateType`          |
+----------------------+-----------------------------------------------------------------------------+

Utilisation basique
-------------------

Ce type de champ est entièrement configurable mais très facile à utiliser. Les options
les plus importantes sont ``input`` et ``widget``.

Supposons que vous ayez un champ ``publishedAt`` dont la date est un objet ``DateTime``.
L'exemple suivant montre comment configurer le type ``date`` pour que le champ soit
rendu comme trois différents champs Choice (listes déroulantes) :

.. code-block:: php

    $builder->add('publishedAt', 'date', array(
        'input'  => 'datetime',
        'widget' => 'choice',
    ));

L'option ``input`` *doit* être changée pour correspondre au type de donnée de la date.
Par exemple, si la donnée du champ ``publishedAt`` est un timestamp unix, vous
aurez besoin de définir ``input`` à ``timestamp``:

.. code-block:: php

    $builder->add('publishedAt', 'date', array(
        'input'  => 'timestamp',
        'widget' => 'choice',
    ));

Le champ supporte aussi un ``array`` ou une ``string`` comme valeurs valides de
l'option ``input``.

Options du champ
----------------

.. include:: /reference/forms/types/options/date_widget.rst.inc

.. _form-reference-date-input:

.. include:: /reference/forms/types/options/date_input.rst.inc

empty_value
~~~~~~~~~~~

**type**: ``string`` ou ``array``

Si votre option Widget est définie à ``choice``, alors ce champ sera représenté comme
une série de listes déroulantes (``select``). L'option ``empty_value`` peut être
utilisée pour définir un choix « vide » en haut de chaque liste déroulante::

    $builder->add('dueDate', 'date', array(
        'empty_value' => '',
    ));

Sinon, vous pouvez aussi spécifier une chaîne de caractères qui sera affichée pour
la valeur « vide»::

    $builder->add('dueDate', 'date', array(
        'empty_value' => array('year' => 'Year', 'month' => 'Month', 'day' => 'Day')
    ));

.. include:: /reference/forms/types/options/years.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. _reference-forms-type-date-format:

.. include:: /reference/forms/types/options/date_format.rst.inc

.. include:: /reference/forms/types/options/model_timezone.rst.inc

.. include:: /reference/forms/types/options/view_timezone.rst.inc

Options surchargées
-------------------

by_reference
~~~~~~~~~~~~

**default**: ``false``

La classe ``DateTime`` est considéré comme des objets immuables.

error_bubbling
~~~~~~~~~~~~~~

**default**: ``false``

Options héritées
----------------

Ces options héritent du type :doc:`field</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

.. include:: /reference/forms/types/options/virtual.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc