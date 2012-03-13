*   ``disabled`` [type: Boolean, default: false]
    Si vous ne voulez pas qu'un utilisateur puisse modifier la valeur d'un champ,
	vous pouvez définir l'option ``disabled`` à true. Toute valeur soumise sera
	ignorée.
	
    .. code-block:: php

        use Symfony\Component\Form\TextField
      
        $field = new TextField('status', array(
            'data' => 'Ancienne valeur',
            'disabled' => true,
        ));
        $field->submit('Nouvelle valeur');

        // affiche "Ancienne valeur"
        echo $field->getData();


