.. index::
    single: Caching; ExpressionLanguage

Cacher les expressions en utilisant les parsers de cache
========================================================

Le composant ExpressionLanguage fournit déjà une méthode
:method:`Symfony\\Component\\ExpressionLanguage\\ExpressionLanguage::compile`
pour mettre en cache des expressions en PHP. Mais, en interne, le composant
met également en cache les expressions parsées/analysées donc les expressions
dupliquées peuvent être compilées/évaluées plus vite.

Le processus
------------

Les méthodes :method:`Symfony\\Component\\ExpressionLanguage\\ExpressionLanguage::evaluate`
et ``compile()`` ont toutes les deux besoin de faire des choses avant de pouvoir retourner
des valeurs. En ce qui concerne ``evaluate()``, ce travail peut être très conséquent.

Les deux méthodes ont besoin de découper (tokenizer) et analyser (parser) les expressions.
Cela est réalisé par la méthode :method:`Symfony\\Component\\ExpressionLanguage\\ExpressionLanguage::parse`.
Elle retourne une :class:`Symfony\\Component\\ExpressionLanguage\\ParsedExpression`.
Maintenant, la méthode ``compile()`` retourne juste la conversion de cet objet en chaine
de caractères. La méthode ``evaluate()`` a besoin de boucler sur les "noeuds" (morceaux
d'une expression enregistrés dans la ``ParsedExpression``) et les évaluer à la volée.

Pour gagner du temps, l'``ExpressionLanguage`` met en cache la ``ParsedExpression``
afin d'éviter les étapes de tokenization et de parsing pour les expressions dupliquées.
La mise en cache est effectuée par une instance de
:class:`Symfony\\Component\\ExpressionLanguage\\ParserCache\\ParserCacheInterface`
(par défaut, elle utilise une :class:`Symfony\\Component\\ExpressionLanguage\\ParserCache\\ArrayParserCache`).
Vous pouvez personnaliser cela en créant votre propre ``ParserCache`` et en l'injectant
dans l'objet via le constructeur::

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage;
    use Acme\ExpressionLanguage\ParserCache\MyDatabaseParserCache;

    $cache = new MyDatabaseParserCache(...);
    $language = new ExpressionLanguage($cache);

.. note::

    Le `DoctrineBridge`_ fournit une implémentation de Parser Cache qui utilise
    la `bibliothèque de cache Doctrine`_, qui vous aide à mette en cache quelle que soit
    la stratégie de cache comme Apc, système de fichiers ou Memcached.

Utiliser les expressions parsées et sérialisées
-----------------------------------------------

Les méthodes ``evaluate()`` et ``compile()`` peuvent toutes les deux prendre en
charque une ``ParsedExpression`` ou une ``SerializedParsedExpression``::

    use Symfony\Component\ExpressionLanguage\ParsedExpression;
    // ...

    $expression = new ParsedExpression($language->parse('1 + 4'));

    echo $language->evaluate($expression); // affiche 5

.. code-block:: php

    use Symfony\Component\ExpressionLanguage\SerializedParsedExpression;
    // ...

    $expression = new SerializedParsedExpression(
        serialize($language->parse('1 + 4'))
    );

    echo $language->evaluate($expression); // affiche 5

.. _DoctrineBridge: https://github.com/symfony/DoctrineBridge
.. _`bibliothèque de cache Doctrine`: http://docs.doctrine-project.org/projects/doctrine-common/en/latest/reference/caching.html
