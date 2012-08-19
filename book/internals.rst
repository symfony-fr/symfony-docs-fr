.. index::
   single: Composants Internes

Composants Internes
===================

Il paraît que vous voulez comprendre comment Symfony2 fonctionne et comment
l'étendre. Cela me rend très heureux ! Cette section est une explication en
profondeur des composants internes de Symfony2.

.. note::

    Vous avez besoin de lire cette section uniquement si vous souhaitez comprendre
    comment Symfony2 fonctionne en arrière-plan, ou si vous voulez étendre Symfony2.

Vue Globale
-----------

Le code de Symfony2 se compose de plusieurs couches indépendantes. Chacune
d'entre elles est construite par dessus celles qui la précèdent.

.. tip::

    L'autoloading (« chargement automatique ») n'est pas géré par le
    framework directement ; ceci est effectué indépendemment à l'aide de
    la classe :class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader`
    et du fichier ``src/autoload.php``. Lisez le :doc:`chapitre dédié
    </components/class_loader>` pour plus d'informations.

Le Composant ``HttpFoundation``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le composant de plus bas niveau est :namespace:`Symfony\\Component\\HttpFoundation`.
HttpFoundation fournit les objets principaux nécessaires pour traiter avec HTTP.
C'est une abstraction orientée objet de quelques fonctions et variables PHP
natives :

* La classe :class:`Symfony\\Component\\HttpFoundation\\Request` abstrait
  les principales variables globales PHP que sont ``$_GET``, ``$_POST``, ``$_COOKIE``,
  ``$_FILES``, et ``$_SERVER`` ;

* Le classe :class:`Symfony\\Component\\HttpFoundation\\Response` abstrait quelques
  fonctions PHP comme ``header()``, ``setcookie()``, et ``echo`` ;

* La classe :class:`Symfony\\Component\\HttpFoundation\\Session` et l'interface
  :class:`Symfony\\Component\\HttpFoundation\\SessionStorage\\SessionStorageInterface`
  font abstraction des fonctions de gestion des sessions ``session_*()``.

Le Composant ``HttpKernel``
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Par-dessus HttpFoundation se trouve le composant :namespace:`Symfony\\Component\\HttpKernel`.
HttpKernel gère la partie dynamique de HTTP ; c'est une fine surcouche au-dessus
des classes Request et Response pour standardiser la façon dont les requêtes
sont gérées. Il fournit aussi des points d'extension et des outils qui en font
un point de démarrage idéal pour créer un framework Web sans trop d'efforts.

Optionnellement, il ajoute de la configurabilité et de l'extensibilité, grâce
au composant Dependency Injection et à un puissant système de plugin (bundles).

.. seealso::

    Apprenez en plus en lisant les chapitres :doc:`Dependency Injection </book/service_container>`
    et :doc:`Bundles </cookbook/bundles/best_practices>`.

Le Bundle ``FrameworkBundle``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le bundle :namespace:`Symfony\\Bundle\\FrameworkBundle` est le bundle qui lie
les principaux composants et bibliothèques ensembles afin de fournir un framework
MVC léger et rapide. Il est fourni avec une configuration par défaut ainsi
qu'avec des conventions afin d'en faciliter l'apprentissage.

.. index::
   single: Internals; Kernel

Le Kernel
---------

La classe :class:`Symfony\\Component\\HttpKernel\\HttpKernel` est la classe
centrale de Symfony2 et est responsable de la gestion des requêtes clientes.
Son but principal est de « convertir » un objet
:class:`Symfony\\Component\\HttpFoundation\\Request` en un objet
:class:`Symfony\\Component\\HttpFoundation\\Response`.

Chaque Kernel Symfony2 implémente
:class:`Symfony\\Component\\HttpKernel\\HttpKernelInterface` ::

    function handle(Request $request, $type = self::MASTER_REQUEST, $catch = true)

.. index::
   single: Composants Internes; Résolution du contrôleur

Les Contrôleurs
~~~~~~~~~~~~~~~

Pour convertir une Requête en une Réponse, le Kernel repose sur un « Contrôleur ».
Un Contrôleur peut être n'importe quel « callable » (code qui peut être appelé) PHP.

Le Kernel délègue la sélection de quel Contrôleur devrait être exécuté à une
implémentation de
:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface` ::

    public function getController(Request $request);

    public function getArguments(Request $request, $controller);

La méthode
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getController`
retourne le Contrôleur (un « callable » PHP) associé à la Requête donnée. L'implémentation par
défaut (:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver`) recherche un
attribut de la requête ``_controller`` qui représente le nom du contrôleur (une chaîne de
caractères « classe::méthode », comme ``Bundle\BlogBundle\PostController:indexAction``).

.. tip::
    L'implémentation par défaut utilise le
    :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener` pour définir
    l'attribut de la Requête ``_controller`` (voir :ref:`kernel-core-request`).

La méthode
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getArguments`
retourne un tableau d'arguments à passer au Contrôleur. L'implémentation par défaut résoud
automatiquement les arguments de la méthode, basé sur les attributs de la Requête.

.. sidebar:: Faire correspondre les arguments de la méthode du Contrôleur aux attributs de la Requête

    Pour chaque argument d'une méthode, Symfony2 essaye d'obtenir la valeur d'un attribut
    d'une Requête avec le même nom. S'il n'est pas défini, la valeur par défaut de l'argument
    est utilisée si elle est définie ::

        // Symfony2 va rechercher un attribut « id » (obligatoire)
        // et un nommé « admin » (optionnel)
        public function showAction($id, $admin = true)
        {
            // ...
        }

.. index::
  single: Composants Internes; Gestion de la requête

Gestion des Requêtes
~~~~~~~~~~~~~~~~~~~~

La méthode ``handle()`` prend une ``Requête`` et retourne *toujours* une ``Réponse``.
Pour convertir la ``Requête``, ``handle()`` repose sur le « Resolver » et sur une
chaîne ordonnée de notifications d'évènements (voir la prochaine section pour plus
d'informations à propos de chaque évènement) :

1. Avant de faire quoi que ce soit d'autre, l'évènement ``kernel.request`` est
   notifié -- si l'un des listeners (« écouteurs » en français) retourne une
   ``Réponse``, il saute directement à l'étape 8 ;

2. Le « Resolver » est appelé pour déterminer le Contrôleur à exécuter ;

3. Les listeners de l'évènement ``kernel.controller`` peuvent maintenant
   manipuler le « callable » Contrôleur de la manière dont ils souhaitent
   (le changer, créer un « wrapper » au-dessus de lui, ...) ;

4. Le Kernel vérifie que le Contrôleur est un « callable » PHP valide ;

5. Le « Resolver » est appelé pour déterminer les arguments à passer au Contrôleur ;

6. Le Kernel appelle le Contrôleur ;

7. Si le Contrôleur ne retourne pas une ``Réponse``, les listeners de l'évènement
   ``kernel.view`` peuvent convertir la valeur retournée par le Contrôleur en une ``Réponse`` ;

8. Les listeners de l'évènement ``kernel.response`` peuvent manipuler la ``Réponse``
   (contenu et en-têtes) ;

9. La Réponse est retournée.

Si une Exception est capturée pendant le traitement de la Requête, l'évènement
``kernel.exception`` est notifié et les listeners ont alors une chance de
convertir l'Exception en une Réponse. Si cela fonctionne, l'évènement
``kernel.response`` sera notifié ; si non, l'Exception sera re-jetée.

Si vous ne voulez pas que les Exceptions soient capturées (pour des requêtes imbriquées
par exemple), désactivez l'évènement ``kernel.exception`` en passant ``false`` en tant
que troisième argument de la méthode ``handle()``.

.. index::
  single: Composants Internes; Requêtes internes

Requêtes Internes
~~~~~~~~~~~~~~~~~

A tout moment durant la gestion de la requête (la « master »), une sous-requête
peut être gérée. Vous pouvez passer le type de requête à la méthode ``handle()``
(son second argument) :

* ``HttpKernelInterface::MASTER_REQUEST``;
* ``HttpKernelInterface::SUB_REQUEST``.

Le type est passé à tous les évènements et les listeners peuvent ainsi agir
en conséquence (le traitement doit seulement intervenir sur la requête
« master »).

.. index::
   pair: Kernel; Evènement

Les Evènements
~~~~~~~~~~~~~~

Chaque évènement capturé par le Kernel est une sous-classe de
:class:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent`. Cela signifie que
chaque évènement a accès aux mêmes informations de base :

* ``getRequestType()`` - retourne le *type* de la requête
  (``HttpKernelInterface::MASTER_REQUEST`` ou ``HttpKernelInterface::SUB_REQUEST``) ;

* ``getKernel()`` - retourne le Kernel gérant la requête ;

* ``getRequest()`` - retourne la ``Requête`` courante qui est en train d'être gérée.

``getRequestType()``
....................

La méthode ``getRequestType()`` permet aux listeners de connaître le type
de la requête. Par exemple, si un listener doit seulement être activé pour les
requêtes « master », ajoutez le code suivant au début de votre méthode listener ::

    use Symfony\Component\HttpKernel\HttpKernelInterface;

    if (HttpKernelInterface::MASTER_REQUEST !== $event->getRequestType()) {
        // retourne immédiatement
        return;
    }

.. tip::

    Si vous n'êtes pas encore familier avec le « Dispatcher d'Evènements » de
    Symfony2, lisez d'abord la section  :doc:`Documentation du composant Event Dispatcher</components/event_dispatcher/introduction>`.

.. index::
   single: Evènement; kernel.request

.. _kernel-core-request:

L'Evènement ``kernel.request``
..............................

*La Classe Evènement* : :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseEvent`

Le but de cet évènement est soit de retourner un objet ``Response`` immédiatement, soit
de définir des variables afin qu'un Contrôleur puisse être appelé après l'évènement.
Tout listener peut retourner un objet ``Response`` via la méthode ``setResponse()``
sur l'évènement. Dans ce cas, tous les autres listeners ne seront pas appelés.

Cet évènement est utilisé par le ``FrameworkBundle`` afin de remplir l'attribut de la
``Requête`` ``_controller``, via
:class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener`. RequestListener
utilise un objet :class:`Symfony\\Component\\Routing\\RouterInterface` pour faire correspondre
la ``Requête`` et déterminer le nom du Contrôleur (stocké dans l'attribut de la
``Requête`` ``_controller``).

.. index::
   single: Evènement; kernel.controller

L'évènement ``kernel.controller``
.................................

*La Classe Evènement*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterControllerEvent`

Cet évènement n'est pas utilisé par le ``FrameworkBundle``, mais peut être un point
d'entrée utilisé pour modifier le contrôleur qui devrait être exécuté :

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\FilterControllerEvent;

    public function onKernelController(FilterControllerEvent $event)
    {
        $controller = $event->getController();
        // ...

        // le contrôleur peut être remplacé par n'importe quel « callable » PHP
        $event->setController($controller);
    }

.. index::
   single: Evènement; kernel.view

L'évènement ``kernel.view``
...........................

*La Classe Evènement*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForControllerResultEvent`

Cet évènement n'est pas utilisé par le ``FrameworkBundle``, mais il peut être utilisé
pour implémenter un sous-système de vues. Cet évènement est appelé *seulement* si le
Contrôleur *ne* retourne *pas* un objet ``Response``. Le but de cet évènement est
de permettre à d'autres valeurs retournées d'être converties en une ``Réponse``.

La valeur retournée par le Contrôleur est accessible via la méthode ``getControllerResult`` ::

    use Symfony\Component\HttpKernel\Event\GetResponseForControllerResultEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelView(GetResponseForControllerResultEvent $event)
    {
        $val = $event->getReturnValue();
        $response = new Response();
        // personnalisez d'une manière ou d'une autre la Réponse
        // en vous basant sur la valeur retournée

        $event->setResponse($response);
    }

.. index::
   single: Evènement; kernel.response

L'évènement ``kernel.response``
...............................

*La Classe Evènement*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`

L'objectif de cet évènement est de permettre à d'autres systèmes de modifier ou
de remplacer l'objet ``Response`` après sa création :

.. code-block:: php

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $response = $event->getResponse();
        // .. modifiez l'objet Response
    }

Le ``FrameworkBundle`` enregistre plusieurs listeners :

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`:
  collecte les données pour la requête courante ;

* :class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener`:
  injecte la Barre d'Outils de Débuggage Web (« Web Debug Toolbar ») ;

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\ResponseListener`: définit la
  valeur du ``Content-Type`` de la Réponse basée sur le format de la requête ;

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\EsiListener`: ajoute un
  en-tête HTTP ``Surrogate-Control`` lorsque la Réponse a besoin d'être analysée
  pour trouver des balises ESI.

.. index::
   single: Evènement; kernel.exception

.. _kernel-kernel.exception:

L'évènement ``kernel.exception``
................................

*La Classe Evènement*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`

Le ``FrameworkBundle`` enregistre un
:class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener` qui
transmet la ``Requête`` à un Contrôleur donné (la valeur du paramètre
``exception_listener.controller`` -- doit être exprimé suivant la notation
``class::method``).

Un listener sur cet évènement peut créer et définir un objet ``Response``,
créer et définir un nouvel objet ``Exception``, ou ne rien faire :

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\GetResponseForExceptionEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelException(GetResponseForExceptionEvent $event)
    {
        $exception = $event->getException();
        $response = new Response();
        // définissez l'objet Response basé sur l'exception capturée
        $event->setResponse($response);

        // vous pouvez alternativement définir une nouvelle Exception
        // $exception = new \Exception('Some special exception');
        // $event->setException($exception);
    }

.. note::

    Comme Symfony vérifie que le code de statut de la Réponse est le plus
    approprié selon l'exception, définir un code de statut sur la réponse
    ne fonctionnera pas. Si vous voulez réécrire le code de statut (ce que
    vous ne devriez pas faire sans une excellente raison), définissez l'entête
    ``X-Status-Code``::

        return new Response('Error', 404 /* ignoré */, array('X-Status-Code' => 200));

.. index::
   single: Dispatcher d'Evènements

Le Dispatcher d'Evènements
--------------------------

Le dispatcher d'évènements est un composant autonome qui est responsable
d'une bonne partie de la logique sous-jacente et du flux d'une requête Symfony.
Pour plus d'informations, lisez la :doc:`documentation du composant Event Dispatcher</components/event_dispatcher/introduction>`.

.. index::
   single: Profiler

.. _internals-profiler:

Profiler
--------

Lorsqu'il est activé, le profiler de Symfony2 collecte des informations
utiles concernant chaque requête envoyée à votre application et les stocke
pour une analyse future. Utilisez le profiler dans l'environnement de
développement afin de vous aider à débugger votre code et à améliorer
les performances de votre application; utilisez le dans l'environnement
de production pour explorer des problèmes après coup.

Vous avez rarement besoin d'intéragir avec le profiler directement puisque
Symfony2 vous fournit des outils de visualisation tels la Barre d'Outils de
Débuggage Web (« Web Debug Toolbar ») et le Profiler Web (« Web Profiler »).
Si vous utilisez l'Edition Standard de Symfony2, le profiler, la barre d'outils
de débuggage web, et le profiler web sont tous déjà configurés avec des
paramètres prédéfinis.

.. note::

    Le profiler collecte des informations pour toutes les requêtes (simples
    requêtes, redirections, exceptions, requêtes Ajax, requêtes ESI; et pour
    toutes les méthodes HTTP et tous les formats). Cela signifie que pour
    une même URL, vous pouvez avoir plusieurs données de profiling associées
    (une par paire de requête/réponse externe).

.. index::
   single: Profiler; Visualiser

Visualiser les Données de Profiling
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Utiliser la Barre d'Outils de Débuggage Web
...........................................

Dans l'environnement de développement, la barre d'outils de débuggage web
est disponible en bas de toutes les pages. Elle affiche un bon résumé des
données de profiling qui vous donne accès instantanément à plein
d'informations utiles quand quelque chose ne fonctionne pas comme prévu.

Si le résumé fourni par la Barre d'Outils de Débuggage Web n'est pas suffisant,
cliquez sur le lien du jeton (une chaîne de caractères composée de 13 caractères
aléatoires) pour pouvoir accéder au Profiler Web.

.. note::

    Si le jeton n'est pas cliquable, cela signifie que les routes du profiler
    ne sont pas enregistrées (voir ci-dessous pour les informations concernant
    la configuration).

Analyser les données de Profiling avec le Profiler Web
......................................................

Le Profiler Web est un outil de visualisation pour profiler des données que vous
pouvez utiliser en développement pour débugger votre code et améliorer les
performances ; mais il peut aussi être utilisé pour explorer des problèmes
qui surviennent en production. Il expose toutes les informations collectées
par le profiler via une interface web.

.. index::
   single: Profiler; Utiliser le service profiler

Accéder aux informations de Profiling
.....................................

Vous n'avez pas besoin d'utiliser l'outil de visualisation par défaut pour
accéder aux informations de profiling. Mais comment pouvez-vous obtenir
les informations de profiling pour une requête spécifique après coup ?
Lorsque le profiler stocke les données concernant une Requête, il
lui associe aussi un jeton ; ce jeton est disponible dans l'en-tête HTTP
``X-Debug-Token`` de la Réponse ::

    $profile = $container->get('profiler')->loadProfileFromResponse($response);

    $profile = $container->get('profiler')->loadProfile($token);

.. tip::

    Lorsque le profiler est activé mais sans la barre d'outils de débuggage web,
    ou lorsque vous voulez récupérer le jeton pour une requête Ajax, utilisez un
    outil comme Firebug pour obtenir la valeur de l'en-tête HTTP ``X-Debug-Token``.

Utilisez la méthode ``find()`` pour accéder aux jetons basés sur quelques critères :

    // récupère les 10 derniers jetons
    $tokens = $container->get('profiler')->find('', '', 10);

    // récupère les 10 derniers jetons pour toutes les URL contenant /admin/
    $tokens = $container->get('profiler')->find('', '/admin/', 10);

    // récupère les 10 derniers jetons pour les requêtes locales
    $tokens = $container->get('profiler')->find('127.0.0.1', '', 10);

Si vous souhaitez manipuler les données de profiling sur une machine différente
que celle où les informations ont été générées, utilisez les méthodes ``export()``
et ``import()`` ::

    // sur la machine de production
    $profile = $container->get('profiler')->loadProfile($token);
    $data = $profiler->export($profile);

    // sur la machine de développement
    $profiler->import($data);

.. index::
   single: Profiler; Visualiser

Configuration
.............

La configuration par défaut de Symfony2 vient avec des paramètres prédéfinis
pour le profiler, la barre d'outils de débuggage web, et le profiler web.
Voici par exemple la configuration pour l'environnement de développement :

.. configuration-block::

    .. code-block:: yaml

        # charge le profiler
        framework:
            profiler: { only_exceptions: false }

        # active le profiler web
        web_profiler:
            toolbar: true
            intercept_redirects: true
            verbose: true

    .. code-block:: xml

        <!-- xmlns:webprofiler="http://symfony.com/schema/dic/webprofiler" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/webprofiler http://symfony.com/schema/dic/webprofiler/webprofiler-1.0.xsd"> -->

        <!-- charge le profiler -->
        <framework:config>
            <framework:profiler only-exceptions="false" />
        </framework:config>

        <!-- active le profiler web -->
        <webprofiler:config
            toolbar="true"
            intercept-redirects="true"
            verbose="true"
        />

    .. code-block:: php

        // charge le profiler
        $container->loadFromExtension('framework', array(
            'profiler' => array('only-exceptions' => false),
        ));

        // active le profiler web
        $container->loadFromExtension('web_profiler', array(
            'toolbar' => true,
            'intercept-redirects' => true,
            'verbose' => true,
        ));

Quand l'option ``only-exceptions`` est définie comme ``true``, le profiler
collecte uniquement des données lorsqu'une exception est capturée par
l'application.

Quand l'option ``intercept-redirects`` est définie à ``true``, le web
profiler intercepte les redirections et vous donne l'opportunité d'inspecter
les données collectées avant de suivre la redirection.

Quand l'option ``verbose`` est définie à ``true``, la Barre d'Outils de
Débuggage Web affiche beaucoup d'informations. Définir ``verbose`` à ``false``
cache quelques informations secondaires afin de rendre la barre d'outils plus
petite.

Si vous activez le profiler web, vous avez aussi besoin de monter les routes
du profiler :

.. configuration-block::

    .. code-block:: yaml

        _profiler:
            resource: @WebProfilerBundle/Resources/config/routing/profiler.xml
            prefix:   /_profiler

    .. code-block:: xml

        <import resource="@WebProfilerBundle/Resources/config/routing/profiler.xml" prefix="/_profiler" />

    .. code-block:: php

        $collection->addCollection($loader->import("@WebProfilerBundle/Resources/config/routing/profiler.xml"), '/_profiler');

Comme le profiler rajoute du traitement supplémentaire, vous pourriez vouloir
l'activer uniquement selon certaines circonstances dans l'environnement de
production. Le paramètre ``only-exceptions`` limite le profiling aux pages 500,
mais qu'en est-il si vous voulez avoir les informations lorsque l'IP du client
provient d'une adresse spécifique, ou pour une portion limitée du site web ?
Vous pouvez utiliser la correspondance de requête :

.. configuration-block::

    .. code-block:: yaml

        # active le profiler uniquement pour les requêtes venant du réseau 192.168.0.0
        framework:
            profiler:
                matcher: { ip: 192.168.0.0/24 }

        # active le profiler uniquement pour les URLs /admin
        framework:
            profiler:
                matcher: { path: "^/admin/" }

        # associe des règles
        framework:
            profiler:
                matcher: { ip: 192.168.0.0/24, path: "^/admin/" }

        # utilise une instance de correspondance personnalisée définie dans le
        # service "custom_matcher"
        framework:
            profiler:
                matcher: { service: custom_matcher }

    .. code-block:: xml

        <!-- active le profiler uniquement pour les requêtes venant du réseau 192.168.0.0 -->
        <framework:config>
            <framework:profiler>
                <framework:matcher ip="192.168.0.0/24" />
            </framework:profiler>
        </framework:config>

        <!-- active le profiler uniquement pour les URLs /admin -->
        <framework:config>
            <framework:profiler>
                <framework:matcher path="^/admin/" />
            </framework:profiler>
        </framework:config>

        <!-- associe des règles -->
        <framework:config>
            <framework:profiler>
                <framework:matcher ip="192.168.0.0/24" path="^/admin/" />
            </framework:profiler>
        </framework:config>

        <!-- utilise une instance de correspondance personnalisée définie dans le service "custom_matcher" -->
        <framework:config>
            <framework:profiler>
                <framework:matcher service="custom_matcher" />
            </framework:profiler>
        </framework:config>

    .. code-block:: php

        // active le profiler uniquement pour les requêtes venant du réseau 192.168.0.0
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('ip' => '192.168.0.0/24'),
            ),
        ));

        // active le profiler uniquement pour les URLs /admin
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('path' => '^/admin/'),
            ),
        ));

        // associe des règles
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('ip' => '192.168.0.0/24', 'path' => '^/admin/'),
            ),
        ));

        # utilise une instance de correspondance personnalisée définie dans le
        # service "custom_matcher"
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('service' => 'custom_matcher'),
            ),
        ));

En savoir plus grâce au Cookbook
--------------------------------

* :doc:`/cookbook/testing/profiling`
* :doc:`/cookbook/profiler/data_collector`
* :doc:`/cookbook/event_dispatcher/class_extension`
* :doc:`/cookbook/event_dispatcher/method_behavior`

.. _`Composant d'Injection de Dépendances de Symfony2`: https://github.com/symfony/DependencyInjection
