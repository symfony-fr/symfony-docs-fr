format
~~~~~~

**type**: ``integer`` ou ``string`` **default**: ``IntlDateFormatter::MEDIUM``

Option passée à la classe ``IntlDateFormatter``, utilisée pour formatter
la saisie de l'utilisateur au bon format. C'est essentiel quand l'option
`widget`_  est définie comme ``single_text``, et elle définira la manière
dont l'utilisateur devra saisir ses données. 
Par défaut, le format est déterminé selon la locale de l'utilisateur, mais
vous pouvez la surcharger en passant le format sous forme de chaine de
caractères.

Pour plus d'informations sur les formats valides, lisez `Syntaxe des formats Date/Time`_.
Par exemple, pour afficher un simple champ texte qui attend une saisie au format
``yyyy-MM-dd``, utilisez l'option suivante::

    $builder->add('date_created', 'date', array(
        'widget' => 'single_text',
        'format' => 'yyyy-MM-dd',
    ));

.. _`Syntaxe des formats Date/Time`: http://userguide.icu-project.org/formatparse/datetime#TOC-Date-Time-Format-Syntax
