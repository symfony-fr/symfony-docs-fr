File
====

Valide qu'une valeur est un « fichier » valide, qui peut être l'un des formats
suivants :

* Une chaîne de caractères (ou un objet avec une méthode ``__toString()``) qui
  représente un chemin vers un fichier existant ;

* Un objet :class:`Symfony\\Component\\HttpFoundation\\File\\File` valide
  (ce qui inclut les objets de la classe :class:`Symfony\\Component\\HttpFoundation\\File\\UploadedFile`).

Cette contrainte est souvent utilisée dans les formulaires avec le type de champ
:doc:`file</reference/forms/types/file>`.

.. tip::

    Si le fichier que vous validez est une image, essayez la contrainte
    :doc:`Image</reference/constraints/Image>`.

+----------------+---------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`             |
+----------------+---------------------------------------------------------------------+
| Options        | - `maxSize`_                                                        |
|                | - `mimeTypes`_                                                      |
|                | - `maxSizeMessage`_                                                 |
|                | - `mimeTypesMessage`_                                               |
|                | - `notFoundMessage`_                                                |
|                | - `notReadableMessage`_                                             |
|                | - `uploadIniSizeErrorMessage`_                                      |
|                | - `uploadFormSizeErrorMessage`_                                     |
|                | - `uploadErrorMessage`_                                             |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\File`           |
+----------------+---------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\FileValidator`  |
+----------------+---------------------------------------------------------------------+

Utilisation de base
-------------------

Cette contrainte est souvent utilisée sur une propriété qui est affichée dans
un formulaire avec le type de champ :doc:`file</reference/forms/types/file>`.
Par exemple, supposons que vous créiez un formulaire « Auteur » où vous pouvez
uploader une « bio » au format PDF. Dans votre formulaire, la propriété
``bioFile`` sera de type ``file``. La classe ``Author`` ressemblerait à ceci::

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    use Symfony\Component\HttpFoundation\File\File;

    class Author
    {
        protected $bioFile;

        public function setBioFile(File $file = null)
        {
            $this->bioFile = $file;
        }

        public function getBioFile()
        {
            return $this->bioFile;
        }
    }

Pour garantir que l'objet ``File`` ``bioFile`` soit valide, et que le fichier
soit plus petit qu'une certaine taille et soit au format PDF, ajoutez le code
suivant :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author
            properties:
                bioFile:
                    - File:
                        maxSize: 1024k
                        mimeTypes: [application/pdf, application/x-pdf]
                        mimeTypesMessage: Choisissez un fichier PDF valide
                        

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\File(
             *     maxSize = "1024k",
             *     mimeTypes = {"application/pdf", "application/x-pdf"},
             *     mimeTypesMessage = "Choisissez un fichier PDF valide"
             * )
             */
            protected $bioFile;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="bioFile">
                <constraint name="File">
                    <option name="maxSize">1024k</option>
                    <option name="mimeTypes">
                        <value>application/pdf</value>
                        <value>application/x-pdf</value>
                    </option>
                    <option name="mimeTypesMessage">Choisissez un fichier PDF valide</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        // ...

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\File;

        class Author
        {
            // ...

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('bioFile', new File(array(
                    'maxSize' => '1024k',
                    'mimeTypes' => array(
                        'application/pdf',
                        'application/x-pdf',
                    ),
                    'mimeTypesMessage' => 'Choisissez un fichier PDF valide',
                )));
            }
        }

La propriété ``bioFile`` est validée pour garantir qu'il s'agisse bien d'un
fichier. Sa taille et son type MIME sont également validés car les options
correspondantes ont été spécifiées.

Options
-------

maxSize
~~~~~~~

**type**: ``mixed``

Si cette option est définie, la taille du fichier doit être inférieure à la taille
spécifiée pour être valide. La taille du fichier peut être donnée dans l'un des formats
suivants :

* **octets**: Pour spécifier l'option ``maxSize`` en octets, entrez la valeur en numérique
  seulement (ex ``4096``);

* **kilooctets**: Pour spécifier l'option ``maxSize`` en kilooctets, entrez la valeur numérique
  et ajoutez le suffixe « k » en minuscule (ex ``200k``);

* **megaoctet**: Pour spécifier l'option ``maxSize`` en megaoctets, entrez la valeur numérique
  et ajoutez le suffixe « M » en majuscule (ex ``200M``);

mimeTypes
~~~~~~~~~

**type**: ``array`` ou ``string``

Si cette option est définie, le validateur vérifiera que le type MIME
du fichier envoyé correspond au type MIME donné (si l'option est définie sous forme
de chaîne de caractères) ou s'il existe dans la collection de types MIME
donnés (si l'option est définie sous forme de tableau).

maxSizeMessage
~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The file is too large ({{ size }}). Allowed maximum size is {{ limit }}``

Le message affiché si le fichier uploadé est plus lourd que l'option `maxSize`_.

mimeTypesMessage
~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The mime type of the file is invalid ({{ type }}). Allowed mime types are {{ types }}``

Le message affiché si le type MIME du fichier ne correspond pas au(x) type(s) MIME
autorisé(s) par l'option `mimeTypes`_.

notFoundMessage
~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The file could not be found``

Le message affiché si aucun fichier ne correspond au chemin donné. Cette
erreur n'est possible que si la valeur sous-jacente est un chemin sous forme
de chaîne de caractères, puisque l'objet ``File`` ne pourra pas être construit à partir
d'un chemin non valide.

notReadableMessage
~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The file is not readable``

Le message affiché si le fichier existe, mais que la fonction PHP ``is_readable``
échoue à passer le chemin du fichier.

uploadIniSizeErrorMessage
~~~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The file is too large. Allowed maximum size is {{ limit }}``

Le message affiché si le fichier uploadé est plus lourd que le maximum défini dans le paramètre
``upload_max_filesize`` du PHP.ini.

uploadFormSizeErrorMessage
~~~~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The file is too large``

Le message affiché si le fichier uploadé est plus lourd que le maximum
autorisé dans le champ HTML input file.

uploadErrorMessage
~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The file could not be uploaded``

Le message affiché si le fichier ne peut pas être uploadé pour une raison
quelconque, par exemple si l'upload échoue ou qu'il est impossible d'écrire
sur le disque.