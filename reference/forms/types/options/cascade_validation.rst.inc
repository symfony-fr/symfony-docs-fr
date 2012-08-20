cascade_validation
~~~~~~~~~~~~~~~~~~

**type**: Boolean **default**: false

Définissez cette option à ``true`` pour forcer la validation des types
de formulaires imbriqués.
Par exemple, si vous avez un ``ProductType`` qui imbrique un ``CategoryType``,
définir ``cascade_validation`` à ``true`` pour le ``ProductType`` entrainera
également la validation des données du ``CategoryType``.

Au lieu d'utiliser cette option, vous pouvez également utiliser la contrainte
``Valid`` dans votre modèle pour forcer la validation d'un objet enfant stocké
dans la propriété.