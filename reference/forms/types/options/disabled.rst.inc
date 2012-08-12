disabled
~~~~~~~~
**type**: ``boolean``  **default**: ``false``

    Si vous ne voulez pas qu'un utilisateur puisse modifier la valeur d'un champ,
    vous pouvez définir l'option ``disabled`` à true. Toute valeur soumise sera
    ignorée.

.. code-block:: php

    use Symfony\Component\Form\TextField
      
    $field = new TextField('status', array(
        'data' => 'Old data',
        'disabled' => true,
    ));
    $field->submit('New data');

    // prints "Old data"
    echo $field->getData();

