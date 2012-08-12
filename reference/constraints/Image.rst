Image
=====

La contrainte Image fonctionne exactement comme la contrainte :doc:`File</reference/constraints/File>`,
sauf que ses options `mimeTypes`_ et `mimeTypesMessage` sont automatiquement définies
pour fonctionner spécifiquement avec des images.

Lisez la documentation de la contrainte :doc:`File</reference/constraints/File>`
pour tout savoir sur cette contrainte.

Options
-------

Cette contrainte partage toutes ses options avec la contrainte :doc:`File</reference/constraints/File>`.
Cependant, elle modifie tout de même deux des valeurs par défaut :

mimeTypes
~~~~~~~~~

**type**: ``array`` ou ``string`` **default**: un tableau d'image de type mime jpg, gif ou png.

Vous pouvez trouver une liste des types mime existant sur le `site IANA`_.

mimeTypesMessage
~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``This file is not a valid image``


.. _`site IANA`: http://www.iana.org/assignments/media-types/image/index.html