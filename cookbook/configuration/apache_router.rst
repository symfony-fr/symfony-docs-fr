.. index::
   single: Apache Router

Comment utiliser le routeur Apache
==================================

Symfony2, bien que déjà très rapide, fournit également plusieurs astuces pour
augmenter encore la vitesse avec quelques modifications.
L'une de ces astuces est de laisser Apache gérer les routes directement plutôt que
d'utiliser Symfony2 pour le faire.

Changer les paramètres de la configuration du routeur
-----------------------------------------------------

Pour dumper les routes Apache, vous devez d'abord modifier les paramètres
de configuration pour dire à Symfony2 d'utiliser le ``ApacheUrlMatcher`` plutôt
que celui par défaut :

.. code-block:: yaml
    
    # app/config/config_prod.yml
    parameters:
        router.options.matcher.cache_class: ~ # Désactive le cache du routeur
        router.options.matcher_class: Symfony\Component\Routing\Matcher\ApacheUrlMatcher

.. tip::

    Notez que cet :class:`Symfony\\Component\\Routing\\Matcher\\ApacheUrlMatcher`
    étend :class:`Symfony\\Component\\Routing\\Matcher\\UrlMatcher` donc même
    si vous ne regénérez pas les règles url_rewrite, tout fonctionnera (parce qu'à
    la fin de ``ApacheUrlMatcher::match()`` un appel à ``parent::match()``
    est réalisé).
    
Générer les règles mod_rewrite
------------------------------

Pour tester que cela fonctionne, créons une route très basique pour le bundle de démo :

.. code-block:: yaml
    
    # app/config/routing.yml
    hello:
        pattern:  /hello/{name}
        defaults: { _controller: AcmeDemoBundle:Demo:hello }
            
    
Maintenant générez les règles **url_rewrite** :
    
.. code-block:: bash

    $ php app/console router:dump-apache -e=prod --no-debug
    
Ce qui devrait afficher quelque chose du genre :

.. code-block:: apache

    # skip "real" requests
    RewriteCond %{REQUEST_FILENAME} -f
    RewriteRule .* - [QSA,L]

    # hello
    RewriteCond %{REQUEST_URI} ^/hello/([^/]+?)$
    RewriteRule .* app.php [QSA,L,E=_ROUTING__route:hello,E=_ROUTING_name:%1,E=_ROUTING__controller:AcmeDemoBundle\:Demo\:hello]

Vous pouvez maintenant réécrire le `web/.htaccess` pour utiliser les nouvelles règles.
Avec cet exemple, cela ressemblerait à ceci :

.. code-block:: apache

    <IfModule mod_rewrite.c>
        RewriteEngine On

        # skip "real" requests
        RewriteCond %{REQUEST_FILENAME} -f
        RewriteRule .* - [QSA,L]

        # hello
        RewriteCond %{REQUEST_URI} ^/hello/([^/]+?)$
        RewriteRule .* app.php [QSA,L,E=_ROUTING__route:hello,E=_ROUTING_name:%1,E=_ROUTING__controller:AcmeDemoBundle\:Demo\:hello]
    </IfModule>

.. note::

   La procédure ci-dessus devrait être effectuée à chaque fois que vous ajoutez/modifiez une route
   si vous voulez en tirer pleinement avantage

C'est tout !
Nous sommes maintenant prêts à utiliser les règles de routage Apache.
    
Modification supplémentaires
----------------------------

Pour gagner un peu de temps d'éxécution, changez les occurences de ``Request``
par ``ApacheRequest`` dans le fichier ``web/app.php``::

    // web/app.php
    
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';
    //require_once __DIR__.'/../app/AppCache.php';

    use Symfony\Component\HttpFoundation\ApacheRequest;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    //$kernel = new AppCache($kernel);
    $kernel->handle(ApacheRequest::createFromGlobals())->send();