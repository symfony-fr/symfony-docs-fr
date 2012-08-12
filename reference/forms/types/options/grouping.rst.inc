grouping
~~~~~~~~

**type**: ``integer`` **default**: ``false``

Cette valeur est utilisée en interne comme valeur de ``NumberFormatter::GROUPING_USED`` en utilisant
la classe PHP ``NumberFormatter``. Sa documentation est inexistante mais il apparait que si vous la
définissez à ``true``,  les nombres seront regroupés avec les virgules ou par période (selon votre locale) :
``12345.123`` affichera ``12,345.123``.