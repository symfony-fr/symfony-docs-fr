.. index::
   single: Security; Configuration Reference

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
            access_denied_url: /foo/error403

            always_authenticate_before_granting: false

            # whether or not to call eraseCredentials on the token
            erase_credentials: true

            # strategy can be: none, migrate, invalidate
            session_fixation_strategy: migrate

            access_decision_manager:
                strategy: affirmative
                allow_if_all_abstain: false
                allow_if_equal_granted_denied: true

            acl:
                connection: default # n'importe quel nom configuré dans la section doctrine.dbal
                tables:
                    class: acl_classes
                    entry: acl_entries
                    object_identity: acl_object_identities
                    object_identity_ancestors: acl_object_identity_ancestors
                    security_identity: acl_security_identities
                cache:
                    id: service_id
                    prefix: sf2_acl_
                voter:
                    allow_if_object_identity_unavailable: true

            encoders:
                somename:
                    class: Acme\DemoBundle\Entity\User
                Acme\DemoBundle\Entity\User: sha512
                Acme\DemoBundle\Entity\User: plaintext
                Acme\DemoBundle\Entity\User:
                    algorithm: sha512
                    encode_as_base64: true
                    iterations: 5000
                Acme\DemoBundle\Entity\User:
                    id: my.custom.encoder.service.id

            providers:
                memory_provider_name:
                    memory:
                        users:
                            foo: { password: foo, roles: ROLE_USER }
                            bar: { password: bar, roles: [ROLE_USER, ROLE_ADMIN] }
                entity_provider_name:
                    entity: { class: SecurityBundle:User, property: username }

            firewalls:
                somename:
                    pattern: .*
                    request_matcher: some.service.id
                    access_denied_url: /foo/error403
                    access_denied_handler: some.service.id
                    entry_point: some.service.id
                    provider: some_provider_key_from_above
                    context: name
                    stateless: false
                    x509:
                        provider: some_provider_key_from_above
                    http_basic:
                        provider: some_provider_key_from_above
                    http_digest:
                        provider: some_provider_key_from_above
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

            access_control:
                -
                    path: ^/foo
                    host: mydomain.foo
                    ip: 192.0.0.0/8
                    roles: [ROLE_A, ROLE_B]
                    requires_channel: https

            role_hierarchy:
                ROLE_SUPERADMIN: ROLE_ADMIN
                ROLE_SUPERADMIN: 'ROLE_ADMIN, ROLE_USER'
                ROLE_SUPERADMIN: [ROLE_ADMIN, ROLE_USER]
                anything: { id: ROLE_SUPERADMIN, value: 'ROLE_USER, ROLE_ADMIN' }
                anything: { id: ROLE_SUPERADMIN, value: [ROLE_USER, ROLE_ADMIN] }

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
	
	Cette URL **doit** être accessible par un utilisateur normal non authentifié,
	sinon vous pourriez créer une boucle de redirections. Pour plus de détails,
	lisez « :ref:`Eviter les pièges classiques<book-security-common-pitfalls>` ».

*   ``check_path`` (type: ``string``, default: ``/login_check``)
    C'est l'URL à laquelle votre formulaire doit être soumis. Le firewall
	interceptera toute requête (par défaut seulement les requêtes ``POST``)
	envoyée à cette URL et traitera les credentials soumis.
    
	Assurez vous que cette URL est couverte par votre firewall principal
	(c'est-à-dire que vous ne devez pas créer de firewall séparé pour l'URL
    ``check_path``).

*   ``use_forward`` (type: ``Boolean``, default: ``false``)
    Si vous voulez que l'utilisateur soit « forwardé » vers le formulaire
	d'authentification au lieu d'être redirigé, définissez cette option à ``true``.

*   ``username_parameter`` (type: ``string``, default: ``_username``)
    C'est le nom de champ que vous devez donner au champ nom
	d'utilisateur de votre formulaire de connexion. Lorsque vous soumettrez
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
