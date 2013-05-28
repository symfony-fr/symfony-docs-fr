property_path
~~~~~~~~~~~~~

**type**: ``any`` **default**: ``la valeur du champ``

Les champs affichent par défaut la valeur de la propriété de l'objet associé
au formulaire. Lorsque le formulaire est soumis, la valeur soumise est réécrite
dans l'objet.

Si vous voulez surcharger la propriété qu'un champ lit ou remplit, vous pouvez
définir l'option ``property_path``. Sa valeur par défaut est le nom du champ.

Si vous souhaitez que le champ ne soit pas pris en compte lorsque l'objet est lu
ou rempli, vous pouvez définir l'option ``property_path`` à ``false``, mais utiliser
cette propiété dans ce cas est déprécié, vous devriez plutôt utiliser l'option ``mapped``.
