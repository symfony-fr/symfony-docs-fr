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

            # strategy peut valoir : none, migrate, invalidate
            session_fixation_strategy:  migrate
            hide_user_not_found:  true
            always_authenticate_before_granting:  false
            erase_credentials:    true
            access_decision_manager:
                strategy:             affirmative
                allow_if_all_abstain:  false
                allow_if_equal_granted_denied:  true
            acl:

                # N'importe quel nom configuré dans la section doctrine.dbal
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
                # lisez la note en fin d'article sur la sécurité et les performances
                Acme\Your\Class\Name:
                    algorithm:            pbkdf2
                    hash_algorithm:       sha512
                    encode_as_base64:     true
                    iterations:           1000

                # Exemple d'options/valeurs pour voir à quoi ressemble un encodeur personnalisé
                Acme\Your\Class\Name:
                    algorithm:            ~
                    ignore_case:          false
                    encode_as_base64:     true
                    iterations:           5000
                    id:                   ~

            providers:            # Requis
                # Exemples:
                memory:
                    name:                memory
                    users:
                        foo:
                            password:            foo
                            roles:               ROLE_USER
                        bar:
                            password:            bar
                            roles:               [ROLE_USER, ROLE_ADMIN]
                entity:
                    entity:
                        class:               SecurityBundle:User
                        property:            username

                # Exemple d'un provider personnalisé
                some_custom_provider:
                    id:                   ~
                    chain:
                        providers:            []

            firewalls:            # Requis
                # Exemples:
                somename:
                    pattern: .*
                    request_matcher: some.service.id
                    access_denied_url: /foo/error403
                    access_denied_handler: some.service.id
                    entry_point: some.service.id
                    provider: some_key_from_above
                    context: name
                    stateless: false
                    x509:
                        provider: some_key_from_above
                    http_basic:
                        provider: some_key_from_above
                    http_digest:
                        provider: some_key_from_above
                    form_login:
                        check_path: /login_check
                        login_path: /login
                        use_forward: false
                        always_use_default_target_path: false
                        default_target_path: /
                        target_path_parameter: _target_path
                        use_referer: false
                        failure_path: /foo
                        failure_forward: false
                        failure_handler: some.service.id
                        success_handler: some.service.id
                        username_parameter: _username
                        password_parameter: _password
                        csrf_parameter: _csrf_token
                        intention: authenticate
                        csrf_provider: my.csrf_provider.id
                        post_only: true
                        remember_me: false
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

                # Options et valeurs par défaut pour un firewall
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

                # utilise le format « urldecoded »
                path:                 ~ # Exemple: ^/chemin vers la ressource/
                host:                 ~
                ip:                   ~
                methods:              []
                roles:                []
            role_hierarchy:
                ROLE_ADMIN:      [ROLE_ORGANIZER, ROLE_USER]
                ROLE_SUPERADMIN: [ROLE_ADMIN]

.. caution::
    

.. _reference-security-firewall-form-login:

Configuration du formulaire de login
------------------------------------

Lorsque vous utilisez l'écouteur d'authentification ``form_login`` derrière
un firewall, il y a plusieurs options communes pour configurer l'utilisation
du « formulaire de login ».

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

Utiliser l'encodeur PBKDF2 : performance et sécurité
----------------------------------------------------

L'encodeur `PBKDF2`_ fournit un haut niveau de sécurité cryptographique,
et est recommandé par le National Institute of Standards and Technology (NIST).

Mais attention, utiliser PBKDF2 (avec un grand nombre d'itérations) ralentit
le processus. PBHDF2 devrait être utilisé avec prudence.

Une bonne configuration est constituée d'environ 1000 itération et utilise
sha512 comme algorithme de hashage.

.. _`PBKDF2`: http://en.wikipedia.org/wiki/PBKDF2