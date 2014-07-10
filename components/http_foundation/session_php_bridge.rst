.. index::
   single: HTTP
   single: HttpFoundation, Sessions

Intégration avec les session "Legacy"
=====================================

Parfois, il peut être nécessaire d'intégrer Symfony dans une vieille application
("legacy") où vous n'avez pas le niveau de contrôle dont vous avez besoin.

Comme spécifié ailleurs, les sessions Symfony sont conçues pour remplacer
l'usage des fonctions natives PHP ``session_*()`` et l'utilisation de la
variable superglobale ``$_SESSION``. De plus, Symfony doit obligatoirement
démarrer la session.

Cependant, quand vous n'avez vraiment pas le choix, vous pouvez utiliser
une passerelle spéciale :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\PhpBridgeSessionStorage`
qui est conçue pour permettre à Symfony de travailler avec une session qui a
été démarrée en dehors du framework Symfony. A moins d'être très prudent, vous
devez savoir que la session peut être interrompue par quelque chose, par exemple,
si l'application legacy efface la variable ``$_SESSION``.

Un usage typique ressemblerait à ceci::

    use Symfony\Component\HttpFoundation\Session\Session;
    use Symfony\Component\HttpFoundation\Session\Storage\PhpBridgeSessionStorage;

    // l'application legacy configurer la session
    ini_set('session.save_handler', 'files');
    ini_set('session.save_path', '/tmp');
    session_start();

    // interfaçage avec cette session grâce à Symfony
    $session = new Session(new PhpBridgeSessionStorage());

    // Symfony travaille maintenant avec la session
    $session->start();


Cela vous permet de commencer à utiliser l'API de session Symfony et de permettre
la migration de votre application vers les sessions Symfony.

.. note::

    Les sessions Symfony stockent des données comme des attributs dans des "Bags"
    (sacs) spéciaux qui utilisent des clés dans la variable superglobale ``$_SESSION``.
    Cela signifie qu'un session Symfony ne peut pas accéder aux clés de ``$_SESSION``
    qui ont été définies par l'application legacy bien que le contenu de ``$_SESSION``
    ait été enregistré lors de l'enregistrement de la session.
