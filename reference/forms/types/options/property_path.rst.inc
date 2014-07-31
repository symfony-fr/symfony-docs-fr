property_path
~~~~~~~~~~~~~

**type**: ``any`` **default**: ``la valeur du champ``

Les champs affichent par défaut la valeur de la propriété de l'objet associé
au formulaire. Lorsque le formulaire est soumis, la valeur soumise est réécrite
dans l'objet.

Si vous voulez surcharger la propriété qu'un champ lit ou remplit, vous pouvez
définir l'option ``property_path``. Sa valeur par défaut est le nom du champ.

Si vous souhaitez que le champ ne soit pas pris en compte lorsque l'objet est lu
ou rempli, vous pouvez définir l'option ``property_path`` à ``false``, mais
utiliser cette propriété dans ce cas est déprécié, vous devriez plutôt
utiliser l'option ``mapped``.

.. versionadded:: 2.1
    Depuis la version 2.1, l'option ``mapped`` a été ajouté pour cette
    utilisation
