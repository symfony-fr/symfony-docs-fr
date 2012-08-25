required
~~~~~~~~

**type**: ``Boolean`` **default**: ``true``

En passant cette option à true, un `attribut HTML5 required`_ sera ajouté à l'élément.
L'élément ``label`` correspondant aura quant à lui la classe ``required``.

Il s'agit seulement de l'affichage : ce n'est pas lié à la validation. Le mieux est de
laisser Symfony deviner votre type de champ et définier la valeur de cette option en
fonction de la validation.

.. _`attribut HTML5 required`: http://diveintohtml5.info/forms.html
