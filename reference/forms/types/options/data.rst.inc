data
~~~~

**type**: divers **default**: Valeur par défaut du champ de l'objet sous-jacent (s'il y en a un)

Lorsque vous créez un formulaire, chaque champ affiche initialement la valeur de
la propriété correspondante à l'objet du formulaire (si un objet a été
associé au formulaire). Si vous voulez surcharger la valeur initiale d'un formulaire
ou juste d'un champ individuel, vous pouvez utiliser l'option ``data``::

    $builder->add('token', 'hidden', array(
        'data' => 'abcdef',
    ));


