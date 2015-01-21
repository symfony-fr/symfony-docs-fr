.. index::
   single: Security; Configuration reference

Configuration de référence de la Sécurité 
=========================================

Le système de sécurité est l'une des parties les plus puissantes de Symfony2,
et il peut être en grande partie contrôlé via sa configuration.

Configuration complète par défaut
---------------------------------

Voici la configuration complète par défaut du système de sécurité.
Chaque partie sera expliquée dans la section suivante.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            access_denied_url:    ~ # Exemple: /foo/error403

            # la stratégie peut être soit: none, migrate, invalidate
            session_fixation_strategy:  migrate
            hide_user_not_found:  true
            always_authenticate_before_granting:  false
            erase_credentials:    true
            access_decision_manager:
                strategy:             affirmative
                allow_if_all_abstain:  false
                allow_if_equal_granted_denied:  true
            acl:

                # un nom défini dans la section doctrine.dbal
                connection:           ~
                cache:
                    id:                   ~
                    prefix:               sf2_acl_
                provider:             ~
                tables:
                    class:                acl_classes
                    entry:                acl_entries
                    object_identity:      acl_object_identities
                    object_identity_ancestors:  acl_object_identity_ancestors
                    security_identity:    acl_security_identities
                voter:
                    allow_if_object_identity_unavailable:  true

            encoders:
                # Exemples:
                Acme\DemoBundle\Entity\User1: sha512
                Acme\DemoBundle\Entity\User2:
                    algorithm:           sha512
                    encode_as_base64:    true
                    iterations:          5000

                # encodeur PBKDF2
                # lire les notes à propos de PBKDF2 plus bas pour des détails sur la sécurité et la vitesse
                Acme\Your\Class\Name:
                    algorithm:            pbkdf2
                    hash_algorithm:       sha512
                    encode_as_base64:     true
                    iterations:           1000

                # Exemple options/valeurs pour créer un encodeur personnalisé
                Acme\DemoBundle\Entity\User3:
                    id:                   my.encoder.id

            providers:            # Requis
                # Exemples:
                my_in_memory_provider:
                    memory:
                        users:
                            foo:
                                password:           foo
                                roles:              ROLE_USER
                            bar:
                                password:           bar
                                roles:              [ROLE_USER, ROLE_ADMIN]

                my_entity_provider:
                    entity:
                        class:              SecurityBundle:User
                        property:           username
                        manager_name:       ~

                # Exemple d'un provider personnalisé
                my_some_custom_provider:
                    id:                   ~

                # Enchainement de plusieurs providers
                my_chain_provider:
                    chain:
                        providers:          [ my_in_memory_provider, my_entity_provider ]

            firewalls:            # Requis
                # Exemples:
                somename:
                    pattern: .*
                    request_matcher: some.service.id
                    access_denied_url: /foo/error403
                    access_denied_handler: some.service.id
                    entry_point: some.service.id
                    provider: some_key_from_above
                    # Gère comment chaque firewall stockent les informations en session
                    # Lire "Contexte du Firewall" plus bas pour plus de détails
                    context: context_key
                    stateless: false
                    x509:
                        provider: some_key_from_above
                    http_basic:
                        provider: some_key_from_above
                    http_digest:
                        provider: some_key_from_above
                    form_login:
                        # Soumet le formulaire de connection ici
                        check_path: /login_check

                        # l'utilisateur est redirigé ici si il/elle a besoin de se connecter
                        login_path: /login

                        # si true, l'utilisateur est envoyé au formulaire de connexion et non redirigé
                        use_forward: false

                        # Les options de redirection en cas de succès de connexion (lire plus bas)
                        always_use_default_target_path: false
                        default_target_path:            /
                        target_path_parameter:          _target_path
                        use_referer:                    false

                        # Les options de redirection en cas d'échec de connexion (lire plus bas)
                        failure_path:    /foo
                        failure_forward: false
                        failure_path_parameter: _failure_path
                        failure_handler: some.service.id
                        success_handler: some.service.id

                        # le nom des champs username et password
                        username_parameter: _username
                        password_parameter: _password

                        # les options du token csrf
                        csrf_parameter: _csrf_token
                        intention:      authenticate
                        csrf_provider:  my.csrf_provider.id

                        # par défautt, le formulaire de connextion *doit* être de type POST et non GET
                        post_only:      true
                        remember_me:    false

                        # par défaut, une session doit exister avant de soumettre une requête d'authentification
                        # si false, alors Request::hasPreviousSession n'est pas appelé durant l'authentification
                        # nouveau dans Symfony 2.3
                        require_previous_session: true

                    remember_me:
                        token_provider: name
                        key: someS3cretKey
                        name: NameOfTheCookie
                        lifetime: 3600 # in seconds
                        path: /foo
                        domain: somedomain.foo
                        secure: false
                        httponly: true
                        always_remember_me: false
                        remember_me_parameter: _remember_me
                    logout:
                        path:   /logout
                        target: /
                        invalidate_session: false
                        delete_cookies:
                            a: { path: null, domain: null }
                            b: { path: null, domain: null }
                        handlers: [some.service.id, another.service.id]
                        success_handler: some.service.id
                    anonymous: ~

                # Valeurs par défaut pour tout type de firewall
                some_firewall_listener:
                    pattern:              ~
                    security:             true
                    request_matcher:      ~
                    access_denied_url:    ~
                    access_denied_handler:  ~
                    entry_point:          ~
                    provider:             ~
                    stateless:            false
                    context:              ~
                    logout:
                        csrf_parameter:       _csrf_token
                        csrf_provider:        ~
                        intention:            logout
                        path:                 /logout
                        target:               /
                        success_handler:      ~
                        invalidate_session:   true
                        delete_cookies:

                            # Prototype
                            name:
                                path:                 ~
                                domain:               ~
                        handlers:             []
                    anonymous:
                        key:                  4f954a0667e01
                    switch_user:
                        provider:             ~
                        parameter:            _switch_user
                        role:                 ROLE_ALLOWED_TO_SWITCH

            access_control:
                requires_channel:     ~

                # use the urldecoded format
                path:                 ~ # Exemple: ^/path to resource/
                host:                 ~
                ip:                   ~
                methods:              []
                roles:                []
            role_hierarchy:
                ROLE_ADMIN:      [ROLE_ORGANIZER, ROLE_USER]
                ROLE_SUPERADMIN: [ROLE_ADMIN]

.. _reference-security-firewall-form-login:

Configuration du formulaire de login
------------------------------------

Lorsque vous utilisez l'écouteur d'authentification ``form_login`` derrière
un firewall, il y a plusieurs options communes pour configurer l'utilisation
du « formulaire de login ».

Pour toujours plus de détails, lire :doc:`/cookbook/security/form_login`.

Le formulaire d'authentification et son traitement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*   ``login_path`` (type: ``string``, default: ``/login``)
    C'est l'URL vers laquelle l'utilisateur sera redirigé (à moins que ``use_forward``
    ne soit défini à ``true``) lorsqu'il tente d'accéder à une ressource protégée
    sans être complètement authentifié.

    Cette URL **doit** être accessible par un utilisateur normal non-authentifié,
    sinon vous pourriez créer une boucle de redirections. Pour plus de détails,
    lisez « :ref:`Éviter les pièges classiques<book-security-common-pitfalls>` ».

*   ``check_path`` (type: ``string``, default: ``/login_check``)
    C'est l'URL à laquelle votre formulaire doit être soumis. Le firewall
    interceptera toute requête (par défaut seulement les requêtes ``POST``)
    envoyée à cette URL et traitera les droit d'accès soumis.

    Assurez-vous que cette URL est couverte par votre firewall principal
    (c'est-à-dire que vous ne devez pas créer de firewall séparé pour l'URL
    ``check_path``).

*   ``use_forward`` (type: ``Boolean``, default: ``false``)
    Si vous voulez que l'utilisateur soit « forwardé » vers le formulaire
    d'authentification au lieu d'être redirigé, définissez cette option à ``true``.

*   ``username_parameter`` (type: ``string``, default: ``_username``)
    C'est le nom de champ que vous devez donner au champ « nom
    d'utilisateur » de votre formulaire de connexion. Lorsque vous soumettrez
    le formulaire à l'URL ``check_path``, le système de sécurité cherchera
    un paramètre POST avec ce nom.

*   ``password_parameter`` (type: ``string``, default: ``_password``)
    C'est le nom de champ que vous devez donner au champ « mot de passe »
    de votre formulaire de connexion. Lorsque vous soumettrez
    le formulaire à l'URL ``check_path``, le système de sécurité cherchera
    un paramètre POST avec ce nom.

*   ``post_only`` (type: ``Boolean``, default: ``true``)
    Par défaut, vous devez soumettre votre formulaire à l'URL ``check_path``
    avec une requête POST. En définissant cette option à ``false``, vous
    pouvez également envoyer une requête GET.

Rediriger après authentification
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* ``always_use_default_target_path`` (type: ``Boolean``, default: ``false``)
* ``default_target_path`` (type: ``string``, default: ``/``)
* ``target_path_parameter`` (type: ``string``, default: ``_target_path``)
* ``use_referer`` (type: ``Boolean``, default: ``false``)

.. _reference-security-pbkdf2:

Utiliser l'encodeur PBKDF2 : performance et sécurité
----------------------------------------------------

.. versionadded:: 2.2
    L'encodeur de mot de passe PBKDF2 a été ajouté à Symfony 2.2.

L'encodeur `PBKDF2`_ fournit un haut niveau de sécurité cryptographique,
et est recommandé par le National Institute of Standards and Technology (NIST).

Vous pouvez voir une exemple complet d'encodeur ``pbkdf2`` dans le bloc de code
YAML sur cette page.

Mais attention, utiliser PBKDF2 (avec un grand nombre d'itérations) ralentit
le processus. PBKDF2 devrait être utilisé avec prudence.

Une bonne configuration est constituée d'environ 1000 itérations et utilise
sha512 comme algorithme de hashage.

.. _reference-security-bcrypt:

Utiliser l'encodeur BCrypt
--------------------------

.. caution::

    Pour utiliser cet encodeur, vous devez soit utiliser PHP 5.5 ou supérieur,
    ou alors installer la bibliothèque `ircmaxell/password-compat`_ via Composer.

.. versionadded:: 2.2
    L'encodeur de mot de passe BCrypt a été ajouté à Symfony 2.2.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...

            encoders:
                Symfony\Component\Security\Core\User\User:
                    algorithm: bcrypt
                    cost:      15

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <!-- ... -->
            <encoder
                class="Symfony\Component\Security\Core\User\User"
                algorithm="bcrypt"
                cost="15"
            />
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'encoders' => array(
                'Symfony\Component\Security\Core\User\User' => array(
                    'algorithm' => 'bcrypt',
                    'cost'      => 15,
                ),
            ),
        ));

l'option ``cost`` est compris en ``4-31`` et détermine la longueur du mot de passe
qui sera encodé. Chaque unité de plus *double* le temps pour encoder le mot de passe.

Si vous ne définissez pas l'option ``cost``, la valeur par défaut est ``13``.

.. note::

    Vous pouvez changer le ``cost`` à tout moment — même si vous avez déjà
    des mots encoder avec un ``cost`` différent. Les nouveaux mots de passe
    utiliseront le nouveau ``cost``, alors que les mots de passe encodés
    précédemment utiliseront le ``cost`` utilisé lors de leur encodage.

Un salt est généré automatiquement pour chaque nouveau mot de passe
et n'a pas besoin d'être persisté. Puisque le mot de passe encodé contient le salt
utilisé pour son encodage, il suffit juste de persister le mot de passe encodé.

.. note::

    Tous les mots de passe encodés ont une longeur de ``60`` caractères,
    prévoyez suffisament d'espace pour qu'ils puissent être persisté

    .. _reference-security-firewall-context:

Contexte du Firewall
--------------------

La plupart des applications n'ont besoin que d'un seul :ref:`firewall<book-security-firewalls>`.
Mais si votre application *doit* utiliser plusieurs firewalls, vous devez
prendre en compte que si vous êtes authentifié dans un firewall, vous n'
êtes pas automatiquement authentifié dans un autre. En d'autres termes, les
systèmes ne se partagent pas un contexte commun: chaque firewall agit comme
un système de sécurité distinct.

Cependant, chaque firewall à une clé ``context`` optionnel (qui prends par
défaut le nom du firewall), qui est utilisé pour enregistrer et récupérer
les données de sécurité depuis et vers la session. Si cette clé est définie
avec la même valeur dans chaque firewall, alors le "contexte" peut être
partagé:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...

            firewalls:
                somename:
                    # ...
                    context: my_context
                othername:
                    # ...
                    context: my_context

    .. code-block:: xml

       <!-- app/config/security.xml -->
       <security:config>
          <firewall name="somename" context="my_context">
            <! ... ->
          </firewall>
          <firewall name="othername" context="my_context">
            <! ... ->
          </firewall>
       </security:config>

    .. code-block:: php

       // app/config/security.php
       $container->loadFromExtension('security', array(
            'firewalls' => array(
                'somename' => array(
                    // ...
                    'context' => 'my_context'
                ),
                'othername' => array(
                    // ...
                    'context' => 'my_context'
                ),
            ),
       ));

Authentification HTTP-Digest
----------------------------

Pour utiliser une authentification HTTP-Digest vous devez fournir un domaine et une clé:

.. configuration-block::

   .. code-block:: yaml

      # app/config/security.yml
      security:
         firewalls:
            somename:
              http_digest:
               key: "une_chaine_aleatoire"
               realm: "api-securisee"

   .. code-block:: xml

      <!-- app/config/security.xml -->
      <security:config>
         <firewall name="somename">
            <http-digest key="une_chaine_aleatoire" realm="api-securisee" />
         </firewall>
      </security:config>

   .. code-block:: php

      // app/config/security.php
      $container->loadFromExtension('security', array(
           'firewalls' => array(
               'somename' => array(
                   'http_digest' => array(
                       'key'   => 'une_chaine_aleatoire',
                       'realm' => 'api-securisee',
                   ),
               ),
           ),
      ));

.. _`PBKDF2`: http://en.wikipedia.org/wiki/PBKDF2
.. _`ircmaxell/password-compat`: https://packagist.org/packages/ircmaxell/password-compat
