.. index::
    single: Cache; Varnish

Comment utiliser Varnish pour accélérer mon site Web
====================================================

Du fait que le cache de Symfony2 utilise les en-têtes HTTP standards, le
:ref:`symfony-gateway-cache` peut facilement être remplacé par n'importe quel
autre « reverse proxy ». Varnish est un accélérateur HTTP puissant et open-source
capable de délivrer du contenu caché rapidement et incluant le support pour les
:ref:`Edge Side Includes<edge-side-includes>`.

.. index::
    single: Varnish; configuration

Configuration
-------------

Comme vu précédemment, Symfony2 est suffisamment intelligent pour détecter si
il parle à un « reverse proxy » qui comprend ESI ou non. Cela fonctionne
automatiquement lorsque vous utilisez le « reverse proxy » de Symfony2, mais vous
avez besoin d'une configuration spéciale pour que cela fonctionne avec Varnish.
Heureusement, Symfony2 repose déjà sur un autre standard écrit par Akamaï
(`Architecture Edge`_). En conséquence, les conseils de configuration décrits dans ce
chapitre peuvent être utiles même si vous n'utilisez pas Symfony2.

.. note::

    Varnish supporte seulement l'attribut ``src`` pour les balises ESI (les
    attributs ``onerror`` et ``alt`` sont ignorés).

Tout d'abord, configurez Varnish afin qu'il annonce son support d'ESI en
ajoutant un en-tête ``Surrogate-Capability`` aux requêtes transférées à
l'application backend :

.. code-block:: text

    sub vcl_recv {
        set req.http.Surrogate-Capability = "abc=ESI/1.0";
    }

Puis, optimisez Varnish afin qu'il analyse uniquement le contenu de la réponse
lorsqu'il y a au moins une balise ESI en vérifiant l'en-tête ``Surrogate-Control``
que Symfony2 ajoute automatiquement :

.. code-block:: text

    sub vcl_fetch {
        if (beresp.http.Surrogate-Control ~ "ESI/1.0") {
            unset beresp.http.Surrogate-Control;

            // for Varnish >= 3.0
            set beresp.do_esi = true;
            // for Varnish < 3.0
            // esi;
        }
    }

.. caution::

    La compression avec ESI n'était pas supportée dans Varnish jusqu'à la
    version 3.0 (lire `GZIP et Varnish`_). Si vous n'utilisez pas Varnish
    3.0, mettez un serveur web devant Varnish afin qu'il effectue la compression.

.. index::
    single: Varnish; Invalidation

Invalidation du Cache
---------------------

Vous ne devriez jamais avoir besoin d'invalider des données cachées parce que
l'invalidation est déjà prise en compte nativement dans les modèles de cache HTTP
(voir :ref:`http-cache-invalidation`).

Néanmoins, Varnish peut être configuré pour accepter une méthode HTTP spécifique
``PURGE`` qui va invalider le cache pour une ressource donnée :

.. code-block:: text

    sub vcl_hit {
        if (req.request == "PURGE") {
            set obj.ttl = 0s;
            error 200 "Purged";
        }
    }

    sub vcl_miss {
        if (req.request == "PURGE") {
            error 404 "Not purged";
        }
    }

.. caution::

    Vous devez protéger la méthode HTTP ``PURGE`` d'une façon ou d'une autre afin
    d'éviter que des personnes purgent vos données cachées.

.. _`Architecture Edge`: http://www.w3.org/TR/edge-arch
.. _`GZIP et Varnish`: https://www.varnish-cache.org/docs/3.0/phk/gzip.html