.. index::
   single: Tests; HTTP Authentication

Comment simuler une authentification HTTP dans un test fonctionnel
==================================================================

Si votre application requiert une authentification HTTP, vous pouvez transmettre
le nom d'utilisateur et le mot de passe comme variable serveurs à la méthode  ``createClient()``::

    $client = static::createClient(array(), array(
        'PHP_AUTH_USER' => 'username',
        'PHP_AUTH_PW'   => 'pa$$word',
    ));

Vous pouvez aussi les surcharger directement dans l'objet requête ::

    $client->request('DELETE', '/post/12', array(), array(
        'PHP_AUTH_USER' => 'username',
        'PHP_AUTH_PW'   => 'pa$$word',
    ));

Quand votre application utilise une ``formulaire de connexion``, san permettre
l'authentification HTTP, vous pouvez effectuer vos tests plus simplement en 
permettant à la configuration de test d'utiliser l'authentification HTTP. De
cette manière vous l'authentification HTTP sera disponible dans de vos tests 
tout en conservant le formulaire de connexion pour les utilisateurs réels.
Pour cela, dans la configuration de test vous devez inclure la clef
``http_basic``dans votre pare-feu Symfony, à l'intérieur du conteneur traitant 
le ``formulaire de connexion``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        security:
            firewalls:
                ``firewall_du_formulaire``:
                    http_basic:
