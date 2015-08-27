.. index::
   single: Security, Firewall

Le Pare-Feu et le Contexte de Sécurité
======================================

Au centre du composant sécurité il y a l'autorisation, qui est une instance
de :class:`Symfony\\Component\\Security\\Core\\Authorization\\AuthorizationCheckerInterface`. Lorsque 
toutes les étapes du processus d'authentification de l'utilisateur ont été accomplies 
avec succès, vous pouvez demander au contexte de sécurité si l'utilisateur authentifié 
a accès à une certaine action ou une ressource de l'application ::

    use Symfony\Component\Security\Core\Authorization\AuthorizationChecker;
    use Symfony\Component\Security\Core\Exception\AccessDeniedException;
    
    // instance de Symfony\Component\Security\Core\Authentication\Token\Storage\TokenStorageInterface
    $tokenStorage = ...;
    
    // instance de Symfony\Component\Security\Core\Authentication\AuthenticationManagerInterface
    $authenticationManager = ...;

    // instance de Symfony\Component\Security\Core\Authorization\AccessDecisionManagerInterface
    $accessDecisionManager = ...;
    
    $authorizationChecker = new AuthorizationChecker(
        $tokenStorage,
        $authenticationManager,
        $accessDecisionManager
    );

    // ... authentifier l'utilisateur

    if (!$securityContext->isGranted('ROLE_ADMIN')) {
        throw new AccessDeniedException();
    }
.. versionadded:: 2.6
    A partir de symfony 2.6, la :class:`Symfony\\Component\\Security\\Core\\SecurityContext` a été divisé dans les        classes :class:`Symfony\\Component\\Security\\Core\\Authorization\\AuthorizationChecker` et
    :class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\Storage\\TokenStorage`.
    
.. note::

    Lisez les sections dédiées pour en apprendre plus à propos de
    :doc:`/components/security/authentication` et :doc:`/components/security/authorization`.

.. _firewall:

Un Pare-Feu pour les requêtes HTTP
----------------------------------

L'authentification d'un utilisateur est faite par le pare-feu. Une application
peut avoir de nombreuses zones sécurisées, ainsi le pare-feu est configuré en
utilisant un plan de ces zones sécurisées. Pour chacune des ces zones, le plan
contient un "request matcher" et une collection d'écouteurs. Le request matcher
donne au pare-feu la possibilité de déterminer si la requête pointe vers une zone
sécurisée.
Il est demandé aux écouteurs si la requête courante peut être utilisée pour
authentifier l'utilisateur ::

    use Symfony\Component\Security\Http\FirewallMap;
    use Symfony\Component\HttpFoundation\RequestMatcher;
    use Symfony\Component\Security\Http\Firewall\ExceptionListener;

    $map = new FirewallMap();

    $requestMatcher = new RequestMatcher('^/secured-area/');

    // instances de Symfony\Component\Security\Http\Firewall\ListenerInterface
    $listeners = array(...);

    $exceptionListener = new ExceptionListener(...);

    $map->add($requestMatcher, $listeners, $exceptionListener);

Le plan du pare-feu sera donné au pare-feu comme premier argument, accompagné de l'event dispatcher
qui est utilisé par la classe :class:`Symfony\\Component\\HttpKernel\\HttpKernel` ::

    use Symfony\Component\Security\Http\Firewall;
    use Symfony\Component\HttpKernel\KernelEvents;

    // the EventDispatcher used by the HttpKernel
    $dispatcher = ...;

    $firewall = new Firewall($map, $dispatcher);

    $dispatcher->addListener(KernelEvents::REQUEST, array($firewall, 'onKernelRequest');

Le pare-feu est enregistré pour écouter l'évènement ``kernel.request`` qui
sera dispatché par l'HttpKernel au début de chaque requête initiée.
De cette façon, le pare-feu peut éviter que l'utilisateur aille plus
loin que ce qui est autorisé.

.. _firewall_listeners:

Les écouteurs du pare-feu
~~~~~~~~~~~~~~~~~~~~~~~~~

Lorsque le pare-feu est notifié par l'évènement ``kernel.request``, il
demande au plan du pare-feu si une requête correspond à l'une des
zones sécurisées. La première zone sécurisée correspondant à cette requête
retournera un ensemble d'écouteurs correspondants aux écouteurs du pare-feu
(ceux qui implémentent :class:`Symfony\\Component\\Security\\Http\\Firewall\\ListenerInterface`).

Il est demandé à ces écouteurs de gérer la requête courante. Ceci signifie
que : il faut trouver si la requête courante pourrait contenir des informations
permettant d'authentifier l'utilisateur (par exemple l'écouteur d'authentification
HTTP basique vérifie si la requête contient l'entête HTTP ``PHP_AUTH_USER``);

L'écouteur d'Exception
~~~~~~~~~~~~~~~~~~~~~~

Si l'un des écouteurs jète une :class:`Symfony\\Component\\Security\\Core\\Exception\\AuthenticationException`,
l'écouteur d'exception qui a été donné lorsque les zones sécurisées ont été
déclarées prendra la main.

L'écouteur d'exception détermine ce qu'il va se passer ensuite, basé sur les
arguments reçus lorsque l'exception a été créée. Cet écouteur pourrait démarrer
la procédure d'authentification, peut-être demander à l'utilisateur de fournir
ses identifiants à nouveau (lorsqu'il a été authentifié uniquement grâce au
cookie "remember-me"), ou transformer l'exception en une exception de type 
:class:`Symfony\\Component\\HttpKernel\\Exception\\AccessDeniedHttpException`,
qui se soldera éventuellement par une réponse "HTTP/1.1 403: Access Denied".

Points d'entrés
~~~~~~~~~~~~~~~

Lorsqu'un utilisateur n'est pas du tout authentifié (i.e. lorsque le contexte
de sécurité n'a pas encore de jeton), le point d'entrée du pare-feu sera appelé
pour "commencer" le processus d'authentification. Un point d'entrée doit
implémenter l'interface :class:`Symfony\\Component\\Security\\Http\\EntryPoint\\AuthenticationEntryPointInterface`,
qui ne possède qu'une méthode :method:`Symfony\\Component\\Security\\Http\\EntryPoint\\AuthenticationEntryPointInterface::start`.
Cette méthode reçoit l'objet :class:`Symfony\\Component\\HttpFoundation\\Request`
courant et l'exception par laquelle l'écouteur a été déclenchée.
La méthode devrait retourner un objet de type :class:`Symfony\\Component\\HttpFoundation\\Request`.
Cela pourait être, par exemple, la page contenant le formulaire de login ou,
dans le cas d'une authentification HTTP basique, une réponse avec un en-tête
``WWW-Authenticate``, qui invitera l'utilisateur a fournir son nom d'utilisateur
et son mot de passe.

Flux : Pare-feu, Authentification, Authorisation
------------------------------------------------

Vous devriez maintenant pouvoir cerner plus facilement le fonctionnement global
du contexte de sécurité :

#. Le pare-feu est enregistré comme écouteur de l'évènement ``kernel.request``;
#. le pare-feu vérifie le plan du pare-feu afin de déterminer si un pare-feu
   doit être activé pour cette URL;
#. Si un pare-feu est trouvé dans le plan pour cette URL, les écouteurs sont
   notifiés;
#. Chaque écouteur vérifie si la requête courante contient des informations
   d'authentification - un écouteur devra soit (a) authentifier un utilisateur,
   (b) jeter une ``AuthenticationException``, ou (c) ne rien faire (parce qu'il
   n'y a pas d'informatuon d'authentification dans la requête);
#. Une fois l'utilisateur authentifié, vous utiliserez :doc:`/components/security/authorization`
   pour refuser l'accès à certaines ressources.

Lisez les prochaines sections pour en savoir plus sur l':doc:`/components/security/authentication`
et l':doc:`/components/security/authorization`.
