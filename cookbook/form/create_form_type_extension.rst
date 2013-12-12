.. index::
   single: Form; Form type extension

Comment créer une extension de type de formulaire
=================================================

Les :doc:`types de formulaire personnalisés<create_custom_field_type>` sont
supers si vous avez besoin de types de champ qui font quelque chose de
spécifique, comme un sélecteur de civilité, ou un champ pour saisir la TVA.

Mais parfois, vous n'avez pas vraiment besoin d'ajouter de nouveaux types
de champ, vous voulez en fait ajouter de nouvelles fonctionnalités sur des
types existant. C'est ici que les extensions de type entrent en jeu.

Les extensions de type de formulaire ont deux utilisations principales :

#. Vous voulez ajouter une **fonctionnalité générique sur plusieurs types**
   (comme ajouter un texte d'« aide » sur tout les types de champ);
#. Vous voulez ajouter une **fonctionnalité spécifique sur un type** (comme
   ajouter une fonctionnalité « téléchargement » sur un type de champ « file »).

Dans ces deux cas, vous pourrez atteindre votre objectif en personnalisant
l'affichage du formulaire, ou en personnalisant les types de champ. Mais
utiliser les extensions de type de formulaire peut être plus propre (en limitant
la part de logique métier dans les templates) et plus flexible (vous pouvez
ajouter plusieurs extensions de type à un seul type de formulaire)/

Les extensions de type de formulaire peuvent accomplir bien plus que ce que
peuvent faire des types de champ personnalisés, mais au lieu d'être eux-mêmes
des types de champ, ils se **branchent sur des types existants**.

Imaginez que vous devez gérer une entité ``Media``, et que chaque média est
associé à un fichier. Votre formulaire ``Media`` utilise un type file, mais
lorsque vous éditez l'entité, vous voulez avoir un aperçu automatique de l'image
affiché à côté du champ.

Vous pourriez bien évidemment faire cela en personnalisation la manière dont est
affiché le champ dans le template, mais les extensions de type de champ vous
permettent de le faire sans répéter le code.

Définir l'extension de type de formulaire
-----------------------------------------

Votre première tâche est de créer la classe d'extension de type de formulaire.
Appelons-la ``ImageTypeExtension``. Par convention, les extensions de formulaire
se trouvent habituellement dans le répertoire ``Form\Extension`` de l'un de
vos bundles.

Lorsque vous créez une extension de type de formulaire, vous pouvez soit
implémenter l'interface :class:`Symfony\\Component\\Form\\FormTypeExtensionInterface`,
soit étendre la classe :class:`Symfony\\Component\\Form\\AbstractTypeExtension`.
Dans la plupart des cas, il est plus simple d'étendre la classe abstraite::

    // src/Acme/DemoBundle/Form/Extension/ImageTypeExtension.php
    namespace Acme\DemoBundle\Form\Extension;

    use Symfony\Component\Form\AbstractTypeExtension;

    class ImageTypeExtension extends AbstractTypeExtension
    {
        /**
         * Retourne le nom du type de champ qui est étendu
         *
         * @return string Le nom du type qui est étendu
         */
        public function getExtendedType()
        {
            return 'file';
        }
    }

La seule méthode que vous **devez** implémenter est la fonction ``getExtendedType``.
Elle est utilisée pour spécifier le nom du type de formulaire qui est étendu
par votre extension.

.. tip::

    La valeur que vous retournez dans la méthode ``getExtendedType`` correspond
    à la valeur retournée par la méthode ``getName`` de la classe de type de
    formulaire que vous désirez étendre.

En plus de la fonction ``getExtendedType``, vous allez probablement vouloir
surcharger l'une des méthodes suivantes :

* ``buildForm()``

* ``buildView()``

* ``setDefaultOptions()``

* ``finishView()``

Pour plus d'informations sur ce que ces méthodes font, vous pouvez lire
l'article du Cookbook
:doc:`Créer des types de champ personnalisés</cookbook/form/create_custom_field_type>`.


Enregistrer vos extensions de type de formulaire comme service
--------------------------------------------------------------

La prochaine étape est d'indiquer à Symfony que vous avez créé une extension.
Tout ce que vous devez faire pour cela est de la déclarer comme service en
utilisant le tag ``form.type_extension`` :

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_demo_bundle.image_type_extension:
                class: Acme\DemoBundle\Form\Extension\ImageTypeExtension
                tags:
                    - { name: form.type_extension, alias: file }

    .. code-block:: xml

        <service id="acme_demo_bundle.image_type_extension" class="Acme\DemoBundle\Form\Extension\ImageTypeExtension">
            <tag name="form.type_extension" alias="file" />
        </service>

    .. code-block:: php

        $container
            ->register('acme_demo_bundle.image_type_extension', 'Acme\DemoBundle\Form\Extension\ImageTypeExtension')
            ->addTag('form.type_extension', array('alias' => 'file'));

La clé ``alias`` du tag est le type de champ sur lequel appliquer votre extension.
Dans cet exemple, comme vous voulez étendre le type de champ ``file``, vous utilisez
``file`` comme alias.

Ajouter la logique métier à l'extension
---------------------------------------

Le but de votre extension est d'afficher de jolies images à côté des champs
d'upload de fichier (quand le modèle associé contient des images). Pour atteindre
cet objectif, supposons que vous utilisez une approche similaire à celle décrite
dans :doc:`Comment gérer l'upload de fichiers avec Doctrine</cookbook/doctrine/file_uploads>`:
vous avez un modèle Média avec une propriété ``file`` (qui correspond au champ file)
et une propriété ``path`` (qui correspond au chemin de l'image dans la base de données)::

    // src/Acme/DemoBundle/Entity/Media.php
    namespace Acme\DemoBundle\Entity;

    use Symfony\Component\Validator\Constraints as Assert;

    class Media
    {
        // ...

        /**
         * @var string Le chemin stocké en base de données
         */
        private $path;

        /**
         * @var \Symfony\Component\HttpFoundation\File\UploadedFile
         * @Assert\File(maxSize="2M")
         */
        public $file;

        // ...

        /**
         * Retourne l'url de l'image
         *
         * @return null|string
         */
        public function getWebPath()
        {
            // ... $webPath est l'url complète de l'image, qui est utilisée dans le template

            return $webPath;
        }

Votre classe d'extension de type de formulaire devra faire deux choses pour
étendre le type de formulaire ``file`` :

#. Surcharger la méthode ``setDefaultOptions`` pour ajouter une option image_path;
#. Surcharger les méthodes ``buildForm`` et ``buildView`` pour passer l'url de l'image
   à la vue.

La logique est la suivante : lorsque vous ajoutez un champ de formulaire du
type ``file``, vous pourrez alors spécifier une nouvelle option : ``image_path``.
Cette option indiquera au champ de fichier comment récupérer l'url de l'image
actuelle pour l'afficher dans la vue::

    // src/Acme/DemoBundle/Form/Extension/ImageTypeExtension.php
    namespace Acme\DemoBundle\Form\Extension;

    use Symfony\Component\Form\AbstractTypeExtension;
    use Symfony\Component\Form\FormView;
    use Symfony\Component\Form\FormInterface;
    use Symfony\Component\PropertyAccess\PropertyAccess;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class ImageTypeExtension extends AbstractTypeExtension
    {
        /**
         * Retourne le nom du type de champ qui est étendu
         *
         * @return string Le nom du type étendu
         */
        public function getExtendedType()
        {
            return 'file';
        }

        /**
         * Ajoute l'option image_path
         *
         * @param \Symfony\Component\OptionsResolver\OptionsResolverInterface $resolver
         */
        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setOptional(array('image_path'));
        }

        /**
         * Passe l'url de l'image à la vue
         *
         * @param \Symfony\Component\Form\FormView $view
         * @param \Symfony\Component\Form\FormInterface $form
         * @param array $options
         */
        public function buildView(FormView $view, FormInterface $form, array $options)
        {
            if (array_key_exists('image_path', $options)) {
                $parentData = $form->getParent()->getData();

                if (null !== $parentData) {
                   $accessor = PropertyAccess::createPropertyAccessor();
                   $imageUrl = $accessor->getValue($parentData, $options['image_path']);
                } else {
                    $imageUrl = null;
                }

                // définit une variable "image_url" qui sera disponible à l'affichage du champ
                $view->vars['image_url'] = $imageUrl;
            }
        }

    }

Surcharger le fragment de template du widget File
-------------------------------------------------

Chaque type de champ est affiché grâce à un fragment de template. Ces fragments
de templates peuvent être surchargés pour personnaliser l'affichage du formulaire.
Pour plus d'informations, vous pouvez consulter l'article
:ref:`cookbook-form-customization-form-themes`.

Dans votre classe d'extension, vous avez ajouté une nouvelle variable (``image_url``),
mais vous n'avez pas encore tiré profit de cette nouvelle variable dans vos templates.
Spécifiquement, vous devez surcharger le bloc ``file_widget`` pour le faire :

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/DemoBundle/Resources/views/Form/fields.html.twig #}
        {% extends 'form_div_layout.html.twig' %}

        {% block file_widget %}
            {% spaceless %}

            {{ block('form_widget') }}
            {% if image_url is not null %}
                <img src="{{ asset(image_url) }}"/>
            {% endif %}

            {% endspaceless %}
        {% endblock %}

    .. code-block:: html+php

        <!-- src/Acme/DemoBundle/Resources/views/Form/file_widget.html.php -->
        <?php echo $view['form']->widget($form) ?>
        <?php if (null !== $image_url): ?>
            <img src="<?php echo $view['assets']->getUrl($image_url) ?>"/>
        <?php endif ?>

.. note::

    Vous devrez changer votre fichier de configuration ou spécifier
    explicitement que votre formulaire utilise un thème pour que Symfony
    utilise le bloc que vous avez surchargé. Pour plus d'informations, lisez
    :ref:`cookbook-form-customization-form-themes`.

Utiliser l'extension de type de formulaire
------------------------------------------

A partir de maintenant, lorsque vous ajouterez un champ de type ``file``
dans un formulaire, vous pourrez spécifier l'option ``image_path`` qui sera
utilisée pour afficher une image à côté du champ. Par exemple::

    // src/Acme/DemoBundle/Form/Type/MediaType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;

    class MediaType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('name', 'text')
                ->add('file', 'file', array('image_path' => 'webPath'));
        }

        public function getName()
        {
            return 'media';
        }
    }

Lorsque vous afficherez le formulaire, si le modèle sous-jacent a déjà été associé
à une image, vous la verrez affichée à côté du champ d'upload.
