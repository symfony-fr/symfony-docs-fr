data_class
~~~~~~~~~~

**type**: ``string``

Cette option est utilisée pour définir le mapper de données approprié qui
sera utilisé par le formulaire, donc vous pouvez l'utiliser pour tout type
de champ de formulaire qui nécessite un objet.

.. code-block:: php

    $builder->add('media', 'sonata_media_type', array(
        'data_class' => 'Acme\DemoBundle\Entity\Media',
    ));