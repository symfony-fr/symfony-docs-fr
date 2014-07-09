.. index::
    single: Extending; ExpressionLanguage

Étendre ExpressionLanguage
================================

Vous pouvez étendre le composant ExpressionLanguage en ajoutant des fonctions personnalisées.
Par exemple, dans Symfony, la partie Sécurité possède ses propres fonctions pour vérifier les
droits des utilisateurs.

.. note::

    Si vous voulez savoir comment utiliser les fonctions dans une expression, lisez
    ":ref:`component-expression-functions`".

Déclarer des fonctions
----------------------

Les fonctions sont déclarées dans chaque instance spécifique de ``ExpressionLanguage``.
Cela signifie que les fonctions peuvent être utilisées dans n'importe quelle expression
exécutée par cette instance.

Pour déclarer une fonction, utilisez
:method:`Symfony\\Component\\ExpressionLanguage\\ExpressionLanguage::register`.
Cette méthode demande 3 arguments :

* **name** - Le nom de la fonction dans une expression;
* **compiler** - Une fonction exécutée à la compilation d'une expression qui utilise la fonction;
* **evaluator** - Une fonction exécutée lorsque l'expression est évaluée.

.. code-block:: php

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage;

    $language = new ExpressionLanguage();
    $language->register('lowercase', function ($str) {
        if (!is_string($str)) {
            return $str;
        }

        return sprintf('strtolower(%s)', $str);
    }, function ($arguments, $str) {
        if (!is_string($str)) {
            return $str;
        }

        return strtolower($str);
    });

    echo $language->evaluate('lowercase("HELLO")');

Cela affichera ``hello``. On passe une variable ``arguments`` aux deux méthodes
**compiler** et **evaluator**. Elle est égale au second argument de
``evaluate()`` ou ``compile()`` (ex "values" lors de l'évaluation ou "names" lors
de la compilation).

Créer une nouvelle classe ExpressionLanguage
--------------------------------------------

Lorsque vous utilisez la classe ``ExpressionLanguage`` dans votre bibliothèque,
il est recommandé de créer une nouvelle classe ``ExpressionLanguage`` et d'y
déclarer vos fonctions. Surchargez ``registerFunctions`` pour ajouter vos propres
fonctions::

    namespace Acme\AwesomeLib\ExpressionLanguage;

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage as BaseExpressionLanguage;

    class ExpressionLanguage extends BaseExpressionLanguage
    {
        protected function registerFunctions()
        {
            parent::registerFunctions(); // n'oubliez pas de déclarer aussi les fonctions du noyau

            $this->register('lowercase', function ($str) {
                if (!is_string($str)) {
                    return $str;
                }

                return sprintf('strtolower(%s)', $str);
            }, function ($arguments, $str) {
                if (!is_string($str)) {
                    return $str;
                }

                return strtolower($str);
            });
        }
    }