.. index::
    single: Dependency Injection

Le Composant d'Injection de Dépendance (« Dependency Injection » en anglais)
============================================================================

    Le composant d'Injection de Dépendance vous permet de standardiser et de
    centraliser la manière dont les objets sont construits dans votre application.

Pour une introduction sur l'Injection de Dépendance et des conteneurs
de service, lisez le chapitre :doc:`/book/service_container`.

Installation
------------

Vous pouvez installer le composant de différentes manières :

* Utilisez le dépôt Git officiel (https://github.com/symfony/DependencyInjection) ;
* Installez le via PEAR (`pear.symfony.com/DependencyInjection`) ;
* Installez le via Composer (`symfony/dependency-injection` dans Packagist).

Utilisation Basique
-------------------

Vous pourriez avoir une classe toute simple comme par exemple ``Mailer``, que l'on peut
voir ci-dessous, que vous voulez rendre disponible en tant que service :


.. code-block:: php

    class Mailer
    {
        private $transport;

        public function __construct()
        {
            $this->transport = 'sendmail';
        }

        // ...
    }

Vous pouvez enregistrer cette dernière dans le conteneur en tant que
service :

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;

    $container = new ContainerBuilder();
    $container->register('mailer', 'Mailer');

Une amélioration que l'on pourrait apporter à la classe afin de la rendre
plus flexible serait de permettre au conteneur de définir la propriété
``transport`` utilisée. Si vous changez la classe afin que la propriété soit
passée au constructeur, cela nous donne :

.. code-block:: php

    class Mailer
    {
        private $transport;

        public function __construct($transport)
        {
            $this->transport = $transport;
        }

        // ...
    }

Ensuite, vous pouvez définir votre choix de transport dans le conteneur :

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;

    $container = new ContainerBuilder();
    $container->register('mailer', 'Mailer')
        ->addArgument('sendmail');

Cette classe est maintenant beaucoup plus flexible car nous avons séparé
le choix du transport - qui est maintenant du ressort du conteneur - de
l'implémentation de la classe.

Le mode de transport d'email que vous avez choisi pourrait être quelque chose
que d'autres services ont besoin de connaître. Vous pouvez éviter d'avoir à le
changer à différents endroits en en faisant un paramètre dans le conteneur et
en y faisant référence par la suite lorsque vous définissez l'argument du constructeur
du service ``Mailer`` :

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;

    $container = new ContainerBuilder();
    $container->setParameter('mailer.transport', 'sendmail');
    $container->register('mailer', 'Mailer')
        ->addArgument('%mailer.transport%');

Maintenant que le service ``mailer`` est dans le conteneur, vous pouvez
l'injecter comme une dépendance dans d'autres classes. Si vous avez une
classe ``NewsletterManager`` comme ceci :

.. code-block:: php

    use Mailer;

    class NewsletterManager
    {
        private $mailer;

        public function __construct(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

Alors vous pouvez aussi l'enregistrer en tant que service et lui passer le
service ``mailer`` :

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Reference;

    $container = new ContainerBuilder();

    $container->setParameter('mailer.transport', 'sendmail');
    $container->register('mailer', 'Mailer')
        ->addArgument('%mailer.transport%');

    $container->register('newsletter_manager', 'NewsletterManager')
        ->addArgument(new Reference('mailer');

Si le ``NewsletterManager`` n'avait pas toujours besoin du ``Mailer`` et que l'injection
était optionnelle, alors vous pourriez utiliser une injection par mutateur à
la place :

.. code-block:: php

    use Mailer;

    class NewsletterManager
    {
        private $mailer;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

Vous pouvez maintenant choisir de ne pas injecter un ``Mailer`` dans le
``NewsletterManager``. Mais si vous le désirez, alors le conteneur peut
appeler la méthode du mutateur :

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Reference;

    $container = new ContainerBuilder();

    $container->setParameter('mailer.transport', 'sendmail');
    $container->register('mailer', 'Mailer')
        ->addArgument('%mailer.transport%');

    $container->register('newsletter_manager', 'NewsletterManager')
        ->addMethodCall('setMailer', new Reference('mailer');

Vous pourriez alors récupérer votre service ``newsletter_manager`` depuis
le conteneur comme cela :

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Reference;

    $container = new ContainerBuilder();

    //--

    $newsletterManager = $container->get('newsletter_manager');

Eviter que votre code devienne dépendant du Conteneur
-----------------------------------------------------

Tandis que vous pouvez récupérer directement des services depuis le conteneur,
il est plus judicieux de minimiser cela. Par exemple, dans le ``NewsletterManager``,
nous avons injecté le service ``mailer`` plutôt que de le demander depuis
le conteneur. Nous pourrions avoir injecté le conteneur et ensuite
récupéré depuis ce dernier le service ``mailer`` mais cela voudrait dire
que ce service serait lié à ce conteneur en particulier rendant ainsi
difficile la réutilisation de cette classe quelque part d'autre.

Vous allez devoir récupérer un service depuis le conteneur à un moment ou à
un autre mais cela devrait être limité autant que possible au point d'entrée
de votre application.

Initialiser le Conteneur avec des fichiers de configuration
-----------------------------------------------------------

Tout comme vous avez initialisé vos services en utilisant PHP ci-dessus,
vous pouvez aussi utiliser des fichiers de configuration. Pour
faire cela, vous devez installer en sus le composant « Config » :

* Utilisez le dépôt Git officiel (https://github.com/symfony/Config) ;
* Installez le via PEAR (`pear.symfony.com/Config`) ;
* Installez le via Composer (`symfony/config` dans Packagist).

Chargement d'un fichier de configuration XML :

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\Config\FileLocator;
    use Symfony\Component\DependencyInjection\Loader\XmlFileLoader;

    $container = new ContainerBuilder();
    $loader = new XmlFileLoader($container, new FileLocator(__DIR__));
    $loader->load('services.xml');

Chargement d'un fichier de configuration YAML :

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\Config\FileLocator;
    use Symfony\Component\DependencyInjection\Loader\YamlFileLoader;

    $container = new ContainerBuilder();
    $loader = new YamlFileLoader($container, new FileLocator(__DIR__));
    $loader->load('services.yml');

Les services ``newsletter_manager`` et ``mailer`` peuvent aussi être initialisés
en utilisant des fichiers de configuration :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            mailer.transport: sendmail

        services:
            mailer:
                class:     Mailer
                arguments: [%mailer.transport%]
            newsletter_manager:
                class:     NewsletterManager
                calls:
                    - [ setMailer, [ @mailer ] ]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="mailer.transport">sendmail</parameter>
        </parameters>

        <services>
            <service id="mailer" class="Mailer">
                <argument>%mailer.transport%</argument>
            </service>

            <service id="newsletter_manager" class="NewsletterManager">
                <call method="setMailer">
                     <argument type="service" id="mailer" />
                </call>
            </service>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('mailer.transport', 'sendmail');
        $container->register('mailer', 'Mailer')
           ->addArgument('%mailer.transport%');

        $container->register('newsletter_manager', 'NewsletterManager')
           ->addMethodCall('setMailer', new Reference('mailer');
