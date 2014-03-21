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

Peronnaliser un formulaire en se basant sur les données
-------------------------------------------------------

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

Ajouter un souscripteur d'évènement sur un formulaire
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour une meilleur réutilisabilité ou s'il y a trop de logique dans votre
écouteur d'évènement, vous pouvez également déplacer la logique qui crée
le champ ``name`` dans un :ref:`souscripteur d'évènements <event_dispatcher-using-event-subscribers>`::

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

Maintenant, la logique qui crée le champ ``name`` est située dans sa propre
classe de souscripteur::

    // src/Acme/DemoBundle/Form/EventListener/AddNameFieldSubscriber.php
    namespace Acme\DemoBundle\Form\EventListener;

    use Symfony\Component\Form\FormEvent;
    use Symfony\Component\Form\FormEvents;
    use Symfony\Component\EventDispatcher\EventSubscriberInterface;

    class AddNameFieldSubscriber implements EventSubscriberInterface
    {
        public static function getSubscribedEvents()
        {
            // Dit au dispatcher que vous voulez écouter l'évènement
            // form.pre_set_data et que la méthode preSetData doit être appelée
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

Comment gérer des formulaires en se basant sur les données utilisateur
----------------------------------------------------------------------

Parfois, vous voulez qu'un formulaire soit généré dynamiquement en se basant
sur les données du formulaire mais aussi sur quelque chose d'autre - par exemple
des données de l'utilisateur connecté.
Supposons que vous travaillez sur un site social où un utilisateur ne peut envoyer
des messages qu'à des personnes marquées comme amies sur ce site. Dans ce cas, la
liste des personnes à qui l'utilisateur peut envoyer des messages ne doit contenir
que des amis.

Créer le type de formulaire
~~~~~~~~~~~~~~~~~~~~~~~~~~~

En utilisant un écouteur d'évènements, votre formulaire
pourrait ressembler à ceci::

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
                // ... Ajouter une liste de choix d'amis de l'utilisateur connecté
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

Le problème est maintenant de récupérer l'utilisateur actuel et de créer un
champ select qui ne contient que des amis de l'utilisateur.

Heureusement, il est assez facile d'injecter un service dans le formulaire.
Cela peut être fait dans le constructeur::

    private $securityContext;

    public function __construct(SecurityContext $securityContext)
    {
        $this->securityContext = $securityContext;
    }

.. note::

    Vous devez vous demander, maintenant que vous avez accès à l'utilisateur (au
    travers du security context), pourquoi ne pas utiliser directement 
    ``buildForm`` et éviter de passer par un écouteur d'évènement? C'est
    parce que le faire dans la méthode ``buildForm`` signifierait que tout
    le type de formulaire sera modifié et non pas juste l'instance de formulaire
    que vous voulez. Ce n'est généralement pas très grave, mais techniquement,
    un seul tupe de formulaire pourrait être utilisé dans une seule requête
    pour créer plusieurs formulaires ou champs

Personnaliser le type de formulaire
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Maintenant que vous avez toutes les bases, vous pouvez tirer avantage du ``SecurityContext``
et remplir votre écouteur::

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

            // récupère le user et vérifie rapidement qu'il existe bien
            $user = $this->securityContext->getToken()->getUser();
            if (!$user) {
                throw new \LogicException(
                    'Le FriendMessageFormType ne peut pas être utilisé sans utilisateur connecté!'
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
                            // construit une requête personnalisée
                            // retourne $er->createQueryBuilder('u')->addOrderBy('fullName', 'DESC');

                            // ou appelle une méthode d'un repository qui retourne un query builder
                            // l'instance $er est une instance de UserRepository
                            // retourne $er->createOrderByFullNameQueryBuilder();
                        },
                    );

                    // crée le champ, cela équivaut à  $builder->add()
                    // nom du champ, type de champ, donnée, options
                    $form->add('friend', 'entity', $formOptions);
                }
            );
        }

        // ...
    }

.. note::

    Les options ``multiple`` et ``expanded`` valent false par défaut
    car le type de champ est ``entity``.

Utiliser le formulaire
~~~~~~~~~~~~~~~~~~~~~~

Votre formulaire est maintenant prêt à être utilisé et il y a deux manières
possibles de l'utiliser dans un contrôleur :

a) le créer manuellement et y passer le security context;

ou

b) le définir comme service.

a) Créer le formulaire manuellement
...................................

C'est très simple, et probablement la meilleur approche à moins que vous
n'utilisiez votre nouveau type de champ à plusieurs endroits ou que
vous l'imbriquez dans d'autres formulaires::

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

b) Definir le formulaire comme service
......................................

Pour définir votre formulaire comme service, créez simplement un service
classique que vous taggerez avec :ref:`dic-tags-form-type`.

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

Si vous désirez le créer dans un contrôleur ou dans n'importe quel autre
service qui a accès la form factory, alors faites::

    use Symfony\Component\DependencyInjection\ContainerAware;

    class FriendMessageController extends ContainerAware
    {
        public function newAction(Request $request)
        {
            $form = $this->get('form.factory')->create('acme_friend_message');

            // ...
        }
    }

Si vous étendez la classe ``Symfony\Bundle\FrameworkBundle\Controller\Controller``, appelez simplement::

    $form = $this->createForm('acme_friend_message');

Vous pouvez également l'imbriquer facilement dans un autre formulaire::

    // dans une classe "form type"
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $builder->add('message', 'acme_friend_message');
    }

.. _cookbook-form-events-submitted-data:

Génération dynamique pour formulaire soumis
-------------------------------------------

Un autre cas qui peut survenir est que vous voulez personnaliser votre formulaire
selon des données soumises par l'utilisateur. Par exemple, imaginez que vous ayez
un formulaire d'inscription pour des rassemblements sportifs. Certains évènements
vous permettront de spécifier votre position préférée sur le terrain. Ce serait
par exemple une liste de choix. Cependant, les choix possibles dépendront de chaque
sport. Le football aura des attaquants, des défenseurs, un gardien, ... mais le
baseball aura un lanceur et pas de gardien. Vous devrez corriger les options pour
que la validation soit bonne.

Le rassemblement est au formulaire comme une entité. Nous pouvons donc accéder
à chaque sport comme ceci::

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

                    // ce sera votre entité, c-a-d SportMeetup
                    $data = $event->getData();

                    $positions = $data->getSport()->getAvailablePositions();

                    $form->add('position', 'entity', array('choices' => $positions));
                }
            );
        }
    }

Lorsque vous construisez ce formulaire pour l'afficher à l'utilisateur la
première fois, alors cet exemple fonctionne parfaitement.

Cependant, les choses se compliqueront lorsque vous gèrerez la soumission.
Ceci est dû au fait que l'évènement ``PRE_SET_DATA`` nous donne la donnée
avec laquelle vous commencez (un objet ``SportMeetup`` vide), et *non pas*
la données soumise.

Dans un formulaire, vous pouvez écouter les évènements suivants:

* ``PRE_SET_DATA``
* ``POST_SET_DATA``
* ``PRE_SUBMIT``
* ``SUBMIT``
* ``POST_SUBMIT``

.. versionadded:: 2.3
    Les évènements ``PRE_SUBMIT``, ``SUBMIT`` et ``POST_SUBMIT`` ont été ajoutés
    dans Symfony 2.3. Avant, ils étaient nommés ``PRE_BIND``, ``BIND`` et ``POST_BIND``.

.. versionadded:: 2.2.6
    Le comportement de l'évènement ``POST_SUBMIT`` a changé dans la version 2.2.6. Ci-dessous,
    un exemple d'utilisation.

La solution est d'éjouter un écouteur ``POST_SUBMIT`` sur le champ dont votre nouveau
champ dépend. Si vous ajoutez un écouteur ``POST_SUBMIT`` sur un champ enfant (ex ``sport``),
et ajoutez un nouvel enfant au formulaire parent, le composant Form détectera automatiquement le
nouveau champ et lui associera les données soumises par le client.

Le type de formulaire ressemble maintenant à ceci::

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
                    // ce sera votre entité, c-a-d SportMeetup
                    $data = $event->getData();

                    $formModifier($event->getForm(), $data->getSport());
                }
            );

            $builder->get('sport')->addEventListener(
                FormEvents::POST_SUBMIT,
                function(FormEvent $event) use ($formModifier) {
                    // Il est important de récupérer ici $event->getForm()->getData(),
                    // car $event->getData() vous renverra la données initiale (vide)
                    $sport = $event->getForm()->getData();

                    // puisque nous avons ajouté l'écouteur à l'enfant, il faudra passer
                    // le parent aux fonctions de callback!
                    $formModifier($event->getForm()->getParent(), $sport);
                }
            );
        }
    }

Vous pouvez constater que vous devez écouter ces deux évènements et avoir différentes
fonctions de callback juste parce que dans deux scénarios différents, les données que
vous pouvez utiliser sont disponibles dans différents évènements. A part cela, les
écouteurs réalisent exactement la même chose dans un formulaire donné.

Mais il manque encore la mise à jour du formulaire côté client après que la
sélection du sport a été faite. Cela devrait être fait grâce à un appel AJAX
dans votre application. Dans ce controller, vous pourrez soumettre votre
formulaire, mais au lieu de le traiter, simplement utiliser le formulaire
soumis pour afficher les champs mises à jour. La réponse de l'appel AJAX
pourra alors être utilisée pour mettre à jour la vue.

.. _cookbook-dynamic-form-modification-suppressing-form-validation:

Supprimer la validation de formulaire
-------------------------------------

Pour supprimer la validation, vous pouvez utiliser l'évènement ``POST_SUBMIT`` et
empêcher le :class:`Symfony\\Component\\Form\\Extension\\Validator\\EventListener\\ValidationListener`
d'être appelé.

Vous pouvez être amené à faire cela si vous définissez ``group_validation`` à
``false`` car, même dans ce cas, certaines vérifications sont tout de même
faites. Par exemple, un fichier uploadé sera quand même vérifié pour voir s'il
est trop volumineux, et un formulaire vérifiera également si des champs supplémentaires
ont été soumis. Pour désactiver tout cela, utilisez un écouteur::

    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\Form\FormEvents;

    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $builder->addEventListener(FormEvents::POST_SUBMIT, function($event) {
            $event->stopPropagation();
        }, 900); // Définissez toujours une priorité plus grande que le ValidationListener

        // ...
    }

.. caution::

    En faisant cela, vous risquez de désactiver plus que la validation du
    formulaire, puisque ``POST_SUBMIT`` peut avoir d'autres écouteurs.
