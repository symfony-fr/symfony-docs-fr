.. index::
   single: Security; Force HTTPS

Comment forcer HTTPS ou HTTP pour des URLs Différentes
======================================================

Vous pouvez forcer certaines parties de votre site à utiliser le protocole ``HTTPS``
dans la configuration de la sécurité. Cela s'effectue grâce aux règles
``access_control`` en utilisant l'option ``requires_channel``. Par exemple, si vous
voulez forcer toutes les URLs commençant par ``/secure`` à utiliser ``HTTPS``, alors
vous pourriez utiliser la configuration suivante :

.. configuration-block::

        .. code-block:: yaml

            access_control:
                - path: ^/secure
                  roles: ROLE_ADMIN
                  requires_channel: https

        .. code-block:: xml

            <access-control>
                <rule path="^/secure" role="ROLE_ADMIN" requires_channel="https" />
            </access-control>

        .. code-block:: php

            'access_control' => array(
                array('path' => '^/secure', 
                      'role' => 'ROLE_ADMIN', 
                      'requires_channel' => 'https'
                ),
            ),

Le formulaire de login lui-même a besoin d'autoriser un accès anonyme, sinon
les utilisateurs seront incapables de s'authentifier. Pour le forcer à utiliser
``HTTPS`` vous pouvez toujours utiliser les règles de ``access_control`` en
vous servant du rôle ``IS_AUTHENTICATED_ANONYMOUSLY`` :

.. configuration-block::

        .. code-block:: yaml

            access_control:
                - path: ^/login
                  roles: IS_AUTHENTICATED_ANONYMOUSLY
                  requires_channel: https

        .. code-block:: xml

            <access-control>
                <rule path="^/login" 
                      role="IS_AUTHENTICATED_ANONYMOUSLY" 
                      requires_channel="https" />
            </access-control>

        .. code-block:: php

            'access_control' => array(
                array('path' => '^/login', 
                      'role' => 'IS_AUTHENTICATED_ANONYMOUSLY', 
                      'requires_channel' => 'https'
                ),
            ),

Il est aussi possible de spécifier l'utilisation d'``HTTPS`` dans la
configuration de routage, lisez :doc:`/cookbook/routing/scheme` pour
plus de détails.
