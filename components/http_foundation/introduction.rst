.. index::
   single: HTTP
   single: HttpFoundation
   single: Components; HttpFoundation

Le composant HttpFoundation
===========================

    Le composant HttpFoundation définit une couche orientée objet pour la
    spécification HTTP.

Avec PHP, la requête est représentée par certaines variables globales (``$_GET``,
``$_POST``, ``$_FILE``, ``$_COOKIE``, ``$_SESSION``...) et la réponse est générée
à l'aide de fonctions (``echo``, ``header``, ``setcookie``, ...).

Le composant Symfony2 HttpFoundation remplace ces variables par défauts et ces 
fonctions par un ensemble Orienté Objet.

Installation
------------

Vous pouvez installer le composant de différentes façon :

* En utilisant le dépôt officiel Git (https://github.com/symfony/HttpFoundation) ;
* En l'installant à l'aide de Composer (``symfony/http-foundation`` sur `Packagist`_).

Requête
-------

La manière la plus commune pour créer une requête est de l'initialiser à l'aide
des variables globales en « cours de validité » avec
:method:`Symfony\\Component\\HttpFoundation\\Request::createFromGlobals` ::

    use Symfony\Component\HttpFoundation\Request;

    $request = Request::createFromGlobals();

qui est l'équivalent de la forme plus verbeuse mais aussi plus flexible de l'appel
:method:`Symfony\\Component\\HttpFoundation\\Request::__construct` ::

    $request = new Request($_GET, $_POST, array(), $_COOKIE, $_FILES, $_SERVER);

Accéder aux données provenant de la requête
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Un objet requête contient les informations concernant le client. Ces informations
peuvent être consultées via plusieurs propriétés publiques :

* ``request``: équivalent à ``$_POST`` ;

* ``query``: équivalent à ``$_GET`` (``$request->query->get('name')``) ;

* ``cookies``: équivalent à ``$_COOKIE`` ;

* ``attributes``: n'a pas d'équivalent - c'est une propriété utilisée par votre
  application pour conserver certaines données (voir
  :ref:`ci-dessous<component-foundation-attributes>`) ;

* ``files``: équivalent à ``$_FILE`` ;

* ``server``: équivalent à ``$_SERVER`` ;

* ``headers``: globalement équivalent à un sous-ensemble du tableau ``$_SERVER``
  (``$request->headers->get('Content-Type')``).

Chaque propriété est une instance (ou une sous-classe) de
:class:`Symfony\\Component\\HttpFoundation\\ParameterBag`, qui est la classe
de conservation des données :

* ``request``: :class:`Symfony\\Component\\HttpFoundation\\ParameterBag` ;

* ``query``:   :class:`Symfony\\Component\\HttpFoundation\\ParameterBag` ;

* ``cookies``: :class:`Symfony\\Component\\HttpFoundation\\ParameterBag` ;

* ``attributes``: :class:`Symfony\\Component\\HttpFoundation\\ParameterBag` ;

* ``files``:   :class:`Symfony\\Component\\HttpFoundation\\FileBag` ;

* ``server``:  :class:`Symfony\\Component\\HttpFoundation\\ServerBag` ;

* ``headers``: :class:`Symfony\\Component\\HttpFoundation\\HeaderBag`.

Toutes les instances de :class:`Symfony\\Component\\HttpFoundation\\ParameterBag` 
ont des méthodes pour consulter et mettre à jour les données incluses :

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::all`: Retourne les
  paramètres ;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::keys`: Retourne
  les clés des paramètres ;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::replace`:
  Remplace les paramètres courants par un nouvel ensemble ;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::add`: Ajoute des 
  paramètres ;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::get`: Retourne un
  paramètre par son nom ;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::set`: Attribue la
  valeur d'un paramètre nommé ;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::has`: Retourne
  vrai si le paramètre est défini ;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::remove`: Supprime
  un paramètre.

Une instance de la :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`
contient aussi certaines méthodes de filtrage :

* :method:`Symfony\\Component\\HttpFoundation\\Request::getAlpha`: Retourne
  les caractères alphabétiques de la valeur d'un paramètre ;

* :method:`Symfony\\Component\\HttpFoundation\\Request::getAlnum`: Retourne
  les caractères alphanumériques de la valeur d'un paramètre ;

* :method:`Symfony\\Component\\HttpFoundation\\Request::getDigits`: Retourne
  les chiffres de la valeur d'un paramètre ;

* :method:`Symfony\\Component\\HttpFoundation\\Request::getInt`: Retourne
  la valeur d'un paramètre convertie en entier ;

* :method:`Symfony\\Component\\HttpFoundation\\Request::filter`: Filtre la valeur
  d'un paramètre en utilisant la fonction PHP ``filter_var()``.

Tous les accesseurs (« getters » en anglais) prennent jusqu'à trois arguments :
le premier est le nom du paramètre, le second la valeur par défaut si le
paramètre n'existe pas::

    // la chaîne de requête est '?foo=bar'

    $request->query->get('foo');
    // retourne bar

    $request->query->get('bar');
    // retourne null

    $request->query->get('bar', 'bar');
    // retourne 'bar'


Quand PHP importe la requête, il utilise des paramètres comme ``foo[bar]=bar``
d'une manière spéciale en créant un tableau. Vous pouvez ainsi utiliser le 
paramètre ``foo`` pour accéder au tableau contenant l'élément ``bar``. Mais 
parfois, vous ne voulez que la valeur du paramètre avec son nom original:
``foo[bar]``. C'est possible à l'aide des accesseurs du `ParameterBag` à l'aide
de la méthode :method:`Symfony\\Component\\HttpFoundation\\Request::get` en
utilisant un troisième argument::

        // la chaîne de requête est '?foo[bar]=bar'

        $request->query->get('foo');
        // retourne array('bar' => 'bar')

        $request->query->get('foo[bar]');
        // retourne null

        $request->query->get('foo[bar]', null, true);
        // retourne 'bar'

.. _component-foundation-attributes:

Dernier point, et non des moindres, vous pouvez aussi stocker des données
additionnelles à l'intérieur de la requête, grâce à la propriété publique
``attribut``, qui est elle-même une instance de la classe
:class:`Symfony\\Component\\HttpFoundation\\ParameterBag`. Ceci est le plus
souvent utilisé pour attacher des informations qui seront utiles tout au long de
la Requête et dont l'accès sera disponible à différents endroits de votre
application. Pour savoir comment utiliser cette propriété à l'intérieur du 
framework, voyez :ref:`en lire plus<book-fundamentals-attributes>`.

Identifier une Requête
~~~~~~~~~~~~~~~~~~~~~~

Si, dans votre application, vous devez identifier une requête, le plus couramment,
cela peut être effectué via les informations « path info » de votre requête,
disponibles à l'aide de la méthode
:method:`Symfony\\Component\\HttpFoundation\\Request::getPathInfo` ::

    // Pour une requête http://example.com/blog/index.php/post/hello-world
    // le path info est « /post/hello-world »
    $request->getPathInfo();

Simuler une Requête
~~~~~~~~~~~~~~~~~~~

A la place de créer une requête basée sur les variables globales PHP, vous pouvez 
simuler une requête::

    $request = Request::create('/hello-world', 'GET', array('name' => 'Fabien'));

La méthode :method:`Symfony\\Component\\HttpFoundation\\Request::create` crée
une requête basée sur les informations de chemin, une méthode et certains
paramètres (les paramètres d'interrogations ou de requête dépendent de la méthode
HTTP utilisée) ; et bien entendu, vous pouvez aussi surcharger toutes ces variables
(Symfony crée des variables par défaut pour toutes les variables PHP globales).

A partir de cette requête, vous pouvez ensuite ré-écrire les variables globales
PHP à l'aide de la méthode 
:method:`Symfony\\Component\\HttpFoundation\\Request::overrideGlobals`::

    $request->overrideGlobals();

.. tip::

    Vous pouvez aussi dupliquer une requête existante via
    :method:`Symfony\\Component\\HttpFoundation\\Request::duplicate` ou
    changer quelques paramètres grâce à un seul appel à la méthode
    :method:`Symfony\\Component\\HttpFoundation\\Request::initialize`.

Accéder à la Session
~~~~~~~~~~~~~~~~~~~~

Si vous avez une session attachée à la requête, vous pouvez y accéder à l'aide de
la méthode :method:`Symfony\\Component\\HttpFoundation\\Request::getSession`;
La méthode
:method:`Symfony\\Component\\HttpFoundation\\Request::hasPreviousSession`
vous informe sur l'existence d'une session démarrée dans une requête antérieure.

Accèder aux données Headers `Accept-*`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez facilement accéder aux données des Headers ``Accept-*``
en utilisant les méthodes suivantes:

* :method:`Symfony\\Component\\HttpFoundation\\Request::getAcceptableContentTypes`:
  renvoie la liste des types de contenu acceptés par ordre décroissant de qualité;

* :method:`Symfony\\Component\\HttpFoundation\\Request::getLanguages`:
  renvoie la liste des langues acceptées par ordre décroissant de qualité;

* :method:`Symfony\\Component\\HttpFoundation\\Request::getCharsets`:
  renvoie la liste des langues acceptées par ordre décroissant de qualité;

.. versionadded:: 2.2
	La classe :class:`Symfony\\Component\\HttpFoundation\\AcceptHeader` est nouvelle en Symfony 2.2.
  
Si vous avez besoin d'avoir un accès complet aux données parsées de ``Accept``, ``Accept-Language``,
``Accept-Charset`` or ``Accept-Encoding``, vous pouvez utilisé
:class:`Symfony\\Component\\HttpFoundation\\AcceptHeader` utility class::

	use Symfony\Component\HttpFoundation\AcceptHeader;

    $accept = AcceptHeader::fromString($request->headers->get('Accept'));
    if ($accept->has('text/html') {
        $item = $accept->get('html');
        $charset = $item->getAttribute('charset', 'utf-8');
        $quality = $item->getQuality();
    }

    // Les éléments accepts sont triés par ordre décroissant de qualité
    $accepts = AcceptHeader::fromString($request->headers->get('Accept'))->all();

Accéder à d'autres données
~~~~~~~~~~~~~~~~~~~~~~~~~~

La classe « Request » contient de nombreuses autres méthodes utilisables pour accéder
aux informations la concernant. Jetez un oeil à l'API pour de plus amples
informations à leur propos.

Réponse
-------

L'objet :class:`Symfony\\Component\\HttpFoundation\\Response` contient toutes
les informations qui seront utiles lors de l'envoi de la réponse au client pour
une requête donnée. Le constructeur prend jusqu'à trois arguments : le contenu de
la réponse, le code du statut, et un tableau conprenant les en-têtes HTTP (« HTTP
headers » en anglais)::

    use Symfony\Component\HttpFoundation\Response;

    $response = new Response('Content', 200, array('content-type' => 'text/html'));

Ces informations peuvent aussi être manipulées après la création de l'objet Response::

    $response->setContent('Hello World');

    // L'attribut public « headers » est aussi un ResponseHeaderBag
    $response->headers->set('Content-Type', 'text/plain');

    $response->setStatusCode(404);

Quand vous annoncez le ``Content-Type`` de la réponse, vous pouvez attribuer le
« charset » (« jeu de caractères » en français), mais il est conseillé de l'indiquer
via la méthode
:method:`Symfony\\Component\\HttpFoundation\\Response::setCharset` ::

    $response->setCharset('ISO-8859-1');

Notez que par défaut, Symfony assume que vos réponses sont encodées en UTF-8.

Envoyer la réponse
~~~~~~~~~~~~~~~~~~

Avant d'envoyer la réponse, vous devez vous assurer qu'elle est conforme avec les
les spécifications HTTP en appelant la méthode
:method:`Symfony\\Component\\HttpFoundation\\Response::prepare`::

    $response->prepare($request);

Envoyez la réponse n'est ensuite qu'un simple appel à la méthode
:method:`Symfony\\Component\\HttpFoundation\\Response::send`::

    $response->send();

Définir les Cookies
~~~~~~~~~~~~~~~~~~~~

Les cookies utilisés dans la réponse peuvent être manipulés via l'attribut
public ``headers``::

    use Symfony\Component\HttpFoundation\Cookie;

    $response->headers->setCookie(new Cookie('foo', 'bar'));

La méthode
:method:`Symfony\\Component\\HttpFoundation\\ResponseHeaderBag::setCookie`
prend une instance de la classe
:class:`Symfony\\Component\\HttpFoundation\\Cookie` comme argument.

Vous pouvez effacer un cookie à l'aide de la méthode
:method:`Symfony\\Component\\HttpFoundation\\Response::clearCookie`.

Gestion du cache HTTP
~~~~~~~~~~~~~~~~~~~~~

La classe :class:`Symfony\\Component\\HttpFoundation\\Response` possède de 
nombreuses méthodes permettant de manipuler les en-têtes HTTP en relation avec
le cache :

* :method:`Symfony\\Component\\HttpFoundation\\Response::setPublic` ;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setPrivate` ;
* :method:`Symfony\\Component\\HttpFoundation\\Response::expire` ;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setExpires` ;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setMaxAge` ;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setSharedMaxAge` ;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setTtl` ;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setClientTtl` ;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setLastModified` ;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setEtag` ;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setVary`.

La méthode :method:`Symfony\\Component\\HttpFoundation\\Response::setCache`
peut être utilisée afin de définir les informations les plus triviales en un
seul appel::

    $response->setCache(array(
        'etag'          => 'abcdef',
        'last_modified' => new \DateTime(),
        'max_age'       => 600,
        's_maxage'      => 600,
        'private'       => false,
        'public'        => true,
    ));

Afin de vérifier que les validateurs de la réponse (``ETag``, ``Last-Modified``)
correspondent aux valeurs conditionnelles indiquées dans la requête client, vous
pouvez utiliser la méthode 
:method:`Symfony\\Component\\HttpFoundation\\Response::isNotModified`::

    if ($response->isNotModified($request)) {
        $response->send();
    }

Si la réponse n'est pas modifiée, le code de statut indiqué sera 304 et le contenu
sera supprimé.

Rediriger l'utilisateur
~~~~~~~~~~~~~~~~~~~~~~~

Afin de rediriger le client vers une autre URL, vous pouvez utilisez la classe
:class:`Symfony\\Component\\HttpFoundation\\RedirectResponse`::

    use Symfony\Component\HttpFoundation\RedirectResponse;

    $response = new RedirectResponse('http://example.com/');

Session
-------

Les informations concernant la session se trouvent dans leur propre document : :doc:`/components/http_foundation/sessions`.

.. _Packagist: https://packagist.org/packages/symfony/http-foundation
