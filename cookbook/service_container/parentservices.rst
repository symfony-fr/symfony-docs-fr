Comment Gérer les Dépendances Communes avec des Services Parents
================================================================

En ajoutant plus de fonctionnalités à votre application, vous pouvez 
commencer à observer des classes qui partagent certaines dépendances. Par 
exemple, vous pouvez avoir un gestionnaire de newsletter qui utilise des 
« setters » pour injecter ses dépendances::

    namespace Acme\HelloBundle\Mail;

    use Acme\HelloBundle\Mailer;
    use Acme\HelloBundle\EmailFormatter;

    class NewsletterManager
    {
        protected $mailer;
        protected $emailFormatter;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setEmailFormatter(EmailFormatter $emailFormatter)
        {
            $this->emailFormatter = $emailFormatter;
        }
        // ...
    }

et de même une classe GreetingCard qui partage les mêmes dépendances::

    namespace Acme\HelloBundle\Mail;

    use Acme\HelloBundle\Mailer;
    use Acme\HelloBundle\EmailFormatter;

    class GreetingCardManager
    {
        protected $mailer;
        protected $emailFormatter;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setEmailFormatter(EmailFormatter $emailFormatter)
        {
            $this->emailFormatter = $emailFormatter;
        }
        // ...
    }

le service config pour ces classes devrait être alors :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Mail\NewsletterManager
            greeting_card_manager.class: Acme\HelloBundle\Mail\GreetingCardManager
        services:
            my_mailer:
                # ...
            my_email_formatter:
                # ...
            newsletter_manager:
                class:     %newsletter_manager.class%
                calls:
                    - [ setMailer, [ @my_mailer ] ]
                    - [ setEmailFormatter, [ @my_email_formatter] ]

            greeting_card_manager:
                class:     %greeting_card_manager.class%
                calls:
                    - [ setMailer, [ @my_mailer ] ]
                    - [ setEmailFormatter, [ @my_email_formatter] ]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Mail\NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">Acme\HelloBundle\Mail\GreetingCardManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="my_email_formatter" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_email_formatter" />
                </call>
            </service>
            <service id="greeting_card_manager" class="%greeting_card_manager.class%">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_email_formatter" />
                </call>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Mail\NewsletterManager');
        $container->setParameter('greeting_card_manager.class', 'Acme\HelloBundle\Mail\GreetingCardManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('my_email_formatter', ... );
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_email_formatter')
        ));
        $container->setDefinition('greeting_card_manager', new Definition(
            '%greeting_card_manager.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_email_formatter')
        ));

Beaucoup de répétitions sont alors présentes tant dans les deux classes, que dans
la configuration. Cela implique que si vous changez par exemple le ``Mailer`` de
la classe ``EmailFormatter``, vous devez modifier la configuration en deux endroits.
De la même façon, si vous avez besoin d'effectuer des changement sur les « setters »
(setMailer ou setEmailFormatter), vous auriez besoin de modifier les deux classes.
La solution classique est alors d'extraire ces méthode dans une classe parente des
deux classes précédentes::

    namespace Acme\HelloBundle\Mail;

    use Acme\HelloBundle\Mailer;
    use Acme\HelloBundle\EmailFormatter;

    abstract class MailManager
    {
        protected $mailer;
        protected $emailFormatter;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setEmailFormatter(EmailFormatter $emailFormatter)
        {
            $this->emailFormatter = $emailFormatter;
        }
        // ...
    }

Les classes ``NewsletterManager`` et ``GreetingCardManager`` peuvent alors étendre 
cette classe abstraite::

    namespace Acme\HelloBundle\Mail;

    class NewsletterManager extends MailManager
    {
        // ...
    }

et::

    namespace Acme\HelloBundle\Mail;

    class GreetingCardManager extends MailManager
    {
        // ...
    }

De façon similaire, le conteneur de service supporte aussi l'héritage de service
au travers de clés de configuration ; ainsi vous pouvez réduire les répétitions en
spécifiant un parent pour un service particulier.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Mail\NewsletterManager
            greeting_card_manager.class: Acme\HelloBundle\Mail\GreetingCardManager
            mail_manager.class: Acme\HelloBundle\Mail\MailManager
        services:
            my_mailer:
                # ...
            my_email_formatter:
                # ...
            mail_manager:
                class:     %mail_manager.class%
                abstract:  true
                calls:
                    - [ setMailer, [ @my_mailer ] ]
                    - [ setEmailFormatter, [ @my_email_formatter] ]
            
            newsletter_manager:
                class:     %newsletter_manager.class%
                parent: mail_manager
            
            greeting_card_manager:
                class:     %greeting_card_manager.class%
                parent: mail_manager
            
    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Mail\NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">Acme\HelloBundle\Mail\GreetingCardManager</parameter>
            <parameter key="mail_manager.class">Acme\HelloBundle\Mail\MailManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="my_email_formatter" ... >
              <!-- ... -->
            </service>
            <service id="mail_manager" class="%mail_manager.class%" abstract="true">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_email_formatter" />
                </call>
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%" parent="mail_manager"/>
            <service id="greeting_card_manager" class="%greeting_card_manager.class%" parent="mail_manager"/>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Mail\NewsletterManager');
        $container->setParameter('greeting_card_manager.class', 'Acme\HelloBundle\Mail\GreetingCardManager');
        $container->setParameter('mail_manager.class', 'Acme\HelloBundle\Mail\MailManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('my_email_formatter', ... );
        $container->setDefinition('mail_manager', new Definition(
            '%mail_manager.class%'
        ))->SetAbstract(
            true
        )->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_email_formatter')
        ));
        $container->setDefinition('newsletter_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%newsletter_manager.class%'
        );
        $container->setDefinition('greeting_card_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%greeting_card_manager.class%'
        );

Dans ce contexte, un service ``parent`` implique que les arguments et les méthodes appelées
par le service parent devront être disponibles dans le service enfant. Les « setters » définis
seront ainsi appelés au moment où le service enfant sera instancié.

.. note::

   Si vous supprimez la clé parente de la configuration, les services seront toujours
   instanciés et étendront toujours leur classe mère (ici ``MailManager``). La différence
   étant que les ``appels`` aux « setters » (setMailer et setEmailFormatter) ne seront plus
   effectués au moment de l'instanciation.

La classe parente est définie comme abstraite  afin de ne pas être instanciée directement.
La mention abstraite dans le fichier de configuration permet de créer un service qui ne sera
pas activé et qui sera seulement utilisé en tant que parent. En d'autre termes, il apparaît
plus comme un prototype que comme un service à part entière.

Surcharger les dépendances parentes
-----------------------------------

Vous pourriez avoir besoin de surcharger les arguments ou les méthodes 
provenant d'un service parent et transmises à un seul service enfant.
En ajoutant les appels de méthodes au service enfant, les dépendances 
assignées par un parent peuvent être surchargées. Ainsi, que vous avez besoin
de définir une dépendance différente à la classe ``NewsletterManager``,
la configuration deviendrait :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Mail\NewsletterManager
            greeting_card_manager.class: Acme\HelloBundle\Mail\GreetingCardManager
            mail_manager.class: Acme\HelloBundle\Mail\MailManager
        services:
            my_mailer:
                # ...
            my_alternative_mailer:
                # ...
            my_email_formatter:
                # ...
            mail_manager:
                class:     %mail_manager.class%
                abstract:  true
                calls:
                    - [ setMailer, [ @my_mailer ] ]
                    - [ setEmailFormatter, [ @my_email_formatter] ]
            
            newsletter_manager:
                class:     %newsletter_manager.class%
                parent: mail_manager
                calls:
                    - [ setMailer, [ @my_alternative_mailer ] ]
            
            greeting_card_manager:
                class:     %greeting_card_manager.class%
                parent: mail_manager
            
    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Mail\NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">Acme\HelloBundle\Mail\GreetingCardManager</parameter>
            <parameter key="mail_manager.class">Acme\HelloBundle\Mail\MailManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="my_alternative_mailer" ... >
              <!-- ... -->
            </service>
            <service id="my_email_formatter" ... >
              <!-- ... -->
            </service>
            <service id="mail_manager" class="%mail_manager.class%" abstract="true">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_email_formatter" />
                </call>
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%" parent="mail_manager">
                 <call method="setMailer">
                     <argument type="service" id="my_alternative_mailer" />
                </call>
            </service>
            <service id="greeting_card_manager" class="%greeting_card_manager.class%" parent="mail_manager"/>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Mail\NewsletterManager');
        $container->setParameter('greeting_card_manager.class', 'Acme\HelloBundle\Mail\GreetingCardManager');
        $container->setParameter('mail_manager.class', 'Acme\HelloBundle\Mail\MailManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('my_alternative_mailer', ... );
        $container->setDefinition('my_email_formatter', ... );
        $container->setDefinition('mail_manager', new Definition(
            '%mail_manager.class%'
        ))->SetAbstract(
            true
        )->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_email_formatter')
        ));
        $container->setDefinition('newsletter_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%newsletter_manager.class%'
        )->addMethodCall('setMailer', array(
            new Reference('my_alternative_mailer')
        ));
        $container->setDefinition('greeting_card_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%greeting_card_manager.class%'
        );

La classe ``GreetingCardManager`` recevra les même arguments qu'avant, mais la classe
``NewsletterManager`` recevra une instance de ``my_alternative_mailer`` à la place du
service ``my_mailer``.

Collections de Dépendances
--------------------------

Surcharger les « setters », comme dans l'exemple précédent, provoque deux appels,
l'un pour la définition parente, l'autre pour la définition de l'enfant. Dans 
ce cas particulier cela n'aura pas d'incidence, les objets mailer étant simplement
remplacés.

Dans certains cas cependant, cela peut causer un problème (utilisation de variables
statiques, appels internes avec mutations, ajouts, enregistrement dans une collection, ...),
comme dans la classe suivante::

    namespace Acme\HelloBundle\Mail;

    use Acme\HelloBundle\Mailer;
    use Acme\HelloBundle\EmailFormatter;

    abstract class MailManager
    {
        protected $filters;

        public function setFilter($filter)
        {
            $this->filters[] = $filter;
        }
        // ...
    }

Si vous avez une configuration telle que celle-ci:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Mail\NewsletterManager
            mail_manager.class: Acme\HelloBundle\Mail\MailManager
        services:
            my_filter:
                # ...
            another_filter:
                # ...
            mail_manager:
                class:     %mail_manager.class%
                abstract:  true
                calls:
                    - [ setFilter, [ @my_filter ] ]
                    
            newsletter_manager:
                class:     %newsletter_manager.class%
                parent: mail_manager
                calls:
                    - [ setFilter, [ @another_filter ] ]
            
    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Mail\NewsletterManager</parameter>
            <parameter key="mail_manager.class">Acme\HelloBundle\Mail\MailManager</parameter>
        </parameters>

        <services>
            <service id="my_filter" ... >
              <!-- ... -->
            </service>
            <service id="another_filter" ... >
              <!-- ... -->
            </service>
            <service id="mail_manager" class="%mail_manager.class%" abstract="true">
                <call method="setFilter">
                     <argument type="service" id="my_filter" />
                </call>
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%" parent="mail_manager">
                 <call method="setFilter">
                     <argument type="service" id="another_filter" />
                </call>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Mail\NewsletterManager');
        $container->setParameter('mail_manager.class', 'Acme\HelloBundle\Mail\MailManager');

        $container->setDefinition('my_filter', ... );
        $container->setDefinition('another_filter', ... );
        $container->setDefinition('mail_manager', new Definition(
            '%mail_manager.class%'
        ))->SetAbstract(
            true
        )->addMethodCall('setFilter', array(
            new Reference('my_filter')
        ));
        $container->setDefinition('newsletter_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%newsletter_manager.class%'
        )->addMethodCall('setFilter', array(
            new Reference('another_filter')
        ));

Dans cet exemple, le ``setFilter`` provenant du service ``newsletter_manager``
sera appelé deux fois, impliquant que la collection ``$filters`` contiendra à la
fois les objets ``my_filter`` et ``another_filter``. Si vous voulez simplement
ajouter des filtres à une classe, cela n'aura pas de conséquences. Si l'ordre des
filtres et leurs existences importe, il faudra alors redéfinir le service en ajoutant
tous les appels à effectuer et en lui enlevant dans la configuration le paramètre
``parent``. Il peux cependant conserver l'héritage de classe simplifiant certains
appels.