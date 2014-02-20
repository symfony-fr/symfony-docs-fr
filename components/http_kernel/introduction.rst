.. index::
   single: HTTP
   single: HttpKernel
   single: Components; HttpKernel

Le composant HttpKernel
=======================

    Le composant HttpKernel fournit un process structuré pour convertir
    une ``Request`` en ``Response`` en utilisant l'EventDispatcher.
    Il est suffisamment flexible pour créer un framework full-stack (Symfony),
    un micro-framework (Silex) ou un système de CMS avancé (Drupal).

Installation
------------

Vous pouvez installer le composant de 2 façons :

* :doc:`Via Composer </components/using_components>` (``symfony/http-kernel`` sur Packagist_);
* Via le dépôt Git officiel (https://github.com/symfony/HttpKernel).

Le Workflow d'une Requête
-------------------------

Chaque interaction HTTP démarre avec une requête et se termine par une réponse.
Votre travail en tant que développeur est de créer le code PHP qui lit les informations
de cette requête (c.-à-d. l'URL) et qui va créer puis renvoyer une réponse (c.-à-d. une page
HTML ou une chaîne JSON).

.. image:: /images/components/http_kernel/request-response-flow.png
   :align: center

Habituellement, certains frameworks ou systèmes sont conçus pour
gérer toutes les tâches répétitives (c.-à-d. routage, sécurité, etc.) afin qu'un
développeur puisse facilement construire chaque *page* de l'application.
La façon dont ces systèmes sont implémentés varie grandement. Le composant
HttpKernel fournit une interface qui formalise le processus de traitement de
requête et de création d'une réponse appropriée. Le composant est conçu pour
être le cœur de toute application ou framework, aussi variée l'architecture
de ce système soit-elle::

    namespace Symfony\Component\HttpKernel;

    use Symfony\Component\HttpFoundation\Request;

    interface HttpKernelInterface
    {
        // ...

        /**
         * @return Response A Response instance
         */
        public function handle(
            Request $request,
            $type = self::MASTER_REQUEST,
            $catch = true
        );
    }

En interne, :method:`HttpKernel::handle() <Symfony\\Component\\HttpKernel\\HttpKernel::handle>` (
l'implémentation concrète de :method:`HttpKernelInterface::handle() <Symfony\\Component\\HttpKernel\\HttpKernelInterface::handle>` )
définit un workflow qui commence par une :class:`Symfony\\Component\\HttpFoundation\\Request`
et se termine avec une :class:`Symfony\\Component\\HttpFoundation\\Response`.

.. image:: /images/components/http_kernel/01-workflow.png
   :align: center

Les détails exacts de ce workflow sont la clé pour comprendre comment le
kernel (et le Framework Symfony ou toute autre librairie utilisant le kernel)
fonctionne.

HttpKernel : Piloté par événements
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La méthode ``HttpKernel::handle()`` fonctionne en interne en déclenchant des
événements. Ceci rend la méthode flexible, mais aussi abstraite étant donné
que tout le "travail" d'un(e) framework/application construit(e) est réalisé
dans des écouteurs d'événements.

Pour aider dans les explications de ce processus, ce document traite de toutes
les étapes du processus et explique comment une implémentation spécifique de
HttpKernel (le Framework Symfony) fonctionne.

Pour commencer, utiliser la classe :class:`Symfony\\Component\\HttpKernel\\HttpKernel`
est très simple et nécessite la création d'un :doc:`EventDispatcher </components/event_dispatcher/introduction>`
et d'un :ref:`controller resolver <component-http-kernel-resolve-controller>`
(expliqué plus bas). Pour terminer votre kernel fonctionnel, vous devrez ajouter
plus d'écouteurs aux événements décrits ci-dessous::

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpKernel\HttpKernel;
    use Symfony\Component\EventDispatcher\EventDispatcher;
    use Symfony\Component\HttpKernel\Controller\ControllerResolver;

    // crée l'objet Request
    $request = Request::createFromGlobals();

    $dispatcher = new EventDispatcher();
    // ... on peut ajouter des écouteurs

    // crée le controller resolver
    $resolver = new ControllerResolver();
    // instancie le kernel
    $kernel = new HttpKernel($dispatcher, $resolver);

    // exécute le kernel, qui transforme la requête en réponse
    // en lançant des événements, appelant un contrôleur et en retournant la réponse
    $response = $kernel->handle($request);

    // envoie les headers et fait un echo du contenu
    $response->send();

    // déclenche l'événement kernel.terminate
    $kernel->terminate($request, $response);

Consultez ":ref:`http-kernel-working-example`" pour une implémentation plus concrète.

Pour des informations plus générales sur l'ajout d'écouteurs aux événement décrits ci-dessous,
consultez :ref:`http-kernel-creating-listener`.

.. tip::

    Fabien Potencier a aussi écrit une excellente série sur l'utilisation du
    composant HttpKernel et d'autres composants Symfony2 pour créer votre
    propre framework. Consultez `Create your own framework... on top of the Symfony2 Components`_.

.. _component-http-kernel-kernel-request:

1) L'événement ``kernel.request``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Usage typique** : Ajouter plus d'informations à la ``Request``, initialiser
des parties du système, ou encore retourner une ``Response`` si possible (par exemple une
couche de sécurité qui refuse l'accès).

:ref:`Table d'information des événements du kernel <component-http-kernel-event-table>`

Le premier événement déclenché dans :method:`HttpKernel::handle <Symfony\\Component\\HttpKernel\\HttpKernel::handle>`
est ``kernel.request``, qui peut avoir différentes sortes d'écouteurs.

.. image:: /images/components/http_kernel/02-kernel-request.png
   :align: center

Les écouteurs de ces événements peuvent être variés. Certains écouteurs (comme un écouteur de sécurité)
pourraient avoir suffisamment d'informations pour créer un objet ``Response`` immédiatement.
Par exemple, si une couche de sécurité a déterminé que l'utilisateur n'a pas l'accès à la page demandée,
cet écouteur peut retourner une :class:`Symfony\\Component\\HttpFoundation\\RedirectResponse`
vers la page de login ou une réponse 403 Accès refusé.

Si une ``Response`` est renvoyée à ce moment là, le processus avance directement
jusqu'à l'événement :ref:`kernel.response <component-http-kernel-kernel-response>`.

.. image:: /images/components/http_kernel/03-kernel-request-response.png
   :align: center

D'autres écouteurs initialisent simplement des valeurs ou ajoutent des informations
à la requête. Par exemple, un écouteur pourrait déterminer la locale à partir de l'objet ``Request``.

Un autre écouteur commun est le routage. Un écouteur de routage peut traiter
l'objet ``Request`` et déterminer le contrôleur qui devrait être exécuté (consultez la
section suivante). En fait, l'objet ``Request`` possède un attributes bag (un conteneur d'attributs) 
":ref:`attributes <component-foundation-attributes>`" qui est l'endroit idéal pour
stocker ces données additionnelles et spécifiques à l'application à propos de
la requête. Cela signifie que si votre écouteur de routing parvient à déterminer
le contrôleur, il peut le stocker sur les attributs de ``Request`` (qui peuvent ainsi
être utilisés par votre controller resolver).

En résumé, le rôle de l'événement ``kernel.request`` est soit de créer et renvoyer
une ``Response`` directement, soit d'ajouter de l'information à la ``Request``
(c.-à-d. récupérer la locale ou définir d'autres informations sur les attributs de ``Request``).

.. sidebar:: ``kernel.request`` dans le framework Symfony

    L'écouteur le plus important de ``kernel.request`` dans le framework Symfony
    est le :class:`Symfony\\Component\\HttpKernel\\EventListener\\RouterListener`.
    Cette classe exécute la couche de routing, qui renvoie un *tableau* d'informations
    à propos de la requête qui correspond, ceci inclut le ``_controller`` et tous les
    placeholders qui sont dans le pattern de la route. (c.-à-d. ``{slug}``). Consultez
    :doc:`Le Composant de Routage </components/routing/introduction>` pour plus d'informations.

    Ce tableau d'informations est stocké dans la propriété ``attributes`` de l'objet
    :class:`Symfony\\Component\\HttpFoundation\\Request`. Ajouter les informations de routage
    ici ne fait rien, mais elles sont ensuite exploitées par le controller resolver.

.. _component-http-kernel-resolve-controller:

2) Résolution du Contrôleur
~~~~~~~~~~~~~~~~~~~~~~~~~~~

En partant du principe qu'aucun écouteur de ``kernel.request`` n'a été capable
de créer une ``Response``, l'étape suivante dans HttpKernel est de déterminer
et préparer (c.-à-d. résoudre) le contrôleur. Le contrôleur est une partie du code
de l'application finale qui est responsable de la création et du renvoi de la
``Response`` pour une page spécifique. Le seul prérequis est qu'il soit un callable
PHP (c.-à-d. une fonction, une méthode, un objet implémentant la méthode ``__invoke`` 
ou une closure).

Mais *la manière* dont le contrôleur exact est déterminé pour une requête dépend complètement
de votre application. C'est le travail du "controller resolver" (une classe qui implémente
:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface` et qui est un
des arguments du contructeur de ``HttpKernel``).

.. image:: /images/components/http_kernel/04-resolve-controller.png
   :align: center

Votre travail est de créer une classe qui implémente l'interface et déclare ces deux
méthodes : ``getController`` et ``getArguments``. En fait, une implémentation par défaut
existe déjà, et vous pouvez l'utiliser directement ou l'étudier:
:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver`.
Cette implémentation est expliquée dans l'encart un peu plus bas::

    namespace Symfony\Component\HttpKernel\Controller;

    use Symfony\Component\HttpFoundation\Request;

    interface ControllerResolverInterface
    {
        public function getController(Request $request);

        public function getArguments(Request $request, $controller);
    }

En interne, la méthode ``HttpKernel::handle`` appelle en premier lieu
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getController`
sur le controller resolver. Cette méthode reçoit l'objet ``Request`` et doit déterminer
puis retourner un callable PHP (le contrôleur) basé sur les informations de la requête.

La seconde méthode, :method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getArguments`,
sera appelée après qu'un autre événement (``kernel.controller``) est déclenché.

.. sidebar:: Résolution du contrôleur dans le Framework Symfony

    Le Framework Symfony utilise la classe built-in
    :class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver`
    (en vérité, il utilise une sous-classe avec quelques fonctionnalités additionnelles
    mentionnées ci-dessous). Cette classe s'appuie sur les informations qui ont été
    placées dans la propriété ``attributes`` de l'objet ``Request`` pendant l'exécution
    de ``RouterListener``.

    **getController**

    Le ``ControllerResolver`` cherche une clé ``_controller``
    sur la propriété ``attributes`` de l'objet ``Request`` (souvenez-vous, cette information
    est typiquement placée dans ``Request`` via l'écouteur ``RouterListener``).
    Cette chaîne de caractères est ensuite convertie en callable PHP comme suit:

    a) Le format ``AcmeDemoBundle:Default:index`` de ``_controller``
    est changé en une autre chaîne contenant le nom complet de la classe et
    le nom complet de la méthode en suivant les conventions en vigueur dans Symfony2
    (c.-à-d. ``Acme\DemoBundle\Controller\DefaultController::indexAction``). Cette transformation
    est spécifique à la sous-classe :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\ControllerResolver`
    utilisée par le Framework Symfony2.

    b) Une nouvelle instance du contrôleur est instanciée sans arguments.

    c) Si le contrôleur implémente :class:`Symfony\\Component\\DependencyInjection\\ContainerAwareInterface`,
    la méthode ``setContainer`` est appelée sur l'objet contrôleur et le container lui est passé en argument. 
    Cette étape aussi est spécifique à la sous-classe :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\ControllerResolver`
    utilisée par le Framework Symfony2.

    Il y a aussi quelques petites variations sur le processus décrit plus haut
     (c.-à-d. si vous déclarez vos contrôleurs en tant que services).

.. _component-http-kernel-kernel-controller:

3) L'événement ``kernel.controller``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Usage typique** : Initialiser des services ou changer le contrôleur juste avant
son exécution.

:ref:`Table d'information des événements du kernel <component-http-kernel-event-table>`

Après que le contrôleur callable a été déterminé, ``HttpKernel::handle``
déclenche l'événement ``kernel.controller``. Les écouteurs de cet événement
pourraient initialiser certaines parties du systèmes ayant besoin que d'autres
choses aient été déterminées (c.-à-d. le contrôleur, les informations de route),
mais avant que le contrôleur ne soit exécuté. Pour voir quelques exemples,
regardez la section Symfony2 plus bas.

.. image:: /images/components/http_kernel/06-kernel-controller.png
   :align: center

Les écouteurs de cet événement peuvent aussi changer complètement le contrôleur
callable en appelant :method:`FilterControllerEvent::setController <Symfony\\Component\\HttpKernel\\Event\\FilterControllerEvent::setController>`
sur l'objet événement qui est passé à l'écouteur.

.. sidebar:: ``kernel.controller`` dans le Framework Symfony

    Il y a quelques écouteurs mineurs sur l'événement ``kernel.controller`` dans
    le framework Symfony, et beaucoup portent sur la collection de données pour le
    profiler quand celui ci est activé.

    Un écouteur intéressant provient de :doc:`SensioFrameworkExtraBundle </bundles/SensioFrameworkExtraBundle/index>`,
    qui est inclu dans la Symfony Standard Edition. La fonctionnalité
    :doc:`@ParamConverter </bundles/SensioFrameworkExtraBundle/annotations/converters>`
    de cet écouteur vous permet de passer un objet complet (c.-à-d. un objet ``Post``) à
    votre contrôleur à la place d'un scalaire (c.-à-d. un paramètre ``id`` déclaré
    dans votre route). Cet écouteur (``ParamConverterListener``) utilise la
    reflexion pour examiner chaque argument du contrôleur et essaie d'utiliser
    différentes méthodes pour convertir ces derniers en objets, qui sont ensuite
    stockés dans la propriété ``attributes`` de l'objet ``Request``. Consultez
    la section suivante pour savoir en quoi c'est important.

4) Récupérer les arguments du contrôleur
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ensuite, ``HttpKernel::handle`` appelle
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getArguments`.
Rappelez vous que le contrôleur renvoyé par ``getController`` est un callable PHP.
Le rôle de ``getArguments`` est de renvoyer un tableau d'arguments qui devraient être
passés au contrôleur. Libre à vous de décider comment ceci est réalisé, cependant la classe built-in
:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver` est un bon exemple.

.. image:: /images/components/http_kernel/07-controller-arguments.png
   :align: center

A ce moment là, le kernel dispose d'un callable PHP (le contrôleur) et d'un tableau d'arguments
qui devraient être passés au callable lors de son exécution.

.. sidebar:: Récupération des arguments du contrôleur dans le Framework Symfony2

    Maintenant que vous savez exactement ce qu'est un contrôleur callable (habituellement
    une méthode dans un objet contrôleur), le ``ControllerResolver`` utilise `reflection`_
    sur le callable pour retourner un tableau des noms (names) de chaque argument.
    Il parcourt ensuite ces arguments et utilise l'astuce suivante pour déterminer
    quelle valeur devrait être passée à chaque argument:

    a) Si l'attributes bag (conteneur d'attributs) de la ``Request`` contient une clé
    qui correspond au nom de l'argument, cette valeur est utilisée. Par exemple,
    lorsque le premier argument d'un contrôleur est ``$slug`` et qu'il y a une clé
    ``slug`` dans l'attributes bag de ``Request``, cette valeur est utilisée (et 
    provient en général du routing via ``RouterListener``).

    b) Si l'argument du contrôleur est typé comme étant un objet
    :class:`Symfony\\Component\\HttpFoundation\\Request`, alors ``Request``
    est passée en valeur.

.. _component-http-kernel-calling-controller:

5) Appeler le contrôleur
~~~~~~~~~~~~~~~~~~~~~~~~

L'étape suivante est simple! ``HttpKernel::handle`` exécute le contrôleur.

.. image:: /images/components/http_kernel/08-call-controller.png
   :align: center

Le rôle du contrôleur est de construire la réponse pour une ressource donnée.
Cela pourrait être une page HTML, une chaîne JSON ou n'importe quoi. A l'inverse
de toutes les autres étapes du processus jusqu'ici, cette étape est implémentée par
le "développeur final", pour chaque page qui est construite.

Habituellement, le contrôleur retourne un objet ``Response``. Si c'est le cas, alors
le travail du kernel est presque fini! Dans ce cas, l'étape suivante est l'événement
:ref:`kernel.response <component-http-kernel-kernel-response>`.

.. image:: /images/components/http_kernel/09-controller-returns-response.png
   :align: center

En revanche, si le contrôleur retourne autre chose qu'une ``Response``, alors le kernel
a un peu plus de travail à faire - :ref:`kernel.view <component-http-kernel-kernel-view>`
(puisque le but est de *toujours* générer un objet ``Response``).

.. note::

    Un contrôleur doit renvoyer *quelque chose*. Si un contrôleur retourne ``null``,
    une exception est levée immédiatement.

.. _component-http-kernel-kernel-view:

6) L'événement ``kernel.view``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Usage typique** : Transformer une valeur retournée par un contrôleur autre que
``Response`` en ``Response``

:ref:`Table d'information des événements du kernel <component-http-kernel-event-table>`

Si le contrôleur ne retourne pas un objet ``Response``, alors le kernel déclenche
un autre événement (``kernel.view``). Le rôle d'un écouteur de cet événement
est d'utiliser la valeur de retour du contrôleur (c.-à-d. un tableau ou un objet)
et de créer une ``Response``.

.. image:: /images/components/http_kernel/10-kernel-view.png
   :align: center

Cela peut être utile si vous souhaitez utiliser une couche "vue" : au lieu
de retourner une ``Response`` depuis le contrôleur, on renvoie des données
qui représentent la page. Un écouteur de cet événement pourrait alors exploiter
ces données pour créer une ``Response`` au bon format (c.-à-d. HTML, JSON, etc.).

A ce moment, si aucun écouteur ne définit une réponse dans l'événement, alors
une exception est levée : soit le contrôleur soit au moins un des écouteurs doivent
*toujours* retourner une ``Response``.

.. sidebar:: ``kernel.view`` dans le Framework Symfony

    Il n'y a aucun écouteur par défaut dans le Framework Symfony pour l'événement
    ``kernel.view``. Toutefois, un core bundle
    (:doc:`SensioFrameworkExtraBundle </bundles/SensioFrameworkExtraBundle/index>`)
    ajoute un écouteur à cet événement. Si votre contrôleur renvoie un tableau, et que
    vous placez l'annotation :doc:`@Template </bundles/SensioFrameworkExtraBundle/annotations/view>`
    au dessus du contrôleur, alors cet écouteur effectue le rendu d'un template, passe
    le tableau retourné par votre contrôleur à ce template et crée une ``Response``
    contenant le contenu de ce template.

    De plus, un bundle populaire de la communauté `FOSRestBundle`_ implémente un écouteur
    sur cet événement et qui fournit une couche de vue robuste capable d'utiliser un seul
    contrôleur pour retourner plusieurs type différents de réponses (c.-à-d. HTML, JSON, XML, etc.).

.. _component-http-kernel-kernel-response:

7) L'événement ``kernel.response``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Usage typique** : Modifier l'objet ``Response`` juste avant son envoi.

:ref:`Table d'information des événements du kernel <component-http-kernel-event-table>`

L'objectif du kernel est de transformer une ``Request`` en ``Response``. La réponse
pourrait être créée pendant l'événement :ref:`kernel.request <component-http-kernel-kernel-request>`,
retournée depuis le :ref:`controller <component-http-kernel-calling-controller>`,
ou retournée par un des écouteurs de l'événement :ref:`kernel.view <component-http-kernel-kernel-view>`.

Quel que soit le créateur de la ``Response``, un autre événement (``kernel.response``)
est déclenché juste après. Un écouteur typique de cet événement va modifier l'objet
``Response``, par exemple en modifiant un header, en ajoutant un cookie, voir
même en changeant le contenu de la ``Response`` elle-même (par exemple en injectant du
JavaScript avant la fin du tag ``</body>`` d'une réponse HTML).

Après le déclenchement de cet événement, l'objet ``Response`` dans sa forme finale
est retourné par :method:`Symfony\\Component\\HttpKernel\\HttpKernel::handle`. Dans
le cas d'usage le plus commun, vous pouvez directement appeler la méthode
:method:`Symfony\\Component\\HttpFoundation\\Response::send`, qui envoie les headers
et affiche le contenu de ``Response``.

.. sidebar:: ``kernel.response`` dans le Framework Symfony

    Il y a plusieurs écouteurs mineurs sur cet événement dans le Framework Symfony,
    et la plupart modifient la réponse. Par exemple, l'écouteur
    :class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener`
    injecte du JavaScript à la fin de votre page dans l'environnement ``dev`` qui
    permet l'affichage de la barre de débogage. Un autre écouteur,
    :class:`Symfony\\Component\\Security\\Http\\Firewall\\ContextListener`,
    sérialise les informations de l'utilisateur en cours dans la session afin
    qu'elles puissent être rechargées lors de la requête suivante.

.. _component-http-kernel-kernel-terminate:

8) L'événement ``kernel.terminate``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Usage typique** : Effectuer des actions "longues" après que la réponse a été
envoyée à l'utilisateur.

:ref:`Table d'informations des événements du kernel <component-http-kernel-event-table>`

L'événement final du processus HttpKernel est ``kernel.terminate`` et est unique
car étant déclenché *après* l'appel à ``HttpKernel::handle``, et après que la réponse a
été envoyée à l'utilisateur. Souvenez vous, plus haut, le code utilisant le kernel se
termine comme ceci::

    // envoie les headers et affiche le contenu de la réponse
    $response->send();

    // déclenche l'événement kernel.terminate
    $kernel->terminate($request, $response);

Comme vous pouvez le voir, en appelant ``$kernel->terminate`` après avoir envoyé
la réponse, vous déclencherez l'événement ``kernel.terminate`` où vous pourrez effectuer
certaines actions qui ont été différées en vue de renvoyer une réponse aussi vite que
possible au client (comme envoyer des emails).

.. note::
    Utiliser ``kernel.terminate`` est optionnel, et vous ne devriez l'appeler que si
    le kernel implémente :class:`Symfony\\Component\\HttpKernel\\TerminableInterface`.

.. sidebar:: ``kernel.terminate`` dans le Framework Symfony

    Si vous utilisez SwiftmailerBundle avec Symfony2 avec le ``memory`` spooling, alors
    le :class:`Symfony\\Bundle\\SwiftmailerBundle\\EventListener\\EmailSenderListener`
    est activé. Ce dernier envoie tous les emails planifiés pour envoi pendant le 
    traitement de la requête.

.. _component-http-kernel-kernel-exception:

Gérer les exceptions : l'événement ``kernel.exception``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Usage typique** : Gérer certains types d'exceptions et créer une ``Response`` pour
ces exceptions.

:ref:`Table d'information des événements du kernel<component-http-kernel-event-table>`

Si une exception est levée à n'importe quel instant dans ``HttpKernel::handle``, un autre
événement (``kernel.exception``) est déclenché. En interne, le corps de la méthode ``handle``
est encapsulé dans un bloc try-catch. Quand une exception est levée, l'événement ``kernel.exception``
est déclenché pour que votre application puisse réagir à cette exception.

.. image:: /images/components/http_kernel/11-kernel-exception.png
   :align: center

Tout écouteur de cet événement reçoit un objet :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`
qui vous permet d'accéder à l'exception d'origine via la méthode
:method:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent::getException`
Un écouteur typique sur cet événement va traiter certains types d'exceptions et créer
une ``Response`` d'erreur appropriée.

Par exemple, pour générer une page 404, vous pourriez lever un type spécial d'exception
et ensuite ajouter un écouteur sur cet événement qui va l'intercepter et créer
puis retourner une ``Response`` 404. En fait, le composant HttpKernel est fourni avec
un :class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener`. Si vous
choisissez d'utiliser ce dernier, il gérera une grande variété d'exceptions par défaut
(voir l'encart plus bas pour plus de détails).

.. sidebar:: ``kernel.exception`` dans le Framework Symfony

    Il y a deux écouteurs principaux pour ``kernel.exception`` lorsque vous utilisez
    le framework Symfony.

    **ExceptionListener dans HttpKernel**

    Le premier est livré avec le composant HttpKernel et est appelé
    :class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener`.
    Cet écouteur a plusieurs rôles:

    1) L'exception levée est convertie en un objet
    :class:`Symfony\\Component\\HttpKernel\\Exception\\FlattenException`,
    qui contient toutes les informations à propos de la requête, et qui peut
    être affiché ou sérialisée.

    2) Si l'exception d'origine implémente
    :class:`Symfony\\Component\\HttpKernel\\Exception\\HttpExceptionInterface`,
    alors ``getStatusCode`` et ``getHeaders`` sont appelées pour alimenter le header
    et le code de statut de l'objet ``FlattenException``. Ces valeurs
    sont utilisées pour créer la réponse finale.

    3) Un contrôleur est exécuté et reçoit la ``FlattenException``. Le contrôleur
    exact à utiliser est passé comme argument du constructeur de l'écouteur. C'est
    ce contrôleur qui renverra la ``Response`` finale pour cette page d'erreur.

    **ExceptionListener dans Security**

    L'autre écouteur important est le :class:`Symfony\\Component\\Security\\Http\\Firewall\\ExceptionListener`.
    Le but de cet écouteur est de gérer les exceptions de sécurité, et, quand c'est
    approprié, d'*aider* l'utilisateur à s'authentifier (c.-à-d. de le rediriger sur la
    page de connexion).

.. _http-kernel-creating-listener:

Créer un écouteur d'événements
------------------------------

Comme vous avez pu le constater, vous pouvez attacher des écouteurs d'événements à tous
les événements déclenchés durant le cycle d'exécution de ``HttpKernel::handle``.
Typiquement, un écouteur est une classe PHP dont une méthode est exécutée, voir
:doc:`/components/event_dispatcher/introduction`. 

Le nom de chaque événement du kernel est défini en tant que constante dans la classe 
:class:`Symfony\\Component\\HttpKernel\\KernelEvents`. De plus, chaque écouteur 
d'événement reçoit un argument unique, qui est une classe fille de
:class:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent`. Cet objet contient
les informations à propos de l'état actuel du système et chaque type d'événement
possède ses propres objets.

.. _component-http-kernel-event-table:

+-------------------+--------------------------------+-------------------------------------------------------------------------------------+
| **Nom**           | **Constante** ``KernelEvents`` | **Argument passé à l'écouteur**                                                     |
+-------------------+--------------------------------+-------------------------------------------------------------------------------------+
| kernel.request    | ``KernelEvents::REQUEST``      | :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseEvent`                    |
+-------------------+--------------------------------+-------------------------------------------------------------------------------------+
| kernel.controller | ``KernelEvents::CONTROLLER``   | :class:`Symfony\\Component\\HttpKernel\\Event\\FilterControllerEvent`               |
+-------------------+--------------------------------+-------------------------------------------------------------------------------------+
| kernel.view       | ``KernelEvents::VIEW``         | :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForControllerResultEvent` |
+-------------------+--------------------------------+-------------------------------------------------------------------------------------+
| kernel.response   | ``KernelEvents::RESPONSE``     | :class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`                 |
+-------------------+--------------------------------+-------------------------------------------------------------------------------------+
| kernel.terminate  | ``KernelEvents::TERMINATE``    | :class:`Symfony\\Component\\HttpKernel\\Event\\PostResponseEvent`                   |
+-------------------+--------------------------------+-------------------------------------------------------------------------------------+
| kernel.exception  | ``KernelEvents::EXCEPTION``    | :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`        |
+-------------------+--------------------------------+-------------------------------------------------------------------------------------+

.. _http-kernel-working-example:

Un exemple fonctionnel complet
------------------------------

Lors de l'utilisation du composant HttpKernel, vous êtes libre d'attacher tous les
écouteurs que vous souhaitez aux événements du kernel et d'utiliser tout controller
resolver qui implémente :class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface`.
Toutefois, le composant HttpKernel est fourni avec des écouteurs built-in ainsi qu'un
ControllerResolver qui peuvent être utilisés pour un exemple fonctionnel::

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;
    use Symfony\Component\HttpKernel\HttpKernel;
    use Symfony\Component\EventDispatcher\EventDispatcher;
    use Symfony\Component\HttpKernel\Controller\ControllerResolver;
    use Symfony\Component\Routing\RouteCollection;
    use Symfony\Component\Routing\Route;
    use Symfony\Component\Routing\Matcher\UrlMatcher;
    use Symfony\Component\Routing\RequestContext;

    $routes = new RouteCollection();
    $routes->add('hello', new Route('/hello/{name}', array(
            '_controller' => function (Request $request) {
                return new Response(sprintf("Hello %s", $request->get('name')));
            }
        )
    ));

    $request = Request::createFromGlobals();

    $matcher = new UrlMatcher($routes, new RequestContext());

    $dispatcher = new EventDispatcher();
    $dispatcher->addSubscriber(new RouterListener($matcher));

    $resolver = new ControllerResolver();
    $kernel = new HttpKernel($dispatcher, $resolver);

    $response = $kernel->handle($request);
    $response->send();

    $kernel->terminate($request, $response);

.. _http-kernel-sub-requests:

Sous-requêtes
-------------

En plus de la requête "principale" ("main") qui est envoyée dans ``HttpKernel::handle``,
vous pouvez aussi envoyer une "sous-requête". Une sous-requête parait et agit de la
même façon que n'importe quelle autre requête, mais sert typiquement à gérer le rendu
de petites portions de la page au lieu d'une page complète. Vous utiliserez
fréquemment les sous requêtes depuis votre contrôleur (ou peut être depuis un template,
qui sera rendu par votre contrôleur).

.. image:: /images/components/http_kernel/sub-request.png
   :align: center

Pour exécuter une sous-requête, utilisez ``HttpKernel::handle``, mais changez les arguments
comme suit::

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpKernel\HttpKernelInterface;

    // ...

    // create some other request manually as needed
    $request = new Request();
    // for example, possibly set its _controller manually
    $request->attributes->add('_controller', '...');

    $response = $kernel->handle($request, HttpKernelInterface::SUB_REQUEST);
    // do something with this response

Ceci crée un autre cycle complet requête-réponse où cette nouvelle ``Request`` est
transformée en ``Response``. La seule différence en interne est que certains écouteurs,
(par exemple ceux de la sécurité) peuvent uniquement agir sur la requête principale.
Chaque écouteur reçoit une classe fille de :class:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent`,
dont :method:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent::getRequestType`
peut être utilisé pour définir si l'écouteur traite les requêtes principales ou les
sous-requêtes.

Par exemple, un écouteur qui ne traitera qu'une requête principale ressemble à ceci::

    use Symfony\Component\HttpKernel\HttpKernelInterface;
    // ...

    public function onKernelRequest(GetResponseEvent $event)
    {
        if (HttpKernelInterface::MASTER_REQUEST !== $event->getRequestType()) {
            return;
        }

        // ...
    }

.. _Packagist: https://packagist.org/packages/symfony/http-kernel
.. _reflection: http://php.net/manual/en/book.reflection.php
.. _FOSRestBundle: https://github.com/friendsofsymfony/FOSRestBundle
.. _`Create your own framework... on top of the Symfony2 Components`: http://fabien.potencier.org/article/50/create-your-own-framework-on-top-of-the-symfony2-components-part-1
