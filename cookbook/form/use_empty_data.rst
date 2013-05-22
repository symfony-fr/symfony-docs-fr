.. index::
   single: Form; Empty data

Comment configurer des données vierges pour une classe de Formulaire
====================================================================

L'option ``empty_data`` vous permet de spécifier un jeu de données
vierges pour votre classe de formulaire. Ces données vierges seront
utilisées si vous soumettez votre formulaire, mais sans appeler la
méthode ``setData()`` ou sans avoir initialisé votre formulaire lors
de sa création. Par exemple::

    public function indexAction()
    {
        $blog = ...;

        // $blog est passé lors de la création, donc l'option empty_data option
        // n'est pas nécessaire
        $form = $this->createForm(new BlogType(), $blog);

        // Aucune données n'est passée, donc l'option empty_data est utilisée pour
        // obtenir des « données initiales »
        $form = $this->createForm(new BlogType());
    }

Par défaut, ``empty_data`` est définie à ``null``. Ou alors, si vous avez
spécifié l'option ``data_class`` de votre classe de formulaire, l'option
``empty_data`` sera définie par défaut comme une nouvelle instance de cette
classe. L'instance sera créée en appelant le constructeur sans aucun argument.

Si vous voulez surcharger ce comportement par défaut, il y a deux manières de
procéder.

Possibilité 1: Instancier une nouvelle classe
---------------------------------------------

L'une des raisons pour lesquelles vous voudrez utiliser cette possibilité
est que vous voulez utiliser un constructeur en passant des arguments.
Souvenez-vous, par défaut, l'option ``data_class`` appelle le constructeur
sans argument::

    // src/Acme/DemoBundle/Form/Type/BlogType.php

    // ...
    use Symfony\Component\Form\AbstractType;
    use Acme\DemoBundle\Entity\Blog;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class BlogType extends AbstractType
    {
        private $someDependency;

        public function __construct($someDependency)
        {
            $this->someDependency = $someDependency;
        }
        // ...

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'empty_data' => new Blog($this->someDependency),
            ));
        }
    }

Vous pouvez instancier votre classe comme vous le voulez. Dans cet exemple,
nous passons une dépendance dans la classe ``BlogType`` au moment où elle
est instanciée, puis nous l'utilisons pour instancier un objet ``Blog``.
L'important est que vous pouvez définir ``empty_data`` comme étant un
« nouvel » objet qui correspond exactement à ce que vous voulez utiliser.

Possibilité 2: Fournir une Closure
----------------------------------

Il est préférable d'utiliser une closure car l'objet ne sera créé uniquement
lorsqu'il sera nécessaire.

La closure doit prendre une instance de  ``FormInterface`` comme premier
argument::

    use Symfony\Component\OptionsResolver\OptionsResolverInterface;
    use Symfony\Component\Form\FormInterface;
    // ...

    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'empty_data' => function (FormInterface $form) {
                return new Blog($form->get('title')->getData());
            },
        ));
    }