.. index::
    single: Symfony2 Twig extensions

Extensions Twig Symfony2
========================

Twig est le moteur de template par défaut de Symfony2. De lui-même, il contient
déjà plein de fonctions pré-construites, des filtres, des tags et des tests
(`http://twig.sensiolabs.org/documentation`_ puis allez en bas de la page).

Symfony2 ajoute plus d'extensions personnalisées par dessus Twig pour intégrer
certains composants dans les templates Twig. Vous trouverez ci-dessous toutes
les informations sur les fonctions personnalisées, les filtres, les tags et les
tests qui sont ajoutés par le noyau du framework Symfony2.

Il peut également exister des tags dans les bundles que vous utilisez qui ne sont
pas listés ici.

Fonctions
---------

.. versionadded:: 2.2
    Les fonctions ``render`` et ``controller`` sont nouvelles depuis Symfony 2.2. avant,
    le tag ``{% render %}`` était utilisé et avait une signature différente.

+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| Syntaxe de la fonction                             | Usage                                                                                      |
+====================================================+============================================================================================+
| ``render(uri, options = {})``                      | Rend le fragment de template pour un controller ou une URL donnée.                         |
| ``render(controller('B:C:a', {params}))``          | Pour plus d'information, lire :ref:`templating-embedding-controller`.                      |
| ``render(path('route', {params}))``                |                                                                                            |
| ``render(url('route', {params}))``                 |                                                                                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``render_esi(controller('B:C:a', {params}))``      | Génère un tag ESI quand cela est possible sinon le fragment de template.                   |
| ``render_esi(url('route', {params}))``             | Pour plus d'information, lire :ref:`templating-embedding-controller`.                      |
| ``render_esi(path('route', {params}))``            |                                                                                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``render_hinclude(controller(...))``               | Génère un tag Hinclude pour un controller ou une URL donné.                                |
| ``render_hinclude(url('route', {params}))``        | Pour plus d'information, lire  :ref:`templating-embedding-controller`.                     |
| ``render_hinclude(path('route', {params}))``       |                                                                                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``controller(attributes = {}, query = {})``        | Utilisé avec le tag ``render`` pour référencer le controller que vous voulez rendre.       |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``asset(path, packageName = null)``                | Récupère le chemin public de la ressource. Plus d'infos sur                                |
|                                                    | ":ref:`book-templating-assets`".                                                           |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``asset_version(packageName = null)``              | Récupère la version actuelle du package. Plus d'infos sur                                  |
|                                                    | ":ref:`book-templating-assets`".                                                           |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form(view, variables = {})``                     | Affiche un formulaire HTML complet, plus d'informations                                    |
|                                                    | ici :ref:`the Twig Form reference<reference-forms-twig-form>`.                             |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_start(view, variables = {})``               | Affiche le premier tag d'un formulaire HTML, plus d'informations                           |
|                                                    | ici :ref:`the Twig Form reference<reference-forms-twig-start>`.                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_end(view, variables = {})``                 | Affiche le dernier tag d'un formulaire HTML avec l'ensemble des champs qui n'ont pas       |
|                                                    | encore été rendu, plus d'informations                                                      |
|                                                    | ici :ref:`the Twig Form reference<reference-forms-twig-end>`.                              |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_enctype(view)``                             | Affiche l'attribut ``enctype="multipart/form-data"`` si le formulaire contient             |
|                                                    | au moins un champ d'upload.Plus d'infos sur                                                |
|                                                    | :ref:`the Twig Form reference<reference-forms-twig-enctype>`.                              |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_widget(view, variables = {})``              | Affiche un formulaire complet, ou le widget HTML spécifique d'un champ.                    |
|                                                    | Plus d'infos sur  :ref:`the Twig Form reference<reference-forms-twig-widget>`.             |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_errors(view)``                              | Affiche toutes les erreurs d'un champ donné, ou les erreurs "globales".                    |
|                                                    | Plus d'infos sur  :ref:`the Twig Form reference<reference-forms-twig-errors>`.             |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_label(view, label = null, variables = {})`` | Affiche le libellé d'un champ donné. Plus d'infos sur                                      |
|                                                    | :ref:`the Twig Form reference<reference-forms-twig-label>`.                                |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_row(view, variables = {})``                 | Affiche un champ (libellé du champ, erreur et widget) donné. Plus d'infos sur              |
|                                                    | :ref:`the Twig Form reference<reference-forms-twig-row>`.                                  |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_rest(view, variables = {})``                | Affiche tous les champs qui n'ont pas encore été affichés. Plus d'infos sur                |
|                                                    | :ref:`the Twig Form reference<reference-forms-twig-rest>`.                                 |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``csrf_token(intention)``                          | Affiche le jeton CSRF. Utilisez cette fonction si vous voulez une protection CSRF          |
|                                                    | sans devoir créer de formulaire.                                                           |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``is_granted(role, object = null, field = null)``  | Retourne ``true`` si l'utilisateur actuel possède le rôle requis.                          |
|                                                    | Plus d'infos sur  ":ref:`book-security-template`"                                          |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``logout_path(key)``                               | Génère l'URL relative de déconnexion pour le pare-feu donné.                               |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``logout_url(key)``                                | Similaire à ``logout_path(...)`` mais génère une URL absolue.                              |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``path(name, parameters = {})``                    | Récupère une URL relative pour la route donnée. Plus d'infos sur                           |
|                                                    | ":ref:`book-templating-pages`".                                                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``url(name, parameters = {})``                     | Similaire à ``path(...)`` mais génère une URL absolue.                                     |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+

Filtres
-------

+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| Syntaxe du filtre                                                               | Usage                                                             |
+=================================================================================+===================================================================+
| ``text|humanize``                                                               | Rend un nom technique lisible par un humain (remplace les         |
|                                                                                 | underscores par des espaces et ajoute une majuscule à la chaine). |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``text|trans(arguments = {}, domain = 'messages', locale = null)``              | Traduit le texte dans le langage actuel. Plus d'infos sur         |
|                                                                                 | :ref:`Translation Filters<book-translation-filters>`.             |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``text|transchoice(count, arguments = {}, domain = 'messages', locale = null)`` | Traduit le texte en tenant compte de la pluralisation. Plus       |
|                                                                                 | d'infos sur :ref:`Translation Filters<book-translation-filters>`. |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``variable|yaml_encode(inline = 0)``                                            | Transforme une variable texte en une syntaxe YAML.                |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``variable|yaml_dump``                                                          | Affiche une syntaxe YAML avec son type.                           |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``classname|abbr_class``                                                        | Affiche un élément ``abbr`` avec le nom raccourci d'une classe    |
|                                                                                 | PHP.                                                              |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``methodname|abbr_method``                                                      | Affiche une méthode PHP dans un élément ``abbr``.                 |
|                                                                                 | (ex ``Symfony\Component\HttpFoundation\Response::getContent``     |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``arguments|format_args``                                                       | Affiche une chaine de caractères avec les arguments d'une         |
|                                                                                 | fonction et leurs types.                                          |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``arguments|format_args_as_text``                                               | Similaire à ``[...]|format_args``, mais sépare les éléments.      |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``path|file_excerpt(line)``                                                     | Affiche une partie de code d'un fichier autour de la ligne donnée.|
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``path|format_file(line, text = null)``                                         | Affiche le chemin d'un fichier dans un lien.                      |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``exceptionMessage|format_file_from_text``                                      | Similaire à ``format_file`` sauf qu'il extrait les erreurs PHP    |
|                                                                                 | par défaut dans un fichier (ex 'in foo.php on line 45')           |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``path|file_link(line)``                                                        | Affiche le chemin d'un fichier (et le numéro de ligne)            |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+

Tags
----

.. versionadded:: 2.3
    Le tag ``stopwatch`` a été ajouté à la version Symfony 2.3

+---------------------------------------------------+-------------------------------------------------------------------+
| Syntaxe du tag                                    | Usage                                                             |
+===================================================+===================================================================+
| ``{% form_theme form 'file' %}``                  | Permet de chercher, dans le fichier donné, le bloc de formulaire  |
|                                                   | à surchager : :doc:`/cookbook/form/form_customization`.           |
+---------------------------------------------------+-------------------------------------------------------------------+
| ``{% trans with {variables} %}...{% endtrans %}`` | Traduit et affiche le texte. Pour plus d'informations, lisez      |
|                                                   | :ref:`book-translation-tags`                                      |
+---------------------------------------------------+-------------------------------------------------------------------+
| ``{% transchoice count with {variables} %}``      | Traduit et affiche le texte en tenant compte de la pluralisation. |
| ...                                               | Pour plus d'informations, lisez :ref:`book-translation-tags`      |
| ``{% endtranschoice %}``                          |                                                                   |
+---------------------------------------------------+-------------------------------------------------------------------+
| ``{% trans_default_domain language %}``           | Défini le domaine par défaut pour le catalogue de messages dans   |
|                                                   | modèle en cours                                                   |
+---------------------------------------------------+-------------------------------------------------------------------+
| ``{% stopwatch name %}...{% endstopwatch %}``     | Calcule le temps d'éxécution du code entre les deux balises et    |
|                                                   | l'ajoute à la timeline du profiler.                               |
+---------------------------------------------------+-------------------------------------------------------------------+

Tests
-----

+---------------------------------------------------+------------------------------------------------------------------------------+
| Syntaxe du test                                   | Usage                                                                        |
+===================================================+==============================================================================+
| ``selectedchoice(choice, selectedValue)``         | Retourne ``true`` si le choix est sélectionné pour la valeur donnée.         |
+---------------------------------------------------+------------------------------------------------------------------------------+

Variables globales
------------------

+-------------------------------------------------------+------------------------------------------------------------------------------------+
| Variable                                              | Usage                                                                              |
+=======================================================+====================================================================================+
| ``app`` *Attributes*: ``app.user``, ``app.request``   | La variable ``app`` est disponible partout, et vous donne un accès rapide à        |
| ``app.session``, ``app.environment``, ``app.debug``   | plusieurs objets souvent nécessaires. La variable ``app`` est une instance de la   |
| ``app.security``                                      | classe :class:`Symfony\\Bundle\\FrameworkBundle\\Templating\\GlobalVariables`      |
+-------------------------------------------------------+------------------------------------------------------------------------------------+

Extensions de l'Edition Standard de Symfony
-------------------------------------------

L'Edition Standard de Symfony apporte certains bundles au framework Symfony2.
Ces bundles peuvent posséder d'autres extensions Twig :

* **Twig Extension** inclut toutes les extensions qui ne font pas partie
  du noyau de Twig mais qui peuvent servir. Vous pouvez en lire plus sur 
  `la documentation officielle des extensions Twig`_
* **Assetic** ajoute les tags ``{% stylesheets %}``, ``{% javascripts %}`` et 
  ``{% image %}``. Vous pouvez en savoir plus sur eux en lisant
  :doc:`the Assetic Documentation</cookbook/assetic/asset_management>`;

.. _`la documentation officielle des extensions Twig`: http://twig.sensiolabs.org/doc/extensions/index.html
.. _`http://twig.sensiolabs.org/documentation`: http://twig.sensiolabs.org/documentation
