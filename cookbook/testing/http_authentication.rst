.. index::
   single: Tests; HTTP authentication

Comment simuler une authentification HTTP dans un Test Fonctionnel
==================================================================

Si votre application requiert une authentification HTTP, vous pouvez transmettre
le nom d'utilisateur et le mot de passe comme variable serveurs à la méthode
``createClient()``::

    $client = static::createClient(array(), array(
        'PHP_AUTH_USER' => 'username',
        'PHP_AUTH_PW'   => 'pa$$word',
    ));

Vous pouvez aussi les outrepasser directement dans l'objet requête::

    $client->request('DELETE', '/post/12', array(), array(), array(
        'PHP_AUTH_USER' => 'username',
        'PHP_AUTH_PW'   => 'pa$$word',
    ));

Quand votre application utilise un ``form_login``, vous pouvez effectuer vos tests
plus simplement en permettant à la configuration de test d'utiliser
l'authentification HTTP. De cette manière, vous pouvez utiliser le code décrit
ci-dessus pour vous authentifier dans les tests, mais néanmoins conserver le fait
que vos utilisateurs doivent se connecter via l'usuel ``form_login``.
Pour cela, dans la configuration de test vous devez inclure la clé
``http_basic`` dans votre pare-feu Symfony, à l'intérieur du conteneur traitant
le ``form_login`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        security:
            firewalls:
                your_firewall_name:
                    http_basic:
