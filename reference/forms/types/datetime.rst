.. index::
   single: Forms; Fields; datetime

Type de champ Datetime
======================

Ce type de champ permet à l'utilisateur de manipuler une donnée qui représente
une date et une heure (ex: ``1984-06-05 12:15:30``).

Il peut être rendu comme un input text ou des balises select. Le format de la donnée
réelle peut être un objet ``DateTime``, une string, un timestamp ou un tableau.

+----------------------+-----------------------------------------------------------------------------+
| Type de données      | peut être ``DateTime``, une chaîne de caractères, un timestamp,             |
|                      | ou un tableau (voir l'option ``input``)                                     |
+----------------------+-----------------------------------------------------------------------------+
| Rendu comme          | simple champ texte ou trois champs select                                   |
+----------------------+-----------------------------------------------------------------------------+
| Options              | - `date_widget`_                                                            |
|                      | - `time_widget`_                                                            |
|                      | - `widget`_                                                                 |
|                      | - `input`_                                                                  |
|                      | - `date_format`_                                                            |
|                      | - `format`_                                                                 |
|                      | - `hours`_                                                                  |
|                      | - `minutes`_                                                                |
|                      | - `seconds`_                                                                |
|                      | - `years`_                                                                  |
|                      | - `months`_                                                                 |
|                      | - `days`_                                                                   |
|                      | - `with_minutes`_                                                           |
|                      | - `with_seconds`_                                                           |
|                      | - `model_timezone`_                                                         |
|                      | - `view_timezone`_                                                          |
|                      | - `empty_value`_                                                            |
+----------------------+-----------------------------------------------------------------------------+
| Options              | - `data`_                                                                   |
| héritées             | - `invalid_message`_                                                        |
|                      | - `invalid_message_parameters`_                                             |
|                      | - `read_only`_                                                              |
|                      | - `disabled`_                                                               |
|                      | - `mapped`_                                                                 |
|                      | - `inherit_data`_                                                           |
+----------------------+-----------------------------------------------------------------------------+
| Type parent          | :doc:`form</reference/forms/types/form>`                                    |
+----------------------+-----------------------------------------------------------------------------+
| Classe               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\DateTimeType`      |
+----------------------+-----------------------------------------------------------------------------+

Options du champ
----------------

date_widget
~~~~~~~~~~~

**type**: ``string`` **default**: ``choice``

Définit l'option ``widget`` pour le type :doc:`date</reference/forms/types/date>`

time_widget
~~~~~~~~~~~

**type**: ``string`` **default**: ``choice``

Définit l'option ``widget`` pour le type :doc:`time</reference/forms/types/time>`

input
~~~~~

**type**: ``string`` **default**: ``datetime``

Le format de la donnée *finale* - c'est-à-dire le format sous lequel la date
est stockée dans l'objet. Les valeurs valides sont :

* ``string`` (ex: ``2011-06-05 12:15:00``)
* ``datetime`` (un objet ``DateTime``)
* ``array`` (ex: ``array(2011, 06, 05, 12, 15, 0)``)
* ``timestamp`` (ex: ``1307276100``)

La valeur provenant du formulaire sera aussi normalisée dans ce format.

.. include:: /reference/forms/types/options/_date_limitation.rst.inc

date_format
~~~~~~~~~~~

**type**: ``integer`` ou ``string`` **default**: ``IntlDateFormatter::MEDIUM``

Définit l'option ``format`` qui sera passée au champ date.
Jetez un oeil à :ref:`l'option format du type date<reference-forms-type-date-format>`
pour plus de détails.

format
~~~~~~

**type**: ``string`` **default**: ``Symfony\Component\Form\Extension\Core\Type\DateTimeType::HTML5_FORMAT``

Si l'option ``widget`` est définie à ``single_text``, cette option spécifie
le format du champ, c-à-d la manière dont Symfony interprètera la date saisie
sous forme de chaine de caractères. Par défaut, c'est le format `RFC 3339`_ qui
est utilisé par le champ ``datetime`` HTML5. Garder la valeur par défaut fera
que le champ sera affiché comme un champ ``input` avec l'attribut ``type="datetime"``.

.. include:: /reference/forms/types/options/hours.rst.inc

.. include:: /reference/forms/types/options/minutes.rst.inc

.. include:: /reference/forms/types/options/seconds.rst.inc

.. include:: /reference/forms/types/options/years.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/with_minutes.rst.inc

.. include:: /reference/forms/types/options/with_seconds.rst.inc

.. include:: /reference/forms/types/options/model_timezone.rst.inc

.. include:: /reference/forms/types/options/view_timezone.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

Options héritées
----------------

Ces options héritent du type :doc:`field</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

.. _`RFC 3339`: http://tools.ietf.org/html/rfc3339