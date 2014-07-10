.. index::
    single: Syntax; ExpressionLanguage

Syntaxe des expressions
=======================

Le composant ExpressionLanguage utilise une syntaxe spécifique basée sur la syntaxe
de Twig. Dans ce document, vous trouverez toutes les syntaxes supportées.

Valeurs explicites
------------------

Le composant supporte:

* **strings** - simples et doubles quotes (ex ``'hello'``)
* **numbers** - ex ``103``
* **arrays** - notation JSON (ex ``[1, 2]``)
* **hashes** - notation JSON (ex ``{ foo: 'bar' }``)
* **booleans** - ``true`` et ``false``
* **null** - ``null``

.. _component-expression-objects:

Travailler avec les objets
--------------------------

Lorsque vous passez des objets à une expression, vous pouvez utiliser
différentes syntaxes pour accéder aux propriétés des objets et appeler
leurs méthodes.

Accéder aux propriétés publiques
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez utiliser la syntaxe ``.`` pour accéder aux propriétés publiques des objets,
comme en JavaScript::

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

Cela affichera ``Granny Smith``.

Appeler les méthodes
~~~~~~~~~~~~~~~~~~~~

Comme en JavaScript, la syntaxe ``.`` peut également être utilisée pour faire appel
aux méthodes de l'objet::

    class Robot
    {
        public function sayHi($times)
        {
            $greetings = array();
            for ($i = 0; $i < $times; $i++) {
                $greetings[] = 'Hi';
            }

            return implode(' ', $greetings).'!';
        }
    }

    $robot = new Robot();

    echo $language->evaluate(
        'robot.sayHi(3)',
        array(
            'robot' => $robot,
        )
    );

Cela affichera ``Hi Hi Hi!``.

.. _component-expression-functions:

Travailler avec les fonctions
-----------------------------

Vous pouvez aussi utiliser des fonctions dans les expressions en utilisant
la même syntaxe qu'en PHP et JavaScript. Le composant ExpressionLanguage
est fourni avec une fonction par défaut : ``constant()``, qui retournera
la valeur d'une constante PHP::

    define('DB_USER', 'root');

    echo $language->evaluate(
        'constant("DB_USER")'
    );

Cela affichera ``root``.

.. tip::

    Pour savoir comment déclarer vos propres fonctions et les utiliser dans
    les expressions, lisez ":doc:`/components/expression_language/extending`".

.. _component-expression-arrays:

Travailler avec les tableaux
----------------------------

Si vous passez un tableau dans une expression, utilisez la syntaxe ``[]``
pour accéder aux clés du tableau, comme en JavaScript::

    $data = array('vie' => 10, 'univers' => 10, 'tout' => 22);

    echo $language->evaluate(
        'data["vie"] + data["univers"] + data["tout"]',
        array(
            'data' => $data,
        )
    );

Cela affichera ``42``.

Opérateurs supportés
--------------------

Le composant est fourni avec de nombreux opérateurs :

Opérateurs arithmétiques
~~~~~~~~~~~~~~~~~~~~~~~~

* ``+`` (addition)
* ``-`` (soustraction)
* ``*`` (multiplication)
* ``/`` (division)
* ``%`` (modulo)
* ``**`` (puissance)

Par exemple::

    echo $language->evaluate(
        'vie + univers + tout',
        array(
            'vie' => 10,
            'univers' => 10,
            'tout' => 22,
        )
    );

Cela affichera ``42``.

Opérateurs bit à bit
~~~~~~~~~~~~~~~~~~~~

* ``&`` (et)
* ``|`` (ou)
* ``^`` (ou exclusif)

Opérateurs de comparaison
~~~~~~~~~~~~~~~~~~~~~~~~~

* ``==`` (égal)
* ``===`` (identique)
* ``!=`` (non égal)
* ``!==`` (non identique)
* ``<`` (inférieur à)
* ``>`` (supérieur à)
* ``<=`` (inférieur ou égal à)
* ``>=`` (supérieur ou égal à)
* ``matches`` (expression régulière)

.. tip::

    Pour tester si une chaine ne correspond *pas* à une regex, utilisez
    l'opérateur logique ``not`` combiné avec l'opérateur ``matches``::

        $language->evaluate('not ("foo" matches "/bar/")'); // retourne true

    Vous devez utiliser les parenthèses car l'opérateur unaire ``not`` prévaut sur l'opérateur
    binaire ``matches``.

Exemples::

    $ret1 = $language->evaluate(
        'vie == tout',
        array(
            'vie' => 10,
            'univers' => 10,
            'tout' => 22,
        )
    );

    $ret2 = $language->evaluate(
        'vie > tout',
        array(
            'vie' => 10,
            'univers' => 10,
            'tout' => 22,
        )
    );

Les deux variables seront définies à  ``false``.

Opérateurs logiques
~~~~~~~~~~~~~~~~~~~

* ``not`` ou ``!``
* ``and`` ou ``&&``
* ``or`` pi ``||``

Par exemple::

    $ret = $language->evaluate(
        'vie < univers or vie < tout',
        array(
            'vie' => 10,
            'univers' => 10,
            'tout' => 22,
        )
    );

Cette variable ``$ret`` sera définie à ``true``.

Opérateurs de chaines
~~~~~~~~~~~~~~~~~~~~~

* ``~`` (concaténation)

Par exemple::

    echo $language->evaluate(
        'firstName~" "~lastName',
        array(
            'firstName' => 'Arthur',
            'lastName' => 'Dent',
        )
    );

Cela affichera ``Arthur Dent``.

Opérateurs de tableaux
~~~~~~~~~~~~~~~~~~~~~~

* ``in`` (contient)
* ``not in`` (ne contient pas)

Par exemple::

    class User
    {
        public $group;
    }

    $user = new User();
    $user->group = 'human_resources';

    $inGroup = $language->evaluate(
        'user.group in ["human_resources", "marketing"]',
        array(
            'user' => $user
        )
    );

La variable ``$inGroup`` vaudra ``true``.

Opérateurs numériques
~~~~~~~~~~~~~~~~~~~~~

* ``..`` (plage)

Par exemple::

    class User
    {
        public $age;
    }

    $user = new User();
    $user->age = 34;

    $language->evaluate(
        'user.age in 18..45',
        array(
            'user' => $user,
        )
    );

Cela vaudra ``true``, car ``user.age`` est compris entre
``18`` et ``45``.

Opérateurs ternaires
~~~~~~~~~~~~~~~~~~~~

* ``foo ? 'yes' : 'no'``
* ``foo ?: 'no'`` (égal à ``foo ? foo : 'no'``)
* ``foo ? 'yes'`` (égal à ``foo ? 'yes' : ''``)