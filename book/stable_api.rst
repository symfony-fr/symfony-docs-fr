.. index::
   single: Stable API

L'API stable de Symfony2
========================

L'API stable de Symfony2 est un sous-ensemble de toutes les méthodes publiques
de Symfony2 (composants et bundles "du coeur") qui partagent les propriétés suivantes:

* le namespace et le nom de la classe ne changera pas,
* le nom de la méthode ne changera pas,
* la signatude de la méthode (arguments et valeur de retour) ne changera pas,
* l'objectif de la méthode ne changera pas

L'implémentation elle-même peut changer. La seule raison valable d'un changement 
de l'API stable de Symfony2 serait de corriger un problème de sécurité.

L'API stable est basée sur un principe de liste blanche ("whitelist"), taggée 
par `@api`. En conséquence, tout ce qui n'est pas explicitement taggé ne fait pas
partie de l'API stable.

.. tip::

    Chaque bundle tiers devrait aussi publier sa propre API stable.
    
Tout comme Symfony 2.0, les composants suivants ont leur propre API publique :

* BrowserKit
* ClassLoader
* Console
* CssSelector
* DependencyInjection
* DomCrawler
* EventDispatcher
* Finder
* HttpFoundation
* HttpKernel
* Locale
* Process
* Routing
* Templating
* Translation
* Validator
* Yaml
