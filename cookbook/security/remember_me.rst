.. index::
   single: Security; "Remember me"

Comment ajouter la fonctionnalité de login « Se souvenir de moi »
=================================================================

Une fois qu'un utilisateur est authentifié, ses informations de connexion
sont généralement stockées dans la session. Cela signifie que lorsque la
session se termine, cet utilisateur sera déconnecté et devra fournir de nouveau
ses informations de login la prochaine fois qu'il voudra accéder à
l'application. Vous pouvez laisser aux utilisateurs la possibilité de choisir
de rester connecté plus longtemps que la durée d'une session en utilisant
un cookie avec l'option pare-feu ``remember_me``. Le pare-feu a besoin
d'avoir une clé secrète configurée, qui est utilisée pour encrypter le
contenu du cookie. Il possède aussi plusieurs options avec des valeurs
par défaut qui sont détaillées ici :

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        firewalls:
            main:
                remember_me:
                    key:      "%secret%"
                    lifetime: 3600
                    path:     /
                    domain:   ~ # Prend la valeur par défaut du domaine courant depuis $_SERVER

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <remember-me
                    key      = "%secret%"
                    lifetime = "3600"
                    path     = "/"
                    domain   = "" <!-- Prend la valeur par défaut du domaine courant depuis $_SERVER -->
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('remember_me' => array(
                    'key'                     => '%secret%',
                    'lifetime'                => 3600,
                    'path'                    => '/',
                    'domain'                  => '', // Prend la valeur par défaut du domaine courant depuis $_SERVER
                )),
            ),
        ));

C'est une bonne idée de fournir la possibilité à l'utilisateur de choisir
s'il veut utiliser la fonctionnalité « remember me » ou non, comme cela
ne sera pas toujours approprié. La manière usuelle d'effectuer ceci est
d'ajouter une « checkbox » au formulaire de login. En donnant le nom
``_remember_me`` à la « checkbox », le cookie va automatiquement être
défini lorsque la « checkbox » sera cochée et que l'utilisateur se sera
connecté avec succès. Donc, votre formulaire de login spécifique
pourrait au final ressembler à cela :

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

            <input type="checkbox" id="remember_me" name="_remember_me" checked />
            <label for="remember_me">Keep me logged in</label>

            <input type="submit" name="login" />
        </form>

    .. code-block:: html+php

        <?php // src/Acme/SecurityBundle/Resources/views/Security/login.html.php ?>
        <?php if ($error): ?>
            <div><?php echo $error->getMessage() ?></div>
        <?php endif; ?>

        <form action="<?php echo $view['router']->generate('login_check') ?>" method="post">
            <label for="username">Username:</label>
            <input type="text" id="username"
                   name="_username" value="<?php echo $last_username ?>" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            <input type="checkbox" id="remember_me" name="_remember_me" checked />
            <label for="remember_me">Keep me logged in</label>

            <input type="submit" name="login" />
        </form>

L'utilisateur va donc être connecté automatiquement lors de ses prochaines
visites tant que le cookie restera valide.

Forcer l'utilisateur à se ré-authentifier avant d'accéder à certaines ressources
--------------------------------------------------------------------------------

Lorsque l'utilisateur retourne sur votre site, il ou elle est authentifié
automatiquement en se basant sur les informations stockées dans le cookie
« remember me ». Cela permet à l'utilisateur d'accéder à des ressources protégées
comme si l'utilisateur s'était authentifié lors de sa visite sur le site.

Cependant, dans certains cas, vous pourriez vouloir forcer l'utilisateur à se
ré-authentifier avant d'accéder à certains ressources. Par exemple, vous pourriez
autoriser un utilisateur avec un cookie « remember me » à voir les informations
basiques de son compte, mais par contre vous pourriez lui imposer de se
ré-authentifier avant de modifier cette information.

Le composant de sécurité fournit une manière simple de faire cela. En plus
des rôles qui leurs sont explicitement assignés, les utilisateurs possèdent
automatiquement l'un des rôles suivants dépendant de la manière dont ils sont
authentifiés :

* ``IS_AUTHENTICATED_ANONYMOUSLY`` - automatiquement assigné à un utilisateur
  qui se trouve dans une zone protégée du site par un pare-feu mais qui ne s'est
  pas connecté/loggué. Cela est possible uniquement si l'accès anonyme a été
  autorisé.

* ``IS_AUTHENTICATED_REMEMBERED`` - automatiquement assigné à un utilisateur
  qui a été authentifié via un cookie « remember me ».

* ``IS_AUTHENTICATED_FULLY`` - automatiquement assigné à un utilisateur qui
  a fourni ses informations de login durant la session courante.

Vous pouvez utiliser ces rôles pour contrôler l'accès en plus des autres
rôles explicitement assignés.

.. note::

    Si vous avez le rôle ``IS_AUTHENTICATED_REMEMBERED``, alors vous avez
    aussi le rôle ``IS_AUTHENTICATED_ANONYMOUSLY``. Si vous avez le rôle
    ``IS_AUTHENTICATED_FULLY``, alors vous possédez aussi les deux autres
    rôles. En d'autres termes, ces rôles représentent trois niveaux croissants
    de « force » d'authentification.

Vous pouvez utiliser ces rôles additionnels pour effectuer un contrôle d'une
granularité plus fine sur l'accès à certaines parties d'un site. Par exemple,
vous pourriez souhaiter que votre utilisateur soit capable de voir son compte en
se rendant à ``/account`` lorsqu'il est authentifié par cookie, mais qu'il
doive fournir ses informations de login pour pouvoir éditer les détails de son
compte. Vous pouvez effectuer ceci en sécurisant certaines actions d'un contrôleur
spécifique en utilisant ces rôles. L'action « edit » dans le contrôleur
pourrait être sécurisée en utilisant le contexte du service.

Dans l'exemple suivant, l'action est autorisée seulement si l'utilisateur
possède le rôle ``IS_AUTHENTICATED_FULLY``.

.. code-block:: php

    use Symfony\Component\Security\Core\Exception\AccessDeniedException
    // ...

    public function editAction()
    {
        if (false === $this->get('security.context')->isGranted(
            'IS_AUTHENTICATED_FULLY'
        )) {
            throw new AccessDeniedException();
        }

        // ...
    }

Vous pouvez aussi choisir d'installer et d'utiliser le bundle optionnel
JMSSecurityExtraBundle_ qui peut sécuriser votre contrôleur en utilisant
des annotations :

.. code-block:: php

    use JMS\SecurityExtraBundle\Annotation\Secure;

    /**
     * @Secure(roles="IS_AUTHENTICATED_FULLY")
     */
    public function editAction($name)
    {
        // ...
    }

.. tip::

    Si vous aviez aussi un contrôle d'accès dans votre configuration de
    sécurité qui requiert qu'un utilisateur possède un rôle ``ROLE_USER``
    afin d'accéder à n'importe quelle partie de la zone « account », alors
    vous auriez la situation suivante :

    * Si un utilisateur non-authentifié (ou authentifié anonymement) essaye
      d'accéder à la zone « account », il sera demandé à cet utilisateur de
      s'authentifier.

    * Une fois que l'utilisateur a entré son nom d'utilisateur et son mot de
      passe, et en supposant que l'utilisateur recoive le rôle ``ROLE_USER``
      par votre configuration, ce dernier aura le rôle ``IS_AUTHENTICATED_FULLY``
      et sera capable d'accéder à n'importe quelle page de la section
      « account », incluant l'action ``editAction`` du contrôleur.

    * Enfin, supposons que la session de l'utilisateur se termine ; quand ce dernier
      retourne sur le site, il sera capable d'accéder à chaque page de la partie
      « account » - exceptée la page « edit » - sans être obligé de se
      ré-authentifier. Cependant, quand il essaye d'accéder à l'action ``editAction``
      du contrôleur, il sera obligé de se ré-authentifier, puisqu'il n'est pas
      (encore) totalement authentifié.

Pour plus d'informations sur la sécurisation de services ou de méthodes de cette
manière, lisez :doc:`/cookbook/security/securing_services`.

.. _JMSSecurityExtraBundle: https://github.com/schmittjoh/JMSSecurityExtraBundle
