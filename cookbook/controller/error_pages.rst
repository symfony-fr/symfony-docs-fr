.. index::
   single: Controller; Customize error pages
   single: Error pages

Comment personnaliser les pages d'erreur
========================================

Lorsqu'une exception quelconque est lancée dans Symfony2, cette dernière
est « capturée » par la classe ``Kernel`` et éventuellement transmise à
un contrôleur spécial, ``TwigBundle:Exception:show`` pour qu'il la gère.
Ce contrôleur, qui fait partie du coeur de ``TwigBundle``, détermine quelle
template d'erreur afficher et le code de statut qui devrait être défini
pour l'exception donnée.

Les pages d'erreur peuvent être personnalisées de deux manières différentes,
dépendant du niveau de contrôle que vous souhaitez :

1. Personnalisez les templates d'erreur des différentes pages d'erreur
   (expliqué ci-dessous) ;

2. Remplacez le contrôleur d'exception par défaut ``TwigBundle::Exception:show``
   par votre propre contrôleur et gérez le comme vous le désirez (voir
   :ref:`exception_controller dans la référence de Twig<config-twig-exception-controller>`) ;

.. tip::

    La personnalisation de la gestion d'exception est en fait bien plus
    puissante que ce qui est écrit dans ces lignes. Un évènement interne,
    ``kernel.exception``, est lancé et permet d'avoir le contrôle complet
    de la gestion des exceptions. Pour plus d'informations, voir
    :ref:`kernel-kernel.exception`.

Tous les templates d'erreur se trouvent dans le ``TwigBundle``. Pour surcharger
ces templates, utilisez simplement la méthode standard qui permet
de surcharger un template qui se trouve dans un bundle. Pour plus d'informations,
voir :ref:`overriding-bundle-templates`.

Par exemple, pour surcharger le template d'erreur par défaut qui est
affiché à l'utilisateur final, créez un nouveau template situé à cet emplacement
``app/Resources/TwigBundle/views/Exception/error.html.twig`` :

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Une erreur est survenue : {{ status_text }}</title>
    </head>
    <body>
        <h1>Oups! Une erreur est survenue</h1>
        <h2>Le serveur a retourné une erreur "{{ status_code }} {{ status_text }}".</h2>
    </body>
    </html>

.. tip::

    Si vous n'êtes pas familier avec Twig, ne vous inquiétez pas. Twig est
    un moteur de templating simple, puissant et optionnel qui est intégré
    dans ``Symfony2``. Pour plus d'informations à propos de Twig, voir
    :doc:`/book/templating`.

En plus de la page d'erreur HTML standard, Symfony fournit une page d'erreur
par défaut pour quasiment tous les formats de réponse les plus communs,
incluant JSON (``error.json.twig``), XML (``error.xml.twig``), et même
Javascript (``error.js.twig``), pour n'en nommer que quelques uns. Pour surcharger
n'importe lequel de ces templates, créez simplement un nouveau fichier
avec le même nom dans le répertoire ``app/Resources/TwigBundle/views/Exception``.
C'est la manière standard de surcharger n'importe quel template qui se trouve
dans un bundle.

.. _cookbook-error-pages-by-status-code:

Personnaliser la page 404 et les autres pages d'erreur
------------------------------------------------------

Vous pouvez aussi personnaliser des templates d'erreur spécifiques en vous
basant sur le code de statut HTTP. Par exemple, créez un template
``app/Resources/TwigBundle/views/Exception/error404.html.twig`` pour
afficher une page spéciale pour les erreurs 404 (page non trouvée).

Symfony utilise l'algorithme suivant pour déterminer quel template utiliser :

* Premièrement, il cherche un template pour le format et le code de statut donné
  (ex ``error404.json.twig``) ;

* S'il n'existe pas, il cherche un template pour le format donné (ex
  ``error.json.twig``) ;

* S'il n'existe pas, il se rabat sur le template HTML (ex
  ``error.html.twig``).

.. tip::

    Pour voir la liste complète des templates d'erreur par défaut, jetez un
    oeil au répertoire ``Resources/views/Exception`` du ``TwigBundle``. Dans
    une installation standard de Symfony2, le ``TwigBundle`` se
    trouve à cet emplacement : ``vendor/symfony/symfony/src/Symfony/Bundle/TwigBundle``.
    Souvent, la façon la plus facile de personnaliser une page d'erreur
    est de la copier depuis le ``TwigBundle`` vers le dossier
    ``app/Resources/TwigBundle/views/Exception``, puis de la modifier.

.. note::

    Les pages d'exception aidant au débuggage qui sont montrées au développeur
    peuvent aussi être personnalisées de la même manière en créant des templates
    comme ``exception.html.twig`` pour la page d'exception HTML standard
    ou ``exception.json.twig`` pour la page d'exception JSON.
