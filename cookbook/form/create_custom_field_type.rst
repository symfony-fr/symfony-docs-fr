.. index::
   single: Formulaire; Type de champ personnalisé

Comment Créer un Type de Champ de Formulaire Personnalisé
=========================================================

Symfony est livré avec un ensemble de types de champ essentiels disponible
pour construire des formulaires. Cependant, il y a des situations où nous
voulons créer un type de champ de formulaire personnalisé pour un besoin
spécifique. Cet article assume que nous avons besoin d'une définition de
champ qui s'occupe du sexe/genre d'une personne, basé sur le champ existant
« choice ». Cette section explique comment le champ est défini, comment nous
pouvons personnaliser son affichage et, finalement, comment nous pouvons le
déclarer afin de pouvoir l'utiliser dans notre application.

Définir le Type de Champ
------------------------

Afin de créer le type de champ personnalisé, nous devons créer tout d'abord la
classe représentant le champ. Dans notre situation, la classe s'occupant du
type de champ sera nommée `GenderType` et le fichier sera stocké dans le répertoire
par défaut pour les champs de formulaire, c'est à dire ``<BundleName>\Form\Type``.
Assurez-vous que le champ étend :class:`Symfony\\Component\\Form\\AbstractType`::

    # src/Acme/DemoBundle/Form/Type/GenderType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;

    class GenderType extends AbstractType
    {
        public function getDefaultOptions()
        {
            return array(
                'choices' => array(
                    'm' => 'Male',
                    'f' => 'Female',
                )
            );
        }

        public function getParent(array $options)
        {
            return 'choice';
        }

        public function getName()
        {
            return 'gender';
        }
    }

.. tip::

    L'endroit où est stocké ce fichier n'est pas important - le répertoire
    ``Form\Type`` est seulement une convention.

Ici, la valeur retournée par la fonction ``getParent`` indique que nous
étendons le type de champ ``choice``. Cela signifie que, par défaut, nous
héritons de toute la logique et du rendu de l'affichage de ce type de champ.
Pour avoir un aperçu de cette logique, jetez un oeil à la classe `ChoiceType`_.
Il y a trois méthodes qui sont particulièrement importantes :

* ``buildForm()`` - Chaque type de champ possède une méthode ``buildForm``, qui
  est l'endroit où vous configurez et construisez n'importe quel(s) champ(s). Notez
  que c'est la même méthode que vous utilisez pour initialiser *vos* formulaires,
  et cela fonctionne de la même manière ici.

* ``buildView()`` - Cette méthode est utilisée pour définir quelconques
  variables supplémentaires dont vous aurez besoin lors du rendu de votre
  champ dans un template. Par exemple, dans `ChoiceType`_, une variable
  ``multiple`` est définie et utilisée dans le template pour définir (ou
  ne pas définir) l'attribut ``multiple`` pour le champ ``select``. Voir
  `Créer un Template pour le Champ`_ pour plus de détails.

* ``getDefaultOptions()`` - Cette méthode définit des options pour votre
  formulaire qui peuvent être utilisées dans ``buildForm()`` et
  ``buildView()``. Il y a beaucoup d'options communes à tous les champs
  (voir `FieldType`_), mais vous pouvez créer ici n'importe quelle autre dont
  vous pourriez avoir besoin.

.. tip::

    Si vous créez un champ qui se compose de beaucoup de champs, alors
    assurez-vous de définir votre type « parent » en tant que ``form``
    ou quelque chose qui étend ``form``. Aussi, si vous avez besoin de
    modifier la « vue » (« view » en anglais) de n'importe lequel de vos
    types enfants depuis votre type parent, utilisez la méthode
    ``buildViewBottomUp()``.

La méthode ``getName()`` retourne un identifiant qui devrait être unique
dans votre application. Ce dernier est utilisé dans différents endroits,
comme par exemple lorsque vous personnalisez la manière dont votre formulaire
sera rendu.

Le but de votre champ était d'étendre le type « choice » afin d'activer
la sélection du sexe/genre. Cela est accompli en définissant ``choices``
par une liste de genres possibles.

Créer un Template pour le Champ
-------------------------------

Chaque type de champ est rendu par un fragment de template, qui est déterminé
en partie par la valeur retournée par votre méthode ``getName()``. Pour plus
d'informations, voir :ref:`cookbook-form-customization-form-themes`.

Dans ce cas, comme notre champ parent est ``choice``, nous n'avons pas
*besoin* de faire quoi que ce soit car notre type de champ personnalisé
sera automatiquement rendu comme un type ``choice``. Mais pour le bien
fondé de cet exemple, supposons que quand notre champ est « étendu » (i.e.
boutons radio ou checkbox, à la place d'un champ « select »), nous souhaitons
toujours l'afficher dans un élément ``ul``. Dans le template de votre thème de
formulaire (voir le lien ci-dessus pour plus de détails), créez un bloc
``gender_widget`` pour gérer ceci :

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Form/fields.html.twig #}

    {% block gender_widget %}
    {% spaceless %}
        {% if expanded %}
            <ul {{ block('widget_container_attributes') }}>
            {% for child in form %}
                <li>
                    {{ form_widget(child) }}
                    {{ form_label(child) }}
                </li>
            {% endfor %}
            </ul>
        {% else %}
            {# laisse le widget choice afficher la balise select #}
            {{ block('choice_widget') }}
        {% endif %}
    {% endspaceless %}
    {% endblock %}

.. note::

    Assurez-vous que c'est le bon préfixe du widget qui est utilisé. Dans cet
    exemple, le nom devrait être ``gender_widget``, si l'on se fie à la valeur
    retournée par ``getName``. De plus, le fichier de configuration principal
    devrait pointer vers le template du formulaire personnalisé afin qu'il soit
    utilisé lors de l'affichage de tous les formulaires.

    .. code-block:: yaml

        # app/config/config.yml

        twig:
            form:
                resources:
                    - 'AcmeDemoBundle:Form:fields.html.twig'

Utiliser le Type de Champ
-------------------------

Vous pouvez dès lors utiliser votre type de champ personnalisé en créant
tout simplement une nouvelle instance du type dans l'un de vos formulaires::

    // src/Acme/DemoBundle/Form/Type/AuthorType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;
    
    class AuthorType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('gender_code', new GenderType(), array(
                'empty_value' => 'Choose a gender',
            ));
        }
    }

Mais cela fonctionne uniquement car le ``GenderType()`` est très simple.
Que se passerait-il si les différents genres étaient stockés dans un fichier
de configuration ou dans une base de données ? La prochaine section explique
comment des types de champ plus complexes peuvent résoudre ce problème.

Créer votre Type de Champ en tant que Service
---------------------------------------------

Jusqu'ici, cet article a assumé que vous aviez un type de champ personnalisé
très simple. Mais si vous avez besoin d'accéder à la configuration, à une
connexion à la base de données, ou quelconque autre service, alors vous
allez vouloir déclarer votre type personnalisé en tant que service. Par
exemple, supposons que nous stockions les paramètres du sexe/genre dans une
configuration :

.. configuration-block::

    .. code-block:: yaml
    
        # app/config/config.yml
        parameters:
            genders:
                m: Male
                f: Female

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="genders" type="collection">
                <parameter key="m">Male</parameter>
                <parameter key="f">Female</parameter>
            </parameter>
        </parameters>

Pour utiliser ce paramètre, nous allons définir notre type de champ personnalisé
en tant que service, en injectant la valeur du paramètre ``genders`` en tant que
premier argument de la fonction ``__construct`` (devant être créée) :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/config/services.yml
        services:
            form.type.gender:
                class: Acme\DemoBundle\Form\Type\GenderType
                arguments:
                    - "%genders%"
                tags:
                    - { name: form.type, alias: gender }

    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/config/services.xml -->
        <service id="form.type.gender" class="Acme\DemoBundle\Form\Type\GenderType">
            <argument>%genders%</argument>
            <tag name="form.type" alias="gender" />
        </service>

.. tip::

    Assurez-vous que le fichier des services est importé. Voir
    :ref:`service-container-imports-directive` pour plus de détails.

Soyez sûr que l'attribut ``alias`` du tag corresponde à la valeur retournée
par la méthode ``getName`` définie plus tôt. Nous allons voir l'importance de
cela dans un moment quand nous utiliserons le type de champ personnalisé.
Mais tout d'abord, ajoutez une méthode ``__construct`` à ``GenderType``,
qui reçoit la configuration du sexe/genre::

    # src/Acme/DemoBundle/Form/Type/GenderType.php
    namespace Acme\DemoBundle\Form\Type;
    // ...

    class GenderType extends AbstractType
    {
        private $genderChoices;
        
        public function __construct(array $genderChoices)
        {
            $this->genderChoices = $genderChoices;
        }
    
        public function getDefaultOptions()
        {
            return array(
                'choices' => $this->genderChoices,
            );
        }
        
        // ...
    }

Super ! Le ``GenderType`` est maintenant « rempli » par les paramètres de
la configuration et déclaré en tant que service. Et parce que nous avons
utilisé l'alias ``form.type`` dans sa configuration, utiliser le champ est
maintenant beaucoup plus facile::

    // src/Acme/DemoBundle/Form/Type/AuthorType.php
    namespace Acme\DemoBundle\Form\Type;
    // ...

    class AuthorType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('gender_code', 'gender', array(
                'empty_value' => 'Choose a gender',
            ));
        }
    }

Notez qu'à la place d'instancier une nouvelle instance, nous pouvons simplement
y référer grâce à l'alias utilisé dans la configuration de notre service, ``gender``.
Amusez-vous !

.. _`ChoiceType`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Extension/Core/Type/ChoiceType.php
.. _`FieldType`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Extension/Core/Type/FieldType.php