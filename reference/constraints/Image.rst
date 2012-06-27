Image
=====

La contrainte Image fonctionne exactement comme la contrainte :doc:`File</reference/constraints/File>`,
sauf que ses options `mimeTypes`_ et `mimeTypesMessage` sont automatiquement définies
pour fonctionner spécifiquement avec des images.

De plus, dans la version 2.1 de Symfony, elle a des options qui vous permettent
de valider une image selon sa largeur et sa hauteur.

Lisez la documentation de la contrainte :doc:`File</reference/constraints/File>`
pour tout savoir sur cette contrainte.

+----------------+---------------------------------------------------------------------------+
| S'applique à   | :ref:`propriété ou méthode<validation-property-target>`                   |
+----------------+---------------------------------------------------------------------------+
| Options        | - `mimeTypes`_                                                            |
|                | - `minWidth`_                                                             |
|                | - `maxWidth`_                                                             |
|                | - `maxHeight`_                                                            |
|                | - `minHeight`_                                                            |
|                | - `mimeTypesMessage`_                                                     |
|                | - `sizeNotDetectedMessage`_                                               |
|                | - `maxWidthMessage`_                                                      |
|                | - `minWidthMessage`_                                                      |
|                | - `maxHeightMessage`_                                                     |
|                | - `minHeightMessage`_                                                     |
|                | - Voir :doc:`File</reference/constraints/File>` pour les options héritées |
+----------------+---------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\File`                 |
+----------------+---------------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\FileValidator`        |
+----------------+---------------------------------------------------------------------------+

Utilisation de base
-------------------

Cette contrainte est le plus souvent utilisée sur une propriété qui sera affichée
sous forme de champ :doc:`file</reference/forms/types/file>` dans un formulaire.
Par exemple, supposons que vous créiez un formulaire « Auteur » (« Author » en anglais)
dans lequel vous pouvez uploader une image représentant le « portrait » (« headshot »
en anglais) de l'auteur. Dans votre formulaire, la propriété ``headshot`` sera un type
``file``. La classe ``Author`` pourrait ressembler à ce qui suit::

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    use Symfony\Component\HttpFoundation\File\File;

    class Author
    {
        protected $headshot;

        public function setHeadshot(File $file = null)
        {
            $this->headshot = $file;
        }

        public function getHeadshot()
        {
            return $this->headshot;
        }
    }

Pour garantir que l'objet ``File`` ``headshot`` soit une image valide et dont la
taille se conforme à certaines contraintes de hauteur et largeur, ajoutez le code
suivant :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author
            properties:
                headshot:
                    - Image:
                        minWidth: 200
                        maxWidth: 400
                        minHeight: 200
                        maxHeight: 400
                        

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\File(
             *     minWidth = 200,
             *     maxWidth = 400,
             *     minHeight = 200,
             *     maxHeight = 400,
             * )
             */
            protected $headshot;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="headshot">
                <constraint name="File">
                    <option name="minWidth">200</option>
                    <option name="maxWidth">400</option>
                    <option name="minHeight">200</option>
                    <option name="maxHeight">400</option>
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
                $metadata->addPropertyConstraint('headshot', new File(array(
                    'minWidth' => 200,
                    'maxWidth' => 400,
                    'minHeight' => 200,
                    'maxHeight' => 400,
                )));
            }
        }

La propriété ``headshot`` est maintenant validée pour garantir qu'il s'agit bien
d'une image et que sa taille respecte une certaine hauteur et une certaine largeur.

Options
-------

Cette contrainte partage toutes ses options avec la contrainte :doc:`File</reference/constraints/File>`.
Cependant, elle modifie les valeurs par défaut de deux options, et possède plusieurs
autres options supplémentaires.

mimeTypes
~~~~~~~~~

**type**: ``array`` ou ``string`` **default**: ``image/*``

mimeTypesMessage
~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``This file is not a valid image``

.. versionadded:: 2.1
    Toutes les options min/max width/height sont une nouveauté de la version 2.1 de Symfony.

minWidth
~~~~~~~~

**type**: ``integer``

Si cette option est définie, la largeur du fichier image devra être plus grand ou
égale à cette valeur exprimée en pixels.

maxWidth
~~~~~~~~

**type**: ``integer``

Si cette option est définie, la largeur du fichier image devra être plus petite ou
égale à cette valeur exprimée en pixels.

minHeight
~~~~~~~~~

**type**: ``integer``

Si cette option est définie, la hauteur du fichier image devra être plus grande ou
égale à cette valeur exprimée en pixels.

maxHeight
~~~~~~~~~

**type**: ``integer``

Si cette option est définie, la hauteur du fichier image devra être plus petite ou
égale à cette valeur exprimée en pixels.

sizeNotDetectedMessage
~~~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The size of the image could not be detected``

Si le système n'est pas capable de déterminer la taille de l'image, cette erreur
sera affichée. Elle n'apparaîtra que si au moins une des quatres options sur les tailles
est définie.

maxWidthMessage
~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The image width is too big ({{ width }}px). Allowed maximum width is {{ max_width }}px``

Le message d'erreur si la largeur de l'image excède `maxWidth`_.

minWidthMessage
~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The image width is too small ({{ width }}px). Minimum width expected is {{ min_width }}px``

Le message d'erreur si la largeur de l'image est plus petite que `minWidth`_.

maxHeightMessage
~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The image height is too big ({{ height }}px). Allowed maximum height is {{ max_height }}px``

Le message d'erreur si la hauteur de l'image excède `maxHeight`_.

minHeightMessage
~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The image height is too small ({{ height }}px). Minimum height expected is {{ min_height }}px``

Le message d'erreur si la hauteur de l'image est plus petite que `minHeight`_.