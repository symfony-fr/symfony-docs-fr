:orphan:

Glossaire
=========

.. glossary::
   :sorted:

   Distribution
        Une *Distribution* est un ensemble de composants Symfony2, une sélection
        de bundles, une arborescence structurée, une configuration par défaut et
        un système de configuration optionnel.

   Projet
        Un *Projet* est une arborescence composée d'une Application, d'un
        certain nombre de "bundles", de librairies tierces, d'un autoloader et
        de scripts.

   Application
        Une *Application* est une arborescence contenant la *configuration* pour
        un certain nombre de "Bundles" donné.

   Bundle
        Un *Bundle* est un ensemble structuré de fichiers (scripts PHP, feuilles
        de style CSS, javascripts, images,...) qui *implémente* une fonctionalité
        unique (un blog, un forum, etc...). Dans Symfony2, (*presque*) tout 
        réside dans un bundle. (voir :ref:`page-creation-bundles`)

   Contrôleur Frontal
        Un Contrôleur Frontal est un petit script PHP situé dans le répertoire 
        web de votre projet. En règle générale, *toutes* les requêtes sont 
        traitées par l'exécution du même contrôleur frontal, dont la mission est
        d'amorcer votre application Symfony.

   Contrôleur
        Un *contrôleur* est une fonction PHP qui détient toute la logique nécessaire
        pour retourner un objet ``Réponse`` qui représente une page particulière.
        Typiquement, une route est associée à un contrôleur qui utilise ensuite 
        informations de la requête pour la traiter, effectuer des actions et
        finalement construire puis retourner un objet ``Réponse``.

   Service
        Un *Service* est un terme générique désignant tout objet PHP qui effectue
        une tâche spécifique. Un service est généralement utilisé «globalement»,
        comme un objet de connexion à la base de données ou un objet qui envoie
        des e-mails par exemple. Dans Symfony2, les services sont souvent
        configurés et récupérés à partir du conteneur de services. Une application
        qui a de nombreux services découplés est qualifiée d'`architecture
        orientée services`_.

   Conteneur de Services
        Un *Conteneur de Services*, aussi nommé *Conteneur d'Injection de
        Dépendances*, est un objet spécial qui gère les instances de services au
        cœur de l'application. Au lieu de créer des services directement, le
        développeur *éduque* le conteneur de services (en le paramétrant) sur
        la façon de créer des services. Le contrôleur de services prend en charge
        l'instantiation passive et l'injection des services qui en dépendent.
        Voir le chapitre :doc:`/book/service_container`.

   Specification HTTP
        La *Specification HTTP* est un document de référence qui décrit le
        Protocole de Transfert HTTP (un ensemble de règles portant sur la
        communication requête-réponse d'un traditionnel client-serveur). Cette
        spécification définit le format utilisé pour la requête et la réponse 
        ainsi que les différents entêtes HTTP possibles que chacun peut avoir.
        Pour plus d'informations, lisez l'article sur `Wikipedia`_ ou le 
        `HTTP 1.1 RFC`_.

   Environment
        An environment is a string (e.g. ``prod`` or ``dev``) that corresponds
        to a specific set of configuration. The same application can be run
        on the same machine using different configuration by running the application
        in different environments. This is useful as it allows a single application
        to have a ``dev`` environment built for debugging and a ``prod`` environment
        that's optimized for speed.

   Vendor
        A *vendor* is a supplier of PHP libraries and bundles including Symfony2
        itself. Despite the usual commercial connotations of the word, vendors
        in Symfony often (even usually) include free software. Any library you
        add to your Symfony2 project should go in the ``vendor`` directory. See
        :ref:`The Architecture: Using Vendors <using-vendors>`.

   Acme
        *Acme* is a sample company name used in Symfony demos and documentation.
        It's used as a namespace where you would normally use your own company's
        name (e.g. ``Acme\BlogBundle``).

   Action
        An *action* is a PHP function or method that executes, for example,
        when a given route is matched. The term action is synonymous with
        *controller*, though a controller may also refer to an entire PHP
        class that includes several actions. See the :doc:`Controller Chapter </book/controller>`.

   Asset
        An *asset* is any non-executable, static component of a web application,
        including CSS, JavaScript, images and video. Assets may be placed
        directly in the project's ``web`` directory, or published from a :term:`Bundle`
        to the web directory using the ``assets:install`` console task.

   Kernel
        The *Kernel* is the core of Symfony2. The Kernel object handles HTTP
        requests using all the bundles and libraries registered to it. See
        :ref:`The Architecture: The Application Directory <the-app-dir>` and the
        :doc:`/book/internals/kernel` chapter.

   Firewall
        In Symfony2, a *Firewall* doesn't have to do with networking. Instead,
        it defines the authentication mechanisms (i.e. it handles the process
        of determining the identity of your users), either for the whole
        application or for just a part of it. See the
        :doc:`/book/security` chapters.

   YAML 
        *YAML* is a recursive acronym for "YAML Ain't a Markup Language". It's a
        lightweight, humane data serialization language used extensively in
        Symfony2's configuration files.  See the :doc:`/reference/YAML` reference
        chapter.




.. _`architecture orientée services`: http://fr.wikipedia.org/wiki/Architecture_orient%C3%A9e_services
.. _`Wikipedia`: http://fr.wikipedia.org/wiki/Hypertext_Transfer_Protocol
.. _`HTTP 1.1 RFC`: http://www.w3.org/Protocols/rfc2616/rfc2616.html
