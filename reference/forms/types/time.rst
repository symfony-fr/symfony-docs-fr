.. index::
   single: Forms; Fields; time

Type de champ Time
==================

Un champ pour saisir un temps.

Ce champ peut être affiché comme champ texte, une série de champs texte (ex heures, minutes,
secondes) ou une série de listes déroulantes. La donnée finale peut être stockée comme un objet
``DateTime``, une chaîne de caractères, un timestamp ou un tableau.

+----------------------+-----------------------------------------------------------------------------+
| Type de donnée       | Peut être un objet``DateTime``, une chaîne de caractère, un timestamp,      |
|                      | ou un tableau (voir l'option ``input`` )                                    |
+----------------------+-----------------------------------------------------------------------------+
| Rendu comme          | peut être différentes balises (voir plus bas)                               |
+----------------------+-----------------------------------------------------------------------------+
| Options              | - `widget`_                                                                 |
|                      | - `input`_                                                                  |
|                      | - `with_seconds`_                                                           |
|                      | - `hours`_                                                                  |
|                      | - `minutes`_                                                                |
|                      | - `seconds`_                                                                |
|                      | - `data_timezone`_                                                          |
|                      | - `user_timezone`_                                                          |
+----------------------+-----------------------------------------------------------------------------+
| Options              | - `invalid_message`_                                                        |
| héritées             | - `invalid_message_parameters`_                                             |
+----------------------+-----------------------------------------------------------------------------+
| Type parent          | form                                                                        |
+----------------------+-----------------------------------------------------------------------------+
| Classe               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TimeType`          |
+----------------------+-----------------------------------------------------------------------------+

Utilisation basique
-------------------

Ce type de champ est entièrement configurable mais très facile à utiliser. Les options
les plus importantes sont ``input`` et ``widget``.

Supposez que vous avec un champ ``startTime`` dont la donnée sous-jacente est un objet
``DateTime``. L'exemple suivant configure le type ``time`` pour que le champ soit composé
de trois listes déroulantes :

.. code-block:: php

    $builder->add('startTime', 'time', array(
        'input'  => 'datetime',
        'widget' => 'choice',
    ));

L'option ``input`` *doit* être changée pour correspondre au type de donnée date sous-jacent.
Par exemple, si la données du champ ``startTime`` est un timestamp unix, vous devrez définir
l'option ``input`` à ``timestamp`` :

.. code-block:: php

    $builder->add('startTime', 'time', array(
        'input'  => 'timestamp',
        'widget' => 'choice',
    ));

Le champ supporte aussi ``array`` et ``string`` comme valeurs valides de l'option ``input``.

Options du champ
----------------

widget
~~~~~~

**type**: ``string`` **default**: ``choice``


Cette option définit la manière dont le champ doit être affiché. Les choix suivants sont possibles :

* ``choice``: rend deux (ou trois si `with_seconds`_ est à true) listes déroulantes.

* ``text``: rend deux ou trois champs input texte (heures, minutes, secondes).

* ``single_text``: rend un simple input texte. La donnée saisie sera validée en fonction
  du format ``hh:mm`` (ou ``hh:mm:ss`` si vous utilisez les secondes).

input
~~~~~

**type**: ``string`` **default**: ``datetime``

Le format de la donnée *finale*, c'est-à-dire le format dans lequel la donnée
sera stockée dans votre objet. Les valeurs autorisées sont :

* ``string`` (ex ``12:17:26``)
* ``datetime`` (un objet``DateTime``)
* ``array`` (ex ``array('hour' => 12, 'minute' => 17, 'second' => 26)``)
* ``timestamp`` (ex ``1307232000``)

la valeur qui provient du formulaire sera également normalisée selon ce format.

.. include:: /reference/forms/types/options/with_seconds.rst.inc

.. include:: /reference/forms/types/options/hours.rst.inc

.. include:: /reference/forms/types/options/minutes.rst.inc

.. include:: /reference/forms/types/options/seconds.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc

Options héritées
----------------

Ces options héritent du type :doc:`field</reference/forms/types/field>` :

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc