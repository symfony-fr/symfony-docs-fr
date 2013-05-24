attr
~~~~

**type**: array **default**: Tableau vide

Si vous voulez ajouter des attributs supplémentaires à une représentation HTML d'un bouton,
Vous pouvez utiliser l'option ``attr``. Il s'agit d'un tableau associatif
dont les clés sont des attributs HTML. Cela peut être utile si vous avez besoin de
personnaliser une classe pour un bouton::

    $builder->add('save', 'button', array(
        'attr' => array('class' => 'save'),
    ));



