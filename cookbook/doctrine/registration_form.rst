.. index::
   single: Doctrine; Simple Registration Form
   single: Form; Simple Registration Form

Comment implémenter un simple formulaire de création de compte
==============================================================

Certains formulaires ont des champs en plus, dont les valeurs n'ont
pas besoin d'être stockées en base de données. Par exemple, vous pourriez
vouloir créer un formulaire de création de compte avec des champs en plus
(comme par exemple une checkbox « Accepter les conditions ») et imbriquer
le formulaire qui contient les informations relatives au compte.

Un modèle simple : User
-----------------------

Vous avez une entité ``User`` simple associée à la base de données::

    // src/Acme/AccountBundle/Entity/User.php
    namespace Acme\AccountBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Symfony\Component\Validator\Constraints as Assert;
    use Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity;

    /**
     * @ORM\Entity
     * @UniqueEntity(fields="email", message="Email already taken")
     */
    class User
    {
        /**
         * @ORM\Id
         * @ORM\Column(type="integer")
         * @ORM\GeneratedValue(strategy="AUTO")
         */
        protected $id;

        /**
         * @ORM\Column(type="string", length=255)
         * @Assert\NotBlank()
         * @Assert\Email()
         */
        protected $email;

        /**
         * @ORM\Column(type="string", length=255)
         * @Assert\NotBlank()
         */
        protected $plainPassword;

        public function getId()
        {
            return $this->id;
        }

        public function getEmail()
        {
            return $this->email;
        }

        public function setEmail($email)
        {
            $this->email = $email;
        }

        public function getPlainPassword()
        {
            return $this->plainPassword;
        }

        public function setPlainPassword($password)
        {
            $this->plainPassword = $password;
        }
    }

Cette entité ``User`` contient trois champs et deux d'entre eux (``email`` et
``plainPassword``) doivent être affichés dans le formulaire. La propriété email
doit être unique dans la base de données, ce qui est réalisé par l'ajout d'une
validation au début de la classe.

.. note::

    Si vous voulez intégrer cet utilisateur dans le système de sécurité, vous
    devez implémenter la :ref:`UserInterface<book-security-user-entity>` du
    composant Security.

Créer un formulaire pour le modèle
----------------------------------

Ensuite, créez le formulaire pour le modèle ``User``::

    // src/Acme/AccountBundle/Form/Type/UserType.php
    namespace Acme\AccountBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class UserType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('email', 'email');
            $builder->add('plainPassword', 'repeated', array(
               'first_name' => 'password',
               'second_name' => 'confirm',
               'type' => 'password',
            ));
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'Acme\AccountBundle\Entity\User'
            ));
        }

        public function getName()
        {
            return 'user';
        }
    }

Il n'y a que deux champs : ``email`` et ``plainPassword`` (dupliqué pour confirmer
le mot de passe saisi). L'option ``data_class`` spécifie au formulaire le nom de
la classe associée (c-a-d l'entité ``User``).

.. tip::

    Pour en savoir plus sur le composant Formulaire, lisez :doc:`/book/forms`.

Imbriquer le formulaire User dans le formulaire de création de compte
---------------------------------------------------------------------

Le formulaire que vous utiliserez pour la page de création de compte n'est
pas le même que le formulaire qui est utilisé pour modifier simplement l'objet
``User`` (c-a-d ``UserType``). Le formulaire de création de compte contiendra
quelques champs supplémentaires, comme « Accepter les conditions », dont les valeurs
ne seront pas stockées en base de données.

Commencez par créer une simple classe qui représente la « création de compte »
(« registration » en anglais)::

    // src/Acme/AccountBundle/Form/Model/Registration.php
    namespace Acme\AccountBundle\Form\Model;

    use Symfony\Component\Validator\Constraints as Assert;

    use Acme\AccountBundle\Entity\User;

    class Registration
    {
        /**
         * @Assert\Type(type="Acme\AccountBundle\Entity\User")
         */
        protected $user;

        /**
         * @Assert\NotBlank()
         * @Assert\True()
         */
        protected $termsAccepted;

        public function setUser(User $user)
        {
            $this->user = $user;
        }

        public function getUser()
        {
            return $this->user;
        }

        public function getTermsAccepted()
        {
            return $this->termsAccepted;
        }

        public function setTermsAccepted($termsAccepted)
        {
            $this->termsAccepted = (Boolean) $termsAccepted;
        }
    }

Ensuite, créez le formulaire pour ce modèle ``Registration``::

    // src/Acme/AccountBundle/Form/Type/RegistrationType.php
    namespace Acme\AccountBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;

    class RegistrationType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('user', new UserType());
            $builder->add('terms', 'checkbox', array('property_path' => 'termsAccepted'));
        }

        public function getName()
        {
            return 'registration';
        }
    }

Vous n'avez pas besoin d'utiliser de méthode spéciale pour imbriquer le
formulaire ``UserType``. Un formulaire est aussi un champ, donc vous pouvez
l'ajouter comme n'importe quel champ, avec l'objectif que la propriété
``Registration.user`` contienne une instance de la classe ``User``.

Gérer la soumission du formulaire
---------------------------------

Ensuite, vous aurez besoin d'un contrôleur pour prendre en charge le formulaire.
Commencez par créer un simple contrôleur pour afficher le formulaire de création
de compte::

    // src/Acme/AccountBundle/Controller/AccountController.php
    namespace Acme\AccountBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Response;

    use Acme\AccountBundle\Form\Type\RegistrationType;
    use Acme\AccountBundle\Form\Model\Registration;

    class AccountController extends Controller
    {
        public function registerAction()
        {
            $form = $this->createForm(new RegistrationType(), new Registration());

            return $this->render('AcmeAccountBundle:Account:register.html.twig', array('form' => $form->createView()));
        }
    }

et son template :

.. code-block:: html+jinja

    {# src/Acme/AccountBundle/Resources/views/Account/register.html.twig #}
    <form action="{{ path('create')}}" method="post" {{ form_enctype(form) }}>
        {{ form_widget(form) }}

        <input type="submit" />
    </form>

Enfin, créez le contrôleur qui prendra en charge la soumission du formulaire. Il
se chargera de la validation, et enregistrera les données dans la base de données::

    public function createAction()
    {
        $em = $this->getDoctrine()->getEntityManager();

        $form = $this->createForm(new RegistrationType(), new Registration());

        $form->handleRequest($this->getRequest());

        if ($form->isValid()) {
            $registration = $form->getData();

            $em->persist($registration->getUser());
            $em->flush();

            return $this->redirect(...);
        }

        return $this->render('AcmeAccountBundle:Account:register.html.twig', array('form' => $form->createView()));
    }

C'est tout ! Maintenant, votre formulaire valide et vous permet d'enregistrer
un objet ``User`` dans la base de données. La checkbox supplémentaire ``terms``
du modèle ``Registration`` est utilisée durant la validation, mais n'est plus
utilisée ensuite lors de l'enregistrement de l'utilisateur en base de données.
