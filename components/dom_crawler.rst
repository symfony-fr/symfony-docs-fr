.. index::
   single: DomCrawler
   single: Components; DomCrawler

Le Composant DomCrawler
=======================

    Le Composant DomCrawler facilite la navigation DOM dans les documents HTML
    et XML.

Installation
------------

Vous pouvez installer le composant de différentes manières :

* Utilisez le dépôt Git officiel (https://github.com/symfony/DomCrawler) ;
* Installez le via Composer (``symfony/dom-crawler`` sur `Packagist`_).

Utilisation
-----------

La classe :class:`Symfony\\Component\\DomCrawler\\Crawler` fournit des méthodes
pour interroger et manipuler des documents HTML et XML.

Une instance du Crawler représente un ensemble (:phpclass:`SplObjectStorage`)
d'objets :phpclass:`DOMElement`, qui sont finalement des noeuds au travers desquels
vous pouvez naviguer aisément::

    use Symfony\Component\DomCrawler\Crawler;

    $html = <<<'HTML'
    <html>
        <body>
            <p class="message">Hello World!</p>
            <p>Hello Crawler!</p>
        </body>
    </html>
    HTML;

    $crawler = new Crawler($html);

    foreach ($crawler as $domElement) {
        print $domElement->nodeName;
    }

Les classes spécialisées :class:`Symfony\\Component\\DomCrawler\\Link`
et :class:`Symfony\\Component\\DomCrawler\\Form` sont utiles pour interagir
avec des liens et formulaires HTML lorsque vous naviguez au travers de
l'arbre HTML.

Filtrage de Noeud
~~~~~~~~~~~~~~~~~

Utiliser des expressions XPath est très facile::

    $crawler = $crawler->filterXPath('descendant-or-self::body/p');

.. tip::

    ``DOMXPath::query`` est utilisée en interne pour exécuter une requête XPath.

Filtrer des noeuds est d'autant plus facile si vous avez le Composant
``CssSelector`` installé.
Cela vous permet d'utiliser des sélecteurs similaires à ceux de jQuery pour
naviguer dans le DOM::

    $crawler = $crawler->filter('body > p');

Une fonction anonyme peut être utilisée pour filtrer des noeuds à l'aide
de critères plus complexes::

    $crawler = $crawler->filter('body > p')->reduce(function ($node, $i) {
        // filtre les noeuds pairs
        return ($i % 2) == 0;
    });

Pour supprimer un noeud, la fonction anonyme doit retourner « false ».

.. note::

    Toutes les méthodes de filtrage retournent une nouvelle instance de
    :class:`Symfony\\Component\\DomCrawler\\Crawler` avec le contenu filtré.

Navigation au travers des noeuds
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Accède au noeud par sa position dans la liste::

    $crawler->filter('body > p')->eq(0);

Récupère le premier ou dernier noeud de la sélection courante::

    $crawler->filter('body > p')->first();
    $crawler->filter('body > p')->last();

Récupère les noeuds du même niveau que la sélection courante::

    $crawler->filter('body > p')->siblings();

Récupère les noeuds de même niveau après ou avant la sélection courante::

    $crawler->filter('body > p')->nextAll();
    $crawler->filter('body > p')->previousAll();

Récupère tous les noeuds enfants ou parents::

    $crawler->filter('body')->children();
    $crawler->filter('body > p')->parents();

.. note::

    Toutes les méthodes de navigation retournent un nouvelle instance de
    :class:`Symfony\\Component\\DomCrawler\\Crawler`.

Accéder aux valeurs des noeuds
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Accède à la valeur du premier noeud de la sélection courante::

    $message = $crawler->filterXPath('//body/p')->text();

Accède à la valeur de l'attribut du premier noeud de la sélection courante::

    $class = $crawler->filterXPath('//body/p')->attr('class');

Extrait les valeurs de l'attribut et/ou du noeud de la liste des noeuds::

    $attributes = $crawler->filterXpath('//body/p')->extract(array('_text', 'class'));

.. note::

    L'attribut spécial ``_text`` représente la valeur d'un noeud.

Appelez une fonction anonyme sur chaque noeud de la liste::

    $nodeValues = $crawler->filter('p')->each(function ($node, $i) {
        return $node->text();
    });

La fonction anonyme reçoit la position et le noeud en tant qu'arguments.
Le résultat est un tableau de valeurs retournées par les appels de fonction
anonyme.

Ajouter du contenu
~~~~~~~~~~~~~~~~~~

Le « crawler » supporte plusieurs façons d'ajouter du contenu::

    $crawler = new Crawler('<html><body /></html>');

    $crawler->addHtmlContent('<html><body /></html>');
    $crawler->addXmlContent('<root><node /></root>');

    $crawler->addContent('<html><body /></html>');
    $crawler->addContent('<root><node /></root>', 'text/xml');

    $crawler->add('<html><body /></html>');
    $crawler->add('<root><node /></root>');

Comme l'implémentation du « Crawler » est basée sur l'extension DOM, elle est
aussi capable d'intéragir avec les objets natifs :phpclass:`DOMDocument`,
:phpclass:`DOMNodeList` et :phpclass:`DOMNode` :

.. code-block:: php

    $document = new \DOMDocument();
    $document->loadXml('<root><node /><node /></root>');
    $nodeList = $document->getElementsByTagName('node');
    $node = $document->getElementsByTagName('node')->item(0);

    $crawler->addDocument($document);
    $crawler->addNodeList($nodeList);
    $crawler->addNodes(array($node));
    $crawler->addNode($node);
    $crawler->add($document);

Support des Formulaires et des Liens
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Un traitement spécial est réservé pour les liens et formulaires se
trouvant dans l'arbre DOM.

Liens
.....

Pour trouver un lien par son nom (ou une image cliquable via son attribut ``alt``),
utilisez la méthode ``selectLink`` sur un « crawler » existant. Cela retourne
une instance de « Crawler » avec seulement le(s) lien(s) sélectionné(s).
Appeler la méthode ``link()`` vous retourne un objet spécial
:class:`Symfony\\Component\\DomCrawler\\Link`::

    $linksCrawler = $crawler->selectLink('Go elsewhere...');
    $link = $linksCrawler->link();

    // ou faites cela en une seule fois
    $link = $crawler->selectLink('Go elsewhere...')->link();

L'objet :class:`Symfony\\Component\\DomCrawler\\Link` possède plusieurs
méthodes utiles pour récupérer plus d'informations à propos du lien
sélectionné lui-même::


    // retourne l'URI qui peut être utilisée pour effectuer une autre requête
    $uri = $link->getUri();

.. note::
  
    La méthode ``getUri()`` est particulièrement utile car elle « nettoie » la
    valeur de ``href`` et la transforme en une valeur qui peut être utilisée.
    Par exemple, pour un lien tel ``href="#foo"``, cette méthode retournerait
    l'URI complète de la page courante suffixée avec ``#foo``. Le retour de la
    méthode ``getUri()`` est toujours une URI complète avec laquelle vous pouvez
    effectuer l'action de votre choix.

Formulaires
...........

Un traitement spécial est aussi réservé aux formulaires. Une méthode
``selectButton()`` est disponible sur le « Crawler » qui retourne un
autre « Crawler » qui a correspondu à un bouton (``input[type=submit]``,
``input[type=image]``, ou un ``button``) ayant le texte donné. Cette méthode
est très utile car vous pouvez l'utiliser pour retourner un objet
:class:`Symfony\\Component\\DomCrawler\\Form` qui représente le formulaire
dans lequel le bouton se trouve::

    $form = $crawler->selectButton('validate')->form();

    // ou « remplissez » les champs du formulaire avec des données
    $form = $crawler->selectButton('validate')->form(array(
        'name' => 'Ryan',
    ));

L'objet :class:`Symfony\\Component\\DomCrawler\\Form` possède de nombreuses
méthodes utiles pour travailler avec les formulaires::

    $uri = $form->getUri();

    $method = $form->getMethod();

La méthode :method:`Symfony\\Component\\DomCrawler\\Form::getUri` fait plus
que simplement retourner l'attribut ``action`` du formulaire. Si la méthode
du formulaire est GET, alors elle simule le comportement du navigateur et
retourne l'attribut ``action`` suivi par une chaîne de caractères représentant
toutes les valeurs du formulaires suffixées en tant que paramètres de requête.

Vous pouvez virtuellement définir et récupérer des valeurs du formulaire::

    // définit des valeurs du formulaire
    $form->setValues(array(
        'registration[username]' => 'symfonyfan',
        'registration[terms]'    => 1,
    ));

    // récupère un tableau de valeurs - tableau qui est « plat » comme ci-dessus
    $values = $form->getValues();

    // retourne les valeurs telles que PHP les verraient, où « registration » est son
    // propre tableau
    $values = $form->getPhpValues();

Pour travailler avec des champs multi-dimensionnels::

    <form>
        <input name="multi[]" />
        <input name="multi[]" />
        <input name="multi[dimensional]" />
    </form>

Vous devez spécifier le nom du champ entièrement qualifié::

    // Définit un seul champ
    // Set a single field
    $form->setValue('multi[0]', 'value');

    // Définit plusieurs champs en une seule fois
    $form->setValue('multi', array(
        1             => 'value',
        'dimensional' => 'an other value'
    ));

C'est super, mais le meilleur reste à venir ! L'objet ``Form`` vous permet
d'intéragir avec votre formulaire comme un navigateur, en sélectionnant des
valeurs de boutons radio, en cochant des cases « checkbox », et en « uploadant »
des fichiers::

    $form['registration[username]']->setValue('symfonyfan');

    // coche ou décoche une case « checkbox »
    $form['registration[terms]']->tick();
    $form['registration[terms]']->untick();

    // sélectionne une option
    $form['registration[birthday][year]']->select(1984);

    // sélectionne plusieurs options d'un champ « select » multiple ou
    // plusieurs cases « checkbox »
    $form['registration[interests]']->select(array('symfony', 'cookies'));

    // peut même simuler un « upload » de fichier
    $form['registration[photo]']->upload('/path/to/lucas.jpg');

Quel est le but d'effectuer tout cela ? Si vous faites des tests en interne,
vous pouvez récupérer les informations de votre formulaire comme s'il avait
été soumis en utilisant des valeurs PHP::

    $values = $form->getPhpValues();
    $files = $form->getPhpFiles();

Si vous utilisez un client HTTP externe, vous pouvez utiliser le formulaire
pour récupérer toutes les informations dont vous avez besoin pour créer une
requête POST pour le formulaire::

    $uri = $form->getUri();
    $method = $form->getMethod();
    $values = $form->getValues();
    $files = $form->getFiles();

    // maintenant, utilisez n'importe quel client HTTP et postez le formulaire
    // en utilisant ces informations

Un bel exemple d'un système intégré qui utilise tout cela est `Goutte`_.
Goutte comprend l'objet « Crawler » de Symfony et peut l'utiliser pour
soumettre des formulaires directement::

    use Goutte\Client;

    // effectue une requête réelle vers un site externe
    $client = new Client();
    $crawler = $client->request('GET', 'https://github.com/login');

    // sélectionne le formulaire et le remplit avec quelques valeurs
    $form = $crawler->selectButton('Log in')->form();
    $form['login'] = 'symfonyfan';
    $form['password'] = 'anypass';

    // soumet le formulaire
    $crawler = $client->submit($form);

.. _`Goutte`:  https://github.com/fabpot/goutte
.. _Packagist: https://packagist.org/packages/symfony/dom-crawler
