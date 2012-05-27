.. index::
   single: Service Container
   single: Dependency Injection Container

Service Container
=================

Une application PHP moderne est plein d'objets. Un objet peut faciliter l'envoi
des messages e-mail, tandis qu'un autre peut vous permettre de persister les informations
dans une base de données. Dans votre application, vous pouvez créer un objet qui gère
votre inventaire de produits, ou tout autre objet qui traite des données via une API 
tierce. Le fait est qu'une application moderne fait beaucoup de choses et est organisée
entre de nombreux objets qui gèrent chaque tâche.

Dans ce chapitre, nous allons parler d'un objet spécial PHP dans Symfony2 qui vous aide
à instancier, organiser et récupérer les nombreux objets de votre application.
Cet objet, appelé un conteneur de services, vous permettra de standardiser et
centraliser la façon dont les objets sont construits dans votre application. Le conteneur
vous facilite la vie, est super rapide, et met en valeur une architecture qui
encourage un code réutilisable et découplé. Et puisque toutes les classes fondamentales de Symfony2
utilisent le conteneur, vous allez apprendre comment étendre, configurer et utiliser n'importe quel objet
dans Symfony2. En bien des aspects, le conteneur de services est le principal responsable 
de la vitesse et de l'extensibilité de Symfony2.

Enfin, configurer et utiliser le conteneur de services est facile. A la fin
de ce chapitre, vous serez à l'aise pour créer vos propres objets via le
conteneur et pour personnaliser des objets provenant de bundles tiers. Vous allez commencer
à écrire du code qui est plus réutilisable, testable et découplé, tout simplement parce
le conteneur de services facilite l'écriture de code de qualité.

.. index::
   single: Service Container; Qu'est-ce qu'un service ?

Qu'est-ce qu'un Service ?
-------------------------

Plus simplement, un :term:`Service` désigne tout objet PHP qui effectue une sorte de 
tâche « globale ». C'est un nom générique utilisé en informatique
pour décrire un objet qui est créé dans un but précis (par ex. l'envoi des
emails). Chaque service est utilisé tout au long de votre application lorsque vous avez besoin
de la fonctionnalité spécifique qu'il fournit. Vous n'avez pas à faire quelque chose de spécial
pour fabriquer un service : il suffit d'écrire une classe PHP avec un code qui accomplit
une tâche spécifique. Félicitations, vous venez tout juste de créer un service !

.. note::

    En règle générale, un objet PHP est un service s'il est utilisé de façon globale dans votre
    application. Un seul service ``Mailer`` est utilisé globalement pour envoyer des
    messages email tandis que les nombreux objets ``Message`` qu'il délivre
    ne sont *pas* des services. De même, un objet ``Product`` n'est pas un service,
    mais un objet qui persiste des objets ``Product`` dans une base de données *est* un service.

Alors quel est l'avantage ? L'avantage de réfléchir sur les « services » est
que vous commencez à penser à séparer chaque morceau de fonctionnalité dans votre
application dans une série de services. Puisque chaque service ne réalise qu'une fonction,
vous pouvez facilement accéder à chaque service et utiliser ses fonctionnalités chaque fois que vous
en avez besoin. Chaque service peut également être plus facilement testé et configuré puisqu'il
est séparé des autres fonctionnalités de votre application. Cette idée
est appelée `service-oriented architecture`_ et n'est pas unique à Symfony2
ou encore PHP. Structurer votre application autour d'un ensemble de classes de services indépendants
est une bonne pratique orientée objet célèbre et fiable. Ces compétences
sont les clés pour devenir un bon développeur dans presque tous les langages.
	
.. index::
   single: Service Container ; Définition

Définition d'un Conteneur de Services
-------------------------------------

Un :term:`Service Container` (ou *dependency injection container*) est simplement
un objet PHP qui gère l'instanciation des services (c-a-d objets).
Par exemple, supposons que nous avons une simple classe PHP qui envoie des messages email.
Sans un conteneur de services, nous devons manuellement créer l'objet chaque fois que
nous en avons besoin :

.. code-block:: php

    use Acme\HelloBundle\Mailer;

    $mailer = new Mailer('sendmail');
    $mailer->send('ryan@foobar.net', ... );

Ceci est assez facile. La classe imaginaire ``Mailer`` nous permet de configurer
la méthode utilisée pour envoyer les messages par e-mail (par exemple ``sendmail``, ``smtp``, etc)
Mais que faire si nous voulions utiliser le service mailer ailleurs ? Nous ne 
voulons certainement pas répéter la configuration du mailer *chaque* fois que nous devons utiliser
l'objet ``Mailer``. Que se passe-t-il si nous avions besoin de changer le ``transport`` de
``sendmail`` à ``smtp`` partout dans l'application ? Nous aurions besoin de traquer 
chaque endroit où nous avons créé un service ``Mailer`` et de le changer.

.. index::
   single: Service Container; Configuring services

Créer/Configurer les services dans le Conteneur
-----------------------------------------------

Une meilleure solution est de laisser le conteneur de services créer l'objet ``Mailer``
pour vous. Pour que cela fonctionne, nous devons *spécifier* au conteneur comment
créer le ``Mailer``. Cela se fait via la configuration, qui peut
être spécifiée en YAML, XML ou PHP :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            my_mailer:
                class:        Acme\HelloBundle\Mailer
                arguments:    [sendmail]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
            <service id="my_mailer" class="Acme\HelloBundle\Mailer">
                <argument>sendmail</argument>
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setDefinition('my_mailer', new Definition(
            'Acme\HelloBundle\Mailer',
            array('sendmail')
        ));

.. note::

    Lorsque Symfony2 s'initialise, il construit le conteneur de services en utilisant la
    configuration de l'application (``app/config/config.yml`` par défaut). Le
    fichier exact qui est chargé est dicté par la méthode ``AppKernel::registerContainerConfiguration()``,
    qui charge un fichier de configuration spécifique à l'environnement (par exemple
    ``config_dev.yml`` pour l'environnement de ``dev`` ou ``config_prod.yml``
    pour la ``prod``).

Une instance de l'objet ``Acme\HelloBundle\Mailer`` est maintenant disponible via 
le conteneur de services. Le conteneur est disponible dans tous les contrôleurs traditionnels
de Symfony2 où vous pouvez accéder aux services du conteneur via la méthode 
de raccourci ``get()`` :

.. code-block:: php

    class HelloController extends Controller
    {
        // ...

        public function sendEmailAction()
        {
            // ...
            $mailer = $this->get('my_mailer');
            $mailer->send('ryan@foobar.net', ... );
        }
    }

Lorsque nous demandons le service ``my_mailer``  du conteneur, le conteneur
construit l'objet et le retourne. Ceci est un autre avantage majeur 
d'utiliser le conteneur de services. A savoir, un service est *jamais* construit avant
qu'il ne soit nécessaire. Si vous définissez un service et ne l'utilisez jamais sur une demande, le service
n'est jamais créé. Cela permet d'économiser la mémoire et d'augmenter la vitesse de votre application.
Cela signifie aussi qu'il y a très peu ou pas d'impact de performance en définissant 
beaucoup de services. Les services qui ne sont jamais utilisés ne sont jamais construits.

Comme bonus supplémentaire, le service ``Mailer`` est seulement créé une fois et la même
instance est retournée chaque fois que vous demandez le service. Ceci est presque toujours
le comportement dont vous aurez besoin (c'est plus souple et plus puissant), mais nous allons apprendre
plus tard, comment vous pouvez configurer un service qui a de multiples instances.
	
.. _book-service-container-parameters:

Paramètres de Service
---------------------

La création de nouveaux services (c-a-d objets) via le conteneur est assez 
simple. Les paramètres rendent les définitions de services plus organisées et flexibles :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            my_mailer.class:      Acme\HelloBundle\Mailer
            my_mailer.transport:  sendmail

        services:
            my_mailer:
                class:        %my_mailer.class%
                arguments:    [%my_mailer.transport%]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="my_mailer.class">Acme\HelloBundle\Mailer</parameter>
            <parameter key="my_mailer.transport">sendmail</parameter>
        </parameters>

        <services>
            <service id="my_mailer" class="%my_mailer.class%">
                <argument>%my_mailer.transport%</argument>
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.class', 'Acme\HelloBundle\Mailer');
        $container->setParameter('my_mailer.transport', 'sendmail');

        $container->setDefinition('my_mailer', new Definition(
            '%my_mailer.class%',
            array('%my_mailer.transport%')
        ));

Le résultat final est exactement le même que précédemment - la différence est seulement dans
* comment * nous avons défini le service. En entourant les chaînes ``my_mailer.class`` et
``my_mailer.transport`` par le signe pourcent (``%``), le conteneur sait qu'il 
faut chercher des paramètres avec ces noms. Quand le conteneur est construit, il
cherche la valeur de chaque paramètre et l'utilise dans la définition du service.

.. note::

    Le signe pourcent au sein d'un paramètre ou d'un argument, et qui fait partie
    de la chaine de caractères, doit être échappé par un autre signe pourcent :

    .. code-block:: xml

        <argument type="string">http://symfony.com/?foo=%%s&bar=%%d</argument>

Le but des paramètres est de fournir l'information dans les services. Bien sûr,
il n'y avait rien de mal à définir le service sans utiliser de paramètre.
Les paramètres, cependant, ont plusieurs avantages :

* la séparation et l'organisation de toutes les « options » de service sous une seule
  clé de ``paramètres`` ;

* les valeurs de paramètres peuvent être utilisées dans de multiples définitions de service ;

* Lors de la création d'un service dans un bundle (nous allons voir ceci sous peu), utiliser les paramètres
  permet au service d'être facilement personnalisé dans votre application.

Le choix d'utiliser ou non des paramètres dépend de vous. Les bundles 
tiers de haute qualité utiliseront *toujours* les paramètres puisqu'ils rendent le service 
stocké dans le conteneur plus configurable. Pour les services dans votre application,
cependant, vous pouvez ne pas avoir besoin de la flexibilité des paramètres.  

Tableaux de paramètres
~~~~~~~~~~~~~~~~~~~~~~

Les paramètres ne sont pas obligatoirement des chaines de caractères, ils peuvent aussi
être des tableaux. Pour le format XML, vous devez utiliser l'attribut type="collection"
pour tous les paramètres qui sont des tableaux.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            my_mailer.gateways:
                - mail1
                - mail2
                - mail3
            my_multilang.language_fallback:
                en:
                    - en
                    - fr
                fr:
                    - fr
                    - en

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="my_mailer.gateways" type="collection">
                <parameter>mail1</parameter>
                <parameter>mail2</parameter>
                <parameter>mail3</parameter>
            </parameter>
            <parameter key="my_multilang.language_fallback" type="collection">
                <parameter key="en" type="collection">
                    <parameter>en</parameter>
                    <parameter>fr</parameter>
                </parameter>
                <parameter key="fr" type="collection">
                    <parameter>fr</parameter>
                    <parameter>en</parameter>
                </parameter>
            </parameter>
        </parameters>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.gateways', array('mail1', 'mail2', 'mail3'));
        $container->setParameter('my_multilang.language_fallback',
                                 array('en' => array('en', 'fr'),
                                       'fr' => array('fr', 'en'),
                                ));

Importer d'autres Ressources de Configuration de Conteneur
----------------------------------------------------------

.. tip::

    Dans cette section, nous allons faire référence aux fichiers de configuration de service comme des *ressources*.
    C'est pour souligner le fait que, alors que la plupart des ressources de configuration
    sont des fichiers (par exemple YAML, XML, PHP), Symfony2 est si flexible que la configuration
    pourrait être chargée de n'importe où (par exemple une base de données ou même via un service
    web externe).
	
Le conteneur de services est construit en utilisant une ressource de configuration unique
(``app/config/config.yml`` par défaut). Toutes les autres configurations de service
(y compris la configuration du noyau de Symfony2 et des bundle tiers) doivent
être importées à l'intérieur de ce fichier d'une manière ou d'une autre. Cela vous donne une
flexibilité absolue sur les services dans votre application.

La configuration des services externes peut être importée de deux manières différentes. Tout d'abord,
nous allons parler de la méthode que vous utiliserez le plus souvent dans votre application :
la directive ``imports``. Dans la section suivante, nous allons introduire la
deuxième méthode, qui est la méthode flexible et préférée pour l'importation de 
configuration de services des bundles tiers.


.. index::
   single: Service Container; imports

.. _service-container-imports-directive:

Importer la Configuration avec ``imports``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jusqu'ici, nous avons placé notre définition de conteneur de service ``my_mailer`` directement
dans le fichier de configuration de l'application (par exemple ``app/config/config.yml``).
Bien sûr, puisque la classe ``Mailer`` elle-même vit à l'intérieur de ``AcmeHelloBundle``,
il est plus logique de mettre la définition du conteneur ``my_mailer`` à l'intérieur du
bundle aussi.

Tout d'abord, déplacez la définition du conteneur ``my_mailer`` dans un nouveau fichier de ressource de conteneur à l'intérieur d' ``AcmeHelloBundle``. Si les répertoires ``Resources``
ou ``Resources/config`` n'existent pas, créez-les.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            my_mailer.class:      Acme\HelloBundle\Mailer
            my_mailer.transport:  sendmail

        services:
            my_mailer:
                class:        %my_mailer.class%
                arguments:    [%my_mailer.transport%]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <parameter key="my_mailer.class">Acme\HelloBundle\Mailer</parameter>
            <parameter key="my_mailer.transport">sendmail</parameter>
        </parameters>

        <services>
            <service id="my_mailer" class="%my_mailer.class%">
                <argument>%my_mailer.transport%</argument>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.class', 'Acme\HelloBundle\Mailer');
        $container->setParameter('my_mailer.transport', 'sendmail');

        $container->setDefinition('my_mailer', new Definition(
            '%my_mailer.class%',
            array('%my_mailer.transport%')
        ));

La définition elle-même n'a pas changé, seulement son emplacement. Bien sûr, le conteneur
de service ne connait pas le nouveau fichier de ressources. Heureusement, nous pouvons
facilement importer le fichier de ressources en utilisant la clé ``imports`` dans 
la configuration de l'application.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            hello_bundle:
		 - { resource: @AcmeHelloBundle/Resources/config/services.yml }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <import resource="@AcmeHelloBundle/Resources/config/services.xml"/>
        </imports>

    .. code-block:: php

        // app/config/config.php
        $this->import('@AcmeHelloBundle/Resources/config/services.php');

La directive ``imports`` permet à votre application d'inclure des ressources de configuration
de conteneur de services de n'importe quel autre emplacement (le plus souvent à partir de bundles).
L'emplacement ``resource``, pour les fichiers, est le chemin absolu du fichier de
ressource. La syntaxe spéciale ``@AcmeHello`` résout le chemin du répertoire du
bundle ``AcmeHelloBundle``. Cela vous aide à spécifier le chemin vers la ressource
sans se soucier plus tard, si vous déplacez le ``AcmeHelloBundle`` dans un autre
répertoire.

.. index::
   single: Service Container; Extension configuration

.. _service-container-extension-configuration:

Importer la Configuration via les Extensions de Conteneur
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quand vous développerez avec Symfony2, vous utiliserez le plus souvent la directive ``imports``
pour importer la configuration du conteneur des bundles que vous avez créé spécifiquement
pour votre application. Les configurations des conteneurs des bundles tiers, y compris
les services du noyau de Symfony2, sont habituellement chargés en utilisant une autre méthode qui est plus
souple et facile à configurer dans votre application.

Voici comment cela fonctionne. En interne, chaque bundle définit ses services 
comme nous avons vu jusqu'à présent. A savoir, un bundle utilise un ou plusieurs fichiers de 
ressources de configuration (généralement XML) pour spécifier les paramètres et les services pour ce
bundle. Cependant, au lieu d'importer chacune de ces ressources directement à partir de
la configuration de votre application en utilisant la directive ``imports``, vous pouvez simplement
invoquer une *extension du conteneur de services* à l'intérieur du bundle qui fait le travail pour
vous. Une extension de conteneur de services est une classe PHP créée par l'auteur du bundle
afin d'accomplir deux choses :

* importer toutes les ressources du conteneur de services nécessaires pour configurer les services
  pour le bundle ;

* fournir une configuration sémantique, simple de sorte que le bundle peut
  être configuré sans interagir avec les paramètres de la 
  configuration du conteneur de services du bundle.  

En d'autres termes, une extension de conteneur de services configure les services pour
un bundle en votre nom. Et comme nous le verrons dans un instant, l'extension fournit
une interface pratique, de haut niveau pour configurer le bundle.

Prenez le ``FrameworkBundle`` - le bundle noyau du framework Symfony2 - comme un
exemple. La présence du code suivant dans votre configuration de l'application
invoque l'extension du conteneur de services à l'intérieur du ``FrameworkBundle`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            secret:          xxxxxxxxxx
            charset:         UTF-8
            form:            true
            csrf_protection: true
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config charset="UTF-8" secret="xxxxxxxxxx">
            <framework:form />
            <framework:csrf-protection />
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
            <!-- ... -->
        </framework>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'secret'          => 'xxxxxxxxxx',
            'charset'         => 'UTF-8',
            'form'            => array(),
            'csrf-protection' => array(),
            'router'          => array('resource' => '%kernel.root_dir%/config/routing.php'),
            // ...
        ));

Lorsque la configuration est analysée, le conteneur cherche une extension qui
peut gérer la directive de configuration du ``framework``. L'extension en question,
qui vit dans le ``FrameworkBundle``, est invoquée et la configuration du service
pour le ``FrameworkBundle`` est chargée. Si vous retirez la clé ``framework`` 
de votre fichier de configuration de l'application entièrement, les services noyau de Symfony2
ne seront pas chargés. Le fait est que vous avez la maîtrise : le framework Symfony2
ne contient pas de magie et n'effectue aucune action dont vous n'avez pas le contrôle
dessus.

Bien sûr, vous pouvez faire beaucoup plus que simplement « activer » l'extension du conteneur 
de services du ``FrameworkBundle``. Chaque extension vous permet de facilement
personnaliser le bundle, sans se soucier de la manière dont les services internes sont
définis.

Dans ce cas, l'extension vous permet de personnaliser la ``charset``, ``error_handler``,
``csrf_protection``, ``router`` et bien plus encore. En interne,
le ``FrameworkBundle`` utilise les options spécifiées ici pour définir et configurer
les services qui lui sont spécifiques. Le bundle se charge de créer tous les  
``paramètres`` et ``services`` nécessaires pour le conteneur du service, tout en permettant
une grande partie de la configuration d'être facilement personnalisée. Comme bonus supplémentaire, la plupart des
extensions du conteneur de services sont assez malines pour effectuer la validation -
vous informant des options qui sont manquantes ou du mauvais type de données.

Lors de l'installation ou la configuration d'un bundle, consultez la documentation du bundle pour
savoir comment installer et configurer les services pour le bundle. Les options
disponibles pour les bundles du noyau peuvent être trouvées à :doc:`Reference Guide</reference/index>`.

.. note::

   Nativement, le conteneur de services reconnait seulement les
   directives ``parameters``, ``services``, et ``imports``. Toutes les autres directives
   sont gérées par une extension du conteneur de service.

Si vous voulez exposer une configuration conviviale dans vos propres bundles, lisez
l'entrée du cookbook ":doc:`/cookbook/bundles/extension`".

.. index::
   single: Service Container; Referencing services

Reférencer (Injecter) les Services
----------------------------------

Jusqu'à présent, notre service originel ``my_mailer`` est simple : il suffit d'un seul paramètre
dans son constructeur, qui est facilement configurable. Comme vous le verrez, la vraie
puissance du conteneur est démontrée lorsque vous avez besoin de créer un service qui
dépend d'un ou plusieurs autres services dans le conteneur.

Commençons par un exemple. Supposons que nous ayons un nouveau service, ``NewsletterManager`` ,
qui aide à gérer la préparation et l'envoi d'un message email à
une liste d'adresses. Bien sûr, le service ``my_mailer`` excelle 
vraiment pour envoyer des messages email, donc nous allons l'utiliser dans ``NewsletterManager``
pour gérer l'envoi effectif des messages. Cette fausse classe pourrait ressembler à 
quelque chose comme ceci :

.. code-block:: php

    namespace Acme\HelloBundle\Newsletter;

    use Acme\HelloBundle\Mailer;

    class NewsletterManager
    {
        protected $mailer;

        public function __construct(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

Sans utiliser le conteneur de services, nous pouvons créer une nouvelle ``NewsletterManager`` 
assez facilement à l'intérieur d'un contrôleur :

.. code-block:: php

    public function sendNewsletterAction()
    {
        $mailer = $this->get('my_mailer');
        $newsletter = new Acme\HelloBundle\Newsletter\NewsletterManager($mailer);
        // ...
    }

Cette approche est pas mal, mais si nous décidons plus tard que la classe ``NewsletterManager``
a besoin d'un deuxième ou troisième paramètre de constructeur ? Que se passe-t-il si nous décidons de
refactoriser notre code et de renommer la classe ? Dans les deux cas, vous auriez besoin de trouver tous les
endroits où le ``NewsletterManager`` a été instancié et de le modifier. Bien sûr,
le conteneur de services nous donne une option beaucoup plus attrayante :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager

        services:
            my_mailer:
                # ...
            newsletter_manager:
                class:     %newsletter_manager.class%
                arguments: [@my_mailer]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <argument type="service" id="my_mailer"/>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(new Reference('my_mailer'))
        ));

En YAML, la syntaxe spéciale ``@my_mailer`` indique au conteneur de chercher 
un service nommé ``my_mailer`` et de transmettre cet objet dans le constructeur
de ``NewsletterManager``. Dans ce cas, cependant, le service spécifié ``my_mailer`` 
doit exister. Si ce n'est pas le cas, une exception sera levée. Vous pouvez marquer vos
dépendances comme facultatives - nous en parlerons dans la section suivante.

Utiliser des références est un outil très puissant qui vous permet de créer des classes
de services indépendantes avec des dépendances bien définies. Dans cet exemple, le service
``newsletter_manager`` a besoin du service ``my_mailer`` afin de fonctionner. Lorsque vous définissez
cette dépendance dans le conteneur de service, le conteneur prend soin de tout
le travail de l'instanciation des objets.

Dépendances optionnelles : Setter Injection
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'injection de dépendances dans le constructeur de cette manière est un excellent
moyen de s'assurer que la dépendance est disponible pour utilisation. Si vous avez des
dépendances optionnelles pour une classe, alors la méthode « setter injection » peut
être une meilleure option. Cela signifie d'injecter la dépendance en utilisant un
appel de méthode plutôt que par le constructeur. La classe devrait ressembler à ceci :

.. code-block:: php

    namespace Acme\HelloBundle\Newsletter;

    use Acme\HelloBundle\Mailer;

    class NewsletterManager
    {
        protected $mailer;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

L'injection de la dépendance par la méthode setter a juste besoin d'un changement de la syntaxe :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager

        services:
            my_mailer:
                # ...
            newsletter_manager:
                class:     %newsletter_manager.class%
                calls:
                    - [ setMailer, [ @my_mailer ] ]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ));

.. note::

    Les approches présentées dans cette section sont appelées « constructor injection »
    et « setter injection« ». Le conteneur de service Symfony2 supporte aussi 
    « property injection« ».

Rendre les Références Optionnelles
----------------------------------

Parfois, un de vos services peut avoir une dépendance optionnelle, ce qui signifie
que la dépendance n'est pas requise par le service pour fonctionner correctement. Dans
l'exemple ci-dessus, le service ``my_mailer`` *doit* exister, sinon une exception
sera levée. En modifiant les définitions du service ``newsletter_manager``,
vous pouvez rendre cette référence optionnelle. Le conteneur va ensuite l'injecter si
elle existe et ne rien faire si ce n'est pas le cas :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...

        services:
            newsletter_manager:
                class:     %newsletter_manager.class%
                arguments: [@?my_mailer]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <argument type="service" id="my_mailer" on-invalid="ignore" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;
        use Symfony\Component\DependencyInjection\ContainerInterface;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(new Reference('my_mailer', ContainerInterface::IGNORE_ON_INVALID_REFERENCE))
        ));

En YAML, la syntaxe spéciale ``@?`` indique au conteneur de service que la dependance
est optionnelle. Bien sûr, le ``NewsletterManager`` doit être aussi écrit pour 
permettre une dépendance optionnelle :

.. code-block:: php

        public function __construct(Mailer $mailer = null)
        {
            // ...
        }

Services de Bundle Tiers et Noyau de Symfony
--------------------------------------------

Etant donné que Symfony2 et tous les bundles tiers configurent et récupèrent leurs services
via le conteneur, vous pouvez facilement y accéder, ou même les utiliser dans vos propres
services. Pour garder les choses simples, par défaut Symfony2 n'exige pas que
les contrôleurs soient définis comme des services. Par ailleurs Symfony2 injecte l'ensemble du
conteneur de services dans votre contrôleur. Par exemple, pour gérer le stockage 
des informations sur une session utilisateur, Symfony2 fournit un service ``session``,
auquel vous pouvez accéder de l'intérieur d'un contrôleur standard comme suit :

.. code-block:: php

    public function indexAction($bar)
    {
        $session = $this->get('session');
        $session->set('foo', $bar);

        // ...
    }

Dans Symfony2, vous allez constamment utiliser les services fournis par le noyau de Symfony ou
autres bundles tiers pour effectuer des tâches telles que rendre des templates (``templating``),
envoyer des emails (``mailer``), ou d'accéder à des informations sur la requête (``request``).

Nous pouvons aller plus loin en utilisant ces services à l'intérieur des services
que vous avez créés pour votre application. Modifions le ``NewsletterManager``
afin d'utiliser le vrai service ``mailer`` de Symfony2 (au lieu du faux ``my_mailer``).
Passons aussi le service du moteur de template à ``NewsletterManager``
afin qu'il puisse générer le contenu de l'email via un template :

.. code-block:: php

    namespace Acme\HelloBundle\Newsletter;

    use Symfony\Component\Templating\EngineInterface;

    class NewsletterManager
    {
        protected $mailer;

        protected $templating;

        public function __construct(\Swift_Mailer $mailer, EngineInterface $templating)
        {
            $this->mailer = $mailer;
            $this->templating = $templating;
        }

        // ...
    }

Configurer le conteneur de services est facile :

.. configuration-block::

    .. code-block:: yaml

        services:
            newsletter_manager:
                class:     %newsletter_manager.class%
                arguments: [@mailer, @templating]

    .. code-block:: xml

        <service id="newsletter_manager" class="%newsletter_manager.class%">
            <argument type="service" id="mailer"/>
            <argument type="service" id="templating"/>
        </service>

    .. code-block:: php

        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(
                new Reference('mailer'),
                new Reference('templating')
            )
        ));

Le service ``newsletter_manager`` a désormais accès aux services noyau ``mailer`` 
et ``templating``. C'est une façon commune de créer des services spécifiques
à votre application qui exploitent la puissance des différents services au sein
du framework.

.. tip::

    Soyez sûr que l'entrée ``swiftmailer`` apparaît dans votre configuration de
    l'application. Comme nous l'avons mentionné dans :ref:`service-container-extension-configuration`,
    la clé ``swiftmailer`` invoque l'extension du service de 
    ``SwiftmailerBundle``, qui déclare le service ``mailer``.

.. index::
   single: Service Container; Advanced configuration

Configuration de Conteneur Avancée
----------------------------------

Comme nous l'avons vu, la définition des services à l'intérieur du conteneur est facile, en général
impliquant une clé de configuration de ``service`` et quelques paramètres. Cependant,
le conteneur a plusieurs autres outils disponibles qui aident à *tagger* les services
pour des fonctionnalités spéciales, créent des services plus complexes, et effectuent des opérations
après que le conteneur soit construit.

Marquer les Services comme public / privé
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Lors de la définition des services, vous souhaitez généralement être en mesure d'accéder à ces définitions
au sein de votre code d'application. Ces services sont appelés ``publics``. Par exemple,
le service ``doctrine`` enregistré avec le conteneur en utilisant le DoctrineBundle
est un service public que vous pouvez y accéder via :

.. code-block:: php

   $doctrine = $container->get('doctrine');

Cependant, il y a des cas utiles où vous ne voulez pas qu'un service soit public. Cela
est courant quand un service est seulement défini car il pourrait être utilisé comme un
paramètre pour un autre service.

.. note::

    Si vous utilisez un service privé comme un paramètre pour plus d'un autre service,
    cela se traduira par deux instances différentes étant utilisées puisque l'instanciation
    du service privé se fait « inline » (par exemple ``new PrivateFooBar()``).

Autrement dit : un service sera privé lorsque vous ne voulez pas y accéder
directement à partir de votre code.

Voici un exemple :

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Acme\HelloBundle\Foo
             public: false

    .. code-block:: xml

        <service id="foo" class="Acme\HelloBundle\Foo" public="false" />

    .. code-block:: php

        $definition = new Definition('Acme\HelloBundle\Foo');
        $definition->setPublic(false);
        $container->setDefinition('foo', $definition);

Maintenant que le service est privé, vous *ne pouvez pas* appeler :

.. code-block:: php

    $container->get('foo');

Cependant, si un service a été marqué comme privé, vous pouvez toujours le mettre en alias (voir
ci-dessous) pour accéder à ce service (via l'alias).

.. note::

   Les services sont publics par défaut.

Aliasing
~~~~~~~~

Lors de l'utilisation des bundles de base ou tiers au sein de votre application, vous voudriez peut-être
utiliser des raccourcis pour accéder à certains services. Vous pouvez le faire en les mettant en alias et,
en outre, vous pouvez même mettre en alias les services non publics.

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Acme\HelloBundle\Foo
           bar:
             alias: foo

    .. code-block:: xml

        <service id="foo" class="Acme\HelloBundle\Foo"/>

        <service id="bar" alias="foo" />

    .. code-block:: php

        $definition = new Definition('Acme\HelloBundle\Foo');
        $container->setDefinition('foo', $definition);

        $containerBuilder->setAlias('bar', 'foo');

Cela signifie que quand vous utilisez le conteneur directement, vous pouvez accéder au 
service  ``foo`` en demandant le service ``bar`` comme ceci :

.. code-block:: php

    $container->get('bar'); // Would return the foo service

Fichiers Requis
~~~~~~~~~~~~~~~

Il pourrait y avoir des cas utiles où vous avez besoin d'inclure un autre fichier juste avant
que le service proprement dit ne soit chargé. Pour ce faire, vous pouvez utiliser la directive ``file``.

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Acme\HelloBundle\Foo\Bar
             file: %kernel.root_dir%/src/path/to/file/foo.php

    .. code-block:: xml

        <service id="foo" class="Acme\HelloBundle\Foo\Bar">
            <file>%kernel.root_dir%/src/path/to/file/foo.php</file>
        </service>

    .. code-block:: php

        $definition = new Definition('Acme\HelloBundle\Foo\Bar');
        $definition->setFile('%kernel.root_dir%/src/path/to/file/foo.php');
        $container->setDefinition('foo', $definition);

Veuillez noter que symfony appelera en interne la fonction PHP require_once
ce qui signifie que votre fichier ne sera inclu qu'une seule fois par requête.

.. _book-service-container-tags:

Tags (``tags``)
~~~~~~~~~~~~~~~

De la même manière qu'un billet de blog sur le Web pourrait être taggé avec des noms
telles que « Symfony » ou « PHP », les services configurés dans votre conteneur peuvent également être
taggés. Dans le conteneur de services, un tag laisse supposer que le service est censé
être utilisé dans un but précis. Prenons l'exemple suivant :

.. configuration-block::

    .. code-block:: yaml

        services:
            foo.twig.extension:
                class: Acme\HelloBundle\Extension\FooExtension
                tags:
                    -  { name: twig.extension }

    .. code-block:: xml

        <service id="foo.twig.extension" class="Acme\HelloBundle\Extension\FooExtension">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $definition = new Definition('Acme\HelloBundle\Extension\FooExtension');
        $definition->addTag('twig.extension');
        $container->setDefinition('foo.twig.extension', $definition);

Le tag ``twig.extension`` est un tag spécial que le ``TwigBundle`` utilise 
pendant la configuration. En donnant au service ce tag ``twig.extension``,
le bundle sait que le service ``foo.twig.extension`` devrait être enregistré
comme une extension Twig avec Twig. En d'autres termes, Twig trouve tous les services taggés
avec ``twig.extension`` et les enregistre automatiquement comme des extensions.

Les tags, alors, sont un moyen de dire aux bundles de Symfony2 ou tiers que
votre service doit être enregistré ou utilisé d'une manière spéciale par le bundle.

Ce qui suit est une liste de tags disponibles avec les bundles noyau de Symfony2.
Chacun d'eux a un effet différent sur votre service et de nombreuses tags nécessitent
des paramètres supplémentaires (au-delà du paramètre ``name``).

* assetic.filter
* assetic.templating.php
* data_collector
* form.field_factory.guesser
* kernel.cache_warmer
* kernel.event_listener
* monolog.logger
* routing.loader
* security.listener.factory
* security.voter
* templating.helper
* twig.extension
* translation.loader
* validator.constraint_validator

Apprenez en plus
----------------

* :doc:`/components/dependency_injection/factories`
* :doc:`/components/dependency_injection/parentservices`
* :doc:`/cookbook/controller/service`

.. _`service-oriented architecture`: http://wikipedia.org/wiki/Service-oriented_architecture
