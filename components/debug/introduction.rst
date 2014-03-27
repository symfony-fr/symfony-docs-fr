.. index::
   single: Debug
   single: Components; Debug

Le Composant Debug
==================

    Le composant Debug founit des outils pour faciliter le debugging
    du code PHP.

.. versionadded:: 2.3
    Le Composant Debug est nouveau en Symfony 2.3. Précédemment, les classes
    étaient situées dans le composant HttpKernel.

Installation
------------

Vous pouvez installer le composant de deux manières différentes :

* Utilisez le dépôt Git officiel (https://github.com/symfony/Debug);
* :doc:`Installez le via Composer </components/using_components>` (``symfony/debug`` sur `Packagist`_).

Utilisation
-----------

Le composant Debug fournit quelques outils pour vous aider à débugger
du code PHP. Activer ces outils est aussi simple que de faire ::

    use Symfony\Component\Debug\Debug;

    Debug::enable();

La méthode :method:`Symfony\\Component\\Debug\\Debug::enable` enregistre
un error handler (gestionnaire d'erreur), un exception handler (gestionnaire
d'exception) et une :doc:`classe loader spéciale </components/debug/class_loader>`.

Lisez les sections suivantes pour plus d'informations sur les différents
outils disponibles.

.. caution::

    Vous ne devriez jamais activer les outils de debug en environnement de
    production car ils divulgueraient des informations sensibles à l'utilisateur.

Activer l'Error Handler
-----------------------

La classe :class:`Symfony\\Component\\Debug\\ErrorHandler` attrape les erreurs
PHP et les convertit en exceptions (de la classe :phpclass:`ErrorException`
ou :class:`Symfony\\Component\\Debug\\Exception\\FatalErrorException` pour les
fatal erreurs PHP) ::

    use Symfony\Component\Debug\ErrorHandler;

    ErrorHandler::register();

Activer l'Exception Handler
---------------------------

La classe :class:`Symfony\\Component\\Debug\\ExceptionHandler` attrape les
exceptions PHP non rattrapées et les convertit en jolie réponses PHP. C'est
utile en mode debug pour remplacer la sortie PHP/XDebug par défaut, par
quelque chose de plus joli et plus utile ::

    use Symfony\Component\Debug\ExceptionHandler;

    ExceptionHandler::register();

.. note::

    Si le :doc:`composant HttpFoundation </components/http_foundation/introduction>` est
    disponible, le handler (gestionnaire) utilise un objet Response de Symfony; le cas
    échéant, il retourne une réponse PHP standard.

.. _Packagist: https://packagist.org/packages/symfony/debug
