.. index::
   single: Traductions

Traductions
============

Le terme «internationalisation» se réfère au processus d'extraire des chaines 
et autres spécificités locales hors de votre application et de le mettre dans un fichier
où ils peuvent être traduits et convertis basé sur le locale de l'utilisateur (cad
la langue et le pays). Pour le texte, cela signifie envelopper chaque texte avec une fonction
capable de traduire le texte (ou "message") dans la langue de l'utilisateur::

    // le texte va *toujours* s'afficher en anglais
    echo 'Hello World';

    // le texte peut être traduit dans le langage de l'utilisateur final ou par défaut en anglais
    echo $translator->trans('Hello World');

.. note::

    Le terme *locale* se réfère en gros à la langue et au pays de l'utilisateur. Ca
    peut être n'importe quel chaîne que votre application va utiliser ensuite pour gérer
    les traductions et autres différences de format (par ex. format de monnaie). Nous recommandons 
    le code *language* ISO639-1 , un underscore (``_``), ensuite le code *country* ISO3166 
    (par ex. ``fr_FR`` for French/France).

Dans ce chapitre, nous allons apprendre comment préparer une application à soutenir de multiples
locales et ensuite comment créer des traductions pour plusieurs locales. Dans l'ensemble,
le processus a plusieurs étapes communes :
    
1. Activer et configurer le composant  ``Translation`` de Symfony ;

1. Extraire les chaînes (cad "messages") en les enveloppant dans des appels vers le ``Translator`` ;

1. Créer des ressources de traduction pour chaque locale supporté qui traduit
   chaque message dans l'application ;

1. Déterminer, définir et gérer le locale de l'utilisateur dans la session.


.. index::
   single: Traductions ; Configuration

Configuration
-------------

Les traductions sont traités par le ``Translator`` :term:`service` qui utilise le 
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

L'option ``fallback`` définit le locale de secours quand une traduction
n'existe pas dans le locale de l'utilisateur.

.. tip::

    Quand une traduction n'existe pas pour un locale, le translator essaye tout d'abord
    de trouver une traduction pour ce langage (``fr`` si le locale est 
    ``fr_FR`` par exemple). Si cela échoue également, il regarde pour une traduction
     utilisant le locale de secours.

Le locale utilisé en traductions est celui qui est stocké dans la session de l'utilisateur.

.. index::
   single: Traductions; Traduction basique

Traduction Basique
-----------------

La traduction du texte est faite à travers le service ``translator`` 
(:class:`Symfony\\Component\\Translation\\Translator`). Pour traduire un bloc 
de texte (appelé un *message*), utilisez la méthode
:method:`Symfony\\Component\\Translation\\Translator::trans`. Supposons,
par exemple, que nous traduisons un simple message de l'intérieur d'un controlleur :

.. code-block:: php

    public function indexAction()
    {
        $t = $this->get('translator')->trans('Symfony2 is great');

        return new Response($t);
    }

Quand ce code est exécuté, Symfony2 va essayé de traduire ce message
"Symfony2 is great" basé sur le ``locale`` de l'utilisateur. Pour que cela marche,
nous devons dire à Symfony2 comment traduire le message via une "translation
resource", qui est une collection de traductions de message pour un locale donné.
Ce "dictionnaire" de traduction peut être créé en plusieurs formats différents,
XLIFF étant le format recommandé :

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

Maintentn, si le langage du locale de l'utilisateur est le français, (cad ``fr_FR`` or ``fr_BE``),
le message va être traduit en ``J'aime Symfony2``.

Le Process de Traduction
~~~~~~~~~~~~~~~~~~~~~~~

Pour traduire généralement le message, Symfony2 utilise un processus simple :

* Le ``locale`` de l'utilisateur actuel, qui est stocké dans la session, est déterminé ;

* Un catalogue des messages traduits est chargé depuis les ressources de traduction définies
  pour le ``locale`` (cad ``fr_FR``). Les messages du locale de secours sont
  aussi chargés et ajoutés au catalogue s'ils n'existent pas déjà. Le
  résultat final est un large "dictionnaire" de traductions. Voir `Message Catalogues`_
  pour plus de détails ;

* Si le message est dans le catalogue, la traduction est retournée. Si non,
  le translator retourne le message original.
  
Lorsque vous utilisez  la méthode ``trans()``, Symfony2 cherche la chaîne exacte à l'intérieur
du catalogue de messages approprié et la retourne (si elle existe).

.. index::
   single: Traductions; Message placeholders

Message Placeholders
~~~~~~~~~~~~~~~~~~~~

Parfois, un message contenant une variable a besoin d'être traduit :

.. code-block:: php

    public function indexAction($name)
    {
        $t = $this->get('translator')->trans('Hello '.$name);

        return new Response($t);
    }

Cependant, créer une traduction pour cette chaîne est impossible étant donné que le translator
va essayer de trouver le message exact, incluant les portions de la variable
(cad "Hello Ryan" or "Hello Fabien"). Au lieu d'écrire une traduction
pour tous les itérations possibles de la variable ``$name``, nous pouvons remplacer la
variable avec un placeholder (paramètre de substitution) :

.. code-block:: php

    public function indexAction($name)
    {
        $t = $this->get('translator')->trans('Hello %name%', array('%name%' => $name));

        new Response($t);
    }

Symfony2 va maintenant chercher une traduction du message brut (``Hello %name%``)
et *ensuite* remplacer les paramètres de substitution avec leurs valeurs. Créer une traduction
est fait juste comme avant :

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

    Les paramètres de substitution peuvent prendre n'importe quel forme puisque le message en entier est reconstruit
    utilisant le PHP `strtr function`_. Cependant, la notation ``%var%`` est 
    requis lors de la traduction dans des templates Twig, et est globalement une convention
    sensée à suivre.
    
Comme nous l'avons vu, créer une traduction est un processus en deux étapes :

1. Extraire le message qui a besoin d'être traduit en le traitant à travers 
   le ``Translator``.

1. Créer une traduction pour le message dans chaque que vous avez choisi de 
   supporter.

La deuxième étape est fait en créant des catalogues de messages qui définissent les traductions
pour tout nombre de locales différents.

.. index::
   single: Traduction; Catalogues de Message

Catalogues de Message
------------------

Lorsqu'un message est traduit, Symfony2 compile un catalogue de messages pour le
locale de l'utilisateur et regarde dedans pour une traduction du message. Un catalogue
de message est comme un dictionnaire de traductions pour un locale spécifique. 
Par exemple, le catalogue du locale fr_FR `` `` pourrait contenir la traduction
suivante :

    Symfony2 is Great => J'aime Symfony2

C'est la responsabilité du développeur (ou traducteur) d'une application
internationalisée de créer ces traductions. Les traductions sont stockées sur le
système de fichiers et découverts par Symfony, grâce à certaines conventions.
    
.. tip::

    Chaque fois que vous créez une *nouvelle* ressource de traduction (ou d'installer un bundle
    qui comprend une ressource de traduction), assurez-vous de vider votre cache afin
    que Symfony peut découvrir la nouvelle ressource de traduction :

    .. code-block:: bash
    
        php app/console cache:clear

.. index::
   single: Traductions; Emplacements des ressources de traduction

Emplacements des Traductions et Conventions de Nommage
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 cherche les fichiers de messages (cad traductions) à deux endroits :

* Pour les messages trouvés dans un bundle, les fichiers de messages correspondant doit
  se situer dans le répertoire ``Resources/translations/`` du bundle ;

* Pour outrepasser n'importe quel traduction du bundle, placez les fichiers de messages dans le
  répertoire ``app/Resources/translations``.

Le nom du fichier des traductions est aussi important puisque Symfony2 utilise une convention
pour déterminer les détails sur les traductions. Chaque fichier de message doit être nommé
selon le schéma suivant : ``domain.locale.loader`` :

* **domain**: Une façon optionnelle pour organiser les messages dans des groupes (par ex. ``admin``,
  ``navigation`` ou les ``messages`` par défaut) - voir `Using Message Domains`_;

* **locale**: Le locale de la traduction (par ex. ``en_GB``, ``en``, etc);

* **loader**: Comment Symfony2 doit charger et parser le fichier (par ex. ``xliff``,
  ``php`` ou ``yml``).

Le loader peut être le nom de n'importe quel loader enregistré. Par défaut, Symfony
fournit les loaders suivants :

* ``xliff``: XLIFF file;
* ``php``:   PHP file;
* ``yml``:  YAML file.

Le choix de quel loader utiliser dépend de vous et c'est une question de
goût.

.. note::

    Vous pouvez également stocker des traductions dans une base de données, ou tout autre stockage en
    fournissant une classe personnalisée mettant en oeuvre l'interface
    :class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface`.
    Voir :doc:`Custom Translation Loaders </cookbook/translation/custom_loader>`
    ci-dessous pour apprendre comment enregistrer des loaders personnalisés.

.. index::
   single: Traductions; Créer les ressources de traduction

Creer les Traductions
~~~~~~~~~~~~~~~~~~~~~

Chaque fichier est constitué d'une série de paires id-traduction pour le domaine et
locale donné. L'id est l'identifiant de la traduction individuelle, et peut
être le message dans le locale principal (par exemple "Symfony is great") de votre application
ou un identificateur unique (par exemple "symfony2.great" - voir l'encadré ci-dessous):

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

Symfony2 va découvrir ces fichiers et les utiliser lors de la traduction de 
"Symfony2 is great" ou de "symfony2.great" dans un locale de langue française (par ex.
``fr_FR`` or ``fr_BE``).

.. sidebar:: Using Real or Keyword Messages

    Cet exemple illustre les deux philosophies différentes lors de la création
    des messages à traduire :

    .. code-block:: php

        $t = $translator->trans('Symfony2 is great');

        $t = $translator->trans('symfony2.great');

    Dans la première méthode, les messages sont écrits dans la langue du 
    locale par défaut (Anglais dans ce cas). Ce message est ensuite utilisé comme l' "id"
    lors de la création des traductions
    
    Dans la seconde méthode, les messages sont en fait des "mots clés" qui évoque 
    l'idée du message. Le message mot-clé est ensuite utilisée comme "id" pour
    toutes les traductions. Dans ce cas, les traductions doivent être faites pour le 
    locale par défaut (cad pour traduire ``symfony2.great`` à ``Symfony2 is great``).
    
    La deuxième méthode est très pratique car la clé du message n'aura pas besoin d'être modifié
    dans chaque fichier de traduction si nous décidons que le message devrait en fait
    être "Symfony2 is really great" dans le locale par défaut.
    
    Le choix de la méthode à utiliser dépend de vous, mais le format "mot-clé"
    est souvent recommandé.
    
    En outre, les formats de fichiers ``php`` et ``yaml`` prend en charge les ids imbriqués pour
    éviter de vous répéter, si vous utilisez des mots-clés plutôt que du texte réel pour votre
    ids :
    
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

    Les multiples niveaux sont aplaties en uniques paires id / traduction par
    l'ajout d'un point (.) entre chaque niveau, donc les exemples ci-dessus sont
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
   single: Translations; Domaines de Message

Utiliser les Domaines de Message
---------------------

Comme nous l'avons vu, les fichiers de messages sont organisés dans les différents locales 
qu'ils traduisent. Les fichiers de messages peuvent également être organisées davantage dans des "domaines".
Lors de la création des fichiers de message, le domaine est la première partie du nom de fichier.
Le domaine par défaut est ``messages``. Par exemple, supposons que, pour l'organisation,
les traductions ont été divisés en trois domaines différents: ``messages``, ``admin``
et ``navigation``. La traduction française aurait les fichiers de message
suivant :

* ``messages.fr.xliff``
* ``admin.fr.xliff``
* ``navigation.fr.xliff``

Lors de la traduction des chaînes qui ne sont pas dans le domaine par défaut (``messages``),
vous devez spécifier le domaine comme troisième paramètre de ``trans()``:

.. code-block:: php

    $this->get('translator')->trans('Symfony2 is great', array(), 'admin');

Symfony2 va maintenant chercher le message dans le domaine ``admin`` du locale
de l'utilisateur.

.. index::
   single: Traductions; Locale de l'utilisateur

Gérer le locale de l'utilisateur
--------------------------

Le locale de l'utilisateur courant est stocké dans la session et est accessible
via le service ``session`` :

.. code-block:: php

    $locale = $this->get('session')->getLocale();

    $this->get('session')->setLocale('en_US');

.. index::
   single: Traductions; Fallback et locale par défaut

Fallback et Locale par Défaut
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si le locale n'a pas été explicitement défini dans la session le paramètre de 
configuration ``fallback_locale`` va être utilisé par le ``Translator``. Le paramètre
est par défaut à ``en`` (voir `Configuration`_).

Alternativement, vous pouvez garantir que le locale est défini dans la session de l'utilisateur
en définissant un ``default_locale`` dans le service de session :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session: { default_locale: en }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session default-locale="en" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array('default_locale' => 'en'),
        ));

Le Locale et l'URL
~~~~~~~~~~~~~~~~~~~~~~

Puisque le locale de l'utilisateur est stocké dans la session, il peut être tentant
d'utiliser la même URL pour afficher une ressource dans de nombreuses langues différentes basé
sur le locale de l'utilisateur. Par exemple, ``http://www.example.com/contact`` pourrait
afficher le contenu en anglais pour un utilisateur et en français pour un autre utilisateur. Malheureusement,
cela viole une règle fondamentale de l'Internet : qu'une URL particulière retourne
la même ressource indépendamment de l'utilisateur. Pour mieux "muddy" le problème, quel
version du contenu serait indexé par les moteurs de recherche ?

Une meilleure politique est d'inclure la localisation dans l'URL. Ceci est entièrement prise en charge
par le système de routage en utilisant le paramètre spécial ``_locale`` :

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

Lorsque vous utilisez le paramètre spécial `_locale` dans une route, le locale correspondant
sera *automatiquement défini sur la session de l'utilisateur*. En d'autres termes, si un utilisateur
visite l'URI ``/fr/contact``, le locale ``fr`` sera automatiquement défini
comme le locale pour la session de l'utilisateur.

Vous pouvez maintenant utiliser le locale de l'utilisateur pour créer des routes
à d'autres pages traduites dans votre application.

.. index::
   single: Traductions; Pluralisation

Pluralisation
-------------

La pluralisation des messages est un sujet difficile car les règles peuvent être assez complexes. 
Par exemple, voici la représentation mathématique des règles de la pluralisation 
russe::

    (($number % 10 == 1) && ($number % 100 != 11)) ? 0 : ((($number % 10 >= 2) && ($number % 10 <= 4) && (($number % 100 < 10) || ($number % 100 >= 20))) ? 1 : 2);

Comme vous pouvez voir, en russe, vous pouvez avoir trois formes de pluriel différent, chacun
donnant un index de 0, 1 or 2. Pour chaque forme, le pluriel est différent, et
ainsi la traduction est également différent.

Quand une traduction a des formes différentes dûes à la pluralisation, vous pouvez fournir
toutes les formes comme une chaîne séparé par un pipe (``|``)::

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
décrits et est utilisé pour déterminer quel traduction utiliser et aussi pour définir
le paramètre de substitution ``%count%``.

Basé sur le nombre donné, le translator choisit la bonne forme du pluriel. 
En anglais, la plupart des mots ont une forme singulière quand il y a exactement un objet
et un pluriel pour tous les autres nombres (0, 2, 3 ...). Ainsi, si ``count`` is
``1``, le translator va utiliser la première chaîne (``There is one apple``)
comme la traduction. Sinon, il va utiliser ``There are %count% apples``.

Voici la traduction française::

    'Il y a %count% pomme|Il y a %count% pommes'

Même si la chaîne se ressemble (il est constitué de deux sous-chaînes séparées par un
pipe), les règles françaises sont différentes : la première forme (pas de pluriel) est utilisée lorsque
``count`` is ``0`` or ``1``. Ainsi, le translator va utiliser automatiquement la
première chaîne (``Il y a %count% pomme``) lorsque ``count`` est ``0`` ou ``1``.

Chaque locale a son propre ensemble de règles, certains ayant jusqu'à six différentes
formes plurielles avec des règles complexes pour déterminer quel nombre correspond à quelle forme du pluriel.
Les règles sont assez simples pour l'anglais et le français, mais pour le russe, vous auriez
voulu un indice pour savoir quelle règle correspond à quelle chaîne. Pour aider les traducteurs,
vous pouvez éventuellement "tag" chaque chaîne ::

    'one: There is one apple|some: There are %count% apples'

    'none_or_one: Il y a %count% pomme|some: Il y a %count% pommes'

Les tags sont seulement des indices pour les traducteurs et n'affectent pas la logique
utilisés pour déterminer quelle forme plurielle utiliser. Les tags peuvent être toute 
chaîne descriptive qui se termine par un deux-points (``:``). Les tags aussi n'ont pas besoin d'être le
même dans le message original comme dans la traduction.

.. tip:

    Comme les tags sont optionnels, le translator ne les utilise pas (le translator va
    seulement obtenir une chaîne en fonction de sa position dans la chaîne).

Explicit Interval Pluralization
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La meilleure façon de pluraliser un message est de laisser Symfony2 utiliser la logique interne
de choisir quelle chaînes utiliser basé sur un nombre donné. Parfois, vous aurez
besoin de plus de contrôle ou si vous voulez une traduction différente pour des cas spécifiques (pour
``0``, ou lorsque le nombre est négatif, par exemple). Pour de tels cas, vous pouvez
utiliser des intervalles mathématiques explicites::

    '{0} There is no apples|{1} There is one apple|]1,19] There are %count% apples|[20,Inf] There are many apples'

Les intervalles suivent la notation `ISO 31-11`_ . la chaîne ci-dessus spécifie
quatre différentes intervalles : exactement ``0``, exactement ``1``, ``2-19``, et ``20``
et plus.

Vous pouvez également mélanger les règles mathématiques explicites et des règles standard. Dans ce cas, si
le nombre ne correspond pas à un intervalle spécifique, les règles standard prennent 
effet après la suppression des règles explicites::

    '{0} There is no apples|[20,Inf] There are many apples|There is one apple|a_few: There are %count% apples'

Par exemple, pour ``1`` apple, la règle standard ``There is one apple`` va
être utilisée. Pour ``2-19`` apples, la seconde règle standard ``There are %count%
apples`` va être sélectionnée.

Une :class:`Symfony\\Component\\Translation\\Interval` peut représenter un ensemble fini
de nombres::

    {1,2,3,4}

Ou des nombres entre deux autres nombres::

    [1, +Inf[
    ]-1,2[

Le délimiteur gauche peut être ``[`` (inclusive) ou ``]`` (exclusive). Le 
delimiteur droit peut être ``[`` (exclusive) ou ``]`` (inclusive). A côté des nombres, vous
pouvez utiliser ``-Inf`` and ``+Inf`` pour l'infini.

.. index::
   single: Traductions; Dans les templates

Traductions dans les Templates
-------------------------

La plupart du temps, les traductions surviennent dans des templates. Symfony2 fournit un 
support natif pour les deux templates Twig et PHP.

Twig Templates
~~~~~~~~~~~~~~

Symfony2 fournit des tags Twig spécialisés (``trans`` and ``transchoice``) pour
aider avec les traductions de message des *blocs statiques de texte*:

.. code-block:: jinja

    {% trans %}Hello %name%{% endtrans %}

    {% transchoice count %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

Le tag ``transchoice``  obtient automatiquement la variable ``%count%`` du
contexte actuel et le passe au translator. Ce mécanisme marche seulement
lorsque vous utilisez un paramètre de substitution suivi du pattern ``%var%``.

.. tip::

    Si vous avez besoin d'utiliser le caractère pourcentage (``%``) dans une chaîne, échapper le en
    le doublant : ``{% trans %}Percent: %percent%%%{% endtrans %}``

Vous pouvez également spécifier le domaine de message et passer quelques variables supplémentaires :

.. code-block:: jinja

    {% trans with {'%name%': 'Fabien'} from "app" %}Hello %name%{% endtrans %}

    {% transchoice count with {'%name%': 'Fabien'} from "app" %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

Les filtres ``trans`` and ``transchoice`` peuvent être utilisés pour traduire les 
*textes de variable* et expressions complexes :

.. code-block:: jinja

    {{ message | trans }}

    {{ message | transchoice(5) }}

    {{ message | trans({'%name%': 'Fabien'}, "app") }}

    {{ message | transchoice(5, {'%name%': 'Fabien'}, 'app') }}

.. tip::

    Utiliser les tags ou filtres de traduction ont le même effet, mais avec
    une différence subtile : l'échappement automatique en sortie est appliquée uniquement aux
    variables traduites en utilisant un filtre. En d'autres termes, si vous avez besoin 
    d'être sûr que votre variable traduite n'est *pas* échappé en sortie, vous devez
    appliquer le filtre brut après le filtre de traduction :
    
    .. code-block:: jinja

            {# text translated between tags is never escaped #}
            {% trans %}
                <h3>foo</h3>
            {% endtrans %}

            {% set message = '<h3>foo</h3>' %}

            {# a variable translated via a filter is escaped by default #}
            {{ message | trans | raw }}

            {# but static strings are never escaped #}
            {{ '<h3>foo</h3>' | trans }}

PHP Templates
~~~~~~~~~~~~~

Le service translator est accessible dans les templates PHP à travers le 
helper ``translator`` :

.. code-block:: html+php

    <?php echo $view['translator']->trans('Symfony2 is great') ?>

    <?php echo $view['translator']->transChoice(
        '{0} There is no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10)
    ) ?>

Forcer le Translator Locale
-----------------------------

Lors de la traduction d'un message, Symfony2 utilise le locale de la session de l'utilisateur
ou le ``fallback`` locale si nécessaire. Vous pouvez également spécifier manuellement le
locale à utiliser pour la traduction :

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
----------------------------

La traduction du contenu de bases de données doivent être traitées par Doctrine par le biais
de `Translatable Extension`_. Pour plus d'informations, voir la documentation
pour cette bibliothèque.

Sommaire
-------
Avec le composant de Traduction Symfony2, créer une application internationalisée
n'est plus maintenant un processus douloureux et se résume à quelques
étapes basiques :

* Extraire les messages dans votre application en enveloppant chaque message soit par 
  la :method:`Symfony\\Component\\Translation\\Translator::trans` ou
  la :method:`Symfony\\Component\\Translation\\Translator::transChoice`;

* Traduire chaque message dans des locales multiples en créant des fichiers de message
  de traduction. Symfony2 découvre et traite chaque fichier parce que son nom suit
  une convention spécifique ;
  
* Gérer le locale de l'utilisateur, qui est stocké dans la session.

.. _`strtr function`: http://www.php.net/manual/en/function.strtr.php
.. _`ISO 31-11`: http://en.wikipedia.org/wiki/Interval_%28mathematics%29#The_ISO_notation
.. _`Translatable Extension`: https://github.com/l3pp4rd/DoctrineExtensions
