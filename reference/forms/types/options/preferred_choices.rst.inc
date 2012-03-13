preferred_choices
~~~~~~~~~~~~~~~~~

**type**: ``array`` **default**: ``array()``

Si cette option est spécifiée, alors un sous-ensemble d'options sera mis en
tête de la liste déroulante. L'exemple suivant ramènera l'option « Baz »
en haut de la liste, avec un séparateur visuel entre cette option et le
reste des options.::

    $builder->add('foo_choices', 'choice', array(
        'choices' => array('foo' => 'Foo', 'bar' => 'Bar', 'baz' => 'Baz'),
        'preferred_choices' => array('baz'),
    ));

Notez que cette option n'a de sens que si le champ est affiché sous forme de
``liste déroulante`` (c'est-à-dire si ``expanded`` est à false). Les choix 
favoris et les autres choix sont séparés visuellement par une ligne pointillée
(ex ``-------------------``). Cela est bien sur personnalisable lorsque le champ
est affiché :

.. configuration-block::

    .. code-block:: jinja
    
        {{ form_widget(form.foo_choices, { 'separator': '=====' }) }}

    .. code-block:: php
    
        <?php echo $view['form']->widget($form['foo_choices'], array('separator' => '=====')) ?>
