.. index::
   single: Formulaire; Évènements

Comment générer dynamiquement des Formulaires en Utilisant les Évènements de Formulaire
=======================================================================================

Avant de se lancer directement dans la génération dynamique de formulaire,
commençons par passer en revue ce à quoi ressemble une classe de formulaire
basique::

    // src/Acme/DemoBundle/Form/Type/ProductType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    
    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('name');
            $builder->add('price');
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

Assumons un instant que ce formulaire utilise une classe imaginaire
« Product » qui possède uniquement deux propriétés (« name » et « price »).
Le formulaire généré à partir de cette classe sera identique que ce soit
pour la création d'un Produit ou pour l'édition d'un Produit existant (par
exemple : un Produit récupéré depuis la base de données).

Supposons maintenant que vous ne souhaitiez pas que l'utilisateur puisse
changer la valeur de ``name`` une fois que l'objet a été créé. Pour faire
cela, vous pouvez utiliser le
:ref:`Répartiteur d'Évènements (« Event Dispatcher » en anglais) <book-internals-event-dispatcher>`
de Symfony pour analyser les données de l'objet et modifier le formulaire
en se basant sur les données de l'objet Product. Dans cet article, vous
allez apprendre comment ajouter ce niveau de flexibilité à vos formulaires.

.. _`cookbook-forms-event-subscriber`:

Ajouter Un Souscripteur d'Évènement à une Classe Formulaire
-----------------------------------------------------------

Donc, au lieu d'ajouter directement ce widget « name » via notre classe
formulaire ProductType, déléguons la responsabilité de créer ce champ
particulier à un Souscripteur d'Évènement::

    // src/Acme/DemoBundle/Form/Type/ProductType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Acme\DemoBundle\Form\EventListener\AddNameFieldSubscriber;

    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $subscriber = new AddNameFieldSubscriber($builder->getFormFactory());
            $builder->addEventSubscriber($subscriber);
            $builder->add('price');
        }

        public function getName()
        {
            return 'product';
        }
    }

Le souscripteur d'évènement obtient l'objet FormFactory via son constructeur
afin que notre nouveau souscripteur soit capable de créer le widget du
formulaire une fois qu'il est notifié de l'évènement durant la création du
formulaire.

.. _`cookbook-forms-inside-subscriber-class`:

A l'intérieur de la Classe du Souscripteur d'Évènement
------------------------------------------------------

Le but est de créer un champ « name » *uniquement* si l'objet Product sous-jacent
est nouveau (par exemple : n'a pas été persisté dans la base de données). Basé sur
cela, le souscripteur pourrait ressembler à quelque chose comme ça::

    // src/Acme/DemoBundle/Form/EventListener/AddNameFieldSubscriber.php
    namespace Acme\DemoBundle\Form\EventListener;

    use Symfony\Component\Form\Event\DataEvent;
    use Symfony\Component\Form\FormFactoryInterface;
    use Symfony\Component\EventDispatcher\EventSubscriberInterface;
    use Symfony\Component\Form\FormEvents;

    class AddNameFieldSubscriber implements EventSubscriberInterface
    {
        private $factory;
        
        public function __construct(FormFactoryInterface $factory)
        {
            $this->factory = $factory;
        }
        
        public static function getSubscribedEvents()
        {

            // Informe le répartiteur que nous voulons écouter l'évènement
            // form.pre_set_data et que la méthode preSetData devrait être appelée
            return array(FormEvents::PRE_SET_DATA => 'preSetData');
        }

        public function preSetData(DataEvent $event)
        {
            $data = $event->getData();
            $form = $event->getForm();

            // Durant la création du formulaire, setData() est appelée avec null
            // en argument par le constructeur de FormBuilder. Nous sommes concerné
            // uniquement lorsque setData est appelée et contient un objet Entity
            // (soit nouveau, soit récupéré avec Doctrine). Ce « if » nous permet
            // de passer outre ce cas là (i.e. condition null).
            if (null === $data) {
                return;
            }

            // vérifie si l'objet produit est « nouveau »
            if (!$data->getId()) {
                $form->add($this->factory->createNamed('name', 'text'));
            }
        }
    }

.. caution::

    Il est très facile de mal interpréter l'objectif de la portion de code
    ``if (null === $data)`` de ce souscripteur d'évènement. Afin de bien comprendre
    son rôle, vous pouvez jeter un oeil à la `classe Formulaire`_ en portant votre
    attention où la méthode setData() est appelée à la fin du constructeur, ainsi
    qu'à la méthode setData() elle-même.

La ligne ``FormEvents::PRE_SET_DATA`` est convertie en la chaîne de caractères suivante :
``form.pre_set_data``. La `classe FormEvents`_ a un but organisationnel. C'est un endroit
centralisé où vous trouverez la liste des différents évènements de formulaire disponibles.

Bien que cet exemple aurait pu utiliser de manière tout aussi efficace les évènements ``form.set_data``
ou ``form.post_set_data``, en utilisant ``form.pre_set_data``, nous
garantissons que les données allant être récupérées depuis l'objet ``Event`` n'auront pas été
modifiées par quelconques souscripteurs ou listeners (« écouteurs » en français). Cela parce
que ``form.pre_set_data`` passe un objet `DataEvent`_ au lieu de l'objet `FilterDataEvent`_
passé par l'évènement ``form.set_data``. `DataEvent`_, contrairement à son enfant
`FilterDataEvent`_, ne possède pas de méthode setData().

.. note::

    Vous pouvez voir la liste complète des évènements de formulaire via la
    `classe FormEvents`_, que vous trouverez dans le bundle formulaire.

.. _`DataEvent`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Event/DataEvent.php
.. _`classe FormEvents`: https://github.com/symfony/Form/blob/master/FormEvents.php
.. _`classe Formulaire`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Form.php
.. _`FilterDataEvent`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Event/FilterDataEvent.php
