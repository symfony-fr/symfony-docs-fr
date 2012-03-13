attr
~~~~

**type**: array **default**: Tableau vide

Si vous voulez ajouter des attributs supplémentaires à un champ HTML,
vous pouvez utiliser l'option ``attr`` option. Il s'agit d'un tableau associatif
dont les clés sont des attributs HTML. Cela peut être utile si vous avez besoin de
personnaliser une classe pour un widget::

    $builder->add('body', 'textarea', array(
        'attr' => array('class' => 'tinymce'),
    ));



