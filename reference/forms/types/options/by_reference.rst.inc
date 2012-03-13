by_reference
~~~~~~~~~~~~

**type**: ``Boolean`` **default**: ``true``

Si la valeur réelle d'un champ est un objet et que cette option est à ``true``,
alors l'objet résultant ne sera pas défini lors de l'association au formulaire.
Par exemple, si vous avez sur un objet un champ protected ``author`` qui est
une instance d'un objet ``Author``, alors si l'option ``by_reference`` est à
``false``, cet objet ``Author`` sera mis à jour avec les données soumises, mais
la méthode ``setAuthor`` ne sera pas appelée sur l'objet principal. 
Puisque l'objet ``Author`` est une référence, cela ne fait de réelle différence
que dans le cas où vous avez personnalisé la méthode ``setAuthor``. Dans ce cas,
définissez l'option à ``false``.