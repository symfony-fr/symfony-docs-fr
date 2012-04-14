Comment définir des champs d'applications (scopes) pour vos services
====================================================================

Nous traiterons ici des champs d'applications ou scopes, un sujet d'un niveau
avancé en relation avec :doc:`/book/service_container`. Si vous avez déjà 
observé une erreur mentionnant le terme "scopes" quand vous créez vos services,
ou avez eu besoin de créer de services qui dépendent du service `request`,
cet article est fait pour vous.

Comprendre les champs d'applications
------------------------------------

Le champs d'application d'un service contrôle les interactions d'une instance
de ce service avec son conteneur. Le composant d'injection de dépendance 
fourni deux champs d'application génériques:

- `container` (valeur par défaut): La même instance est utilisé à chaque 
  requête.

- `prototype`: Une nouvelle instance est créer à chaque requête.

Le FrameworkBundle définit lui un troisième champs d'application: `request`. 
Celui-ci le lie à la requête. Ainsi pour chaque sous-requête une nouvelle
instance est créée. La conséquence est qu'il est indisponible en dehors 
de la requête (en ligne de commande par example).

Les champs d'applications ajoutent une contrainte sur les dépendances d'un 
services : un service ne peut pas dépendre d'un champs d'application moins large.
Par example, si vous créez un service generique `my_foo`, mais essayez d'injecter
le composant `request`, vous recevrez une 
:class:`Symfony\\Component\\DependencyInjection\\Exception\\ScopeWideningInjectionException`
au moment de la compilation du conteneur. Pour plus de détails lisez les notes dans
la barre latérale.

.. sidebar:: Champs d'application et dépendances

    Imaginez que vous avez configurer un service `my_mailer`, sans configurer de
    champs d'application pour ce service, par défaut il sera réglé sur `container`.
    Ainsi chaque fois que vous appelerez le conteneur du service `my_mailer`, vous
    recevrez le même objet. Ce qui est habituel dans l'utilisation d'un service.
    
    Imaginez cependant que vous ayez besoin du service `request` dans votre service
    `my_mailer`, peut être parce que vous lisez une URL de la requête courante.
    Vous l'ajouter comme un argument du constructeur. Observons quelles pourraient
	être les conséquences:

    * Quand vous appelez `my_mailer`, une instance de `my_mailer` (appelons le
      *MailerA*) est créer et un service `request` (*RequestA*) lui est envoyé.
      Facile!

    * Vous effectué maintenant une sous requête dans Symfony, ce qui est une façon
      sympatique de dire que vous avez appelez, par exemple, la fonction twig
      `{% render ... %}`, à partir d'un autre controleur. En interne, l'ancien service
      `request` (*RequestA*) est remplacer ainsi par une nouvel instance (*RequestB*).
      Cela se déroule en arrière plan et est tout à fait normal.

    * Dans votre contrôleur intégré, vous interrogez une nouvelle fois le service
     `my_mailer`. Votre service ayant le champ d'application `container`, la 
      même  instance (*MailerA*) est réutilisée. Et voilà le problème: l'instance
      *MailerA* contient toujours l'ancien objet *RequestA*, qui *n'est plus* maintenant 
      l'objet requête mis à jour (*RequestB* est maintenant le service courant `request`).
      C'est subtile mais l'erreur pourrait engendrer des problèmes majeurs, et
	  cela explique pourquoi cela est interdit.

      Ainsi, voilà pourquoi les champs d'applications existent, et comment il peuvent
      causer des problèmes. En continuant cette lecture nous vous indiquerons les 
      solutions préconisées.

.. note::

    Un service peut bien entendu dépendre d'un service provenant d'un champs
	d'application plus étendu. 

Configurer le champs d'application dans la définition
-----------------------------------------------------

Le champs d'application d'un service est indiqué dans la définition de ce service
à l'aide du paramètre *scope*:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        services:
            greeting_card_manager:
                class: Acme\HelloBundle\Mail\GreetingCardManager
                scope: request

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <services>
            <service id="greeting_card_manager" class="Acme\HelloBundle\Mail\GreetingCardManager" scope="request" />
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setDefinition(
            'greeting_card_manager',
            new Definition('Acme\HelloBundle\Mail\GreetingCardManager')
        )->setScope('request');

Si vous n'indiquez pas ce paramètre, il sera lié par défaut au `conteneur`, ce qui
est le fonctionnement habituel d'un service. A moins que votre service ne dépende
d'un autre service qui soit dans un champs d'application plus restreint (le plus 
courant étant `request`), vous n'aurez probablement pas à modifier votre
configuration.

Utiliser un service provenant d'un champs d'application restreint
-----------------------------------------------------------------

Si votre service dépends d'un autre service au champs d'application déterminé,
la meilleure solution est de définir le même champs d'application pour celui-ci
(ou un champs d'application encore plus restreint). Habituellement, cela implique
de placer votre service dans le champs d'application `request`.

Mais celà n'est pas toujours possible (par exemple, un extension twig doit être 
dans le champs d'application `conteneur` au regard de l’environnement Twig 
dont elle est dépendante). Dans ces cas de figure, vous devrez configurer votre
conteneur en tant que service et charger les dépendances provenant d'un champs
d'application restreint à chaque appel, afin d'être certain d'obtenir les instances
mises à jour::

    namespace Acme\HelloBundle\Mail;

    use Symfony\Component\DependencyInjection\ContainerInterface;

    class Mailer
    {
        protected $container;

        public function __construct(ContainerInterface $container)
        {
            $this->container = $container;
        }

        public function sendEmail()
        {
            $request = $this->container->get('request');
            // Utilisez la requête ici
        }
    }

.. caution::

    Faites attention à ne pas enregistrer la requête dans une propriété de 
    votre objet pour un appel futur cela engendrerait les mêmes inconsistances
    que celles décrites précédemment (excepté que dans ce cas, Symfony ne pourrait 
	détecter cette erreur).

La configuration du service pour cette classe :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            my_mailer.class: Acme\HelloBundle\Mail\Mailer
        services:
            my_mailer:
                class:     %my_mailer.class%
                arguments:
                    - "@service_container"
                # scope: container can be omitted as it is the default

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="my_mailer.class">Acme\HelloBundle\Mail\Mailer</parameter>
        </parameters>

        <services>
            <service id="my_mailer" class="%my_mailer.class%">
                 <argument type="service" id="service_container" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('my_mailer.class', 'Acme\HelloBundle\Mail\Mailer');

        $container->setDefinition('my_mailer', new Definition(
            '%my_mailer.class%',
            array(new Reference('service_container'))
        ));

.. note::

    Injecter le container entier dans un service est généralement à proscrire
    (injectez seulement les paramètres utiles). Dans quelques rares cas, cela est 
    nécessaire,. Ainsi quand vous avez un service dans un champs d'application
	``container`` qui a besoin d'un service du champs d'application ``request``.

Si vous définissez un contrôleur comme un service alors vous pourrez appelez l'objet
``Request`` sans injecter le conteneur comme un argument de votre méthode action.
Voir :ref:`book-controller-request-argument` pour les détails.
