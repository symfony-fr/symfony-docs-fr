Issn
====

.. versionadded:: 2.3
    C'est une nouvelle contrainte depuis la version 2.3.

Valide si la valeur est bien un numéro `ISSN`_.

+----------------+-----------------------------------------------------------------------+
| S'applique à   | :ref:`property or method<validation-property-target>`                 |
+----------------+-----------------------------------------------------------------------+
| Options        | - `message`_                                                          |
|                | - `caseSensitive`_                                                    |
|                | - `requireHyphen`_                                                    |
+----------------+-----------------------------------------------------------------------+
| Class          | :class:`Symfony\\Component\\Validator\\Constraints\\Issn`             |
+----------------+-----------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\IssnValidator`    |
+----------------+-----------------------------------------------------------------------+

Utilisation de base
-------------------

.. configuration-block::

    .. code-block:: yaml

        # src/JournalBundle/Resources/config/validation.yml
        Acme\JournalBundle\Entity\Journal:
            properties:
                issn:
                    - Issn: ~

    .. code-block:: php-annotations

        // src/Acme/JournalBundle/Entity/Journal.php
        namespace Acme\JournalBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Journal
        {
            /**
             * @Assert\Issn
             */
             protected $issn;
        }

    .. code-block:: xml

        <!-- src/Acme/JournalBundle/Resources/config/validation.xml -->
        <class name="Acme\JournalBundle\Entity\Journal">
            <property name="issn">
                <constraint name="Issn" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/JournalBundle/Entity/Journal.php
        namespace Acme\JournalBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Journal
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('issn', new Assert\Issn());
            }
        }

Options
-------

message
~~~~~~~

**type**: ``String`` default: ``This value is not a valid ISSN.``

Le message qui présenté si la valeur n'est pas un numéro ISSN.

caseSensitive
~~~~~~~~~~~~~

**type**: ``Boolean`` default: ``false``

La valeur ISSN doit être terminer par un 'x' minuscule par défaut. Si l'option
`caseSensitive`_  est passé à true, le numéro doit se terminer par un 'X' majuscule.

requireHyphen
~~~~~~~~~~~~~

**type**: ``Boolean`` default: ``false``

La valeur par défaut pour un numéro ISSN est composé sans tiret.
Quand l'option `requireHyphen`_ est passé à true, le numéro ISSN doit être
composé avec un tiret.

.. _`ISSN`: http://fr.wikipedia.org/wiki/International_Standard_Serial_Number

