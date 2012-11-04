.. index::
   single: Forms; Fields; collection

Type de champ Collection
========================

Ce type de champ est utilisé pour rendre une « collection » de champs ou de formulaires.
Plus simplement, ce pourrait être un tableau de champs ``text`` qui remplirait un tableau
de champs``emails``. 
Dans les cas les plus complexes, vous pourrez imbriquer des formulaires entiers, ce qui
est très utile lorsque vous créerez des formulaires avec des relations one-to-many
(par exemple un produit pour lequel vous pouvez gérer plusieurs photos de ce produit).

+-------------+-----------------------------------------------------------------------------+
| Rendu comme | dépend de l'option `type`_                                                  |
+-------------+-----------------------------------------------------------------------------+
| Options     | - `type`_                                                                   |
|             | - `options`_                                                                |
|             | - `allow_add`_                                                              |
|             | - `allow_delete`_                                                           |
|             | - `prototype`_                                                              |
|             | - `prototype_name`_                                                         |
+-------------+-----------------------------------------------------------------------------+
| Options     | - `label`_                                                                  |
| héritées    | - `error_bubbling`_                                                         |
|             | - `by_reference`_                                                           |
+-------------+-----------------------------------------------------------------------------+
| Type Parent | :doc:`form</reference/forms/types/form>`                                    |
+-------------+-----------------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CollectionType`    |
+-------------+-----------------------------------------------------------------------------+

Utilisation basique
-------------------

Ce type est utilisé quand vous voulez gérer une collection d'items similaires dans 
un formulaire. Par exemple, supposez que vous avez un champ ``emails`` qui correspond
à un tableau d'adresses email. Dans le formulaire, vous voudrez afficher chaque adresse
email dans son propre champ input::

    $builder->add('emails', 'collection', array(
        // chaque item du tableau sera un champ « email »
        'type'   => 'email',
        // ces options sont passées à chaque type « email »
        'options'  => array(
            'required'  => false,
            'attr'      => array('class' => 'email-box')
        ),
    ));

La façon la plus simple de rendre ces champs est de tout faire en un coup :

.. configuration-block::

    .. code-block:: jinja
    
        {{ form_row(form.emails) }}

    .. code-block:: php
    
        <?php echo $view['form']->row($form['emails]) ?>

Une méthode plus flexible pourrait ressembler à ceci :

.. configuration-block::

    .. code-block:: html+jinja
    
        {{ form_label(form.emails) }}
        {{ form_errors(form.emails) }}

        <ul>
        {% for emailField in form.emails %}
            <li>
                {{ form_errors(emailField) }}
                {{ form_widget(emailField) }}
            </li>
        {% endfor %}
        </ul>

    .. code-block:: html+php

        <?php echo $view['form']->label($form['emails']) ?>
        <?php echo $view['form']->errors($form['emails']) ?>
        
        <ul>
        {% for emailField in form.emails %}
        <?php foreach ($form['emails'] as $emailField): ?>
            <li>
                <?php echo $view['form']->errors($emailField) ?>
                <?php echo $view['form']->widget($emailField) ?>
            </li>
        <?php endforeach; ?>
        </ul>

Dans les deux cas, aucun champ input ne sera affiché tant que le tableau ``emails``
ne contient aucune adresse email.

Dans cet exemple simple, il est toujours impossible d'ajouter de nouvelles adresses
ou d'en supprimer. Ajouter une nouvelle adresse est possible en utilisant l'option
`allow_add`_ (et optionnellement l'option `prototype`_) (voir l'exemple ci-dessous). 
Supprimer des emails du tableau ``emails`` est possible grâce à l'option `allow_delete`_.

Ajouter et supprimer des items
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si `allow_add`_ est défini à ``true``, alors tout item non reconnu qui est envoyé
sera ajouté dans le tableau de façon transparente. C'est génial en théorie, mais en
pratique, il vous faudra plus d'efforts pour adapter la partie client JavaScript.

En poursuivant avec l'exemple précédent, supposez que vous commencez avec deux emails
dans votre tableau de données ``emails``.  Dans ce cas, deux champs input seront rendus,
ce qui ressemblera à quelque chose comme ceci (ça dépendra du nom de votre formulaire) :

.. code-block:: html

    <input type="email" id="form_emails_0" name="form[emails][0]" value="foo@foo.com" />
    <input type="email" id="form_emails_1" name="form[emails][1]" value="bar@bar.com" />

Pour autoriser votre utilisateur à ajouter un autre email, définissez juste `allow_add`_
à ``true`` et, en JavaScript, rendez un autre champ avec le nom ``form[emails][2]``
(en incrémentant à chaque nouveau champ).

Pour vous aider à faire cela plus facilement, définir l'option `prototype`_ à ``true`` 
permet de rendre un « template » de champ que vous pourrez utiliser dans votre fichier
JavaScript afin de créer dynamiquement des nouveaux champs. Un champ prototype rendu
ressemblera à :

.. code-block:: html

    <input type="email" id="form_emails___name__" name="form[emails][__name__]" value="" />

En remplaçant ``__name__`` par une valeur unique (ex: ``2``), vous pouvez construire
et insérer des nouveaux champs HTML dans votre formulaire.

Si vous utilisez jQuery, un exemple simple pourrait ressembler à ceci. Si vous rendez
votre collection de champs en une seule fois (ex: ``form_row(form.emails)``), alors
les choses sont encore plus simples puisque l'attribut ``data-prototype`` est automatiquement
rendu pour vous (avec une légère différence - voir la note ci-dessous) et vous n'avez
besoin que du JavaScript :

.. configuration-block::

    .. code-block:: html+jinja
    
        <form action="..." method="POST" {{ form_enctype(form) }}>
            {# ... #}

            {# stocke le prototype dans l'attribut data-prototype #}
            <ul id="email-fields-list" data-prototype="{{ form_widget(form.emails.vars.prototype) | e }}">
            {% for emailField in form.emails %}
                <li>
                    {{ form_errors(emailField) }}
                    {{ form_widget(emailField) }}
                </li>
            {% endfor %}
            </ul>
        
            <a href="#" id="add-another-email">Add another email</a>
        
            {# ... #}
        </form>

        <script type="text/javascript">
            // garde une trace du nombre de champs email qui ont été affichés
            var emailCount = '{{ form.emails | length }}';

            jQuery(document).ready(function() {
                jQuery('#add-another-email').click(function() {
                    var emailList = jQuery('#email-fields-list');

                    // parcourt le template prototype
                    var newWidget = emailList.attr('data-prototype');
                    // remplace les "__name__" utilisés dans l'id et le nom du prototype
                    // par un nombre unique pour chaque email
                    // le nom de l'attribut final ressemblera à name="contact[emails][2]"
                    newWidget = newWidget.replace(/__name__/g, emailCount);
                    emailCount++;

                    // créer une nouvelle liste d'éléments et l'ajoute à notre liste
                    var newLi = jQuery('<li></li>').html(newWidget);
                    newLi.appendTo(jQuery('#email-fields-list'));

                    return false;
                });
            })
        </script>

.. tip::

    Si vous rendez une collection entière en une seule fois, alors le prototype
    est automatiquement disponible dans l'attribut ``data-prototype`` de l'élément
    (ex: ``div`` ou ``table``) qui encadre votre collection. La seule différence
    c'est que le « form row » est rendu pour vous en entier, ce qui signifie que vous
    n'aurez pas à l'encadrer dans un conteneur quelconque comme nous l'avions fait
    ci-dessus.

Options du champ
----------------

type
~~~~

**type**: ``string`` ou :class:`Symfony\\Component\\Form\\FormTypeInterface` **required**

C'est le type de champ pour chaque item dans la collection (ex: ``text``, ``choice``,
etc). Par exemple, si vous avez un tableau d'adresses email, vous utiliserez le type 
:doc:`email</reference/forms/types/email>`. Si vous voulez imbriquer une collection
d'un autre formulaire, créez une nouvelle instance de votre type de formulaire et passez
la dans cette option.

options
~~~~~~~

**type**: ``array`` **default**: ``array()``

C'est le tableau qui est passé au type formulaire spécifié dans l'option `type`_.
Par exemple, si vous utilisez le type :doc:`choice</reference/forms/types/choice>`
dans l'option `type`_ (pa exemple pour une collection de menus déroulants), alors
vous devrez au moins passer l'option ``choices`` au type sous-jacent::

    $builder->add('favorite_cities', 'collection', array(
        'type'   => 'choice',
        'options'  => array(
            'choices'  => array(
                'nashville' => 'Nashville',
                'paris'     => 'Paris',
                'berlin'    => 'Berlin',
                'london'    => 'London',
            ),
        ),
    ));

allow_add
~~~~~~~~~

**type**: ``Boolean`` **default**: ``false``

Si cette option est définie à ``true``, alors tout item non reconnu qui est soumis
sera ajouté à la collection comme nouvel item. Le tableau final contiendra les items
existants ainsi que les nouveaux items qui auront été soumis. Regardez l'exemple
ci-dessus pour plus de détails.

L'option `prototype`_ peut être utilisée pour rendre un prototype d'item qui pourra être
utilisé - en JavaScript - pour créer des nouveaux items de formulaire dynamiquement
côté client. Pour plus d'informations, voyez l'exemple ci-dessus et :ref:`cookbook-form-collections-new-prototype`.

.. caution::
    Si vous imbriquez des autres formulaires entiers pour prendre en compte une
    relation one-to-many en base de données, vous devrez vous assurer manuellement
    que la clé étrangère de ces nouveaux objets est correctement définie. Si vous
    utilisez Doctrine, ce ne sera pas fait automatiquement. Voyez le lien ci-dessus
    pour plus de détails.

allow_delete
~~~~~~~~~~~~

**type**: ``Boolean`` **default**: ``false``

Si cette option est définie à ``true``, alors si un item existant ne se retrouve
pas dans les données soumises, il sera supprimé du tableau d'items final. Cela signifie
que vous pouvez implémenter un bouton « Supprimer » en JavaScript qui supprimera
simplement un élément formulaire du DOM. Quand l'utilisateur soumettra le formulaire,
l'absence de cet élément des données soumises entraînera la suppression de l'item
dans le tableau final.

Pour plus d'informations, lisez :ref:`cookbook-form-collections-remove`.

.. caution::

    Soyez prudents lorsque vous utilisez cette option en imbriquant une collection
    d'objets. Dans ce cas, si un formulaire imbriqué est supprimé, il *sera* bien
    retiré du tableau final d'objets. Pourtant, selon la logique de votre application,
    lorsque l'un de ces objets est supprimé, vous voudrez probablement supprimer,
    ou au moins retirer, les clé étrangères qui lient cet objet à l'objet principal.
    Rien de tout cela n'est fait automatiquement. Pour plus d'informations, lisez
    :ref:`cookbook-form-collections-remove`.

prototype
~~~~~~~~~

**type**: ``Boolean`` **default**: ``true``

Cette option est utile lorsqu'elle est associée à l'option `allow_add`_. Si elle
est à ``true`` (et que `allow_add`_ est aussi à ``true``), un attribut spécial « prototype »
sera disponible pour que vous puissiez rendre un exemple de « template » à votre page
afin de spécifier ce à quoi le nouvel élément doit ressembler. L'attribut ``name``
donné à cet élément est ``__name__``. Cela vous permet d'ajouter un bouton « Ajouter un
élément » en JavaScript qui parcourt le prototype, remplace ``__name__`` par un
nom unique ou un numéro, et le rend à votre formulaire. Lors de la soumission, il sera
ajouté à votre tableau de données grâce à l'option `allow_add`_.

Le champ prototype peut être rendu via la variable ``prototype`` du champ collection :

.. configuration-block::

    .. code-block:: jinja
    
        {{ form_row(form.emails.vars.prototype) }}

    .. code-block:: php
    
        <?php echo $view['form']->row($form['emails']->getVar('prototype')) ?>

Notez que tout ce dont vous avez vraiment besoin c'est le « widget », mais selon la
manière dont vous rendez votre formulaire, utiliser le « form row » peut être plus
facile pour vous.

.. tip::
    
    Si vous rendez une entière collection de champs en une seule fois, alors le
    prototype du « form row » est automatiquement disponible dans l'attribut ``data-prototype``
    de l'élément (ex ``div`` ou ``table``) qui encadre votre collection.

Pour plus de détails sur l'utilisation de cette option, lisez l'exemple ci-dessus
ou :ref:`cookbook-form-collections-new-prototype`.

prototype_name 
~~~~~~~~~~~~~~

.. versionadded:: 2.1
   
    L'option ``prototype_name`` est un ajout de Symfony 2.1
    
**type**: ``String`` **default**: ``__name__``

Si vous avez plusieurs collections dans votre formulaire, ou pire encore,
des collections imbriquées, vous voudrez peut être changer le joker (placeholder)
pour que les jokers sous-jacents ne soient pas remplacés par la même valeur.

Options héritées
----------------

Ces options sont héritées du type :doc:`field</reference/forms/types/form>`.
Toutes les options ne sont pas listées ici, seulement celles qui s'appliquent le plus
à ce type :

.. include:: /reference/forms/types/options/label.rst.inc

error_bubbling
~~~~~~~~~~~~~~

**type**: ``Boolean`` **default**: ``true``

.. include:: /reference/forms/types/options/_error_bubbling_body.rst.inc

.. _reference-form-types-by-reference:

.. include:: /reference/forms/types/options/by_reference.rst.inc