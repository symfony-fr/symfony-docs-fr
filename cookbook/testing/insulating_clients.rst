.. index::
   single: Tests

Comment tester les interactions de multiples clients
====================================================

Si vous avez besoin de simuler des interaction entre différents clients (pour
un « chat » par exemple), créez plusieurs clients::

    $harry = static::createClient();
    $sally = static::createClient();

    $harry->request('POST', '/say/sally/Hello');
    $sally->request('GET', '/messages');

    $this->assertEquals(201, $harry->getResponse()->getStatusCode());
    $this->assertRegExp('/Hello/', $sally->getResponse()->getContent());

Cependant cela ne fonctionnera que si vous ne maintenez pas dans votre application
un état global et si aucune des bibliothèques tierces n'utilise de techniques requérant
des états globaux. Dans ces cas vous devrez isoler les clients::

    $harry = static::createClient();
    $sally = static::createClient();

    $harry->insulate();
    $sally->insulate();

    $harry->request('POST', '/say/sally/Hello');
    $sally->request('GET', '/messages');

    $this->assertEquals(201, $harry->getResponse()->getStatusCode());
    $this->assertRegExp('/Hello/', $sally->getResponse()->getContent());

Insulated clients transparently execute their requests in a dedicated and
clean PHP process, thus avoiding any side-effects.

Les clients isolés exécute de manière transparente leur requête dans un processus
PHP dédié et « sain », et évitent donc quelconque effet de bord.

.. tip::

    Comme un client isolé est plus lent, vous pouvez garder un client dans le
    processus principal et n'isoler que les autres.
