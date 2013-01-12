.. index::
   single: Routing; Configure redirect to another route without a custom controller

Comment configurer une redirection vers une autre route sans contrôleur personnalisé
====================================================================================

Ce guide explique comment configurer une redirection d'une route vers une autre
sans utiliser de contrôleur spécifique.

Supposez qu'il n'existe pas de contrôleur par défaut pour l'URL ``/`` de votre
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


Dans cet exemple, vous configurez une route pour le chemin ``/`` et laissez
la classe :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\RedirectController`
la gérer. Ce contrôleur est fourni avec Symfony et propose deux actions pour
rediriger les requêtes :

* ``urlRedirect`` redirige vers un autre *chemin*. Vous devez spécifier le paramètre ``path``
  pour qu'il contienne le chemin vers la ressource vers laquelle vous voulez rediriger.

* ``redirect`` (pas montré ici) redirige vers une autre *route*. Vous devez définir le
  paramètre ``route`` avec le *nom* de la route vers laquelle vous voulez rediriger.

Le paramètre ``permanent`` indique aux deux méthodes de retourner un code de statut
HTTP 301 au lieu du code ``302`` par défaut.
