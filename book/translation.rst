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
	les traudctions et autres différences de format (par ex. format de monnaie). Nous recommandons 
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
   single: Translations; Message placeholders

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
variable avec un "placeholder":

.. code-block:: php

    public function indexAction($name)
    {
        $t = $this->get('translator')->trans('Hello %name%', array('%name%' => $name));

        new Response($t);
    }

Symfony2 va maintenant chercher une traduction du message brut (``Hello %name%``)
et *ensuite* remplacer les placeholders avec leurs valeurs. Créer une traduction
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

    The placeholders can take on any form as the full message is reconstructed
    using the PHP `strtr function`_. However, the ``%var%`` notation is
    required when translating in Twig templates, and is overall a sensible
    convention to follow.

As we've seen, creating a translation is a two-step process:

1. Abstract the message that needs to be translated by processing it through
   the ``Translator``.

1. Create a translation for the message in each locale that you choose to
   support.

The second step is done by creating message catalogues that define the translations
for any number of different locales.

.. index::
   single: Translations; Message catalogues

Message Catalogues
------------------

When a message is translated, Symfony2 compiles a message catalogue for the
user's locale and looks in it for a translation of the message. A message
catalogue is like a dictionary of translations for a specific locale. For
example, the catalogue for the ``fr_FR`` locale might contain the following
translation:

    Symfony2 is Great => J'aime Symfony2

It's the responsibility of the developer (or translator) of an internationalized
application to create these translations. Translations are stored on the
filesystem and discovered by Symfony, thanks to some conventions.

.. tip::

    Each time you create a *new* translation resource (or install a bundle
    that includes a translation resource), be sure to clear your cache so
    that Symfony can discover the new translation resource:
    
    .. code-block:: bash
    
        php app/console cache:clear

.. index::
   single: Translations; Translation resource locations

Translation Locations and Naming Conventions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 looks for message files (i.e. translations) in two locations:

* For messages found in a bundle, the corresponding message files should
  live in the ``Resources/translations/`` directory of the bundle;

* To override any bundle translations, place message files in the
  ``app/Resources/translations`` directory.

The filename of the translations is also important as Symfony2 uses a convention
to determine details about the translations. Each message file must be named
according to the following pattern: ``domain.locale.loader``:

* **domain**: An optional way to organize messages into groups (e.g. ``admin``,
  ``navigation`` or the default ``messages``) - see `Using Message Domains`_;

* **locale**: The locale that the translations are for (e.g. ``en_GB``, ``en``, etc);

* **loader**: How Symfony2 should load and parse the file (e.g. ``xliff``,
  ``php`` or ``yml``).

The loader can be the name of any registered loader. By default, Symfony
provides the following loaders:

* ``xliff``: XLIFF file;
* ``php``:   PHP file;
* ``yml``:  YAML file.

The choice of which loader to use is entirely up to you and is a matter of
taste.

.. note::

    You can also store translations in a database, or any other storage by
    providing a custom class implementing the
    :class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface` interface.
    See :doc:`Custom Translation Loaders </cookbook/translation/custom_loader>`
    below to learn how to register custom loaders.

.. index::
   single: Translations; Creating translation resources

Creating Translations
~~~~~~~~~~~~~~~~~~~~~

Each file consists of a series of id-translation pairs for the given domain and
locale. The id is the identifier for the individual translation, and can
be the message in the main locale (e.g. "Symfony is great") of your application
or a unique identifier (e.g. "symfony2.great" - see the sidebar below):

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

Symfony2 will discover these files and use them when translating either
"Symfony2 is great" or "symfony2.great" into a French language locale (e.g.
``fr_FR`` or ``fr_BE``).

.. sidebar:: Using Real or Keyword Messages

    This example illustrates the two different philosophies when creating
    messages to be translated:

    .. code-block:: php

        $t = $translator->trans('Symfony2 is great');

        $t = $translator->trans('symfony2.great');

    In the first method, messages are written in the language of the default
    locale (English in this case). That message is then used as the "id"
    when creating translations.

    In the second method, messages are actually "keywords" that convey the
    idea of the message. The keyword message is then used as the "id" for
    any translations. In this case, translations must be made for the default
    locale (i.e. to translate ``symfony2.great`` to ``Symfony2 is great``).

    The second method is handy because the message key won't need to be changed
    in every translation file if we decide that the message should actually
    read "Symfony2 is really great" in the default locale.

    The choice of which method to use is entirely up to you, but the "keyword"
    format is often recommended. 

    Additionally, the ``php`` and ``yaml`` file formats support nested ids to
    avoid repeating yourself if you use keywords instead of real text for your
    ids:

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

    The multiple levels are flattened into single id/translation pairs by
    adding a dot (.) between every level, therefore the above examples are
    equivalent to the following:

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
   single: Translations; Message domains

Using Message Domains
---------------------

As we've seen, message files are organized into the different locales that
they translate. The message files can also be organized further into "domains".
When creating message files, the domain is the first portion of the filename.
The default domain is ``messages``. For example, suppose that, for organization,
translations were split into three different domains: ``messages``, ``admin``
and ``navigation``. The French translation would have the following message
files:

* ``messages.fr.xliff``
* ``admin.fr.xliff``
* ``navigation.fr.xliff``

When translating strings that are not in the default domain (``messages``),
you must specify the domain as the third argument of ``trans()``:

.. code-block:: php

    $this->get('translator')->trans('Symfony2 is great', array(), 'admin');

Symfony2 will now look for the message in the ``admin`` domain of the user's
locale.

.. index::
   single: Translations; User's locale

Handling the User's Locale
--------------------------

The locale of the current user is stored in the session and is accessible
via the ``session`` service:

.. code-block:: php

    $locale = $this->get('session')->getLocale();

    $this->get('session')->setLocale('en_US');

.. index::
   single: Translations; Fallback and default locale

Fallback and Default Locale
~~~~~~~~~~~~~~~~~~~~~~~~~~~

If the locale hasn't been set explicitly in the session, the ``fallback_locale``
configuration parameter will be used by the ``Translator``. The parameter
defaults to ``en`` (see `Configuration`_).

Alternatively, you can guarantee that a locale is set on the user's session
by defining a ``default_locale`` for the session service:

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

The Locale and the URL
~~~~~~~~~~~~~~~~~~~~~~

Since the locale of the user is stored in the session, it may be tempting
to use the same URL to display a resource in many different languages based
on the user's locale. For example, ``http://www.example.com/contact`` could
show content in English for one user and French for another user. Unfortunately,
this violates a fundamental rule of the Web: that a particular URL returns
the same resource regardless of the user. To further muddy the problem, which
version of the content would be indexed by search engines?

A better policy is to include the locale in the URL. This is fully-supported
by the routing system using the special ``_locale`` parameter:

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

When using the special `_locale` parameter in a route, the matched locale
will *automatically be set on the user's session*. In other words, if a user
visits the URI ``/fr/contact``, the locale ``fr`` will automatically be set
as the locale for the user's session.

You can now use the user's locale to create routes to other translated pages
in your application.

.. index::
   single: Translations; Pluralization

Pluralization
-------------

Message pluralization is a tough topic as the rules can be quite complex. For
instance, here is the mathematic representation of the Russian pluralization
rules::

    (($number % 10 == 1) && ($number % 100 != 11)) ? 0 : ((($number % 10 >= 2) && ($number % 10 <= 4) && (($number % 100 < 10) || ($number % 100 >= 20))) ? 1 : 2);

As you can see, in Russian, you can have three different plural forms, each
given an index of 0, 1 or 2. For each form, the plural is different, and
so the translation is also different.

When a translation has different forms due to pluralization, you can provide
all the forms as a string separated by a pipe (``|``)::

    'There is one apple|There are %count% apples'

To translate pluralized messages, use the
:method:`Symfony\\Component\\Translation\\Translator::transChoice` method:

.. code-block:: php

    $t = $this->get('translator')->transChoice(
        'There is one apple|There are %count% apples',
        10,
        array('%count%' => 10)
    );

The second argument (``10`` in this example), is the *number* of objects being
described and is used to determine which translation to use and also to populate
the ``%count%`` placeholder.

Based on the given number, the translator chooses the right plural form.
In English, most words have a singular form when there is exactly one object
and a plural form for all other numbers (0, 2, 3...). So, if ``count`` is
``1``, the translator will use the first string (``There is one apple``)
as the translation. Otherwise it will use ``There are %count% apples``.

Here is the French translation::

    'Il y a %count% pomme|Il y a %count% pommes'

Even if the string looks similar (it is made of two sub-strings separated by a
pipe), the French rules are different: the first form (no plural) is used when
``count`` is ``0`` or ``1``. So, the translator will automatically use the
first string (``Il y a %count% pomme``) when ``count`` is ``0`` or ``1``.

Each locale has its own set of rules, with some having as many as six different
plural forms with complex rules behind which numbers map to which plural form.
The rules are quite simple for English and French, but for Russian, you'd
may want a hint to know which rule matches which string. To help translators,
you can optionally "tag" each string::

    'one: There is one apple|some: There are %count% apples'

    'none_or_one: Il y a %count% pomme|some: Il y a %count% pommes'

The tags are really only hints for translators and don't affect the logic
used to determine which plural form to use. The tags can be any descriptive
string that ends with a colon (``:``). The tags also do not need to be the
same in the original message as in the translated one.

.. tip:

    As tags are optional, the translator doesn't use them (the translator will
    only get a string based on its position in the string).

Explicit Interval Pluralization
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The easiest way to pluralize a message is to let Symfony2 use internal logic
to choose which string to use based on a given number. Sometimes, you'll
need more control or want a different translation for specific cases (for
``0``, or when the count is negative, for example). For such cases, you can
use explicit math intervals::

    '{0} There is no apples|{1} There is one apple|]1,19] There are %count% apples|[20,Inf] There are many apples'

The intervals follow the `ISO 31-11`_ notation. The above string specifies
four different intervals: exactly ``0``, exactly ``1``, ``2-19``, and ``20``
and higher.

You can also mix explicit math rules and standard rules. In this case, if
the count is not matched by a specific interval, the standard rules take
effect after removing the explicit rules::

    '{0} There is no apples|[20,Inf] There are many apples|There is one apple|a_few: There are %count% apples'

For example, for ``1`` apple, the standard rule ``There is one apple`` will
be used. For ``2-19`` apples, the second standard rule ``There are %count%
apples`` will be selected.

An :class:`Symfony\\Component\\Translation\\Interval` can represent a finite set
of numbers::

    {1,2,3,4}

Or numbers between two other numbers::

    [1, +Inf[
    ]-1,2[

The left delimiter can be ``[`` (inclusive) or ``]`` (exclusive). The right
delimiter can be ``[`` (exclusive) or ``]`` (inclusive). Beside numbers, you
can use ``-Inf`` and ``+Inf`` for the infinite.

.. index::
   single: Translations; In templates

Translations in Templates
-------------------------

Most of the time, translation occurs in templates. Symfony2 provides native
support for both Twig and PHP templates.

Twig Templates
~~~~~~~~~~~~~~

Symfony2 provides specialized Twig tags (``trans`` and ``transchoice``) to
help with message translation of *static blocks of text*:

.. code-block:: jinja

    {% trans %}Hello %name%{% endtrans %}

    {% transchoice count %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

The ``transchoice`` tag automatically gets the ``%count%`` variable from
the current context and passes it to the translator. This mechanism only
works when you use a placeholder following the ``%var%`` pattern.

.. tip::

    If you need to use the percent character (``%``) in a string, escape it by
    doubling it: ``{% trans %}Percent: %percent%%%{% endtrans %}``

You can also specify the message domain and pass some additional variables:

.. code-block:: jinja

    {% trans with {'%name%': 'Fabien'} from "app" %}Hello %name%{% endtrans %}

    {% transchoice count with {'%name%': 'Fabien'} from "app" %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

The ``trans`` and ``transchoice`` filters can be used to translate *variable
texts* and complex expressions:

.. code-block:: jinja

    {{ message | trans }}

    {{ message | transchoice(5) }}

    {{ message | trans({'%name%': 'Fabien'}, "app") }}

    {{ message | transchoice(5, {'%name%': 'Fabien'}, 'app') }}

.. tip::

    Using the translation tags or filters have the same effect, but with
    one subtle difference: automatic output escaping is only applied to
    variables translated using a filter. In other words, if you need to
    be sure that your translated variable is *not* output escaped, you must
    apply the raw filter after the translation filter:

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

The translator service is accessible in PHP templates through the
``translator`` helper:

.. code-block:: html+php

    <?php echo $view['translator']->trans('Symfony2 is great') ?>

    <?php echo $view['translator']->transChoice(
        '{0} There is no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10)
    ) ?>

Forcing the Translator Locale
-----------------------------

When translating a message, Symfony2 uses the locale from the user's session
or the ``fallback`` locale if necessary. You can also manually specify the
locale to use for translation:

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

Translating Database Content
----------------------------

The translation of database content should be handled by Doctrine through
the `Translatable Extension`_. For more information, see the documentation
for that library.

Summary
-------
With the Symfony2 Translation component, creating an internationalized application

no longer needs to be a painful process and boils down to just a few basic
steps:

* Abstract messages in your application by wrapping each in either the
  :method:`Symfony\\Component\\Translation\\Translator::trans` or
  :method:`Symfony\\Component\\Translation\\Translator::transChoice` methods;

* Translate each message into multiple locales by creating translation message
  files. Symfony2 discovers and processes each file because its name follows
  a specific convention;

* Manage the user's locale, which is stored in the session.

.. _`strtr function`: http://www.php.net/manual/en/function.strtr.php
.. _`ISO 31-11`: http://en.wikipedia.org/wiki/Interval_%28mathematics%29#The_ISO_notation
.. _`Translatable Extension`: https://github.com/l3pp4rd/DoctrineExtensions
