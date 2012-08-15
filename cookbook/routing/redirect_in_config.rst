.. index::
   single: Routing; Configure redirect to another route without a custom controller

Comment configurer une redirection vers une autre route sans contrôleur personnalisé
====================================================================================

Ce guide explique comment configurer une redirection d'une route vers une autre
sans utiliser de contrôleur spécifique.

Supposons qu'il n'existe pas de contrôleur par défaut pour l'URL ``/`` de votre
application et que vous voulez rediriger ces requêtes vers ``/app``.

Votre configuration ressemblerait à ceci :

.. code-block:: yaml

    AppBundle:
        resource: "@App/Controller/"
        type:     annotation
        prefix:   /app

    root:
        pattern: /
        defaults:
            _controller: FrameworkBundle:Redirect:urlRedirect
            path: /app
            permanent: true

Votre ``AppBundle`` est enregistré pour prendre en charge tout les requêtes qui commencent
par ``/app``.

Nous configurons une route pour le chemin ``/`` et laissons le 
:class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\RedirectController`
le gérer. Ce contrôleur est prêt à fonctionner et propose deux méthodes pour
gérer les requêtes :

* ``redirect`` redirige vers une autre *route*. Vous devez spécifier le paramètre ``route``
  avec le *nom* de la route vers laquelle vous voulez rediriger.

* ``urlRedirect`` redirige vers un autre *chemin*. Vous devez spécifier le paramètre ``path``
  pour qu'il contienne le chemin vers la ressource vers laquelle vous voulez rediriger.

Le paramètre ``permanent`` indique que les deux méthodes utiliseront un code statut HTTP 301.
