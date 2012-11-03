Comment personnaliser le processus de bootstrap avant les tests
===============================================================

Parfois, lorsque vous lancez des tests, vous avez besoin d'ajouter
des choses au processus d'initialisation (bootstrap) juste avant
de les lancer. Par exemple, si vous lancez un test fonctionnel et que
vous avez introduit une nouvelle ressource de traduction, alors vous devrez
vider votre cache avant de lancer ces tests. Cet article vous explique comment
faire

D'abord, ajouter le fichier suivant::

    // app/tests.bootstrap.php
    if (isset($_ENV['BOOTSTRAP_CLEAR_CACHE_ENV'])) {
        passthru(sprintf(
            'php "%s/console" cache:clear --env=%s --no-warmup',
            __DIR__,
            $_ENV['BOOTSTRAP_CLEAR_CACHE_ENV']
        ));
    }

    require __DIR__.'/bootstrap.php.cache';

Remplacer le fichier de test bootstrap ``bootstrap.php.cache`` dans
``app/phpunit.xml.dist`` par ``tests.bootstrap.php`` :

.. code-block:: xml

    <!-- app/phpunit.xml.dist -->
    bootstrap = "tests.bootstrap.php"

Maintenant, vous pouvez définir pour quel environnement vous voulez vider
le cache dans votre fichier  ``phpunit.xml.dist`` :

.. code-block:: xml

    <!-- app/phpunit.xml.dist -->
    <php>
        <env name="BOOTSTRAP_CLEAR_CACHE_ENV" value="test"/>
    </php>

Cela devient maintenant une variable d'environnement (c-a-d ``$_ENV``)
qui est disponible dans votre fichier bootstrap personnalisé
(``tests.bootstrap.php``).