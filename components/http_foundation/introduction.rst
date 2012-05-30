.. index::
   single: HTTP
   single: HttpFoundation

Le composant HttpFoundation
===========================

    Le composant HttpFoundation défini une couche orienté objet pour la
    spécification HTTP.

Avec PHP, la requête est représentée par certaines variables globales (``$_GET``,
``$_POST``, ``$_FILE``, ``$_COOKIE``, ``$_SESSION``...) et la réponse est générée
à l'aide de fonctions (``echo``, ``header``, ``setcookie``, ...).

Le composant Symfony2 HttpFoundation remplace ces variables par défauts et ces 
fonctions par un ensemble Orienté Objet.

Installation
------------

Vous pouvez installer le composant de différentes façon :

* En utilisant le dépot officiel Git (https://github.com/symfony/HttpFoundation);
* En l'installant à l'aide de PEAR ( `pear.symfony.com/HttpFoundation`);
* En l'installant à l'aide de Composer (`symfony/http-foundation` sur Packagist).

Requête
-------

La manière la plus commune pour créer une requete est de l'initialiser à l'aide
des vairaibles GLobales en cours de validité avec 
:method:`Symfony\\Component\\HttpFoundation\\Request::createFromGlobals`::

    use Symfony\Component\HttpFoundation\Request;

    $request = Request::createFromGlobals();

qui est l'équivalent de la forme plus verbeuse mais aussi plus flexible de l'appel
:method:`Symfony\\Component\\HttpFoundation\\Request::__construct` ::

    $request = new Request($_GET, $_POST, array(), $_COOKIE, $_FILES, $_SERVER);

Accéder aux données provenant de la requête
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Un objet requête contient les informations concernant le client. Ces infomations
peuvent être consulter via plusieurs propriétés publques:

* ``request``: équivalent à ``$_POST``;

* ``query``: équivalent à ``$_GET`` (``$request->query->get('name')``);

* ``cookies``: équivalent à ``$_COOKIE``;

* ``attributes``: n'a pas d'équivalent - c'est un propriété utilisée par votre
                  application pour conserver certaines données (voir :ref:`below<component-foundation-attributes>`)

* ``files``: équivalent à ``$_FILE``;

* ``server``: équivalent à ``$_SERVER``;

* ``headers``: globalement équivalent à un sous ensemeble du tableau ``$_SERVER``
  (``$request->headers->get('Content-Type')``).

Chaque propriété est une instance (ou une sous classe) de
:class:`Symfony\\Component\\HttpFoundation\\ParameterBag`, qui est la classe de
conservation des données:

* ``request``: :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`;

* ``query``:   :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`;

* ``cookies``: :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`;

* ``attributes``: :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`;

* ``files``:   :class:`Symfony\\Component\\HttpFoundation\\FileBag`;

* ``server``:  :class:`Symfony\\Component\\HttpFoundation\\ServerBag`;

* ``headers``: :class:`Symfony\\Component\\HttpFoundation\\HeaderBag`.

Toutes les instances de :class:`Symfony\\Component\\HttpFoundation\\ParameterBag` 
ont des méthodes pour consulter et mettre à jour les données incluses:

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::all`: Retourne les
  paramètres;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::keys`: Retourne
  les clés des paramètres;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::replace`:
  Remplace les paramètres courants par un nouvel ensemble;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::add`: Ajoute des 
  paramètres;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::get`: Retourne a
  paramètre par son nom;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::set`: Attribut la
  valeuar d'un paramètre nommé;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::has`: Retourne
  vrai si le paramètre est défini;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::remove`: Supprime
  un paramètre.

Une instance de la :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`
contient aussi certaines méthodes de filtrage:

* :method:`Symfony\\Component\\HttpFoundation\\Request::getAlpha`: Retourne
  les caractères alphabétiques de la valeur d'un paramètre;

* :method:`Symfony\\Component\\HttpFoundation\\Request::getAlnum`:  Retourne
  les caractères alpha-numériques de la valeur d'un paramètre;

* :method:`Symfony\\Component\\HttpFoundation\\Request::getDigits`: Retourne
  les chiffres de la valeur d'un paramètre;

* :method:`Symfony\\Component\\HttpFoundation\\Request::getInt`: Retourne
  la valeur d'un paramètre convertie en entier;

* :method:`Symfony\\Component\\HttpFoundation\\Request::filter`: Filter la valeur
  d'un paramètre en utilisant la fonction PHP ``filter_var()``

Tous les accesseurs (getters) prennent jusqu'à trois arguments: le premier est le
nom du paramètre, le second la valeur par défaut si le parmètre n'existe pas::

    // la chaine de requête est '?foo=bar'

    $request->query->get('foo');
    // retourne bar

    $request->query->get('bar');
    // retourne null

    $request->query->get('bar', 'bar');
    // retourne 'bar'


Quand PHP importe la requête, il utilisise des paramètres comme ``foo[bar]=bar``
d'une manière spéciale en créant un tableau. Vous pouvez ainsi utiliser le 
paramètre ``foo`` pour accéder au tableau contenant l'élément ``bar``. Mais 
parfois, vous ne voulez que la valeur du paramètre avec son nom original:
``foo[bar]``. C'est possible à l'aide des accesseurs du `ParameterBag` à l'aide
de la méthod :method:`Symfony\\Component\\HttpFoundation\\Request::get` en
utilisant un troisième argument::

        // la chaine de requête est '?foo[bar]=bar'

        $request->query->get('foo');
        // retourne array('bar' => 'bar')

        $request->query->get('foo[bar]');
        // retourne null

        $request->query->get('foo[bar]', null, true);
        // retourne 'bar'

.. _component-foundation-attributes:

Dernier point, et non le moindre, vous pouvez aussi stocker des données
additionnelles à l'intérieur de la requête, grace à la propriété public 
``attribut``, qui est elle même une instance de la classe 
:class:`Symfony\\Component\\HttpFoundation\\ParameterBag`. Ceci est le plus
souvent utilisé pour attacher des informations qui seront utiles tout au long de
la  Requête et dont l'accès sera disponible dans différents points de votre
application. Pour savoir comment utiliser cette propriété à l'intérieur du 
framework, voyez :ref:`read more<book-fundamentals-attributes>`.

Identifier une Requête
~~~~~~~~~~~~~~~~~~~~~~

Si dans votre application, vous devez identifiez une requête; le plus couramment,
cela peut être effectué via les informations "path info" de votre requête, 
disponibles à l'aide de la méthode 
:method:`Symfony\\Component\\HttpFoundation\\Request::getPathInfo` ::

    // Pour une requete http://example.com/blog/index.php/post/hello-world
    // le path info est "/post/hello-world"
    $request->getPathInfo();

Simuler une Requête
~~~~~~~~~~~~~~~~~~~

A la place de créer une requête basée sur les variables globales PHP, vous pouvez 
simuler une requête::

    $request = Request::create('/hello-world', 'GET', array('name' => 'Fabien'));

La méthode :method:`Symfony\\Component\\HttpFoundation\\Request::create` crée
une requête basé sur les informations de chemin, une méthode et certains
paramètres (les paramètres d'intérrogations ou de requête dépendent de la méthode
HTTP utilisée); et bien entendu,vous pouvez aussi surcharger toutes ces variables
(par défaut, Symfony crée des variables par défaut pour toutes les variables PHP
globales).

A partir de cette requête, vous pouvez ensuite réécrire les variables globales
PHP à l'aide de la méthode 
:method:`Symfony\\Component\\HttpFoundation\\Request::overrideGlobals`::

    $request->overrideGlobals();

.. tip::

    You can also duplicate an existing query via
    :method:`Symfony\\Component\\HttpFoundation\\Request::duplicate` or
    change a bunch of parameters with a single call to
    :method:`Symfony\\Component\\HttpFoundation\\Request::initialize`.

Accéder à la Session
~~~~~~~~~~~~~~~~~~~~

Si vous avez une session attaché à la requête, vous pouvez y acceder à l'aide de
la méthode :method:`Symfony\\Component\\HttpFoundation\\Request::getSession`;
La méthode
:method:`Symfony\\Component\\HttpFoundation\\Request::hasPreviousSession`
vous informe sur la précédence d'une session dans une requête antérieure.

Accéder à d'autres données
~~~~~~~~~~~~~~~~~~~~~~~~~~

La classe Request contient de nombreuses autres méthodes utilisable accéder aux 
informations la concernant. Jeter un oeil à l'API pour de plus amples 
informations à leur propos.

Response
--------

L'objet :class:`Symfony\\Component\\HttpFoundation\\Response` contient toutes
les informations qui seront utiles lors de l'envoi de la reponse au client pour
une requête donnée. Le constructeur prend jusqu'à trois arguments: le contenu de 
la réponse, le code du status, et un tableau conpremant les entêtes HTTP (HTTP
headers)::

    use Symfony\Component\HttpFoundation\Response;

    $response = new Response('Content', 200, array('content-type' => 'text/html'));

Ces informations peuvent être aussi manipuler après la création de l'objet Response::

    $response->setContent('Hello World');

    // L'attribut public headers est aussi un ResponseHeaderBag
    $response->headers->set('Content-Type', 'text/plain');

    $response->setStatusCode(404);

Quand vous annoncé le ``Content-Type`` de la réponse, vous pouvez attribuer le
charset, mais il est conseillé de l'indiquer via la méthode
:method:`Symfony\\Component\\HttpFoundation\\Response::setCharset` ::

    $response->setCharset('ISO-8859-1');

Note que par défaut, Symfony assumes que vos réponses sont encodés en UTF-8.

Envoyer la réponse
~~~~~~~~~~~~~~~~~~

Avant d'envoyer la réponse, vous devez vous assurer qu'elle est conforme avec les
les spécifications HTTP en appelant la méthode
:method:`Symfony\\Component\\HttpFoundation\\Response::prepare`::

    $response->prepare($request);

Envoyer la réponse n'est ensuite qu'un simple à la méthode
:method:`Symfony\\Component\\HttpFoundation\\Response::send`::

    $response->send();

Définir les Cookies
~~~~~~~~~~~~~~~~~~~~

Les cookies utilisée dans la réponse peuvent être manipulé à travers l'attribut 
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
nombreuses méthodes permettant de manipuler les entêtes HTTP en relation avec
le cache:

* :method:`Symfony\\Component\\HttpFoundation\\Response::setPublic`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setPrivate`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::expire`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setExpires`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setMaxAge`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setSharedMaxAge`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setTtl`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setClientTtl`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setLastModified`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setEtag`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setVary`;

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
correspondent au valeurs conditionnelles indiquées dans la requête client, vous
pouvez utiliser la méthode 
:method:`Symfony\\Component\\HttpFoundation\\Response::isNotModified`::

    if ($response->isNotModified($request)) {
        $response->send();
    }

Si la réponse n'est pas modifiée, le code de status indiqué sera 304 et le contenu 
sera supprimé.

Redirigé l'utilisateur
~~~~~~~~~~~~~~~~~~~~~~

Afin de redirigé le client vers une autre URL, vous pouvez utilisez la classe
:class:`Symfony\\Component\\HttpFoundation\\RedirectResponse`::

    use Symfony\Component\HttpFoundation\RedirectResponse;

    $response = new RedirectResponse('http://example.com/');

Session
-------

TBD -- Cette partie n'est pas actuellement écrite et sera certainement retravaillée
dans Symfony en version 2.1.