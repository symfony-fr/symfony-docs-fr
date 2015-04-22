.. index::
    single: Console Helpers; Table Helper

Table Helper
============

.. versionadded:: 2.3
    Le Helper ``table`` a été ajouté à Symfony 2.3.

Quand vous développez une application console, il peut être utile d'afficher
des tableaux de données:

.. image:: /images/components/console/table.png

Pour afficher un tableau, utilisez la classe :class:`Symfony\\Component\\Console\\Helper\\TableHelper`,
définissez les entêtes, les lignes et  affichez::

    $table = $this->getHelperSet()->get('table');
    $table
        ->setHeaders(array('ISBN-13', 'TITRE', 'AUTEUR'))
        ->setRows(array(
            array('978-2080712196', 'La divine comédie', 'Dante Alighieri'),
            array('978-2070381951', 'Le conte de deux cités', 'Charles Dickens'),
            array('978-2266232999', 'Le seigneur des anneaux', 'J. R. R. Tolkien'),
            array('978-2013224024', 'Les dix petits nègres', 'Agatha Christie'),
        ))
    ;
    $table->render($output);

L'aspect du tableau peut aussi être modifié. Il y a deux façons de personnaliser
le rendu du tableau: utilisé des layouts nommés ou en personnalisant les options
de rendu.

Personnaliser l'aspect d'un tableau en utilisant des layouts nommés
-------------------------------------------------------------------

Le Helper Table embarque deux layouts déjà configurés:

* ``TableHelper::LAYOUT_DEFAULT``

* ``TableHelper::LAYOUT_BORDERLESS``

Le Layout peut être défini avec la méthode :method:`Symfony\\Component\\Console\\Helper\\TableHelper::setLayout`.

Personnaliser l'aspect d'un tableau en utilisant les options de rendu
---------------------------------------------------------------------

Vous pouvez contrôler le rendu du tableau en définissant avec des valeurs
personnalisées les options de rendu:

*  :method:`Symfony\\Component\\Console\\Helper\\TableHelper::setPaddingChar`
*  :method:`Symfony\\Component\\Console\\Helper\\TableHelper::setHorizontalBorderChar`
*  :method:`Symfony\\Component\\Console\\Helper\\TableHelper::setVerticalBorderChar`
*  :method:`Symfony\\Component\\Console\\Helper\\TableHelper::setVrossingChar`
*  :method:`Symfony\\Component\\Console\\Helper\\TableHelper::setVellHeaderFormat`
*  :method:`Symfony\\Component\\Console\\Helper\\TableHelper::setVellRowFormat`
*  :method:`Symfony\\Component\\Console\\Helper\\TableHelper::setBorderFormat`
*  :method:`Symfony\\Component\\Console\\Helper\\TableHelper::setPadType`
