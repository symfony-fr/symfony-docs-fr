.. index:: 
   single: Security; Customizing form login

Comment personnaliser votre formulaire de login
===============================================

Utiliser un :ref:`formulaire de login<book-security-form-login>` est une méthode
classique et flexible pour gérer l'authentification dans Symfony2. Quasiment chaque
aspect du formulaire de login peut être personnalisé. La configuration complète
par défaut est détaillée dans la prochaine section.

Référence de Configuration du formulaire de login
-------------------------------------------------

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # l'utilisateur est redirigé ici quand il/elle a besoin de se connecter
                        login_path:                     /login

                        # si défini à true, « forward » l'utilisateur vers le formulaire de
                        # login au lieu de le rediriger
                        use_forward:                    false

                        # soumet le formulaire de login vers cette URL
                        check_path:                     /login_check

                        # par défaut, le formulaire de login *doit* être un POST,
                        # et pas un GET
                        post_only:                      true

                        # options de redirection lorsque le login a réussi (vous
                        # pouvez en lire plus ci-dessous)
                        always_use_default_target_path: false
                        default_target_path:            /
                        target_path_parameter:          _target_path
                        use_referer:                    false

                        # options de redirection lorsque le login échoue (vous
                        # pouvez en lire plus ci-dessous)
                        failure_path:                   null
                        failure_forward:                false

                        # noms des champs pour le nom d'utilisateur et le mot
                        # de passe
                        username_parameter:             _username
                        password_parameter:             _password

                        # options du token csrf
                        csrf_parameter:                 _csrf_token
                        intention:                      authenticate

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    check_path="/login_check"
                    login_path="/login"
                    use_forward="false"
                    always_use_default_target_path="false"
                    default_target_path="/"
                    target_path_parameter="_target_path"
                    use_referer="false"
                    failure_path="null"
                    failure_forward="false"
                    username_parameter="_username"
                    password_parameter="_password"
                    csrf_parameter="_csrf_token"
                    intention="authenticate"
                    post_only="true"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    'check_path'                     => '/login_check',
                    'login_path'                     => '/login',
                    'user_forward'                   => false,
                    'always_use_default_target_path' => false,
                    'default_target_path'            => '/',
                    'target_path_parameter'          => _target_path,
                    'use_referer'                    => false,
                    'failure_path'                   => null,
                    'failure_forward'                => false,
                    'username_parameter'             => '_username',
                    'password_parameter'             => '_password',
                    'csrf_parameter'                 => '_csrf_token',
                    'intention'                      => 'authenticate',
                    'post_only'                      => true,
                )),
            ),
        ));

Rediriger après un succès
-------------------------

Vous pouvez changer l'URL de redirection après que le formulaire de login
a été soumis avec succès via plusieurs options de configuration. Par défaut,
le formulaire va rediriger l'utilisateur vers l'URL qu'il a demandée (c-a-d l'URL
qui a déclenchée le formulaire de login qui est montré). Par exemple, si
l'utilisateur a demandé ``http://www.example.com/admin/post/18/edit``, alors
après, il sera éventuellement redirigé vers ``http://www.example.com/admin/post/18/edit``
dans le cas d'un succès de connexion. Cela est effectué en stockant l'URL
demandée dans la session. Si aucune URL n'est présente dans la session (peut-être
que l'utilisateur a été directement sur la page de login), alors l'utilisateur
est redirigé vers la page par défaut, qui est ``/`` (c-a-d. la page d'accueil) par
défaut. Vous pouvez changer ce comportement de différentes façons.

.. note::

    Comme précisé, par défaut, l'utilisateur est redirigé vers la page qu'il
    avait demandée à la base. Quelquefois, cela peut poser des problèmes, comme
    par exemple si une requête AJAX en arrière-plan « apparaît » comme étant la
    dernière URL visitée, redirigeant l'utilisateur vers cette dernière. Pour plus
    d'informations sur comment contrôler ce comportement, lisez
    :doc:`/cookbook/security/target_path`.

Changer la page par défaut
~~~~~~~~~~~~~~~~~~~~~~~~~~

Tout d'abord, la page par défaut peut être définie (c-a-d la page vers laquelle
l'utilisateur est redirigée si aucune page n'avait été précédemment stockée
dans la session). Pour la définir en tant que ``/admin``, utilisez la configuration
suivante :

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # ...
                        default_target_path: /admin

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    default_target_path="/admin"                    
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    // ...
                    'default_target_path' => '/admin',
                )),
            ),
        ));

Maintenant, quand aucune URL n'est définie dans la session, l'utilisateur
va être envoyé vers ``/admin``.

Toujours rediriger vers la page par défaut
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez faire en sorte que les utilisateurs soient toujours redirigés vers la
page par défaut sans tenir compte de l'URL qu'ils avaient demandée en définissant
l'option ``always_use_default_target_path`` à « true » :

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # ...
                        always_use_default_target_path: true
                        
    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    always_use_default_target_path="true"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    // ...
                    'always_use_default_target_path' => true,
                )),
            ),
        ));

Utiliser l'URL référante
~~~~~~~~~~~~~~~~~~~~~~~~

Dans le cas où aucune URL n'a été stockée dans la session, vous pourriez souhaiter
essayer d'utiliser ``HTTP_REFERER`` à la place, comme ce dernier sera souvent
identique. Vous pouvez effectuer cela en définissant ``use_referer`` à « true »
(par défaut la valeur est « false ») :

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # ...
                        use_referer:        true

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    use_referer="true"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    // ...
                    'use_referer' => true,
                )),
            ),
        ));

.. versionadded:: 2.1
    Depuis la version 2.1, si le référant est égal à l'option ``login_path``,
    l'utilisateur sera redirigé vers le ``default_target_path``.

Contrôler l'URL de redirection depuis le formulaire
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez aussi surcharger le chemin vers lequel l'utilisateur est redirigé
via le formulaire lui-même en incluant un champ caché avec le nom ``_target_path``.
Par exemple, pour rediriger vers l'URL définie par une route ``account``,
utilisez ce qui suit :

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/SecurityBundle/Resources/views/Security/login.html.twig #}
        {% if error %}
            <div>{{ error.message }}</div>
        {% endif %}

        <form action="{{ path('login_check') }}" method="post">
            <label for="username">Username:</label>
            <input type="text" id="username" name="_username" value="{{ last_username }}" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            <input type="hidden" name="_target_path" value="account" />

            <input type="submit" name="login" />
        </form>

    .. code-block:: html+php

        <?php // src/Acme/SecurityBundle/Resources/views/Security/login.html.php ?>
        <?php if ($error): ?>
            <div><?php echo $error->getMessage() ?></div>
        <?php endif; ?>

        <form action="<?php echo $view['router']->generate('login_check') ?>" method="post">
            <label for="username">Username:</label>
            <input type="text" id="username" name="_username" value="<?php echo $last_username ?>" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            <input type="hidden" name="_target_path" value="account" />
            
            <input type="submit" name="login" />
        </form>

Maintenant, l'utilisateur va être redirigé vers la valeur du champ caché du
formulaire. La valeur de l'attribut peut être un chemin relatif, une URL
absolue, ou un nom de route. Vous pouvez même changer le nom du champ
caché du formulaire en changeant l'option ``target_path_parameter``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        target_path_parameter: redirect_url

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    target_path_parameter="redirect_url"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    'target_path_parameter' => redirect_url,
                )),
            ),
        ));

Redirection en cas d'échec du login
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En plus de la redirection lorsqu'un utilisateur réussit à se connecter, vous
pouvez aussi définir l'URL vers laquelle l'utilisateur devrait être redirigé
après un échec lors de la phase de login (par exemple : un nom d'utilisateur
ou mot de passe non-valide a été soumis). Par défaut, l'utilisateur est
redirigé vers le formulaire de login lui-même. Vous pouvez définir une URL
différente en utilisant la configuration suivante :

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # ...
                        failure_path: /login_failure
                        
    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    failure_path="login_failure"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    // ...
                    'failure_path' => login_failure,
                )),
            ),
        ));
