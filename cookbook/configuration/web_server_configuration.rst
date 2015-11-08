.. index::
    single: Web Server

Configurer un Server Web
========================

Le moyen conseillé afin de développer votre application Symfony est d'utiliser
le doc:`server web interne à PHP</cookbook/web_server/built_in>`. Cependant,
lorsque vous utilisez une ancienne version de PHP ou lorsque votre application fonctionne
dans un environnement de production, vous aurez besoin d'un serveur web complet.
Cet article décrit différentes manières d'utiliser Symfony avec Apache ou Nginx.

Lorsque vous utilisez Apache, vous pouvez configurer PHP en tant que
ref:`module Apache <web-server-apache-mod-php>` ou avec FastCGI en utilisant
:ref:`PHP FPM <web-server-apache-fpm>`. FastCGI est ici la façon conseillée
d'utiliser PHP :ref:`avec Nginx <web-server-nginx>`.

.. sidebar:: Le Répertoire Web

    Le répertoire web est le dossier d'accueil de tous les fichiers publiques
    et statiques de votre application, incluant les images, les feuilles de styles (stylesheets)
    ainsi que les fichiers JavaScript. C'est aussi ici que sont placés les controllers
    frontaux (``app.php`` et ``app_dev.php``)

    Le répertoire web sert de point d'entrée (communément appelé "document root")
    lors de la configuration de votre serveur web. Dans les exemples ci-dessous, le répertoire ``web/``
    sera ce point d'entrée. Ce répertoire est ``/var/www/project/web/``.

    Si votre hébergeur impose de changer le dossier ``web/`` pour un autre dossier (e.g. ``public_html/``),
    assurez-vous :ref:`d'écraser le dossier <override-web-dir>`.

.. _web-server-apache-mod-php:

Apache avec mod_php/PHP-CGI
---------------------------

La **configuration minimale** pour faire fonctionner votre application sous Apache est la suivante:

.. code-block:: apache

    <VirtualHost *:80>
        ServerName domain.tld
        ServerAlias www.domain.tld

        DocumentRoot /var/www/project/web
        <Directory /var/www/project/web>
            AllowOverride All
            Order Allow,Deny
            Allow from All
        </Directory>

        # décommentez les lignes suivantes si vous installez les "assets" en tant
        # que liens symboliques (symlink) ou si vous rencontrez des problèmes
        # lors de compilations LESS/Sass/CoffeeScript.
        # <Directory /var/www/project>
        #     Options FollowSymlinks
        # </Directory>

        ErrorLog /var/log/apache2/project_error.log
        CustomLog /var/log/apache2/project_access.log combined
    </VirtualHost>

.. tip::

    Si votre système support la variable ``APACHE_LOG_DIR``, vous aurez surement
    l'envie d'utiliser ``${APACHE_LOG_DIR}/`` à ``/var/log/apache2/``.

Utilisez la **configuration optimisée** suivante pour désactiver le support du
``.htaccess`` et améliorer les performances du serveur web:

.. code-block:: apache

    <VirtualHost *:80>
        ServerName domain.tld
        ServerAlias www.domain.tld

        DocumentRoot /var/www/project/web
        <Directory /var/www/project/web>
            AllowOverride None
            Order Allow,Deny
            Allow from All

            <IfModule mod_rewrite.c>
                Options -MultiViews
                RewriteEngine On
                RewriteCond %{REQUEST_FILENAME} !-f
                RewriteRule ^(.*)$ app.php [QSA,L]
            </IfModule>
        </Directory>

        # décommentez les lignes suivantes si vous installez les "assets" en tant
        # que liens symboliques (symlink) ou si vous rencontrez des problèmes
        # lors de compilations LESS/Sass/CoffeeScript.
        # <Directory /var/www/project>
        #     Options FollowSymlinks
        # </Directory>

        ErrorLog /var/log/apache2/project_error.log
        CustomLog /var/log/apache2/project_access.log combined
    </VirtualHost>

.. tip::

    Par défaut et si vous utilisez **php-cgi**, Apache ne passe pas le nom
    d'utilisateur et mot de passe HTTP (basic auth) à PHP. Pour contourner cette
    limitation, vous devez utilisez la ligne de configuration suivante:

    .. code-block:: apache

        RewriteRule .* - [E=HTTP_AUTHORIZATION:%{HTTP:Authorization}]

Utiliser mod_php/PHP-CGI avec Apache 2.4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans Apache 2.4, ``Order Allow,Deny`` a été remplacé par ``Require all granted``.
Ainsi, vous devez modifier les permissions associées à votre configuration
(``Directory``) comme ceci:

.. code-block:: apache

    <Directory /var/www/project/web>
        Require all granted
        # ...
    </Directory>

Pour une configuration avancée d'Apache, lisez la `documentation d'Apache`_ officielle.

.. _web-server-apache-fpm:

Apache avec PHP-FPM
-------------------

Pour utiliser PHP5-FPM avec Apache, vous devez tout d'abord vous assurer d'avoir
le manageur de processus binaire FastCGI ``php-fpm`` et le module FastCGI d'Apache
installé (par exemple, sur un système Debian, vous devez installer les modules
``libapache2-mod-fastcgi`` et ``php5-fpm``).


PHP-FPM utilise ce qu'on appelle communément des *pools* pour gérer les requêtes
FastCGI entrantes. Vous pouvez configurer arbitrairement le nombre de pools
dans votre configuration FPM. Une pool peut-être configurée pour écouter soit
sur un socket TCP (IP et port) ou sur un socket Unix. Chaque pool peut
fonctionner avec différents UID et GID:

.. code-block:: ini

    ; une pool appelée www
    [www]
    user = www-data
    group = www-data

    ; utilise un socket unix
    listen = /var/run/php5-fpm.sock

    ; ou un socket TCP
    listen = 127.0.0.1:9000

Utiliser mod_proxy_fcgi avec Apache 2.4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si vous utilisez Apache 2.4, vous pouvez facilement utiliser ``mod_proxy_fcgi``
pour déléguer les requêtes entrantes à PHP-FPM. Dans ce cas, configurez PHP-FPM
pour pour écouter sur un socket TCP (pour l'instant ``mod_proxy``,
`ne supporte pas les sockets Unix`_), activez ``mod_proxy`` et ``mod_proxy_fcgi``
dans votre configuration Apache et utilisez la directive appelée ``SetHandler``
pour déléguer les requêtes sur des fichiers PHP à PHP FPM:

.. code-block:: apache

    <VirtualHost *:80>
        ServerName domain.tld
        ServerAlias www.domain.tld

        # Décommentez les lignes suivantes pour forcer Apache à passer le
        # header "Authorization" à PHP: nécesasire pour le "basic_auth"
        # avec PHP-FPM et FastCGI.
        #
        # SetEnvIfNoCase ^Authorization$ "(.+)" HTTP_AUTHORIZATION=$1

        # Pour Apache 2.4.9 ou supérieur
        # L'utilisation de "SetHandler" permet d'éviter les problèmes lors de
        # l'utilisation de ProxyPassMatch avec mod_rewrite et mod_autoindex.
        <FilesMatch \.php$>
            SetHandler proxy:fcgi://127.0.0.1:9000
        </FilesMatch>

        # Si vous utilisez une version d'Apache inférieure à 2.4.9, vous devriez
        # penser à une mise à jour ou penser à utiliser ceci:
        # ProxyPassMatch ^/(.*\.php(/.*)?)$ fcgi://127.0.0.1:9000/var/www/project/web/$1

        # Si vous utilisez votre application Symfony via un sous-dossier du dossier
        # d'accueil par défaut ("DocumentRoot"), l'expression régulière doit être
        # changée comme ceci:
        # ProxyPassMatch ^/path-to-app/(.*\.php(/.*)?)$ fcgi://127.0.0.1:9000/var/www/project/web/$1

        DocumentRoot /var/www/project/web
        <Directory /var/www/project/web>
            # active la réecriture via .htaccess
            AllowOverride All
            Require all granted
        </Directory>

        # décommentez les lignes suivantes si vous installez les "assets" en tant
        # que liens symboliques (symlink) ou si vous rencontrez des problèmes
        # lors de compilations LESS/Sass/CoffeeScript.
        # <Directory /var/www/project>
        #     Options FollowSymlinks
        # </Directory>

        ErrorLog /var/log/apache2/project_error.log
        CustomLog /var/log/apache2/project_access.log combined
    </VirtualHost>

PHP-FPM avec Apache 2.2
~~~~~~~~~~~~~~~~~~~~~~~

Avec Apache 2.2 ou inférieur, vous ne pouvez utiliser ``mod_proxy_fcgi``. A la place,
vous devez utiliser la directive ``FastCgiExternalServer``. Ainsi, votre configuration
Apache devrait ressembler à quelque chose comme ça:

.. code-block:: apache

    <VirtualHost *:80>
        ServerName domain.tld
        ServerAlias www.domain.tld

        AddHandler php5-fcgi .php
        Action php5-fcgi /php5-fcgi
        Alias /php5-fcgi /usr/lib/cgi-bin/php5-fcgi
        FastCgiExternalServer /usr/lib/cgi-bin/php5-fcgi -host 127.0.0.1:9000 -pass-header Authorization

        DocumentRoot /var/www/project/web
        <Directory /var/www/project/web>
            # active la réecriture via .htaccess
            AllowOverride All
            Order Allow,Deny
            Allow from all
        </Directory>

        # décommentez les lignes suivantes si vous installez les "assets" en tant
        # que liens symboliques (symlink) ou si vous rencontrez des problèmes
        # lors de compilations LESS/Sass/CoffeeScript.
        # <Directory /var/www/project>
        #     Options FollowSymlinks
        # </Directory>

        ErrorLog /var/log/apache2/project_error.log
        CustomLog /var/log/apache2/project_access.log combined
    </VirtualHost>

Si vous préférez utiliser un socket Unix, vous devrez plutôt utiliser
l'argument ``-socket``:

.. code-block:: apache

    FastCgiExternalServer /usr/lib/cgi-bin/php5-fcgi -socket /var/run/php5-fpm.sock -pass-header Authorization

.. _web-server-nginx:

Nginx
-----

La **configuration minimale** pour faire fonctionner votre application via Nginx est:

.. code-block:: nginx

    server {
        server_name domain.tld www.domain.tld;
        root /var/www/project/web;

        location / {
            # essai de retourner le fichier demandé si disponible, sinon charge app.php
            try_files $uri /app.php$is_args$args;
        }
        # DEV
        # Cette règle devrait être insérée seulement dans votre environnement de développement.
        # En production, ne l'incluez pas et ne déployez pas les fichiers app_php ou config.php.
        location ~ ^/(app_dev|config)\.php(/|$) {
            fastcgi_pass unix:/var/run/php5-fpm.sock;
            fastcgi_split_path_info ^(.+\.php)(/.*)$;
            include fastcgi_params;
            fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;
        }
        # PROD
        location ~ ^/app\.php(/|$) {
            fastcgi_pass unix:/var/run/php5-fpm.sock;
            fastcgi_split_path_info ^(.+\.php)(/.*)$;
            include fastcgi_params;
            fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;
            # Empeêche les URIs incluant le controller par défault.
            # Ceci retournera une erreur 404:
            # http://domain.tld/app.php/some-path
            # Supprime les directives itnernes autorisant les URI comme ceci.
            internal;
        }

        error_log /var/log/nginx/project_error.log;
        access_log /var/log/nginx/project_access.log;
    }

.. note::

    Suivant votre configuration PHP-FPM, la valeur de ``fastcgi_pass`` peut
    aussi être: ``fastcgi_pass 127.0.0.1:9000``.

.. tip::

    Ceci exécute **seulement** ``app.php``, ``app_dev.php`` et ``config.php``
    disponible dans le dossier web. Tous les autres fichiers seront retournés
    tel quel (ils ne seront pas exécutés et leur contenu sera pleinement affiché).
    Si vous déployez les fichiers ``app_dev.php`` et ``config.php``, vous **devez**
    vous assurer que ces fichiers sont sécurisés et non disponibles à des
    utilisateurs externes (le code vérifiant l'adresse IP en haut de chaque
    fichier le vérifie par défaut).

    Si vous avez d'autres fichiers PHP dans votre dossier web devant êtres exécutés,
    assurez vous de les inclure dans le block ``location`` vu au dessus.

Pour une configuration avancée de Nginx, lisez la `documentation Nginx`_ officielle.

.. _`documentation d'Apache`: http://httpd.apache.org/docs/
.. _`ne supporte pas les sockets Unix`: https://bz.apache.org/bugzilla/show_bug.cgi?id=54101
.. _`FastCgiExternalServer`: http://www.fastcgi.com/mod_fastcgi/docs/mod_fastcgi.html#FastCgiExternalServer
.. _`documentation Nginx`: http://wiki.nginx.org/Symfony