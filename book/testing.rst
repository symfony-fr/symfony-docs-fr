.. index::
   single: Tests

Testing
=======

A chaque fois que vous écrivez une nouvelle ligne de code, vous ajoutez aussi
potentiellement de nouveaux bugs. Les tests automatisés devraient vous couvrir
et ce tutoriel vous montre comment écrire des tests unitaires et fonctionnels
pour votre application Symfony2.

Le Framework de Test
--------------------

Les tests Symfony2 s'appuient fortement sur PHPUnit, ses meilleures pratiques,
et quelques conventions. Cette partie ne documente pas PHPUnit lui-même, mais
si vous ne le connaissez pas encore, vous pouvez lire son excellente
`documentation`_.

.. note::

    Symfony2 fonctionne avec PHPUnit 3.5.11 ou plus.

La configuration par défaut de PHPUnit cherche les tests dans le sous-répertoire
``Tests/`` de vos bundles:

.. code-block:: xml

    <!-- app/phpunit.xml.dist -->

    <phpunit bootstrap="../src/autoload.php">
        <testsuites>
            <testsuite name="Project Test Suite">
                <directory>../src/*/*Bundle/Tests</directory>
            </testsuite>
        </testsuites>

        ...
    </phpunit>

Exécuter la suite de test pour une application donnée est très simple:

.. code-block:: bash

    # spécifiez le répertoire de configuration sur la ligne de commande
    $ phpunit -c app/

    # ou exécutez phpunit depuis l'intérieur du répertoire de l'application
    $ cd app/
    $ phpunit

.. tip::

    La couverture du code peut être générée avec l'option ``--coverage-html``.

.. index::
   single: Tests; Tests Unitaires

Tests Unitaires
---------------

Ecrire des tests unitaires Symfony2 n'est en aucun cas différent que d'écrire
des tests unitaires standards PHPUnit. Par convention, il est recommandé de
reproduire la structure du répertoire du bundle dans son sous-répertoire ``Tests/``.
Donc, écrivez les tests pour la classe ``Acme\HelloBundle\Model\Article`` dans le
fichier ``Acme/HelloBundle/Tests/Model/ArticleTest.php``.

Dans un test unitaire, l'autoloading est activé automatiquement via le fichier
``src/autoload.php`` (comme configuré par défaut dans le fichier ``phpunit.xml.dist``).

Exécuter les tests pour un fichier ou répertoire donné est aussi très facile:

.. code-block:: bash

    # exécute tous les tests pour le contrôleur
    $ phpunit -c app src/Acme/HelloBundle/Tests/Controller/

    # exécute tous les tests pour le modèle
    $ phpunit -c app src/Acme/HelloBundle/Tests/Model/

    # exécute les tests pour la classe Article
    $ phpunit -c app src/Acme/HelloBundle/Tests/Model/ArticleTest.php

    # exécute tous les tests pour le Bundle entier
    $ phpunit -c app src/Acme/HelloBundle/

.. index::
   single: Tests; Tests Fonctionnels

Tests Fonctionnels
------------------

Les tests fonctionnels vérifient l'intégration des différentes couches d'une
application (du routing jusqu'aux vues). Ils ne sont pas différents des tests
unitaires tant que PHPUnit est concerné, mais ils possèdent un workflow très
spécifique:

* Faire une requête;
* Tester la réponse;
* Cliquer sur un lien ou soumettre un formulaire;
* Tester la réponse;
* Recommencer ainsi de suite.

Les requêtes, les clics, et les envois sont effectués par un client qui sait
comment parler à l'application. Pour accéder à un tel client, vos tests ont
besoin d'étendre la classe Symfony2 ``WebTestCase``. La Standard Edition de
Symfony2 fournit un test fonctionnel pour le ``DemoController` qui se lit
comme suit::

    // src/Acme/DemoBundle/Tests/Controller/DemoControllerTest.php
    namespace Acme\DemoBundle\Tests\Controller;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;

    class DemoControllerTest extends WebTestCase
    {
        public function testIndex()
        {
            $client = static::createClient();

            $crawler = $client->request('GET', '/demo/hello/Fabien');

            $this->assertTrue($crawler->filter('html:contains("Hello Fabien")')->count() > 0);
        }
    }

La méthode ``createClient()`` retourne un client lié à l'application courante::

    $crawler = $client->request('GET', 'hello/Fabien');

La méthode ``request()`` retourne un objet ``Crawler`` qui peut être utilisé pour
sélectionner des éléments dans la Réponse, cliquer des liens, et soumettre
des formulaires.

.. tip::

    Le Crawler peut être utilisé seulement si le contenu de la Réponse est un
    document XML ou HTML. Pour les autres types de contenu, vous pouvez obtenir
    le contenu de la Réponse avec ``$client->getResponse()->getContent()``.

Cliquez sur un lien en le sélectionnant en premier avec le Crawler en utilisant
soit une expression XPath ou un sélecteur CSS, puis utilisez le Client pour
cliquer dessus::

    $link = $crawler->filter('a:contains("Greet")')->eq(1)->link();

    $crawler = $client->click($link);

Soumettre un form est très similaire; sélectionnez un bouton de formulaire,
optionnellement ré-écrivez quelques valeurs du formulaires, et soumettez le
formulaire correspondant::

    $form = $crawler->selectButton('submit')->form();

    // définit quelques valeurs
    $form['name'] = 'Lucas';

    // soumet le formulaire
    $crawler = $client->submit($form);

Chaque champ de ``Formulaire`` possède des méthodes spécialisées dépendant
de son type::

    // remplit un champ de saisie
    $form['name'] = 'Lucas';

    // sélectionne une option/radio
    $form['country']->select('France');

    // coche une case
    $form['like_symfony']->tick();

    // upload un fichier
    $form['photo']->upload('/path/to/lucas.jpg');

Plutôt que de changer un champ à la fois, vous pouvez aussi passer un tableau
de valeurs à la méthode ``submit()``::

    $crawler = $client->submit($form, array(
        'name'         => 'Lucas',
        'country'      => 'France',
        'like_symfony' => true,
        'photo'        => '/path/to/lucas.jpg',
    ));

Maintenant que vous pouvez naviguer aisément à travers une application, utilisez
les assertions pour tester qu'elle fait effectivement ce que vous souhaitez
qu'elle fasse. Utilisez le Crawler pour faire des assertions sur le DOM::

    // affirme que la réponse correspond au sélecteur CSS donné
    $this->assertTrue($crawler->filter('h1')->count() > 0);

Ou, testez directement par rapport au contenu de la Réponse si vous voulez
juste vérifier que ce dernier contient un certain morceau de texte, ou si
la Réponse n'est pas un document XML/HTML::

    $this->assertRegExp('/Hello Fabien/', $client->getResponse()->getContent());

.. index::
   single: Tests; Assertions

Assertions Utiles
~~~~~~~~~~~~~~~~~

Après quelques temps, vous remarquerez que vous écrivez toujours la même sorte
d'assertion. Afin que vous démarriez plus rapidement, voici une liste des
assertions les plus communes et utiles::

    // affirme que la réponse correspond à un sélecteur CSS donné
    $this->assertTrue($crawler->filter($selector)->count() > 0);

    // affirme que la réponse correspond n fois à un sélecteur CSS donné
    $this->assertEquals($count, $crawler->filter($selector)->count());

    // affirme que l'en-tête de la réponse possède la valeur donnée
    $this->assertTrue($client->getResponse()->headers->contains($key, $value));

    // affirme que le contenu de la réponse correspond à une expression régulière donnée
    $this->assertRegExp($regexp, $client->getResponse()->getContent());

    // affirme que le code de statut de la réponse correspond à celui donné
    $this->assertTrue($client->getResponse()->isSuccessful());
    $this->assertTrue($client->getResponse()->isNotFound());
    $this->assertEquals(200, $client->getResponse()->getStatusCode());

    // affirme que le code de statut de la réponse est une redirection
    $this->assertTrue($client->getResponse()->isRedirected('google.com'));

.. _documentation: http://www.phpunit.de/manual/3.5/en/

.. index::
   single: Tests; Client

Le Client Test
--------------

Le Client test simule un client HTTP comme un navigateur.

.. note::

    Le Client test est basé sur les composants ``BrowserKit` et ``Crawler``.

Faire des requêtes
~~~~~~~~~~~~~~~~~~

Le client sait comment effectuer des requêtes à une application Symfony2::

    $crawler = $client->request('GET', '/hello/Fabien');

La méthode ``request()`` prend en arguments la méthode HTTP et une URL et
retourne une instance de ``Crawler``.

Utilisez le Crawler pour trouver des éléments DOM dans la Réponse. Ces éléments
peuvent ainsi être utilisé pour cliquer sur des liens et soumettre des formulaires::

    $link = $crawler->selectLink('Go elsewhere...')->link();
    $crawler = $client->click($link);

    $form = $crawler->selectButton('validate')->form();
    $crawler = $client->submit($form, array('name' => 'Fabien'));

Les méthodes ``click()`` et ``submit()`` retournent toutes deux un objet
``Crawler``. Ces méthodes sont le meilleur moyen de naviguer au travers d'une
application car cela masque beaucoup de détails. Par exemple, quand vous
soumettez un formulaire, il détecte automatiquement la méthode HTTP et l'URL
du formulaire, il vous fournit une API pour uploader des fichiers,
et il fusionne les valeurs soumises avec celles par défaut du formulaire,
et plus encore.

.. tip::

    Vous en apprendrez davantage sur les objets ``Link`` et ``Form`` dans la
    section Crawler ci-dessous.

Mais vous pouvez aussi simuler les soumissions de formulaires et des requêtes
complexes à l'aide des arguments additionnels de la méthode ``request()``::

    // soumission de formulaire
    $client->request('POST', '/submit', array('name' => 'Fabien'));

    // soumission de formulaire avec un upload de fichier
    $client->request('POST', '/submit', array('name' => 'Fabien'), array('photo' => '/path/to/photo'));

    // spécifie les en-têtes HTTP
    $client->request('DELETE', '/post/12', array(), array(), array('PHP_AUTH_USER' => 'username', 'PHP_AUTH_PW' => 'pa$$word'));

Quand une requête retourne une redirection en tant que réponse, le client la
suit automatiquement. Ce comportement peut être changé via la méthode
``followRedirects()``::

    $client->followRedirects(false);

Quand le client ne suit pas les redirections, vous pouvez le forcer grâce à la
méthode ``followRedirect()``::

    $crawler = $client->followRedirect();

Enfin, et non des moindres, vous pouvez forcer chaque requête à être exécutée
dans son propre processus PHP afin d'éviter quelconques effets secondaires
quand vous travaillez avec plusieurs clients dans le même script::

    $client->insulate();

Naviguer
~~~~~~~~

Le Client supporte de nombreuses opérations qui peuvent être effectuées
à travers un navigateur réel::

    $client->back();
    $client->forward();
    $client->reload();

    // efface tous les cookies et l'historique
    $client->restart();

Accéder aux Objets Internes
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous utilisez le client pour tester votre application, vous pourriez vouloir
accéder aux objets internes du client::

    $history   = $client->getHistory();
    $cookieJar = $client->getCookieJar();

Vous pouvez aussi obtenir les objets liés à la dernière requête::

    $request  = $client->getRequest();
    $response = $client->getResponse();
    $crawler  = $client->getCrawler();

Si vos requêtes ne sont pas isolées, vous pouvez aussi accéder au ``Container``
et au ``Kernel``::

    $container = $client->getContainer();
    $kernel    = $client->getKernel();

Accéder au Container
~~~~~~~~~~~~~~~~~~~~

Il est fortement recommandé qu'un test fonctionnel teste seulement la Réponse.
Mais dans certaines rares circonstances, vous pourriez vouloir accéder à
quelques objets internes pour écrire des assertions. Dans tels cas, vous
pouvez accéder au conteneur d'injection des dépendances::

    $container = $client->getContainer();

Soyez averti que cela ne fonctionne pas si vous isolez le client ou si vous
utilisez une couche HTTP.

.. tip::

    Si les informations que vous avez besoin de vérifier sont disponibles via le
    profileur, utilisez plutôt ces dernières à la place.

Accéder aux données du profileur
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour vérifier les données collectées par le profileur, vous pouvez obtenir
le profil pour la requête courante comme ceci::

    $profile = $client->getProfile();

Redirection
~~~~~~~~~~~

Par défaut, le Client ne suit pas les redirections HTTP, de sorte que vous
puissiez obtenir et examiner la Réponse avant la redirection. Une fois que
vous voulez que le client soit redirigé, appelez la méthode ``followRedirect()``::

    // faites quelque chose qui cause une redirection (ex: remplir un formulaire)

    // suit la redirection
    $crawler = $client->followRedirect();

Si vous souhaitez que le Client soit toujours redirigé automatiquement, vous
pouvez appeler la méthode ``followRedirects()``::

    $client->followRedirects();

    $crawler = $client->request('GET', '/');

    // toutes les redirections sont suivies

    // reconfigure le client en mode redirection manuelle
    $client->followRedirects(false);

.. index::
   single: Tests; Crawler

Le Crawler
-----------

Une instance de Crawler est retournée chaque fois que vous effectuez une requête
avec le Client. Elle vous permet de naviguer à travers des documents HTML, de
sélectionner des noeuds, de trouver des liens et des formulaires.

Créer une instance de Crawler
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Une instance de Crawler est automatiquement créée pour vous quand vous effectuez
une requête avec un Client. Mais vous pouvez créer la vôtre facilement::

    use Symfony\Component\DomCrawler\Crawler;

    $crawler = new Crawler($html, $url);

Le constructeur prend deux arguments: le second est une URL qui est utilisée pour
générer des URLs absolues pour les liens et les formulaires; la première peut être
n'importe lequel des éléments suivants:

* Un document HTML;
* Un document XML;
* Une instance de ``DOMDocument``
* Une instance de ``DOMNodeList``
* Une instance de ``DOMNode``
* Un tableau composé des éléments ci-dessus.

Après création, vous pouvez ajouter plus de noeuds:

+-----------------------+-------------------------------------------------+
| Méthode               | Description                                     |
+=======================+=================================================+
| ``addHTMLDocument()`` | Un document HTML                                |
+-----------------------+-------------------------------------------------+
| ``addXMLDocument()``  | Un document XML                                 |
+-----------------------+-------------------------------------------------+
| ``addDOMDocument()``  | Une instance de ``DOMDocument``                 |
+-----------------------+-------------------------------------------------+
| ``addDOMNodeList()``  | Une instance de ``DOMNodeList``                 |
+-----------------------+-------------------------------------------------+
| ``addDOMNode()``      | Une instance de ``DOMNode``                     |
+-----------------------+-------------------------------------------------+
| ``addNodes()``        | Un tableau composé des éléments ci-dessus       |
+-----------------------+-------------------------------------------------+
| ``add()``             | Accepte n'importe lequel des éléments ci-dessus |
+-----------------------+-------------------------------------------------+

Traverser
~~~~~~~~~

Comme jQuery, le Crawler possède des méthodes lui permettant de naviguer à travers
le DOM d'un document HTML/XML:

+-----------------------+-----------------------------------------------------+
| Méthode               | Description                                         |
+=======================+=====================================================+
| ``filter('h1')``      | Noeuds qui correspondent au sélecteur CSS           |
+-----------------------+-----------------------------------------------------+
| ``filterXpath('h1')`` | Noeuds qui correspondent à l'expression XPath       |
+-----------------------+-----------------------------------------------------+
| ``eq(1)``             | Noeud pour l'index spéficié                         |
+-----------------------+-----------------------------------------------------+
| ``first()``           | Premier noeud                                       |
+-----------------------+-----------------------------------------------------+
| ``last()``            | Dernier noeud                                       |
+-----------------------+-----------------------------------------------------+
| ``siblings()``        | Frères et soeurs                                    |
+-----------------------+-----------------------------------------------------+
| ``nextAll()``         | Tous les frères et soeurs suivants                  |
+-----------------------+-----------------------------------------------------+
| ``previousAll()``     | Tous les frères et soeurs précédents                |
+-----------------------+-----------------------------------------------------+
| ``parents()``         | Noeuds parents                                      |
+-----------------------+-----------------------------------------------------+
| ``children()``        | Noeuds enfants                                      |
+-----------------------+-----------------------------------------------------+
| ``reduce($lambda)``   | Noeuds pour lesquels la lambda ne retourne pas faux |
+-----------------------+-----------------------------------------------------+

Vous pouvez affiner de manière itérative votre sélection de noeuds en enchaînant les
appels de méthodes car chaque méthode retourne une nouvelle instance de Crawler
pour les noeuds correspondants::

    $crawler
        ->filter('h1')
        ->reduce(function ($node, $i)
        {
            if (!$node->getAttribute('class')) {
                return false;
            }
        })
        ->first();

.. tip::

    Utilisez la fonction ``count()`` pour obtenir le nombre de noeuds stockés
    dans un Crawler: ``count($crawler)``.

Extraction d'informations
~~~~~~~~~~~~~~~~~~~~~~~~~

The Crawler can extract information from the nodes::

    // Returns the attribute value for the first node
    $crawler->attr('class');

    // Returns the node value for the first node
    $crawler->text();

    // Extracts an array of attributes for all nodes (_text returns the node value)
    $crawler->extract(array('_text', 'href'));

    // Executes a lambda for each node and return an array of results
    $data = $crawler->each(function ($node, $i)
    {
        return $node->getAttribute('href');
    });

Links
~~~~~

You can select links with the traversing methods, but the ``selectLink()``
shortcut is often more convenient::

    $crawler->selectLink('Click here');

It selects links that contain the given text, or clickable images for which
the ``alt`` attribute contains the given text.

The Client ``click()`` method takes a ``Link`` instance as returned by the
``link()`` method::

    $link = $crawler->link();

    $client->click($link);

.. tip::

    The ``links()`` method returns an array of ``Link`` objects for all nodes.

Forms
~~~~~

As for links, you select forms with the ``selectButton()`` method::

    $crawler->selectButton('submit');

Notice that we select form buttons and not forms as a form can have several
buttons; if you use the traversing API, keep in mind that you must look for a
button.

The ``selectButton()`` method can select ``button`` tags and submit ``input``
tags; it has several heuristics to find them:

* The ``value`` attribute value;

* The ``id`` or ``alt`` attribute value for images;

* The ``id`` or ``name`` attribute value for ``button`` tags.

When you have a node representing a button, call the ``form()`` method to get a
``Form`` instance for the form wrapping the button node::

    $form = $crawler->form();

When calling the ``form()`` method, you can also pass an array of field values
that overrides the default ones::

    $form = $crawler->form(array(
        'name'         => 'Fabien',
        'like_symfony' => true,
    ));

And if you want to simulate a specific HTTP method for the form, pass it as a
second argument::

    $form = $crawler->form(array(), 'DELETE');

The Client can submit ``Form`` instances::

    $client->submit($form);

The field values can also be passed as a second argument of the ``submit()``
method::

    $client->submit($form, array(
        'name'         => 'Fabien',
        'like_symfony' => true,
    ));

For more complex situations, use the ``Form`` instance as an array to set the
value of each field individually::

    // Change the value of a field
    $form['name'] = 'Fabien';

There is also a nice API to manipulate the values of the fields according to
their type::

    // Select an option or a radio
    $form['country']->select('France');

    // Tick a checkbox
    $form['like_symfony']->tick();

    // Upload a file
    $form['photo']->upload('/path/to/lucas.jpg');

.. tip::

    You can get the values that will be submitted by calling the ``getValues()``
    method. The uploaded files are available in a separate array returned by
    ``getFiles()``. The ``getPhpValues()`` and ``getPhpFiles()`` also return
    the submitted values, but in the PHP format (it converts the keys with
    square brackets notation to PHP arrays).

.. index::
   pair: Tests; Configuration

Testing Configuration
---------------------

.. index::
   pair: PHPUnit; Configuration

PHPUnit Configuration
~~~~~~~~~~~~~~~~~~~~~

Each application has its own PHPUnit configuration, stored in the
``phpunit.xml.dist`` file. You can edit this file to change the defaults or
create a ``phpunit.xml`` file to tweak the configuration for your local machine.

.. tip::

    Store the ``phpunit.xml.dist`` file in your code repository, and ignore the
    ``phpunit.xml`` file.

By default, only the tests stored in "standard" bundles are run by the
``phpunit`` command (standard being tests under Vendor\\*Bundle\\Tests
namespaces). But you can easily add more namespaces. For instance, the
following configuration adds the tests from the installed third-party bundles:

.. code-block:: xml

    <!-- hello/phpunit.xml.dist -->
    <testsuites>
        <testsuite name="Project Test Suite">
            <directory>../src/*/*Bundle/Tests</directory>
            <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
        </testsuite>
    </testsuites>

To include other namespaces in the code coverage, also edit the ``<filter>``
section:

.. code-block:: xml

    <filter>
        <whitelist>
            <directory>../src</directory>
            <exclude>
                <directory>../src/*/*Bundle/Resources</directory>
                <directory>../src/*/*Bundle/Tests</directory>
                <directory>../src/Acme/Bundle/*Bundle/Resources</directory>
                <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
            </exclude>
        </whitelist>
    </filter>

Client Configuration
~~~~~~~~~~~~~~~~~~~~

The Client used by functional tests creates a Kernel that runs in a special
``test`` environment, so you can tweak it as much as you want:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        imports:
            - { resource: config_dev.yml }

        framework:
            error_handler: false
            test: ~

        web_profiler:
            toolbar: false
            intercept_redirects: false

        monolog:
            handlers:
                main:
                    type:  stream
                    path:  %kernel.logs_dir%/%kernel.environment%.log
                    level: debug

    .. code-block:: xml

        <!-- app/config/config_test.xml -->
        <container>
            <imports>
                <import resource="config_dev.xml" />
            </imports>

            <webprofiler:config
                toolbar="false"
                intercept-redirects="false"
            />

            <framework:config error_handler="false">
                <framework:test />
            </framework:config>

            <monolog:config>
                <monolog:main
                    type="stream"
                    path="%kernel.logs_dir%/%kernel.environment%.log"
                    level="debug"
                 />               
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config_test.php
        $loader->import('config_dev.php');

        $container->loadFromExtension('framework', array(
            'error_handler' => false,
            'test'          => true,
        ));

        $container->loadFromExtension('web_profiler', array(
            'toolbar' => false,
            'intercept-redirects' => false,
        ));

        $container->loadFromExtension('monolog', array(
            'handlers' => array(
                'main' => array('type' => 'stream',
                                'path' => '%kernel.logs_dir%/%kernel.environment%.log'
                                'level' => 'debug')
           
        )));

You can also change the default environment (``test``) and override the
default debug mode (``true``) by passing them as options to the
``createClient()`` method::

    $client = static::createClient(array(
        'environment' => 'my_test_env',
        'debug'       => false,
    ));

If your application behaves according to some HTTP headers, pass them as the
second argument of ``createClient()``::

    $client = static::createClient(array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

You can also override HTTP headers on a per request basis::

    $client->request('GET', '/', array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

.. tip::

    To provide your own Client, override the ``test.client.class`` parameter,
    or define a ``test.client`` service.

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/testing/http_authentication`
* :doc:`/cookbook/testing/insulating_clients`
* :doc:`/cookbook/testing/profiling`
