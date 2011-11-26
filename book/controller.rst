.. index::
   single: Le Contrôleur

Le Contrôleur
=============

Un contrôleur est une fonction PHP créée par vos soins qui prend l'information
provenant de la requête HTTP et construit puis retourne une réponse HTTP
(sous forme d'un objet Symfony2 ``Response``). La réponse peut être
une page HTML, un document XML, un tableau JSON sérialisé, une image, une
redirection, une erreur 404 ou tout ce que vous pouvez imaginer. Le contrôleur
contient n'importe quelle logique arbitraire dont
*votre application* a besoin pour retourner le contenu d'une page.

Pour en illustrer la simplicité, jetons un oeil à un contrôleur Symfony2
en action. Le contrôleur suivant rend une page qui écrit simplement
``Hello world!``::

    use Symfony\Component\HttpFoundation\Response;

    public function helloAction()
    {
        return new Response('Hello world!');
    }

Le but d'un contrôleur est toujours le même : créer et retourner un objet
``Response``. Durant son cheminement, il peut lire de l'information
depuis la requête, charger une ressource depuis la base de données, envoyer un 
email, ou définir une variable dans la session de l'utilisateur.
Mais dans tous les cas, le contrôleur va finalement retourner l'objet ``Response``
qui sera délivré au client.

Il n'y a pas de magie et aucune autre exigence à prendre en compte ! Voici
quelques exemples communs :

* *Le contrôleur A* prépare un objet ``Response`` représentant le contenu de
  la page d'accueil.

* *Le contrôleur B* lit le paramètre ``slug`` contenu dans la requête pour
  charger une entrée du blog depuis la base de données et crée un objet
  ``Response`` affichant ce blog. Si le ``slug`` ne peut pas être trouvé
  dans la base de données, il crée et retourne un objet ``Response`` avec
  un code de statut 404.

* *Le contrôleur C* gère la soumission d'un formulaire de contact. Il lit
  l'information de ce dernier depuis la requête, enregistre les informations
  du contact dans la base de données et envoie ces dernières par email au webmaster.
  Enfin, il crée un objet ``Response`` qui redirige le navigateur du client vers
  la page « merci » du formulaire de contact.

.. index::
   single: Le Contrôleur; Cycle de vie Requête-contrôleur-réponse

Cycle de vie Requête, Contrôleur, Réponse
-----------------------------------------

Chaque requête gérée par un projet Symfony2 suit le même cycle de vie. Le
framework prend soin des tâches répétitives et exécute finalement un contrôleur
qui contient votre code applicatif personnalisé :

#. Chaque requête est gérée par un unique fichier contrôleur frontal (par exemple:
``app.php`` ou ``app_dev.php``) qui démarre l'application;

#. Le ``Router`` lit l'information depuis la requête (par exemple: l'URI), trouve
   une route qui correspond à cette information, et lit le paramètre ``_controller``
   depuis la route;

#. Le contrôleur correspondant à la route est exécuté et le code interne au
   contrôleur crée et retourne un objet ``Response``;

#. Les en-têtes HTTP et le contenu de l'objet ``Response`` sont envoyés au client.

Créer une page est aussi facile que de créer un contrôleur (#3) et d'implémenter une
route qui fasse correspondre une URL à ce dernier (#2).

.. note::

    Bien que nommé de la même manière, un « contrôleur frontal » est différent
    des « contrôleurs » dont nous allons parler dans ce chapitre. Un contrôleur
    frontal est un petit fichier PHP qui se situe dans votre répertoire web et
    à travers lequel toutes les requêtes sont dirigées. Une application typique
    va avoir un contrôleur frontal de production (par exemple: ``app.php``) et
    un contrôleur frontal de développement (par exemple: ``app_dev.php``). Vous
    n'aurez vraisemblablement jamais besoin d'éditer, de regarder ou de vous
    occuper des contrôleurs frontaux dans votre application.

.. index::
   single: Le Contrôleur; Un exemple simple

Un contrôleur simple
--------------------

Bien qu'un contrôleur puisse être n'importe quelle « chose PHP » appelable (une
fonction, une méthode d'un objet, ou une ``Closure``), dans Symfony2, un
contrôleur est généralement une unique méthode à l'intérieur d'un objet contrôleur.
Les contrôleurs sont aussi appelés *actions*.

.. code-block:: php
    :linenos:

    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
        public function indexAction($name)
        {
          return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

.. tip::

    Notez que le *contrôleur* est la méthode ``indexAction``, qui réside
    dans une *classe contrôleur* (``HelloController``). Ne soyez pas gêné
    par ce nom : une *classe contrôleur* est simplement une manière
    pratique de grouper plusieurs contrôleurs/actions ensemble. Typiquement,
    la classe contrôleur va héberger plusieurs contrôleurs/actions (par exemple :
    ``updateAction``, ``deleteAction``, etc).

Ce contrôleur est relativement simple, mais parcourons-le tout de même :

* *ligne 3*: Symfony2 tire avantage de la fonctionnalité des espaces de noms
  (« namespaces ») de PHP 5.3 afin de donner un espace de noms à la classe entière
  du contrôleur. Le mot-clé ``use`` importe la classe ``Response``, que notre
  contrôleur doit retourner.

* *ligne 6*: Le nom de la classe est la concaténation d'un nom pour la classe
  du contrôleur (par exemple: ``Hello``) et du mot ``Controller``. Ceci est une
  convention qui fournit une uniformité aux contrôleurs et qui leurs permet
  d'être référencés seulement par la première partie du nom (par exemple: ``Hello``)
  dans la configuration de routage (« routing »).

* *ligne 8*: Chaque action dans une classe contrôleur se termine par ``Action``
  et est référencée dans la configuration de routage par le nom de l'action
  (``index``). Dans la prochaine section, vous allez créer une route qui fait
  correspondre une URI à son action. Vous allez apprendre comment les paramètres
  substituables de la route (par exemple ``{name}``) deviennent les arguments de la méthode
  action (``$name``).

* *ligne 10*: Le contrôleur crée et retourne un objet ``Response``.

.. index::
   single: Le Contrôleur; Routes et contrôleurs

Faire correspondre une URL à un Contrôleur
------------------------------------------

Le nouveau contrôleur retourne une simple page HTML. Pour voir cette page dans
votre navigateur, vous avez besoin de créer une route qui va faire correspondre
un pattern d'URL spécifique à ce contrôleur :

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            pattern:      /hello/{name}
            defaults:     { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hello" pattern="/hello/{name}">
            <default key="_controller">AcmeHelloBundle:Hello:index</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

Naviguer à l'URL ``/hello/ryan`` va maintenant exécuter le contrôleur
``HelloController::indexAction()`` et passer en tant que variable ``$name`` la
valeur ``ryan``. Créer une « page » signifie simplement créer une méthode contrôleur
et une route associée.

Notez la syntaxe utilisée pour faire référence au contrôleur : ``AcmeHelloBundle:Hello:index``.
Symfony2 utilise une notation de chaîne de caractères flexible pour faire référence aux
différents contrôleurs. Ceci est la syntaxe la plus commune qui spécifie à Symfony2 de
chercher une classe contrôleur appelée ``HelloController`` dans un bundle appelé
``AcmeHelloBundle``. La méthode ``indexAction()`` est alors exécutée.

Pour plus de détails sur le format de chaîne de caractères utilisé pour référencer
les différents contrôleurs, regardez du côté de :ref:`controller-string-syntax`.

.. note::

    Cet exemple place la configuration de routage directement dans le répertoire
    ``app/config/``. Une meilleure façon d'organiser vos routes est de placer
    chacune d'entre elles dans le bundle auquel elle appartient. Pour plus
    d'informations sur ceci, voyez :ref:`routing-include-external-resources`.

.. tip::

    Vous pouvez en apprendre beaucoup plus sur le système de routage en lisant le
    chapitre :doc:`Routage</book/routing>`.

.. index::
   single: Le Contrôleur; Les arguments du contrôleur

.. _route-parameters-controller-arguments:

Les paramètres de la route en tant qu'arguments du contrôleur
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous savez déjà que le paramètre ``_controller`` ``AcmeHelloBundle:Hello:index``
réfère à une méthode ``HelloController::indexAction()`` qui réside dans le bundle
``AcmeHelloBundle``. Mais ce qui est plus intéressant sont les arguments qui sont passés
à cette méthode :

.. code-block:: php

    <?php
    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
          // ...
        }
    }

Le contrôleur possède un argument unique, ``$name``, qui correspond au
paramètre ``{name}`` de la route associée (``ryan`` dans notre exemple).
En fait, lorsque vous exécutez votre contrôleur, Symfony2 fait correspondre
chaque argument du contrôleur avec un paramètre de la route correspondante.
Prenez l'exemple suivant :

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            pattern:      /hello/{first_name}/{last_name}
            defaults:     { _controller: AcmeHelloBundle:Hello:index, color: green }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hello" pattern="/hello/{first_name}/{last_name}">
            <default key="_controller">AcmeHelloBundle:Hello:index</default>
            <default key="color">green</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{first_name}/{last_name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
            'color'       => 'green',
        )));

Le contrôleur dans cet exemple peut prendre plusieurs arguments::

    public function indexAction($first_name, $last_name, $color)
    {
        // ...
    }

Notez que les deux variables de substitution (``{first_name}``, ``{last_name}``)
ainsi que la variable par défaut ``color`` sont disponibles en tant qu'arguments
dans le contrôleur. Quand une route correspond, les variables de substitution
sont fusionnées avec celles ``par défaut`` afin de construire un tableau
qui est à la disposition de votre contrôleur.

Faire correspondre les paramètres de la route aux arguments du contrôleur est
facile et flexible. Gardez les directives suivantes en tête quand vous développez.

* **L'ordre des arguments du contrôleur n'a pas d'importance**

    Symfony est capable de faire correspondre les noms des paramètres de la route
    aux noms des variables de la signature de la méthode du contrôleur. En d'autres
    termes, il réalise que le paramètre ``{last_name}`` correspond à l'argument
    ``$last_name``. Les arguments du contrôleur pourraient être totalement
    réorganisés que cela fonctionnerait toujours parfaitement::

        public function indexAction($last_name, $color, $first_name)
        {
            // ..
        }

* **Chaque argument requis du contrôleur doit correspondre à un paramètre de la route**

    Le code suivant lancerait une ``RuntimeException`` parce qu'il n'y a pas
    de paramètre ``foo`` défini dans la route::

        public function indexAction($first_name, $last_name, $color, $foo)
        {
            // ..
        }

    Cependant, définir l'argument en tant qu'optionnel est parfaitement valide.
    L'exemple suivant ne lancerait pas d'exception::

        public function indexAction($first_name, $last_name, $color, $foo = 'bar')
        {
            // ..
        }

* **Tous les paramètres de la route n'ont pas besoin d'être des arguments de votre contrôleur**

    Si, par exemple, le paramètre ``last_name`` n'était pas important pour votre
    contrôleur, vous pourriez complètement l'omettre::

        public function indexAction($first_name, $color)
        {
            // ..
        }

.. tip::

    Chaque route possède aussi un paramètre spécial ``_route`` qui est égal
    au nom de la route qui a été reconnue (par exemple: ``hello``). Bien que
    pas très utile généralement, il est néanmoins disponible en tant qu'argument
    du contrôleur au même titre que les autres.

.. _book-controller-request-argument:

La ``Requête`` en tant qu'argument du Contrôleur
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour plus de facilités, Symfony peut aussi vous passer l'objet ``Request``
en tant qu'argument de votre contrôleur. Ceci est spécialement pratique
lorsque vous travaillez avec les formulaires, par exemple::

    use Symfony\Component\HttpFoundation\Request;

    public function updateAction(Request $request)
    {
        $form = $this->createForm(...);
        
        $form->bindRequest($request);
        // ...
    }

.. index::
   single: Le Contrôleur; La classe contrôleur de base

La Classe Contrôleur de Base
----------------------------

Afin de vous faciliter le travail, Symfony2 est fourni avec une classe ``Controller``
de base qui vous assiste dans les tâches les plus communes d'un contrôleur et
qui donne à votre propre classe contrôleur l'accès à n'importe quelle ressource
dont elle pourrait avoir besoin. En étendant cette classe ``Controller``, vous
pouvez tirer parti de plusieurs méthodes d'aide (« helper »).

Ajoutez le mot-clé ``use`` au-dessus de la classe ``Controller`` et modifiez
``HelloController`` pour qu'il l'étende :

.. code-block:: php

    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
          return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

Cela ne change en fait rien au fonctionnement de votre contrôleur. Dans la
prochaine section, vous apprendrez des choses sur les méthodes d'aide que la
classe contrôleur de base met à votre disposition. Ces méthodes sont juste
des raccourcis pour utiliser des fonctionnalités coeurs de Symfony2 qui sont
à votre disposition en utilisant ou non la classe ``Controller`` de base.
Une bonne façon de voir cette fonctionnalité coeur en action est de regarder
la classe :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`
elle-même.

.. tip::

    Etendre la classe de base est *optionnel* dans Symfony; elle contient
    des raccourcis utiles mais rien d'obligatoire. Vous pouvez aussi étendre
    ``Symfony\Component\DependencyInjection\ContainerAware``. L'objet conteneur
    de service (« service container ») sera ainsi accessible à travers la
    propriété ``container``.

.. note::

    Vous pouvez aussi définir vos :doc:`Contrôleurs en tant que Services
    </cookbook/controller/service>`.

.. index::
   single: Controller; Common Tasks

Les Tâches Communes du Contrôleur
---------------------------------

Bien qu'un contrôleur puisse effectuer quoi que soit virtuellement, la plupart
d'entre eux va accomplir les mêmes tâches basiques encore et toujours. Ces tâches,
tel rediriger, forwarder, rendre des templates et accéder aux services,
sont très faciles à gérer dans Symfony2.

.. index::
   single: Le Contrôleur; Rediriger

Rediriger
~~~~~~~~~

Si vous voulez rediriger l'utilisateur sur une autre page, utilisez la méthode
``redirect()``::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'));
    }

La méthode ``generateUrl()`` est juste une fonction d'aide qui génère une URL
pour une route donnée. Pour plus d'informations, lisez le chapitre
:doc:`Routage </book/routing>`.

Par défaut, la méthode ``redirect()`` produit une redirection 302 (temporaire).
Afin d'exécuter une redirection 301 (permanente), modifiez le second argument::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'), 301);
    }

.. tip::

    La méthode ``redirect()`` est simplement un raccourci qui crée un objet
    ``Response`` spécialisé dans la redirection d'utilisateur. Cela revient
    à faire :

    .. code-block:: php

        use Symfony\Component\HttpFoundation\RedirectResponse;

        return new RedirectResponse($this->generateUrl('homepage'));

.. index::
   single: Le Contrôleur; Forwarder

Forwarder
~~~~~~~~~

Vous pouvez aussi facilement forwarder sur un autre contrôleur en interne avec la
méthode ``forward()``. Plutôt que de rediriger le navigateur de l'utilisateur, elle
effectue une sous-requête interne, et appelle le contrôleur spécifié. La méthode
``forward()`` retourne l'objet ``Response`` qui est retourné par ce contrôleur::

    public function indexAction($name)
    {
        $response = $this->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green'
        ));

        // modifiez encore la réponse ou bien retournez-la directement

        return $response;
    }

Notez que la méthode `forward()` utilise la même représentation de chaîne
de caractères du contrôleur que celle utilisée dans la configuration de
routage. Dans ce cas, la classe contrôleur cible va être ``HelloController``
dans le bundle ``AcmeHelloBundle``. Le tableau passé à la méthode devient
les arguments du contrôleur. Cette même interface est utilisée lorsque vous
intégrez des contrôleurs dans des templates (voir :ref:`templating-embedding-controller`).
La méthode contrôleur cible devrait ressembler à quelque chose comme::

    public function fancyAction($name, $color)
    {
        // ... crée et retourne un objet Response
    }

Et comme quand vous créez un contrôleur pour une route, l'ordre des arguments
de ``fancyAction`` n'a pas d'importance. Symfony2 fait correspondre le nom
des clés d'index (par exemple: ``name``) avec le nom des arguments de la
méthode (par exemple: ``$name``). Si vous changez l'ordre des arguments,
Symfony2 va toujours passer la valeur correcte à chaque variable.

.. tip::

    Comme d'autres méthodes de base de ``Controller``, la méthode ``forward``
    est juste un raccourci pour une fonctionnalité coeur de Symfony2. Un
    forward peut être exécuté directement via le service ``http_kernel``.
    Un forward retourne un objet ``Response``::
    
        $httpKernel = $this->container->get('http_kernel');
        $response = $httpKernel->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green',
        ));

.. index::
   single: Le Contrôleur; Rendre des templates

.. _controller-rendering-templates:

Rendre des Templates
~~~~~~~~~~~~~~~~~~~~

Bien que n'étant pas une condition requise, la plupart des contrôleurs vont finalement
délivrer un template qui est responsable de la génération du HTML (ou d'un autre format)
pour le contrôleur. La méthode ``renderView()`` rend un template et retourne son contenu.
Le contenu du template peut être utilisé pour créer un objet ``Response``::

    $content = $this->renderView('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

    return new Response($content);

Ceci peut même être effectué en une seule étape à l'aide de la méthode ``render()``,
qui retourne un objet ``Response`` contenant le contenu du template::

    return $this->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

Dans les deux cas, le template ``Resources/views/Hello/index.html.twig`` dans
``AcmeHelloBundle`` sera délivré.

Le moteur de rendu (« templating engine ») de Symfony est expliqué plus en détails dans
le chapitre :doc:`Templating </book/templating>`

.. tip::

    La méthode ``renderView`` est un raccourci de l'utilisation directe du
    service ``templating``. Ce dernier peut aussi être utilisé directement::
    
        $templating = $this->get('templating');
        $content = $templating->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

.. index::
   single: Le Contrôleur; Accéder aux services

Accéder à d'autres Services
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quand vous étendez la classe contrôleur de base, vous pouvez utiliser n'importe
quel service Symfony2 via la méthode ``get()``. Voici plusieurs services communs
dont vous pourriez avoir besoin::

    $request = $this->getRequest();

    $templating = $this->get('templating');

    $router = $this->get('router');

    $mailer = $this->get('mailer');

Il y a d'innombrables autres services à votre disposition et vous êtes encouragé
à définir les vôtres. Pour lister tous les services disponibles, utilisez la
commande de la console ``container:debug``:

.. code-block:: bash

    php app/console container:debug

Pour plus d'informations, voir le chapitre :doc:`/book/service_container`.

.. index::
   single: Le Contrôleur; Gérer les erreurs
   single: Le Contrôleur; Les pages 404

Gérer les Erreurs et les Pages 404
----------------------------------

Quand « quelque chose » n'est pas trouvé, vous devriez vous servir correctement
du protocole HTTP et retourner une réponse 404. Pour ce faire, vous allez lancer
un type spécial d'exception. Si vous étendez la classe contrôleur de base, faites
comme ça::

    public function indexAction()
    {
        $product = // récupérer l'objet depuis la base de données
        if (!$product) {
            throw $this->createNotFoundException('Le produit n\'existe pas');
        }

        return $this->render(...);
    }

La méthode ``createNotFoundException()`` crée un objet spécial ``NotFoundHttpException``,
qui finalement déclenche une réponse HTTP 404 dans Symfony.

Evidemment, vous êtes libre de lancer une quelconque classe ``Exception`` dans votre
contrôleur - Symfony2 retournera automatiquement un code de réponse HTTP 500.

.. code-block:: php

    throw new \Exception('Quelque chose s'est mal passé!');

Dans chaque cas, une page d'erreur stylée est retournée à l'utilisateur final et une
page d'erreur complète avec des infos de debugging est retournée au développeur
(lorsqu'il affiche cette page en mode debug). Ces deux pages d'erreur peuvent
être personnalisées. Pour de plus amples détails, lisez la partie du cookbook
« :doc:`/cookbook/controller/error_pages` ».

.. index::
   single: Le Contrôleur; La session
   single: Session

Gérer la Session
----------------

Symfony2 fournit un objet session sympa que vous pouvez utiliser pour stocker
de l'information à propos de l'utilisateur (que ce soit une personne réelle
utilisant un navigateur, un bot, ou un service web) entre les requêtes. Par
défaut, Symfony2 stocke les attributs dans un cookie en utilisant les sessions
natives de PHP.

Stocker et récupérer des informations depuis la session peut être effectué
facilement depuis n'importe quel contrôleur::

    $session = $this->getRequest()->getSession();

    // stocke un attribut pour une réutilisation lors d'une future requête utilisateur
    $session->set('foo', 'bar');

    // dans un autre contrôleur pour une autre requête
    $foo = $session->get('foo');

    // définit la « locale » de l'utilisateur
    $session->setLocale('fr');

Ces attributs vont rester affectés à cet utilisateur pour le restant de sa
session.

.. index::
   single Session; Les messages Flash

Les Messages Flash
~~~~~~~~~~~~~~~~~~

Vous pouvez aussi stocker de petits messages qui vont être gardés dans la session
de l'utilisateur pour la requête suivante uniquement. Ceci est utile lors
du traitement d'un formulaire : vous souhaitez rediriger l'utilisateur et afficher un
message spécial lors de la *prochaine* requête. Ces types de message sont appelés
messages « flash ».

Par exemple, imaginez que vous traitiez la soumission d'un formulaire::

    public function updateAction()
    {
        $form = $this->createForm(...);

        $form->bindRequest($this->getRequest());
        if ($form->isValid()) {
            // effectue le traitement du formulaire

            $this->get('session')->setFlash('notice', 'Vos changements ont été sauvegardés!');

            return $this->redirect($this->generateUrl(...));
        }

        return $this->render(...);
    }

Après avoir traité la requête, le contrôleur définit un message flash ``notice``
et puis redirige l'utilisateur. Le nom (``notice``) n'est pas significatif - c'est
juste ce que vous utilisez pour identifier le type du message.

Dans le template de la prochaine action, le code suivant pourrait être utilisé
pour délivrer le message ``notice`` :

.. configuration-block::

    .. code-block:: html+jinja

        {% if app.session.hasFlash('notice') %}
            <div class="flash-notice">
                {{ app.session.flash('notice') }}
            </div>
        {% endif %}

    .. code-block:: php
    
        <?php if ($view['session']->hasFlash('notice')): ?>
            <div class="flash-notice">
                <?php echo $view['session']->getFlash('notice') ?>
            </div>
        <?php endif; ?>

De par leur conception, les messages flash sont faits pour durer pendant exactement une
requête (ils « disparaissent en un éclair/flash »). Ils sont conçus pour être utilisés
au travers des redirections exactement comme vous l'avez fait dans cet exemple.

.. index::
   single: Le Contrôleur; L'objet response

L'Objet Response
----------------

La seule condition requise d'un contrôleur est de retourner un objet ``Response``.
La classe :class:`Symfony\\Component\\HttpFoundation\\Response` est une abstraction
PHP autour de la réponse HTTP - le message texte rempli avec des en-têtes HTTP et
du contenu qui est envoyé au client::

    // crée une simple Réponse avec un code de statut 200 (celui par défaut)
    $response = new Response('Hello '.$name, 200);
    
    // crée une réponse JSON avec un code de statut 200
    $response = new Response(json_encode(array('name' => $name)));
    $response->headers->set('Content-Type', 'application/json');

.. tip::

    La propriété ``headers`` (en-têtes en français) est un objet
    :class:`Symfony\\Component\\HttpFoundation\\HeaderBag` avec plusieurs
    méthodes utiles pour lire et transformer les en-têtes de la ``Response``.
    Les noms des en-têtes sont normalisés et ainsi, utiliser ``Content-Type``
    est équivalent à ``content-type`` ou même ``content_type``.

.. index::
   single: Le Contrôleur; L'objet request

L'Objet Request
---------------

En sus des valeurs de substitution du routage, le contrôleur a aussi accès à
l'objet ``Request`` quand il étend la classe ``Controller`` de base::

    $request = $this->getRequest();

    $request->isXmlHttpRequest(); // est-ce une requête Ajax?

    $request->getPreferredLanguage(array('en', 'fr'));

    $request->query->get('page'); // prend un paramètre $_GET

    $request->request->get('page'); // prend un paramètre $_POST

Comme l'objet ``Response``, les en-têtes de la requête sont stockées dans un
objet ``HeaderBag`` et sont facilement accessibles.

Le mot de la fin
----------------

Chaque fois que vous créez une page, vous allez au final avoir besoin
d'écrire du code qui contient la logique de cette page. Dans Symfony, ceci
est appelé un contrôleur, et c'est une fonction PHP qui peut faire tout ce
qu'il faut pour retourner l'objet final ``Response`` qui sera delivré à
l'utilisateur.

Pour vous simplifier la vie, vous pouvez choisir d'étendre une classe ``Controller``
de base, qui contient des méthodes raccourcis pour de nombreuses tâches
communes d'un contrôleur. Par exemple, sachant que vous ne voulez pas mettre
de code HTML dans votre contrôleur, vous pouvez utiliser la méthode ``render()``
pour délivrer et retourner le contenu d'un template.

Dans d'autres chapitres, vous verrez comment le contrôleur peut être utilisé
pour sauvegarder et aller chercher des objets dans une base de données, traiter
des soumissions de formulaires, gérer le cache et plus encore.

En savoir plus grâce au Cookbook
--------------------------------

* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/controller/service`
