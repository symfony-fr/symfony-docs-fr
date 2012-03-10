.. index::
   single: Forms; Twig form function reference

Fonctions de template de formulaire Twig de référence
=====================================================

Cette documentation de référence liste toutes les fonctions Twig possibles
pour rendre des formulaires. Plusieurs fonctions différentes sont disponibles,
chacune étant chargée de rendre une partie du formulaire (ex libellés, erreurs,
widgets, etc).

form_label(form.name, label, variables)
---------------------------------------

Affiche le libellé d'un champ donné. Le second paramètre, facultatif, vous permet
de spécifier le libellé que vous voulez afficher.

.. code-block:: jinja

    {{ form_label(form.name) }}

    {# Ces deux syntaxes sont équivalentes #}
    {{ form_label(form.name, 'Votre nom', { 'attr': {'class': 'foo'} }) }}
    {{ form_label(form.name, null, { 'label': 'Votre nom', 'attr': {'class': 'foo'} }) }}

form_errors(form.name)
----------------------

Affiche toutes les erreurs d'un champ donné.

.. code-block:: jinja

    {{ form_errors(form.name) }}

    {# affiche les erreurs "globales" #}
    {{ form_errors(form) }}

form_widget(form.name, variables)
---------------------------------

Affiche le widget HTML d'un champ donné. Si vous l'appliquez au formulaire entier
ou à une collection de champs, chaque item du formulaire sera affiché.

.. code-block:: jinja

    {# affiche un widget, et lui affecte la classe "foo" #}
    {{ form_widget(form.name, { 'attr': {'class': 'foo'} }) }}

Le deuxième argument de ``form_widget`` est un tableau de variables. La variable
la plus commune est ``attr``, qui est un tableau d'attibuts HTML à appliquer au widget.
Dans certains cas, des types ont aussi des options liées au template. C'est au cas par cas.

form_row(form.name, variables)
------------------------------

Affiche le « row » (bloc) d'un champ donné, qui est la combinaison du libellé, des erreurs
et du widget.

.. code-block:: jinja

    {# affiche un bloc de champ, mais affiche « foo » comme libellé #}
    {{ form_row(form.name, { 'label': 'foo' }) }}

Le deuxième argument de ``form_row`` est un tableau de variables. Les templates fournis dans
Symfony ne permettent que de surcharger le libellé, comme vous le voyez dans l'exemple ci-dessus.

form_rest(form, variables)
--------------------------

Cette fonction affiche tous les champs d'un formulaire donné qui n'ont pas encore été
affichés. C'est une bonne pratique que de toujours utiliser cette fonction quelque part
dans votre formulaire puisqu'elle affichera tous les champs cachés et vous permettra
de mieux vous rendre compte des champs que vous aurez oublié (car ils seront alors affichés).

.. code-block:: jinja

    {{ form_rest(form) }}

form_enctype(form)
------------------

Si le formulaire contient au moins un champ d'upload de fichier, cette fonction
affichera l'attribut de formulaire ``enctype="multipart/form-data"``. C'est une bonne
pratique de toujours l'ajouter dans votre balise form :

.. code-block:: html+jinja

    <form action="{{ path('form_submit') }}" method="post" {{ form_enctype(form) }}>