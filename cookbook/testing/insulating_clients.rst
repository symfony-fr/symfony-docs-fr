.. index::
   single: Tests

Comment tester les interactions de multiples clients
=====================================================

Si vous avez besoin de simuler des interaction entre différents clients (pour
un tchat par example), créer plusieurs clients::

    $harry = static::createClient();
    $sally = static::createClient();

    $harry->request('POST', '/say/sally/Hello');
    $sally->request('GET', '/messages');

    $this->assertEquals(201, $harry->getResponse()->getStatusCode());
    $this->assertRegExp('/Hello/', $sally->getResponse()->getContent());

Cependant cela ne fonctionnera que si vous ne maintenez pas dans votre application
un état global et si aucune des bibliothèques tierces en liaison n'utilise de
techniques requierant des états globaux. Dans ces cas vous devrez isoler les clients::

    $harry = static::createClient();
    $sally = static::createClient();

    $harry->insulate();
    $sally->insulate();

    $harry->request('POST', '/say/sally/Hello');
    $sally->request('GET', '/messages');

    $this->assertEquals(201, $harry->getResponse()->getStatusCode());
    $this->assertRegExp('/Hello/', $sally->getResponse()->getContent());

L'isolation est invisible pour chacun des clients qui executent leurs requêtes
respectives dans un processus PHP dédié, vous garantissant de tout effet de
bord.

.. tip::

    Comme un client isolé est plus lent, vous pouvez garder un client dans le
	processus principal et n'isoler que les autres.
