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
    {{ form_label(form.name, 'Your Name', {'label_attr': {'class': 'foo'}}) }}
    {{ form_label(form.name, null, {'label': 'Your name', 'label_attr': {'class': 'foo'}}) }}

Lisez ":ref:`twig-reference-form-variables`" pour en savoir plus sur l'argument ``variables``.

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
    {{ form_widget(form.name, {'attr': {'class': 'foo'}}) }}

Le deuxième argument de ``form_widget`` est un tableau de variables. La variable
la plus commune est ``attr``, qui est un tableau d'attributs HTML à appliquer au widget.
Dans certains cas, des types ont aussi des options liées au template. C'est au cas par cas.
Les ``attributes`` ne s'appliquent pas récursivement aux champs enfants si vous affichez
plusieurs champs en même temps (ex ``form_widget(form)``).

Lisez ":ref:`twig-reference-form-variables`" pour en savoir plus sur l'argument ``variables``.

form_row(form.name, variables)
------------------------------

Affiche le « row » (bloc) d'un champ donné, qui est la combinaison du libellé, des erreurs
et du widget.

.. code-block:: jinja

    {# affiche un bloc de champ, mais affiche « foo » comme libellé #}
    {{ form_row(form.name, {'label': 'foo'}) }}

Le deuxième argument de ``form_row`` est un tableau de variables. Les templates fournis dans
Symfony ne permettent que de surcharger le libellé, comme vous le voyez dans l'exemple ci-dessus.

Lisez ":ref:`twig-reference-form-variables`" pour en savoir plus sur l'argument ``variables``.

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

.. _`twig-reference-form-variables`:

Un peu plus sur les « Variables » de formulaire
-----------------------------------------------

Dans presque toutes les fonctions Twig ci-dessus, le dernier argument est
un tableau de « variables » qui sont utilisées lors de l'affichage de la partie
de formulaire. Par exemple, le code suivant affichera le « widget » d'une champ, et
modifiera ses attributs pour inclure une classe spéciale :

.. code-block:: jinja

    {# affiche un widget, mais y ajoute une classe "foo" #}
    {{ form_widget(form.name, { 'attr': {'class': 'foo'} }) }}

Le but de ces variables, ce qu'elles font et d'où elles viennent, n'est
peut être pas clair au premier abord, mais elles sont incroyablement puissantes.
Peu importe où vous affichez une partie de formulaire, le block qui l'affiche
utilise un certain nombre de variables. Par défaut, ces blocks se situent dans
`form_div_layout.html.twig`_.

Jetez un oeil à ``form_label`` à titre d'exemple :

.. code-block:: jinja

    {% block form_label %}
        {% if not compound %}
            {% set label_attr = label_attr|merge({'for': id}) %}
        {% endif %}
        {% if required %}
            {% set label_attr = label_attr|merge({'class': (label_attr.class|default('') ~ ' required')|trim}) %}
        {% endif %}
        {% if label is empty %}
            {% set label = name|humanize %}
        {% endif %}
        <label{% for attrname, attrvalue in label_attr %} {{ attrname }}="{{ attrvalue }}"{% endfor %}>{{ label|trans({}, translation_domain) }}</label>
    {% endblock form_label %}

Ce block se sert de plusieurs variables : ``compound``, ``label_attr``, ``required``,
``label``, ``name`` et ``translation_domain``. Ces variables sont rendues disponibles
par le système d'affichage de formulaires. Mais plus important encore, ce sont les
variables que vous pouvez surcharger en appelant ``form_label`` (car dans cet exemple,
nous affichons le label).

Les variables exactes à surcharger dépendent de la partie du formulaire que vous
affichez (ex label ou widget) et quel champ vous affiches (ex un widget ``choice``
a une option ``expanded`` en plus). Si vous êtes capable de vous plonger dans le
fichier `form_div_layout.html.twig`_, vous saurez toujours quelles options sont
disponibles.

.. tip::

    Sous le capot, ces variables sont rendues disponibles par l'objet ``FormView``
    de votre formulaire lorsque le composant Formulaire appelle ``buildView`` et
    ``buildViewBottomUp`` sur chaque « noeud » de l'arbre formulaire. Pour voir
    quelles variables « vue » possède un champ en particulier, trouvez le code
    source du champ de formulaire (et ses parents) et regardez ces deux fonctions.

.. note::

    Si vous affichez un formulaire entier en une seule fois (ou un formulaire
    imbriqué), l'argument ``variables``ne s'appliquera qu'au formulaire lui-même
    et pas à ses enfants. En d'autres termes, le code suivant ne passera **pas**
    l'attribut classe « foo » à tout les enfants du champ de formulaire :

    .. code-block:: jinja

        {# ne marche **pas**, les variables ne sont pas récursives #}
        {{ form_widget(form, { 'attr': {'class': 'foo'} }) }}

.. _`form_div_layout.html.twig`: https://github.com/symfony/symfony/blob/2.1/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_layout.html.twig