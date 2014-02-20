label_attr
~~~~~~~~~~

**type**: ``array`` **default**: ``array()``

Définit les attributs html pour l'élément ``<label>`` qui seront utilisés
pour afficher le libellé d'un champ. C'est un tableau associatif avec les
attributs html en tant que clé. Ces attributs peuvent également être
définis directement au sein d'un template :

.. configuration-block::

    .. code-block:: jinja

        {{ form_label(form.name, 'Your name', {'label_attr': {'class': 'CUSTOM_LABEL_CLASS'}}) }}

    .. code-block:: php

        echo $view['form']->label(
            $form['name'],
            'Your name',
            array('label_attr' => array('class' => 'CUSTOM_LABEL_CLASS'))
        );