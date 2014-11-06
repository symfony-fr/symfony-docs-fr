.. index::
    single: Security; Impersonating User

Comment se faire passer pour un utilisateur
=========================

Il est parfois utile de pouvoir passer d’un utilisateur à un autre sans avoir
à se déconnecter et se reconnecter (par exemple pour résoudre un bug, 
ou pour comprendre un bug que rencontre un utilisateur et que vous n’arrivez pas à reproduire).
Cette fonctionnalité peut être mise en place très simplement
grâce au paramètre ``switch_user`` de votre pare-feu :

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    # ...
                    switch_user: true

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">
            <config>
                <firewall>
                    <!-- ... -->
                    <switch-user />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main'=> array(
                    // ...
                    'switch_user' => true
                ),
            ),
        ));

Pour changer d’utilisateur, ajoutez simplement ``_switch_user``
en tant que paramètre de requête de votre URL suivi du nom d’utilisateur :

.. code-block:: text

    http://example.com/somewhere?_switch_user=thomas

To switch back to the original user, use the special ``_exit`` username:

.. code-block:: text

    http://example.com/somewhere?_switch_user=_exit

Durant l’imposture, un rôle ``ROLE_PREVIOUS_ADMIN`` est fournit à l’utilisateur. 
Ce rôle peut, par exemple, être utile pour afficher un lien pour quitter l’imposture depuis un template :

.. configuration-block::

    .. code-block:: html+jinja

        {% if is_granted('ROLE_PREVIOUS_ADMIN') %}
            <a href="{{ path('homepage', {'_switch_user': '_exit'}) }}">Exit impersonation</a>
        {% endif %}

    .. code-block:: html+php

        <?php if ($view['security']->isGranted('ROLE_PREVIOUS_ADMIN')): ?>
            <a
                href="<?php echo $view['router']->generate('homepage', array(
                    '_switch_user' => '_exit',
                ) ?>"
            >
                Exit impersonation
            </a>
        <?php endif; ?>

Cette fonctionnalité doit bien sur n’être accessible qu’à un groupe restreint d’utilisateurs.
Par défaut, seuls les utilisateurs ayant le rôle ``ROLE_ALLOWED_TO_SWITCH`` peuvent y accéder.
Vous pouvez changer le nom de ce rôle dans les réglages ``role``.
Pour plus de sécurité, vous pouvez aussi modifier le nom du paramètre de requête
directement via l'option ``parameter`` de votre pare-feu :

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    # ...
                    switch_user: { role: ROLE_ADMIN, parameter: _want_to_be_this_user }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">
            <config>
                <firewall>
                    <!-- ... -->
                    <switch-user role="ROLE_ADMIN" parameter="_want_to_be_this_user" />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main'=> array(
                    // ...
                    'switch_user' => array(
                        'role' => 'ROLE_ADMIN',
                        'parameter' => '_want_to_be_this_user',
                    ),
                ),
            ),
        ));
