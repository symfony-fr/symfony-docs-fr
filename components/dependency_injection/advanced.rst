.. index::
   single: Dependency Injection; Advanced configuration

Configuration Avancée du Conteneur
==================================

Marquer les services comme publics / privés
-------------------------------------------

Lorsque vous définissez des services, vous allez généralement vouloir avoir
accès à ces définitions depuis le code de votre application. Ces services
sont dits ``public``. Par exemple, le service ``doctrine`` enregistré dans
le conteneur lorsque vous utilisez le DoctrineBundle est un service public
auquel vous pouvez accéder via::

   $doctrine = $container->get('doctrine');

Cependant, il y a des cas où vous ne souhaitez pas qu'un service soit public.
Cela arrive souvent quand un service est défini seulement pour être utilisé
comme argument d'un autre service.

.. note::

    Si vous utilisez un service privé comme argument d'un seul autre service,
    cela résultera en une instanciation « instantanée » (par exemple :
    ``new PrivateFooBar()``) à l'intérieur de cet autre service, le rendant publiquement
    indisponible à l'exécution.

Pour faire simple : un service est privé quand vous ne voulez pas y accéder
directement depuis votre code.

Voici un exemple :

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Example\Foo
             public: false

    .. code-block:: xml

        <service id="foo" class="Example\Foo" public="false" />

    .. code-block:: php

        $definition = new Definition('Example\Foo');
        $definition->setPublic(false);
        $container->setDefinition('foo', $definition);

Maintenant que le service est privé, vous *ne pouvez pas* l'appeler::

    $container->get('foo');

Cependant, si un service a été marqué comme privé, vous pouvez toujours
créer un alias de ce dernier (voir ci-dessous) pour y accéder (via l'alias).

.. note::

   Les services sont par défaut publics.

Créer un alias
--------------

Parfois, vous pourriez vouloir utiliser des raccourcis pour accéder à certains
de vos services. Vous pouvez faire cela en créant des alias pour ces derniers ;
de plus, vous pouvez même créer des alias pour les services non publics.

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Example\Foo
           bar:
             alias: foo

    .. code-block:: xml

        <service id="foo" class="Example\Foo"/>

        <service id="bar" alias="foo" />

    .. code-block:: php

        $definition = new Definition('Example\Foo');
        $container->setDefinition('foo', $definition);

        $containerBuilder->setAlias('bar', 'foo');

Cela signifie que lorsque vous utilisez le conteneur directement, vous
pouvez accéder au service ``foo`` en demandant le service ``bar`` comme
cela::

    $container->get('bar'); // Retourne le service foo

Requérir des fichiers
---------------------

Il pourrait y avoir des cas où vous aurez besoin d'inclure un autre fichier
juste avant que le service lui-même soit chargé. Pour faire cela, vous
pouvez utiliser la directive ``file``.

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Example\Foo\Bar
             file: "%kernel.root_dir%/src/path/to/file/foo.php"

    .. code-block:: xml

        <service id="foo" class="Example\Foo\Bar">
            <file>%kernel.root_dir%/src/path/to/file/foo.php</file>
        </service>

    .. code-block:: php

        $definition = new Definition('Example\Foo\Bar');
        $definition->setFile('%kernel.root_dir%/src/path/to/file/foo.php');
        $container->setDefinition('foo', $definition);

Notez que Symfony va appeler en interne la fonction PHP require_once, ce
qui veut dire que votre fichier va être inclus seulement une fois par requête.
