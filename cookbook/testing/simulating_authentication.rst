.. index::
   single: Tests; Simuler une authentification

Comment simuler une authentification avec un token dans un test fonctionnel
===========================================================================

Les requêtes d'authentification dans les tests fonctionnels peuvent
ralentir la suite de tests. Cela peut devenir un problème particulièrement
lorsque le ``form_login`` est utilisé, puisque cela requiert des requêtes
additionnelles pour remplir et soumettre le formulaire.

L'une des solutions est de configurer votre firewall à utiliser ``http_basic``
en environnement de test comme expliqué dans :doc:`/cookbook/testing/http_authentication`.
Une autre façon de faire serait de créer un token vous-même  et de le stocker
dans une session. En faisant cela, vous devez vous assurer que le cookie approprié
est envoyé avec la requête. L'exemple suivant vous montre comment faire ::

    // src/Acme/DemoBundle/Tests/Controller/DemoControllerTest.php
    namespace Acme\DemoBundle\Tests\Controller;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;
    use Symfony\Component\BrowserKit\Cookie;
    use Symfony\Component\Security\Core\Authentication\Token\UsernamePasswordToken;

    class DemoControllerTest extends WebTestCase
    {
        private $client = null;

        public function setUp()
        {
            $this->client = static::createClient();
        }

        public function testSecuredHello()
        {
            $this->logIn();

            $this->client->request('GET', '/demo/secured/hello/Fabien');

            $this->assertTrue($this->client->getResponse()->isSuccessful());
            $this->assertGreaterThan(0, $crawler->filter('html:contains("Hello Fabien")')->count());
        }

        private function logIn()
        {
            $session = $this->client->getContainer()->get('session');

            $firewall = 'secured_area';
            $token = new UsernamePasswordToken('admin', null, $firewall, array('ROLE_ADMIN'));
            $session->set('_security_'.$firewall, serialize($token));
            $session->save();

            $cookie = new Cookie($session->getName(), $session->getId());
            $this->client->getCookieJar()->set($cookie);
        }
    }

.. note::

    La technique décrite dans :doc:`/cookbook/testing/http_authentication`
    est plus propre et de ce fait préférée.
