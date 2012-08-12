.. index::
   single: Debugging

Comment optimiser votre environnement pour le debuggage
=======================================================

Quand vous travaillez sur un projet Symfony sur votre machine locale, vous
devriez utiliser l'environnement ``dev`` (correspondant au contrôleur frontal
``app_dev.php``). Cet environnement est optimisé pour :

* Donner au développeur des informations rapides et claires si quelque chose ne
  se déroule pas comme prévu (à l'aide de la web debug toolbar, d'exceptions
  documentées et présentées clairement, du profiler, ...) ;

* Etre aussi proche que possible à l'environnement de production afin de
  préparer le déploiement du projet.

.. _cookbook-debugging-disable-bootstrap:

Désactiver le bootstrap et le cache des classes
-----------------------------------------------

Pour rendre l'environnement de production aussi rapide que possible, Symfony
crée de longs fichiers PHP dans le dossier cache, qui correspondent à
l'aggrégation des classes PHP dont votre projet a besoin à chaque requête.
Cependant, ce comportement peut désorienter votre IDE ou votre debugger. Nous
allons vous montrer ici comment modifier le mécanisme de cache afin qu'il
permette un débuggage des classes intégrées à Symfony.

Le contrôleur frontal ``app_dev.php`` se compose par défaut du code suivant::

    // ...

    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('dev', true);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

Pour faciliter le travail du debugger, désactivez le cache des classes PHP en
supprimant l'appel ``loadClassCache()`` et en replaçant les fichiers requis comme
ceci::

    // ...

    // require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../vendor/symfony/symfony/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';
    require_once __DIR__.'/../app/autoload.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('dev', true);
    // $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

.. tip::

    Si vous désactivez le cache des classes, n'oubliez pas de revenir aux
    réglages initiaux après votre session de débuggage.

Certains IDEs n'apprécient pas que certaines classes soient enregistrées à
différents emplacements. Pour prévenir ces problèmes, vous pouvez désactiver la
lecture du dossier cache dans votre IDE, ou changer l'extension utilisée par
Symfony pour ces fichiers::

    $kernel->loadClassCache('classes', '.php.cache');
