empty_data
~~~~~~~~~~

**type**: ``divers`` **default**: ``array()`` si ``multiple`` ou ``expanded``, ``''`` sinon

Cette option détermine quelle valeur le champ affichera si le choix ``empty_value``
est sélectionné.

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
