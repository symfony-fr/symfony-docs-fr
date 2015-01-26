.. index::
   single: Form; Custom form rendering

Comment personnaliser le rendu de formulaire
============================================

Symfony vous propose un large choix de méthodes pour personnaliser la manière
dont un formulaire est affiché.
Dans ce guide, vous apprendrez comment personnaliser autant que possible chaque
partie de votre formulaire sans effort, et ceci que vous utilisiez Twig ou PHP
comme moteur de template.

Bases de l'affichage de formulaire
----------------------------------

Rappelons que le libellé, les éventuelles erreurs et le widget HTML d'un champ
de formulaire peuvent être facilement affichés en utilisant la fonction Twig
``form_row`` ou la méthode d'aide (« helper » en anglais) PHP ``row`` :

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_row(form.age) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['age']) ?>

Vous pouvez également afficher chacune de ces trois parties d'un champ
individuellement :

.. configuration-block::

    .. code-block:: jinja

        <div>
            {{ form_label(form.age) }}
            {{ form_errors(form.age) }}
            {{ form_widget(form.age) }}
        </div>

    .. code-block:: php

        <div>
            <?php echo $view['form']->label($form['age']) ?>
            <?php echo $view['form']->errors($form['age']) ?>
            <?php echo $view['form']->widget($form['age']) ?>
        </div>

Dans les deux cas, le libellé du formulaire, les erreurs et le widget HTML sont
affichés en utilisant un ensemble de balises livré d'office avec Symfony.
Par exemple, les deux templates ci-dessus afficheront :

.. code-block:: html

    <div>
        <label for="form_age">Age</label>
        <ul>
            <li>This field is required</li>
        </ul>
        <input type="number" id="form_age" name="form[age]" />
    </div>

Pour tester rapidement un formulaire, vous pouvez afficher l'ensemble du formulaire
en une seule ligne :

.. configuration-block::

    .. code-block:: jinja

        {{ form_widget(form) }}

    .. code-block:: php

        <?php echo $view['form']->widget($form) ?>

La suite de ce document explique comment le rendu de chaque partie du formulaire
peut être modifié à différents niveaux. Pour plus d'informations sur l'affichage
de formulaires en général, lisez :ref:`form-rendering-template`.

.. _cookbook-form-customization-form-themes:

Que sont les thèmes de formulaire ?
-----------------------------------

Symfony utilise des fragments de formulaires - des parties de template qui
affichent juste une partie du formulaire - pour afficher chaque partie du
formulaire : libellés de champs, erreurs, champs texte ``input``, balises
``select``, etc.

Ces fragments sont définis comme blocs dans Twig et comme fichiers de templates
avec PHP.

Un *thème* n'est rien de plus qu'un ensemble de fragments que vous pouvez utiliser
pour afficher un formulaire. En d'autres termes, si vous voulez personnaliser
l'affichage d'une partie d'un formulaire, vous pouvez importer un *thème* qui
contient une personnalisation des fragments de formulaire concernés.

Symfony est fourni avec un thème par défaut (`form_div_layout.html.twig`_ pour Twig
et ``FrameworkBundle:Form`` pour PHP) qui définit chaque fragment nécessaire à
l'affichage des différentes parties d'un formulaire.

Dans la section suivante, vous apprendrez comment personnaliser un thème en surchargeant
un ou l'ensemble de ses fragments.

Par exemple, lorsque le widget d'un type de champ ``integer`` est affiché, un champ
``input`` ``number`` est généré.

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_widget(form.age) }}

    .. code-block:: php

        <?php echo $view['form']->widget($form['age']) ?>

affiche :

.. code-block:: html

    <input type="number" id="form_age" name="form[age]" required="required" value="33" />

En interne, Symfony utilise le fragment ``integer_widget`` pour afficher le champ.
C'est parce que le type de champ est ``integer`` et que vous voulez afficher son ``widget``
(par opposition à son ``libellé`` ou à ses ``erreurs``).

Par défaut, avec Twig, le bloc ``integer_widget`` du template `form_div_layout.html.twig`_ serait choisi.

En PHP, cela serait plutôt le fichier ``integer_widget.html.php`` situé dans le dossier
``FrameworkBundle/Resources/views/Form``.

L'implémentation par défaut du fragment ``integer_widget`` ressemble à ceci :

.. configuration-block::

    .. code-block:: jinja

        {# form_div_layout.html.twig #}
        {% block integer_widget %}
            {% set type = type|default('number') %}
            {{ block('form_widget_simple') }}
        {% endblock integer_widget %}

    .. code-block:: html+php

        <!-- integer_widget.html.php -->
        <?php echo $view['form']->block($form, 'form_widget_simple', array('type' => isset($type) ? $type : "number")) ?>

Comme vous pouvez le voir, ce fragment affiche un autre fragment (``form_widget_simple``) :

.. configuration-block::

    .. code-block:: html+jinja

        {# form_div_layout.html.twig #}
        {% block form_widget_simple %}
            {% set type = type|default('text') %}
            <input type="{{ type }}" {{ block('widget_attributes') }} {% if value is not empty %}value="{{ value }}" {% endif %}/>
        {% endblock form_widget_simple %}

    .. code-block:: html+php

        <!-- FrameworkBundle/Resources/views/Form/form_widget_simple.html.php -->
        <input
            type="<?php echo isset($type) ? $view->escape($type) : 'text' ?>"
            <?php if (!empty($value)): ?>value="<?php echo $view->escape($value) ?>"<?php endif ?>
            <?php echo $view['form']->block($form, 'widget_attributes') ?>
        />

L'idée est qu'un fragment détermine le code HTML généré pour chaque partie du formulaire.
Pour personnaliser l'affichage d'un formulaire, vous devez juste identifier et surcharger
le bon fragment. Un ensemble de personnalisations de fragments d'un formulaire est appelé
« thème » de formulaire. 
Lorsque vous affichez un formulaire, pouvez choisir quel(s) thème(s) de formulaire appliquer.

Dans Twig, un thème est un unique fichier de template et les fragments sont des blocs définis
dans ce fichier.

En PHP, un thème est un répertoire et les fragments sont des fichiers de template individuels
dans ce répertoire.

.. _cookbook-form-customization-sidebar:

.. sidebar:: Savoir quel bloc personnaliser

    Dans cet exemple, le nom du fragment personnalisé est ``integer_widget`` parce
    que vous voulez surcharger le ``widget`` HTML pour tous les types de champ ``integer``.
    Si vous voulez personnaliser les champs « textarea », vous devrez personnaliser ``textarea_widget``.

    Comme vous le voyez, un nom de fragment est une combinaison du type de champ et de la
    partie du formulaire qui doit être affichée (ex ``widget``, ``label``, ``errors``, ``row``).
    En conséquence, pour personnaliser la manière dont les erreurs sont affichées pour les champs
    input ``text`` uniquement, vous devrez personnaliser le fragment ``text_errors``.

    Pourtant, bien souvent, vous voudrez personnaliser l'affichage des erreurs pour *tous*
    les champs. Vous pouvez faire cela en personnalisant le fragment ``form_errors``.
    Cette méthode tire avantage de l'héritage de type de champs. Plus précisément, puisque
    le type ``text`` étend le type ``field``, le composant formulaire cherchera d'abord le
    fragment spécifique au type (par exemple : ``text_errors``) avant de se rabattre sur le
    nom du fragment parent si le spécifique n'existe pas (par exemple : ``form_errors``).

    Pour plus d'informations sur ce sujet, lisez :ref:`form-template-blocks`.

.. _cookbook-form-theming-methods:

Thèmes de formulaire
--------------------

Pour apprécier la puissance des thèmes de formulaire, supposons que vous vouliez
encadrer chaque champ ``number`` par un ``div``. La clé pour faire cela est de
personnaliser le fragment ``integer_widget``.

Thèmes de formulaire avec Twig
------------------------------

Lorsque vous personnalisez un bloc de champ de formulaire Twig, vous avez
deux choix possibles quant à la *localisation* du bloc personnalisé :

+----------------------------------------+-------------------------------------------+-------------------------------------------+
| Méthode                                | Avantages                                 | Inconvénients                             |
+========================================+===========================================+===========================================+
| Dans le même template que le formulaire| Rapide et facile                          | Ne peut pas être réutilisé                |
+----------------------------------------+-------------------------------------------+-------------------------------------------+
| Dans un template séparé                | Peut être réutilisé par d'autres templates| Nécessite la création d'un template       |
+----------------------------------------+-------------------------------------------+-------------------------------------------+

Ces deux méthodes ont les mêmes effets mais ne sont pas aussi avantageuses
l'une que l'autre suivant les situations.

.. _cookbook-form-twig-theming-self:

Méthode 1: Dans le même template que le formulaire
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La manière la plus facile de personnaliser le bloc ``integer_widget``
est de le modifier directement dans le template qui affiche le formulaire.

.. code-block:: html+jinja

    {% extends '::base.html.twig' %}

    {% form_theme form _self %}

    {% block integer_widget %}
        <div class="integer_widget">
            {% set type = type|default('number') %}
            {{ block('form_widget_simple') }}
        </div>
    {% endblock %}

    {% block content %}
        {# affiche le formulaire #}

        {{ form_row(form.age) }}
    {% endblock %}

En utilisant la balise spéciale ``{% form_theme form _self %}``, Twig cherchera
tout bloc surchargé dans le même template. En supposant que le champ ``form.age``
soit du type de champ ``integer``, lorsque le widget sera affiché, le bloc
personnalisé ``integer_widget`` sera utilisé.

L'inconvénient de cette méthode est que les blocs de formulaire personnalisés
ne peuvent pas être réutilisés pour afficher d'autres formulaires dans d'autres
templates. En d'autres termes, cette méthode est spécialement utile pour faire
des changements applicables à un formulaire spécifique de votre application. Si vous
voulez réutiliser vos personnalisations pour certains (ou tous les) autres formulaires,
lisez la section suivante.

.. _cookbook-form-twig-separate-template:

Méthode 2: Dans un template séparé
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez également choisir de mettre le bloc de formulaire personnalisé
``integer_widget`` dans un template séparé. Le code et le résultat final
seront les mêmes, mais vous pourrez alors réutiliser cette personnalisation
de formulaire dans d'autres templates :

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Form/fields.html.twig #}
    {% block integer_widget %}
        <div class="integer_widget">
            {% set type = type|default('number') %}
            {{ block('form_widget_simple') }}
        </div>
    {% endblock %}

Maintenant que vous avez créé le bloc de formulaire personnalisé, vous devez
dire à Symfony de l'utiliser. Dans le template où vous affichez votre formulaire,
indiquez à Symfony d'utiliser votre template via la balise ``form_theme`` :

.. _cookbook-form-twig-theme-import-template:

.. code-block:: html+jinja

    {% form_theme form 'AcmeDemoBundle:Form:fields.html.twig' %}

    {{ form_widget(form.age) }}

Pour afficher le widget ``form.age``, Symfony utilisera le bloc de formulaire
``integer_widget`` du nouveau template et la balise ``input`` sera encadrée par
l'élément ``div`` comme vous l'avez spécifié dans le bloc personnalisé.

.. _cookbook-form-php-theming:

Thème de formulaire en PHP
--------------------------

Si vous utilisez PHP comme moteur de template, la seule méthode pour personnaliser
un fragment est de créer un nouveau fichier de template, ce qui est équivalent à la
seconde méthode utilisée par Twig.

Le fichier de template doit être nommé en fonction du fragment. Vous devez créer un fichier
``integer_widget.html.php`` pour personnaliser le fragment ``integer_widget``.

.. code-block:: html+php

    <!-- src/Acme/DemoBundle/Resources/views/Form/integer_widget.html.php -->
    <div class="integer_widget">
        <?php echo $view['form']->block($form, 'form_widget_simple', array('type' => isset($type) ? $type : "number")) ?>
    </div>

Maintenant que vous avez créé le template de formulaire personnalisé, vous devez
dire à Symfony de l'utiliser. Dans le template où vous affichez votre formulaire,
indiquez à Symfony d'utiliser le thème via la méthode ``setTheme`` :

.. _cookbook-form-php-theme-import-template:

.. code-block:: php

    <?php $view['form']->setTheme($form, array('AcmeDemoBundle:Form')) ;?>

    <?php $view['form']->widget($form['age']) ?>

Pour afficher le widget ``form.age``, Symfony utilisera le template personnalisé
``integer_widget.html.php`` et la balise ``input`` sera encadrée par l'élément ``div``.

.. _cookbook-form-twig-import-base-blocks:

Faire référence aux blocs par défaut d'un formulaire (spécifique à Twig)
------------------------------------------------------------------------

Jusqu'à présent, pour surcharger un bloc de formulaire particulier, la meilleure
méthode consistait à copier le bloc par défaut depuis `form_div_layout.html.twig`_,
le copier dans un nouveau template, et le personnaliser. Dans la plupart des cas, vous
pouvez éviter cela en référençant le bloc de base lorsque vous le personnalisez.

C'est facile à faire, mais cela peut varier sensiblement si vos personnalisations de bloc
sont dans le même template que le formulaire ou pas.

Faire référence aux blocs depuis le même template que le formulaire
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Importer les blocs en ajoutant la balise ``use`` dans le template où vous affichez
le formulaire :

.. code-block:: jinja

    {% use 'form_div_layout.html.twig' with integer_widget as base_integer_widget %}

A partir de maintenant, lorsque les blocs sont importés depuis `form_div_layout.html.twig`_ ,
le bloc ``integer_widget`` sera renommé en ``base_integer_widget``. Cela signifie que
lorsque vous redéfinissez le bloc ``integer_widget``, vous pouvez faire référence
à l'implémentation par défaut via ``base_integer_widget`` :

.. code-block:: html+jinja

    {% block integer_widget %}
        <div class="integer_widget">
            {{ block('base_integer_widget') }}
        </div>
    {% endblock %}

Faire référence aux blocs par défaut depuis un template externe
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vos personnalisations de formulaire se trouvent dans un template
externe, vous pouvez faire référence au bloc par défaut en utilisant
la fonction Twig ``parent()`` :

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Form/fields.html.twig #}
    {% extends 'form_div_layout.html.twig' %}

    {% block integer_widget %}
        <div class="integer_widget">
            {{ parent() }}
        </div>
    {% endblock %}

.. note::

    Il n'est pas possible de faire référence au bloc par défaut si vous
    utilisez PHP comme moteur de template. Vous devrez alors copier manuellement
    le code du bloc par défaut dans votre nouveau fichier de template.

.. _cookbook-form-global-theming:

Faire des personnalisations au niveau de l'application
------------------------------------------------------

Si vous aimeriez qu'une personnalisation de formulaire soit globale à votre
application, vous pouvez faire cela en réalisant cette personnalisation dans
un template externe, et en l'important dans la configuration de votre
application :

Twig
~~~~

En utilisant la configuration suivante, toute personnalisation de blocs de
formulaire qui se trouve dans le template ``AcmeDemoBundle:Form:fields.html.twig``
sera utilisée quand un formulaire sera affiché.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        twig:
            form:
                resources:
                    - 'AcmeDemoBundle:Form:fields.html.twig'
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <twig:config ...>
                <twig:form>
                    <resource>AcmeDemoBundle:Form:fields.html.twig</resource>
                </twig:form>
                <!-- ... -->
        </twig:config>

    .. code-block:: php

        // app/config/config.php

        $container->loadFromExtension('twig', array(
            'form' => array('resources' => array(
                'AcmeDemoBundle:Form:fields.html.twig',
             ))
            // ...
        ));

Par défaut, Twig utilise un layout à base de *div* pour afficher les formulaires.
Cependant, certaines personnes préféreront utiliser un layout à base de *tableau*.
Utilisez la ressource ``form_table_layout.html.twig`` pour utiliser un tel layout :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            form:
                resources: ['form_table_layout.html.twig']
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <twig:config ...>
                <twig:form>
                    <resource>form_table_layout.html.twig</resource>
                </twig:form>
                <!-- ... -->
        </twig:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array(
            'form' => array('resources' => array(
                'form_table_layout.html.twig',
             )),
             ...,
        ));

Si vous ne voulez appliquer ce changement que dans un seul template, ajoutez
la ligne suivante dans votre fichier de template plutôt que d'ajouter le template
comme ressource :

.. code-block:: html+jinja

    {% form_theme form 'form_table_layout.html.twig' %}

Notez que la variable ``form`` dans le code ci-dessus est la vue du formulaire
que vous avez passée à votre template.

PHP
~~~

En utilisant la configuration suivante, tout fragment de formulaire personnalisé
se situant dans le répertoire ``src/Acme/DemoBundle/Resources/views/Form`` sera
utilisé lorsque le formulaire sera affiché.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            templating:
                form:
                    resources:
                        - 'AcmeDemoBundle:Form'
            # ...


    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config ...>
            <framework:templating>
                <framework:form>
                    <resource>AcmeDemoBundle:Form</resource>
                </framework:form>
            </framework:templating>
            <!-- ... -->
        </framework:config>


    .. code-block:: php

        // app/config/config.php
        // PHP
        $container->loadFromExtension('framework', array(
            'templating' => array('form' =>
                array('resources' => array(
                    'AcmeDemoBundle:Form',
             ))),
             ...,
        ));

Par défaut, le moteur de template PHP utilise un layout à base de *div* pour afficher
les formulaires. Néanmoins, certains préféreront afficher leurs formulaires dans un layout
à base de *tableau*. Utilisez la ressource ``FrameworkBundle:FormTable`` pour utiliser un tel
layout :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            templating:
                form:
                    resources:
                        - 'FrameworkBundle:FormTable'

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config ...>
            <framework:templating>
                <framework:form>
                    <resource>FrameworkBundle:FormTable</resource>
                </framework:form>
            </framework:templating>
            <!-- ... -->
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'templating' => array('form' =>
                array('resources' => array(
                    'FrameworkBundle:FormTable',
             ))),
             ...,
        ));

Si vous ne voulez appliquer vos changements que dans un seul template, ajoutez
la ligne suivante dans votre fichier de template plutôt que d'ajouter le template
comme ressource :

.. code-block:: html+php

    <?php $view['form']->setTheme($form, array('FrameworkBundle:FormTable')); ?>

Notez que la variable ``form`` dans le code ci-dessus est la vue du formulaire
que vous avez passée à votre template.

Comment personnaliser un champ individuel
-----------------------------------------

Jusqu'ici, vous avez vu les différentes manières de personnaliser le widget
généré pour tous les types de champ texte. Vous pouvez également personnaliser
un champ individuel. Par exemple, supposons que vous ayez deux champs ``texte``
(``nom`` et ``prénom``) mais que vous vouliez seulement personnaliser
l'un de ces deux champs. Cela peut être fait en personnalisant un fragment dont le
nom est une combinaison de l'attribut « id » du champ et de la partie du champ concerné
(libellé, widget ou erreur). Par exemple :

.. configuration-block::

    .. code-block:: html+jinja

        {% form_theme form _self %}

        {% block _product_name_widget %}
            <div class="text_widget">
                {{ block('form_widget_simple') }}
            </div>
        {% endblock %}

        {{ form_widget(form.name) }}

    .. code-block:: html+php

        <!-- Template principal -->

        <?php echo $view['form']->setTheme($form, array('AcmeDemoBundle:Form')); ?>

        <?php echo $view['form']->widget($form['name']); ?>

        <!-- src/Acme/DemoBundle/Resources/views/Form/_product_name_widget.html.php -->

        <div class="text_widget">
              echo $view['form']->block('form_widget_simple') ?>
        </div>

Ici, le fragment ``_product_name_widget`` définit le template à utiliser pour le widget
du champ dont l'*id* est ``product_name`` (et dont le nom est ``product[name]``).

.. tip::

   La partie ``product`` du champ est le nom du formulaire qui peut être défini
   manuellement ou automatiquement généré en se basant sur le nom du type de
   formulaire (par exemple : ``ProductType`` donnera ``product``). Si vous n'êtes
   pas sûr du nom de votre formulaire, regardez le code source généré de votre
   formulaire.

Vous pouvez aussi surcharger le code d'un champ entier en utilisant la même méthode :

.. configuration-block::

    .. code-block:: html+jinja

        {# _product_name_row.html.twig #}
        {% form_theme form _self %}

        {% block _product_name_row %}
            <div class="name_row">
                {{ form_label(form) }}
                {{ form_errors(form) }}
                {{ form_widget(form) }}
            </div>
        {% endblock %}

    .. code-block:: html+php

        <!-- _product_name_row.html.php -->

        <div class="name_row">
            <?php echo $view['form']->label($form) ?>
            <?php echo $view['form']->errors($form) ?>
            <?php echo $view['form']->widget($form) ?>
        </div>

Autres personnalisations courantes
----------------------------------

Jusqu'à présent, ce document vous a expliqué différentes manières de personnaliser
une unique partie de l'affichage d'un formulaire. L'idée est de personnaliser un fragment
spécifique qui correspond à une partie du formulaire que vous voulez contrôler (lisez
:ref:`nommer les blocs de formulaire<cookbook-form-customization-sidebar>`).

Dans les sections suivantes, vous verrez comment effectuer plusieurs personnalisations
de formulaires communes. Pour appliquer ces personnalisations, utilisez l'une des méthodes
décrites dans la section :ref:`cookbook-form-theming-methods`.

Personnaliser l'affichage des erreurs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. note::

   Le composant formulaire ne prend en charge que la *manière* dont les erreurs
   de validation sont affichées, et non les messages d'erreurs eux-mêmes. Les
   messages d'erreurs sont déterminés par les contraintes de validation que vous
   appliquez à vos objets. Pour plus d'informations, lisez le chapitre sur la
   :doc:`validation</book/validation>`.

Il y a plusieurs manières différentes de personnaliser l'affichage des erreurs
lorsqu'un formulaire soumis n'est pas valide. Les messages d'erreur d'un champ
sont affichés lorsque vous utilisez le « helper » ``form_errors`` :

.. configuration-block::

    .. code-block:: jinja

        {{ form_errors(form.age) }}

    .. code-block:: php

        <?php echo $view['form']->errors($form['age']); ?>

Par défaut, les erreurs sont affichées dans une liste non-ordonnée :

.. code-block:: html

    <ul>
        <li>Ce champ est obligatoire</li>
    </ul>

Pour surcharger l'affichage des erreurs pour *tous* les champs, il vous
suffit de copier, coller et personnaliser le fragment ``form_errors``.

.. configuration-block::

    .. code-block:: html+jinja

        {# fields_errors.html.twig #}
        {% block form_errors %}
            {% spaceless %}
                {% if errors|length > 0 %}
                <ul class="error_list">
                    {% for error in errors %}
                        <li>{{
                            error.messagePluralization is null
                                ? error.messageTemplate|trans(error.messageParameters, 'validators')
                                : error.messageTemplate|transchoice(error.messagePluralization, error.messageParameters, 'validators') 
                        }}</li>
                    {% endfor %}
                </ul>
                {% endif %}
            {% endspaceless %}
        {% endblock form_errors %}

    .. code-block:: html+php

        <!-- fields_errors.html.php -->

        <?php if ($errors): ?>
            <ul class="error_list">
                <?php foreach ($errors as $error): ?>
                    <li><?php
                        if (null === $error->getMessagePluralization()) {
                            echo $view['translator']->trans(
                                $error->getMessageTemplate(),
                                $error->getMessageParameters(),
                                'validators'
                           );
                        } else {
                            echo $view['translator']->transChoice(
                                $error->getMessageTemplate(),
                                $error->getMessagePluralization(),
                                $error->getMessageParameters(),
                                'validators'
                            );
                        }?></li>
                <?php endforeach; ?>
            </ul>
        <?php endif ?>

.. tip::

    Lisez :ref:`cookbook-form-theming-methods` pour savoir comment appliquer ces personnalisations.

Vous pouvez aussi personnaliser l'affichage des erreurs pour un seul type
de champ spécifique. Par exemple, certaines erreurs qui sont plus globales
(c'est-à-dire qui ne sont pas spécifiques à un seul champ) sont affichées
séparément, souvent en haut de votre formulaire :

.. configuration-block::

    .. code-block:: jinja

        {{ form_errors(form) }}

    .. code-block:: php

        <?php echo $view['form']->render($form); ?>

Pour personnaliser *uniquement* le rendu de ces erreurs, suivez les mêmes directives
que ci-dessus, sauf que vous devez maintenant appeler le bloc ``form_errors`` (Twig)
ou le fichier ``form_errors.html.php`` (PHP). Maintenant, lorsque les erreurs du
type ``form`` seront affichées, votre fragment personnalisé sera utilisé au lieu du
fragment par défaut ``form_errors``.

Personnaliser le « Form Row »
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quand vous pouvez le faire, la manière la plus simple d'afficher un champ
de formulaire est la fonction ``form_row``, qui affiche le libellé, les erreurs
et le widget HTML d'un champ. Pour personnaliser le code généré utilisé pour afficher
*tous* les champs de formulaire, surchargez le fragment ``form_row``. Par exemple,
supposons que vous vouliez ajouter une classe à l'élément ``div`` qui entoure chaque
bloc :

.. configuration-block::

    .. code-block:: html+jinja

        {# form_row.html.twig #}
        {% block form_row %}
            <div class="form_row">
                {{ form_label(form) }}
                {{ form_errors(form) }}
                {{ form_widget(form) }}
            </div>
        {% endblock form_row %}

    .. code-block:: html+php

        <!-- fform_row.html.php -->
        <div class="form_row">
            <?php echo $view['form']->label($form) ?>
            <?php echo $view['form']->errors($form) ?>
            <?php echo $view['form']->widget($form) ?>
        </div>

.. tip::

    Lisez :ref:`cookbook-form-theming-methods` pour savoir comment appliquer cette personnalisation.

Ajouter une astérisque « obligatoire » sur les libellés de champs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous voulez marquer tous les champs obligatoires par une astérisque (``*``),
vous pouvez le faire en personnalisant le fragment ``form_label``.

Avec Twig, si vous faites cette personnalisation dans le même template que votre
formulaire, modifiez le tag ``use`` et ajoutez ce qui suit :

.. code-block:: html+jinja

    {% use 'form_div_layout.html.twig' with form_label as base_form_label %}

    {% block form_label %}
        {{ block('base_form_label') }}

        {% if required %}
            <span class="required" title="Ce champ est obligatoire">*</span>
        {% endif %}
    {% endblock %}

Avec Twig, si vous faites les changements dans un template séparé, ajoutez
ce qui suit :

.. code-block:: html+jinja

    {% extends 'form_div_layout.html.twig' %}

    {% block form_label %}
        {{ parent() }}

        {% if required %}
            <span class="required" title="Ce champ est obligatoire">*</span>
        {% endif %}
    {% endblock %}

Si vous utilisez PHP comme moteur de template, vous devrez copier le contenu
depuis le template original :

.. code-block:: html+php

    <!-- form_label.html.php -->

    <!-- contenu original -->  
    <?php if ($required) { $label_attr['class'] = trim((isset($label_attr['class']) ? $label_attr['class'] : '').' required'); } ?>
    <?php if (!$compound) { $label_attr['for'] = $id; } ?>
    <?php if (!$label) { $label = $view['form']->humanize($name); } ?>
    <label <?php foreach ($label_attr as $k => $v) { printf('%s="%s" ', $view->escape($k), $view->escape($v)); } ?>><?php echo $view->escape($view['translator']->trans($label, array(), $translation_domain)) ?></label>
 

    <!-- personnalisation -->
    <?php if ($required) : ?>
        <span class="required" title="Ce champ est obligatoire">*</span>
    <?php endif ?>

.. tip::

    Lisez :ref:`cookbook-form-theming-methods` pour savoir comment appliquer cette personnalisation.

Ajouter des messages d'« aide »
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez aussi personnaliser vos widgets de formulaire avec un message
d'« aide » (« help » en anglais) facultatif.

Avec Twig, si vous faites les changements dans le même template que votre
formulaire, modifiez le tag ``use`` et ajoutez ce qui suit :

.. code-block:: html+jinja

    {% use 'form_div_layout.html.twig' with form_widget_simple as base_form_widget_simple %}

    {% block form_widget_simple %}
        {{ block('base_form_widget_simple') }}

        {% if help is defined %}
            <span class="help">{{ help }}</span>
        {% endif %}
    {% endblock %}

Avec Twig, si vous faites les changements dans un template séparé, procédez comme suit :

.. code-block:: html+jinja

    {% extends 'form_div_layout.html.twig' %}

    {% block form_widget_simple %}
        {{ parent() }}

        {% if help is defined %}
            <span class="help">{{ help }}</span>
        {% endif %}
    {% endblock %}

Si vous utilisez PHP comme moteur de template, vous devrez copier le contenu
depuis le template original :

.. code-block:: html+php

    <!-- form_widget_simple.html.php -->

    <!-- Contenu original -->
    <input
        type="<?php echo isset($type) ? $view->escape($type) : 'text' ?>"
        <?php if (!empty($value)): ?>value="<?php echo $view->escape($value) ?>"<?php endif ?>
        <?php echo $view['form']->block($form, 'widget_attributes') ?>
    />

    <!-- Personnalisation -->
    <?php if (isset($help)) : ?>
        <span class="help"><?php echo $view->escape($help) ?></span>
    <?php endif ?>

Pour afficher un message d'aide en dessous du champ, passez le dans
une variable ``help`` :

.. configuration-block::

    .. code-block:: jinja

        {{ form_widget(form.title, {'help': 'foobar'}) }}

    .. code-block:: php

        <?php echo $view['form']->widget($form['title'], array('help' => 'foobar')) ?>

.. tip::

    Lisez :ref:`cookbook-form-theming-methods` pour savoir comment appliquer ces personnalisations.

Utiliser les Variables
----------------------

La plupart des fonctions disponibles pour afficher les différents parties
d'un formulaire (ex widget, label, erreurs, etc) vous permettent également
de réaliser certaines personnalisations directement. Jetez un oeil à l'exemple
suivant :

.. configuration-block::

    .. code-block:: jinja

        {# affiche un widget, mais y ajoute la classe "foo" #}
        {{ form_widget(form.name, { 'attr': {'class': 'foo'} }) }}

    .. code-block:: php

        <!-- affiche un widget, mais y ajoute la classe "foo" -->
        <?php echo $view['form']->widget($form['name'], array('attr' => array(
            'class' => 'foo',
        ))) ?>

Le tableau passé comme second argument contient des « variables » de formulaire.
Pour plus de détails sur ce concept de Twig, lisez :ref:`twig-reference-form-variables`.

.. _`form_div_layout.html.twig`: https://github.com/symfony/symfony/blob/2.1/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_layout.html.twig
