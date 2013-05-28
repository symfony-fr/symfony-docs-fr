label
~~~~~

**type**: ``string`` **default**: Le label est "guessed" pour le champ name.

Définissez le label que vous souhaitez pour le bouton. Ce label peut aussi
être défini directement dans le modèle.

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_widget(form.save, { 'label': 'Cliquez ici' }) }}

    .. code-block:: html+php

        <?php echo $view['form']->widget($form['save'], array('label' => 'Cliquez ici')) ?>
