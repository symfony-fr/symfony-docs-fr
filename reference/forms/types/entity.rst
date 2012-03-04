.. index::
   single: Forms; Fields; choice

Type de champ Entity
====================

Un champ ``choice`` spécial qui est conçu pour charger ses options d'une entité
Doctrine. Par exemple, si vous avez une entité ``Category``, vous pourrez utiliser
ce champ pour afficher une liste ``select`` de tout ou de certains objets ``Category``
depuis la base de données.

+-------------+----------------------------------------------------------------------+
| Rendu comme | peut être plusieurs balises (voir :ref:`forms-reference-choice-tags`)|
+-------------+----------------------------------------------------------------------+
| Options     | - `class`_                                                           |
|             | - `property`_                                                        |
|             | - `query_builder`_                                                   |
|             | - `em`_                                                              |
+-------------+----------------------------------------------------------------------+
| Options     | - `required`_                                                        |
| héritées    | - `label`_                                                           |
|             | - `multiple`_                                                        |
|             | - `expanded`_                                                        |
|             | - `preferred_choices`_                                               |
|             | - `empty_value`_                                                     |
|             | - `read_only`_                                                       |
|             | - `error_bubbling`_                                                  |
+-------------+----------------------------------------------------------------------+
| Type parent | :doc:`choice</reference/forms/types/choice>`                         |
+-------------+----------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Bridge\\Doctrine\\Form\\Type\\EntityType`           |
+-------------+----------------------------------------------------------------------+

Utilisation de base
-------------------

Le type ``entity`` n'a qu'une seule option obligatoire : l'entité qui doit être listée
dans le champ Choice::

    $builder->add('users', 'entity', array(
        'class' => 'AcmeHelloBundle:User',
    ));

Dans ce cas, tout les objets ``User`` seront chargés depuis la base de données et seront
affichés soit comme une balise ``select``, soit un ensemble de boutons radio ou de checkboxes
(cela dépendra des valeurs des options ``multiple`` et ``expanded``).

Utiliser une requête personnalisée pour les entités
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous avez besoin de spécifier une requête personnalisée à utiliser pour charger
les entitées (par exemple si vous ne voulez que certaines entités, ou que vous voulez
les classer), utilisez l'option ``query_builder``. La façon la plus simple d'utiliser
cette option est la suivante::

    use Doctrine\ORM\EntityRepository;
    // ...

    $builder->add('users', 'entity', array(
        'class' => 'AcmeHelloBundle:User',
        'query_builder' => function(EntityRepository $er) {
            return $er->createQueryBuilder('u')
                ->orderBy('u.username', 'ASC');
        },
    ));

.. include:: /reference/forms/types/options/select_how_rendered.rst.inc

Options du champ
----------------

class
~~~~~

**type**: ``string`` **obligatoire*

La classe de votre entité (ex: ``AcmeStoreBundle:Category``). Cela peut être
le nom complet de la classe (ex: ``Acme\StoreBundle\Entity\Category``) ou son alias
(voir ci-dessus).

property
~~~~~~~~

**type**: ``string``

C'est la propriété qui doit être utilisée pour afficher l'entité sous forme de
texte dans l'élément HTML. Si vous le laissez vite, l'objet entité sera converti
en texte et devra alors implémenter la méthode ``__toString()``.

query_builder
~~~~~~~~~~~~~

**type**: ``Doctrine\ORM\QueryBuilder`` or a Closure

Si elle est spécifiée, cette option sera utilisée pour requêter un sous-ensemble
d'objets (et leur classement) qui sera affiché dans le champ. La valeur de cette
option peut être soit un objet ``QueryBuilder`` soit une Closure. Si vous utilisez
une Closure, elle ne doit prendre qu'un seul argument qui est l'objet ``EntityRepository``
de l'entité.

em
~~

**type**: ``string`` **default**: le gestionnaire d'entité par défaut

Si elle est spécifiée, cette option définit le gestionnaire d'entité (entity manager)
qui sera utilisé pour charger les objets au lieu du gestionnaire par défaut.

Options héritées
----------------

Ces options sont héritées du type :doc:`choice</reference/forms/types/choice>` :

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

Ces options sont héritées du type :doc:`field</reference/forms/types/field>` :

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
