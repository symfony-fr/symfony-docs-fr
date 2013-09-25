:orphan:

Glossaire
=========

.. glossary::
   :sorted:

   Distribution
        Une *Distribution* est un ensemble fait de composants Symfony2, une
        sélection de bundles, un structure pertinente, une configuration par
        défaut, et un système de configuration optionnel.

   Projet
        Un *Projet* est un répertoire composé d'une Application, un ensemble de
        bundles, des bibliothèques tierces, un chargeur automatique (autoloader), et
        des contrôleurs frontaux.

   Application
        Une *Application* est un répertoire qui contient une *configuration* pour un
        ensemble de Bundles donné.

   Bundle
        Un *Bundle* est un répertoire qui contient un ensemble de fichiers (classes PHP,
        feuilles de style, JavaScripts, images, ...) qui *implémentent* une fonctionnalité
        unique (un blog, un forum, etc). Dans Symfony2, (*presque*) tout se trouve dans
        un bundle. (voir :ref:`page-creation-bundles`)

   Contrôleur frontal
        Un *Contrôleur Frontal* (Front Controller) est un court script PHP qui se trouve
        dans le dossier web de votre projet. Typiquement, *toute* requête est prise en charge
        en éxécutant le même contrôleur frontal, dont le rôle est de démarrer l'application
        Symfony.

   Contrôleur
        Un *contrôleur* est une fonction PHP qui contient la logique nécessaire afin
        de retourner un objet ``Response`` représentant une page particulière.
        Typiquement, une route est associée au contrôleur qui utilise les informations de la
        requête pour traiter les informations, éxécuter des actions, et finalement construire
        et retourner un objet ``Response``.

   Service
        Un *Service* est un terme générique pour tout objet PHP qui éxécute une tâche
        spécifique. Un service est souvent utilisé « globalement », comme un objet
        de connexion à une base de données ou un objet qui envoie des emails. Dans
        Symfony2, un service est souvent configuré et récupéré par le conteneur de
        services. Une application qui a de nombreux services découplés suit une
        `architecture orientée services`_.

   Conteneur de services
        Un *Conteneur de services*, aussi connu sous le nom de
        *Conteneur d'Injection de Dépendances*, est un objet spécial qui gère
        l'instanciation des services au sein d'une application. Plutôt que de créer
        les services directement, le développeur *prépare* le conteneur de services
        (via la configuration) sur la manière de créer les serrvices. Le conteneur
        de services prend en charge l'instanciation et l'injection des services
        dépendants. Lisez le chapitre :doc:`/book/service_container`.

   Spécification HTTP
        La *Spécification Http* est un document qui décrit le protocole HTTP (HyperText
        Transfer Protocol), c'est-à-dire un ensemble de règles qui définissent les
        échanges classiques client-serveur requête-réponse. La spécification définit
        le format utilisé pour une requête et une réponse, tout comme les différents
        entêtes possibles que chacun peut avoir. Pour plus d'informations, lisez
        l'article `Wikipedia sur HTTP`_ ou la `HTTP 1.1 RFC`_.

   Environnement
        Un environnement est une chaîne de caractères (ex ``prod`` ou ``dev``) qui
        correspond à un ensemble de configurations spécifiques. La même application peut
        être éxécutée sur la même machine avec une configuration différente en l'éxécutant
        dans différents environnements. C'est très utile puisque cela permet à une
        application unique d'avoir un environnement de ``dev`` conçu pour débuguer et
        un environnement de ``prod`` qui est optimisé pour de meilleures performances.

   Vendor
        Un *vendor* est un fournisseur de bibliothèques PHP et de bundles, incluant
        Symfony2 lui-même. Malgré la connotation commercial du terme, les vendors
        de Symfony sont souvent (et même très souvent) des logiciels libres. Toute
        bibliothèque que vous ajoutez dans votre projet Symfony2 devrait se trouver
        dans le répertoire ``vendor``. Lisez
        :ref:`L'Architecture: Utilisation de bibliothèques externes <using-vendors>`.

   Acme
        *Acme* est un exemple d'entreprise utilisé dans Symofny pour les exemples et
        la documentation. Il est utilisé dans les namespaces où vous devriez normalement
        utiliser votre propre nom d'entreprise (ex ``Acme\BlogBundle``).

   Action
        Une *action* est une fonction PHP ou une méthode qui s'éxécute, par exemple,
        lorsqu'une route correspondante est trouvée. Le terme action est synonyme
        de *contrôleur*, bien qu'un contrôleur fasse aussi référence à une classe PHP entière
        qui inclut plusieurs actions. Lisez le :doc:`chapitre sur les Contrôleurs </book/controller>`.

   Asset
        Un *asset* désigne tout ce qui n'est pas éxécutable, composants web statiques
        incluant les CSS, le JavaScript, les images et les vidéos. Les assets peuvent être
        placés directement dans le répertoire``web`` du projet, ou copiés par un :term:`Bundle`
        dans le dossier web en utilisant la commande ``assets:install``.

   Kernel
        Le *Kernel* (noyau) est le coeur de Symfony2. L'objet Kernel prend en charge
        les requêtes HTTP en utilisant tous les bundles et bibliothèques qui sont enregistrés.
        Lisez :ref:`L'Architecture : Le répertoire Application<the-app-dir>` et le chapitre
        :doc:`/book/internals`.

   Firewall
        Dans Symfony2, un *Firewall* n'a rien à voir avec le réseau. En fait,
        il définit les mécanismes d'authentification (c'est-à-dire qu'il prend
        en charge le processus d'identification de vos utilisateurs) pour toute
        l'application ou juste une partie de celle-ci. Lisez le chapitre
        :doc:`/book/security`.

   Yaml
        *YAML* est un acronyme récursif pour « YAML Ain't a Markup Language ». Il s'agit
        d'un langage de sérialisation de données léger et intuitif, utilisé abondamment
        dans les fichiers de configuration Symfony2. Lisez le chapitre :doc:`/components/yaml`.

   Injection de Dépendance
        L'Injection de Dépendance est un patron de conception (design pattern) très fortement 
        utilisé dans le framework Symfony2. Il encourage la mise en place d'une architecture 
        d'application moins couplée et plus maintenable. Le principe majeur de ce design pattern 
        est de permettre aux développeurs d'injecter des objets (aussi connus sous le nom de services) 
        dans d'autres objets, en les passant généralement en tant que paramètres d'un constructeur 
        ou d'un mutateur. Différents niveaux de couplage peuvent être établis en fonction de 
        la méthode utilisée pour injecter les objets entre eux. Le pattern d'Injection de Dépendance 
        est très souvent associé à un autre type d'objet: le :doc:`/book/service_container` 

.. _`architecture orientée services`: http://fr.wikipedia.org/wiki/Architecture_orient%C3%A9e_services
.. _`Wikipedia sur HTTP`: http://fr.wikipedia.org/wiki/HTTP
.. _`HTTP 1.1 RFC`: http://www.w3.org/Protocols/rfc2616/rfc2616.html
