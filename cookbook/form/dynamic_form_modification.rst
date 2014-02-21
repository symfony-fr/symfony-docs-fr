.. index::
   single: Form; Events

Comment modifier dynamiquement les formulaires en utilisant les évènements
==========================================================================

Souvent, un formulaire ne pourra pas être créé de façon statique. Dans cet
article, vous apprendrez à personnaliser vos formulaires selon trois cas
courants:

1) :ref:`cookbook-form-events-underlying-data`

Exemple: vous avez un formulaire "Product" et vous devez modifier/ajouter/supprimer
un champ selon les données de l 'objet avec lequel vous travaillez.

2) :ref:`cookbook-form-events-user-data`

Exemple: vous créez un formulaire "Message" et vous devez construire une liste
déroulante qui ne contient que les utilisateurs qui sont amis avec l'utilisateur
*actuellement* authentifié.

3) :ref:`cookbook-form-events-submitted-data`

Exemple: dans un formulaire d'inscription, vous avez un champ "pays" et un champ "état"
que vous voulez alimenter selon la valeur du champ "pays".

.. _cookbook-form-events-underlying-data:

Customizing your Form based on the underlying Data
--------------------------------------------------

Avant de se lancer directement dans la génération dynamique de formulaire,
commençons par rappeler ce à quoi ressemble une classe de formulaire
basique::

    // src/Acme/DemoBundle/Form/Type/ProductType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('name');
            $builder->add('price');
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'Acme\DemoBundle\Entity\Product'
            ));
        }

        public function getName()
        {
            return 'product';
        }
    }

.. note::

    Si la section de code ci-dessus ne vous est pas familière, vous avez
    probablement besoin de lire d'abord :doc:`le chapitre sur les
    Formulaires </book/forms>` avant d'aller plus loin.


Supposons un instant que ce formulaire utilise une classe imaginaire
« Product » qui possède uniquement deux propriétés (« name » et « price »).
Le formulaire généré à partir de cette classe sera identique que ce soit
pour la création d'un Produit ou pour l'édition d'un Produit existant (par
exemple : un Produit récupéré depuis la base de données).

Supposons maintenant que vous ne souhaitiez pas que l'utilisateur puisse
changer la valeur de ``name`` une fois que l'objet a été créé. Pour faire
cela, vous pouvez utiliser le
:doc:`Répartiteur d'Évènements</components/event_dispatcher/introduction>`
de Symfony pour analyser les données de l'objet et modifier le formulaire
en se basant sur les données de l'objet Product. Dans cet article, vous
allez apprendre comment ajouter ce niveau de flexibilité à vos formulaires.

.. _`cookbook-forms-event-listener`:

Ajouter un écouteur d'évènement à une classe de formulaire
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Donc, au lieu d'ajouter directement ce widget `name``, la responsabilité
de créer ce champ en particulier est laissée à un écouteur d'évènement::

    // src/Acme/DemoBundle/Form/Type/ProductType.php
    namespace Acme\DemoBundle\Form\Type;

    // ...
    use Symfony\Component\Form\FormEvent;
    use Symfony\Component\Form\FormEvents;

    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('price');

            $builder->addEventListener(FormEvents::PRE_SET_DATA, function(FormEvent $event) {
                // ... adding the name field if needed
            });
        }

        // ...
    }

Le but est de créer un champ ``name`` *uniquement* si l'objet ``Product`` sous-jacent
est nouveau (par exemple : n'a pas été persisté dans la base de données). Partant
de ce principe, l'écouteur d'évènement pourrait ressembler à quelque chose comme ça::


    // ...
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        // ...
        $builder->addEventListener(FormEvents::PRE_SET_DATA, function(FormEvent $event){
            $product = $event->getData();
            $form = $event->getForm();

            // vérifie si l'objet Product est "nouveau"
            // Si aucune donnée n'est passée au formulaire, la donnée est "null".
            // Ce doit être considéré comme un nouveau "Product"
            if (!$product || null === $product->getId()) {
                $form->add('name', 'text');
            }
        });
    }

.. versionadded:: 2.2
    La possibilité de passer une chaine de caractères à
    :method:`FormInterface::add <Symfony\\Component\\Form\\FormInterface::add>`
    est une nouveauté de Symfony 2.2.

.. note::
    Vous pouvez bien sûr utiliser n'importe quel type de callback au lieu d'une closure,
    par exemple une méthode de l'objet `ProductType`` pour une meilleur lisibilité::

        // ...
        class ProductType extends AbstractType
        {
            public function buildForm(FormBuilderInterface $builder, array $options)
            {
                // ...
                $builder->addEventListener(FormEvents::PRE_SET_DATA, array($this, 'onPreSetData'));
            }

            public function onPreSetData(FormEvent $event){
                // ...
            }
        }

.. note::

    La ligne ``FormEvents::PRE_SET_DATA`` est convertie en
    ``form.pre_set_data``. La classe :class:`Symfony\\Component\\Form\\FormEvents`
    a un but organisationnel. C'est un endroit centralisé où vous trouverez
    la liste des différents évènements de formulaire disponibles. Vous pouvez
    voir la liste complète des évènement de formulaire dans la classe
    :class:`Symfony\\Component\\Form\\FormEvents`.

.. _`cookbook-forms-event-subscriber`:

Adding an Event Subscriber to a Form Class
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For better reusability or if there is some heavy logic in your event listener,
you can also move the logic for creating the ``name`` field to an
:ref:`event subscriber <event_dispatcher-using-event-subscribers>`::

    // src/Acme/DemoBundle/Form/Type/ProductType.php
    namespace Acme\DemoBundle\Form\Type;

    // ...
    use Acme\DemoBundle\Form\EventListener\AddNameFieldSubscriber;

    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('price');

            $builder->addEventSubscriber(new AddNameFieldSubscriber());
        }

        // ...
    }

Now the logic for creating the ``name`` field resides in it own subscriber
class::

    // src/Acme/DemoBundle/Form/EventListener/AddNameFieldSubscriber.php
    namespace Acme\DemoBundle\Form\EventListener;

    use Symfony\Component\Form\FormEvent;
    use Symfony\Component\Form\FormEvents;
    use Symfony\Component\EventDispatcher\EventSubscriberInterface;

    class AddNameFieldSubscriber implements EventSubscriberInterface
    {
        public static function getSubscribedEvents()
        {
            // Tells the dispatcher that you want to listen on the form.pre_set_data
            // event and that the preSetData method should be called.
            return array(FormEvents::PRE_SET_DATA => 'preSetData');
        }

        public function preSetData(FormEvent $event)
        {
            $product = $event->getData();
            $form = $event->getForm();

            if (!$product || null === $product->getId()) {
                $form->add('name', 'text');
            }
        }
    }


.. _cookbook-form-events-user-data:

How to Dynamically Generate Forms based on user Data
----------------------------------------------------

Sometimes you want a form to be generated dynamically based not only on data
from the form but also on something else - like some data from the current user.
Suppose you have a social website where a user can only message people marked
as friends on the website. In this case, a "choice list" of whom to message
should only contain users that are the current user's friends.

Creating the Form Type
~~~~~~~~~~~~~~~~~~~~~~

Using an event listener, your form might look like this::

    // src/Acme/DemoBundle/Form/Type/FriendMessageFormType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\Form\FormEvents;
    use Symfony\Component\Form\FormEvent;
    use Symfony\Component\Security\Core\SecurityContext;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class FriendMessageFormType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('subject', 'text')
                ->add('body', 'textarea')
            ;
            $builder->addEventListener(FormEvents::PRE_SET_DATA, function(FormEvent $event){
                // ... add a choice list of friends of the current application user
            });
        }

        public function getName()
        {
            return 'acme_friend_message';
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
        }
    }

The problem is now to get the current user and create a choice field that
contains only this user's friends.

Luckily it is pretty easy to inject a service inside of the form. This can be
done in the constructor::

    private $securityContext;

    public function __construct(SecurityContext $securityContext)
    {
        $this->securityContext = $securityContext;
    }

.. note::

    You might wonder, now that you have access to the User (through the security
    context), why not just use it directly in ``buildForm`` and omit the
    event listener? This is because doing so in the ``buildForm`` method
    would result in the whole form type being modified and not just this
    one form instance. This may not usually be a problem, but technically
    a single form type could be used on a single request to create many forms
    or fields.

Customizing the Form Type
~~~~~~~~~~~~~~~~~~~~~~~~~

Now that you have all the basics in place you can take advantage of the ``SecurityContext``
and fill in the listener logic::

    // src/Acme/DemoBundle/FormType/FriendMessageFormType.php

    use Symfony\Component\Security\Core\SecurityContext;
    use Doctrine\ORM\EntityRepository;
    // ...

    class FriendMessageFormType extends AbstractType
    {
        private $securityContext;

        public function __construct(SecurityContext $securityContext)
        {
            $this->securityContext = $securityContext;
        }

        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('subject', 'text')
                ->add('body', 'textarea')
            ;

            // grab the user, do a quick sanity check that one exists
            $user = $this->securityContext->getToken()->getUser();
            if (!$user) {
                throw new \LogicException(
                    'The FriendMessageFormType cannot be used without an authenticated user!'
                );
            }

            $builder->addEventListener(
                FormEvents::PRE_SET_DATA,
                function(FormEvent $event) use ($user) {
                    $form = $event->getForm();

                    $formOptions = array(
                        'class' => 'Acme\DemoBundle\Entity\User',
                        'property' => 'fullName',
                        'query_builder' => function(EntityRepository $er) use ($user) {
                            // build a custom query
                            // return $er->createQueryBuilder('u')->addOrderBy('fullName', 'DESC');

                            // or call a method on your repository that returns the query builder
                            // the $er is an instance of your UserRepository
                            // return $er->createOrderByFullNameQueryBuilder();
                        },
                    );

                    // create the field, this is similar the $builder->add()
                    // field name, field type, data, options
                    $form->add('friend', 'entity', $formOptions);
                }
            );
        }

        // ...
    }

.. note::

    The ``multiple`` and ``expanded`` form options will default to false
    because the type of the friend field is ``entity``.

Using the Form
~~~~~~~~~~~~~~

Our form is now ready to use and there are two possible ways to use it inside
of a controller:

a) create it manually and remember to pass the security context to it;

or

b) define it as a service.

a) Creating the Form manually
.............................

This is very simple, and is probably the better approach unless you're using
your new form type in many places or embedding it into other forms::

    class FriendMessageController extends Controller
    {
        public function newAction(Request $request)
        {
            $securityContext = $this->container->get('security.context');
            $form = $this->createForm(
                new FriendMessageFormType($securityContext)
            );

            // ...
        }
    }

b) Defining the Form as a Service
.................................

To define your form as a service, just create a normal service and then tag
it with :ref:`dic-tags-form-type`.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            acme.form.friend_message:
                class: Acme\DemoBundle\Form\Type\FriendMessageFormType
                arguments: ["@security.context"]
                tags:
                    - { name: form.type, alias: acme_friend_message }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
            <service id="acme.form.friend_message" class="Acme\DemoBundle\Form\Type\FriendMessageFormType">
                <argument type="service" id="security.context" />
                <tag name="form.type" alias="acme_friend_message" />
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        $definition = new Definition('Acme\DemoBundle\Form\Type\FriendMessageFormType');
        $definition->addTag('form.type', array('alias' => 'acme_friend_message'));
        $container->setDefinition(
            'acme.form.friend_message',
            $definition,
            array('security.context')
        );

If you wish to create it from within a controller or any other service that has
access to the form factory, you then use::

    use Symfony\Component\DependencyInjection\ContainerAware;

    class FriendMessageController extends ContainerAware
    {
        public function newAction(Request $request)
        {
            $form = $this->get('form.factory')->create('acme_friend_message');

            // ...
        }
    }

If you extend the ``Symfony\Bundle\FrameworkBundle\Controller\Controller`` class, you can simply call::

    $form = $this->createForm('acme_friend_message');

You can also easily embed the form type into another form::

    // inside some other "form type" class
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $builder->add('message', 'acme_friend_message');
    }

.. _cookbook-form-events-submitted-data:

Dynamic generation for submitted Forms
--------------------------------------

Another case that can appear is that you want to customize the form specific to
the data that was submitted by the user. For example, imagine you have a registration
form for sports gatherings. Some events will allow you to specify your preferred
position on the field. This would be a ``choice`` field for example. However the
possible choices will depend on each sport. Football will have attack, defense,
goalkeeper etc... Baseball will have a pitcher but will not have a goalkeeper. You
will need the correct options in order for validation to pass.

The meetup is passed as an entity field to the form. So we can access each
sport like this::

    // src/Acme/DemoBundle/Form/Type/SportMeetupType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\Form\FormEvent;
    use Symfony\Component\Form\FormEvents;
    // ...

    class SportMeetupType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('sport', 'entity', array(...))
            ;

            $builder->addEventListener(
                FormEvents::PRE_SET_DATA,
                function(FormEvent $event) {
                    $form = $event->getForm();

                    // this would be your entity, i.e. SportMeetup
                    $data = $event->getData();

                    $positions = $data->getSport()->getAvailablePositions();

                    $form->add('position', 'entity', array('choices' => $positions));
                }
            );
        }
    }

When you're building this form to display to the user for the first time,
then this example works perfectly.

However, things get more difficult when you handle the form submission. This
is because the ``PRE_SET_DATA`` event tells us the data that you're starting
with (e.g. an empty ``SportMeetup`` object), *not* the submitted data.

On a form, we can usually listen to the following events:

* ``PRE_SET_DATA``
* ``POST_SET_DATA``
* ``PRE_SUBMIT``
* ``SUBMIT``
* ``POST_SUBMIT``

.. versionadded:: 2.3
    The events ``PRE_SUBMIT``, ``SUBMIT`` and ``POST_SUBMIT`` were added in
    Symfony 2.3. Before, they were named ``PRE_BIND``, ``BIND`` and ``POST_BIND``.

.. versionadded:: 2.2.6
    The behavior of the ``POST_SUBMIT`` event changed slightly in 2.2.6, which the
    below example uses.

The key is to add a ``POST_SUBMIT`` listener to the field that your new field
depends on. If you add a ``POST_SUBMIT`` listener to a form child (e.g. ``sport``),
and add new children to the parent form, the Form component will detect the
new field automatically and map it to the submitted client data.

The type would now look like::

    // src/Acme/DemoBundle/Form/Type/SportMeetupType.php
    namespace Acme\DemoBundle\Form\Type;

    // ...
    use Acme\DemoBundle\Entity\Sport;
    use Symfony\Component\Form\FormInterface;

    class SportMeetupType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('sport', 'entity', array(...))
            ;

            $formModifier = function(FormInterface $form, Sport $sport) {
                $positions = $sport->getAvailablePositions();

                $form->add('position', 'entity', array('choices' => $positions));
            };

            $builder->addEventListener(
                FormEvents::PRE_SET_DATA,
                function(FormEvent $event) use ($formModifier) {
                    // this would be your entity, i.e. SportMeetup
                    $data = $event->getData();

                    $formModifier($event->getForm(), $data->getSport());
                }
            );

            $builder->get('sport')->addEventListener(
                FormEvents::POST_SUBMIT,
                function(FormEvent $event) use ($formModifier) {
                    // It's important here to fetch $event->getForm()->getData(), as
                    // $event->getData() will get you the client data (that is, the ID)
                    $sport = $event->getForm()->getData();

                    // since we've added the listener to the child, we'll have to pass on
                    // the parent to the callback functions!
                    $formModifier($event->getForm()->getParent(), $sport);
                }
            );
        }
    }

You can see that you need to listen on these two events and have different callbacks
only because in two different scenarios, the data that you can use is available in different events.
Other than that, the listeners always perform exactly the same things on a given form.

One piece that may still be missing is the client-side updating of your form
after the sport is selected. This should be handled by making an AJAX call
back to your application. In that controller, you can submit your form, but
instead of processing it, simply use the submitted form to render the updated
fields. The response from the AJAX call can then be used to update the view.

.. _cookbook-dynamic-form-modification-suppressing-form-validation:

Suppressing Form Validation
---------------------------

To suppress form validation you can use the ``POST_SUBMIT`` event and prevent
the :class:`Symfony\\Component\\Form\\Extension\\Validator\\EventListener\\ValidationListener`
from being called.

The reason for needing to do this is that even if you set ``group_validation``
to ``false`` there  are still some integrity checks executed. For example
an uploaded file will still be checked to see if it is too large and the form
will still check to see if non-existing fields were submitted. To disable
all of this, use a listener::

    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\Form\FormEvents;

    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $builder->addEventListener(FormEvents::POST_SUBMIT, function($event) {
            $event->stopPropagation();
        }, 900); // Always set a higher priority than ValidationListener

        // ...
    }

.. caution::

    By doing this, you may accidentally disable something more than just form
    validation, since the ``POST_SUBMIT`` event may have other listeners.