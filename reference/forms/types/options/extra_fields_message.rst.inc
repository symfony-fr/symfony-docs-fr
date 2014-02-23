extra_fields_message
~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``This form should not contain extra fields.``

C'est le message d'erreur de validation qui est utilisé si la donnée de
formulaire soumise contient un ou plusieurs champs qui ne font pas partie
de la définition de formulaire. La variable ``{{ extra_fields }}`` peut
être utilisée pour afficher une liste de noms de champs supplémentaires soumis
séparés par une virgule.