.. index::
   single: Tests

Les Tests
=========

A chaque fois que vous écrivez une nouvelle ligne de code, vous ajoutez aussi
potentiellement de nouveaux bugs. Pour créer de meilleurs applications et plus
fiables, vous devriez tester votre code avec des tests fonctionnels et des tests
unitaires.

Le Framework de Test PHPUnit
----------------------------

Symfony2 intègre une bibliothèque indépendante - appelée PHPUnit - qui vous fournit
un framework de tests complet. Ce chapitre ne couvre par PHPUnit elle-même, mais
elle a sa propre `documentation`_.

.. note::

    Symfony2 fonctionne avec PHPUnit 3.5.11 au minimum, mais la version 3.6.4 est
    nécessaire pour tester le coeur du framework.

Chaque test - que ce soit un test fonctionnel ou un test unitaire - est une classe
PHP qui devrait se trouver dans le sous-répertoire `Tests/` de vos bundles. Si vous
suivez cette règle, alors vous pouvez lancer les tests de votre application avec la
commande suivante :

.. code-block:: bash

    # spécifier le répertoire de configuration
    $ phpunit -c app/

L'option ``-c`` indique à PHPUnit de chercher le fichier de configuration dans le
répertoire ``app/``. Si vous voulez en savoir plus sur les options de PHPUnit, jetez
un oeil au fichier ``app/phpunit.xml.dist``.

.. tip::

    La couverture de code peut être générée avec l'option ``--coverage-html``.

.. index::
   single: Tests; Unit tests

Tests unitaires
---------------

Un test unitaire teste habituellement une classe PHP spécifique. Si vous voulez tester
le comportement général de votre application, lisez la sections sur les `Tests Fonctionnels`_.

Écrire des tests unitaires avec Symfony2 n'est pas différent d'écrire des tests
unitaires standards PHPUnit. Supposez, par exemple, que vous avez un classe
*incroyablement* simple appelée ``Calculator`` dans le répertoire ``Utility/`` de
votre bundle :

.. code-block:: php

    // src/Acme/DemoBundle/Utility/Calculator.php
    namespace Acme\DemoBundle\Utility;
    
    class Calculator
    {
        public function add($a, $b)
        {
            return $a + $b;
        }
    }

Pour tester cela, créez le fichier ``CalculatorTest`` dans le dossier ``Tests/Utility``
de votre:

.. code-block:: php

    // src/Acme/DemoBundle/Tests/Utility/CalculatorTest.php
    namespace Acme\DemoBundle\Tests\Utility;

    use Acme\DemoBundle\Utility\Calculator;

    class CalculatorTest extends \PHPUnit_Framework_TestCase
    {
        public function testAdd()
        {
            $calc = new Calculator();
            $result = $calc->add(30, 12);

            // vérifie que votre classe a correctement calculé!
            $this->assertEquals(42, $result);
        }
    }

.. note::

    Par convention, le sous-répertoire ``Tests/`` doit reproduire la structure de
    votre bundle. Donc, si vous testez une classe du répertoire ``Utility/`` du
    bundle, mettez le test dans le répertoire ``Tests/Utility/``.

Comme dans votre vraie application, le chargement automatique est activé via
le fichier ``bootstrap.php.cache`` (comme configuré par défaut dans le fichier 
``phpunit.xml.dist``).

Exécuter les tests pour un fichier ou répertoire donné est aussi très facile :

.. code-block:: bash

    # lancer tous les tests du répertoire Utility
    $ phpunit -c app src/Acme/DemoBundle/Tests/Utility/

    # lancer les tests de la classe Calculator
    $ phpunit -c app src/Acme/DemoBundle/Tests/Utility/CalculatorTest.php

    # lancer tous les tests d'un Bundle
    $ phpunit -c app src/Acme/DemoBundle/

.. index::
   single: Tests; Functional tests

Tests Fonctionnels
------------------

Les tests fonctionnels vérifient l'intégration des différentes couches d'une
application (du routing jusqu'aux vues). Ils ne sont pas différents des tests
unitaires tant que PHPUnit est concerné, mais ils possèdent un workflow très
spécifique :

* Faire une requête;
* Tester la réponse;
* Cliquer sur un lien ou soumettre un formulaire;
* Tester la réponse;
* Recommencer ainsi de suite.

Votre premier test fonctionnel
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les tests fonctionnels sont de simples fichiers PHP qui se trouvent dans le répertoire
``Tests/Controller`` du bundle. Si vous voulez tester des pages gérées par la classe
``DemoController``, commencez par créer un nouveau fichier ``DemoControllerTest.php``
qui étend la classe spéciale ``WebTestCase``.

Par exemple, la Symfony2 Standard Edition fournit un simple test fonctionnel pour
son ``DemoController`` (`DemoControllerTest`_) :

.. code-block:: php

    // src/Acme/DemoBundle/Tests/Controller/DemoControllerTest.php
    namespace Acme\DemoBundle\Tests\Controller;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;

    class DemoControllerTest extends WebTestCase
    {
        public function testIndex()
        {
            $client = static::createClient();

            $crawler = $client->request('GET', '/demo/hello/Fabien');

            $this->assertGreaterThan(0, $crawler->filter('html:contains("Hello Fabien")')->count());
        }
    }

.. tip::

    Pour lancer vos tests fonctionnels, la classe ``WebTestCase`` initie (bootstrap)
    le noyau (kernel) de votre application. Dans la plupart des cas, cela fonctionnera
    automatiquement. Cependant, si votre kernel n'est pas dans le répertoire habituel,
    vous devrez modifier le fichier ``phpunit.xml.dist`` pour définir la variable
    d'environnement ``KERNEL_DIR`` afin qu'elle pointe vers le répertoire du kernel :

    .. code-block:: xml

        <phpunit>
            <!-- ... -->
            <php>
                <server name="KERNEL_DIR" value="/path/to/your/app/" />
            </php>
            <!-- ... -->
        </phpunit>

La méthode ``createClient()`` retourne un client, qui ressemble au navigateur que
vous utilisez pour surfer sur vos sites:

.. code-block:: php

    $crawler = $client->request('GET', '/demo/hello/Fabien');

La méthode ``request()`` (plus d'informations sur :ref:`la méthode Request<book-testing-request-method-sidebar>`)
retourne un objet :class:`Symfony\\Component\\DomCrawler\\Crawler` qui peut être
utilisé pour sélectionner les éléments dans la Réponse, cliquer sur les liens, et
soumettre des formulaires.

.. tip::

    Le Crawler peut être utilisé seulement si le contenu de la Réponse est un
    document XML ou HTML.
    Pour obtenir le contenu brut, appelez la méthode ``$client->getResponse()->getContent()``.

Cliquez sur un lien en le sélectionnant avec le Crawler en utilisant
soit une expression XPath ou un sélecteur CSS, puis utilisez le Client pour
cliquer dessus. Par exemple, le code suivant trouve tous les liens avec le text
``Greet``, puis sélectionne le second et clique dessus :

.. code-block:: php

    $link = $crawler->filter('a:contains("Greet")')->eq(1)->link();

    $crawler = $client->click($link);

Soumettre un formulaire est très similaire : sélectionnez un bouton de ce dernier,
ré-écrivez quelques unes de ses valeurs si besoin est puis soumettez-le :

.. code-block:: php

    $form = $crawler->selectButton('submit')->form();

    // définit certaines valeurs
    $form['name'] = 'Lucas';
    $form['form_name[subject]'] = 'Hey there!';

    // soumet le formulaire
    $crawler = $client->submit($form);

.. tip::

    Le formulaire peut également gérer les envois et contient des méthodes pour
    remplir les différents types des champs de formulaire (ex ``select()`` et ``tick()``).
    Pour plus de détails, lisez la section `Formulaires`_.

Maintenant que vous pouvez naviguer aisément à travers une application, utilisez
les assertions pour tester qu'elle fait effectivement ce que vous souhaitez
qu'elle fasse. Utilisez le Crawler pour faire des assertions sur le DOM :

.. code-block:: php

    // affirme que la réponse correspond au sélecteur CSS donné
    $this->assertGreaterThan(0, $crawler->filter('h1')->count());

Ou alors, testez directement le contenu de la Réponse si vous voulez
juste vérifier qu'il contient un certain texte, ou si la Réponse n'est pas un
document XML/HTML :

.. code-block:: php

    $this->assertRegExp('/Hello Fabien/', $client->getResponse()->getContent());

.. _book-testing-request-method-sidebar:

.. sidebar:: Plus d'infos sur la méthode ``request()``:

    La signature complète de la méthdode ``request()`` est::

        request(
            $method,
            $uri, 
            array $parameters = array(), 
            array $files = array(), 
            array $server = array(), 
            $content = null, 
            $changeHistory = true
        )

    Le tableau ``server`` contient les valeurs brutes que vous trouveriez normalement
    dans la variable superglobale `$_SERVER`_. Par exemple, pour définir les entêtes
    HTTP `Content-Type` et `Referer`, vous procéderiez comme suit :

    .. code-block:: php

        $client->request(
            'GET',
            '/demo/hello/Fabien',
            array(),
            array(),
            array(
                'CONTENT_TYPE' => 'application/json',
                'HTTP_REFERER' => '/foo/bar',
            )
        );

.. index::
   single: Tests; Assertions

.. sidebar:: Assertions Utiles
    
    Afin que vous démarriez plus rapidement, voici une liste des
    assertions les plus communes et utiles :

    .. code-block:: php

        // Vérifie qu'il y a au moins une balise h2 dans la classe "subtitle"
        $this->assertGreaterThan(0, $crawler->filter('h2.subtitle')->count());

        // Vérifie qu'il y a exactement 4 balises h2 sur la page
        $this->assertCount(4, $crawler->filter('h2'));

        // Vérifie que l'entête "Content-Type" vaut "application/json"
        $this->assertTrue($client->getResponse()->headers->contains('Content-Type', 'application/json'));

        // Vérifie que le contenu retourné correspond à la regex
        $this->assertRegExp('/foo/', $client->getResponse()->getContent());

        // Vérifie que le status de la réponse est 2xx
        $this->assertTrue($client->getResponse()->isSuccessful());
        // Vérifie que le status de la réponse est 404
        $this->assertTrue($client->getResponse()->isNotFound());
        // Vérifie un status spécifique
        $this->assertEquals(200, $client->getResponse()->getStatus());

        // Vérifie que la réponse est redirigée vers /demo/contact
        $this->assertTrue($client->getResponse()->isRedirect('/demo/contact'));
        // ou vérifie simplement que la réponse est redirigée vers une URL quelconque
        $this->assertTrue($client->getResponse()->isRedirect());

.. index::
   single: Tests; Client

Travailler avec le Client Test
------------------------------

Le Client Test simule un client HTTP comme un navigateur et lance des requêtes à
votre application Symfony2 :

.. code-block:: php

    $crawler = $client->request('GET', '/hello/Fabien');

La méthode ``request()`` prend en arguments la méthode HTTP et une URL, et
retourne une instance de ``Crawler``.

Utilisez le Crawler pour trouver des éléments DOM dans la Réponse. Ces éléments
peuvent ainsi être utilisés pour cliquer sur des liens et soumettre des formulaires :

.. code-block:: php

    $link = $crawler->selectLink('Go elsewhere...')->link();
    $crawler = $client->click($link);

    $form = $crawler->selectButton('validate')->form();
    $crawler = $client->submit($form, array('name' => 'Fabien'));

Les méthodes ``click()`` et ``submit()`` retournent toutes deux un objet
``Crawler``. Ces méthodes sont le meilleur moyen de naviguer dans une
application car elles s'occupent de beaucoup de choses pour vous, comme détecter
la méthode HTTP à partir d'un formulaire et vous fournir une bonne API pour uploader
des fichiers.

.. tip::

    Vous en apprendrez davantage sur les objets ``Link`` et ``Form`` dans la
    section :ref:`Crawler<book-testing-crawler>` ci-dessous.

Mais vous pouvez aussi simuler les soumissions de formulaires et des requêtes
complexes grâce à la méthode ``request()`` :

.. code-block:: php

    // Soumettre directement un formulaire (mais utiliser le Crawler est plus simple)
    $client->request('POST', '/submit', array('name' => 'Fabien'));

    // Soumission de formulaire avec upload de fichier
    use Symfony\Component\HttpFoundation\File\UploadedFile;

    $photo = new UploadedFile(
        '/path/to/photo.jpg',
        'photo.jpg',
        'image/jpeg',
        123
    );
    // or
    $photo = array(
        'tmp_name' => '/path/to/photo.jpg',
        'name' => 'photo.jpg',
        'type' => 'image/jpeg',
        'size' => 123,
        'error' => UPLOAD_ERR_OK
    );
    $client->request(
        'POST',
        '/submit',
        array('name' => 'Fabien'),
        array('photo' => $photo)
    );

    // Exécute une requête DELETE et passe des entête HTTP
    $client->request(
        'DELETE',
        '/post/12',
        array(),
        array(),
        array('PHP_AUTH_USER' => 'username', 'PHP_AUTH_PW' => 'pa$$word')
    );

Enfin, vous pouvez forcer chaque requête à être exécutée
dans son propre processus PHP afin d'éviter quelconques effets secondaires
quand vous travaillez avec plusieurs clients dans le même script :

.. code-block:: php

    $client->insulate();

Naviguer
~~~~~~~~

Le Client supporte de nombreuses opérations qui peuvent être effectuées
à travers un navigateur réel :

.. code-block:: php

    $client->back();
    $client->forward();
    $client->reload();

    // efface tous les cookies et l'historique
    $client->restart();

Accéder aux Objets Internes
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous utilisez le client pour tester votre application, vous pourriez vouloir
accéder aux objets internes du client :

.. code-block:: php

    $history   = $client->getHistory();
    $cookieJar = $client->getCookieJar();

Vous pouvez aussi obtenir les objets liés à la dernière requête :

.. code-block:: php

    $request  = $client->getRequest();
    $response = $client->getResponse();
    $crawler  = $client->getCrawler();

Si vos requêtes ne sont pas isolées, vous pouvez aussi accéder au ``Container``
et au ``Kernel`` :

.. code-block:: php

    $container = $client->getContainer();
    $kernel    = $client->getKernel();

Accéder au Container
~~~~~~~~~~~~~~~~~~~~

Il est fortement recommandé qu'un test fonctionnel teste seulement la Réponse.
Mais dans certains cas rares, vous pourriez vouloir accéder à
quelques objets internes pour écrire des assertions. Dans tels cas, vous
pouvez accéder au conteneur d'injection de dépendances :

.. code-block:: php

    $container = $client->getContainer();

Notez bien que cela ne fonctionne pas si vous isolez le client ou si vous
utilisez une couche HTTP. Pour une liste des services disponibles dans votre
application, utilisez la commande ``container:debug``.

.. tip::

    Si les informations que vous avez besoin de vérifier sont disponibles via le
    profileur, utilisez le.

Accéder aux données du profileur
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour chaque requête, le profileur Symfony collecte et stocke diverses informations
sur la gestion interne des requêtes.
Par exemple, le profileur peut être utilisé pour vérifier qu'une page donnée ne
dépasse pas un certain nombre de requêtes en base de données lors de son chargement.

Pour obtenir le profileur de la dernière requête, utilisez le code suivant:

.. code-block:: php

    $profile = $client->getProfile();

Pour des détails spécifique sur l'utilisation du profileur au sein d'un test,
lisez la documentation du cookbook :doc:`/cookbook/testing/profiling`.

Redirection
~~~~~~~~~~~

Lorsqu'une requête retourne une réponse redirigée, le client ne la suit
pas automatiquement. Vous pouvez examiner la réponse puis forcer une redirection
grâce à la méthode ``followRedirect()``::

    $crawler = $client->followRedirect();

Si vous voulez que le client suive automatiquement tous les redirections,
vous pouvez le forcer avec la méthode ``followRedirects()``::

    $client->followRedirects();

.. index::
   single: Tests; Crawler

.. _book-testing-crawler:

Le Crawler
----------

Une instance de Crawler est retournée chaque fois que vous effectuez une requête
avec le Client. Elle vous permet de naviguer à travers des documents HTML, de
sélectionner des noeuds, de trouver des liens et des formulaires.

Traverser
~~~~~~~~~

Comme jQuery, le Crawler possède des méthodes lui permettant de naviguer à travers
le DOM d'un document HTML/XML. Par exemple, le code suivant trouve tout les éléments
``input[type=submit]``, sélectionne le dernier de la page et sélectionne son élément
parent immédiat :

.. code-block:: php

    $newCrawler = $crawler->filter('input[type=submit]')
        ->last()
        ->parents()
        ->first()
    ;

Beaucoup d'autres méthodes sont également disponibles :

+------------------------+----------------------------------------------------+
| Method                 | Description                                        |
+========================+====================================================+
| ``filter('h1.title')`` | Noeuds qui correspondent au sélecteur CSS          |
+------------------------+----------------------------------------------------+
| ``filterXpath('h1')``  | Noeuds qui correspondent à l'expression XPath      |
+------------------------+----------------------------------------------------+
| ``eq(1)``              | Noeud pour l'index spécifié                        |
+------------------------+----------------------------------------------------+
| ``first()``            | Premier noeud                                      |
+------------------------+----------------------------------------------------+
| ``last()``             | Dernier noeud                                      |
+------------------------+----------------------------------------------------+
| ``siblings()``         | Éléments de même niveau (siblings)                 |
+------------------------+----------------------------------------------------+
| ``nextAll()``          | Tous les siblings suivants                         |
+------------------------+----------------------------------------------------+
| ``previousAll()``      | Tous les siblings précédents                       |
+------------------------+----------------------------------------------------+
| ``parents()``          | Retourne les noeuds parents                        |
+------------------------+----------------------------------------------------+
| ``children()``         | Retourne les noeuds enfants                        |
+------------------------+----------------------------------------------------+
| ``reduce($lambda)``    | Noeuds pour lesquels $lambda ne retourne pas false |
+------------------------+----------------------------------------------------+

Puisque chacune de ses méthodes retourne une instance de ``Crawler``, vous pouvez
affiner votre sélection de noeud en enchainant les appels de méthodes :

.. code-block:: php

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

    Utilisez la fonction ``count()`` pour avoir le nombre de noeud contenus dans
    le Crawler :
    ``count($crawler)``

Extraction d'informations
~~~~~~~~~~~~~~~~~~~~~~~~~

Le Crawler peut extraire des informations des noeuds :

.. code-block:: php

    // retourne la valeur de l'attribut du premier noeud
    $crawler->attr('class');

    // retourne la valeur du noeud pour le premier noeud
    $crawler->text();

    // extrait un tableau d'attributs pour tous les noeuds (_text retourne
    // la valeur du noeud)
    // retourne un tableau de chaque élément du crawler, chacun avec les attributs
    // value et href
    $info = $crawler->extract(array('_text', 'href'));

    // exécute une lambda pour chaque noeud et retourne un tableau de résultats
    $data = $crawler->each(function ($node, $i)
    {
        return $node->attr('href');
    });

Liens
~~~~~

Vous pouvez sélectionner les liens grâce aux méthodes ci-dessus ou grâce au raccourci
très pratique ``selectLink()`` :

.. code-block:: php

    $crawler->selectLink('Click here');

Cela sélectionne tous les liens qui contiennent le texte donné, ou les images
cliquables dont l'attribut ``alt``contient ce texte. Comme les autres méthodes
de filtre, cela retourne un autre objet ``Crawler``.

Une fois que vous avez sélectionné un lien, vous avez accès à l'objet spécial ``Link``,
qui possède des méthodes utiles et spécifiques aux liens (comme ``getMethod()``
et ``getUri()``).Pour cliquer sur un lien, utiliser la méthode ``click()`` du Client
et passez la à un objet ``Link`` :

.. code-block:: php

    $link = $crawler->selectLink('Click here')->link();

    $client->click($link);

Formulaires
~~~~~~~~~~~

Comme pour les liens, vous sélectionnez les formulaires à l'aide de la méthode
``selectButton()`` :

.. code-block:: php

    $buttonCrawlerNode = $crawler->selectButton('submit');

.. note::

    Notez que nous sélectionnons les boutons de formulaire et non pas les formulaires
    eux-mêmes car un formulaire peut contenir plusieurs boutons; si vous utilisez l'API
    de traversement, gardez en mémoire que vous devez chercher un bouton.

La méthode ``selectButton()`` peut sélectionner des balises ``button`` et des
balises ``input`` de type submit; elle utilise plusieurs parties du bouton pour les trouver :

* La valeur de l'attribut ``value``;

* La valeur de l'attribut ``id`` ou ``alt`` pour les images;

* La valeur de l'attribut ``id`` ou ``name`` pour les balises ``button``.

Lorsque vous avez un Crawler qui représente un bouton, appelez la méthode ``form()``
pour obtenir une instance de ``Form`` pour le formulaire contenant le noeud du bouton :

.. code-block:: php

    $form = $buttonCrawlerNode->form();

Quand vous appelez la méthode ``form()``, vous pouvez aussi passer un tableau de
valeurs de champs qui ré-écrit les valeurs par défaut :

.. code-block:: php

    $form = $buttonCrawlerNode->form(array(
        'name'              => 'Fabien',
        'my_form[subject]'  => 'Symfony rocks!',
    ));

Et si vous voulez simuler une méthode HTTP spécifique pour le formulaire, passez la
comme second argument :

.. code-block:: php

    $form = $buttonCrawlerNode->form(array(), 'DELETE');

Le Client peut soumettre des instances de ``Form`` :

.. code-block:: php

    $client->submit($form);

Les valeurs des champs peuvent aussi être passées en second argument de la
méthode ``submit()`` :

.. code-block:: php

    $client->submit($form, array(
        'name'              => 'Fabien',
        'my_form[subject]'  => 'Symfony rocks!',
    ));

Pour les situations plus complexes, utilisez l'instance de ``Form`` en tant
que tableau pour définir la valeur de chaque champ individuellement :

.. code-block:: php

    // Change la valeur d'un champ
    $form['name'] = 'Fabien';
    $form['my_form[subject]'] = 'Symfony rocks!';

Il y a aussi une sympathique API qui permet de manipuler les valeurs des champs
selon leur type :

.. code-block:: php

    // sélectionne une option/radio
    $form['country']->select('France');

    // Coche une checkbox
    $form['like_symfony']->tick();

    // Upload un fichier
    $form['photo']->upload('/path/to/lucas.jpg');

.. tip::
    Vous pouvez obtenir les valeurs qui sont soumises en appelant la méthode
    ``getValues()`` de l'objet ``Form``. Les fichiers uploadés sont disponibles
    dans un tableau séparé retourné par ``getFiles()``. Les méthodes ``getPhpValues()`` et
    ``getPhpFiles()`` retournent aussi les valeurs soumises, mais au format
    PHP (cela convertit les clés avec la notation entre crochets - ex 
    ``my_form[subject]`` - en tableaux PHP).

.. index::
   pair: Tests; Configuration

Configuration de Test
---------------------

Le Client utilisé par les tests fonctionnels crée un Kernel qui est exécuté dans
un environnement de ``test``. Puisque Symfony charge le ``app/config/config_test.yml``
dans l'environnement de ``test``, vous pouvez modifier la configuration de votre
application spécifiquement pour les tests.

Par exemple, par défaut, le swiftmailer est configuré pour ne *pas* envoyer de mails
en environnement de ``test``. Vous pouvez le voir sous l'option de configuration
``swiftmailer`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        # ...

        swiftmailer:
            disable_delivery: true

    .. code-block:: xml

        <!-- app/config/config_test.xml -->
        <container>
            <!-- ... -->

            <swiftmailer:config disable-delivery="true" />
        </container>

    .. code-block:: php

        // app/config/config_test.php
        // ...

        $container->loadFromExtension('swiftmailer', array(
            'disable_delivery' => true
        ));

Vous pouvez aussi utiliser un environnement entièrement différent, ou surcharger le
mode debug par défaut (``true``) en les passant en tant qu'option à la méthode
``createClient()`` :

.. code-block:: php

    $client = static::createClient(array(
        'environment' => 'my_test_env',
        'debug'       => false,
    ));

Si votre application se comporte selon certaines en-têtes HTTP, passez les en
tant que second argument de ``createClient()`` :

.. code-block:: php

    $client = static::createClient(array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

Vous pouvez aussi ré-écrire les en-têtes HTTP par requête (i.e. et non pas pour
toutes les requêtes) :

.. code-block:: php

    $client->request('GET', '/', array(), array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

.. tip::

    Le client test est disponible en tant que service dans le conteneur en
    environnement de ``test`` (ou n'importe où si l'option :ref:`framework.test<reference-framework-test>`
    est activée). Cela signifie que vous pouvez surcharge entièrement le service
    en cas de besoin.

.. index::
   pair: PHPUnit; Configuration

Configuration PHPUnit
~~~~~~~~~~~~~~~~~~~~~

Chaque application possède sa propre configuration PHPUnit, stockée dans le
fichier ``phpunit.xml.dist``. Vous pouvez éditer ce fichier pour changer les
valeurs par défaut ou vous pouvez créer un fichier ``phpunit.xml`` pour personnaliser
la configuration de votre machine locale.

.. tip::

    Stockez le fichier ``phpunit.xml.dist`` dans votre gestionnaire de code, et
    ignorez le fichier ``phpunit.xml``.

Par défaut, seulement les tests situés dans des bundles « standards » sont exécutés
par la commande ``phpunit`` (standard étant des tests situés dans les répertoires
``src/*/Bundle/Tests`` ou ``src/*/Bundle/*Bundle/Tests``). Mais vous pouvez aisément
ajouter d'autres répertoires. Par exemple, la configuration suivante ajoute les tests
de bundles tiers installés :

.. code-block:: xml

    <!-- hello/phpunit.xml.dist -->
    <testsuites>
        <testsuite name="Project Test Suite">
            <directory>../src/*/*Bundle/Tests</directory>
            <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
        </testsuite>
    </testsuites>

Pour inclure d'autres répertoires dans la couverture du code, éditez aussi
la section ``<filter>`` :

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

En savoir plus
--------------

* :doc:`/components/dom_crawler`
* :doc:`/components/css_selector`
* :doc:`/cookbook/testing/http_authentication`
* :doc:`/cookbook/testing/insulating_clients`
* :doc:`/cookbook/testing/profiling`


.. _`DemoControllerTest`: https://github.com/symfony/symfony-standard/blob/master/src/Acme/DemoBundle/Tests/Controller/DemoControllerTest.php
.. _`$_SERVER`: http://php.net/manual/en/reserved.variables.server.php
.. _`documentation`: http://www.phpunit.de/manual/3.5/en/
