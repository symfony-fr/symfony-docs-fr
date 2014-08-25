.. index::
    single: PropertyAccess
    single: Components; PropertyAccess

Le Composant PropertyAccess
===========================

    Le composant PropertyAccess fournit des fonctions pour lire et écrire
    depuis/dans un objet ou un tableau en une simple chaîne de caractères.

Installation
------------

Vous pouvez installer le composant de deux manières différentes :

* :doc:`Installez-le via Composer</components/using_components>` (``symfony/property-access`` on `Packagist`_);
* Utilisez le repository Git officiel (https://github.com/symfony/PropertyAccess).

Utilisation
-----------

Le point d'entrée de ce composant est la méthode factory (usine)
:method:`PropertyAccess::createPropertyAccessor<Symfony\\Component\\PropertyAccess\\PropertyAccess::createPropertyAccessor>`.
Cette factory va créer une nouvelle instance de la classe
:class:`Symfony\\Component\\PropertyAccess\\PropertyAccessor` avec
la configuration par défaut suivante ::

    use Symfony\Component\PropertyAccess\PropertyAccess;

    $accessor = PropertyAccess::createPropertyAccessor();

.. versionadded:: 2.3
    Avant Symfony 2.3, la méthode :method:`Symfony\\Component\\PropertyAccess\\PropertyAccess::createPropertyAccessor`
    était appelée ``getPropertyAccessor()``.

Lire depuis des tableaux
------------------------

Vous pouvez lire un tableau avec la méthode
:method:`PropertyAccessor::getValue<Symfony\\Component\\PropertyAccess\\PropertyAccessor::getValue>`.
Ceci est fait en utilisant la notation d'indice qui est utilisée dans PHP ::

    // ...
    $person = array(
        'first_name' => 'Wouter',
    );

    echo $accessor->getValue($person, '[first_name]'); // 'Wouter'
    echo $accessor->getValue($person, '[age]'); // null

Comme vous pouvez le voir, la méthode va retourner ``null`` si l'index
n'existe pas.

Vous pouvez également utiliser les tableaux multi dimensionnels ::

    // ...
    $persons = array(
        array(
            'first_name' => 'Wouter',
        ),
        array(
            'first_name' => 'Ryan',
        )
    );

    echo $accessor->getValue($persons, '[0][first_name]'); // 'Wouter'
    echo $accessor->getValue($persons, '[1][first_name]'); // 'Ryan'

Lire depuis des Objets
----------------------

La méthode ``getValue`` est une méthode très robuste, et vous pouvez voir
toutes ses fonctionnalités lorsque vous travaillez avec des objets.

Accéder aux propriétés publiques
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour lire depuis des propriétés, utilisez la notation "point" ::

    // ...
    $person = new Person();
    $person->firstName = 'Wouter';

    echo $accessor->getValue($person, 'firstName'); // 'Wouter'

    $child = new Person();
    $child->firstName = 'Bar';
    $person->children = array($child);

    echo $accessor->getValue($person, 'children[0].firstName'); // 'Bar'

.. caution::

    Accéder à des propriétés publiques est la dernière option utilisée par
    ``PropertyAccessor``. Il essaie d'accéder à la valeur en utilisant les
    méthodes ci-dessous avant d'utiliser directement la propriété. Par
    exemple, si vous avez une propriété publique qui possède un accesseur,
    il utilisera ce dernier.

Utiliser les accesseurs
~~~~~~~~~~~~~~~~~~~~~~~

La méthode  ``getValue`` supporte également la lecture en utilisant les
accesseurs. La méthode sera créée en utilisant les conventions de nommage
communes pour les accesseurs. Elle "camelize" le nom de la propriété (
``first_name`` devient ``FirstName``) et le préfixe par ``get``. Donc la
méthode devient ``getFirstName`` ::

    // ...
    class Person
    {
        private $firstName = 'Wouter';

        public function getFirstName()
        {
            return $this->firstName;
        }
    }

    $person = new Person();

    echo $accessor->getValue($person, 'first_name'); // 'Wouter'

Utiliser les Hassers/Issers
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Et cela ne s'arrête pas là. Si aucun getter n'a été trouvé, l'accesseur va
chercher un isser ou un hasser. Cette méthode est créé en utilisant la même
méthode décrite pour les getters, cela signifie que vous pouvez faire quelque
chose comme ça ::

    // ...
    class Person
    {
        private $author = true;
        private $children = array();

        public function isAuthor()
        {
            return $this->author;
        }

        public function hasChildren()
        {
            return 0 !== count($this->children);
        }
    }

    $person = new Person();

    if ($accessor->getValue($person, 'author')) {
        echo 'He is an author';
    }
    if ($accessor->getValue($person, 'children')) {
        echo 'He has children';
    }

Cela aura pour résultat : ``He is an author``

La méthode magique ``__get()``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La méthode ``getValue`` peut également utiliser la méthode
magique ``__get`` ::

    // ...
    class Person
    {
        private $children = array(
            'Wouter' => array(...),
        );

        public function __get($id)
        {
            return $this->children[$id];
        }
    }

    $person = new Person();

    echo $accessor->getValue($person, 'Wouter'); // array(...)

La méthode magique ``__call()``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Finalement, ``getValue`` peut utiliser la méthode magique ``__call``, mais
vous avez besoin d'activer cette fonctionnalité en utilisant la classe
:class:`Symfony\\Component\\PropertyAccess\\PropertyAccessorBuilder` ::

    // ...
    class Person
    {
        private $children = array(
            'wouter' => array(...),
        );

        public function __call($name, $args)
        {
            $property = lcfirst(substr($name, 3));
            if ('get' === substr($name, 0, 3)) {
                return isset($this->children[$property])
                    ? $this->children[$property]
                    : null;
            } elseif ('set' === substr($name, 0, 3)) {
                $value = 1 == count($args) ? $args[0] : null;
                $this->children[$property] = $value;
            }
        }
    }

    $person = new Person();

    // Activer la méthode magique __call
    $accessor = PropertyAccess::getPropertyAccessorBuilder()
        ->enableMagicCall()
        ->getPropertyAccessor();

    echo $accessor->getValue($person, 'wouter'); // array(...)

.. versionadded:: 2.3
    L'utilisation de la méthode magique ``__call()`` a été ajoutée
    dans Symfony 2.3.

.. caution::

    La fonctionnalité ``__call`` est désactivée par défaut. Vous pouvez l'activer
    en appelant ma méthode
    :method:`PropertyAccessorBuilder::enableMagicCallEnabled<Symfony\\Component\\PropertyAccess\\PropertyAccessorBuilder::enableMagicCallEnabled>`
    consultez `Activer d'autres fonctionnalités`_.

Ecrire dans des tableaux
------------------------

La classe ``PropertyAccessor`` peut faire bien plus que lire dans un tableau,
elle peut également écrire dans un tableau. Cela peut être effectué en utilisant
la méthode :method:`PropertyAccessor::setValue<Symfony\\Component\\PropertyAccess\\PropertyAccessor::setValue>` ::

    // ...
    $person = array();

    $accessor->setValue($person, '[first_name]', 'Wouter');

    echo $accessor->getValue($person, '[first_name]'); // 'Wouter'
    // ou
    // echo $person['first_name']; // 'Wouter'

Ecrire dans des objets
----------------------

La méthode ``setValue`` dispose des mêmes fonctionnalités que la méthode
``getValue``. Vous pouvez utiliser les setters (mutateurs), la méthode
magique ``__set`` ou les propriétés pour écrire des valeurs ::

    // ...
    class Person
    {
        public $firstName;
        private $lastName;
        private $children = array();

        public function setLastName($name)
        {
            $this->lastName = $name;
        }

        public function __set($property, $value)
        {
            $this->$property = $value;
        }

        // ...
    }

    $person = new Person();

    $accessor->setValue($person, 'firstName', 'Wouter');
    $accessor->setValue($person, 'lastName', 'de Jong');
    $accessor->setValue($person, 'children', array(new Person()));

    echo $person->firstName; // 'Wouter'
    echo $person->getLastName(); // 'de Jong'
    echo $person->children; // array(Person());

Vous pouvez également utiliser ``__call`` pour écrire des valeurs mais vous
avez besoin d'activer la fonctionnalités. Consultez `Activer d'autres fonctionnalités`_.

.. code-block:: php

    // ...
    class Person
    {
        private $children = array();

        public function __call($name, $args)
        {
            $property = lcfirst(substr($name, 3));
            if ('get' === substr($name, 0, 3)) {
                return isset($this->children[$property])
                    ? $this->children[$property]
                    : null;
            } elseif ('set' === substr($name, 0, 3)) {
                $value = 1 == count($args) ? $args[0] : null;
                $this->children[$property] = $value;
            }
        }

    }

    $person = new Person();

    // Activez la méthode magique __call
    $accessor = PropertyAccess::getPropertyAccessorBuilder()
        ->enableMagicCall()
        ->getPropertyAccessor();

    $accessor->setValue($person, 'wouter', array(...));

    echo $person->getWouter(); // array(...)

Mélanger les Objets et les Tableaux
-----------------------------------

Vous pouvez aussi mélanger objets et tableaux ensemble ::

    // ...
    class Person
    {
        public $firstName;
        private $children = array();

        public function setChildren($children)
        {
            $this->children = $children;
        }

        public function getChildren()
        {
            return $this->children;
        }
    }

    $person = new Person();

    $accessor->setValue($person, 'children[0]', new Person);
    // équivaut à $person->getChildren()[0] = new Person()

    $accessor->setValue($person, 'children[0].firstName', 'Wouter');
    // équivaut à $person->getChildren()[0]->firstName = 'Wouter'

    echo 'Hello '.$accessor->getValue($person, 'children[0].firstName'); // 'Wouter'
    // équivaut à $person->getChildren()[0]->firstName

Activer d'autres fonctionnalités
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La classe :class:`Symfony\\Component\\PropertyAccess\\PropertyAccessor` peut
être configurée pour des fonctionnalités supplémentaires. Pour cela, vous
pouvez utiliser la classe :class:`Symfony\\Component\\PropertyAccess\\PropertyAccessorBuilder` ::

    // ...
    $accessorBuilder = PropertyAccess::createPropertyAccessorBuilder();

    // Activer la méthode magique __call
    $accessorBuilder->enableMagicCall();

    // Désactiver la méthode magique __call
    $accessorBuilder->disableMagicCall();

    // Vérifier si la méthode magique __call est activée
    $accessorBuilder->isMagicCallEnabled() // renvoie le booléen true ou false

    // A la fin, réccupérer l'accesseur de propriété (property accessor) configuré
    $accessor = $accessorBuilder->getPropertyAccessor();

    // Ou tout en même temps
    $accessor = PropertyAccess::createPropertyAccessorBuilder()
        ->enableMagicCall()
        ->getPropertyAccessor();

Ou vous pouvez passer les paramètres directement dans le constructeur (non recommandé) ::

    // ...
    $accessor = new PropertyAccessor(true) // cela active la gestion de la méthode magique __call


.. _Packagist: https://packagist.org/packages/symfony/property-access
