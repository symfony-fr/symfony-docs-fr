.. index::
   single: Dependency Injection; Injection types

Types d'Injection
=================

Rendre les dépendances d'une classe explicites et exiger qu'elles soient
injectées dans cette dernière est une bonne manière de rendre une classe
plus réutilisable, testable, et découplée des autres.

Il y a plusieurs manières d'injecter des dépendances. Chaque type d'injection
a ses propres avantages et inconvénients à prendre en considération, ainsi
que différentes façons de fonctionner lorsque vous les utiliser avec le
conteneur de service.

Injection via le Constructeur
-----------------------------

La manière la plus commune d'injecter des dépendances est via le constructeur
de la classe. Pour effectuer cela, vous avez besoin d'ajouter un argument
à la signature du constructeur afin d'accepter la dépendance::

    class NewsletterManager
    {
        protected $mailer;

        public function __construct(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

Vous pouvez spécifier quel service vous souhaiteriez injecter dans cette
classe via la configuration du conteneur de service :

.. configuration-block::

    .. code-block:: yaml

       services:
            my_mailer:
                # ...
            newsletter_manager:
                class:     NewsletterManager
                arguments: [@my_mailer]

    .. code-block:: xml

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="NewsletterManager">
                <argument type="service" id="my_mailer"/>
            </service>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setDefinition('my_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            'NewsletterManager',
            array(new Reference('my_mailer'))
        ));

.. tip::

    Le fait de requérir l'injection d'un certain type d'objet signifie que vous
    pouvez être sûr qu'une dépendance appropriée a été injectée. Grâce
    à cela, vous allez recevoir immédiatement une erreur claire si une
    dépendance inappropriée est injectée. En forçant le type grâce à une
    interface plutôt que via une classe, vous pouvez rendre le choix de
    la dépendance plus flexible. Et en supposant que vous utilisez uniquement
    des méthodes définies dans l'interface, vous pouvez tirer parti de
    cette flexibilité tout en continuant d'utiliser l'objet de manière
    sécurisée.

Utiliser l'injection via le constructeur propose plusieurs avantages :

* Si la dépendance est une condition requise et que la classe ne peut pas
  fonctionner sans elle, alors l'injecter via le constructeur permet de
  s'assurer que la dépendance sera présente lorsque la classe sera utilisée
  puisque la classe ne peut pas être construite sans elle.

* Le constructeur est appelé seulement une fois lorsque l'objet est créé,
  donc vous pouvez être sûr que la dépendance ne changera pas pendant la
  durée de vie de l'objet.

Ces avantages signifient que l'injection via constructeur n'est pas envisageable
pour travailler avec des dépendances optionnelles. Ce type d'injection
est aussi plus difficile à utiliser avec les hiérarchies de classe : si
une classe utilise l'injection via le constructeur, alors l'étendre et
surcharger le constructeur devient problématique.

Injection via Mutateur
----------------------

Un autre type possible d'injection dans une classe se fait par l'ajout
d'une méthode mutateur qui accepte la dépendance::

    class NewsletterManager
    {
        protected $mailer;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

.. configuration-block::

    .. code-block:: yaml

       services:
            my_mailer:
                # ...
            newsletter_manager:
                class:     NewsletterManager
                calls:
                    - [ setMailer, [ @my_mailer ] ]

    .. code-block:: xml

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="NewsletterManager">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
            </service>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setDefinition('my_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            'NewsletterManager'
        ))->addMethodCall('setMailer', array(new Reference('my_mailer')));

Cette fois les avantages sont :

* L'injection par mutateur fonctionne bien avec les dépendances optionnelles.
  Si vous n'avez pas besoin de la dépendance, alors n'appelez pas le mutateur,
  tout simplement ;

* Vous pouvez appeler le mutateur plusieurs fois. Cela est particulièrement
  utile si la méthode ajoute la dépendance dans une collection. Vous pouvez
  ainsi avoir un nombre variable de dépendances.

Les inconvénients d'une injection par mutateur sont :

* Le mutateur peut encore être appelé après la construction
  donc vous ne pouvez pas être sûr que la dépendance n'ait pas été remplacée
  pendant la durée de vie de l'objet (excepté si vous ajoutez une vérification
  explicite dans la méthode mutateur qui contrôle s'il n'a pas déjà été
  appelé) ;

* Vous ne pouvez pas être sûr que le mutateur sera appelé et vous devez
  ajouter des contrôles qui vérifient que toute dépendance requise
  est injectée.

Injection via une Propriété
---------------------------

Une autre possibilité est de simplement définir des champs publics dans
la classe::

    class NewsletterManager
    {
        public $mailer;

        // ...
    }

.. configuration-block::

    .. code-block:: yaml

       services:
            my_mailer:
                # ...
            newsletter_manager:
                class:     NewsletterManager
                properties:
                    mailer: @my_mailer

    .. code-block:: xml

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="NewsletterManager">
                <property name="mailer" type="service" id="my_mailer" />
            </service>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setDefinition('my_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            'NewsletterManager'
        ))->setProperty('mailer', new Reference('my_mailer')));

Utiliser l'injection via une propriété n'apporte presque que des inconvénients,
cette méthode est similaire à l'injection par mutateur mais avec d'autres
problèmes importants en plus :

* Vous ne pouvez pas du tout contrôler quand la dépendance est définie,
  elle peut être changée à n'importe quel moment pendant la durée de vie
  de l'objet ;

* Vous ne pouvez pas utiliser la détection de type donc vous ne pouvez
  pas être sûr du type de la dépendance injectée excepté si vous écrivez
  dans le code de la classe un test qui vérifie l'objet instancié avant
  de l'utiliser.

Mais, il est utile de savoir que ceci peut être effectué avec le conteneur
de service, spécialement si vous travaillez avec du code qui n'est pas
sous votre contrôle, comme avec une bibliothèque tierce, qui utilise des
propriétés publiques pour ses dépendances.
