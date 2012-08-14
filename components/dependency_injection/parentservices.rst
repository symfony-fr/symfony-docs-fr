.. index::
   single: Dependency Injection; Parent Services

Gérer les dépendances communes avec des services parents
========================================================

Au fur et à mesure que vous ajoutez de la fonctionnalité à votre application, vous
pourriez commencer à avoir des classes liées partageant les mêmes dépendances. Par
exemple, vous pourriez avoir un gestionnaire de lettres d'information (« Newsletter
Manager » en anglais) qui utilise l'injection via des mutateurs pour définir ses dépendances::

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

et aussi une classe « Greeting Card » (« carte de voeux » en français) qui
partage les mêmes dépendances::

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

La configuration des services pour ces classes ressemble à quelque
chose comme ça :

.. configuration-block::

    .. code-block:: yaml

        parameters:
            # ...
            newsletter_manager.class: NewsletterManager
            greeting_card_manager.class: GreetingCardManager
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

        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">GreetingCardManager</parameter>
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

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'NewsletterManager');
        $container->setParameter('greeting_card_manager.class', 'GreetingCardManager');

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

Il y a beaucoup de répétitions dans chacune des classes et dans la configuration.
Cela signifie que si vous changiez, par exemple, le ``Mailer`` des classes de
``EmailFormatter`` injecté via le constructeur, vous devriez mettre à jour
la configuration à deux endroits. De même, si vous deviez effectuer des
changements dans les méthodes de mutation, vous devriez faire cela dans les
deux classes. La manière usuelle de gérer les méthodes communes de ces classes
liées serait de les extraire dans une classe parente::

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

Les classes ``NewsletterManager`` et ``GreetingCardManager`` peuvent alors
étendre cette classe parente::

    class NewsletterManager extends MailManager
    {
        // ...
    }

et::

    class GreetingCardManager extends MailManager
    {
        // ...
    }

De façon similaire, le conteneur de service de Symfony2 supporte aussi
l'extension de services via la configuration qui vous permet de réduire le
nombre de répétitions en spécifiant un parent pour un service.

.. configuration-block::

    .. code-block:: yaml

        parameters:
            # ...
            newsletter_manager.class: NewsletterManager
            greeting_card_manager.class: GreetingCardManager
            mail_manager.class: MailManager
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

        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">GreetingCardManager</parameter>
            <parameter key="mail_manager.class">MailManager</parameter>
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

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'NewsletterManager');
        $container->setParameter('greeting_card_manager.class', 'GreetingCardManager');
        $container->setParameter('mail_manager.class', 'MailManager');

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

Dans ce contexte, avoir un service ``parent`` implique que les arguments et
appels de méthode du service parent devrait être utilisés pour les services
enfants. Spécifiquement, les méthodes mutateurs définies pour le service parent
seront appelées lorsque les services enfants seront instanciés.

.. note::

    Si vous supprimez la clé de configuration ``parent``, les services seront
    toujours instanciés et étendront toujours la classe ``MailManager``. La
    différence est que le fait d'omettre la clé de configuration ``parent``
    signifiera que les ``appels`` définis sur le service ``mail_manager``
    ne seront pas exécutés quand les services enfants seront instanciés.

La classe parente est abstraite comme elle ne devrait pas être directement
instanciée. La définir comme abstraite dans le fichier de configuration
comme cela a été fait ci-dessus signifiera qu'elle ne peut être utilisée
uniquement en tant que service parent et ne peut pas être utilisée directement
en tant que service à injecter et sera supprimée au moment de la compilation.

Surcharger des dépendances parentes
-----------------------------------

Il se peut qu'à un moment ou à un autre vous souhaitiez surcharger quelle
classe est passée en tant que dépendance d'un seul de vos services enfants.
Heureusement, en ajoutant la configuration d'appel de méthode pour
le service enfant, les dépendances définies par la classe parente seront
surchargées. Donc si vous aviez besoin de passer une dépendance différente
uniquement à la classe ``NewsletterManager``, la configuration ressemblerait à
quelque chose comme ça :

.. configuration-block::

    .. code-block:: yaml

        parameters:
            # ...
            newsletter_manager.class: NewsletterManager
            greeting_card_manager.class: GreetingCardManager
            mail_manager.class: MailManager
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

        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">GreetingCardManager</parameter>
            <parameter key="mail_manager.class">MailManager</parameter>
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

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'NewsletterManager');
        $container->setParameter('greeting_card_manager.class', 'GreetingCardManager');
        $container->setParameter('mail_manager.class', 'MailManager');

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

La classe ``GreetingCardManager`` va recevoir les mêmes dépendances qu'avant,
mais la classe ``NewsletterManager`` quant à elle va avoir le service
``my_alternative_mailer` à la place du service ``my_mailer``.

Collections de dépendances
--------------------------

Veuillez noter que la méthode de mutation surchargée dans l'exemple
précédent est en fait appelée deux fois - une fois par la définition
du parent et une fois par la définition de l'enfant. Dans l'exemple
précédent, cela fonctionnait bien, puisque le second appel à ``setMailer``
remplaçait l'objet ``mailer`` défini par le premier appel.

Dans certains cas, cependant, cela peut être problématique. Par exemple,
si l'appel de la méthode surchargée implique l'ajout de quelque chose à une
collection, alors deux objets seront ajoutés à cette collection. Ce qui
suit montre un tel cas, avec une classe parente qui ressemble à cela::

    abstract class MailManager
    {
        protected $filters;

        public function setFilter($filter)
        {
            $this->filters[] = $filter;
        }
        // ...
    }

Si vous aviez la configuration suivante :

.. configuration-block::

    .. code-block:: yaml

        parameters:
            # ...
            newsletter_manager.class: NewsletterManager
            mail_manager.class: MailManager
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

        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">NewsletterManager</parameter>
            <parameter key="mail_manager.class">MailManager</parameter>
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

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'NewsletterManager');
        $container->setParameter('mail_manager.class', 'MailManager');

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

Dans cet exemple, la méthode ``setFilter`` du service ``newsletter_manager``
sera appelée deux fois, donnant comme résultat le tableau ``$filters``
contenant les deux objets ``my_filter`` et ``another_filter``. Cela est
parfait si vous voulez simplement ajouter des filtres additionnels aux
sous-classes. Si vous souhaitez remplacer les filtres passés à la sous-classe,
supprimer le paramètre ``parent`` de la configuration va éviter que la classe
de base appelle ``setFilter``.
