Le Contrôleur
=============

Toujours avec nous après ces deux premières parties ? Vous êtes déjà un
inconditionnel de Symfony2 ! Sans plus tarder, nous allons voir ce que les
contrôleurs peuvent faire pour vous.

Utiliser les formats
--------------------

De nos jours, une application Web doit être en mesure de livrer plus que de
simples fichiers HTML. Du XML pour les flux RSS ou des Web Services, du JSON
pour les requêtes Ajax,... Bref, il y a beaucoup de formats différents à choisir.
Manipuler ces formats dans Symfony2 est simple. Modifiez ``routing.yml`` et
ajoutez un ``_format`` avec une valeur d'attribut ``xml``::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    /**
     * @Route("/hello/{name}", defaults={"_format"="xml"}, name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

En utilisant le format (défini par la valeur ``_format``) dans la requête, Symfony2
choisira automatiquement le bon template, ici ``hello.xml.twig``:

.. code-block:: xml+php

    <!-- src/Acme/DemoBundle/Resources/views/Demo/hello.xml.twig -->
    <hello>
        <name>{{ name }}</name>
    </hello>

C'est tout ce qu'il y a à faire. Pour les formats standards, Symfony2 choisira
automatiquement le meilleur en-tête ``Content-Type`` pour la réponse. Si vous
voulez prendre en charge des formats différents pour une seule action, utilisez
l'emplacement ``{_format}`` dans le pattern à la place::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    /**
     * @Route("/hello/{name}.{_format}", defaults={"_format"="html"}, requirements={"_format"="html|xml|json"}, name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

Le contrôleur sera désormais appelé par des URLs comme ``/demo/hello/Fabien.xml``
ou ``/demo/hello/Fabien.json``.

L'entrée ``requirements`` définit les expressions régulières qui doivent
correspondre à des emplacements réservés. Dans cet exemple, si vous essayez de
demander la ressource ``/hello/Fabien.js``, vous obtiendrez une erreur HTTP 404,
car elle ne correspond pas à l'exigence ``_format``.

Redirections et renvois
-----------------------

Si vous voulez rediriger un utilisateur vers une autre page, utilisez la méthode
``redirect()``::

    return $this->redirect($this->generateUrl('_demo_hello', array('name' => 'Lucas')));

La méthode ``generateUrl()`` est la même que la méthode ``path()`` que nous
avions utilisée dans les templates. Elle prend le nom de la route et un tableau
de paramètres comme arguments et retourne la jolie adresse qui
convient.

Vous pouvez facilement renvoyer une action vers une autre avec la méthode
``forward()``. En interne, Symfony crée une «sous-requête», et retourne l'objet
``Response`` de cette sous-requête::

    $response = $this->forward('AcmeDemoBundle:Hello:fancy', array('name' => $name, 'color' => 'green'));

    // do something with the response or return it directly

Obtenir des informations de la requête
--------------------------------------

En plus des paramètres venant des routes, le contrôleur peut également accéder
à l'objet ``Request``::

    $request = $this->$this->getRequest();

    $request->isXmlHttpRequest(); // is it an Ajax request?

    $request->getPreferredLanguage(array('en', 'fr'));

    $request->query->get('page'); // get a $_GET parameter

    $request->request->get('page'); // get a $_POST parameter

Dans un template, vous pouvez accéder à l'objet ``Request`` via la variable ``app.request``:

.. code-block:: html+jinja

    {{ app.request.query.get('page') }}

    {{ app.request.parameter('page') }}

Persister les données en session
--------------------------------

Même si le protocole HTTP est «stateless», Symfony2 fournit un objet session
très pratique qui représente le client (une personne physique qui utilise un
navigateur, un robot ou un web service). Entre deux requêtes, Symfony2 stocke les
attributs dans un cookie en utilisant les sessions PHP natives.

Stocker et retrouver les informations en session peut être fait très facilement
dans un contrôleur::

    $session = $this->$this->getRequest()->getSession();

    // stocke un attribut pour une future requête
    $session->set('foo', 'bar');

    // dans un autre contrôleur et une autre requête
    $foo = $session->get('foo');

    // définit la locale de l'utilisateur
    $session->setLocale('fr');

Vous pouvez aussi stocker de courts messages qui ne seront disponibles que pour
la prochaine requête::

    // stocke un message pour la prochaine requête (dans un contrôleur)
    $session->setFlash('notice', 'Congratulations, your action succeeded!');

    // affiche le message lors de la requêtes suivante (dans un template)
    {{ app.session.flash('notice') }}

C'est utile quand vous avez besoin d'afficher un message de succès avant de
rediriger l'utilisateur vers une autre page (qui affichera alors le message).

Sécuriser les ressources
------------------------

La Symfony Standard Edition est fournie avec une configuration de sécurité simple
qui suffit à la plupart des besoins:

.. code-block:: yaml

    # app/config/security.yml
    security:
        encoders:
            Symfony\Component\Security\Core\User\User: plaintext

        role_hierarchy:
            ROLE_ADMIN:       ROLE_USER
            ROLE_SUPER_ADMIN: [ROLE_USER, ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH]

        providers:
            in_memory:
                users:
                    user:  { password: userpass, roles: [ 'ROLE_USER' ] }
                    admin: { password: adminpass, roles: [ 'ROLE_ADMIN' ] }

        firewalls:
            dev:	
                pattern:  ^/(_(profiler|wdt)|css|images|js)/
                security: false

            login:
                pattern:  ^/demo/secured/login$
                security: false

            secured_area:
                pattern:    ^/demo/secured/
                form_login:
                    check_path: /demo/secured/login_check
                    login_path: /demo/secured/login
                logout:
                    path:   /demo/secured/logout
                    target: /demo/

Cette configuration requiert que les utilisateurs soient connectés pour toute URL
commençant par ``/demo/secured/`` et définit deux utilisateurs valides : ``user``
et ``admin``.
De plus, l'utilisateur ``admin`` a le rôle ``ROLE_ADMIN``, qui inclut aussi le rôle
``ROLE_USER`` (regardez le paramètre ``role_hierarchy``).

.. tip::
    
    Pour des raisons de lisibilité, les mots de passe sont stockés en clair dans
    cette configuration, mais vous pouvez utiliser un algorithme en modifiant la
    section ``encoders``.

Aller à l'URL ``http://localhost/Symfony/web/app_dev.php/demo/secured/hello``
vous redirigera automatiquement au formulaire d'authentification car la ressource
est protégée par un ``firewall``.

Vous pouvez aussi forcer l'action à exiger un rôle donné en utilisant l'annotation
``@Secure`` du contrôleur::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    use JMS\SecurityExtraBundle\Annotation\Secure;

    /**
     * @Route("/hello/admin/{name}", name="_demo_secured_hello_admin")
     * @Secure(roles="ROLE_ADMIN")
     * @Template()
     */
    public function helloAdminAction($name)
    {
        return array('name' => $name);
    }

Maintenant, connecté en tant que ``user`` (qui n'a *pas* le rôle ``ROLE_ADMIN``)
et depuis la page  sécurisée «hello» cliquez sur le lien «ressource sécurisée Hello».
Symfony2 devrait retourner un code HTTP 403, indiquant que la ressource est «interdite»
à cet utilisateur.

.. note::

    La couche de sécurité de Symfony2 est très flexible et est livrée avec différents
    fournisseurs (par exemple un pour l'ORM Doctrine) et des fournisseurs 
    d'authentification (comme HTTP basic, HTTP digest, ou le certificat X509).
    Lisez le chapitre «:doc:`/book/security`» pour avoir plus d'information sur
    leur configuration ou leur utilisation.

Cacher les ressources
---------------------

Dès que votre site commencera à générer du trafic, vous voudrez éviter de générer
les ressources encore et encore. Symfony2 utilise le cache HTTP pour gérer la mise
en cache des ressources. Pour une stratégie de mise en cache basique, utilisez
l'annotation ``@Cache()``::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Cache;

    /**
     * @Route("/hello/{name}", name="_demo_hello")
     * @Template()
     * @Cache(maxage="86400")
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

Dans cet exemple, la ressource peut être mise en cache pour une journée. Mais vous
pouvez également utiliser la validation plutôt que l'expiration ou une combinaison
des deux si cela correspond mieux à vos besoins.

La mise en cache des ressources est gérée par le reverse proxy inclu dans Symfony2.
Mais, puisque la mise en cache est gérée par des entêtes HTTP classiques, vous
pouvez remplacer le reverse proxy Symfony par Varnish ou Squid et l'adapter
facilement à votre application.

.. note::

    Mais si jamais vous ne pouvez pas cacher vos pages en entier ? Symfony2 a
    la solution via les Edge Side Includes (ESI), qui sont supportés nativement.
    Lisez le chapitre «:doc:`/book/http_cache`» pour en savoir plus.

Le mot de la fin
----------------

C'est tout ce qu'il y a à faire et je ne suis même pas sûr que nous avons passé
les 10 minutes que l'on s'était allouées. Nous avons brièvement présenté les 
Bundles dans la première partie et toutes les caractéristiques que nous avons 
apprises jusqu'à maintenant font partie du «core framework Bundle».
Mais grâce aux Bundles, tout peut être étendu ou remplacé dans Symfony2.
C'est le thème de la :doc:`prochaine partie de ce tutoriel<the_architecture>`.

