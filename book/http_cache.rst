.. index::
   single: Cache

Le Cache HTTP
=============

L'essence d'une application web riche est d'être dynamique. Peu
importe l'efficacité de votre application, le traitement d'une requête
sera toujours plus important que l'envoi d'une page statique.

Pour la plupart des applications web, cela ne pose pas de
problème. Symfony2 est d'une rapidité foudroyante, et à moins que vous
ne fassiez de sérieux remaniements, chaque requête sera traitée
rapidement sans trop « stresser » votre serveur.

Mais si la fréquentation de votre site augmente, ce traitement peut devenir un
problème. Le processus qui s'effectue à chaque requête
peut être exécuté une unique fois. C'est exactement l'objectif de la
mise en cache.

La Mise en cache
----------------

Le moyen le plus efficace d'améliorer les performances d'une
application est de mettre en cache l'intégralité d'une réponse pour ne
plus avoir à rappeler l'application pour les requêtes suivantes. Bien
sûr, ce n'est pas toujours possible pour les sites web fortement
dynamiques, ou peut être que si. A travers ce chapitre, vous verrez comment
fonctionne le système de cache de Symfony2 et en quoi c'est la meilleure
approche possible.

Le système de cache de Symfony2 est différent, car il se base sur la
simplicité et la puissance du cache HTTP tel qu'il est défini dans la
:term:`spécification HTTP`. Au lieu de réinventer un processus de
mise en cache, Symfony2 adopte la norme qui définit la
communication de base sur le Web. Une fois que vous avez compris
les fondamentaux de la validation HTTP et de l'expiration de la mise
en cache, vous serez prêt à maîtriser le système de cache de
Symfony2.

Dans le but de comprendre comment mettre en cache avec Symfony,
nous allons parcourir ce sujet en 4 étapes :

#. Une :ref:`passerelle de cache <gateway-caches>`, ou
   reverse proxy, est une couche indépendante qui se trouve devant
   votre application. La passerelle met en cache les réponses telles
   qu'elles sont retournées par l'application et répond aux requêtes
   avec les réponses qui sont en cache avant qu'elles n'atteignent
   l'application. Symfony2 possède sa propre passerelle par défaut,
   mais n'importe quelle autre peut être également utilisée.

#. Les en-têtes du :ref:`cache HTTP<http-cache-introduction>`
   sont utilisés pour communiquer avec la passerelle de cache et tout
   autre cache entre votre application et le client. Symfony2 en propose
   par défaut et fournit une interface puissante pour interagir avec eux.

#. :ref:`L'expiration et la validation<http-expiration-validation>` HTTP
   sont les deux modèles utilisés pour déterminer si le contenu d'un cache est
   *valide* (peut être réutilisé à partir du cache) ou *périmé* (doit être
   régénéré par l'application).

#. Les :ref:`Edge Side Includes <edge-side-includes>` (ESI)
   autorisent le cache HTTP à mettre en cache des
   fragments de pages (voir des fragments imbriqués) de façon
   indépendante. Avec les ESI, vous pouvez même mettre en cache une
   page entière pendant 60 minutes, mais un bloc imbriqué dans cette
   page uniquement 5 minutes.

Puisque la mise en cache via HTTP n'est pas spécifique à Symfony, de
nombreux articles existent à ce sujet. Si vous n'êtes pas familier avec la
mise cache HTTP, nous vous recommandons *fortement* de lire l'article de
Ryan Tomayko `Things Caches Do`_. Le tutoriel de Mark Nottingham,
`Cache Tutorial`_, est également une ressource très complète sur
ce sujet.

.. index::
   single: Cache; Proxy
   single: Cache; Reverse proxy
   single: Cache; Gateway

.. _gateway-caches:

La mise en cache avec la Passerelle de Cache
--------------------------------------------

Lors d'une mise en cache via HTTP, le *cache* est complètement séparé
de votre application. Il est placé entre votre application et le client
effectuant des requêtes.

Le travail du cache est d'accepter les requêtes du client et de les
transmettre à votre application. Le cache recevra aussi en retour des
réponses de votre application et les enverra au client. Le cache est au milieu
(« middle-man ») dans ce jeu de communication requête-réponse
entre le client et votre application.

Lors d'une communication, le cache stockera toutes les réponses qu'ils
estiment « stockables » (voir :ref:`http-cache-introduction`). Si la même
ressource est demandée, le cache renvoie le contenu mis en cache au
client, en ignorant entièrement l'application.

Ce type de cache est connu sous le nom de passerelle de cache
HTTP. Beaucoup d'autres solutions existent telles que `Varnish`_,
`Squid in reverse proxy mode`_ et le reverse proxy de Symfony2.

.. index::
   single: Cache; Types of

Les types de caches
~~~~~~~~~~~~~~~~~~~

Mais une passerelle de cache ne possède pas qu'un seul type de
cache. Les en-têtes de cache HTTP envoyées par votre application sont
interprétées par trois différents types de cache :

* *Le cache du navigateur* : tous les navigateurs ont leur propre
  cache qui est utile quand un utilisateur demande la page précédente
  ou des images et autres médias. Le cache du navigateur est privé, car
  les ressources stockées ne sont pas partagées avec d'autres
  applications.

* *Le « cache proxy »* : un proxy est un cache *partagé* car plusieurs
  applications peuvent se placer derrière un seul proxy. Il est
  habituellement installé par les entreprises pour diminuer le temps
  de réponse des sites et la consommation des ressources réseau.

* *Passerelle de cache* : comme un proxy, ce système de cache est
  également partagé, mais du côté du serveur. Installé par des
  administrateurs réseau, il permet aux sites d'être plus extensibles,
  sûrs et performants.

.. tip::

    Les passerelles de cache peuvent être désignées comme des « reverse
    proxy », « surrogate proxy » ou même des accélérateurs HTTP.

.. note::

    La notion de cache *privé* par rapport au cache *partagé* sera
    expliquée plus en détail lorsque la mise en cache des contenus liés
    à exactement un utilisateur (les informations sur un compte
    utilisateur par exemple) sera abordée.

Toutes les réponses de l'application iront communément dans un ou deux
des deux premiers types de cache. Ces systèmes ne sont pas sous votre contrôle, 
mais suivent les directives du cache HTTP définies dans les réponses.

.. index::
   single: Cache; Symfony2 reverse proxy

.. _`symfony-gateway-cache`:

Symfony2 Reverse Proxy
~~~~~~~~~~~~~~~~~~~~~~

Symfony2 contient un reverse proxy (aussi appelé passerelle de cache)
écrit en PHP. Son activation entraînera la mise en cache immédiate des
réponses stockables de l'application. L'installer est aussi simple que ça. Chaque
nouvelle application Symfony2 contient un noyau pré-configuré
(AppCache) qui encapsule le noyau par défaut (AppKernel). Le cache kernel (cache
du noyau) *est* le reverse proxy.

Pour activer le mécanisme de cache, il faut modifier le code du
contrôleur principal pour qu'il utilise le cache kernel :

.. code-block:: php

    // web/app.php

    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';
    require_once __DIR__.'/../app/AppCache.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    // wrap the default AppKernel with the AppCache one
    $kernel = new AppCache($kernel);
    $request = Request::createFromGlobals();
    $response = $kernel->handle($request);
    $response->send();
    $kernel->terminate($request, $response);

Le cache kernel se comportera immédiatement comme un « reverse proxy » en
mettant en cache les réponses de l'application et en les renvoyant au
client.

.. tip::

    Le cache kernel a une méthode spéciale ``getLog()`` qui retourne
    une chaîne de caractères décrivant ce qui se passe dans la couche
    du cache. Dans l'environnement de développement, il est possible
    de l'utiliser pour du débogage ou afin de valider votre stratégie
    de mise en cache : ::

        error_log($kernel->getLog());

L'objet ``AppCache`` a une configuration par défaut, mais
peut être reconfiguré finement grâce à une série d'options que vous
pouvez paramétrer en surchargeant la méthode 
:method:`Symfony\\Bundle\\FrameworkBundle\\HttpCache\\HttpCache::getOptions`::

    // app/AppCache.php

    use Symfony\Bundle\FrameworkBundle\HttpCache\HttpCache;

    class AppCache extends HttpCache
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
    ``debug`` est mise automatiquement à la valeur de debug de l'objet
    ``AppKernel`` encapsulé.

Voici une liste des principales options :

* ``default_ttl`` : Le nombre de secondes pendant lesquelles une entrée du
  cache devrait être considérée comme « valide » quand il n'y a pas
  d'information explicite fournie dans une réponse. Une valeur
  explicite pour les en-têtes ``Cache-Control`` ou ``Expires``
  surcharge cette valeur (par défaut : ``0``);


* ``private_headers`` : Type d'en-têtes de requête qui déclenche le
  comportement « privé » du ``Cache-Control`` pour les réponses qui ne
  spécifient pas leur état, c'est-à-dire, si la réponse est ``public``
  ou ``private`` via une directive du ``Cache-Control``. (par défaut : ``Authorization``
  et ``Cookie``);

* ``allow_reload`` : Définit si le client peut forcer ou non un
  rechargement du cache en incluant une directive du ``Cache-Control``
  « no-cache » dans la requête. Définissez-la à ``true`` pour la conformité
  avec la RFC 2616 (par défaut : ``false``);

* ``allow_revalidate`` : Définit si le client peut forcer une
  revalidation du cache en incluant une directive de ``Cache-Control``
  « max-age=0 » dans la requête. Définissez-la à ``true`` pour la conformité
  avec la RFC 2616 (par défaut : ``false``);

* ``stale_while_revalidate`` : Spécifie le nombre de secondes par
  défaut (la granularité est la seconde parce que le TTL de la réponse
  est en seconde) pendant lesquelles le cache peut renvoyer une
  réponse « périmée » alors que la nouvelle réponse est calculée en
  arrière-plan (par défaut : ``2``). Ce paramètre est surchargé par
  l'extension HTTP ``stale-while-revalidate`` du ``Cache-Control``
  (cf. RFC 5861);

* ``stale_if_error`` : Spécifie le nombre de secondes par défaut (la
  granularité est la seconde) pendant lesquelles le cache peut
  renvoyer une réponse « périmée » quand une erreur est rencontrée (par
  défaut : ``60``). Ce paramètre est surchargé par l'extension HTTP
  ``stale-if-error`` du ``Cache-Control`` (cf. RFC 5961).

Si le paramètre ``debug`` est à ``true``, Symfony2 ajoute
automatiquement l'en-tête ``X-Symfony-Cache`` à la réponse contenant
des informations utiles à propos des caches « hits » (utilisation du
cache) et « misses » (page ou réponse non présente en cache).

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
   plus tard vers Varnish quand votre trafic augmentera.

   Pour plus d'informations concernant Varnish avec Symfony2, veuillez-
   vous reportez au chapitre du cookbook :doc:`How to use Varnish
   </cookbook/cache/varnish>`.

.. note::

    Les performances du reverse proxy de Symfony2 ne sont pas liées à
    la complexité de votre application. C'est parce que le noyau de
    l'application n'est démarré que quand la requête lui est
    transmise.

.. index::
   single: Cache; HTTP

.. _http-cache-introduction:

Introduction à la mise en cache avec HTTP
-----------------------------------------

Pour tirer parti des couches de gestion du cache, l'application doit
être capable de communiquer quelles réponses peuvent être mises en
cache et les règles, qui décident quand et comment le cache devient
obsolète. Cela se fait en définissant des en-têtes de gestion de cache
HTTP dans la réponse.

.. tip::

    Il faut garder à l'esprit que « HTTP » n'est rien d'autre que le
    langage (un simple langage texte) que les clients web (les
    navigateurs par exemple) et les serveurs utilisent pour
    communiquer entre eux. Parler de mise en cache HTTP revient à
    parler de la partie du langage qui permet aux clients et aux
    serveurs d'échanger les informations relatives à la gestion du
    cache.

HTTP définit quatre en-têtes spécifiques à la mise en cache des réponses :

* ``Cache-Control``
* ``Expires``
* ``ETag``
* ``Last-Modified``

L'en-tête le plus important et le plus versatile est l'en-tête
``Cache-Control`` qui est en réalité une collection d'informations
diverses sur le cache.

.. note::

    Tous ces en-têtes seront complètement détaillés dans la section
    :ref:`http-expiration-validation`.

.. index::
   single: Cache; Cache-Control header
   single: HTTP headers; Cache-Control

L'en-tête Cache-Control
~~~~~~~~~~~~~~~~~~~~~~~

Cet en-tête est unique du fait qu'il contient non pas une, mais un
ensemble varié d'informations sur la possibilité de mise en cache d'une
réponse. Chaque information est séparée par une virgule :

     Cache-Control: private, max-age=0, must-revalidate

     Cache-Control: max-age=3600, must-revalidate

Symfony fournit une abstraction du ``Cache-Control`` pour faciliter sa
gestion::

    $response = new Response();

    // marquer la réponse comme publique ou privée
    $response->setPublic();
    $response->setPrivate();

    // définir l'âge max des caches privés ou des caches partagés
    $response->setMaxAge(600);
    $response->setSharedMaxAge(600);

    // définir une directive personnalisée du Cache-Control
    $response->headers->addCacheControlDirective('must-revalidate', true);

Réponse publique et réponse privée
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les passerelles de cache et les caches « proxy » sont considérés comme
étant « partagés » car leur contenu est partagé par plusieurs
utilisateurs. Si une réponse spécifique à un utilisateur est par
erreur stockée dans ce type de cache, elle pourrait être renvoyée à un
nombre quelconque d'autres utilisateurs. Imaginez si les informations
concernant votre compte sont mises en cache et ensuite envoyées à tous
les utilisateurs suivants qui souhaitent accéder à leur page de compte !

Pour gérer cette situation, chaque réponse doit être définie comme
étant publique ou privée :

* *public*: Indique que la réponse peut être mise en cache, à la fois,
   par les caches privés et les caches publiques;

* *private*: Indique que toute la réponse concerne un unique
   utilisateur et qu'elle ne doit pas être stockée dans les caches
   publics.

Symfony considère par défaut chaque réponse comme étant privée. Pour
tirer parti des caches partagés (comme le reverse proxy de Symfony2),
la réponse devra explicitement être définie comme publique.

.. index::
   single: Cache; Safe methods

Méthodes sures
~~~~~~~~~~~~~~

La mise en cache HTTP ne fonctionne qu'avec les méthodes « sures »
(telles que GET et HEAD). « Être sûr » signifie que l'état de
l'application n'est jamais modifié par le serveur au moment de servir
la requête (il est bien sûr possible de loguer des informations,
mettre en cache des données, etc.). Cela a deux conséquences :

* L'état de l'application ne devrait *jamais* être modifié en répondant
  à une requête GET ou HEAD. Même s'il n'y a pas de passerelle de
  cache, la présence d'un cache « proxy » signifie qu'aucune requête
  GET ou HEAD ne pourrait pas atteindre le serveur.

* Ne pas mettre en cache les méthodes PUT, POST ou DELETE. Ces
  méthodes sont normalement utilisées pour changer l'état de
  l'application (supprimer un billet de blog par exemple). La mise en
  cache de ces méthodes empêcherait certaines requêtes d'atteindre et de
  modifier l'application.

Règles de mise en cache et configuration par défaut
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

HTTP 1.1 permet de tout mettre en cache par défaut à moins qu'il n'y
ait un en-tête ``Cache-Control``. En pratique, la plupart des
systèmes de cache ne font rien quand les requêtes contiennent un
cookie, ont un en-tête d'autorisation, utilisent une méthode non sure
(i.e. PUT, POST, DELETE), ou quand les réponses ont un code de
redirection.

Symfony2 définit automatiquement une configuration de l'en-tête
Cache-Control quand aucun n'est défini par le développeur en suivant
ces règles :

* Si aucun en-tête de cache n'est défini (``Cache-Control``, ``Expires``, ``ETag``
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
-----------------------------

La spécification HTTP définit deux modèles de mise en cache :

* Avec le `modèle d'expiration`_, on spécifie simplement combien de
  temps une réponse doit être considérée comme « valide » en incluant un
  en-tête ``Cache-Control`` et/ou ``Expires``. Les systèmes de cache qui
  comprennent les directives n'enverront pas la même requête jusqu'à ce
  que la version en cache devienne « invalide ».

* Quand une page est dynamique (c-a-d quand son contenu change
  souvent), le `modèle de validation`_ est souvent nécessaire. Avec ce
  modèle, le système de cache stocke la réponse, mais demande au
  serveur à chaque requête si la réponse est encore
  valide. L'application utilise un identifiant unique (l'en-tête ``Etag``)
  et/ou un timestamp (l'en-tête ``Last-Modified``) pour vérifier si la
  page a changé depuis sa mise en cache.

Le but de ces deux modèles est de ne jamais générer deux fois la même
réponse en s'appuyant sur le système de cache pour stoker et renvoyer
la réponse valide.

.. sidebar:: En lisant la spécification HTTP

    La spécification HTTP définit un langage simple, mais puissant dans
    lequel les clients et les serveurs peuvent communiquer. En tant
    que développeur web, le modèle requête-réponse est le plus
    populaire. Malheureusement, le document de spécification - `RFC 2616`_ - 
    peut être difficile à lire.

    Il existe actuellement une tentative (`HTTP Bis`_) de réécriture
    du RFC 2616.  Elle ne décrit pas une nouvelle version du HTTP
    mais clarifie plutôt la spécification originale du HTTP. Elle est
    découpée en sept parties ; tout ce qui concerne la gestion du
    cache se retrouve dans deux chapitres dédiés (`P4 - Conditional
    Requests`_ et `P6 - Caching: Browser and intermediary caches`_).

    En tant que développeur web, il est fortement recommandé de lire
    la spécification. Sa clarté et sa puissance - même plus dix ans après
    sa création - est inestimable. Ne soyez pas rebuté par
    l'apparence du document - son contenu est beaucoup plus beau que son aspect.

.. index::
   single: Cache; HTTP expiration

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

D'après la spécification HTTP, « les champs de l'en-tête ``Expires``
donnent la date après laquelle la réponse est considérée comme
invalide ». Cet en-tête peut être défini avec la méthode ``setExpires()``
de l'objet ``Response``. Elle prend un objet ``DateTime`` en argument :

.. code-block:: php

    $date = new DateTime();
    $date->modify('+600 seconds');

    $response->setExpires($date);

L'en-tête HTTP résultante sera :

.. code-block:: text

    Expires: Thu, 01 Mar 2011 16:00:00 GMT

.. note::

    La méthode ``setExpires()`` convertit automatiquement la date au
    format GMT comme demandé par la spécification.

Notez que dans toutes les versions HTTP précédant la 1.1, le serveur d'origine
n'était pas obligé d'envoyer l'entête ``Date``. En conséquence, le cache
(par exemple le navigateur) pourrait être obligé de consulter l'horloge
locale afin d'évaluer l'entête ``Expires`` rendant ainsi le calcul de la
durée de vie sensible aux décalages d'horloges.
Une autre limitation de l'entête  ``Expires`` est que la spécification déclare
que « les serveurs HTTP/1.1 ne devraient pas envoyer des dates ``Expires`` de
plus d'un an dans le futur ».

.. index::
   single: Cache; Cache-Control header
   single: HTTP headers; Cache-Control

Expiration avec l'en-tête ``Cache-Control``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

À cause des limitations de l'en-tête ``Expires``, bien souvent, il faut utiliser
l'en-tête ``Cache-Control``. Rappelez-vous que l'en-tête ``Cache-Control`` est
utilisé pour spécifier une grande partie des directives de cache. Pour le modèle
d'expiration, il y a deux directives, ``max-age`` et ``s-maxage``. La première
est utilisée par tous les systèmes de cache alors que la seconde n'est utilisée que
par les systèmes de cache partagés :

.. code-block:: php

    // Définir le nombre de secondes après lesquelles la réponse
    // ne devrait plus être considérée comme valide
    $response->setMaxAge(600);

    // Idem mais uniquement pour les caches partagés
    $response->setSharedMaxAge(600);

L'en-tête ``Cache-Control`` devrait être (il peut y avoir d'autres directives) :

.. code-block:: text

    Cache-Control: max-age=600, s-maxage=600

.. index::
   single: Cache; Validation

Validation
~~~~~~~~~~

S'il faut mettre à jour une ressource dès qu'il y a un changement de
données, le modèle d'expiration ne convient pas. Avec le modèle
d'expiration, l'application ne sera pas appelée jusqu'au moment où le
cache devient invalide.

Le modèle de validation du cache corrige ce problème. Dans ce modèle,
le cache continue de stocker les réponses. La différence est que pour
chaque requête, le cache demande à l'application si la réponse en cache
est encore valide. Si la réponse en cache est encore valide,
l'application renvoie un statut 304 et aucun contenu. Le cache sait
que la réponse en cache est valide.

Ce modèle permet d'économiser beaucoup de bande passante, car la même
réponse n'est pas envoyée deux fois au même client (un code 304 est
envoyé à la place). Si l'application est bien construite, il est
possible de déterminer le minimum de données nécessitant l'envoi de
réponse 304 et aussi d'économiser des ressources CPU (voir ci-dessous
pour un exemple d'implémentation).

.. tip::

    Le code 304 signifie « Non modifié ». C'est important, car la réponse
    associée à ce code ne contient pas le contenu demandé en
    réalité. Au lieu de cela, la réponse est simplement un ensemble
    léger de directives qui informe le cache qu'il devrait utiliser la
    réponse stockée.

Comme avec le modèle d'expiration, il y a deux différents types
d'en-têtes HTTP qui peuvent être utilisés pour implémenter ce modèle :
``ETag`` et ``Last-Modified``.

.. index::
   single: Cache; ETag header
   single: HTTP headers; ETag

Validation avec l'en-tête ``ETag``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'en-tête ``ETag`` est une chaîne de caractères (appelée « entity-tag »)
qui identifie de façon unique une représentation de la ressource
appelée. Il est entièrement généré et défini par votre application tel
que vous pouvez spécifier, par exemple, si la ressource ``/about``,
stockée en cache, sera mise à jour avec ce que votre application
retourne. Un ``ETag`` est similaire à une empreinte et est utilisé
pour comparer rapidement si deux versions différentes d'une ressource
sont équivalentes. Comme une empreinte, chaque ``ETag`` doit être
unique pour toutes les représentations de la même ressource.

Voici une implémentation simple qui génère l'en-tête ETag depuis un
md5 du contenu :

.. code-block:: php

    public function indexAction()
    {
        $response = $this->render('MyBundle:Main:index.html.twig');
        $response->setETag(md5($response->getContent()));
        $response->setPublic(); // permet de s'assurer que la réponse est publique, et qu'elle peut donc être cachée
        $response->isNotModified($this->getRequest());

        return $response;
    }

La méthode :method:`Symfony\\Component\\HttpFoundation\\Response::isNotModified`
method:`Symfony\\Component\\HttpFoundation\\Response::isNotModified`
compare le ``ETag`` envoyé avec la requête avec celui défini dans l'objet ``Response``.
S'ils sont identiques, la méthode renvoie automatiquement le code 304 en
``Response``.

Cet algorithme est assez simple et très générique, mais il est
nécessaire de créer entièrement l'objet ``Response`` avant de pouvoir
calculer l'en-tête ETag, ce qui n'est pas optimal. En d'autres termes,
cette approche économise la bande passante, mais pas l'utilisation du
CPU.

Dans la section :ref:`optimizing-cache-validation`, vous verrez
comment le modèle de validation peut être utilisé plus intelligemment
pour déterminer la validité d'un cache sans faire autant de travail.

.. tip::

    Symfony2 supporte aussi les ETags moins robustes en définissant le
    second argument à ``true`` pour la méthode
    :method:`Symfony\\Component\\HttpFoundation\\Response::setETag`.

.. index::
   single: Cache; Last-Modified header
   single: HTTP headers; Last-Modified

Validation avec l'en-tête ``Last-Modified``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'en-tête ``Last-Modified`` est la seconde forme de la
validation. D'après la spécification HTTP, les champs de l'en-tête
``Last-Modified`` indiquent la date et l'heure à laquelle le serveur
d'origine croit que la représentation a été modifiée pour la dernière
fois. En d'autres termes, l'application décide si oui ou non le
contenu du cache a été mis à jour, en se basant sur le fait que, si oui
ou non le cache a été mis à jour depuis que la réponse a été mise en
cache.

Par exemple, vous pouvez utiliser la date de dernière mise à jour de tous les objets
nécessitant de calculer le rendu de la ressource comme valeur de l'en-tête
``Last-Modified`` :

.. code-block:: php

    public function showAction($articleSlug)
    {
        // ...

        $articleDate = new \DateTime($article->getUpdatedAt());
        $authorDate = new \DateTime($author->getUpdatedAt());

        $date = $authorDate > $articleDate ? $authorDate : $articleDate;

        $response->setLastModified($date);
        // Définit la réponse comme publique. Sinon elle sera privée par défaut.
        $response->setPublic();

        if ($response->isNotModified($this->getRequest())) {
            return $response;
        }

        // ajoutez du code ici pour remplir la réponse avec le contenu complet

        return $response;
    }

La méthode :method:`Symfony\\Component\\HttpFoundation\\Response::isNotModified`
compare l'en-tête ``If-Modified-Since`` envoyé par la requête avec l'en-tête
``Last-Modified`` défini pour la réponse. S'ils sont équivalents, l'objet
``Response`` contiendra le code 304.

.. note::

    L'en-tête de la requête ``If-Modified-Since`` est égal à l'en-tête de
    la dernière réponse ``Last-Modified`` du client pour une ressource
    donnée. C'est grâce à cela que le client et le serveur communiquent
    et constatent ou non si la ressource a été mise à jour depuis
    qu'elle est en cache.

.. index::
   single: Cache; Conditional get
   single: HTTP; 304

.. _optimizing-cache-validation:

Optimiser son code avec le modèle de validation du cache
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le but principal de toutes les stratégies de mise en cache est de
diminuer la charge de l'application. Autrement dit, moins
l'application aura à « travailler » pour renvoyer un status 304, 
mieux ce sera. La méthode ``Response::isNotModified()`` fait
exactement ça en exposant un modèle simple et efficace :

.. code-block:: php

    public function showAction($articleSlug)
    {
        // Obtenir le minimum d'informations pour calculer
        // l'ETag ou la dernière valeur modifiée (Last-Modified value)
        // (basé sur l'objet Request, les données sont recueillies
        // d'une base de données ou d'un couple clé-valeur
        // par exemple)
        $article = // ...

        // Créer un objet Response avec un en-tête ETag
        // et/ou un en-tête Last-Modified
        $response = new Response();
        $response->setETag($article->computeETag());
        $response->setLastModified($article->getPublishedAt());

        // Définit la réponse comme publique. Sinon elle sera privée par défaut.
        $response->setPublic();

        // Vérifier que l'objet Response n'est pas modifié
        // pour un objet Request donné
        if ($response->isNotModified($this->getRequest())) {
            // Retourner immédiatement un objet 304 Response
            return $response;
        } else {
            // faire plus de travail ici - comme récupérer plus de données
            $comments = // ...
            
            // ou formatter un template avec la $response déjà existante
            return $this->render(
                'MyBundle:MyController:article.html.twig',
                array('article' => $article, 'comments' => $comments),
                $response
            );
        }
    }

Quand l'objet ``Response`` n'est pas modifié, la méthode
``isNotModified()`` définit automatiquement le code 304, enlève le
contenu et les en-têtes qui ne doivent pas être présents pour un
status ``304`` (voir la
:method:`Symfony\\Component\\HttpFoundation\\Response::setNotModified`).

.. index::
   single: Cache; Vary
   single: HTTP headers; Vary

Faire varier la Response
~~~~~~~~~~~~~~~~~~~~~~~~

Jusqu'ici, chaque URI est considérée comme une représentation unique
de la ressource cible. Par défaut, la mise en cache HTTP est faite en
donnant l'URI de la ressource comme clé de cache. Si deux personnes
demandent la même URI d'une ressource qui peut être mise en cache, la
deuxième personne recevra la version qui est dans le cache.

Dans certains cas, ce n'est pas suffisant et des versions différentes
de la même URI ont besoin d'être mises en cache en fonction des
valeurs d'un ou plusieurs en-têtes. Par exemple, si les pages sont
compressées parce que le client le supporte, n'importe quelle URI a
deux représentations : une quand le client accepte la compression,
l'autre quand le client ne l'accepte pas. Cette détermination est
faite grâce à la valeur de l'en-tête ``Accept-Encoding``.

Dans ce cas, le cache doit contenir une version compressée et une
version non compressée de la réponse pour une URI particulière et les
envoyer en fonction de la valeur ``Accept-Encoding`` de la requête. Cela
est possible en utilisant l'en-tête ``Vary`` de la réponse, qui est une
liste des différents en-têtes séparés par des virgules dont les
valeurs définissent une représentation différente de la même
ressource.

.. code-block:: text

    Vary: Accept-Encoding, User-Agent

.. tip::

    Cet en-tête ``Vary`` particulier permettra la mise en cache de versions
    différentes de la même ressource en se basant sur l'URI et la
    valeur des en-têtes ``Accept-Encoding`` et ``User-Agent``.

L'objet ``Response`` propose une interface pour gérer l'en-tête ``Vary`` :

.. code-block:: php

    // définir une en-tête "vary"
    $response->setVary('Accept-Encoding');

    // définir plusieurs en-têtes "vary"
    $response->setVary(array('Accept-Encoding', 'User-Agent'));

La méthode ``setVary()`` prend un nom d'en-tête ou un tableau de noms
d'en-tête pour lesquels la réponse varie.

Expiration et Validation
~~~~~~~~~~~~~~~~~~~~~~~~

Il est possible bien entendu d'utiliser à la fois le modèle de
validation et d'expiration pour un même objet ``Response``. Mais comme
le modèle d'expiration l'emporte sur le modèle de validation, il est
facile de bénéficier du meilleur des deux modèles. En d'autres termes
en utilisant à la fois l'expiration et la validation, vous pouvez
programmer le cache pour qu'il fournisse son contenu pendant qu'il
vérifie à intervalle régulier (l'expiration) que ce contenu est
toujours valide.

.. index::
    pair: Cache; Configuration

Les autres méthodes de l'objet Response
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La classe Response fournit beaucoup d'autres méthodes en relation avec
la gestion du cache. Voici les plus utiles :

.. code-block:: php

    // Marquer l'objet Response comme obsolète
    $response->expire();

    // Forcer le retour d'une réponse 304 nettoyé avec aucun contenu
    $response->setNotModified();

La plupart des en-têtes en relation avec la gestion du cache peuvent
être définis avec la seule méthode
:method:`Symfony\\Component\\HttpFoundation\\Response::setCache`::

    // Définir la configuration du cache avec un seul appel
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

Utilisation de la technologie « Edge Side Includes »
----------------------------------------------------

Les passerelles de caches sont une bonne solution pour améliorer les
performances d'un site. Mais elles ont une limitation : elles peuvent
uniquement mettre en cache une page dans son intégralité. Si ce n'est
pas possible de mettre une page entière en cache ou si des parties de
cette page sont plus dynamiques que d'autres, cela pose
problème. Heureusement, Symfony2 fournit une solution pour ces
situations, basée sur la technologie « Edge Side Includes », aussi appelée
`ESI`_. Akamaï a écrit cette spécification il y a 10 ans ; elle permet
de mettre en cache une partie de page avec une stratégie différente de
l'ensemble de la page.

La spécification « ESI » décrit des marqueurs (« tags ») qui peuvent être
embarqués dans la page pour communiquer avec la passerelle de
cache. Un seul marqueur est implémenté dans Symfony2, ``include`` car
c'est le seul qui est utile en dehors du contexte Akamaï : 

.. code-block:: html

    <!DOCTYPE html>
    <html>
        <body>
            <!-- ... some content -->

            <!-- Embed the content of another page here -->
            <esi:include src="http://..." />

            <!-- ... some content -->
        </body>
    </html>

.. note::

    L'exemple montre que chaque marqueur ESI a une URL complète
    (fully-qualified). Un marqueur ESI représente un morceau de page
    qui peut être appelé via une URL donnée.

Quand une requête est envoyée, la passerelle de cache appelle la page
entière depuis son espace de stockage ou depuis le « backend » de
l'application. Si la réponse contient un ou plusieurs marqueurs ESI,
ils sont gérés de la même manière. En d'autres termes, la passerelle de cache récupère
les fragments de page de son cache, ou demande à l'application de les recalculer.
Quand tous les marqueurs ont été calculés, la passerelle les « fusionne » avec la
page principale et envoie le contenu final vers le client.

Le processus est géré de manière transparente au niveau de la
passerelle de cache (c-a-d à l'extérieur de l'application). Comme vous
pouvez le voir, si vous décidez de prendre l'avantage des marqueurs
ESI, Symfony2 réalise le procédé pour les inclure presque sans effort.

Utiliser ESI avec Symfony2
~~~~~~~~~~~~~~~~~~~~~~~~~~

Premièrement, pour utiliser ESI, il faut l'activer dans la
configuration de l'application :

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

Maintenant, prenons l'exemple d'une page statique excepté pour
l'espace « Actualités » qui se trouve en base de page. Avec ESI, il est
possible de mettre en cache la partie qui gère les actualités
indépendamment du reste de la page.

.. code-block:: php

    public function indexAction()
    {
        $response = $this->render('MyBundle:MyController:index.html.twig');
        // définit l'âge maximal partagé - cela marque aussi la réponse comme étant publique
        $response->setSharedMaxAge(600);

        return $response;
    }

Dans cet exemple, la page a une espérance de vie de 10 minutes en
cache. Dans un deuxième temps, incluons l'élément relatif à
l'actualité dans un template via une action embarquée. Ceci sera
réalisé grâce au « helper » ``render`` (voir la documentation sur
:ref:`templating-embedding-controller` pour plus de détails).

Comme le contenu embarqué provient d'une autre page (ou d'un autre
contrôleur), Symfony2 utilise le « helper » standard ``render`` pour
configurer le marqueur ESI :

.. configuration-block::

    .. code-block:: jinja

        {% render '...:news' with {}, {'standalone': true} %}

    .. code-block:: php

        <?php echo $view['actions']->render('...:news', array(), array('standalone' => true)) ?>

Définir ``standalone`` à ``true`` permet à Symfony2 de savoir que
l'action doit être renvoyée en tant que marqueur ESI. Vous devez vous
demander pourquoi vous devriez préférer utiliser un « helper » au lieu
d'écrire simplement le marqueur ESI vous-même. C'est parce que
l'utilisation d'un helper permettra à l'application de fonctionner
même s'il n'y a pas de passerelle de cache installée. Voyons cela plus
en détail.

Quand standalone est défini à ``false`` (la valeur par défaut), Symfony2
fusionne le contenu de la page inclue avec le contenu de la page
principale avant d'envoyer la réponse au client. Mais quand standalone
est défini à ``true``, *et* si Symfony2 détecte qu'il y a un dialogue avec
une passerelle de cache qui supporte ESI, l'application génère le
marqueur. Mais s'il n'y a pas de passerelle ou si elle ne supporte pas le
ESI, Symfony2 fusionnera simplement les contenus comme si standalone
était défini à ``false``.

.. note::

    Symfony2 détecte si la passerelle gère les marqueurs ESI grâce à
    une autre spécification de Akamaï qui est d'ores et déjà supporté
    par le reverse proxy de Symfony2.

L'action embarquée peut maintenant spécifier ces propres règles de
gestion du cache, entièrement indépendamment du reste de la page.

.. code-block:: php

    public function newsAction()
    {
      // ...

      $response->setSharedMaxAge(60);
    }

Avec ESI, la page complète sera valide pendant 600 secondes, mais le
composant de gestion des actualités ne le sera que pendant 60
secondes.

Un prérequis à l'utilisation de ESI est que les actions embarquées
soient accessibles via une URL pour que la passerelle de cache puisse
les recharger indépendamment du reste de la page. Bien sûr, une action
ne peut pas être appelée à moins qu'il y ait une route qui pointe vers
elle. Symfony2 le prend en charge via une route et un contrôleur
génériques. Pour que l'inclusion du marqueur ESI fonctionne
correctement, il faut définir une route ``_internal`` :

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

    Puisque la route permet à toutes les actions d'être appelées
    depuis une URL, il est possible de les protéger avec le pare-feu de
    Symfony2 (en autorisant l'accès uniquement aux adresses IP de vos
    serveurs proxy). Lisez le paragraphe :ref:`Sécuriser par IP<book-security-securing-ip>`
    du :doc:`chapitre Sécurité </book/security>` pour plus d'informations sur
    comment faire cela.

Un des grands avantages de cette stratégie de cache est qu'il est
possible d'avoir une application aussi dynamique que souhaité et
tout en faisant appel à cette application le moins possible.

.. note::

    Une fois que ESI est utilisée, il ne faut pas oublier de toujours
    utiliser la directive ``s-maxage`` à la place de
    ``max-age``. Comme le navigateur ne reçoit que la réponse
    « agrégée » de la ressource, il n'est pas conscient de son
    « sous-contenu », il suit la directive ``max-age`` et met toute la
    page en cache. Ce qui n'est pas souhaitable.

Le helper ``render`` supporte deux autres méthodes utiles :

* ``alt``: utilisée comme l'attribut ``alt`` du marqueur ESI, il
  permet de spécifier une URL alternative si la ressource ``src`` ne
  peut pas être trouvée ;

* ``ignore_errors``: s'il est défini à ``true``, un attribut ``onerror`` sera ajouté à
  l'ESI avec une valeur ``continue`` indiquant que, en cas d'échec, la
  passerelle de cache enlèvera le marqueur ESI sans erreur ou warning.

.. index::
    single: Cache; Invalidation

.. _http-cache-invalidation:

Invalidation du cache
---------------------

    « There are only two hard things in Computer Science: cache invalidation
    and naming things. » --Phil Karlton

    Ceci peut être traduit comme : 
    « Il existe uniquement deux opérations délicates en Informatique :
    l'invalidation de cache et nommer les choses. »

L'invalidation des données du cache ne devrait pas être gérée au
niveau de l'application parce que l'invalidation est déjà prise en
compte nativement par le modèle de gestion du cache HTTP. Si la
validation est utilisée, il ne devrait pas y avoir besoin d'utiliser
l'invalidation par définition ; si l'expiration est utilisée et qu'il y
a besoin d'invalider une ressource, c'est que date d'expiration a été
définie trop loin dans le futur.

.. note::

    Puisque l'invalidation est un sujet spécifique à chaque type de reverse proxy,
    si vous ne vous occupez pas de l'invalidation, vous pouvez passer d'un reverse
    proxy à l'autre sans changer quoi que ce soit au code de votre application.

En fait, tous les « reverse proxies » fournissent un moyen de purger les
données du cache mais il faut l'éviter autant que possible. Le moyen
le plus standard est de purger le cache pour une URL donnée en
l'appelant avec la méthode HTTP spéciale ``PURGE``.

Voici comment configurer le reverse proxy de Symfony2 pour supporter méthode HTTP ``PURGE`` :

.. code-block:: php

    // app/AppCache.php
 
    use Symfony\Bundle\FrameworkBundle\HttpCache\HttpCache;

    class AppCache extends HttpCache
    {
        protected function invalidate(Request $request)
        {
            if ('PURGE' !== $request->getMethod()) {
                return parent::invalidate($request);
            }

            $response = new Response();
            if (!$this->getStore()->purge($request->getUri())) {
                $response->setStatusCode(404, 'Not purged');
            } else {
                $response->setStatusCode(200, 'Purged');
            }

            return $response;
        }
    }

.. caution::

    Il faut protéger cette méthode HTTP ``PURGE`` d'une manière ou d'une
    autre pour éviter que n'importe qui ne puisse purger le cache.

Résumé
------

Symfony2 a été conçu pour suivre les règles éprouvées du protocole
HTTP. La mise en cache n'y fait pas exception. Comprendre le système
de cache de Symfony2 signifie une bonne compréhension des modèles de
gestion du cache HTTP et de les utiliser efficacement. Ceci veut dire
qu'au lieu de vous appuyer uniquement sur la documentation et les
exemples de code de Symfony2, vous pouvez vous ouvrir à un monde plein
de connaissances relatives au cache et passerelles de cache HTTP telles que
Varnish.

En savoir plus grâce au Cookbook
--------------------------------

* :doc:`/cookbook/cache/varnish`

.. _`Things Caches Do`: http://tomayko.com/writings/things-caches-do
.. _`Cache Tutorial`: http://www.mnot.net/cache_docs/
.. _`Varnish`: https://www.varnish-cache.org/
.. _`Squid in reverse proxy mode`: http://wiki.squid-cache.org/SquidFaq/ReverseProxy
.. _`modèle d'expiration`: http://tools.ietf.org/html/rfc2616#section-13.2
.. _`modèle de validation`: http://tools.ietf.org/html/rfc2616#section-13.3
.. _`RFC 2616`: http://tools.ietf.org/html/rfc2616
.. _`HTTP Bis`: http://tools.ietf.org/wg/httpbis/
.. _`P4 - Conditional Requests`: http://tools.ietf.org/html/draft-ietf-httpbis-p4-conditional-12
.. _`P6 - Caching: Browser and intermediary caches`: http://tools.ietf.org/html/draft-ietf-httpbis-p6-cache-12
.. _`ESI`: http://www.w3.org/TR/esi-lang
