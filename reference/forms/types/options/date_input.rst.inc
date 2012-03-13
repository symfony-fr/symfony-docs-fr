input
~~~~~

**type**: ``string`` **default**: ``datetime``

Le format de la donnée *finale*, c'est-à-dire le format dans lequel la date
sera stockée dans l'objet sous-jacent. Les valeurs valides sont :

* ``string`` (ex ``2011-06-05``)
* ``datetime`` (un objet ``DateTime``)
* ``array`` (ex ``array('year' => 2011, 'month' => 06, 'day' => 05)``)
* ``timestamp`` (ex ``1307232000``)

La valeur qui provient du formulaire sera également normalisée dans ce format.