.. index::
   single: Form; The "inherit_data" option

Comment réduire la duplication de code avec "inherit_data"
==========================================================

.. versionadded:: 2.3
    L'option ``inherit_data`` s'appelait ``virtual`` avant Symfony 2.3.

L'option de champ de formulaire ``inherit_data`` peut être très utile si
vous avez des champs dupliqués dans différentes entités. Par exemple, imaginez
que vous avez deux entités, une ``Company`` et un ``Customer``::

    // src/Acme/HelloBundle/Entity/Company.php
    namespace Acme\HelloBundle\Entity;

    class Company
    {
        private $name;
        private $website;

        private $address;
        private $zipcode;
        private $city;
        private $country;
    }

.. code-block:: php

    // src/Acme/HelloBundle/Entity/Customer.php
    namespace Acme\HelloBundle\Entity;

    class Customer
    {
        private $firstName;
        private $lastName;

        private $address;
        private $zipcode;
        private $city;
        private $country;
    }

Comme vous pouvez le voir, chaque entité partage quelques champs communs :
``address``, ``zipcode``, ``city``, ``country``.

Commencez par créer deux formulaires pour ces entités, ``CompanyType`` 
et ``CustomerType``::

    // src/Acme/HelloBundle/Form/Type/CompanyType.php
    namespace Acme\HelloBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;

    class CompanyType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('name', 'text')
                ->add('website', 'text');
        }
    }

.. code-block:: php

    // src/Acme/HelloBundle/Form/Type/CustomerType.php
    namespace Acme\HelloBundle\Form\Type;

    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\Form\AbstractType;

    class CustomerType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('firstName', 'text')
                ->add('lastName', 'text');
        }
    }

Au lieu d'inclure les champs dupliqués ``address``, ``zipcode``, ``city``
et ``country`` dans les deux formulaires, créez un troisième formulaire
appelé ``LocationType`` pour cela::

    // src/Acme/HelloBundle/Form/Type/LocationType.php
    namespace Acme\HelloBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class LocationType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('address', 'textarea')
                ->add('zipcode', 'text')
                ->add('city', 'text')
                ->add('country', 'text');
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'inherit_data' => true
            ));
        }

        public function getName()
        {
            return 'location';
        }
    }

Le formulaire ``LocationType`` a une option intéressante qui est définie,
``inherit_data``. Cette option permet au formulaire d'hériter des données
de son formulaire parent. S'il est imbriqué dans le formulaire ``CompanyType``,
les champs de ``LocationType`` accèderont aux propriétés de l'instance de
``Company``. S'il est imbriqué dans le formulaire ``CustomerType``, il
accèdera aux propriétés de l'instance de ``Customer``. Facile non ?

.. note::

    Au lieu de définir l'option ``inherit_data`` dans ``LocationType``, vous
    pouvez aussi (comme toute option) la passer comme troisième argument de
    ``$builder->add()``.

Finalement, ajoutez le formulaire ``LocationType`` à vos deux formulaires originaux::

    // src/Acme/HelloBundle/Form/Type/CompanyType.php
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        // ...

        $builder->add('foo', new LocationType(), array(
            'data_class' => 'Acme\HelloBundle\Entity\Company'
        ));
    }

.. code-block:: php

    // src/Acme/HelloBundle/Form/Type/CustomerType.php
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        // ...

        $builder->add('bar', new LocationType(), array(
            'data_class' => 'Acme\HelloBundle\Entity\Customer'
        ));
    }

C'est tout! Vous avez extrait les définitions de champs dupliqués dans un
formulaire séparé que vous pouvez réutiliser où vous le voulez.
