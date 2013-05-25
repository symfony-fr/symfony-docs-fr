Image
=====

La contrainte Image fonctionne exactement comme la contrainte :doc:`File</reference/constraints/File>`,
sauf que ses options `mimeTypes`_ et `mimeTypesMessage` sont automatiquement définies
pour fonctionner spécifiquement avec des images.

De plus, depuis Symfony 2.1, la contrainte Image possède des options vous permettant
d'ajouter de la validation sur la largeur et la hauteur de l'image.

Lisez la documentation de la contrainte :doc:`File</reference/constraints/File>`
pour tout savoir sur cette contrainte.

+----------------+----------------------------------------------------------------------+
| S'applique à   | :ref:`property or method<validation-property-target>`                |
+----------------+----------------------------------------------------------------------+
| Options        | - `mimeTypes`_                                                       |
|                | - `minWidth`_                                                        |
|                | - `maxWidth`_                                                        |
|                | - `maxHeight`_                                                       |
|                | - `minHeight`_                                                       |
|                | - `mimeTypesMessage`_                                                |
|                | - `sizeNotDetectedMessage`_                                          |
|                | - `maxWidthMessage`_                                                 |
|                | - `minWidthMessage`_                                                 |
|                | - `maxHeightMessage`_                                                |
|                | - `minHeightMessage`_                                                |
|                | - See :doc:`File</reference/constraints/File>` for inherited options |
+----------------+----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\File`            |
+----------------+----------------------------------------------------------------------+
| Validateur     | :class:`Symfony\\Component\\Validator\\Constraints\\FileValidator`   |
+----------------+----------------------------------------------------------------------+

Utilisation de base
-------------------

Cette contrainte est le plus couramment utilisée sur une propriété qui sera rendue
dans un formulaire en tant que type de formulaire :doc:`file</reference/forms/types/file>`.
Par exemple, supposons que vous créiez un formulaire pour rentrer des informations
concernant un auteur où vous pouvez « uploader » un « headshot » (« photo portrait »
en anglais) de ce dernier. Dans votre formulaire, la propriété ``headshot`` serait
de type ``file``. La classe ``Author`` pourrait ressembler à cela::

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

Pour garantir que l'objet ``File`` ``headshot`` soit une image valide et que
ses dimensions soient comprises dans un certain intervalle, ajoutez ce qui suit :

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
             * @Assert\Image(
             *     minWidth = 200,
             *     maxWidth = 400,
             *     minHeight = 200,
             *     maxHeight = 400
             * )
             */
            protected $headshot;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="headshot">
                <constraint name="Image">
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
        use Symfony\Component\Validator\Constraints\Image;

        class Author
        {
            // ...

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('headshot', new Image(array(
                    'minWidth' => 200,
                    'maxWidth' => 400,
                    'minHeight' => 200,
                    'maxHeight' => 400,
                )));
            }
        }

La propriété ``headshot`` est validée de manière à garantir que c'est une image
réelle et que ses dimensions sont comprises entre une certaine largeur et hauteur.

Options
-------

Cette contrainte partage toutes ses options avec la contrainte :doc:`File</reference/constraints/File>`.
Cependant, elle modifie tout de même deux des valeurs d'option par défaut et
ajoute plusieurs autres options.

mimeTypes
~~~~~~~~~

**type**: ``array`` ou ``string`` **default**: ``image/*``

Vous pouvez trouver une liste des types mime existants sur le `site IANA`_.

mimeTypesMessage
~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``This file is not a valid image``

minWidth
~~~~~~~~

**type**: ``integer``

Si définie, la largeur de l'image doit être supérieure ou égale à cette valeur
en pixels.

maxWidth
~~~~~~~~

**type**: ``integer``

Si définie, la largeur de l'image doit être inférieure ou égale à cette valeur
en pixels.

minHeight
~~~~~~~~~

**type**: ``integer``

Si définie, la hauteur de l'image doit être supérieure ou égale à cette valeur
en pixels.

maxHeight
~~~~~~~~~

**type**: ``integer``

Si définie, la hauteur de l'image doit être inférieure ou égale à cette valeur
en pixels.

sizeNotDetectedMessage
~~~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The size of the image could not be detected``

Si le système n'est pas capable de déterminer la taille de l'image, cette erreur
sera affichée. Cela se produit uniquement quand au moins une des quatre options
de contraintes de taille a été définie.

maxWidthMessage
~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The image width is too big ({{ width }}px). Allowed maximum width is {{ max_width }}px``

Le message d'erreur si la largeur de l'image est supérieure à `maxWidth`_.

minWidthMessage
~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The image width is too small ({{ width }}px). Minimum width expected is {{ min_width }}px``

Le message d'erreur si la largeur de l'image est inférieure à `minWidth`_.

maxHeightMessage
~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The image height is too big ({{ height }}px). Allowed maximum height is {{ max_height }}px``

Le message d'erreur si la hauteur de l'image est supérieure à `maxHeight`_.

minHeightMessage
~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The image height is too small ({{ height }}px). Minimum height expected is {{ min_height }}px``

Le message d'erreur si la hauteur de l'image est inférieure à `maxHeight`_.

.. _`site IANA`: http://www.iana.org/assignments/media-types/image/index.html
