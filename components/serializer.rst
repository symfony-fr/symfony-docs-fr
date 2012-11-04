.. index::
   single: Serializer 
   single: Components; Serializer

Le Composant Serializer
=======================

   Le composant Serializer est destiné à être utilisé pour transformer
   des objets en un format spécifique (XML, JSON, Yaml, ...) et inversement.

Pour faire cela, le composant Serializer suit le schéma simple suivant.

.. image:: /images/components/serializer/serializer_workflow.png

Comme vous pouvez le voir sur l'image ci-dessus, un tableau est utilisé
comme intermédiaire. De cette manière, les encodeurs (Encoder) ne se chargeront
que de transformer des **formats** spécifiques en **tableaux** et vice versa.
De la même manière, les normaliseurs (Normalizer) ne transformeront que
des **objets** en **tableaux** et vice versa.

La sérialisation est un sujet complexe, et même si ce composant ne fonctionne
pas pour tout les cas de figure, il peut être un outil très utile pour sérialiser
ou désérialiser vos objets.

Installation
------------

Vous pouvez installer le composant de différentes manières :

* Utilisez le dépôt Git officiel (https://github.com/symfony/Serializer);
* Installez le via Composer (``symfony/serializer`` on `Packagist`_).

Utilisation
-----------

Utiliser le composant Serializer est très simple. Vous avez juste besoin
de définir la classe :class:`Symfony\\Component\\Serializer\\Serializer`
pour spécifier quels encodeurs et quels normaliseurs seront disponibles::

    use Symfony\Component\Serializer\Serializer;
    use Symfony\Component\Serializer\Encoder\XmlEncoder;
    use Symfony\Component\Serializer\Encoder\JsonEncoder;
    use Symfony\Component\Serializer\Normalizer\GetSetMethodNormalizer;

    $encoders = array(new XmlEncoder(), new JsonEncoder());
    $normalizers = array(new GetSetMethodNormalizer());

    $serializer = new Serializer($normalizers, $encoders);

Sérialiser un objet
~~~~~~~~~~~~~~~~~~~

Pour les besoins de cet exemple, supposons que la classe suivante
existe déjà dans notre projet::

    namespace Acme;

    class Person
    {
        private $age;
        private $name;

        // Getters
        public function getName()
        {
            return $this->name;
        }

        public function getAge()
        {
            return $this->age;
        }

        // Setters
        public function setName($name)
        {
            $this->name = $name;
        }

        public function setAge($age)
        {
            $this->age = $age;
        }
    }

Maintenant, si vous voulez sérialiser un objet en JSON, vous devez juste
utiliser le service Serializer précédemment créé::

    $person = new Acme\Person();
    $person->setName('foo');
    $person->setAge(99);

    $serializer->serialize($person, 'json'); // Output: {"name":"foo","age":99}

Le premier paramètre de la méthode :method:`Symfony\\Component\\Serializer\\Serializer::serialize`
est l'objet à sérialiser et le second est utilisé pour choisir l'encodeur, dans notre cas
:class:`Symfony\\Component\\Serializer\\Encoder\\JsonEncoder`.

Désérialiser un objet
~~~~~~~~~~~~~~~~~~~~~

Voyons maintenant comment faire l'opération inverse. Cette fois, l'information
de la classe `People` est encodée en format XML::

    $data = <<<EOF
    <person>
        <name>foo</name>
        <age>99</age>
    </person>
    EOF;

    $person = $serializer->deserialize($data,'Acme\Person','xml');

Dans ce cas, la méthode :method:`Symfony\\Component\\Serializer\\Serializer::deserialize`
nécessite trois paramètres :

1. L'information à décoder
2. Le nom de la classe dans laquelle sera décodée l'information
3. L'encodeur utilisé pour convertir l'information en tableau

JMSSerializationBundle
----------------------

Il existe un bundle tierce populaire, `JMSSerializationBundle`_, qui étend
(et parfois remplace) la fonctionnalité de sérialisation. Cela inclut la
capacité de configurer la manière dont vos objets doivent être sérialisée et
désérialisés via des annotations (ou YML, XML et PHP), l'intégration avec
l'ORM Doctrine, et la prise en charge de cas de figure plus complexes (par
exemple les références circulaires).

.. _`JMSSerializationBundle`: https://github.com/schmittjoh/JMSSerializerBundle
.. _Packagist: https://packagist.org/packages/symfony/serializer