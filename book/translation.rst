.. index::
   single: Traductions

Traductions
============

Le terme �internationalisation� se r�f�re au processus d'extraire des chaines 
et autres sp�cificit�s locales hors de votre application et de le mettre dans un fichier
o� ils peuvent �tre traduits et convertis bas� sur le locale de l'utilisateur (cad
la langue et le pays). Pour le texte, cela signifie envelopper chaque texte avec une fonction
capable de traduire le texte (ou "message") dans la langue de l'utilisateur::

    // le texte va *toujours* s'afficher en anglais
    echo 'Hello World';

    // le texte peut �tre traduit dans le langage de l'utilisateur final ou par d�faut en anglais
    echo $translator->trans('Hello World');

.. note::

    Le terme *locale* se r�f�re en gros � la langue et au pays de l'utilisateur. Ca
    peut �tre n'importe quel cha�ne que votre application va utiliser ensuite pour g�rer
    les traductions et autres diff�rences de format (par ex. format de monnaie). Nous recommandons 
    le code *language* ISO639-1 , un underscore (``_``), ensuite le code *country* ISO3166 
    (par ex. ``fr_FR`` for French/France).

Dans ce chapitre, nous allons apprendre comment pr�parer une application � soutenir de multiples
locales et ensuite comment cr�er des traductions pour plusieurs locales. Dans l'ensemble,
le processus a plusieurs �tapes communes :
    
1. Activer et configurer le composant  ``Translation`` de Symfony ;

1. Extraire les cha�nes (cad "messages") en les enveloppant dans des appels vers le ``Translator`` ;

1. Cr�er des ressources de traduction pour chaque locale support� qui traduit
   chaque message dans l'application ;

1. D�terminer, d�finir et g�rer le locale de l'utilisateur dans la session.


.. index::
   single: Traductions ; Configuration

Configuration
-------------

Les traductions sont trait�s par le ``Translator`` :term:`service` qui utilise le 
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

L'option ``fallback`` d�finit le locale de secours quand une traduction
n'existe pas dans le locale de l'utilisateur.

.. tip::

    Quand une traduction n'existe pas pour un locale, le translator essaye tout d'abord
    de trouver une traduction pour ce langage (``fr`` si le locale est 
    ``fr_FR`` par exemple). Si cela �choue �galement, il regarde pour une traduction
     utilisant le locale de secours.

Le locale utilis� en traductions est celui qui est stock� dans la session de l'utilisateur.

.. index::
   single: Traductions; Traduction basique

Traduction Basique
-----------------

La traduction du texte est faite � travers le service ``translator`` 
(:class:`Symfony\\Component\\Translation\\Translator`). Pour traduire un bloc 
de texte (appel� un *message*), utilisez la m�thode
:method:`Symfony\\Component\\Translation\\Translator::trans`. Supposons,
par exemple, que nous traduisons un simple message de l'int�rieur d'un controlleur :

.. code-block:: php

    public function indexAction()
    {
        $t = $this->get('translator')->trans('Symfony2 is great');

        return new Response($t);
    }

Quand ce code est ex�cut�, Symfony2 va essay� de traduire ce message
"Symfony2 is great" bas� sur le ``locale`` de l'utilisateur. Pour que cela marche,
nous devons dire � Symfony2 comment traduire le message via une "translation
resource", qui est une collection de traductions de message pour un locale donn�.
Ce "dictionnaire" de traduction peut �tre cr�� en plusieurs formats diff�rents,
XLIFF �tant le format recommand� :

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

Maintentn, si le langage du locale de l'utilisateur est le fran�ais, (cad ``fr_FR`` or ``fr_BE``),
le message va �tre traduit en ``J'aime Symfony2``.

Le Process de Traduction
~~~~~~~~~~~~~~~~~~~~~~~

Pour traduire g�n�ralement le message, Symfony2 utilise un processus simple :

* Le ``locale`` de l'utilisateur actuel, qui est stock� dans la session, est d�termin� ;

* Un catalogue des messages traduits est charg� depuis les ressources de traduction d�finies
  pour le ``locale`` (cad ``fr_FR``). Les messages du locale de secours sont
  aussi charg�s et ajout�s au catalogue s'ils n'existent pas d�j�. Le
  r�sultat final est un large "dictionnaire" de traductions. Voir `Message Catalogues`_
  pour plus de d�tails ;

* Si le message est dans le catalogue, la traduction est retourn�e. Si non,
  le translator retourne le message original.
  
Lorsque vous utilisez  la m�thode ``trans()``, Symfony2 cherche la cha�ne exacte � l'int�rieur
du catalogue de messages appropri� et la retourne (si elle existe).

.. index::
   single: Traductions; Message placeholders

Message Placeholders
~~~~~~~~~~~~~~~~~~~~

Parfois, un message contenant une variable a besoin d'�tre traduit :

.. code-block:: php

    public function indexAction($name)
    {
        $t = $this->get('translator')->trans('Hello '.$name);

        return new Response($t);
    }

Cependant, cr�er une traduction pour cette cha�ne est impossible �tant donn� que le translator
va essayer de trouver le message exact, incluant les portions de la variable
(cad "Hello Ryan" or "Hello Fabien"). Au lieu d'�crire une traduction
pour tous les it�rations possibles de la variable ``$name``, nous pouvons remplacer la
variable avec un placeholder (param�tre de substitution) :

.. code-block:: php

    public function indexAction($name)
    {
        $t = $this->get('translator')->trans('Hello %name%', array('%name%' => $name));

        new Response($t);
    }

Symfony2 va maintenant chercher une traduction du message brut (``Hello %name%``)
et *ensuite* remplacer les param�tres de substitution avec leurs valeurs. Cr�er une traduction
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

    Les param�tres de substitution peuvent prendre n'importe quel forme puisque le message en entier est reconstruit
    utilisant le PHP `strtr function`_. Cependant, la notation ``%var%`` est 
    requis lors de la traduction dans des templates Twig, et est globalement une convention
    sens�e � suivre.
    
Comme nous l'avons vu, cr�er une traduction est un processus en deux �tapes :

1. Extraire le message qui a besoin d'�tre traduit en le traitant � travers 
   le ``Translator``.

1. Cr�er une traduction pour le message dans chaque que vous avez choisi de 
   supporter.

La deuxi�me �tape est fait en cr�ant des catalogues de messages qui d�finissent les traductions
pour tout nombre de locales diff�rents.

.. index::
   single: Traduction; Catalogues de Message

Catalogues de Message
------------------

Lorsqu'un message est traduit, Symfony2 compile un catalogue de messages pour le
locale de l'utilisateur et regarde dedans pour une traduction du message. Un catalogue
de message est comme un dictionnaire de traductions pour un locale sp�cifique. 
Par exemple, le catalogue du locale fr_FR `` `` pourrait contenir la traduction
suivante :

    Symfony2 is Great => J'aime Symfony2

C'est la responsabilit� du d�veloppeur (ou traducteur) d'une application
internationalis�e de cr�er ces traductions. Les traductions sont stock�es sur le
syst�me de fichiers et d�couverts par Symfony, gr�ce � certaines conventions.
    
.. tip::

    Chaque fois que vous cr�ez une *nouvelle* ressource de traduction (ou d'installer un bundle
    qui comprend une ressource de traduction), assurez-vous de vider votre cache afin
    que Symfony peut d�couvrir la nouvelle ressource de traduction :

    .. code-block:: bash
    
        php app/console cache:clear

.. index::
   single: Traductions; Emplacements des ressources de traduction

Emplacements des Traductions et Conventions de Nommage
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 cherche les fichiers de messages (cad traductions) � deux endroits :

* Pour les messages trouv�s dans un bundle, les fichiers de messages correspondant doit
  se situer dans le r�pertoire ``Resources/translations/`` du bundle ;

* Pour outrepasser n'importe quel traduction du bundle, placez les fichiers de messages dans le
  r�pertoire ``app/Resources/translations``.

Le nom du fichier des traductions est aussi important puisque Symfony2 utilise une convention
pour d�terminer les d�tails sur les traductions. Chaque fichier de message doit �tre nomm�
selon le sch�ma suivant : ``domain.locale.loader`` :

* **domain**: Une fa�on optionnelle pour organiser les messages dans des groupes (par ex. ``admin``,
  ``navigation`` ou les ``messages`` par d�faut) - voir `Using Message Domains`_;

* **locale**: Le locale de la traduction (par ex. ``en_GB``, ``en``, etc);

* **loader**: Comment Symfony2 doit charger et parser le fichier (par ex. ``xliff``,
  ``php`` ou ``yml``).

Le loader peut �tre le nom de n'importe quel loader enregistr�. Par d�faut, Symfony
fournit les loaders suivants :

* ``xliff``: XLIFF file;
* ``php``:   PHP file;
* ``yml``:  YAML file.

Le choix de quel loader utiliser d�pend de vous et c'est une question de
go�t.

.. note::

    Vous pouvez �galement stocker des traductions dans une base de donn�es, ou tout autre stockage en
    fournissant une classe personnalis�e mettant en oeuvre l'interface
    :class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface`.
    Voir :doc:`Custom Translation Loaders </cookbook/translation/custom_loader>`
    ci-dessous pour apprendre comment enregistrer des loaders personnalis�s.

.. index::
   single: Traductions; Cr�er les ressources de traduction

Creer les Traductions
~~~~~~~~~~~~~~~~~~~~~

Chaque fichier est constitu� d'une s�rie de paires id-traduction pour le domaine et
locale donn�. L'id est l'identifiant de la traduction individuelle, et peut
�tre le message dans le locale principal (par exemple "Symfony is great") de votre application
ou un identificateur unique (par exemple "symfony2.great" - voir l'encadr� ci-dessous):

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

Symfony2 va d�couvrir ces fichiers et les utiliser lors de la traduction de 
"Symfony2 is great" ou de "symfony2.great" dans un locale de langue fran�aise (par ex.
``fr_FR`` or ``fr_BE``).

.. sidebar:: Using Real or Keyword Messages

    Cet exemple illustre les deux philosophies diff�rentes lors de la cr�ation
    des messages � traduire :

    .. code-block:: php

        $t = $translator->trans('Symfony2 is great');

        $t = $translator->trans('symfony2.great');

    Dans la premi�re m�thode, les messages sont �crits dans la langue du 
    locale par d�faut (Anglais dans ce cas). Ce message est ensuite utilis� comme l' "id"
    lors de la cr�ation des traductions
    
    Dans la seconde m�thode, les messages sont en fait des "mots cl�s" qui �voque 
    l'id�e du message. Le message mot-cl� est ensuite utilis�e comme "id" pour
    toutes les traductions. Dans ce cas, les traductions doivent �tre faites pour le 
    locale par d�faut (cad pour traduire ``symfony2.great`` � ``Symfony2 is great``).
    
    La deuxi�me m�thode est tr�s pratique car la cl� du message n'aura pas besoin d'�tre modifi�
    dans chaque fichier de traduction si nous d�cidons que le message devrait en fait
    �tre "Symfony2 is really great" dans le locale par d�faut.
    
    Le choix de la m�thode � utiliser d�pend de vous, mais le format "mot-cl�"
    est souvent recommand�.
    
    En outre, les formats de fichiers ``php`` et ``yaml`` prend en charge les ids imbriqu�s pour
    �viter de vous r�p�ter, si vous utilisez des mots-cl�s plut�t que du texte r�el pour votre
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
    �quivalents � ce qui suit :
    
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

Comme nous l'avons vu, les fichiers de messages sont organis�s dans les diff�rents locales 
qu'ils traduisent. Les fichiers de messages peuvent �galement �tre organis�es davantage dans des "domaines".
Lors de la cr�ation des fichiers de message, le domaine est la premi�re partie du nom de fichier.
Le domaine par d�faut est ``messages``. Par exemple, supposons que, pour l'organisation,
les traductions ont �t� divis�s en trois domaines diff�rents: ``messages``, ``admin``
et ``navigation``. La traduction fran�aise aurait les fichiers de message
suivant :

* ``messages.fr.xliff``
* ``admin.fr.xliff``
* ``navigation.fr.xliff``

Lors de la traduction des cha�nes qui ne sont pas dans le domaine par d�faut (``messages``),
vous devez sp�cifier le domaine comme troisi�me param�tre de ``trans()``:

.. code-block:: php

    $this->get('translator')->trans('Symfony2 is great', array(), 'admin');

Symfony2 va maintenant chercher le message dans le domaine ``admin`` du locale
de l'utilisateur.

.. index::
   single: Traductions; Locale de l'utilisateur

G�rer le locale de l'utilisateur
--------------------------

Le locale de l'utilisateur courant est stock� dans la session et est accessible
via le service ``session`` :

.. code-block:: php

    $locale = $this->get('session')->getLocale();

    $this->get('session')->setLocale('en_US');

.. index::
   single: Traductions; Fallback et locale par d�faut

Fallback et Locale par D�faut
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si le locale n'a pas �t� explicitement d�fini dans la session le param�tre de 
configuration ``fallback_locale`` va �tre utilis� par le ``Translator``. Le param�tre
est par d�faut � ``en`` (voir `Configuration`_).

Alternativement, vous pouvez garantir que le locale est d�fini dans la session de l'utilisateur
en d�finissant un ``default_locale`` dans le service de session :

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

Puisque le locale de l'utilisateur est stock� dans la session, il peut �tre tentant
d'utiliser la m�me URL pour afficher une ressource dans de nombreuses langues diff�rentes bas�
sur le locale de l'utilisateur. Par exemple, ``http://www.example.com/contact`` pourrait
afficher le contenu en anglais pour un utilisateur et en fran�ais pour un autre utilisateur. Malheureusement,
cela viole une r�gle fondamentale de l'Internet : qu'une URL particuli�re retourne
la m�me ressource ind�pendamment de l'utilisateur. Pour mieux "muddy" le probl�me, quel
version du contenu serait index� par les moteurs de recherche ?

Une meilleure politique est d'inclure la localisation dans l'URL. Ceci est enti�rement prise en charge
par le syst�me de routage en utilisant le param�tre sp�cial ``_locale`` :

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

Lorsque vous utilisez le param�tre sp�cial `_locale` dans une route, le locale correspondant
sera *automatiquement d�fini sur la session de l'utilisateur*. En d'autres termes, si un utilisateur
visite l'URI ``/fr/contact``, le locale ``fr`` sera automatiquement d�fini
comme le locale pour la session de l'utilisateur.

Vous pouvez maintenant utiliser le locale de l'utilisateur pour cr�er des routes
� d'autres pages traduites dans votre application.

.. index::
   single: Traductions; Pluralisation

Pluralisation
-------------

La pluralisation des messages est un sujet difficile car les r�gles peuvent �tre assez complexes. 
Par exemple, voici la repr�sentation math�matique des r�gles de la pluralisation 
russe::

    (($number % 10 == 1) && ($number % 100 != 11)) ? 0 : ((($number % 10 >= 2) && ($number % 10 <= 4) && (($number % 100 < 10) || ($number % 100 >= 20))) ? 1 : 2);

Comme vous pouvez voir, en russe, vous pouvez avoir trois formes de pluriel diff�rent, chacun
donnant un index de 0, 1 or 2. Pour chaque forme, le pluriel est diff�rent, et
ainsi la traduction est �galement diff�rent.

Quand une traduction a des formes diff�rentes d�es � la pluralisation, vous pouvez fournir
toutes les formes comme une cha�ne s�par� par un pipe (``|``)::

    'There is one apple|There are %count% apples'

Pour traduire des messages pluralis�s, utilisez la m�thode 
:method:`Symfony\\Component\\Translation\\Translator::transChoice` :

.. code-block:: php

    $t = $this->get('translator')->transChoice(
        'There is one apple|There are %count% apples',
        10,
        array('%count%' => 10)
    );

Le second param�tre (``10`` dans cet exemple), est le *nombre* d'objets �tant
d�crits et est utilis� pour d�terminer quel traduction utiliser et aussi pour d�finir
le param�tre de substitution ``%count%``.

Bas� sur le nombre donn�, le translator choisit la bonne forme du pluriel. 
En anglais, la plupart des mots ont une forme singuli�re quand il y a exactement un objet
et un pluriel pour tous les autres nombres (0, 2, 3 ...). Ainsi, si ``count`` is
``1``, le translator va utiliser la premi�re cha�ne (``There is one apple``)
comme la traduction. Sinon, il va utiliser ``There are %count% apples``.

Voici la traduction fran�aise::

    'Il y a %count% pomme|Il y a %count% pommes'

M�me si la cha�ne se ressemble (il est constitu� de deux sous-cha�nes s�par�es par un
pipe), les r�gles fran�aises sont diff�rentes : la premi�re forme (pas de pluriel) est utilis�e lorsque
``count`` is ``0`` or ``1``. Ainsi, le translator va utiliser automatiquement la
premi�re cha�ne (``Il y a %count% pomme``) lorsque ``count`` est ``0`` ou ``1``.

Chaque locale a son propre ensemble de r�gles, certains ayant jusqu'� six diff�rentes
formes plurielles avec des r�gles complexes pour d�terminer quel nombre correspond � quelle forme du pluriel.
Les r�gles sont assez simples pour l'anglais et le fran�ais, mais pour le russe, vous auriez
voulu un indice pour savoir quelle r�gle correspond � quelle cha�ne. Pour aider les traducteurs,
vous pouvez �ventuellement "tag" chaque cha�ne ::

    'one: There is one apple|some: There are %count% apples'

    'none_or_one: Il y a %count% pomme|some: Il y a %count% pommes'

Les tags sont seulement des indices pour les traducteurs et n'affectent pas la logique
utilis�s pour d�terminer quelle forme plurielle utiliser. Les tags peuvent �tre toute 
cha�ne descriptive qui se termine par un deux-points (``:``). Les tags aussi n'ont pas besoin d'�tre le
m�me dans le message original comme dans la traduction.

.. tip:

    Comme les tags sont optionnels, le translator ne les utilise pas (le translator va
    seulement obtenir une cha�ne en fonction de sa position dans la cha�ne).

Explicit Interval Pluralization
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La meilleure fa�on de pluraliser un message est de laisser Symfony2 utiliser la logique interne
de choisir quelle cha�nes utiliser bas� sur un nombre donn�. Parfois, vous aurez
besoin de plus de contr�le ou si vous voulez une traduction diff�rente pour des cas sp�cifiques (pour
``0``, ou lorsque le nombre est n�gatif, par exemple). Pour de tels cas, vous pouvez
utiliser des intervalles math�matiques explicites::

    '{0} There is no apples|{1} There is one apple|]1,19] There are %count% apples|[20,Inf] There are many apples'

Les intervalles suivent la notation `ISO 31-11`_ . la cha�ne ci-dessus sp�cifie
quatre diff�rentes intervalles : exactement ``0``, exactement ``1``, ``2-19``, et ``20``
et plus.

Vous pouvez �galement m�langer les r�gles math�matiques explicites et des r�gles standard. Dans ce cas, si
le nombre ne correspond pas � un intervalle sp�cifique, les r�gles standard prennent 
effet apr�s la suppression des r�gles explicites::

    '{0} There is no apples|[20,Inf] There are many apples|There is one apple|a_few: There are %count% apples'

Par exemple, pour ``1`` apple, la r�gle standard ``There is one apple`` va
�tre utilis�e. Pour ``2-19`` apples, la seconde r�gle standard ``There are %count%
apples`` va �tre s�lectionn�e.

Une :class:`Symfony\\Component\\Translation\\Interval` peut repr�senter un ensemble fini
de nombres::

    {1,2,3,4}

Ou des nombres entre deux autres nombres::

    [1, +Inf[
    ]-1,2[

Le d�limiteur gauche peut �tre ``[`` (inclusive) ou ``]`` (exclusive). Le 
delimiteur droit peut �tre ``[`` (exclusive) ou ``]`` (inclusive). A c�t� des nombres, vous
pouvez utiliser ``-Inf`` and ``+Inf`` pour l'infini.

.. index::
   single: Traductions; Dans les templates

Traductions dans les Templates
-------------------------

La plupart du temps, les traductions surviennent dans des templates. Symfony2 fournit un 
support natif pour les deux templates Twig et PHP.

Twig Templates
~~~~~~~~~~~~~~

Symfony2 fournit des tags Twig sp�cialis�s (``trans`` and ``transchoice``) pour
aider avec les traductions de message des *blocs statiques de texte*:

.. code-block:: jinja

    {% trans %}Hello %name%{% endtrans %}

    {% transchoice count %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

Le tag ``transchoice``  obtient automatiquement la variable ``%count%`` du
contexte actuel et le passe au translator. Ce m�canisme marche seulement
lorsque vous utilisez un param�tre de substitution suivi du pattern ``%var%``.

.. tip::

    Si vous avez besoin d'utiliser le caract�re pourcentage (``%``) dans une cha�ne, �chapper le en
    le doublant : ``{% trans %}Percent: %percent%%%{% endtrans %}``

Vous pouvez �galement sp�cifier le domaine de message et passer quelques variables suppl�mentaires :

.. code-block:: jinja

    {% trans with {'%name%': 'Fabien'} from "app" %}Hello %name%{% endtrans %}

    {% transchoice count with {'%name%': 'Fabien'} from "app" %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

Les filtres ``trans`` and ``transchoice`` peuvent �tre utilis�s pour traduire les 
*textes de variable* et expressions complexes :

.. code-block:: jinja

    {{ message | trans }}

    {{ message | transchoice(5) }}

    {{ message | trans({'%name%': 'Fabien'}, "app") }}

    {{ message | transchoice(5, {'%name%': 'Fabien'}, 'app') }}

.. tip::

    Utiliser les tags ou filtres de traduction ont le m�me effet, mais avec
    une diff�rence subtile : l'�chappement automatique en sortie est appliqu�e uniquement aux
    variables traduites en utilisant un filtre. En d'autres termes, si vous avez besoin 
    d'�tre s�r que votre variable traduite n'est *pas* �chapp� en sortie, vous devez
    appliquer le filtre brut apr�s le filtre de traduction :
    
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

Le service translator est accessible dans les templates PHP � travers le 
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
ou le ``fallback`` locale si n�cessaire. Vous pouvez �galement sp�cifier manuellement le
locale � utiliser pour la traduction :

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

Traduire le Contenu d'une Base de Donn�es
----------------------------

La traduction du contenu de bases de donn�es doivent �tre trait�es par Doctrine par le biais
de `Translatable Extension`_. Pour plus d'informations, voir la documentation
pour cette biblioth�que.

Sommaire
-------
Avec le composant de Traduction Symfony2, cr�er une application internationalis�e
n'est plus maintenant un processus douloureux et se r�sume � quelques
�tapes basiques :

* Extraire les messages dans votre application en enveloppant chaque message soit par 
  la :method:`Symfony\\Component\\Translation\\Translator::trans` ou
  la :method:`Symfony\\Component\\Translation\\Translator::transChoice`;

* Traduire chaque message dans des locales multiples en cr�ant des fichiers de message
  de traduction. Symfony2 d�couvre et traite chaque fichier parce que son nom suit
  une convention sp�cifique ;
  
* G�rer le locale de l'utilisateur, qui est stock� dans la session.

.. _`strtr function`: http://www.php.net/manual/en/function.strtr.php
.. _`ISO 31-11`: http://en.wikipedia.org/wiki/Interval_%28mathematics%29#The_ISO_notation
.. _`Translatable Extension`: https://github.com/l3pp4rd/DoctrineExtensions
