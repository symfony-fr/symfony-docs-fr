.. index::
    single: Forms; Custom Type Guesser

Créer un Type Guesser personnalisé
==================================

Le composant Form peut deviner le type et quelques options d'un champ de
formulaire en utilisant un type guesser (déduction automatique du type). Le composant
inclut déjà un type guesser qui utilise les assertions du composant
Validation, mais vous pouvez également ajouter vos propres type guessers.

.. sidebar:: Les Type Guessers dans les Bridges

    Symfony fournit également quelques type guessers de formulaire dans les bridges :

    * :class:`Symfony\\Bridge\\Propel1\\Form\\PropelTypeGuesser` fourni par
      le bridge Propel1;
    * :class:`Symfony\\Bridge\\Doctrine\\Form\\DoctrineOrmTypeGuesser`
      fourni par le bridge Doctrine.

Créer une PHPDoc pour le Type Guesser
-------------------------------------

Dans cette section, vous allez construire un guesser (devin) qui va pouvoir
lire les informations concernant les champs d'un formulaire depuis la PHPDoc
des propriétés. Premièrement, vous aurez besoin de créer une classe qui implémente
la classe :class:`Symfony\\Component\\Form\\FormTypeGuesserInterface`.
Cette interface requiert 4 méthodes :

* :method:`Symfony\\Component\\Form\\FormTypeGuesserInterface::guessType` -
  essaie de deviner le type d'un champ;
* :method:`Symfony\\Component\\Form\\FormTypeGuesserInterface::guessRequired` -
  essaie de deviner la valeur de l'option :ref:`requise <reference-form-option-required>`;
* :method:`Symfony\\Component\\Form\\FormTypeGuesserInterface::guessMaxLength` -
  essaie de deviner la valeur de l'option :ref:`max_length <reference-form-option-max_length>`;
* :method:`Symfony\\Component\\Form\\FormTypeGuesserInterface::guessPattern` -
  esssaie de deviner la valeur de l'option :ref:`pattern <reference-form-option-pattern>`.

Commencez par créer la classe et ses méthodes. Ensuite, vous apprendrez comment les implémenter.

.. code-block:: php

    namespace Acme\Form;

    use Symfony\Component\Form\FormTypeGuesserInterface;

    class PhpdocTypeGuesser implements FormTypeGuesserInterface
    {
        public function guessType($class, $property)
        {
        }

        public function guessRequired($class, $property)
        {
        }

        public function guessMaxLength($class, $property)
        {
        }

        public function guessPattern($class, $property)
        {
        }
    }

Deviner le Type
~~~~~~~~~~~~~~~

Lorsque vous devinez un type, la méthode retourne soit une instance
de :class:`Symfony\\Component\\Form\\Guess\\TypeGuess` ou rien, pour
déterminer que le type guesser ne peut pas deviner le type.

Le constructeur ``TypeGuess`` requiert 3 options :

* Le nom du type (l'un des :doc:`types de formulaire </reference/forms/types>`);
* Des options additionnelles (par exemple, lorsque le type est ``entity``,
  vous voudrez également fixer l'option ``class``). Si aucun type n'est
  deviné, cela doit être fixé avec un tableau vide;
* Le niveau de confiance (ou probabilité) indiquant que le type deviné
  est correct. Cela peut être l'une des constantes de la classe
  :class:`Symfony\\Component\\Form\\Guess\\Guess` : ``LOW_CONFIDENCE``,
  ``MEDIUM_CONFIDENCE``, ``HIGH_CONFIDENCE``, ``VERY_HIGH_CONFIDENCE``. Après
  que tous les types guessers ont été exécutés, le type avec la confiance
  la plus haute est executé.

Avec ce que vous venez d'apprendre, vous pouvez facilement implémenter la méthode
``guessType`` de ``PHPDocTypeGuesser`` ::

    namespace Acme\Form;

    use Symfony\Component\Form\Guess\Guess;
    use Symfony\Component\Form\Guess\TypeGuess;

    class PhpdocTypeGuesser implements FormTypeGuesserInterface
    {
        public function guessType($class, $property)
        {
            $annotations = $this->readPhpDocAnnotations($class, $property);

            if (!isset($annotations['var'])) {
                return; // devine que rien n'est disponible dans l'annotation @var
            }

            // le cas échéant, basez le type sur l'annotation @var
            switch ($annotations['var']) {
                case 'string':
                    // il y a une forte probabilité que le type soit une string
                    // lorsque @var string est utilisé
                    return new TypeGuess('text', array(), Guess::HIGH_CONFIDENCE);

                case 'int':
                case 'integer':
                    // les entiers peuvent également être l'id d'une entité ou une
                    // checkbox (0 ou 1)
                    return new TypeGuess('integer', array(), Guess::MEDIUM_CONFIDENCE);

                case 'float':
                case 'double':
                case 'real':
                    return new TypeGuess('number', array(), Guess::MEDIUM_CONFIDENCE);

                case 'boolean':
                case 'bool':
                    return new TypeGuess('checkbox', array(), Guess::HIGH_CONFIDENCE);

                default:
                    // il y a une très petite probabilité que celui-ci soit correct
                    return new TypeGuess('text', array(), Guess::LOW_CONFIDENCE);
            }
        }

        protected function readPhpDocAnnotations($class, $property)
        {
            $reflectionProperty = new \ReflectionProperty($class, $property);
            $phpdoc = $reflectionProperty->getDocComment();

            // parsez le $phpdoc en tableau comme :
            // array('type' => 'string', 'since' => '1.0')
            $phpdocTags = ...;

            return $phpdocTags;
        }
    }

Ce type guesser peut maintenant deviner le type du champ d'une propriété
si elle possède une PHPDoc !

Deviner les options d'un champs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les trois autres méthodes (``guessMaxLength``, ``guessRequired`` et
``guessPattern``) retournent une instance de
:class:`Symfony\\Component\\Form\\Guess\\ValueGuess` avec la valeur de
l'option. Ce constructeur possède 2 arguments :

* La valeur de l'option;
* Le niveau de probabilité que la valeur devinée soit correcte (utilisant
  les constantes de la class ``Guess``).

``null`` est deviné lorsque vous croyez que la valeur d'une option pourrait
ne pas être fixée.

.. caution::

    Vous devez être très vigilant en utilisant la méthode ``guessPattern``.
    Lorsque le type est un float, vous ne pouvez pas l'utiliser pour déterminer
    une valeur minimum ou maximum de ce float (par exemple vous voulez qu'un
    float soit plus grand que ``5``, ``4.512313`` n'est pas valide alors que
    ``length(4.512314) > length(5)`` l'est, donc le motif reussira). Dans
    ce cas, la valeur devrit être fixée à ``null`` avec une confiance
    à ``MEDIUM_CONFIDENCE``.

Enregistrer un Type Guesser
---------------------------

La dernière chose qu'il vous reste à faire est d'enregistrer votre type guesser
personnalisé en utilisant la méthode
:method:`Symfony\\Component\\Form\\FormFactoryBuilder::addTypeGuesser` ou la méthode
:method:`Symfony\\Component\\Form\\FormFactoryBuilder::addTypeGuessers`::

    use Symfony\Component\Form\Forms;
    use Acme\Form\PHPDocTypeGuesser;

    $formFactory = Forms::createFormFactoryBuilder()
        // ...
        ->addTypeGuesser(new PHPDocTypeGuesser())
        ->getFormFactory();

    // ...

.. note::

    Lorsque vous utilisez le framework Symfony, vous devez enregistrer votre type guesser
    en tant que service et le taguer avec ``form.type_guesser``. Pour plus d'informations,
    consultez :ref:`les tags supportés par le Conteneur d'Injection Dépendances <reference-dic-type_guesser>`.
