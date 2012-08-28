pattern
~~~~~~~

**type**: ``string``

Cette option n'est utile que si l'option `widget`_ est définie avec la valeur ``choice``.
Le masque par défaut se base sur l'option `format`_, est essaye de trouver
les caractères ``M``, ``d``, et ``y`` du format. Si aucune occurence n'est
trouvée, la valeur par défaut est la chaîne de caractères ``{{ year }}-{{ month }}-{{ day }}``.
Les identifiants de cette option sont :

  * ``{{ year }}``: Remplacé par le widget ``year``
  * ``{{ month }}``: Remplacé par le widget ``month``
  * ``{{ day }}``: Remplacé par le widget ``day``