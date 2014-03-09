.. index::
   single: Security, Authorization

Authorisation
=============

Lorsque n'importe quel fournisseur d'authenfication
(consultez :ref:`authentication_providers`) a vérifié  le jeton pas encore
authentifié, un jeton authentifié sera retourné. L'écouteur d'authentification
devrait initialiser le jeton directement dans
:class:`Symfony\\Component\\Security\\Core\\SecurityContextInterface` en utilisant
sa méthode :method:`Symfony\\Component\\Security\\Core\\SecurityContextInterface::setToken`.

A partir de là, l'utilisateur est authentifié, i.e. identifié. Maintenant, les autres
parties de l'application peuvent utiliser le jeton pour décider si oui ou non
l'utilisateur peut demander une certaine URI, ou modifier un objet. Cette
décision se prise par une instance de la classe
:class:`Symfony\\Component\\Security\\Core\\Authorization\\AccessDecisionManagerInterface`.

Une décision d'authorisation sera toujours basée sur quelques éléments :

* Le jeton courant
    Par exemple, le jeton de la méthode :method:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\TokenInterface::getRoles
    devrait être utilisé pour retrouver les rôles de l'utilisateur courant
    (par exemple ``ROLE_SUPER_ADMIN``), ou une décision pourrait être basée
    sur la classe du jeton.
* Un ensemble d'attributs
    Chaque attribut représente les droits que l'utilisateur devrait avoir,
    par exemple ``ROLE_ADMIN`` pour être sûre que l'utilisateur est un
    administrateur.
* Un objet (optionnel)
    N'importe quel objet pour lequel les contrôles d'accès doivent être
    vérifiés, comme un article ou un objet commentaire.

.. _components-security-access-decision-manager:

Gestionnaire de décision d'accès
--------------------------------

La décision pour effectuer une action demandant à l'utilisateur d'être authorisé ou
non peut être un processus compliqué. Le gestionnaire standard
:class:`Symfony\\Component\\Security\\Core\\Authorization\\AccessDecisionManager`
dépend lui même de plusieurs voteurs ("voters" en anglais) et rend un verdict final
basé sur tous les votes (soit positif, négatif ou neutre) il a reçu. Il reconnait
plusieurs stratégies :

* ``affirmatif`` (par défaut)
    donne accès dès que n'importe voteur retourne une réponse affirmative;

* ``consensus``
    donne accès s'il y a plus de voteurs donnant accès qu'il y a le refusant;

* ``unanime``
    ne donne accès que si aucun des voteurs n'a refusé l'accès;

.. code-block:: php

    use Symfony\Component\Security\Core\Authorization\AccessDecisionManager;

    // instances de Symfony\Component\Security\Core\Authorization\Voter\VoterInterface
    $voters = array(...);

    // l'un des suivants : "affirmatif", "consensus", "unanime"
    $strategy = ...;

    // donne accès ou non, si tous les voteurs s'abstiennent
    $allowIfAllAbstainDecisions = ...;

    // donne accès ou non lorsqu'il y a une majorité (ne s'applique qu'à la stratégie "consensus")
    $allowIfEqualGrantedDeniedDecisions = ...;

    $accessDecisionManager = new AccessDecisionManager(
        $voters,
        $strategy,
        $allowIfAllAbstainDecisions,
        $allowIfEqualGrantedDeniedDecisions
    );

.. seealso::

    Vous pouvez changer la stratégie par défaut en
    :ref:`configuration <security-voters-change-strategy>`.

Voteurs
-------

Les voteurs sont des instances de l'interface
:class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface`,
ce qui signifie qu'ils doivent implémenter quelques méthodes permettant
au gestionnaire de décision de les utiliser :

* ``supportsAttribute($attribute)``
    sera utilisé pour vérifier si le voteur sait comment traiter les attributs donnés;

* ``supportsClass($class)``
    sera utilisé pour vérifier si le voteur est capable de donner accès ou le refuser
    pour un objet d'une classé donnée;

* ``vote(TokenInterface $token, $object, array $attributes)``
    cette méthode se charge du fameux vote et retour une valeur égale à l'une des
    constantes de la classe :class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface`,
    i.e. ``VoterInterface::ACCESS_GRANTED``, ``VoterInterface::ACCESS_DENIED``
    ou ``VoterInterface::ACCESS_ABSTAIN``;

Le composant de sécurité contient quelques voteur standards couvrants de
nombreuses cas d'utilisation :


AuthenticatedVoter
~~~~~~~~~~~~~~~~~~

Le voteur :class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\AuthenticatedVoter`
supporte les attributs ``IS_AUTHENTICATED_FULLY``, ``IS_AUTHENTICATED_REMEMBERED``,
et ``IS_AUTHENTICATED_ANONYMOUSLY``. Il se charge de donner les accès en se basant sur le
niveau courant d'authentification, i.e. est-ce que l'utilisateur est complètement authentifié
ou est-ce qu'il est authentifié grâce au cookie "se souvenir de moi", ou est-ce qu'il est authentifié
anonymement?

.. code-block:: php

    use Symfony\Component\Security\Core\Authentication\AuthenticationTrustResolver;

    $anonymousClass = 'Symfony\Component\Security\Core\Authentication\Token\AnonymousToken';
    $rememberMeClass = 'Symfony\Component\Security\Core\Authentication\Token\RememberMeToken';

    $trustResolver = new AuthenticationTrustResolver($anonymousClass, $rememberMeClass);

    $authenticatedVoter = new AuthenticatedVoter($trustResolver);

    // instance de Symfony\Component\Security\Core\Authentication\Token\TokenInterface
    $token = ...;

    // n'importe quel objet
    $object = ...;

    $vote = $authenticatedVoter->vote($token, $object, array('IS_AUTHENTICATED_FULLY'));

RoleVoter
~~~~~~~~~

La classe :class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\RoleVoter`
supporte les attributs commençants par ``ROLE_`` et donne accès à l'utilisateur lorsque
l'attribut requis ``ROLE_*`` peut être retrouvé dans le tableau des rôles retourné par
la méthode :method:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\TokenInterface::getRoles`
du jeton ::

    use Symfony\Component\Security\Core\Authorization\Voter\RoleVoter;

    $roleVoter = new RoleVoter('ROLE_');

    $roleVoter->vote($token, $object, 'ROLE_ADMIN');

RoleHierarchyVoter
~~~~~~~~~~~~~~~~~~

La classe :class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\RoleHierarchyVoter`
étend la classe :class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\RoleVoter`
et fournit quelques fonctionnalités supplémentaires : elle sait comment traiter la hierarchie
des rôles. Par exemple, un rôle ``ROLE_SUPER_ADMIN`` peut avoir les sous-rôles ``ROLE_ADMIN``
et ``ROLE_USER``, ainsi dans le cas ou un objet recquiert que l'utilisateur ait
le rôle ``ROLE_ADMIN``, l'accès est donné aux utilisateurs qui ont en fait le rôle ``ROLE_ADMIN``,
mais également au utilisateurs aillant de le rôle ``ROLE_SUPER_ADMIN`` ::

    use Symfony\Component\Security\Core\Authorization\Voter\RoleHierarchyVoter;
    use Symfony\Component\Security\Core\Role\RoleHierarchy;

    $hierarchy = array(
        'ROLE_SUPER_ADMIN' => array('ROLE_ADMIN', 'ROLE_USER'),
    );

    $roleHierarchy = new RoleHierarchy($hierarchy);

    $roleHierarchyVoter = new RoleHierarchyVoter($roleHierarchy);

.. note::

    Lorsque vous faites votre propre voteur, vous devriez bien évidemment
    injecter dans le constructeur de celui-ci toutes les dépendances nécessaire
    pour une prise de décision.

Les Roles
---------

Les rôles dont des objets représentant uen expression des droits
qu'un utilisateur possède.
Le seul prérequis est que l'objet implémente l'interface
:class:`Symfony\\Component\\Security\\Core\\Role\\RoleInterface`, ce qui
signifie qu'il doit avoir une méthode :method:`Symfony\\Component\\Security\\Core\\Role\\Role\\RoleInterface::getRole`
qui retroune une string représentant le rôle lui-même. La classe par défaut
:class:`Symfony\\Component\\Security\\Core\\Role\\Role` retourne simplement son
premier argument de constructeur ::

    use Symfony\Component\Security\Core\Role\Role;

    $role = new Role('ROLE_ADMIN');

    // va afficher 'ROLE_ADMIN'
    echo $role->getRole();

.. note::

    La majorité des jetons d'authentification étendent la classe
    :class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\AbstractToken`, ce qui
    signifie que les rôles donnés à leur constructeur seront automatiquement convertit
    d'une string à ces objects simples ``Role``.

Utiliser le gestionnaire de décision
------------------------------------

L'écouteur d'accès
~~~~~~~~~~~~~~~~~~

Le gestionnaire de décision d'accès peut être utilisé à n'importe quel moment
dans une requête pour décider si oui ou non l'utilisateur courant peut avoir
accès une ressource donnée. Une méthode optionnelle, mais utile, pour restreindre
l'accès en se basant sur le motif d'URL est la classe
:class:`Symfony\\Component\\Security\\Http\\Firewall\\AccessListener`, qui est l'un
des écouteurs pare-feu (consultez :ref:`firewall_listeners`) qui est déclenché
pour chaque requête correspondante au plan du pare-feu (consultez :ref:`firewall`).

Il utilise un plan d'accès (qui devrait être du type
:class:`Symfony\\Component\\Security\\Http\\AccessMapInterface`) contenant les
"request matchers" et une liste d'attributs requis pour l'utilisateur courant pour
récupérer l'accès à l'application ::

    use Symfony\Component\Security\Http\AccessMap;
    use Symfony\Component\HttpFoundation\RequestMatcher;
    use Symfony\Component\Security\Http\Firewall\AccessListener;

    $accessMap = new AccessMap();
    $requestMatcher = new RequestMatcher('^/admin');
    $accessMap->add($requestMatcher, array('ROLE_ADMIN'));

    $accessListener = new AccessListener(
        $securityContext,
        $accessDecisionManager,
        $accessMap,
        $authenticationManager
    );

Le contexte de sécurité
~~~~~~~~~~~~~~~~~~~~~~~

Le gestionnaire de décision d'accès est également disponible pour les autres
parties de l'application via la méthode
:method:`Symfony\\Component\\Security\\Core\\SecurityContext::isGranted` de la
classe :class:`Symfony\\Component\\Security\\Core\\SecurityContext`.
Un appel à cette méthode délèguera directement la question au gestionnaire
de décision d'accès ::


    use Symfony\Component\Security\SecurityContext;
    use Symfony\Component\Security\Core\Exception\AccessDeniedException;

    $securityContext = new SecurityContext(
        $authenticationManager,
        $accessDecisionManager
    );

    if (!$securityContext->isGranted('ROLE_ADMIN')) {
        throw new AccessDeniedException();
    }
