.. index::
   single: Forms; Twig form function reference

Fonctions et variables de référence du modèle de formulaire Twig
================================================================

Quand vous utilisez les formulaires dans un modèle Twig, il y a plusieurs choses
puissantes à votre disposition:

* :ref:`Functions<reference-form-twig-functions>` pour rendre chaque partie du formulaire
* :ref:`Variables<twig-reference-form-variables>` pour récupérer *certaine* information à propos des champs

Vous utilisez souvent des fonctions pour rendre vos champs. Par contre,
les variables sont moins utilisées, mais infiniment plus puissantes
puisque vous pouvez accéder aux labels, aux attributs, aux erreurs et plus encore.

.. _reference-form-twig-functions:

Fonctions de rendu des formulaires
----------------------------------

Ce manuel couvre toutes les fonctions possibles de Twig disponible pour
rendre des formulaires. Il ya plusieurs fonctions différentes disponibles,
et chacune est responsable du rendu d'une partie différente du formulaire
( par exemple des labels, des erreurs, les widgets, etc.)

.. _reference-forms-twig-form:

form(view, variables)
---------------------

Rends le code HTML complet d'un formulaire.

.. code-block:: jinja

    {# Rends le formulaire et change la méthode de soumission #}
    {{ form(form, {'method': 'GET'}) }}

Vous utiliserez cette fonction pour prototyper ou si vous utilisez des
thèmes personnalisés de formulaires. Si vous avez besoin de plus de flexibilité,
vous devez utiliser les fonctions décrites plus bas pour rendre toutes les
différentes parties d'un formulaire.

.. code-block:: jinja

    {{ form_start(form) }}
        {{ form_errors(form) }}

        {{ form_row(form.name) }}
        {{ form_row(form.dueDate) }}

        <input type="submit" value="Envoyez le formulaire"/>
    {{ form_end(form) }}

.. _reference-forms-twig-start:

form_start(view, variables)
---------------------------

Rend le début du code HTML d'un formulaire. Cette fonction prends en charge
la configuration de la méthode ainsi que la cible de l'action du formulaire.
Il inclura bien sur, la propriété ``enctype`` si le formulaire contient un
champs upload.

.. code-block:: jinja

    {# Rend le début du formulaire et change la méthode de soumission #}
    {{ form_start(form, {'method': 'GET'}) }}

.. _reference-forms-twig-end:

form_end(view, variables)
-------------------------

Rend la fin du code HTML d'un formulaire.

.. code-block:: jinja

    {{ form_end(form) }}

Cette fonction ne rends pas la sortie de ``form_rest()`` seulement si vous
 définissez ``render_rest`` à ``false``:

.. code-block:: jinja

    {# ne pas rendre les champs non définis #}
    {{ form_end(form, {'render_rest': false}) }}

.. _reference-forms-twig-label:

form_label(view, label, variables)
----------------------------------

Affiche le libellé d'un champ donné. Le second paramètre, facultatif, vous permet
de spécifier le libellé que vous voulez afficher.

.. code-block:: jinja

    {{ form_label(form.name) }}

    {# Ces deux syntaxes sont équivalentes #}
    {{ form_label(form.name, 'Votre nom', {'label_attr': {'class': 'foo'}}) }}
    {{ form_label(form.name, null, {'label': 'Votre nom', 'label_attr': {'class': 'foo'}}) }}

Lisez ":ref:`twig-reference-form-variables`" pour en savoir plus sur l'argument ``variables``.

.. _reference-forms-twig-errors:

form_errors(view)
----------------------

Affiche toutes les erreurs d'un champ donné.

.. code-block:: jinja

    {{ form_errors(form.name) }}

    {# affiche les erreurs "globales" #}
    {{ form_errors(form) }}

.. _reference-forms-twig-widget:

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

.. _reference-forms-twig-row:

form_row(form.name, variables)
------------------------------

Affiche le « row » (bloc) d'un champ donné, qui est la combinaison du libellé, des erreurs
et du widget.

.. code-block:: jinja

    {# affiche un bloc de champ, mais affiche « foo » comme libellé #}
    {{ form_row(form.name, {'label': 'foo'}) }}

Le deuxième argument de ``form_row`` est un tableau de variables. Les modèles fournis dans
Symfony ne permettent que de surcharger le libellé, comme vous le voyez dans l'exemple ci-dessus.

Lisez ":ref:`twig-reference-form-variables`" pour en savoir plus sur l'argument ``variables``.

.. _reference-forms-twig-rest:

form_rest(form, variables)
--------------------------

Cette fonction affiche tous les champs d'un formulaire donné qui n'ont pas encore été
affichés. C'est une bonne pratique que de toujours utiliser cette fonction quelque part
dans votre formulaire puisqu'elle affichera tous les champs cachés et vous permettra
de mieux vous rendre compte des champs que vous aurez oublié (car ils seront alors affichés).

.. code-block:: jinja

    {{ form_rest(form) }}

.. _reference-forms-twig-enctype:

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

.. tip::

    Pour la liste complète des variables, lisez :ref:`reference-form-twig-variables`.

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

Jetez un oeil à ``form_label`` à titre d'exemple:

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
    Si vous affichez un formulaire entier en une seule fois
    (ou un formulaire imbriqué), l'argument ``variables`` ne s'appliquera qu'au
    formulaire lui-même et pas à ses enfants. En d'autres termes, le code suivant
    ne passera **pas** l'attribut classe « foo » à tous les enfants du champ de
    formulaire :

    .. code-block:: jinja

        {# ne marche **pas**, les variables ne sont pas récursives #}
        {{ form_widget(form, { 'attr': {'class': 'foo'} }) }}

.. _reference-form-twig-variables:

Variables de formulaires
~~~~~~~~~~~~~~~~~~~~~~~~

Les variables suivantes sont communes à tous les types de champs. Certains
types de champ peuvent plus de variables et certaines variables ne s'appliquent
qu'à un certain type de champ.

Supposons que vous une variable ``form`` dans votre modèle, et que vous
souhaitiez référencer des variables sur le champ ``name``, accéder à ces variables
est possible en utilisant la propriété ``vars`` de l'objet :class:`Symfony\\Component\\Form\\FormView`

.. configuration-block::

    .. code-block:: html+jinja

        <label for="{{ form.name.vars.id }}"
            class="{{ form.name.vars.required ? 'required' : '' }}">
            {{ form.name.vars.label }}
        </label>

    .. code-block:: html+php

        <label for="<?php echo $view['form']->get('name')->vars['id'] ?>"
            class="<?php echo $view['form']->get('name')->vars['required'] ? 'required' : '' ?>">
            <?php echo $view['form']->get('name')->vars['label'] ?>
        </label>

+-----------------+-----------------------------------------------------------------------------------------+
| Variable        | Utilisation                                                                             |
+=================+=========================================================================================+
| ``id``          | L'attribut HTML ``id`` qui est rendu                                                    |
+-----------------+-----------------------------------------------------------------------------------------+
| ``name``        | Le nom du champ( exemple ``title``) - mais pas l'attribut HTML ``name`` qui             |
|                 | est accessible par la variable ``full_name``                                            |
+-----------------+-----------------------------------------------------------------------------------------+
| ``full_name``   | L'attribut HTML ``name`` qui est rendu                                                  |
+-----------------+-----------------------------------------------------------------------------------------+
| ``errors``      | Un tableau de toutes les erreurs attachées à *ce* champ (ex? ``form.title.errors``).    |
|                 | Notez que vous ne pouvez pas utiliser ``form.errors`` pour déterminer si un champ est   |
|                 | valide, il ne contient que les erreurs "globales" : certains champs peuvent avoir des   |
|                 | erreurs. Sinon, utilisez la variable ``valid``                                          |
+-----------------+-----------------------------------------------------------------------------------------+
| ``valid``       | Retourne ``true`` ou  ``false`` selon que le formulaire entier est valide               |
+-----------------+-----------------------------------------------------------------------------------------+
| ``value``       | La valeur qui sera utilisé au moment du rendu (couramment l'attribut HTML ``value``)    |
+-----------------+-----------------------------------------------------------------------------------------+
| ``read_only``   | Si ``true``, ``readonly="readonly"`` est ajouté au champ                                |
+-----------------+-----------------------------------------------------------------------------------------+
| ``disabled``    | Si ``true``, ``disabled="disabled"`` est ajouté au champ                                |
+-----------------+-----------------------------------------------------------------------------------------+
| ``required``    | Si ``true``, un attribut ``required`` est ajouté au champ pour activer la validation    |
|                 | HTML5. De plus, une classe ``required`` est ajoutée au libellé.                         |
+-----------------+-----------------------------------------------------------------------------------------+
| ``max_length``  | Ajoute un attribut HTML ``maxlength`` à l'élément                                       |
+-----------------+-----------------------------------------------------------------------------------------+
| ``pattern``     | Ajoute un attribut HTML ``pattern`` à l'élément                                         |
+-----------------+-----------------------------------------------------------------------------------------+
| ``label``       | La chaine de caractère libellée qui sera rendue                                         |
+-----------------+-----------------------------------------------------------------------------------------+
| ``multipart``   | If ``true``, ``form_enctype`` will render ``enctype="multipart/form-data"``.            |
|                 | This only applies to the root form element.                                             |
+-----------------+-----------------------------------------------------------------------------------------+
| ``attr``        | Un tableau clé-valeur qui sera rendu pour les attributs HTML pour le champ              |
+-----------------+-----------------------------------------------------------------------------------------+
| ``label_attr``  | Un tableau clé-valeur qui sera rendu pour les attributs HTML sur le libellé             |
+-----------------+-----------------------------------------------------------------------------------------+
| ``compound``    | Détermine si l'emplacement est un groupe de champs                                      |
|                 | (par exemple, un champ ``choice``, qui est un ensemble de case à cocher)                |
+-----------------+-----------------------------------------------------------------------------------------+

.. _`form_div_layout.html.twig`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_layout.html.twig
