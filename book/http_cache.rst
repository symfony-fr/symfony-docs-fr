.. index::
   single: Cache

Le Cache HTTP
==========

Le propre d'une application web riche est d'être dynamique. Peu
importe l'efficacité de votre application, le traitement d'une requête
sera toujours plus important que l'envoi d'une page statique.

Pour la plupart des applications web, cela ne pose pas de
problème. Symfony2 est d'une rapidité foudrayante, et à moins que vous
ne fassiez de sérieux remaniements, chaque requête sera traitée
rapidement sans trop "stresser" sur votre serveur.

Mais si la fréquentation de votre site augmente, ce traitement peut devenir un
problème. Le processus qui s'effectue à chaque requête
peut être exécuter une unique fois. C'est exactement l'objectif de la
mise en cache.

La Mise en cache
----------------------------------

Le moyen le plus efficace d'améliorer les performances d'une
application est mettre en cache l'intégralité d'une réponse pour ne plus avoir à rappeler l'application pour les requêtes suivantes. Bien
sûr, ce n'est pas toujours possible pour les sites web fortement
dynamiques. A travers ce chapitre, nous allons
décrire comment fonctionne le système de cache de Symfony2 et
pourquoi nous pensons que c'est la meilleur approche possible.

Le système de cache de Symfony2 est différent car il se base sur la
simplicité et la puissance du cache HTTP tel qu'il est défini dans les
:term:`HTTP specification`. Au lieu de réinventer la méthodologie de
la mise en cache, Symfony2 adopte la norme qui définit la
communication élémentaire sur le Web. Une fois que vous avez compris
les fondamentaux de la validation HTTP et de l'expiration de la mise
en cache de modèles, vous serez prêt à maîtriser le système de cache de
Symfony2.

Nous allons parcourir ce sujet en quatre étapes :

* **Etape 1**: Une :ref:`passerelle de cache <gateway-caches>`, ou
    reverse proxy, est une couche indépendante qui se trouve devant
    votre application. Le passerelle mais en cache les réponses telles
    qu'elles sont retournées par l'application et répond aux requêtes
    dont les réponses sont en cache avant qu'elles n'atteignent
    l'application. Symfony2 possède sa propre passerelle par défaut,
    mais toute technologie peut être utiliser.

* **Etape 2**: Les en-têtes du :ref:`cache HTTP
    <http-cache-introduction>` sont utilisées pour communiquer avec la
    passerelle de cache et tout autre cache entre votre application et
    le client. Symfony2 propose par défaut par des valeurs 
    raisonnables et fournit interface riche pour intéragir avec
    les en-têtes de cache.

* **Etape 3**: :ref:`L'expiration et la validation
  <http-expiration-validation>` sont les deux modèles utilisés pour
  déterminer si le contenu d'un cache est *valide* (peut être
  réutilisé à partir du cache) ou *périmé* (doit être regénérer par
  l'application).

* **Etape 4**: :ref:`Edge Side Includes <edge-side-includes>` (ESI)
    autorise le cache HTTP à mettre en cache des
    fragments de pages (voir des fragments imbriqués) de façon
    indépendante. Avec l'ESI, vous pouvez même mettre en cache une
    page entière pendant 60 minutes, mais un bloc imbriqué dans cette
    page uniquement 5 minutes.

La mise en cache via HTTP n'est pas réservée à Symfony, beaucoup
d'articles existent à ce sujet. Si vous n'êtes pas familié avec la
mise cache par HTTP, nous vous recommandons *chaudement* l'article de
Ryan Tomayko `Things Caches Do`_. Une autre ressource appronfondie sur
ce sujet est le tutoriel de Mark Nottingham, `Cache Tutorial`_.

.. index::
   single: Cache; Proxy
   single: Cache; Reverse Proxy
   single: Cache; Gateway

.. _gateway-caches:

La mise en cache avec la Passerelle de Cache
----------------------------

Lors d'une mise en cache via HTTP, le *cache* est complétement séparé
de votre application. Il est placé entre votre application et le client
effectuant des requêtes.

Le travail du cache est d'accepter les requêtes du client et de les
transmettre à votre application. Le cache recevra aussi en retour des
réponses de votre application et les enverra au client. Le cache est au milieu
("middle-man") dans ce jeu de commmunication requête-réponse
entre le client et votre application.

Lors d'une communication, le cache stockera toutes les réponses qu'ils
estimes "stockable" (voir :ref:`http-cache-introduction`). Si la même
ressource est demandée, le cache renvoie le contenu mis en cache au
client, en ignorant entièrement l'application.

Ce type de cache est connu sous le nom de passerelle de cache
HTTP. Beaucoup d'autres solutions existent telles que `Varnish`_,
`Squid in reverse proxy mode`_ et le reverse proxy de Symfony2.

.. index::
   single: Cache; Types of

Les types de caches
~~~~~~~~~~~~~~~

Mais une passerelle de cache ne possède pas qu'un seul type de
cache. Les en-têtes de cache HTTP envoyé par votre application sont
interprétés par trois différents types de cache :

* *Le cache du navigateur* : tous les navigateurs ont leur propre
  cache qui est utile quand un utilisateur demande la page précédente
  ou des images et autres médias. Le cache du navigateur est privé car
  les ressources stockées ne sont pas partagées avec d'autres
  applications.

* *Le "cache proxy"* : un proxy est un cache *partagé* car plusieurs
  applications peut se placer derrière un seul proxy. Il est
  habituellement installé par les entreprises pour diminuer le temps
  de réponse des sites et la consommation des ressources réseaux.

* *Passerelle de cache* : comme une proxy, ce système de cache est
  également partagé mais du côté du serveur. Installé par des
  administrateurs réseau, il permet aux sites d'être plus extensibles,
  sûrs et performants.

.. tip::

    Les passerelles de cache peuvent être désignées comme des "reverse
    proxy", "surrogate proxy" ou des accélérateurs HTTP.

.. note::

    La signification de cache privé par rapport au cache partagé sera
    expliqué plus en détails lorsque nous verrons les contenus liés à
    exactement un utilisateur (les informations sur un compte
    utilisateur par exemple).

Toutes les réponses de l'application iront communément dans un ou deux
des deux premiers types de cache ou l'autre des deux types de
cache. Ces systèmes ne sont pas contrôlés par l'application mais
suivent les directives du cache HTTP définies dans les réponses.

.. index::
   single: Cache; Symfony2 Reverse Proxy

.. _`symfony-gateway-cache`:

Symfony2 Reverse Proxy
~~~~~~~~~~~~~~~~~~~~~~

Symfony2 contient un reverse proxy (aussi appelé passerelle de cache)
écrit en PHP. Son activation entrainera la mise en cache immédiate des
réponses stockables de l'application. L'installé est si simple. Chaque
nouvelle application Symfony2 contient un noyau pré-configuré
(AppCache) qui encapsule le noyau par défault (AppKernel). Le cache
noyau *est* le reverse proxy.

Pour activer le mécanisme de cache, il faut modifier le code du
contrôleur principal pour qu'il utilise le cache noyau : ::

    // web/app.php

    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';
    require_once __DIR__.'/../app/AppCache.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    // wrap the default AppKernel with the AppCache one
    $kernel = new AppCache($kernel);
    $kernel->handle(Request::createFromGlobals())->send();

Le cache noyau se comportera immédiatement comme un "reverse proxy" en
mettant en cache les réponses de l'application et en les renvoyant au
client.

.. tip::

    Le cache Noyau a une méthode spéciale ``getLog()`` qui retourne
    une chaine de caractère décrivant ce qui se passe dans la couche
    du cache. Dans l'environnement de développement, il est possible
    de l'utiliser pour du débogage ou afin de valider votre stratégie
    de mise en cache : ::

        error_log($kernel->getLog());

L'objet ``AppCache`` a un configuration par défaut raisonnable mais
peut être reconfigurer finement grâce à une série d'options que vous
pouvez paramètrer en surchargeant la méthode ``getOptions()`` : ::

    // app/AppCache.php
    class AppCache extends Cache
    {
        protected function getOptions()
        {
            return array(
                'debug'                  => false,
                'default_ttl'            => 0,
                'private_headers'        => array('Authorization', 'Cookie'),
                'allow_reload'           => false,
                'allow_revalidate'       => false,
                'stale_while_revalidate' => 2,
                'stale_if_error'         => 60,
            );
        }
    }

.. tip::

    A moins que la méthode ``getOptions()`` soit surchargée, l'option
    ``debug`` est mise automatiquement à la valeur ``débogage`` du
    ``AppKernel`` encapsulé.

Voici une liste des principales options :

* ``default_ttl`` : Le nombre de seconde pendant laquelle une entrée du
  cache devrait être considérée comme "valide" quand il n'y a pas
  d'information explicite fournie dans une réponse. Une valeur
  explicite pour les en-têtes ``Cache-Control`` ou ``Expires``
  surcharge cette valeur (par défaut : ``0``);


* ``private_headers`` : Type d'en-têtes de requête qui déclenche le
  comportement "privé" du ``Cache-Control`` pour les réponses qui ne
  spécifie pas leur état, c'est à dire, si la réponse est ``public``
  ou ``private`` via une directive du ``Cache-Control``. (par défaut
  : ``Authorization`` et ``Cookie``);

* ``allow_reload`` : Définit si le client peut forcer ou non un rechargement du cache en incluant une directive du ``Cache-Control`` "no-cache" dans la requête. Définit à ``true`` pour la conformité avec la RFC 2616 (par défaut : ``false``);

* ``allow_revalidate`` : Définit si le client peut forcer une revalidation du cache en incluant une directive de ``Cache-Control`` "max-age=0" dans la requête. Défini à ``true`` pour la conformité avec la RFC 2616 (par defaut : ``false``);

* ``stale_while_revalidate`` : Spécifie le nombre de secondes par
  défaut (la granularité est la seconde parce que le TTL de la réponse
  est en seconde) pendant lesquelles le cache peut renvoyer une
  réponse "périmée" alors que la nouvelle réponse est calculée en
  arrière-plan (par défaut : ``2``). Ce paramètre est surchargé par
  l'extension HTTP ``stale-while-revalidate`` du ``Cache-Control``
  (cf. RFC 5861);

* ``stale_if_error`` : Spécifie le nombre de seconde par défaut (la
  granularité est la seconde) pendant lesquelles le cache peut
  renvoyer une réponse "périmée" quand une erreur est rencontrée (par
  défaut : ``60``). Ce paramètre est surchargé par l'extension HTTP
  ``stale-if-error`` du ``Cache-Control`` (cf. RFC 5961).

SI le paramètre ``debug`` est à ``true``, Symfony2 ajoute
automatiquement l'en-têtes ``X-Symfony-Cache`` à la réponse contenant
des informations utiles à propos des cache "hits" (utilisation du
cache) et "misses" (page ou réponse non présente en cache).

.. sidebar:: Passer d'un Reverse Proxy à un autre

   Le reverse proxy de Symfony2 est un formidable outil lors de la
   phase de développement de votre site web ou lors d'un déploiement
   sur des serveurs mutualisés sur lesquels il n'est pas possible
   d'installer d'autres outils que ceux proposés par PHP. Mais il
   n'est pas aussi performant que des proxy écrits en C. C'est
   pourquoi il est fortement recommandé d'utiliser Varnish ou Squid
   sur les serveurs de production si possible. La bonne nouvelle est
   qu'il est très simple de passer d'un proxy à un autre sans
   qu'aucune modification ne soit nécessaire dans le code. Vous pouvez
   commencez avec le reverse proxy de Symfony2 puis le mettre à jour
   plus tard vers Varnish quand votre traffic augmentera.

   Pour plus d'information concernant Varnish avec Symfony2, veuillez
   vous reportez au chapitre du cookbook :doc:`How to use Varnish
   </cookbook/cache/varnish>`.

.. note::

    Les performances du reverse proxy de Symfony2 ne sont pas liées à
    la complexité de votre application. C'est parce que le noyau de
    m'application n'est démarré que quand la requête lui est
    transmise.

.. index::
   single: Cache; HTTP

.. _http-cache-introduction:

Introduction à la mise en cache avec HTTP
----------------------------

Pour tirer partie des couches de gestion du cache, l'application doit
être capable de communiquer quelles réponses peuvent être mises en
cache et les règles qui décident quand et comment le cache devient
obsolète. Cela se fait en définissant des en-têtes de gestion de cache
HTTP dans la réponse.

.. tip::

    Il faut garder à l'esprit que "HTTP" n'est rien d'autre que le
    langage (un simple langage texte) que les clients web (les
    navigateurs par exemple) et les serveurs utilisent pour
    communiquer entre eux. Parler de mise en cache HTTP revient à
    parler de la partie du langage qui permet aux clients et aux
    serveurs d'échanger les informations relatives à la gestion du
    cache.

HTTP définit quatre en-têtes spécifiques à la mise en cache des réponse :

* ``Cache-Control``
* ``Expires``
* ``ETag``
* ``Last-Modified``

L'en-tête le plus important et le plus versatile est l'en-tête
``Cache-Control`` qui est en réalité une collection d'informations
diverses sur le cache.

.. note::

    Tous ces en-têtes seront complétement détaillés dans la section
    :ref:`http-expiration-validation`.

.. index::
   single: Cache; Cache-Control Header
   single: HTTP headers; Cache-Control

L'en-tête Cache-Control
~~~~~~~~~~~~~~~~~~~~~~~~

Cet en-tête est unique du fait qu'il contient non pas une, mais un
ensemble varié d'information sur la possiblité de mise en cache d'une
réponse. Chaque information est séparée par une virgule :

     Cache-Control: private, max-age=0, must-revalidate

     Cache-Control: max-age=3600, must-revalidate

Symfony fournit une abstraction du ``Cache-Control`` pour faciliter sa
gestion :

.. code-block:: php

    $response = new Response();

    // mark the response as either public or private
    $response->setPublic();
    $response->setPrivate();

    // set the private or shared max age
    $response->setMaxAge(600);
    $response->setSharedMaxAge(600);

    // set a custom Cache-Control directive
    $response->headers->addCacheControlDirective('must-revalidate', true);

Réponse publique et réponse privée
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les passerelles de cache et les caches "proxy" sont considérés comme
étant "partagés" car leur contenu est partagé par plusieurs
utilisateurs. Si une réponse spécifique à un utilisateur est par
erreur stockée dans ce type de cache, elle pourrait être renvoyée à un
nombre quelconque d'autres utilisateurs. Imaginez si les informations
concernant votre compte sont mises en cache et ensuite envoyées à tous
les utilisateurs suivants qui souhaite accéder à leur page de compte !

Pour gérer cette situation, chaque réponse doit être définie comme
étant publique ou privée :

* *public*: Indique que la réponse peut être mise en cache, à la fois, par les caches privés et les caches publiques;

* *private*: Indique que toute la réponse concerne un unique utilisateur et qu'elle ne doit pas être stockée dans les caches publiques.

Symfony considère par défaut chaque réponse comme étant privée. Pour
tirer parti des caches partagés (comme le reverse proxy de Symfony2),
la réponse devra explicitement être définie comme publique.

.. index::
   single: Cache; Safe methods

Méthodes sûres
~~~~~~~~~~~~

La mise en cache HTTP ne fonctionne qu'avec les méthodes "sûres"
(telles que GET et HEAD). "Être sûr" signifie que l'état de
l'application n'est jamais modifié par le serveur au moment de servir
la requête (il est bien-sûr possible de loguer des informations,
mettre en cache des données, etc.). Cela a deux conséquences :

Safe Methods
~~~~~~~~~~~~

HTTP caching only works for "safe" HTTP methods (like GET and HEAD). Being
safe means that you never change the application's state on the server when
serving the request (you can of course log information, cache data, etc).
This has two very reasonable consequences:

* L'état de l'application ne devrait *jamais* être modifié en répondant
  à une requête GET ou HEAD. Même s'il n'y a pas de passerelle de
  cache, la présence d'un cache "proxy" signifie qu'aucune requête
  GET ou HEAD ne pourrait pas atteindre le serveur.

* Ne pas mettre en cache les méthodes PUT, POST ou DELETE. Ces
  méthodes sont normalement utilisées pour changer l'état de
  l'application (supprimer un billet de blog par exemple). La mise en
  cache ces méthodes empêcherait certaine requête d'atteindre et de
  modifier l'application.

Règles de mise en cache et configuration par défaut
~~~~~~~~~~~~~~~~~~~~~~~~~~

HTTP 1.1 permet de tout mettre en cache par défaut à moins qu'il n'y
ait un en-tête ``Cache-Control``. En pratique, la plus part des
systèmes de cache ne font rien quand les requêtes contiennent un
cookie, ont un en-tête d'autorisation, utilisent une méthode non sûr
(i.e. PUT, POST, DELETE), ou quand les réponses ont un code de
redirection.

Symfony2 définit automatiquement une configuration de l'en-tête
Cache-Control quand aucun n'est défini par le développeur en suivant
ces règles :

* Si aucun en-tête de cache n'est défini (``Cache-Control``, ``Expires``, ``Etag``
  ou ``Last-Modified``), ``Cache-Control`` est défini à ``no-cache``, ce qui veut
  dire que la réponse ne sera pas mise en cache;

* Si ``Cache-Control`` est vide (mais que l'un des autres en-têtes de cache est
  présent) sa valeur est définie à ``private, must-revalidate``;

* Mais si au moins une directive ``Cache-Control`` est définie et
  aucune directive 'publique' ou ``private`` n'a pas été ajoutée
  explicitement, Symfony2 ajoute la directive ``private``
  automatiquement (sauf quand ``s-maxage`` est défini).

.. _http-expiration-validation:

HTTP Expiration et Validation
------------------------------

La spécification HTTP définit deux modèles de mise en cache :

* Avec le `modèle expiration`_, on spécifie simplement combien de
  temps une réponse doit être considérée comme "valide" en incluant un
  en-tête ``Cache-Control`` et/ou ``Expires``. Les système de cache qui
  comprennent les directives n'enverront pas la même requête jusqu'à ce
  que la version en cache devienne "invalide".

* Quand une page est dynamique (i.e. quand son contenu change
  souvent), le `modèle validation`_ est souvent nécessaire. Avec ce
  modèle, le système de cache stocke la réponse mais demande au
  serveur à chaque requête si la réponse est encore
  valide. L'application utilise un identifiant unique (l'en-tête ``Etag``)
  et/ou un timestamp (l'en-tête ``Last-Modified``) pour vérifier si la
  page a changé depuis sa mise en cache.

Le but des deux modèles est de ne jamais générer deux fois la même
réponse en s'appuyant sur le système de cache pour stoker et renvoyer
la réponse valide.

.. sidebar:: En lisant la spécification HTTP

    La spécification HTTP définit un langage simple mais puissant dans
    lequel les clients et les serveurs peuvent communiquer. En tant
    que développeur web, le modèle requête-réponse est le plus
    populaire. Malheureusement, le document de spécification - `RFC 2616`_ - 
    peut être difficile à lire.

    Il existe actuellement une tentative (`HTTP Bis`_) de réécriture
    de la RFC 2616.  Elle ne décrit pas une nouvelle version du HTTP
    mais clarifie plutôt la spécification originale du HTTP. Elle est
    découpée en sept parties ; tout ce qui concerne la gestion du
    cache se retrouve dans deux chapitres dédiés (`P4 - Conditional
    Requests`_ et `P6 - Caching: Browser and intermediary caches`_).

    En tant que développeur web, il est fortement recommandé de lire
    la spécification. Ca clarté et sa puissance - mais dix ans après
    sa création - est inestimable. Ne soyez pas répulser par
    l'apparence de la spec - son contenu est beaucoup plus beau que sa
    couverture.

.. index::
   single: Cache; HTTP Expiration

Expiration
~~~~~~~~~~

Le modèle d'expiration du cache est le plus efficace et le plus simple
à mettre en place et devrait être utilisé dès que possible. Quand une
réponse est mise en cache avec une directive d'expiration, le cache
stockera la réponse et la renverra directement sans solliciter
l'application avant son expiration.

Ce modèle est mis en oeuvre avec deux en-têtes HTTP presque identiques :
``Expires`` ou ``Cache-Control``.

.. index::
   single: Cache; Expires header
   single: HTTP headers; Expires

Expiration avec l'en-tête ``Expires``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

D'après la spécification HTTP, "les champs de l'en-tête ``Expires``
donne la date après laquelle la réponse est considérée comme
invalide". Cet en-tête peut être définit avec la méthode ``setExpires()``
de l'objet ``Response``. Elle prend un objet ``DateTime`` en argument : ::

    $date = new DateTime();
    $date->modify('+600 seconds');

    $response->setExpires($date);

L'en-tête HTTP résultante sera : ::

    Expires: Thu, 01 Mar 2011 16:00:00 GMT

.. note::

    La méthode ``setExpires()`` convertit automatiquement la date au
    format GMT comme demandé par la spécification.

L'en-tête ``Expires`` souffre de deux limitations. D'abord, l'heure du
serveur web et celle du serveur de cache (le navigateur par exemple)
doivent être synchronisées. Aussi, la spécification déclare que "les
serveurs HTTP/1.1 ne devraient pas envoyer des dates ``Expires`` de
plus d'un an dans le futur".

.. index::
   single: Cache; Cache-Control header
   single: HTTP headers; Cache-Control

Expiration avec l'en-tête ``Cache-Control``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

À cause des limitations de l'en-tête ``Expires``, la plus part du temps, il faut utiliser
l'en-tête ``Cache-Control``. Rappelez-vous que l'en-tête ``Cache-Control`` est utilisé pour spécifier beaucoup de différentes directives de cache. Pour le modèle d'expiration, il y a
deux directives, ``max-age`` et ``s-maxage``. La première est utilisée par
tous les systèmes de cache alors que la seconde n'est utilisée que par les
systèmes de cache partagés : ::

    // Sets the number of seconds after which the response
    // should no longer be considered fresh
    $response->setMaxAge(600);

    // Same as above but only for shared caches
    $response->setSharedMaxAge(600);

L'en-tête ``Cache-Control`` devrait être (il peut y avoir d'autres directives) : ::

    Cache-Control: max-age=600, s-maxage=600

.. index::
   single: Cache; Validation

Validation
~~~~~~~~~~

S'il faut mettre à jour une ressource dès qu'il y a un changement dans
les données, le modèle d'expiration ne convient pas. Avec le modèle
d'expiration, l'application ne sera pas appelée jusqu'au moment où le
cache devient invalide.

Le modèle de validation du cache corrige ce problème. Dans ce modèle,
le cache continue de stoker les réponses. La différence est que pour
chaque requête, le cache demande à l'application si la réponse en cache
est encore valide. Si la réponse en cache est encore valide,
l'application renvoie un statut 304 et aucun contenu. Le cache sait
que la réponse en cache est valide.

Ce modèle permet d'économiser beaucoup de bande passante car la même
réponse n'est pas envoyée deux fois au même client (un code 304 est
envoyé à la place). Si l'application est bien construite, il est
possible de déterminer le minimum de données nécessitant l'envoie de
réponse 304 et aussi d'économiser des ressources CPU (voir ci-dessous
pour un exemple d'implémentation).

.. tip::

    Le code 304 signifie "Non modifié". C'est important car la réponse
    associée à ce code ne contient pas le contenu demandé en
    réalité. Au lieu de cela, la réponse est simplement un ensemble
    léger de directives qui informe le cache qu'il devrait utiliser la
    réponse stockée.

Comme avec le modèle d'expiration, il y a deux différents types
d'en-tête HTTP qui peuvent être utilisés pour implémenter ce modèle :
``ETag`` et ``Last-Modified``.

.. index::
   single: Cache; Etag header
   single: HTTP headers; Etag

Validation avec l'en-tête ``ETag``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'en-tête ``ETag`` est une chaîne de caractère (appelée "entity-tag")
qui identifie de façon unique une représentation de la ressource
appelée. Il est entièrement généré et défini par votre application tel
que vous pouvez spécifier, par exemple, si la ressource ``/about``,
stockée en cache, sera mise à jour avec ce que votre application
retourne. Un ``ETag`` est similaire à une empreinte et est utilisé
pour comparer rapidement si deux versions différentes d'une ressource
sont équivalentes. Comme une empreinte, chaque ``ETag`` doit être
unique pour toutes les représentations de la même ressource.

Voici une implémentation simple qui génère l'en-tête ETag depuis un
hachage md5 du contenu : ::

    public function indexAction()
    {
        $response = $this->render('MyBundle:Main:index.html.twig');
        $response->setETag(md5($response->getContent()));
        $response->isNotModified($this->getRequest());

        return $response;
    }

La méthode ``Response::isNotModified()`` compare le ``ETag`` envoyé avec la
requête avec celui défini dans l'objet ``Reponse``. S'ils sont
identiques, la méthode renvoie automatiquement le code 304 en ``Response``.

Cet algorithme est assez simple et très généric, mais il est
nécessaire de créer entièrement la ``Response`` avant de pouvoir
calculer l'en-tête ETag, ce qui n'est pas optimal. En d'autre terme,
cette approche économise la bande passante mais pas l'utilisation du
CPU.

Dans la section :ref:`optimizing-cache-validation`, nous verrons
comment le modèle de validation peut être utiliser plus intelligeament
pour déterminer la validité d'un cache sans faire autant de travail.

.. tip::

    Symfony2 also supports weak ETags by passing ``true`` as the second
    argument to the
    :method:`Symfony\\Component\\HttpFoundation\\Response::setETag` method.

.. index::
   single: Cache; Last-Modified header
   single: HTTP headers; Last-Modified

Validation with the ``Last-Modified`` Header
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``Last-Modified`` header is the second form of validation. According
to the HTTP specification, "The ``Last-Modified`` header field indicates
the date and time at which the origin server believes the representation
was last modified." In other words, the application decides whether or not
the cached content has been updated based on whether or not it's been updated
since the response was cached.

For instance, you can use the latest update date for all the objects needed to
compute the resource representation as the value for the ``Last-Modified``
header value::

    public function showAction($articleSlug)
    {
        // ...

        $articleDate = new \DateTime($article->getUpdatedAt());
        $authorDate = new \DateTime($author->getUpdatedAt());

        $date = $authorDate > $articleDate ? $authorDate : $articleDate;

        $response->setLastModified($date);
        $response->isNotModified($this->getRequest());

        return $response;
    }

The ``Response::isNotModified()`` method compares the ``If-Modified-Since``
header sent by the request with the ``Last-Modified`` header set on the
response. If they are equivalent, the ``Response`` will be set to a 304 status
code.

.. note::

    The ``If-Modified-Since`` request header equals the ``Last-Modified``
    header of the last response sent to the client for the particular resource.
    This is how the client and server communicate with each other and decide
    whether or not the resource has been updated since it was cached.

.. index::
   single: Cache; Conditional Get
   single: HTTP; 304

.. _optimizing-cache-validation:

Optimizing your Code with Validation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The main goal of any caching strategy is to lighten the load on the application.
Put another way, the less you do in your application to return a 304 response,
the better. The ``Response::isNotModified()`` method does exactly that by
exposing a simple and efficient pattern::

    public function showAction($articleSlug)
    {
        // Get the minimum information to compute
        // the ETag or the Last-Modified value
        // (based on the Request, data are retrieved from
        // a database or a key-value store for instance)
        $article = // ...

        // create a Response with a ETag and/or a Last-Modified header
        $response = new Response();
        $response->setETag($article->computeETag());
        $response->setLastModified($article->getPublishedAt());

        // Check that the Response is not modified for the given Request
        if ($response->isNotModified($this->getRequest())) {
            // return the 304 Response immediately
            return $response;
        } else {
            // do more work here - like retrieving more data
            $comments = // ...
            
            // or render a template with the $response you've already started
            return $this->render(
                'MyBundle:MyController:article.html.twig',
                array('article' => $article, 'comments' => $comments),
                $response
            );
        }
    }

When the ``Response`` is not modified, the ``isNotModified()`` automatically sets
the response status code to ``304``, removes the content, and removes some
headers that must not be present for ``304`` responses (see
:method:`Symfony\\Component\\HttpFoundation\\Response::setNotModified`).

.. index::
   single: Cache; Vary
   single: HTTP headers; Vary

Varying the Response
~~~~~~~~~~~~~~~~~~~~

So far, we've assumed that each URI has exactly one representation of the
target resource. By default, HTTP caching is done by using the URI of the
resource as the cache key. If two people request the same URI of a cacheable
resource, the second person will receive the cached version.

Sometimes this isn't enough and different versions of the same URI need to
be cached based on one or more request header values. For instance, if you
compress pages when the client supports it, any given URI has two representations:
one when the client supports compression, and one when it does not. This
determination is done by the value of the ``Accept-Encoding`` request header.

In this case, we need the cache to store both a compressed and uncompressed
version of the response for the particular URI and return them based on the
request's ``Accept-Encoding`` value. This is done by using the ``Vary`` response
header, which is a comma-separated list of different headers whose values
trigger a different representation of the requested resource::

    Vary: Accept-Encoding, User-Agent

.. tip::

    This particular ``Vary`` header would cache different versions of each
    resource based on the URI and the value of the ``Accept-Encoding`` and
    ``User-Agent`` request header.

The ``Response`` object offers a clean interface for managing the ``Vary``
header::

    // set one vary header
    $response->setVary('Accept-Encoding');

    // set multiple vary headers
    $response->setVary(array('Accept-Encoding', 'User-Agent'));

The ``setVary()`` method takes a header name or an array of header names for
which the response varies.

Expiration and Validation
~~~~~~~~~~~~~~~~~~~~~~~~~

You can of course use both validation and expiration within the same ``Response``.
As expiration wins over validation, you can easily benefit from the best of
both worlds. In other words, by using both expiration and validation, you
can instruct the cache to server the cached content, while checking back
at some interval (the expiration) to verify that the content is still valid.

.. index::
    pair: Cache; Configuration

More Response Methods
~~~~~~~~~~~~~~~~~~~~~

The Response class provides many more methods related to the cache. Here are
the most useful ones::

    // Marks the Response stale
    $response->expire();

    // Force the response to return a proper 304 response with no content
    $response->setNotModified();

Additionally, most cache-related HTTP headers can be set via the single
``setCache()`` method::

    // Set cache settings in one call
    $response->setCache(array(
        'etag'          => $etag,
        'last_modified' => $date,
        'max_age'       => 10,
        's_maxage'      => 10,
        'public'        => true,
        // 'private'    => true,
    ));

.. index::
  single: Cache; ESI
  single: ESI

.. _edge-side-includes:

Using Edge Side Includes
------------------------

Gateway caches are a great way to make your website perform better. But they
have one limitation: they can only cache whole pages. If you can't cache
whole pages or if parts of a page has "more" dynamic parts, you are out of
luck. Fortunately, Symfony2 provides a solution for these cases, based on a
technology called `ESI`_, or Edge Side Includes. Akamaï wrote this specification
almost 10 years ago, and it allows specific parts of a page to have a different
caching strategy than the main page.

The ESI specification describes tags you can embed in your pages to communicate
with the gateway cache. Only one tag is implemented in Symfony2, ``include``,
as this is the only useful one outside of Akamaï context:

.. code-block:: html

    <html>
        <body>
            Some content

            <!-- Embed the content of another page here -->
            <esi:include src="http://..." />

            More content
        </body>
    </html>

.. note::

    Notice from the example that each ESI tag has a fully-qualified URL.
    An ESI tag represents a page fragment that can be fetched via the given
    URL.

When a request is handled, the gateway cache fetches the entire page from
its cache or requests it from the backend application. If the response contains
one or more ESI tags, these are processed in the same way. In other words,
the gateway cache either retrieves the included page fragment from its cache
or requests the page fragment from the backend application again. When all
the ESI tags have been resolved, the gateway cache merges each into the main
page and sends the final content to the client.

All of this happens transparently at the gateway cache level (i.e. outside
of your application). As you'll see, if you choose to take advantage of ESI
tags, Symfony2 makes the process of including them almost effortless.

Using ESI in Symfony2
~~~~~~~~~~~~~~~~~~~~~

First, to use ESI, be sure to enable it in your application configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            esi: { enabled: true }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config ...>
            <!-- ... -->
            <framework:esi enabled="true" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'esi'    => array('enabled' => true),
        ));

Now, suppose we have a page that is relatively static, except for a news
ticker at the bottom of the content. With ESI, we can cache the news ticker
independent of the rest of the page.

.. code-block:: php

    public function indexAction()
    {
        $response = $this->render('MyBundle:MyController:index.html.twig');
        $response->setSharedMaxAge(600);

        return $response;
    }

In this example, we've given the full-page cache a lifetime of ten minutes.
Next, let's include the news ticker in the template by embedding an action.
This is done via the ``render`` helper (See `templating-embedding-controller`
for more details).

As the embedded content comes from another page (or controller for that
matter), Symfony2 uses the standard ``render`` helper to configure ESI tags:

.. configuration-block::

    .. code-block:: jinja

        {% render '...:news' with {}, {'standalone': true} %}

    .. code-block:: php

        <?php echo $view['actions']->render('...:news', array(), array('standalone' => true)) ?>

By setting ``standalone`` to ``true``, you tell Symfony2 that the action
should be rendered as an ESI tag. You might be wondering why you would want to
use a helper instead of just writing the ESI tag yourself. That's because
using a helper makes your application work even if there is no gateway cache
installed. Let's see how it works.

When standalone is ``false`` (the default), Symfony2 merges the included page
content within the main one before sending the response to the client. But
when standalone is ``true``, *and* if Symfony2 detects that it's talking
to a gateway cache that supports ESI, it generates an ESI include tag. But
if there is no gateway cache or if it does not support ESI, Symfony2 will
just merge the included page content within the main one as it would have
done were standalone set to ``false``.

.. note::

    Symfony2 detects if a gateway cache supports ESI via another Akamaï
    specification that is supported out of the box by the Symfony2 reverse
    proxy.

The embedded action can now specify its own caching rules, entirely independent
of the master page.

.. code-block:: php

    public function newsAction()
    {
      // ...

      $response->setSharedMaxAge(60);
    }

With ESI, the full page cache will be valid for 600 seconds, but the news
component cache will only last for 60 seconds.

A requirement of ESI, however, is that the embedded action be accessible
via a URL so the gateway cache can fetch it independently of the rest of
the page. Of course, an action can't be accessed via a URL unless it has
a route that points to it. Symfony2 takes care of this via a generic route
and controller. For the ESI include tag to work properly, you must define
the ``_internal`` route:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        _internal:
            resource: "@FrameworkBundle/Resources/config/routing/internal.xml"
            prefix:   /_internal

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@FrameworkBundle/Resources/config/routing/internal.xml" prefix="/_internal" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection->addCollection($loader->import('@FrameworkBundle/Resources/config/routing/internal.xml', '/_internal'));

        return $collection;

.. tip::

    Since this route allows all actions to be accessed via a URL, you might
    want to protect it by using the Symfony2 firewall feature (by allowing
    access to your reverse proxy's IP range).

One great advantage of this caching strategy is that you can make your
application as dynamic as needed and at the same time, hit the application as
little as possible.

.. note::

    Once you start using ESI, remember to always use the ``s-maxage``
    directive instead of ``max-age``. As the browser only ever receives the
    aggregated resource, it is not aware of the sub-components, and so it will
    obey the ``max-age`` directive and cache the entire page. And you don't
    want that.

The ``render`` helper supports two other useful options:

* ``alt``: used as the ``alt`` attribute on the ESI tag, which allows you
  to specify an alternative URL to be used if the ``src`` cannot be found;

* ``ignore_errors``: if set to true, an ``onerror`` attribute will be added
  to the ESI with a value of ``continue`` indicating that, in the event of
  a failure, the gateway cache will simply remove the ESI tag silently.

.. index::
    single: Cache; Invalidation

.. _http-cache-invalidation:

Cache Invalidation
------------------

    "There are only two hard things in Computer Science: cache invalidation
    and naming things." --Phil Karlton

You should never need to invalidate cached data because invalidation is already
taken into account natively in the HTTP cache models. If you use validation,
you never need to invalidate anything by definition; and if you use expiration
and need to invalidate a resource, it means that you set the expires date
too far away in the future.

.. note::

    It's also because there is no invalidation mechanism that you can use any
    reverse proxy without changing anything in your application code.

Actually, all reverse proxies provide ways to purge cached data, but you
should avoid them as much as possible. The most standard way is to purge the
cache for a given URL by requesting it with the special ``PURGE`` HTTP method.

Here is how you can configure the Symfony2 reverse proxy to support the
``PURGE`` HTTP method::

    // app/AppCache.php
    class AppCache extends Cache
    {
        protected function invalidate(Request $request)
        {
            if ('PURGE' !== $request->getMethod()) {
                return parent::invalidate($request);
            }

            $response = new Response();
            if (!$this->store->purge($request->getUri())) {
                $response->setStatusCode(404, 'Not purged');
            } else {
                $response->setStatusCode(200, 'Purged');
            }

            return $response;
        }
    }

.. caution::

    You must protect the ``PURGE`` HTTP method somehow to avoid random people
    purging your cached data.

Summary
-------

Symfony2 was designed to follow the proven rules of the road: HTTP. Caching
is no exception. Mastering the Symfony2 cache system means becoming familiar
with the HTTP cache models and using them effectively. This means that, instead
of relying only on Symfony2 documentation and code examples, you have access
to a world of knowledge related to HTTP caching and gateway caches such as
Varnish.

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/cache/varnish`

.. _`Things Caches Do`: http://tomayko.com/writings/things-caches-do
.. _`Cache Tutorial`: http://www.mnot.net/cache_docs/
.. _`Varnish`: http://www.varnish-cache.org/
.. _`Squid in reverse proxy mode`: http://wiki.squid-cache.org/SquidFaq/ReverseProxy
.. _`expiration model`: http://tools.ietf.org/html/rfc2616#section-13.2
.. _`validation model`: http://tools.ietf.org/html/rfc2616#section-13.3
.. _`RFC 2616`: http://tools.ietf.org/html/rfc2616
.. _`HTTP Bis`: http://tools.ietf.org/wg/httpbis/
.. _`P4 - Conditional Requests`: http://tools.ietf.org/html/draft-ietf-httpbis-p4-conditional-12
.. _`P6 - Caching: Browser and intermediary caches`: http://tools.ietf.org/html/draft-ietf-httpbis-p6-cache-12
.. _`ESI`: http://www.w3.org/TR/esi-lang
