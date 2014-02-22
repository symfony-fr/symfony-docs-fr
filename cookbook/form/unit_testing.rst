.. index::
   single: Form; Form testing

Comment tester unitairement vos formulaires
===========================================

Le composant Formulaire consiste en 3 objets: un type de formulaire
(qui implémente :class:`Symfony\\Component\\Form\\FormTypeInterface`), le
:class:`Symfony\\Component\\Form\\Form` et la
:class:`Symfony\\Component\\Form\\FormView`.

La seule classe habituellement manipulée par les programmeurs est la classe
de type de formulaire qui sert de plan pour le formulaire. Elle est utilisée
pour générer le ``Formulaire`` et la vue (``FormView``). Vous pouvez la tester
directement en bouchonnant les interactions avec la Factory mais cela risque d'être
complexe. Il est préférable de la passer à la FormFactory comme c'est le cas dans
une véritable application. C'est simple à initialiser et vous pouvez faire confiance
aux composants de Symfony pour les utiliser dans vos tests.

Il existe déjà une classe dont vous pouvez tirer parti pour tester de simples
types de champs : :class:`Symfony\\Component\\Form\\Test\\TypeTestCase`.
Elle est utilisée pour tester les types de champ de Symfony et vous pouvez
l'utiliser pour tester vos propres types.

.. versionadded:: 2.3
    La classe ``TypeTestCase`` a migré vers ``Symfony\Component\Form\Test``
    dans Symfony 2.3. Auparavant, la classe était située dans
    ``Symfony\Component\Form\Tests\Extension\Core\Type``.

.. note::

    Selon la manière dont vous avez installé Symfony ou le composant Form,
    les tests ne seront peut être pas téléchargés. Utilisez l'option
    ``--prefer-source`` de Composer si c'est le cas.

Les bases
---------

L'implémentation la plus simple de ``TypeTestCase`` ressemble à::

    // src/Acme/TestBundle/Tests/Form/Type/TestedTypeTests.php
    namespace Acme\TestBundle\Tests\Form\Type;

    use Acme\TestBundle\Form\Type\TestedType;
    use Acme\TestBundle\Model\TestObject;
    use Symfony\Component\Form\Test\TypeTestCase;

    class TestedTypeTest extends TypeTestCase
    {
        public function testSubmitValidData()
        {
            $formData = array(
                'test' => 'test',
                'test2' => 'test2',
            );

            $type = new TestedType();
            $form = $this->factory->create($type);

            $object = new TestObject();
            $object->fromArray($formData);

            // submit the data to the form directly
            $form->submit($formData);

            $this->assertTrue($form->isSynchronized());
            $this->assertEquals($object, $form->getData());

            $view = $form->createView();
            $children = $view->children;

            foreach (array_keys($formData) as $key) {
                $this->assertArrayHasKey($key, $children);
            }
        }
    }

Alors, qu'est ce que ça teste? Voici une petite explication.

Premièrement, vous vérifiez que ``FormType`` compile. Cela inclut
l'héritage de classe de base, la fonction ``buildForm`` et la résolution
d'options. Ce doit être le premier test que vous écrivez.::

    $type = new TestedType();
    $form = $this->factory->create($type);

Ce test vérifie qu'aucune transformation de données utilisée par le formulaire
n'échoue. La méthode :method:`Symfony\\Component\\Form\\FormInterface::isSynchronized`
est simplement définie à ``false`` si une transformation de données lance une exception::

    $form->submit($formData);
    $this->assertTrue($form->isSynchronized());

.. note::

    Ne testez pas la validation : elle est appliquée par un écouteur
    qui n'est pas actif dans le cas de test et est liée à la configuration
    de validation. Au lieu de ça, testez unitairement vos contraintes
    directement.

Ensuite, vérifiez la soumission et le mapping du formulaire. Le test
ci-dessous vérifie que tous les champs sont correctement spécifiés::

    $this->assertEquals($object, $form->getData());

Enfin, vérifiez la création du ``FormView``. Vous devriez véirifier que
tous les widgets que vous voulez afficher sont disponible dans la propriété
``children``::

    $view = $form->createView();
    $children = $view->children;

    foreach (array_keys($formData) as $key) {
        $this->assertArrayHasKey($key, $children);
    }

Ajouter un Type dont votre formulaire dépend
--------------------------------------------

Votre formulaire peut dépendre d'autres types qui sont définis comme
services. Cela ressemblerait à ceci::

    // src/Acme/TestBundle/Form/Type/TestedType.php

    // ... the buildForm method
    $builder->add('acme_test_child_type');

Pour créer correctement votre formulaire, vous devez rendre le type
disponible à la Facotry dans votre test. La manière la plus simple est
de l'enregistrer manuellement avant de créer le formulaire parent en
utilisant la classe ``PreloadedExtension``::

    // src/Acme/TestBundle/Tests/Form/Type/TestedTypeTests.php
    namespace Acme\TestBundle\Tests\Form\Type;

    use Acme\TestBundle\Form\Type\TestedType;
    use Acme\TestBundle\Model\TestObject;
    use Symfony\Component\Form\Test\TypeTestCase;
    use Symfony\Component\Form\PreloadedExtension;

    class TestedTypeTest extends TypeTestCase
    {
        protected function getExtensions()
        {
            $childType = new TestChildType();
            return array(new PreloadedExtension(array(
                $childType->getName() => $childType,
            ), array()));
        }

        public function testSubmitValidData()
        {
            $type = new TestedType();
            $form = $this->factory->create($type);

            // ... your test
        }
    }

.. caution::

    Assurez vous que le type enfant que vous ajoutez est également testé.
    Autrement, vous pourriez avoir des erreurs qui ne sont pas liées au
    formulaire que vous testez mais à ses enfants.

Ajouter des Extensions spécifiques
----------------------------------

Il arrive souvent que vous utilisiez des options qui sont ajoutées par des
:doc:`extensions de formulaire </cookbook/form/create_form_type_extension>`.
Cela peut être, par exemple, ``ValidatorExtension`` avec son option ``invalid_message``.
Le ``TypeTestCase`` ne charge que les extensions de bases donc une exception
"Invalid option" sera levée si vous tentez de l'utilisez dans une classe de test
qui dépend d'autres extensions. Vous devez ajouter ces extensions à l'objet Factory::

    // src/Acme/TestBundle/Tests/Form/Type/TestedTypeTests.php
    namespace Acme\TestBundle\Tests\Form\Type;

    use Acme\TestBundle\Form\Type\TestedType;
    use Acme\TestBundle\Model\TestObject;
    use Symfony\Component\Form\Test\TypeTestCase;
    use Symfony\Component\Form\Forms;
    use Symfony\Component\Form\FormBuilder;
    use Symfony\Component\Form\Extension\Validator\Type\FormTypeValidatorExtension;

    class TestedTypeTest extends TypeTestCase
    {
        protected function setUp()
        {
            parent::setUp();

            $this->factory = Forms::createFormFactoryBuilder()
                ->addExtensions($this->getExtensions())
                ->addTypeExtension(
                    new FormTypeValidatorExtension(
                        $this->getMock('Symfony\Component\Validator\ValidatorInterface')
                    )
                )
                ->addTypeGuesser(
                    $this->getMockBuilder(
                        'Symfony\Component\Form\Extension\Validator\ValidatorTypeGuesser'
                    )
                        ->disableOriginalConstructor()
                        ->getMock()
                )
                ->getFormFactory();

            $this->dispatcher = $this->getMock('Symfony\Component\EventDispatcher\EventDispatcherInterface');
            $this->builder = new FormBuilder(null, null, $this->dispatcher, $this->factory);
        }

        // ... your tests
    }

Tester différents jeux de données
---------------------------------

Si vous n'êtes pas familier avec le `fournisseur de données`_ de PHPUnit, cela
peut être une bonne opportunité de l'utiliser::

    // src/Acme/TestBundle/Tests/Form/Type/TestedTypeTests.php
    namespace Acme\TestBundle\Tests\Form\Type;

    use Acme\TestBundle\Form\Type\TestedType;
    use Acme\TestBundle\Model\TestObject;
    use Symfony\Component\Form\Test\TypeTestCase;

    class TestedTypeTest extends TypeTestCase
    {

        /**
         * @dataProvider getValidTestData
         */
        public function testForm($data)
        {
            // ... your test
        }

        public function getValidTestData()
        {
            return array(
                array(
                    'data' => array(
                        'test' => 'test',
                        'test2' => 'test2',
                    ),
                ),
                array(
                    'data' => array(),
                ),
                array(
                    'data' => array(
                        'test' => null,
                        'test2' => null,
                    ),
                ),
            );
        }
    }

Le code ci-dessus lancera votre test trois fois avec trois jeux de données
différents. Cela permet de découpler les données de test du test lui-même
et de tester facilement plusieurs jeux de données

Vous pouvez également passer un autre argument, comme un booléen si le
formulaire doit être synchronisé avec le jeu de données ou non etc.

.. _`fournisseur de données`: http://www.phpunit.de/manual/current/en/writing-tests-for-phpunit.html#writing-tests-for-phpunit.data-providers