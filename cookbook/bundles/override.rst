.. index::
   single: Bundle; Héritage

Comment surcharger n'importe quelle partie d'un bundle
======================================================

Ce document est une référence succinte sur comment surcharger différentes
partie d'un bundle tiers.

Templates
---------

Pour des informations sur la surcharge de templates, lisez
* :ref:`overriding-bundle-templates`.
* :doc:`/cookbook/bundles/inheritance`

Routage
-------

Le routage n'est jamais importé automatiquement dans Symfony2. Si vous voulez
inclure les routes d'un bundle, alors elles doivent être importées manuellement
à un endroit de votre application (ex ``app/config/routing.yml``).

La manière la plus simple de « surcharger » les routes d'un bundle est de
ne pas les importer du tout. Plutôt que d'importer les routes d'un bundle tiers,
copiez simplement le fichier de routage dans votre application, modifiez le, et
importez le.

Contrôleurs
-----------

En partant du principe que les bundles tiers n'utisent pas de contrôleurs qui
ne soient pas des services (ce qui est presque toujours le cas), vous pouvez
facilement surcharger les contrôleurs grâce à l'héritage de bundle. Pour plus
d'informations, lisez :doc:`/cookbook/bundles/inheritance`.

Services & Configuration
------------------------

Pour surcharger/étendre un service, vous avez deux options. Premièrement,
vous pouvez redéfinir la valeur du paramètre qui contient le nom de la classe
du service en spécifiant votre propre classe dans ``app/config/config.yml``.
Bien sûr, cela n'est possible que si le nom de la classe est défini comme paramètre
dans la configuration du service du bundle. Par exemple, pour surcharger la classe
utilisé par le service ``translator``de Symfony, vous pouvez surcharger le paramètre
``translator.class``. Savoir quel paramètre surcharger peut nécessiter un peu de
recherche. Pour le Translator, le paramètre est défini et utilisé dans le fichier
``Resources/config/translation.xml`` du FrameworkBundle :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            translator.class:      Acme\HelloBundle\Translation\Translator

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="translator.class">Acme\HelloBundle\Translation\Translator</parameter>
        </parameters>

    .. code-block:: php

        // app/config/config.php

        $container->setParameter('translator.class', 'Acme\HelloBundle\Translation\Translator');

Deuxièmement, si la classe n'est pas spécifiée dans les paramètres, si vous voulez
vous assurer que la classe est bien toujours surchargée lorsque votre bundle est
utilisé, ou si vous avez besoin de faire un peu plus de modifications, vous devrez
utiliser une passe de compilation::

    namespace Foo\BarBundle\DependencyInjection\Compiler;

    use Symfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    class OverrideServiceCompilerPass implements CompilerPassInterface
    {
        public function process(ContainerBuilder $container)
        {
            $definition = $container->getDefinition('original-service-id');
            $definition->setClass('Foo\BarBundle\YourService');
        }
    }

Dans cet exemple, nous retrouvons la définition du service original, et nous changeons
son nom de classe en notre propre nom de classe.

Lisez :doc:`/cookbook/service_container/compiler_passes` pour savoir comment utiliser
les passes de compilation. Si vous voulez faire plus que simplement surcharger la classe,
comme par exemple ajouter une méthode, vous ne pouvez utiliser que la méthode de la passe
de compilation.

Entités et mapping
------------------

En cours...

Formulaires
-----------

Pour surcharger un type de formulaire, il faut l'enregistrer comme service
(c'est-à-dire que vous devez le tagger avec « form.type »). Vous pourrez alors
le surchargez comme vous surchargeriez n'importe quel service, comme c'est expliqué
dans `Services & Configuration`_. Bien sûr, cela ne fonctionnera que si le type est
appelé par son alias, et non pas s'il est instancié. Exemple::

    $builder->add('name', 'custom_type');

au lieu de::

    $builder->add('name', new CustomType());


Métadonnées de Validation
-------------------------

En cours...

Traductions
-----------

En cours...