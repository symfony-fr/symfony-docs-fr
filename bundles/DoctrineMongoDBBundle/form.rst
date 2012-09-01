Comment implémenter un simple formulaire de création de compte avec MongoDB
===========================================================================

Certains formulaires ont des champs en plus, dont les valeurs n'ont
pas besoin d'être stockées en base de données. Par exemple, vous pourriez
vouloir créer un formulaire de création de compte avec des champs en plus
(comme par exemple une case à cocher « Accepter les conditions ») et imbriquer
le formulaire qui contient les informations relatives au compte. Nous
utiliserons MongoDB pour stocker les données.

Un modèle simple : User
-----------------------

Dans ce tutoriel, nous commençons avec le modèle d'un document ``User``::

    // src/Acme/AccountBundle/Document/User.php
    namespace Acme\AccountBundle\Document;

    use Doctrine\ODM\MongoDB\Mapping\Annotations as MongoDB;
    use Symfony\Component\Validator\Constraints as Assert;
    use Doctrine\Bundle\MongoDBBundle\Validator\Constraints\Unique as MongoDBUnique;

    /**
     * @MongoDB\Document(collection="users")
     * @MongoDBUnique(fields="email")
     */
    class User
    {
        /**
         * @MongoDB\Id
         */
        protected $id;

        /**
         * @MongoDB\Field(type="string")
         * @Assert\NotBlank()
         * @Assert\Email()
         */
        protected $email;

        /**
         * @MongoDB\Field(type="string")
         * @Assert\NotBlank()
         */
        protected $password;

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

        public function getPassword()
        {
            return $this->password;
        }

        // cryptage simpliste uniquement pour l'exemple (ne pas copier SVP !)
        public function setPassword($password)
        {
            $this->password = sha1($password);
        }
    }

Ce document ``User`` contient trois champs et deux d'entre eux (``email`` et
``password``) doivent être affichés dans le formulaire. La propriété email
doit être unique dans la base de données, ce qui est réalisé par l'ajout d'une
validation au début de la classe.

.. note::

    Si vous voulez intégrer cet utilisateur dans le système de sécurité, vous
    devez implémenter l'interface :ref:`UserInterface<book-security-user-entity>`
    du composant « Security ».

Créer un formulaire pour le modèle
----------------------------------

Ensuite, créez le formulaire pour le modèle ``User``::

    // src/Acme/AccountBundle/Form/Type/UserType.php
    namespace Acme\AccountBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\Extension\Core\Type\RepeatedType;
    use Symfony\Component\Form\FormBuilder;

    class UserType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('email', 'email');
            $builder->add('password', 'repeated', array(
               'first_name' => 'password',
               'second_name' => 'confirm',
               'type' => 'password'
            ));
        }

        public function getDefaultOptions(array $options)
        {
            return array('data_class' => 'Acme\AccountBundle\Document\User');
        }

        public function getName()
        {
            return 'user';
        }
    }

Nous n'avons ajouté que deux champs : ``email`` et ``plainPassword`` (dupliqué
pour confirmer le mot de passe saisi). L'option ``data_class`` spécifie au formulaire
le nom de la classe associée (c'est-à-dire votre document ``User``).

.. tip::

    Pour en savoir plus sur le composant Formulaire, lisez :doc:`/book/forms`.

Imbriquer le formulaire User dans le formulaire de création de compte
---------------------------------------------------------------------

Le formulaire que vous utiliserez pour la page de création de compte n'est
pas le même que le formulaire qui est utilisé pour modifier simplement l'objet
``User`` (c'est-à-dire ``UserType``). Le formulaire de création de compte contiendra
quelques champs supplémentaires, comme « Accepter les conditions », dont la valeur
ne sera pas stockée en base de données.

En d'autres termes, créez un second formulaire pour la création de compte, qui
imbrique le formulaire ``User`` et ajoute les champs supplémentaires dont vous
avez besoin. Commencez par créer une classe simple qui représente la « création
de compte » (« registration » en anglais)::

    // src/Acme/AccountBundle/Form/Model/Registration.php
    namespace Acme\AccountBundle\Form\Model;

    use Symfony\Component\Validator\Constraints as Assert;

    use Acme\AccountBundle\Document\User;

    class Registration
    {
        /**
         * @Assert\Type(type="Acme\AccountBundle\Document\User")
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
            $this->termsAccepted = (boolean)$termsAccepted;
        }
    }

Ensuite, créez le formulaire pour ce modèle ``Registration``::

    // src/Acme/AccountBundle/Form/Type/RegistrationType.php
    namespace Acme\AccountBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\Extension\Core\Type\RepeatedType;
    use Symfony\Component\Form\FormBuilder;

    class RegistrationType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
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
``Registration.user`` contienne une instance de la classe ``UserType``.

Gérer la soumission du formulaire
---------------------------------

Ensuite, vous aurez besoin d'un contrôleur pour prendre en charge le formulaire.
Commencez par créer un contrôleur simple pour afficher le formulaire de création
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
se chargera de la validation, et enregistrera les données dans MongoDB::

    public function createAction()
    {
        $dm = $this->get('doctrine_mongodb')->getManager();

        $form = $this->createForm(new RegistrationType(), new Registration());

        $form->bindRequest($this->getRequest());

        if ($form->isValid()) {
            $registration = $form->getData();

            $dm->persist($registration->getUser());
            $dm->flush();

            return $this->redirect(...);
        }

        return $this->render('AcmeAccountBundle:Account:register.html.twig', array('form' => $form->createView()));
    }

C'est tout ! Maintenant, votre formulaire vous permet de valider et d'enregistrer
un objet ``User`` dans MongoDB.
