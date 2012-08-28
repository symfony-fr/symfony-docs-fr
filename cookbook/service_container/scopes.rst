.. index::
   single: Dependency Injection; Scopes

Comment travailler avec les champs d'applications (« scopes » en anglais)
=========================================================================

Nous traiterons ici des champs d'applications, un sujet d'un niveau
avancé en relation avec :doc:`/book/service_container`. Si vous avez déjà 
observé une erreur mentionnant le terme « scopes » lors de la création de vos
services, ou avez eu besoin de créer des services qui dépendent du service
`request`, cet article est fait pour vous.

Comprendre les champs d'applications
------------------------------------

Le champs d'application d'un service contrôle les intéractions d'une instance
de ce service avec son conteneur. Le composant d'injection de dépendance 
fourni deux champs d'applications génériques :

- `container` (valeur par défaut): la même instance est utilisée à chaque
  requête.

- `prototype`: Une nouvelle instance est créée à chaque requête.

Le FrameworkBundle définit lui un troisième champ d'application : `request`.
Celui-ci le lie à la requête. Ainsi pour chaque sous-requête une nouvelle
instance est créée. La conséquence est qu'il est indisponible en dehors 
de la requête (en ligne de commande par example).

Les champs d'applications ajoutent une contrainte sur les dépendances d'un 
service : un service ne peut pas dépendre d'un champ d'application moins large.
Par exemple, si vous créez un service générique `my_foo`, mais essayez d'injecter
le composant `request`, vous recevrez une 
:class:`Symfony\\Component\\DependencyInjection\\Exception\\ScopeWideningInjectionException`
au moment de la compilation du conteneur. Pour plus de détails, lisez les notes dans
la barre latérale.

.. sidebar:: Champs d'application et dépendances

    Imaginez que vous ayez configuré un service `my_mailer`, sans configurer de
    champs d'application pour ce service ; par défaut il sera réglé sur `container`.
    Ainsi chaque fois que vous appellerez le conteneur du service `my_mailer`, vous
    recevrez le même objet. Ce qui est habituel dans l'utilisation d'un service.
    
    Imaginez cependant que vous ayez besoin du service `request` dans votre service
    `my_mailer`, peut être parce que vous lisez une URL de la requête courante.
    Vous l'ajoutez comme un argument du constructeur. Observons quelles pourraient
    être les conséquences :

    * Quand vous appelez `my_mailer`, une instance de `my_mailer` (appelons le
      *MailerA*) est créée et un service `request` (*RequestA*) lui est envoyé.
      Facile!

    * Vous effectuez maintenant une sous-requête dans Symfony, ce qui est une façon
      sympatique de dire que vous avez appelez, par exemple, la fonction twig
      `{% render ... %}`, à partir d'un autre contrôleur. En interne, l'ancien service
      `request` (*RequestA*) est ainsi remplacé par une nouvelle instance (*RequestB*).
      Cela se déroule en arrière-plan et est tout à fait normal.

    * Dans votre contrôleur intégré, vous interrogez une nouvelle fois le service
      `my_mailer`. Votre service ayant le champ d'application `container`, la
      même  instance (*MailerA*) est réutilisée. Et voilà le problème : l'instance
      *MailerA* contient toujours l'ancien objet *RequestA*, qui *ne correspond plus*
      maintenant à l'objet requête mis à jour (*RequestB* est maintenant le service
      courant `request`). C'est subtile mais l'erreur pourrait engendrer des problèmes
      majeurs, et cela explique pourquoi cela est interdit.

      Ainsi, voilà pourquoi les champs d'applications existent, et comment il peuvent
      causer des problèmes. En continuant cette lecture nous vous indiquerons les 
      solutions préconisées.

.. note::

    Un service peut bien entendu dépendre d'un service provenant d'un champ
    d'application plus étendu.

Configurer le champ d'application dans la définition
----------------------------------------------------

Le champ d'application d'un service est indiqué dans la définition de ce service
à l'aide du paramètre *scope* :

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
d'un autre service qui soit dans un champ d'application plus restreint (le plus
courant étant `request`), vous n'aurez probablement pas à modifier votre
configuration.

Utiliser un service provenant d'un champ d'application restreint
----------------------------------------------------------------

Si votre service dépend d'un autre service au champ d'application déterminé,
la meilleure solution est de définir le même champ d'application pour celui-ci
(ou un champ d'application encore plus restreint). Habituellement, cela implique
de placer votre service dans le champ d'application `request`.

Mais celà n'est pas toujours possible (par exemple, une extension twig doit être
dans le champ d'application `conteneur` au regard de l’environnement Twig
dont elle est dépendante). Dans ces cas de figure, vous devrez configurer votre
conteneur en tant que service et charger les dépendances provenant d'un champ
d'application restreint à chaque appel, afin d'être certain d'obtenir les instances
mises à jour::

    // src/Acme/HelloBundle/Mail/Mailer.php
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
    votre objet pour un appel futur ; cela engendrerait les mêmes inconsistances
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
    nécessaire quand vous avez un service dans un champ d'application
    ``container`` qui a besoin d'un service du champ d'application ``request``.

Si vous définissez un contrôleur comme un service, alors vous pourrez appelez l'objet
``Request`` sans injecter le conteneur comme un argument de votre méthode action.
Voir :ref:`book-controller-request-argument` pour plus de détails.
