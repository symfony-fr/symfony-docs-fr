.. index::
   single: Symfony Fundamentals

Les fondamentaux de Symfony et HTTP
====================================

Félicitations! Grâce à l'apprentissage de Symfony, vous êtes sur la bonne voie pour
devenir un développeur web plus *productif* et *populaire* (en fait la popularité
ne dépend que de vous). Symfony est construit de manière à revenir à
l'essentiel : implémenter des outils qui vous aident à développer plus rapidement
et à construire des applications plus robustes, sans pour autant vous gêner dans votre
progression.
Symfony repose sur les meilleures idées provenant de diverses technologies : les outils
et concepts que vous êtes sur le point d'apprendre représentent les efforts de
milliers de personnes depuis de nombreuses années. En d'autres termes, vous
n'apprenez pas juste "Symfony", vous apprenez les fondamentaux du web,
les bonnes pratiques de développement, et comment utiliser de nombreuses
nouvelles bibliothèques PHP, internes ou indépendantes de Symfony. Alors,
soyez prêt !

Fidèle à la philosophie de Symfony, ce chapitre débute par une explication du
concept fondamental du développement web : HTTP. Quelles que soient vos
connaissances ou votre langage de programmation préféré, ce chapitre **doit
être lu** par tout un chacun.

HTTP est simple
---------------

HTTP (HyperText Transfer Protocol pour les geeks) est un langage texte qui
permet à deux machines de communiquer ensemble. C'est tout ! Par exemple,
lorsque vous regardez la dernière BD de `xkcd`_, la conversation suivante
(approximative) se déroule:

.. image:: /images/http-xkcd.png
   :align: center

Et alors que l'actuel langage utilisé est un peu plus formel, cela reste
toujours très simple. HTTP est le terme utilisé pour décrire ce simple
langage texte. Et peu importe comment vous développez sur
le web, le but de votre serveur est *toujours* de comprendre de simples
requêtes composées de texte, et de retourner de simples réponses composées
elles aussi de texte.

Symfony est construit sur les bases de cette réalité. Que vous le
réalisiez ou non, HTTP est quelque chose que vous utilisez tous les jours.
Avec Symfony, vous allez apprendre comment le maîtriser.

.. index::
   single: HTTP; Request-response paradigm

Étape 1: Le Client envoie une Requête
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Chaque conversation sur le web débute avec une *requête*. La requête est
un message textuel créé par un client (par exemple: un navigateur, une
application smartphone, etc...) dans un format spécial connu sous le nom d'HTTP.
Le client envoie cette requête à un serveur, et puis attend la réponse.

Jetez un oeil à la première partie de l'interaction (la requête) entre un
navigateur et le serveur web xkcd:

.. image:: /images/http-xkcd-request.png
   :align: center

Dans le langage HTTP, cette requête HTTP ressemblerait à quelque chose
comme ça:

.. code-block:: text

    GET / HTTP/1.1
    Host: xkcd.com
    Accept: text/html
    User-Agent: Mozilla/5.0 (Macintosh)

Ce simple message communique *tout* ce qui est nécessaire concernant la
ressource que le client a demandée. La première ligne d'une requête HTTP
est la plus importante et contient deux choses: l'URI et la méthode HTTP.

L'URI (par exemple: ``/``, ``/contact``, etc...) est l'adresse unique ou
la localisation qui identifie la ressource que le client veut. La méthode
HTTP (par exemple: ``GET``) définit ce que vous voulez *faire* avec la
ressource. Les méthodes HTTP sont les *verbes* de la requête et définissent
les divers moyens par lesquels vous pouvez agir sur la ressource:

+----------+-----------------------------------------+
| *GET*    | Récupère la ressource depuis le serveur |
+----------+-----------------------------------------+
| *POST*   | Crée une ressource sur le serveur       |
+----------+-----------------------------------------+
| *PUT*    | Met à jour la ressource sur le serveur  |
+----------+-----------------------------------------+
| *DELETE* | Supprime la ressource sur le serveur    |
+----------+-----------------------------------------+

Avec ceci en mémoire, vous pouvez imaginer ce à quoi ressemblerait une
requête HTTP pour supprimer une entrée spécifique d'un blog, par exemple:

.. code-block:: text

    DELETE /blog/15 HTTP/1.1

.. note::

    Il y a en fait neuf méthodes HTTP définies par la spécification HTTP,
    mais beaucoup d'entre elles ne sont pas largement utilisées ou supportées.
    En réalité, beaucoup de navigateurs modernes ne supportent que ``POST`` et
    ``GET`` dans les formulaires HTML. Les autres méthodes sont en revanche
    supportées dans les requêtes XMLHttpRequests et par le routeur de Symfony.

En plus de la première ligne, une requête HTTP contient invariablement
d'autres lignes d'informations appelées entêtes de requête. Les entêtes
peuvent fournir un large éventail d'informations telles que l'entête ``Host``,
le format de réponse que le client accepte (``Accept``) et
l'application que le client utilise pour effectuer la requête (``User-Agent``).
Beaucoup d'autres entêtes existent et peuvent être trouvés sur la page
Wikipedia `Liste des headers HTTP`_ (anglais).

Étape 2: Le Serveur retourne une réponse
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Une fois que le serveur a reçu la requête, il connaît exactement quelle ressource
le client a besoin (via l'URI) et ce que le client souhaite faire avec cette
ressource (via la méthode). Par exemple, dans le cas d'une requête GET, le
serveur prépare la ressource et la retourne dans une réponse HTTP. Considérez
la réponse du serveur web xkcd :

.. image:: /images/http-xkcd.png
   :align: center

Traduit en HTTP, la réponse envoyée au navigateur va ressembler à quelque chose
comme ça :

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 02 Apr 2011 21:05:05 GMT
    Server: lighttpd/1.4.19
    Content-Type: text/html

    <html>
      <!-- ... code HTML de la BD xkcd -->
    </html>

La réponse HTTP contient la ressource demandée (le contenu HTML dans ce cas),
ainsi que d'autres informations à propos de la réponse. La première ligne
est spécialement importante et contient le code de statut de la réponse
HTTP (200 dans ce cas). Le code de statut communique le résultat global
de la requête retournée au client. A-t-elle réussi ? Y'a-t-il eu une
erreur ? Différents codes de statut existent qui indiquent le succès, une
erreur, ou que le client a besoin de faire quelque chose (par exemple:
rediriger sur une autre page). Une liste complète peut être trouvée sur
la page Wikipedia `Liste des codes HTTP`_ .

Comme la requête, une réponse HTTP contient de l'information additionnelle
sous forme d'entêtes HTTP. Par exemple, ``Content-Type`` est un entête
de réponse HTTP très important. Le corps d'une même ressource peut être retournée
dans de multiples formats incluant HTML, XML ou JSON et l'entête ``Content-Type``
utilise les Internet Media Types, comme ``text/html``, pour dire au client quel format
doit être retourné. Une liste des médias types les plus communs peut être trouvée sur
la page Wikipedia `Liste des types de media usuels`_.

De nombreuses autres entêtes existent, dont quelques-uns sont très puissants.
Par exemple, certains entêtes peuvent être utilisés pour créer un puissant
système de cache.

Requêtes, Réponses et Développement Web
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cette conversation requête-réponse est le procédé fondamental qui régit
toute communication sur le web. Et tout aussi important et puissant que ce
procédé soit, il est inéluctablement simple.

Le plus important est que : quel que soit le langage que vous utilisez, le
type d'application que vous construisez (web, mobile, API JSON), ou la
philosophie de développement que vous suivez, l'objectif final d'une
application est **toujours** de comprendre chaque requête et de créer et
retourner la réponse appropriée.

Symfony est conçu pour correspondre à cette réalité.

.. tip::

    Pour en savoir plus à propos de la spécification HTTP, lisez la RFC originale
    `HTTP 1.1 RFC`_ ou le `HTTP Bis`_, qui est un effort actif pour clarifier la
    spécification originale. Un super outil pour inspecter/vérifier les entêtes
    de la requête et de la réponse durant votre navigation est l'extension
    pour Firefox `Live HTTP Headers`_.

.. index::
   single: Symfony Fundamentals; Requests and responses

Requêtes et réponses en PHP
---------------------------

Alors comment interagissez-vous avec la « requête » et créez-vous la « réponse »
quand vous utilisez PHP ? En réalité, PHP vous abstrait une partie du processus
global::

    $uri = $_SERVER['REQUEST_URI'];
    $foo = $_GET['foo'];

    header('Content-type: text/html');
    echo 'L\'URI demandée est: '.$uri;
    echo 'La valeur du paramètre "foo" est: '.$foo;

Aussi étrange que cela puisse paraître, cette petite application utilise les
informations de la requête HTTP afin de créer une réponse.
Plutôt que d'analyser le message texte de la requête HTTP directement,
PHP prépare des variables superglobales telles que ``$_SERVER`` et ``$_GET``
qui contiennent toute les informations de la requête. De même, au lieu de
retourner la réponse texte HTTP formatée, vous pouvez utiliser la fonction
``header()`` pour créer des entêtes de réponse et simplement délivrer le
contenu actuel qui sera la partie contenu du message de la réponse.
PHP va ainsi créer une véritable réponse HTTP et la retourner au client :

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 03 Apr 2011 02:14:33 GMT
    Server: Apache/2.2.17 (Unix)
    Content-Type: text/html

    L'URI demandée est: /testing?foo=symfony
    La valeur du paramètre "foo" est: symfony

Requêtes et Réponses dans Symfony
---------------------------------

Symfony fournit une alternative à l'approche brute de PHP via deux classes
qui vous permettent d'interagir avec la requête et la réponse HTTP de manière
plus facile. La classe :class:`Symfony\\Component\\HttpFoundation\\Request`
est une simple représentation orientée objet du message de la requête HTTP.
Avec elle, vous avez toute l'information de la requête à votre portée::

    use Symfony\Component\HttpFoundation\Request;

    $request = Request::createFromGlobals();

    // l'URI demandée (par exemple: /about) sans aucun paramètre
    $request->getPathInfo();

    // obtenir respectivement des variables GET et POST
    $request->query->get('foo');
    $request->request->get('bar', 'valeur par défaut si bar est inexistant');

    // obtenir les variables SERVER
    $request->server->get('HTTP_HOST');

    // obtenir une instance de UploadedFile identifiée par foo
    $request->files->get('foo');

    // obtenir la valeur d'un COOKIE value
    $request->cookies->get('PHPSESSID');

    // obtenir un entête de requête HTTP request header, normalisé en minuscules
    $request->headers->get('host');
    $request->headers->get('content_type');

    $request->getMethod();          // GET, POST, PUT, DELETE, HEAD
    $request->getLanguages();       // un tableau des langues que le client accepte

En bonus, la classe ``Request`` effectue beaucoup de travail en arrière-plan
dont vous n'aurez jamais à vous soucier. Par exemple, la méthode ``isSecure()``
vérifie les *trois* valeurs PHP qui peuvent indiquer si oui ou non l'utilisateur
est connecté via une connexion sécurisée (c-a-d ``https``).


.. sidebar:: Attributs de ParameterBags et Request

    Comme vu ci-dessus, les variables ``$_GET`` et ``$_POST`` sont accessibles
    respectivement via les propriétés publiques ``query`` et ``request``. Chacun
    de ces objets est un objet :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`
    qui a des méthodes comme :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::get`,
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::has`,
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::all` et bien d'autres.
    En fait, chaque propriété publique utilisée dans l'exemple précédent est
    une instance de ParameterBag.

    .. _book-fundamentals-attributes:

    La classe Request a aussi une propriété publique ``attributes`` qui contient
    des données spéciales liées au fonctionnement interne de l'application. Pour
    le framework Symfony, la propriété ``attributes`` contient les valeurs retournées
    par la route identifiée, comme ``_controller``, ``id`` (si vous utilisez le joker ``{id}``),
    et même le nom de la route (``_route``). La propriété ``attributes`` existe pour
    vous permettre d'y stocker des informations spécifiques liées au contexte de
    la requête.


Symfony fournit aussi une classe ``Response`` : une simple représentation PHP du
message d'une réponse HTTP. Cela permet à votre application d'utiliser une
interface orientée objet pour construire la réponse qui doit être retournée
au client::

    use Symfony\Component\HttpFoundation\Response;
    $response = new Response();

    $response->setContent('<html><body><h1>Hello world!</h1></body></html>');
    $response->setStatusCode(Response::HTTP_OK);
    $response->headers->set('Content-Type', 'text/html');

    // affiche les entêtes HTTP suivies du contenu
    $response->send();

Si Symfony n'offrait rien d'autre, vous devriez néanmoins déjà avoir en votre
possession une boîte à outils pour accéder facilement aux informations de la
requête et une interface orientée objet pour créer la réponse. Bien que vous
appreniez les nombreuses et puissantes fonctions de Symfony, gardez à l'esprit
que le but de votre application est toujours *d'interpréter une requête et de
créer la réponse appropriée basée sur votre logique applicative*.

.. tip::

    Les classes ``Request`` et ``Response`` font partie d'un composant
    autonome inclus dans Symfony appelé ``HttpFoundation``. Ce composant peut
    être utilisé de manière entièrement indépendante de Symfony et fournit aussi
    des classes pour gérer les sessions et les uploads de fichier.

Le Parcours de la Requête à la Réponse
--------------------------------------

Comme HTTP lui-même, les objets ``Request`` et ``Response`` sont assez simples.
La partie difficile de la création d'une application est d'écrire ce qui vient
entre les deux. En d'autres termes, le réel travail commence lors de l'écriture
du code qui interprète l'information de la requête et crée la réponse.

Votre application fait probablement beaucoup de choses comme envoyer des emails,
gérer des soumissions de formulaires, enregistrer des choses dans une base de données,
délivrer des pages HTML et protéger du contenu de façon sécurisée. Comment pouvez-vous
vous occuper de tout cela tout en conservant votre code organisé et maintenable ?

Symfony a été créé pour résoudre ces problématiques afin que vous n'ayez pas à le
faire vous-même.

Le Contrôleur Frontal
~~~~~~~~~~~~~~~~~~~~~

Traditionnellement, les applications étaient construites de telle sorte que
chaque « page » d'un site avait son propre fichier physique:

.. code-block:: text

    index.php
    contact.php
    blog.php

Il y a plusieurs problèmes avec cette approche, ce qui inclut la non-flexibilité
des URLs (que se passait-il si vous souhaitiez changer ``blog.php`` en
``news.php`` sans que tous vos liens existants ne cessent de fonctionner ?)
et le fait que chaque fichier *doive* manuellement inclure tout un ensemble
de fichiers coeurs pour que la sécurité, les connexions à la base de données
et le « look » du site puissent rester cohérents.

Une bien meilleure solution est d'utiliser un simple fichier PHP appelé
:term:`contrôleur frontal`: qui s'occupe de chaque requête arrivant dans
votre application. Par exemple:

+------------------------+-----------------------+
| ``/index.php``         | exécute ``index.php`` |
+------------------------+-----------------------+
| ``/index.php/contact`` | exécute ``index.php`` |
+------------------------+-----------------------+
| ``/index.php/blog``    | exécute ``index.php`` |
+------------------------+-----------------------+

.. tip::

    En utilisant la fonction ``mod_rewrite`` d'Apache (ou son équivalent
    avec d'autres serveurs web), les URLs peuvent être facilement réécrites
    afin de devenir simplement ``/``, ``/contact`` et ``/blog``.

Maintenant, chaque requête est gérée exactement de la même façon. Plutôt
que d'avoir des URLs individuelles exécutant des fichiers PHP différents,
le contrôleur frontal est *toujours* exécuté, et le routage (« routing ») des
différentes URLs vers différentes parties de votre application est effectué
en interne. Cela résoud les deux problèmes de l'approche originale.
Presque toutes les applications web modernes font ça – incluant les
applications comme WordPress.

Rester Organisé
~~~~~~~~~~~~~~~

Dans votre contrôleur frontal, vous devez déterminer quelle portion de code
doit être exécuté et quel est le contenu qui doit être retourné. Pour le
savoir, vous allez devoir inspecter l'URI entrante et exécuter les différentes
parties de votre code selon cette valeur. Cela peut rapidement devenir moche::

    // index.php
    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;

    $request = Request::createFromGlobals();
    $path = $request->getPathInfo(); // Le chemin de l'URI demandée

    if (in_array($path, array('', '/'))) {
        $response = new Response('Bienvenue sur le site.');
    } elseif ('/contact' === $path) {
        $response = new Response('Contactez nous');
    } else {
        $response = new Response('Page non trouvée.', Response::HTTP_NOT_FOUND);
    }
    $response->send();

Résoudre ce problème peut être difficile. Heureusement, c'est *exactement* ce pourquoi
Symfony a été conçu.

Le Déroulement d'une Application Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quand vous laissez Symfony gérer chaque requête, la vie est beaucoup plus facile.
Symfony suit un schéma simple et identique pour toutes les requêtes :

.. _request-flow-figure:

.. figure:: /images/request-flow.png
   :align: center
   :alt: Le déroulement d'une requête Symfony

   Les requêtes entrantes sont interprétées par le routing et passées aux
   fonctions des contrôleurs qui retournent des objets ``Response``.

Chaque « page » de votre site est définie dans un fichier de configuration du
routing qui relie différentes URLs à différentes fonctions PHP. Le travail de
chaque fonction PHP, appelée :term:`contrôleur`, est de créer puis retourner 
un objet ``Response``, construit à partir des informations de la requête, à l'aide 
des outils mis à disposition par le framework. En d'autres termes, le contrôleur 
est le lieu où *votre* code se trouve : c'est là que vous interprétez la requête et 
que vous créez une réponse.

C'est aussi simple que cela ! Revoyons cela :

* Chaque requête exécute un même et unique fichier ayant le rôle de contrôleur frontal;

* Le système de routing détermine quelle fonction PHP doit être exécutée
  en se basant sur les informations provenant de la requête et la configuration de
  routage que vous avez créé;

* La fonction PHP correcte est exécutée, là où votre code crée et retourne
  l'objet ``Response`` approprié.

Une Requête Symfony en Action
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sans aller trop loin dans les détails, voyons ce procédé en action. Supposons
que vous vouliez ajouter une page ``/contact`` à votre application Symfony.
Premièrement, commencez par ajouter une entrée pour ``/contact`` dans votre
fichier de configuration du routing:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        contact:
            path:     /contact
            defaults: { _controller: AppBundle:Main:contact }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact" path="/contact">
                <default key="_controller">AppBundle:Main:contact</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/contact', array(
            '_controller' => 'AppBundle:Main:contact',
        )));

        return $collection;


Lorsque quelqu'un visite la page ``/contact``, il y a correspondance avec cette route,
et le contrôleur spécifié est exécuté. Comme vous l'apprendrez dans le
:doc:`chapitre sur le routage</book/routing>`, la chaîne de caractères ``AppBundle:Main:contact``
est une syntaxe raccourcie qui pointe vers une méthode PHP spécifique ``contactAction`` dans la
classe appelée ``MainController``::

    // src/AppBundle/Controller/MainController.php
    namespace AppBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class MainController
    {
        public function contactAction()
        {
            return new Response('<h1>Contactez nous!</h1>');
        }
    }

Dans cet exemple très simple, le contrôleur crée simplement un objet
:class:`Symfony\\Component\\HttpFoundation\\Response` contenant le HTML
``<h1>Contactez nous!</h1>``. Dans le :doc:`chapitre Contrôleur</book/controller>`, vous allez
apprendre comment un contrôleur peut retourner des templates, permettant à votre code de
« présentation » (c-a-d du code qui retourne du HTML) de se trouver dans un fichier de template
séparé. Cela libère le contrôleur et lui permet de s'occuper seulement des choses complexes :
interagir avec la base de données, gérer les données soumises, ou envoyer des emails.


Symfony: Construisez votre application, pas vos outils
-------------------------------------------------------

Vous savez maintenant que le but de toute application est d'interpréter
chaque requête entrante et de créer une réponse appropriée. Avec le temps,
une application grandit et il devient plus difficile de garder le code organisé
et maintenable. Invariablement, les mêmes tâches complexes reviennent encore
et toujours : persister des éléments dans la base de données, afficher et
réutiliser des templates, gérer des soumissions de formulaires, envoyer
des emails, valider des entrées d'utilisateurs et gérer la sécurité.

La bonne nouvelle est qu'aucun de ces problèmes n'est unique. Symfony fournit
un framework rempli d'outils qui vous permettent de construire votre
application, mais pas vos outils. Avec Symfony, rien ne vous est imposé :
vous êtes libre d'utiliser le framework Symfony en entier, ou juste une partie
de Symfony indépendamment.

.. index::
   single: Symfony Components


Outils Autonomes: Les *Composants* Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Donc *qu'est-ce* que Symfony ? Premièrement, Symfony est une collection de plus
de vingt bibliothèques indépendantes qui peuvent être utilisées dans *n'importe quel*
projet PHP. Ces bibliothèques, appelées les *Composants Symfony*, contiennent
des choses utiles en toute situation, quelle que soit
la manière dont votre projet est développé. Pour en nommer quelques-unes :


:doc:`HttpFoundation</components/http_foundation/introduction>`
    Contient les classes ``Request`` et ``Response``, ainsi que
    d'autres classes pour la gestion des sessions et des uploads
    de fichiers.

:doc:`Routing</components/routing/introduction>`
    Un puissant et rapide système qui vous permet de lier une URI
    spécifique (par exemple: ``/contact``) à l'information lui permettant
    de savoir comment gérer cette requête (par exemple: exécute la
    méthode ``contactAction()``).

:doc:`Form </components/form/introduction>`
    Un framework complet et flexible pour la création de formulaires et
    la gestion de la soumission de ces derniers.

`Validator`_
    Un système permettant de créer des règles à propos de données et de
    valider ou non les données utilisateurs soumises suivant ces règles.

:doc:`Templating</components/templating>`
    Une boîte à outils pour afficher des templates, gérer leur héritage
    (c-a-d qu'un template est décoré par un layout) et effectuer
    d'autres tâches communes aux templates.

:doc:`Security </components/security/introduction>`
    Une puissante bibliothèque pour gérer tous les types de sécurité
    dans une application.

:doc:`Translation </components/translation/introduction>`
    Un framework pour traduire les chaînes de caractères dans votre
    application.

Chacun de ces composants est découplé et peut être utilisé dans *n'importe quel*
projet PHP, que vous utilisiez le framework Symfony ou pas.
Chaque partie est faite pour être utilisée en cas de besoin et remplacée quand cela
est nécessaire.

La Solution Complète: Le *Framework* Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Donc finalement, *qu'est-ce* que le *Framework* Symfony ? Le *Framework Symfony*
est une bibliothèque PHP qui accomplit deux tâches distinctes :

#. Fournir une sélection de composants (les Composants Symfony) et
   des bibliothèques tierces (ex `Swiftmailer`_ pour envoyer des emails);

#. Fournir une configuration et une bibliothèque « colle » qui lie toutes ces
   pièces ensembles.

Le but du framework est d'intégrer plein d'outils indépendants afin de
fournir une expérience substantielle au développeur. Même le framework lui-même
est un bundle Symfony (c-a-d un plugin) qui peut être configuré ou remplacé
entièrement.

Symfony fournit un puissant ensemble d'outils pour développer rapidement des
applications web sans pour autant s'imposer à votre application. Les utilisateurs
normaux peuvent commencer rapidement à développer en utilisant une distribution
Symfony, ce qui fournit un squelette de projet avec des paramètres par défaut.
Pour les utilisateurs avancés, le ciel est la seule limite.

.. _`xkcd`: http://xkcd.com/
.. _`HTTP 1.1 RFC`: http://www.w3.org/Protocols/rfc2616/rfc2616.html
.. _`HTTP Bis`: http://datatracker.ietf.org/wg/httpbis/
.. _`Live HTTP Headers`: https://addons.mozilla.org/en-US/firefox/addon/live-http-headers/
.. _`Liste des codes HTTP`: http://fr.wikipedia.org/wiki/Liste_des_codes_HTTP
.. _`Liste des headers HTTP`: https://en.wikipedia.org/wiki/List_of_HTTP_header_fields
.. _`Liste des types de media usuels`: https://www.iana.org/assignments/media-types/media-types.xhtml
.. _`Validator`: https://github.com/symfony/validator
.. _`Swift Mailer`: http://swiftmailer.org/
