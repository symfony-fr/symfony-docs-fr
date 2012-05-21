.. index::
   single: Traductions

Traductions
===========

Le terme « internationalisation » (souvent abrégé `i18n`_)désigne le processus d'abstraction des textes
et autres spécificités locales en dehors de votre application qui sont ensuite placés
dans un fichier où ils peuvent être traduits et convertis en se basant sur la locale de
l'utilisateur (i.e. la langue et le pays). Pour du texte, cela signifie l'encadrer avec
une fonction capable de traduire le texte (ou « message ») dans la langue de l'utilisateur :

.. code-block:: php

    // le texte va *toujours* s'afficher en anglais
    echo 'Hello World';

    // le texte peut être traduit dans la langue de l'utilisateur final ou par défaut en anglais
    echo $translator->trans('Hello World');

.. note::

    Le terme *locale* désigne en gros la langue et le pays de l'utilisateur. Cela
    peut être n'importe quelle chaîne de caractères que votre application va utiliser
    pour gérer les traductions et autres différences de format (par ex. format de monnaie).
    Nous recommandons le code *langue* `ISO639-1`_, un « underscore » (``_``), et ensuite le code
    *pays* `ISO3166 Alpha-2`_ (par ex. ``fr_FR`` pour Français/France).

Dans ce chapitre, nous allons apprendre comment préparer une application à gérer de multiples
locales et ensuite comment créer des traductions pour plusieurs locales. Dans l'ensemble,
le processus a plusieurs étapes communes :
    
1. Activer et configurer le composant ``Translation`` de Symfony ;

2. Faire abstraction des chaînes de caractères (i.e. « messages ») en les encadrant
   avec des appels au ``Translator`` ;

3. Créer des ressources de traduction pour chaque locale supportée qui traduit
   chaque message dans l'application ;

4. Déterminer, définir et gérer la locale de l'utilisateur dans la requête, et
   optionnellement dans la session.


.. index::
   single: Traductions; Configuration

Configuration
-------------

Les traductions sont traitées par le :term:`service` ``Translator`` qui utilise la 
locale de l'utilisateur pour chercher et retourner les messages traduits. Avant de l'utiliser,
activez le ``Translator`` dans votre configuration :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            translator: { fallback: en }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:translator fallback="en" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'translator' => array('fallback' => 'en'),
        ));

L'option ``fallback`` définit la locale de secours à utiliser quand une traduction
n'existe pas dans la locale de l'utilisateur.

.. tip::

    Quand une traduction n'existe pas pour une locale donnée, le traducteur (« Translator »)
    essaye tout d'abord de trouver une traduction pour cette langue (``fr`` si
    la locale est ``fr_FR`` par exemple). Si cela échoue également, il regarde
    alors pour une traduction utilisant la locale de secours.

La locale utilisée pour les traductions est celle qui est stockée dans la requête.
Vous pouvez la définir grâce à l'attribut ``_locale`` de vos routes (:ref:`book-translation-locale-url`).

.. index::
   single: Traductions; Traduction basique

Traduction basique
------------------

La traduction du texte est faite à travers le service ``translator`` 
(:class:`Symfony\\Component\\Translation\\Translator`). Pour traduire un bloc 
de texte (appelé un *message*), utilisez la méthode
:method:`Symfony\\Component\\Translation\\Translator::trans`. Supposons,
par exemple, que nous traduisons un simple message dans un contrôleur :

.. code-block:: php

    public function indexAction()
    {
        $t = $this->get('translator')->trans('Symfony2 is great');

        return new Response($t);
    }

Quand ce code est exécuté, Symfony2 va essayer de traduire le message
« Symfony2 is great » en se basant sur la ``locale`` de l'utilisateur. Pour que
cela marche, nous devons dire à Symfony2 comment traduire le message via une
« ressource de traduction », qui est une collection de traductions de messages
pour une locale donnée. Ce « dictionnaire » de traduction peut être créé en
plusieurs formats différents, XLIFF étant le format recommandé :

.. configuration-block::

    .. code-block:: xml

        <!-- messages.fr.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony2 is great</source>
                        <target>J'aime Symfony2</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // messages.fr.php
        return array(
            'Symfony2 is great' => 'J\'aime Symfony2',
        );

    .. code-block:: yaml

        # messages.fr.yml
        Symfony2 is great: J'aime Symfony2

Maintenant, si la langue de la locale de l'utilisateur est le français, (par ex. ``fr_FR``
ou ``fr_BE``), le message va être traduit par ``J'aime Symfony2``.

Le processus de traduction
~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour traduire le message, Symfony2 utilise un processus simple :

* La ``locale`` de l'utilisateur actuel, qui est stockée dans la requête (ou stockée
  comme ``_locale`` en session), est déterminée ;

* Un catalogue des messages traduits est chargé depuis les ressources de traduction définies
  pour la ``locale`` (par ex. ``fr_FR``). Les messages de la locale de secours sont aussi
  chargés et ajoutés au catalogue s'ils n'existent pas déjà. Le résultat final est un large
  « dictionnaire » de traductions. Voir `Catalogues de Message`_ pour plus de détails ;

* Si le message est dans le catalogue, la traduction est retournée. Sinon, le traducteur
  retourne le message original.
  
Lorsque vous utilisez la méthode ``trans()``, Symfony2 cherche la chaîne de caractères
exacte à l'intérieur du catalogue de messages approprié et la retourne (si elle existe).

.. index::
   single: Traductions; Message avec paramètres de substitution

Message avec paramètres de substitution
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Parfois, un message contenant une variable a besoin d'être traduit :

.. code-block:: php

    public function indexAction($name)
    {
        $t = $this->get('translator')->trans('Hello '.$name);

        return new Response($t);
    }

Cependant, créer une traduction pour cette chaîne de caractères est impossible étant
donné que le traducteur va essayer de trouver le message exact, incluant les portions
de la variable (par ex. « Hello Ryan » ou « Hello Fabien »). Au lieu d'écrire une traduction
pour toutes les itérations possibles de la variable ``$name``, nous pouvons remplacer la
variable avec un paramètre de substitution (« placeholder ») :

.. code-block:: php

    public function indexAction($name)
    {
        $t = $this->get('translator')->trans('Hello %name%', array('%name%' => $name));

        new Response($t);
    }

Symfony2 va maintenant chercher une traduction du message brut (``Hello %name%``)
et *ensuite* remplacer les paramètres de substitution avec leurs valeurs. Créer une traduction
se fait comme précédemment :

.. configuration-block::

    .. code-block:: xml

        <!-- messages.fr.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Hello %name%</source>
                        <target>Bonjour %name%</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // messages.fr.php
        return array(
            'Hello %name%' => 'Bonjour %name%',
        );

    .. code-block:: yaml

        # messages.fr.yml
        'Hello %name%': Hello %name%

.. note::

    Les paramètres de substitution peuvent prendre n'importe quelle forme
    puisque le message en entier est reconstruit en utilisant la fonction
    PHP `strtr`_. Cependant, la notation ``%var%`` est requise
    pour les traductions dans les templates Twig, et c'est une convention
    générale à suivre.
    
Comme nous l'avons vu, créer une traduction est un processus en deux étapes :

1. Faire abstraction du message qui a besoin d'être traduit en le passant à travers 
   le ``Translator``.

2. Créer une traduction pour le message dans chaque locale que vous avez choisi de 
   supporter.

La deuxième étape est faite en créant des catalogues de messages qui définissent les traductions
pour chacune des différentes locales.

.. index::
   single: Traduction; Catalogues de Message

Catalogues de Message
---------------------

Lorsqu'un message est traduit, Symfony2 compile un catalogue de messages pour la
locale de l'utilisateur et cherche dedans une traduction du message. Un catalogue
de messages est comme un dictionnaire de traductions pour une locale spécifique. 
Par exemple, le catalogue de la locale ``fr_FR`` pourrait contenir la traduction
suivante :

    Symfony2 is Great => J'aime Symfony2

C'est la responsabilité du développeur (ou du traducteur) d'une application
internationalisée de créer ces traductions. Les traductions sont stockées sur le
système de fichiers et reconnues par Symfony, grâce à certaines conventions.
    
.. tip::

    Chaque fois que vous créez une *nouvelle* ressource de traduction (ou installez un bundle
    qui comprend une ressource de traduction), assurez-vous de vider votre cache afin
    que Symfony puisse reconnaître les nouvelles traductions :

    .. code-block:: bash
    
        php app/console cache:clear

.. index::
   single: Traductions; Emplacements des ressources de traduction

Emplacements des Traductions et Conventions de Nommage
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 cherche les fichiers de messages (c-a-d les traductions) aux endroits suivants :

* Le répertoire ``<répertoire racine du noyau>/Resources/translations``;

* Le répertoire ``<répertoire racine du noyau>/Resources/<nom du bundle>/translations``;

* Le répertoire ``Resources/translations/`` du bundle.

Les répertoire sont listés par ordre de priorité. Cela signifie que vous pouvez
surcharger les messages de traduction d'un bundle dans l'un des deux premiers répertoires.

Le système de surcharge se base sur les clés : seules les clés surchargées ont
besoin d'être listées dans un fichier de plus grande priorité. Quand une clé n'est
pas trouvée dans un fichier de traductions, le service Translator cherchera automatiquement
dans les fichiers de moindre priorité.

Le nom des fichiers de traductions est aussi important puisque Symfony2 utilise une convention
pour déterminer les détails à propos des traductions. Chaque fichier de messages doit être nommé
selon le schéma suivant : ``domaine.locale.format`` :

* **domaine**: Une façon optionnelle d'organiser les messages dans des groupes (par ex. ``admin``,
  ``navigation`` ou ``messages`` par défaut) - voir `Utiliser les Domaines de Message`_;

* **locale**: La locale de la traduction (par ex. ``en_GB``, ``en``, etc);

* **format**: Comment Symfony2 doit charger et analyser le fichier (par ex. ``xliff``,
  ``php`` ou ``yml``).

La valeur du format peut être le nom de n'importe quel format enregistré. Par défaut, Symfony
fournit les formats suivants :

* ``xliff`` : fichier XLIFF ;
* ``php`` :   fichier PHP ;
* ``yml`` :  fichier YAML.

Le choix du format à utiliser dépend de vous, c'est une question de goût.

.. note::

    Vous pouvez également stocker des traductions dans une base de données, ou
    tout autre système de stockage en fournissant une classe personnalisée implémentant
    l'interface :class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface`.

.. index::
   single: Traductions; Créer les ressources de traduction

Créer les Traductions
~~~~~~~~~~~~~~~~~~~~~

Le fait de créer des fichiers de traduction est une partie importante de la 
« localisation » (souvent abrégée `L10n`_). Les fichiers de traduction consistent
en une série de paires id-traduction pour un domaine et une locale donnés.
La source est l'identifiant de la traduction individuelle, et peut
être le message dans la locale principale (par exemple « Symfony is great ») de votre application
ou un identificateur unique (par exemple « symfony2.great » - voir l'encadré ci-dessous) :

.. configuration-block::
    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/translations/messages.fr.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony2 is great</source>
                        <target>J'aime Symfony2</target>
                    </trans-unit>
                    <trans-unit id="2">
                        <source>symfony2.great</source>
                        <target>J'aime Symfony2</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // src/Acme/DemoBundle/Resources/translations/messages.fr.php
        return array(
            'Symfony2 is great' => 'J\'aime Symfony2',
            'symfony2.great'    => 'J\'aime Symfony2',
        );

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/translations/messages.fr.yml
        Symfony2 is great: J'aime Symfony2
        symfony2.great:    J'aime Symfony2

Symfony2 va reconnaître ces fichiers et les utiliser lors de la traduction de
« Symfony2 is great » ou de « symfony2.great » dans une locale de langue française
(par ex. ``fr_FR`` or ``fr_BE``).

.. sidebar:: Utiliser des mots-clés ou des messages

    Cet exemple illustre les deux philosophies différentes lors de la création
    des messages à traduire :

    .. code-block:: php

        $t = $translator->trans('Symfony2 is great');

        $t = $translator->trans('symfony2.great');

    Dans la première méthode, les messages sont écrits dans la langue de la
    locale par défaut (anglais dans ce cas). Ce message est ensuite utilisé
    comme « id » lors de la création des traductions.
    
    Dans la seconde méthode, les messages sont en fait des « mots-clés » qui évoquent
    l'idée du message. Le message mot-clé est ensuite utilisé comme « id » pour
    toutes les traductions. Dans ce cas, les traductions doivent (aussi) être faites pour la
    locale par défaut (i.e. pour traduire ``symfony2.great`` en ``Symfony2 is great``).
    
    La deuxième méthode est très pratique car la clé du message n'aura pas besoin d'être modifiée
    dans chaque fichier de traduction si nous décidons que le message devrait en fait
    être « Symfony2 is really great » dans la locale par défaut.
    
    Le choix de la méthode à utiliser dépend entièrement de vous, mais le format « mot-clé »
    est souvent recommandé.
    
    En outre, les formats de fichiers ``php`` et ``yaml`` prennent en charge les ids imbriqués pour
    éviter de vous répéter, si vous utilisez des mots-clés plutôt que du texte brut comme id :
    
    .. configuration-block::

        .. code-block:: yaml

            symfony2:
                is:
                    great: Symfony2 is great
                    amazing: Symfony2 is amazing
                has:
                    bundles: Symfony2 has bundles
            user:
                login: Login

        .. code-block:: php

            return array(
                'symfony2' => array(
                    'is' => array(
                        'great' => 'Symfony2 is great',
                        'amazing' => 'Symfony2 is amazing',
                    ),
                    'has' => array(
                        'bundles' => 'Symfony2 has bundles',
                    ),
                ),
                'user' => array(
                    'login' => 'Login',
                ),
            );

    Les multiples niveaux sont convertis en paires uniques id / traduction par
    l'ajout d'un point (.) entre chaque niveau ; donc les exemples ci-dessus sont
    équivalents à ce qui suit :
    
    .. configuration-block::

        .. code-block:: yaml

            symfony2.is.great: Symfony2 is great
            symfony2.is.amazing: Symfony2 is amazing
            symfony2.has.bundles: Symfony2 has bundles
            user.login: Login

        .. code-block:: php

            return array(
                'symfony2.is.great' => 'Symfony2 is great',
                'symfony2.is.amazing' => 'Symfony2 is amazing',
                'symfony2.has.bundles' => 'Symfony2 has bundles',
                'user.login' => 'Login',
            );

.. index::
   single: Traductions; Domaines de messages
   
Utiliser les Domaines de Messages
---------------------------------

Comme nous l'avons vu, les fichiers de messages sont organisés par les différentes locales
qu'ils traduisent. Pour plus de structure, les fichiers de messages peuvent également être organisés en
« domaines ». Lors de la création des fichiers de messages, le domaine est la première
partie du nom du fichier. Le domaine par défaut est ``messages``. Par exemple, supposons que,
par soucis d'organisation, les traductions ont été divisées en trois domaines différents : ``messages``,
``admin`` et ``navigation``. La traduction française aurait les fichiers de message suivants :

* ``messages.fr.xliff``
* ``admin.fr.xliff``
* ``navigation.fr.xliff``

Lors de la traduction de chaînes de caractères qui ne sont pas dans le domaine par défaut
(``messages``), vous devez spécifier le domaine comme troisième argument de ``trans()`` :

.. code-block:: php

    $this->get('translator')->trans('Symfony2 is great', array(), 'admin');

Symfony2 va maintenant chercher le message dans le domaine ``admin`` de la locale
de l'utilisateur.

.. index::
   single: Traductions; Locale de l'utilisateur

Gérer la Locale de l'Utilisateur
--------------------------------

La locale de l'utilisateur courant est stockée dans la session et est accessible
via l'objet ``request`` :

.. code-block:: php

    // access the reqest object in a standard controller
    $request = $this->getRequest();

    $request->setLocale('en_US');

.. index::
   single: Traductions; Solution de secours et locale par défaut

Il est aussi possible de stocker la locale en session plutôt qu'en requête. Si vous
faites cela, toutes les requêtes auront la même locale.

.. code-block:: php

    $this->get('session')->set('_locale', 'en_US');

Lisez le chapitre :ref:`book-translation-locale-url` pour voir comment définir
la locale dans vos routes.

Solution de Secours et Locale par Défaut
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si la locale n'a pas été explicitement définie dans la session, le paramètre de
configuration ``fallback_locale`` va être utilisé par le ``Translator``. Le paramètre
est défini comme ``en`` par défaut (voir `Configuration`_).

Alternativement, vous pouvez garantir que la locale soit définie dans chaque requête
de l'utilisateur en définissant le paramètre ``default_locale`` du framework :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            default_locale: en

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:default-locale>en</framework:default-locale>
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'default_locale' => 'en',
        ));

.. versionadded:: 2.1
     Le paramètre ``default_locale`` était à la base défini dans la clé session,
     cependant cela a changé dans la version 2.1. C'est parce que la locale est
     maintenant définie dans la requête et non plus dans la session

.. _book-translation-locale-url:

La locale et l'URL
~~~~~~~~~~~~~~~~~~

Puisque vous pouvez stocker la locale de l'utilisateur dans la session, il peut être tentant
d'utiliser la même URL pour afficher une ressource dans de nombreuses langues différentes
en se basant sur la locale de l'utilisateur. Par exemple, ``http://www.example.com/contact``
pourrait afficher le contenu en anglais pour un utilisateur, et en français pour un autre
utilisateur. Malheureusement, cela viole une règle fondamentale du Web qui dit qu'une URL
particulière retourne la même ressource indépendamment de l'utilisateur. Pour enfoncer encore
plus le clou, quel version du contenu serait indexée par les moteurs de recherche ?

Une meilleure politique est d'inclure la locale dans l'URL. Ceci est entièrement pris
en charge par le système de routage en utilisant le paramètre spécial ``_locale`` :

.. configuration-block::

    .. code-block:: yaml

        contact:
            pattern:   /{_locale}/contact
            defaults:  { _controller: AcmeDemoBundle:Contact:index, _locale: en }
            requirements:
                _locale: en|fr|de

    .. code-block:: xml

        <route id="contact" pattern="/{_locale}/contact">
            <default key="_controller">AcmeDemoBundle:Contact:index</default>
            <default key="_locale">en</default>
            <requirement key="_locale">en|fr|de</requirement>
        </route>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/{_locale}/contact', array(
            '_controller' => 'AcmeDemoBundle:Contact:index',
            '_locale'     => 'en',
        ), array(
            '_locale'     => 'en|fr|de'
        )));

        return $collection;

Lorsque vous utilisez le paramètre spécial `_locale` dans une route, la locale correspondante
sera *automatiquement définie dans la session de l'utilisateur*. En d'autres termes, si un
utilisateur visite l'URI ``/fr/contact``, la locale ``fr`` sera automatiquement définie comme
la locale de sa session.

Vous pouvez maintenant utiliser la locale de l'utilisateur pour créer des routes
pointant vers d'autres pages traduites de votre application.

.. index::
   single: Traductions; Pluralisation

Pluralisation
-------------

La pluralisation des messages est un sujet difficile car les règles peuvent être assez complexes. 
Par exemple, voici la représentation mathématique des règles de la pluralisation russe :

.. code-block:: text

    (($number % 10 == 1) && ($number % 100 != 11)) ? 0 : ((($number % 10 >= 2) && ($number % 10 <= 4) && (($number % 100 < 10) || ($number % 100 >= 20))) ? 1 : 2);

Comme vous pouvez le voir, en russe, vous pouvez avoir trois différentes formes de pluriel, chacune
donnant un index de 0, 1 ou 2. Pour chaque forme, le pluriel est différent, et ainsi la traduction
est également différente.

Quand une traduction a des formes différentes dues à la pluralisation, vous pouvez fournir
toutes les formes comme une chaîne de caractères séparée par un pipe (``|``):

.. code-block:: text

    'There is one apple|There are %count% apples'

Pour traduire des messages pluralisés, utilisez la méthode 
:method:`Symfony\\Component\\Translation\\Translator::transChoice` :

.. code-block:: php

    $t = $this->get('translator')->transChoice(
        'There is one apple|There are %count% apples',
        10,
        array('%count%' => 10)
    );

Le second paramètre (``10`` dans cet exemple), est le *nombre* d'objets étant
décrits et est utilisé pour déterminer quelle traduction utiliser et aussi pour définir
le paramètre de substitution ``%count%``.

En se basant sur le nombre donné, le traducteur choisit la bonne forme du pluriel.
En anglais, la plupart des mots ont une forme singulière quand il y a exactement un objet
et un pluriel pour tous les autres nombres (0, 2, 3 ...). Ainsi, si ``count`` vaut
``1``, le traducteur va utiliser la première chaîne de caractères (``There is one apple``)
comme traduction. Sinon, il va utiliser ``There are %count% apples``.

Voici la traduction française :

.. code-block:: text

    'Il y a %count% pomme|Il y a %count% pommes'

Même si la chaîne de caractères se ressemble (elle est constituée de deux sous-chaînes séparées par un
pipe), les règles françaises sont différentes : la première forme (pas de pluriel) est utilisée lorsque
``count`` vaut ``0`` ou ``1``. Ainsi, le traducteur va utiliser automatiquement la première chaîne
(``Il y a %count% pomme``) lorsque ``count`` vaut ``0`` ou ``1``.

Chaque locale a son propre ensemble de règles, certaines ayant jusqu'à six différentes
formes plurielles avec des règles complexes pour déterminer quel nombre correspond à quelle forme du pluriel.
Les règles sont assez simples pour l'anglais et le français, mais pour le russe, vous auriez
voulu un indice pour savoir quelle règle correspond à quelle chaîne de caractères. Pour aider les traducteurs,
vous pouvez éventuellement « tagger » chaque chaîne :

.. code-block:: text

    'one: There is one apple|some: There are %count% apples'

    'none_or_one: Il y a %count% pomme|some: Il y a %count% pommes'

Les tags sont seulement des indices pour les traducteurs et n'affectent pas la logique
utilisée pour déterminer quelle forme de pluriel utiliser. Les tags peuvent être toute 
chaîne descriptive qui se termine par un deux-points (``:``). Les tags n'ont pas besoin d'être les
mêmes dans le message original que dans la traduction.

.. tip:

    Comme les tags sont optionnels, le traducteur ne les utilise pas (il va seulement
    obtenir une chaîne de caractères en fonction de sa position dans la chaîne).

Intervalle Explicite de Pluralisation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La meilleure façon de pluraliser un message est de laisser Symfony2 utiliser sa logique interne
pour choisir quelle chaîne utiliser en se basant sur un nombre donné. Parfois, vous aurez besoin de plus
de contrôle ou vous voudrez une traduction différente pour des cas spécifiques (pour ``0``, ou
lorsque le nombre est négatif, par exemple). Pour de tels cas, vous pouvez utiliser des
intervalles mathématiques explicites :

.. code-block:: text

    '{0} There are no apples|{1} There is one apple|]1,19] There are %count% apples|[20,Inf] There are many apples'

Les intervalles suivent la notation `ISO 31-11`_ . La chaîne de caractères ci-dessus spécifie
quatre intervalles différents : exactement ``0``, exactement ``1``, ``2-19``, et ``20``
et plus.

Vous pouvez également mélanger les règles mathématiques explicites et les règles standards.
Dans ce cas, si le nombre ne correspond pas à un intervalle spécifique, les règles standards
prennent effet après la suppression des règles explicites :

.. code-block:: text

    '{0} There are no apples|[20,Inf] There are many apples|There is one apple|a_few: There are %count% apples'

Par exemple, pour ``1`` pomme (« apple »), la règle standard ``There is one apple`` va
être utilisée. Pour ``2-19`` pommes (« apples »), la seconde règle standard ``There are %count%
apples`` va être sélectionnée.

Une classe :class:`Symfony\\Component\\Translation\\Interval` peut représenter un ensemble fini
de nombres :

.. code-block:: text

    {1,2,3,4}

Ou des nombres entre deux autres nombres :

.. code-block:: text

    [1, +Inf[
    ]-1,2[

Le délimiteur gauche peut être ``[`` (inclusif) ou ``]`` (exclusif). Le delimiteur droit
peut être ``[`` (exclusif) ou ``]`` (inclusif). En sus des nombres, vous pouvez utiliser
``-Inf`` and ``+Inf`` pour l'infini.

.. index::
   single: Traductions; Dans les templates

Traductions dans les Templates
------------------------------

La plupart du temps, les traductions surviennent dans les templates. Symfony2 supporte
nativement les deux types de templates que sont Twig et PHP.

Templates Twig
~~~~~~~~~~~~~~

Symfony2 fournit des balises Twig spécialisées (``trans`` et ``transchoice``) pour
aider à la traduction des *blocs statiques de texte* :

.. code-block:: jinja

    {% trans %}Hello %name%{% endtrans %}

    {% transchoice count %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

La balise ``transchoice`` prend automatiquement la variable ``%count%`` depuis le
contexte actuel et la passe au traducteur. Ce mécanisme fonctionne seulement
lorsque vous utilisez un paramètre de substitution suivi du pattern ``%var%``.

.. tip::

    Si vous avez besoin d'utiliser le caractère pourcentage (``%``) dans une chaîne de caractères,
    échappez-le en le doublant : ``{% trans %}Percent: %percent%%%{% endtrans %}``

Vous pouvez également spécifier le domaine de messages et passer quelques variables supplémentaires :

.. code-block:: jinja

    {% trans with {'%name%': 'Fabien'} from "app" %}Hello %name%{% endtrans %}

    {% trans with {'%name%': 'Fabien'} from "app" into "fr" %}Hello %name%{% endtrans %}

    {% transchoice count with {'%name%': 'Fabien'} from "app" %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

Les filtres ``trans`` et ``transchoice`` peuvent être utilisés pour traduire les
*textes de variable* ainsi que les expressions complexes :

.. code-block:: jinja

    {{ message|trans }}

    {{ message|transchoice(5) }}

    {{ message|trans({'%name%': 'Fabien'}, "app") }}

    {{ message|transchoice(5, {'%name%': 'Fabien'}, 'app') }}

.. tip::

    Utiliser les balises ou filtres de traduction a le même effet, mais avec
    une différence subtile : l'échappement automatique en sortie est appliqué
    uniquement aux variables traduites via un filtre. En d'autres termes, si
    vous avez besoin d'être sûr que votre variable traduite n'est *pas* échappée
    en sortie, vous devez appliquer le filtre brut après le filtre de traduction :
    
    .. code-block:: jinja

            {# le texte traduit entre les balises n'est jamais échappé #}
            {% trans %}
                <h3>foo</h3>
            {% endtrans %}

            {% set message = '<h3>foo</h3>' %}

            {# une variable traduite via un filtre est échappée par défaut #}
            {{ message|trans|raw }}

            {# mais les chaînes de caractères statiques ne sont jamais échappées #}
            {{ '<h3>foo</h3>'|trans }}

.. versionadded:: 2.1

     Vous pouvez maintenant définir un domaine de traduction pour un template
     Twig entier avec une seule balise :
   
     .. code-block:: jinja
  
            {% trans_default_domain "app" %}

    Notez que cela n'affecte que le template courant, pas les templates inclus
    pour éviter les effets de bord.

Templates PHP
~~~~~~~~~~~~~

Le service de traduction est accessible dans les templates PHP à travers l'outil
d'aide ``translator`` :

.. code-block:: html+php

    <?php echo $view['translator']->trans('Symfony2 is great') ?>

    <?php echo $view['translator']->transChoice(
        '{0} There is no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10)
    ) ?>

Forcer la Locale du Traducteur
------------------------------

Lors de la traduction d'un message, Symfony2 utilise la locale de la requête courante
ou la locale ``de secours`` (« fallback locale ») si nécessaire. Vous pouvez également spécifier
manuellement la locale à utiliser pour la traduction :

.. code-block:: php

    $this->get('translator')->trans(
        'Symfony2 is great',
        array(),
        'messages',
        'fr_FR',
    );

    $this->get('translator')->trans(
        '{0} There is no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10),
        'messages',
        'fr_FR',
    );

Traduire le Contenu d'une Base de Données
-----------------------------------------

La traduction du contenu de bases de données devrait être traitée par Doctrine grâce
à l'extension `Translatable Extension`_. Pour plus d'informations, voir la documentation
pour cette bibliothèque.

.. _book-translation-constraint-messages:

Traduire les messages de contraintes
------------------------------------

La meilleure manière de comprendre les contraintes de traduction est de les voir en action.
Pour commencer, supposez que vous avez créé un objet PHP que vous avez besoin d'utiliser
quelque part dans votre application :

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    class Author
    {
        public $name;
    }

Ajoutez des contraintes avec l'une des méthodes supportées. Définissez le texte source à
traduire dans l'option message. Par exemple, pour garantir qu'une propriété $name n'est pas vide, ajoutez le code suivant :

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                name:
                    - NotBlank: { message: "author.name.not_blank" }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank(message = "author.name.not_blank")
             */
            public $name;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="name">
                    <constraint name="NotBlank">
                        <option name="message">author.name.not_blank</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Author
        {
            public $name;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('name', new NotBlank(array(
                    'message' => 'author.name.not_blank'
                )));
            }
        }

Créez un fichier de traduction pour le catalogue ``validators`` pour les messages
de contraintes, typiquement dans le répertoire ``Resources/translations/`` du bundle.
Lisez `Catalogues de Messages`_ pour en savoir plus

.. configuration-block::

    .. code-block:: xml

        <!-- validators.fr.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>author.name.not_blank</source>
                        <target>Saisissez un nom</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // validators.fr.php
        return array(
            'author.name.not_blank' => 'Saisissez un nom.',
        );

    .. code-block:: yaml

        # validators.fr.yml
        author.name.not_blank: Saisissez un nom.

Résumé
------

Avec le composant Traduction de Symfony2, créer une application internationalisée
n'a plus besoin d'être un processus douloureux et se résume simplement à quelques
étapes basiques :

* Extraire les messages dans votre application en entourant chacun d'entre eux par
  la méthode :method:`Symfony\\Component\\Translation\\Translator::trans` ou par
  la méthode :method:`Symfony\\Component\\Translation\\Translator::transChoice`;

* Traduire chaque message dans de multiples locales en créant des fichiers de message
  de traduction. Symfony2 découvre et traite chaque fichier grâce à leur nom qui suit
  une convention spécifique ;
  
* Gérer la locale de l'utilisateur, qui est stockée dans la requête, ou une fois pour
  toutes en session.

.. _`i18n`: http://fr.wikipedia.org/wiki/Internationalisation_et_localisation
.. _`L10n`: http://fr.wikipedia.org/wiki/Localisation_(informatique)
.. _`strtr function`: http://www.php.net/manual/fr/function.strtr.php
.. _`ISO 31-11`: http://en.wikipedia.org/wiki/ISO_31-11
.. _`Translatable Extension`: https://github.com/l3pp4rd/DoctrineExtensions
.. _`ISO3166 Alpha-2`: http://fr.wikipedia.org/wiki/ISO_3166-1#Table_de_codage
.. _`ISO639-1`: http://fr.wikipedia.org/wiki/Liste_des_codes_ISO_639-1