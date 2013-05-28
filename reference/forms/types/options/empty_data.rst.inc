empty_data
~~~~~~~~~~

**type**: ``divers`` **default**: ``array()`` si ``multiple`` ou ``expanded``, ``''`` sinon

Cette option détermine quelle valeur le champ affichera si le choix ``empty_value``
est sélectionné.

La valeur par défaut de cette option dépends des options du champ :

* Si ``compound`` est à ``true`` et  ``data_class`` est défini, alors ``new $data_class()``;
* Si ``compound`` est à ``true`` et ``data_class`` n'est pas défini, alors ``array()``;
* Si ``compound`` est à ``false``, alors ``null``.

.. tip::

L'option ``compound`` est définie à ``true`` quand le champ représente en fait une collection
de champs (ex un formulaire de champs).

Par exemple, si vous voulez que le champ ``gender`` soit défini comme ``null`` si
aucune valeur n'est sélectionnée, vous pouvez le faire comme ceci :

.. code-block:: php

    $builder->add('gender', 'choice', array(
        'choices' => array(
            'm' => 'Masculin',
            'f' => 'Féminin'
        ),
        'required'    => false,
        'empty_value' => 'Choisissez votre sexe',
        'empty_data'  => null
    ));

.. note::

Si vous voulez définir l'otion ``empty_data`` pour l'intégralité de la
la classe du formulaire, allez à l'article suivant :doc:`/cookbook/form/use_empty_data`
