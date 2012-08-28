.. index::
   single: Form; Data transformers

Comment utiliser les Convertisseurs de Données
==============================================

Vous allez souvent éprouver le besoin de transformer les données entrées par
l'utilisateur dans un formulaire en quelque chose d'autre qui sera utilisé
dans votre programme. Vous pourriez effectuer ceci manuellement dans votre
contrôleur, mais que se passe-t-il si vous voulez réutiliser ce formulaire
spécifique à différents endroits ?

Supposons que vous ayez une relation « one-to-one » de « Task » (« Tâche » en français) vers
« Issue » (« Problème » en français), par exemple : une tâche possède de manière
optionnelle un problème qui lui est lié. Ajouter un élément « listbox » contenant
tous les problèmes possibles peut éventuellement engendrer une très longue liste dans
laquelle il est impossible de trouver quoi que ce soit. Vous pourriez vouloir utiliser
plutôt un champ texte dans lequel l'utilisateur pourra simplement taper le numéro
du problème recherché.

Vous pouvez essayer de faire cela dans votre contrôleur mais ce n'est pas la meilleure
solution. Ce serait beaucoup mieux si votre problème pouvait automatiquement être
converti en un objet Issue.
C'est maintenant que le Convertisseur de Données entre en jeu.

Créer le convertisseur (Transformer)
------------------------------------

Créez d'abord la classe `IssueToNumberTransformer`, elle sera chargée de convertir
un numéro de problème en un objet Issue et inversement::

    // src/Acme/TaskBundle/Form/DataTransformer/IssueToNumberTransformer.php
    namespace Acme\TaskBundle\Form\DataTransformer;

    use Symfony\Component\Form\DataTransformerInterface;
    use Symfony\Component\Form\Exception\TransformationFailedException;
    use Doctrine\Common\Persistence\ObjectManager;
    use Acme\TaskBundle\Entity\Issue;

    class IssueToNumberTransformer implements DataTransformerInterface
    {
        /**
         * @var ObjectManager
         */
        private $om;

        /**
         * @param ObjectManager $om
         */
        public function __construct(ObjectManager $om)
        {
            $this->om = $om;
        }

        /**
         * Transforms an object (issue) to a string (number).
         *
         * @param  Issue|null $issue
         * @return string
         */
        public function transform($issue)
        {
            if (null === $issue) {
                return "";
            }

            return $issue->getNumber();
        }

        /**
         * Transforms a string (number) to an object (issue).
         *
         * @param  string $number
         * @return Issue|null
         * @throws TransformationFailedException if object (issue) is not found.
         */
        public function reverseTransform($number)
        {
            if (!$number) {
                return null;
            }

            $issue = $this->om
                ->getRepository('AcmeTaskBundle:Issue')
                ->findOneBy(array('number' => $number))
            ;

            if (null === $issue) {
                throw new TransformationFailedException(sprintf(
                    'Le problème avec le numéro "%s" ne peut pas être trouvé!',
                    $number
                ));
            }

            return $issue;
        }
    }


.. tip::

    Si vous voulez qu'un nouveau problème soit créé quand un numéro inconnu est saisi,
    vous pouvez l'instancier plutôt que de lancer l'exception ``TransformationFailedException``.

Utiliser le Convertisseur
-------------------------

Maintenant que le convertisseur est construit, il vous suffit juste de
l'ajouter à votre champ Issue dans un formulaire.

    Vous pouvez également utiliser les convertisseurs sans créer de nouveau
    type de champ de formulaire personnalisé en appelant ``addModelTransformer``
    (ou ``addViewTransformer``, lisez `Convertisseurs de modèle et de vue`_)
    sur n'importe quel constructeur de champ::

        use Symfony\Component\Form\FormBuilderInterface;
        use Acme\TaskBundle\Form\DataTransformer\IssueToNumberTransformer;

        class TaskType extends AbstractType
        {
            public function buildForm(FormBuilderInterface $builder, array $options)
            {
                // ...

                // cela suppose que le gestionnaire d'entité a été passé en option
                $entityManager = $options['em'];
                $transformer = new IssueToNumberTransformer($entityManager);

                // ajoute un champ texte normal, mais y ajoute aussi notre convertisseur
                $builder->add(
                    $builder->create('issue', 'text')
                        ->addModelTransformer($transformer)
                );
            }

            // ...
        }

Cet exemple requiert que vous ayez passé le gestionnaire d'entité en option
lors de la création du formulaire. Plus tard, vous apprendrez comment vous
pourriez créer un type de champ ``issue`` personnalisé pour éviter de devoir
faire cela dans votre contrôleur::

    $taskForm = $this->createForm(new TaskType(), $task, array(
        'em' => $this->getDoctrine()->getEntityManager(),
    ));

Cool, vous avez fini ! Votre utilisateur sera maintenant capable de saisir un numéro
de problème dans le champ texte, et il sera converti en un object Issue qui représente
ce problème. Cela signifie que, après avoir réussi à associer (bind) les données, le
framework Formulaire passera un véritable objet Issue à la méthode ``Task::setIssue()``
plutôt que son numéro.

Si le problème ne peut pas être retrouvé à partir de son numéro, une erreur sera
créée pour ce champ et le message de cette erreur peut être contrôlé grâce à l'option
de champ ``invalid_message``.

.. caution::

    Veuillez noter qu'ajouter le convertisseur requiert une syntaxe un peu plus
    complexe lorsque vous ajoutez un champ. L'exemple suivant est **incorrect**
    car le convertisseur serait appliqué au formulaire entier au lieu d'être juste
    appliqué au champ::

        // C'EST INCORRECT, LE CONVERTISSEUR SERA APPLIQUE A TOUT LE FORMULAIRE
        // regardez l'exemple ci-dessus pour connaitre la bonne syntaxe
        $builder->add('issue', 'text')
            ->addModelTransformer($transformer);

Convertisseurs de modèle et de vue
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    Les noms et méthodes des convertisseurs ont été changés dans Symfony 2.1.
    ``prependNormTransformer`` devient ``addModelTransformer`` et ``appendClientTransformer``
    devient ``addViewTransformer``.

Dans l'exemple ci-dessus, le convertisseur a été utilisé comme convertisseur de
« modèle ». En fait, il y a deux types différents de convertisseurs, et trois
types différents de données.

Dans tout formulaire, les trois types de données sont :

1) **Données modèle** - C'est une donnée dans le format utilisé dans votre application
(ex, un objet ``Issue``). Si vous appelez ``Form::getData`` ou ``Form::setData``, 
vous traitez avec la donnée « modèle ».

2) **Données normalisée** - C'est la version normalisée de votre donnée, et c'est
bien souvent la même que votre donnée « modèle » (mais pas dans notre exemple).
Elle n'est en général par utilisée directement.

3) **Donnée vue** - C'est le format qui est utilisé pour remplir les champs eux-mêmes.
C'est aussi le format dans lequel l'utilisateur soumettra ses données. Quand vous appelez
les méthodes ``Form::bind($data)``, la variable ``$data`` est dans le format de données
« vue ».

Les deux différents types de convertisseurs vous aident à faire des conversions
entre ces types de données :

**Convertisseurs modèle**:
    - ``transform``: « donnée modèle » => « donnée normalisée »
    - ``reverseTransform``: « donnée normalisée » => « donnée modèle »

**Convertisseurs vue**:
    - ``transform``: « donnée normalisée » => « donnée vue »
    - ``reverseTransform``: « donnée vue » => « donnée normalisée »

Le convertisseur que vous utiliserez dépendra de votre situation.

Pour utiliser le convertisseur vue, appelez ``addViewTransformer``.

Alors pourquoi avons nous utilisé le convertisseur modèle ?
-----------------------------------------------------------

Dans notre exemple, le champ est un champ ``texte``, et nous voulons
toujours qu'un champ texte soit un format simple, scalaire dans l'un des
formats « normalisé » ou « vue ». Pour cette raison, le convertisseur le plus
approprié était le convertisseur « modèle » (qui convertit un format *normalisé*,
le numéro du problème, en un format *modèle*, l'objet Issue, et inversement).

La différence entre les convertisseurs est subtile et vous devriez toujours
penser à ce que la donnée « normalisée » d'un champ devrait être. Par exemple,
la donnée « normalisée » d'un champ ``texte`` est une chaîne de caractères, mais
c'est un objet ``DateTime`` pour un champ ``date``.

Utiliser les convertisseurs dans un type de champ personnalisé
--------------------------------------------------------------

Dans l'exemple ci-dessus, vous aviez appliqué le convertisseur sur un champ
``texte`` normal. C'était facile mais cela a deux inconvénients :

1) Vous devez toujours vous souvenir d'appliquer le convertisseur lorsque vous ajoutez
des champs pour saisir des numéros de problème

2) Vous devez vous soucier de passer l'option ``em`` quand vous créez le
formulaire qui utilise le convertisseur.

Pour ces raisons, vous pourriez choisir de :doc:`créer un type de champ personnalisé</cookbook/form/create_custom_field_type>`.
Premièrement, créez la classe du type de champ personnalisé::

    // src/Acme/TaskBundle/Form/Type/IssueSelectorType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Acme\TaskBundle\Form\DataTransformer\IssueToNumberTransformer;
    use Doctrine\Common\Persistence\ObjectManager;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class IssueSelectorType extends AbstractType
    {
        /**
         * @var ObjectManager
         */
        private $om;

        /**
         * @param ObjectManager $om
         */
        public function __construct(ObjectManager $om)
        {
            $this->om = $om;
        }

        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $transformer = new IssueToNumberTransformer($this->om);
            $builder->addModelTransformer($transformer);
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'invalid_message' => 'The selected issue does not exist',
            ));
        }

        public function getParent()
        {
            return 'text';
        }

        public function getName()
        {
            return 'issue_selector';
        }
    }

Ensuite, enregistrez votre type comme service et taggez le avec ``form.type`` pour qu'il
soit reconnu comme type de champ personnalisé :

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_demo.type.issue_selector:
                class: Acme\TaskBundle\Form\Type\IssueSelectorType
                arguments: ["@doctrine.orm.entity_manager"]
                tags:
                    - { name: form.type, alias: issue_selector }

    .. code-block:: xml

        <service id="acme_demo.type.issue_selector" class="Acme\TaskBundle\Form\Type\IssueSelectorType">
            <argument type="service" id="doctrine.orm.entity_manager"/>
            <tag name="form.type" alias="issue_selector" />
        </service>

Maintenant, lorsque vous aurez besoin d'utiliser votre type de champ spécial ``issue_selector``,
ce sera très facile::

    // src/Acme/TaskBundle/Form/Type/TaskType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;

    class TaskType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('task')
                ->add('dueDate', null, array('widget' => 'single_text'));
                ->add('issue', 'issue_selector');
        }

        public function getName()
        {
            return 'task';
        }
    }