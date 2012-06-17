Utiliser des Convertisseurs de Données
======================================

Vous allez souvent éprouver le besoin de transformer les données entrées par
l'utilisateur dans un formulaire en quelque chose d'autre qui sera utilisé
dans votre programme. Vous pourriez effectuer ceci manuellement dans votre
contrôleur, mais que se passe-t-il si vous voulez utiliser ce formulaire
spécifique à différents endroits ?

Disons que vous ayez une relation « one-to-one » de « Task » (« Tâche » en français) pour
« Issue » (« Problème » en français), par exemple : une tâche possède de manière
optionnelle un problème qui lui est lié. Ajouter un élément « listbox » contenant
tous les problèmes possibles peut éventuellement engendrer une très long liste dans
laquelle il est impossible de trouver quelque chose. Vous voudrez plutôt ajouter
un champ texte dans lequel l'utilisateur peut simplement entrer le numéro du problème.
Dans le contrôleur, vous pouvez convertir ce numéro de problème en une tâche, et
éventuellement ajouter des erreurs au formulaire si elle n'a pas été trouvée, mais
bien sûr, cela n'est pas très propre.

Cela serait mieux si ce problème était automatiquement converti en un objet « Issue »,
afin d'être utilisé dans votre action. C'est ici que les Convertisseurs de Données
entrent en jeu.

Tout d'abord, créez un type de formulaire personnalisé qui a un Convertisseur de
Données lui étant attaché, et qui retourne le problème par numéro : le type
« sélecteur de problème ». Eventuellement, cela sera simplement un champ texte, comme
nous configurons le parent des champs comme étant un champ texte, dans lequel vous
pourrez entrer le numéro du problème. Le champ affichera une erreur si un numéro
non-existant est entré::

    // src/Acme/TaskBundle/Form/Type/IssueSelectorType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;
    use Acme\TaskBundle\Form\DataTransformer\IssueToNumberTransformer;
    use Doctrine\Common\Persistence\ObjectManager;

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
            $builder->addViewTransformer($transformer);
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

.. tip::

    Vous pouvez aussi utiliser les convertisseurs sans avoir à créer un nouveau
    type de formulaire personnalisé en appelant ``appendClientTransformer`` sur
    n'importe quel constructeur de champ::

        use Symfony\Component\Form\FormBuilderInterface;
        use Acme\TaskBundle\Form\DataTransformer\IssueToNumberTransformer;

        class TaskType extends AbstractType
        {
            public function buildForm(FormBuilderInterface $builder, array $options)
            {
                // ...

                // ceci assume que le gestionnaire d'entité a été passé en tant qu'option
                $entityManager = $options['em'];
                $transformer = new IssueToNumberTransformer($entityManager);

                // utilise un champ texte normal, mais transforme le texte en un objet issue
                $builder
                    ->add('issue', 'text')
                    ->addViewTransformer($transformer)
                ;
            }

            // ...
        }

Ensuite, nous créons le convertisseur de données, qui se charge d'effectuer la
conversion::

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
         * Transforme un objet (issue) en une chaîne de caractères (nombre)
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
         * Transforme une chaîne de caractères (nombre) en un objet (issue)
         *
         * @param  string $number
         * @return Issue|null
         * @throws TransformationFailedException si l'objet (issue) n'est pas trouvé.
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
                    'An issue with number "%s" does not exist!',
                    $number
                ));
            }

            return $issue;
        }
    }

Finalement, puisque nous avons décidé de créer un type de formulaire personnalisé
qui utilise le convertisseur de données, déclarez le Type dans le conteneur de
service, afin que le gestionnaire d'entité puisse automatiquement être injecté :

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

Vous pouvez désormais ajouter le type à votre formulaire via son alias
comme suit::

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
                ->add('issue', 'issue_selector')
            ;
        }

        public function getName()
        {
            return 'task';
        }
    }

Maintenant, cela va être très facile d'utiliser ce type « sélecteur » à n'importe
quel endroit dans votre application pour sélectionner un problème par son numéro.
Aucune logique ne doit être ajoutée à votre contrôleur.

Si vous voulez qu'un nouveau problème (« issue ») soit créé lorsqu'un numéro
inconnu est soumis, vous pouvez l'instancier plutôt que de lancer l'exception
TransformationFailedException, et même le persister dans votre gestionnaire
d'entité si la tâche n'a pas d'options de « cascade » pour ce problème.
