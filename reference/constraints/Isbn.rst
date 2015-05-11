Isbn
====

.. versionadded:: 2.3
    C'est une nouvelle contrainte depuis la version 2.3.

Cette contrainte valide si un numéro ISBN( International Standard Book Numbers)
est soit un nombre de type ISBN-10, de type ISBN-13 ou les deux.

+----------------+----------------------------------------------------------------------+
| S'applique à   | :ref:`property or method<validation-property-target>`                |
+----------------+----------------------------------------------------------------------+
| Options        | - `isbn10`_                                                          |
|                | - `isbn13`_                                                          |
|                | - `isbn10Message`_                                                   |
|                | - `isbn13Message`_                                                   |
|                | - `bothIsbnMessage`_                                                 |
+----------------+----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Isbn`            |
+----------------+----------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\IsbnValidator`   |
+----------------+----------------------------------------------------------------------+

Utilisation de base
-------------------

Pour utiliser un validateur ``Isbn``, simplement appliquez à une propriété, une méthode,
ou à un objet qui contient un numéro ISBN.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BookcaseBunlde/Resources/config/validation.yml
        Acme\BookcaseBunlde\Entity\Book:
            properties:
                isbn:
                    - Isbn:
                        isbn10: true
                        isbn13: true
                        bothIsbnMessage: Cette valeur n'est pas de type ISBN-10 et/ou de type ISBN-13.

    .. code-block:: php-annotations

        // src/Acme/BookcaseBunlde/Entity/Book.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Book
        {
            /**
             * @Assert\Isbn(
             *     isbn10 = true,
             *     isbn13 = true,
             *     bothIsbnMessage = "Cette valeur n'est pas de type ISBN-10 et/ou de type ISBN-13."
             * )
             */
            protected $isbn;
        }

    .. code-block:: xml

        <!-- src/Acme/BookcaseBunlde/Resources/config/validation.xml -->
        <class name="Acme\BookcaseBunlde\Entity\Book">
            <property name="isbn">
                <constraint name="Isbn">
                    <option name="isbn10">true</option>
                    <option name="isbn13">true</option>
                    <option name="bothIsbnMessage">Cette valeur n'est pas de type ISBN-10 et/ou de type ISBN-13.</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BookcaseBunlde/Entity/Book.php
        namespace Acme\BookcaseBunlde\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Book
        {
            protected $isbn;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('isbn', new Assert\Isbn(array(
                    'isbn10'          => true,
                    'isbn13'          => true,
                    'bothIsbnMessage' => 'Cette valeur n'est pas de type ISBN-10 et/ou de type ISBN-13.'
                )));
            }
        }

Options
-------

isbn10
~~~~~~

**type**: ``boolean`` [:ref:`default option<validation-default-option>`]

Si cette option requise est mis à ``true``, la contrainte vérifie si le code
est bien un code ISBN-10.

isbn13
~~~~~~

**type**: ``boolean`` [:ref:`default option<validation-default-option>`]

Si cette option requise est mis à ``true``, la contrainte vérifie si le code
est bien un code ISBN-13.

isbn10Message
~~~~~~~~~~~~~

**type**: ``string`` **default**: ``This value is not a valid ISBN-10.``

Le message qui est présenté si l'option `isbn10`_ est à true et que la valeur
n'est pas un ISBN-10.

isbn13Message
~~~~~~~~~~~~~

**type**: ``string`` **default**: ``This value is not a valid ISBN-13.``

Le message qui est présenté si l'option `isbn13`_ est à true et que la valeur
n'est pas un ISBN-13.

bothIsbnMessage
~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``This value is neither a valid ISBN-10 nor a valid ISBN-13.``

Le message qui est présenté si les options `isbn10`_ et `isbn13`_ sont à true et que la valeur
n'est ni un ISBN-10, ni un ISBN-13.
