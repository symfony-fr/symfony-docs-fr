empty_value
~~~~~~~~~~~

**type**: ``string`` ou ``Boolean``

Cette option détermine si une option « vide » spécial (ex : « Choisissez une option »)
soit apparaitre en haut de la liste déroulante. Cette option ne s'applique que si les
deux option ``expanded`` et ``multiple`` ont été définies à false.

* Ajoute une valeur vide avec « Choisissez une option » comme texte::

    $builder->add('states', 'choice', array(
        'empty_value' => 'Choisissez une option',
    ));

* Garantie qu'aucune option « vide » n'est affichée::

    $builder->add('states', 'choice', array(
        'empty_value' => false,
    ));

Si vous ne définissez pas l'option ``empty_value``, alors une option vide (sans texte)
sera automatiquement ajoutée si et seulement si l'option ``required`` est à false::

    // Une option vide (sans texte) sera ajoutée
    $builder->add('states', 'choice', array(
        'required' => false,
    ));