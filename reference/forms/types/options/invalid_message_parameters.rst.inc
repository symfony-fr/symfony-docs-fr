invalid_message_parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``array`` **default**: ``array()``

Lorsque vous définissez l'option ``invalid_message``, vous pourriez avoir besoin
d'inclure des variables dans la chaîne de caractères. Cela est possible en ajoutant
des paramètres à l'option, et en incluant les variables dans cette option::

    $builder->add('some_field', 'some_type', array(
        // ...
        'invalid_message'            => 'Le format est invalide, il doit contenir %num% lettres',
        'invalid_message_parameters' => array('%num%' => 6),
    ));