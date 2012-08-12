label
~~~~~

**type**: ``string`` **default**: Le libellé est à deviné à partir du nom du champ.

Définit le libellé qui sera utilisé à l'affichage du champ. Le libellé peut aussi
être directement défini dans le template :
    
.. code-block:: jinja

    {{ form_label(form.name, 'Votre nom') }}