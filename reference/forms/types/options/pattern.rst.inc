pattern
~~~~~~~

**type**: ``string`` **default**: ``null``

Cela ajouter un attribut HTML5 ``pattern`` pour restreindre la donnée d'un
champ à une expression régulières.

.. caution:

    L'attribut ``pattern`` offre une validation côté client pour des
    raisons pratiques uniquement. Elle ne doit pas être utilisée en
    remplacement d'une vraie validation côté serveur.

.. note:

	Lorsque vous utilisez les contraintes de validation, cette option
	cette option est parfois définie automatiquement pour correspondre
	à la validation côté serveur.