.. index::
    single: Expressions
    Single: Components; Expression Language

Le composant ExpressionLanguage
===============================

    Le composant ExpressionLanguage fournit un moteur qui peut compiler et
    évaluer des expressions. Une expression est un code simple qui retourne une valeur
    (souvent un booléen mais pas toujours).

.. versionadded:: 2.4
    Le composant ExpressionLanguage a été ajouté dans Symfony 2.4.

Installation
------------

Vous pouvez l'installer de deux manières différentes:

* :doc:`Installez-le via Composer </components/using_components>` (``symfony/expression-language`` sur `Packagist`_);
* Utilisez le dépôt Git officiel (https://github.com/symfony/expression-language).

Comment le moteur d'expression peut-il m'aider?
-----------------------------------------------

Le but du composant est de permettre à ses utilisateurs d'utiliser des
expression au sein d'une configuration qui contient une logique plus complexe.
Par exemple, le Framework Symfony2 utilise les expressions dans la sécurité, pour
la validation de règles et dans la correspondance de routes.

En plus d'utiliser le composant dans le framework lui-même, le composant
ExpressionLanguage est le parfait candidat pour les fondations d'un
*moteur de règles métier*. L'idée est de laisser le webmaster d'un site configurer
des choses de manière dynamique en utilisant PHP mais sans introduire de failles
de sécurité:

.. _component-expression-language-examples:

.. code-block:: text

    # Récupère le prix spécial si
    user.getGroup() in ['good_customers', 'collaborator']

    # Met en avant un article sur la home quand
    article.commentCount > 100 and article.category not in ["misc"]

    # Envoie une alerte quand
    product.stock < 15

Les expressions peuvent être vues comme un bac à sable PHP très limité et
immunisé contre les injections externes puisque vous déclarez explicitement
quelles variables sont disponibles dans une expression.

Usage
-----

Le composant ExpressionLanguage peut compiler et évaluer des expressions.
Une expression est un petit code simple qui retourne souvent un booléen qui
peut être utilisé dans un ``if`` par le code qui exécute l'expression. Un
exemple simple d'une expression est ``1 + 2``. Vous pouvez également utiliser
des expressions plus complexes comme ``someArray[3].someMethod('bar')``.

Le composant fournit deux méthodes pour travailler avec les expressions:

* **evaluation**: l'expression est évaluée sans être compilée en PHP;
* **compile**: l'expression est compilée en PHP, elle peut donc être mise en cache et évaluée.

La classe principale du composant est
:class:`Symfony\\Component\\ExpressionLanguage\\ExpressionLanguage`::

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage;

    $language = new ExpressionLanguage();

    echo $language->evaluate('1 + 2'); // affiche 3

    echo $language->compile('1 + 2'); // affiche (1 + 2)

Syntaxe des expressions
-----------------------

Lisez :doc:`/components/expression_language/syntax` pour en savoir plus sur
la syntaxe du composant ExpressionLanguage.

Passer des variables
--------------------

Vous pouvez également passer des variables, qui peuvent être de n'importe quel type
PHP valide (incluant les objets), dans une expression::

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage;

    $language = new ExpressionLanguage();

    class Apple
    {
        public $variety;
    }

    $apple = new Apple();
    $apple->variety = 'Granny Smith';

    echo $language->evaluate(
        'fruit.variety',
        array(
            'fruit' => $apple,
        )
    );

Cela affichera "Granny Smith". Pour plus d'informations, lisez l'article
:doc:`/components/expression_language/syntax` et plus particulièrement
:ref:`component-expression-objects` et :ref:`component-expression-arrays`.

Mise en cache
-------------

Le composant fournit différentes stratégies de mise en cache, lisez
:doc:`/components/expression_language/caching` pour en savoir plus.

.. _Packagist: https://packagist.org/packages/symfony/expression-language