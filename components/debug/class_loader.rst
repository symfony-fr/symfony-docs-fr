.. index::
    single: Class Loader; DebugClassLoader
    single: Debug; DebugClassLoader

Débugger un Class Loader
========================

.. versionadded:: 2.4
    Le ``DebugClassLoader`` du composant Debug est nouveau dans Symfony 2.4.
    Précédemment, cette classe était située dans le composant Class Loader.

La classe :class:`Symfony\\Component\\Debug\\DebugClassLoader` tente de
jeter des exceptions plus explicites lorsqu'une classe n'est pas retrouvée
par les autoladers enregistrés. Tous les autoloaders qui implémentent la
méthode ``findFile()`` sont remplacé avec un wrapper ``DebugClassLoader``.

L'utilisation de ``DebugClassLoader`` est aussi facile que d'appeler
la méthode statique :method:`Symfony\\Component\\Debug\\DebugClassLoader::enable` ::

    use Symfony\Component\ClassLoader\DebugClassLoader;

    DebugClassLoader::enable();
