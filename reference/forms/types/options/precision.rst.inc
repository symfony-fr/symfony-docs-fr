precision
~~~~~~~~~

**type**: ``integer`` **default**: Selon la locale (souvent autour de ``3``)

Cette option spécifie combien de décimales seront autorisées à l'arrondissement
de la valeur soumise (via ``rounding_mode``). Par exemple si ``precision``
est défini à ``2``, une valeur soumise de ``20.123`` sera arrondie, par
exemple, à ``20.12`` (selon votre ``rounding_mode``).
