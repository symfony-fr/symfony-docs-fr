.. index::
   single: Locale
   single: Components; Locale

Le Composant Locale
===================

    Le composant « Locale » fournit une solution de secours pour gérer les cas où l'extension ``intl`` est
    manquante. De plus, il étend l'implémentation de la classe native :phpclass:`Locale` avec plusieurs
    méthodes pratiques.

Des solutions de substitution pour les fonctions et classes suivantes sont fournies :

* :phpfunction:`intl_is_failure`
* :phpfunction:`intl_get_error_code`
* :phpfunction:`intl_get_error_message`
* :phpclass:`Collator`
* :phpclass:`IntlDateFormatter`
* :phpclass:`Locale`
* :phpclass:`NumberFormatter`

.. note::

    L'implémentation de Stub supporte uniquement la locale ``en``.

Installation
------------

Vous pouvez installer le composant de différentes manières :

* Utilisez le dépôt Git officiel (https://github.com/symfony/Locale) ;
* Installez le via PEAR (`pear.symfony.com/Locale`) ;
* Installez le via Composer (`symfony/locale` dans Packagist).

Utilisation
-----------

Tirer parti de ce « code de secours » inclut le fait de requérir les bouts de fonctions et d'ajouter les
morceaux de classes à l'« autoloader ».

Lorsque vous utilisez le composant « ClassLoader », le code suivant est suffisant pour pallier l'extension
``intl`` manquante :

.. code-block:: php

    if (!function_exists('intl_get_error_code')) {
        require __DIR__.'/path/to/src/Symfony/Component/Locale/Resources/stubs/functions.php';

        $loader->registerPrefixFallbacks(array(__DIR__.'/path/to/src/Symfony/Component/Locale/Resources/stubs'));
    }

La classe :class:`Symfony\\Component\\Locale\\Locale` enrichit la classe native :phpclass:`Locale` avec des
fonctionnalités supplémentaires :

.. code-block:: php

    use Symfony\Component\Locale\Locale;

    // récupère les noms de pays pour une locale ou récupère tous les codes de pays
    $countries = Locale::getDisplayCountries('pl');
    $countryCodes = Locale::getCountries();

    // récupère les noms de langue pour une locale ou récupère tous les codes de langue
    $languages = Locale::getDisplayLanguages('fr');
    $languageCodes = Locale::getLanguages();

    // récupère les noms de locale pour un code donné ou récupère tous les codes de locale
    $locales = Locale::getDisplayLocales('en');
    $localeCodes = Locale::getLocales();

    // récupère les versions ICU
    $icuVersion = Locale::getIcuVersion();
    $icuDataVersion = Locale::getIcuDataVersion();
