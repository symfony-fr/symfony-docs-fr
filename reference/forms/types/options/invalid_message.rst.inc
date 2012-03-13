invalid_message
~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``This value is not valid``

C'est le message d'erreur de validation qui est utilisé quand les données
saisies sont validées par le type de champ. Cela arrive, par exemple, quand
l'utilisateur saisie une chaine de caractères dans un champ :doc:`time</reference/forms/types/time>`
et que cette chaine ne peut être convertie en temps. Pour les messages de validation
classiques (par exemple quand vous définissez une longueur minimum dans un champ), définissez
les avec vos règles de validation (:ref:`reference<book-validation-constraint-configuration>`).