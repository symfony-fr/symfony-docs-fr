.. index::
   single: Forms; Fields; repeated

Type de champ Repeated
======================

C'est un « groupe » spécial de champs qui crée deux champs identiques dont les valeurs
doivent correspondre (sinon une erreur de validation s'affiche). L'usage le plus commun
est lorsque vous avez besoin que l'utilisateur tape une nouvelle fois son mot de passe
ou son email pour vérifier qu'ils sont justes.

+-------------+------------------------------------------------------------------------+
| Rendu comme | Champ input ``text`` par défaut, mais voyez l'option `type`_ option    |
+-------------+------------------------------------------------------------------------+
| Options     | - `type`_                                                              |
|             | - `options`_                                                           |
|             | - `first_options`_                                                     |
|             | - `second_options`_                                                    |
|             | - `first_name`_                                                        |
|             | - `second_name`_                                                       |
+-------------+------------------------------------------------------------------------+
| Options     | - `error_bubbling`_                                                    |
| surchargées |                                                                        |
+-------------+------------------------------------------------------------------------+
| Options     | - `invalid_message`_                                                   |
| héritées    | - `invalid_message_parameters`_                                        |
|             | - `mapped`_                                                            |
|             | - `error_mapping`_                                                     |
+-------------+------------------------------------------------------------------------+
| Type parent | :doc:`field</reference/forms/types/form>`                              |
+-------------+------------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\RepeatedType` |
+-------------+------------------------------------------------------------------------+

Exemple d'utilisation
---------------------

.. code-block:: php

    $builder->add('password', 'repeated', array(
        'type' => 'password',
        'invalid_message' => 'Les mots de passe doivent correspondre',
        'options' => array('required' => true),
        'first_options'  => array('label' => 'Mot de passe'),
        'second_options' => array('label' => 'Mot de passe (validation)'),
    ));

Lors de la soumission réussie d'un formulaire, la valeur saisie dans les deux
champs « password » devient la donnée de la clé ``password``. En d'autres termes,
même si deux champs sont soumis en réalité, la donnée finale du formulaire est juste
la valeur unique dont vous avez besoin (généralement une chaîne de caractères).

L'option la plus importante est ``type``, qui peut être n'importe quel type de champ
et qui détermine le réel type des deux champs sous-jacents. L'option ``options`` est passée
à chacun de ces deux champs ce qui signifie, dans cet exemple, que toute option
supportée par le type ``password`` peut être passée dans ce tableau.

Affichage
~~~~~~~~~

Le type de champ ``repeated`` est en fait composé de 2 champs, que vous pouvez afficher en
une fois, ou individuellement. Pour les affichers en une seule fois, vous pouvez faire :

.. configuration-block::

    .. code-block:: jinja

        {{ form_row(form.password) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['password']) ?>

Pour afficher chaque champ indivuellement, vous pouvez faire :

.. configuration-block::

    .. code-block:: jinja

        {{ form_row(form.password.first) }}
        {{ form_row(form.password.second) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['password']['first']) ?>
        <?php echo $view['form']->row($form['password']['second']) ?>

.. note::

    Les noms des sous-champs sont ``first`` et ``second`` par défaut,
    mais peuvent être modifiés via les options `first_name`_ et `second_name`_.

Validation
~~~~~~~~~~

L'une des fonctionnalités clé du champ ``repeated`` est sa validation interne
(vous n'avez rien besoin de faire pour l'activer) qui force les 2 champs à avoir
la même valeur. Si les deux valeurs ne sont pas identiques, une erreur sera
envoyée à l'utilisateur.

L'option ``invalid_message`` est utilisée pour personnaliser l'erreur qui
sera affichée si les deux valeurs ne correspondent pas.

Options du champ
----------------

type
~~~~

**type**: ``string`` **default**: ``text``

Les deux champs sous-jacents auront ce type de champ. Par exemple, passer le
type ``password`` retournera deux champs « mot de passe ».

options
~~~~~~~

**type**: ``array`` **default**: ``array()``

Ce tableau d'options sera passé à chacun des deux champs sous-jacents. En d'autres
termes, ce sont les options qui personnalisent les types de champs individuellement.
Par exemple, si l'option ``type`` est définie comme ``password``, ce tableau peut
contenir les options ``always_empty`` ou ``required``, c'est-à-dire deux options qui
sont supportées par le type de champ ``password``.

first_options
~~~~~~~~~~~~~

**type**: ``array`` **default**: ``array()``

.. versionadded:: 2.1
    L'option ``first_options`` est une nouveauté de Symfony 2.1.

Option additionnelle (elle sera fusionnée avec `options` ci-dessus) qui devrait
être passée *uniquement* au premier champ. Elle est essentiellement utile pour
personnaliser le libellé::

    $builder->add('password', 'repeated', array(
        'first_options'  => array('label' => 'Mot de passe'),
        'second_options' => array('label' => 'Mot de passe (validation)'),
    ));

second_options
~~~~~~~~~~~~~~

**type**: ``array`` **default**: ``array()``

.. versionadded:: 2.1
    L'option ``second_options`` est une nouveauté de Symfony 2.1.

Option additionnelle (elle sera fusionnée avec `options` ci-dessus) qui devrait
être passée *uniquement* au premier champ. Elle est essentiellement utile pour
personnaliser le libellé (voir `first_options`_).

first_name
~~~~~~~~~~

**type**: ``string`` **default**: ``first``

C'est le nom de champ qui sera utilisé par le premier champ. Ce n'est en fait
pas très important puisque la donnée saisie dans les deux champs sera disponible
en utilisant la clé du champ ``repeated`` lui-même (ex ``password``).
Cependant, si vous ne spécifiez pas de libellé, ce nom de champ est utilisé pour
deviner le libellé à votre place.

second_name
~~~~~~~~~~~

**type**: ``string`` **default**: ``second``

Le même que ``first_name``, mais pour le second champ.

Options surchargées
-------------------

error_bubbling
~~~~~~~~~~~~~~

**default**: ``false``

Options héritées
----------------

Ces options héritent du type :doc:`form</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc