.. index::
   single: Tests; Profiling

Comment utiliser le Profiler dans un test fonctionnel
=====================================================

Il est grandement recommandé qu'un test fonctionnel ne teste que l'objet Response.
Cependant si vous écrivez des tests qui surveillent vos serveurs de productions, vous
désirerez peut-être écrire des tests sur les données provenant du profiler. En effet
ce dernier fournit des méthodes efficaces, permettant de contrôler de nombreux
comportements et d'appliquer certaines métriques.

Le Profiler de Symfony2 :ref:`Profiler <internals-profiler>` collecte de nombreuses
informations à chaque requête. Utilisez celle-ci pour vérifier le nombre d'appels
à la base de données, le temps passé dans l'exécution du framework, ...

Avant de pouvoir écrire des assertions, vous devez toujours vous assurez que le
profiler est disponible (il est activé par défaut dans l'environnement de ``test`` )::

    class HelloControllerTest extends WebTestCase
    {
        public function testIndex()
        {
            $client = static::createClient();
            $crawler = $client->request('GET', '/hello/Fabien');

            // Ecrire des assertions concernant la réponse
            // ...

            // Vérifier que le profiler est activé
            if ($profile = $client->getProfile()) {
                // vérifier le nombre de requêtes
                $this->assertTrue($profile->getCollector('db')->getQueryCount() < 10);

                // Vérifier le temps utilisé par le framework
                $this->assertTrue( $profile->getCollector('timer')->getTime() < 0.5);
            }
        }
    }

Si un test échoue à cause des données du profilage (trop d'appels à la base de données
par exemple), vous pouvez utiliser le Profiler Web afin d’analyser la requête après que
les tests soient terminés. Cela est réalisable simplement en intégrant le token suivant
dans les messages d'erreurs::

    $this->assertTrue(
        $profile->get('db')->getQueryCount() < 30,
        sprintf('Checks that query count is less than 30 (token %s)', $profile->getToken())
    );

.. caution::

     Le profiler conserve des données différentes suivant l'environnement utilisé
     (particulièrement si vous utiliser SQLite, ce qui est la configuration par
     défaut).

.. note::

    Les informations du profiler sont disponibles même si vous isolez un client
    ou utilisez une couche HTTP particulière pour vos tests.

.. tip::

    L'API intégrée vous permet de connaitre les méthodes et les interfaces disponibles :
    :doc:`data collectors</cookbook/profiler/data_collector>`.
