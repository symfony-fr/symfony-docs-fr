.. index::
   single: Filesystem

Le Composant « Filesystem » (« système de fichiers » en français)
=================================================================

    Le Composant « Filesystem » fournit des utilitaires basiques pour
    le système de fichiers.

.. versionadded:: 2.1
    Le Composant « Filesystem » fait son apparition dans Symfony 2.1. Auparavant,
    la classe ``Filesystem`` était située dans le composant ``HttpKernel``.

Installation
------------

Vous pouvez installer le composant de différentes manières :

* Utilisez le dépôt officiel Git (https://github.com/symfony/Filesystem) ;
* Installez le via Composer (`symfony/filesystem` sur Packagist).

Utilisation
-----------

La classe :class:`Symfony\\Component\\Filesystem\\Filesystem` est l'unique
endroit où se trouve les utilitaires pour effectuer des opérations sur le
système de fichiers::

    use Symfony\Component\Filesystem\Filesystem;
    use Symfony\Component\Filesystem\Exception\IOException;

    $fs = new Filesystem();

    try {
        $fs->mkdir('/tmp/random/dir/' . mt_rand());
    } catch (IOException $e) {
        echo "An error occured while creating your directory";
    }

.. note::

    Les méthodes :method:`Symfony\\Component\\Filesystem\\Filesystem::mkdir`,
    :method:`Symfony\\Component\\Filesystem\\Filesystem::chgrp`,
    :method:`Symfony\\Component\\Filesystem\\Filesystem::chown`,
    :method:`Symfony\\Component\\Filesystem\\Filesystem::remove` et
    :method:`Symfony\\Component\\Filesystem\\Filesystem::touch` peuvent
    recevoir une chaîne de caractères, un tableau ou n'importe quel objet
    implémentant :phpclass:`Traversable` en tant qu'argument cible.

Mkdir
~~~~~

« Mkdir » crée un répertoire. Sur les systèmes de fichiers posix, les répertoires
sont créés avec une valeur de mode qui par défaut est `0777`. Vous pouvez
utiliser le second argument pour définir votre propre mode::

    $fs->mkdir('/tmp/photos', 0700);

.. note::

    Vous pouvez passer un tableau ou quelconque objet :phpclass:`Traversable`
    en tant que premier argument.

Exists
~~~~~~

« Exists » vérifie la présence de tous les fichiers ou répertoires et retourne « false »
si un fichier est manquant::

    // ce répertoire existe, retourne « true »
    $fs->exists('/tmp/photos');

    // rabbit.jpg existe, bottle.png n'existe pas, retourne « false »
    $fs->exists(array('rabbit.jpg', 'bottle.png'));

.. note::

    Vous pouvez passer un tableau ou n'importe quel objet :phpclass:`Traversable`
    en tant que premier argument.

Copy
~~~~

Cette méthode est utilisée pour copier des fichiers. Si la cible existe déjà,
le fichier est copié seulement si la date de modification de la source est
plus récente que celle de la cible. Ce comportement peut être surchargé par
un troisième argument booléen::

    // fonctionne uniquement si image-ICC a été modifié après image.jpg
    $fs->copy('image-ICC.jpg', 'image.jpg');

    // image.jpg va être écrasé
    $fs->copy('image-ICC.jpg', 'image.jpg', true);

Touch
~~~~~

« Touch » définit la date de modification et d'accès d'un fichier. La date courante
est utilisée par défaut. Vous pouvez définir la vôtre avec le second argument.
Le troisième argument est la date d'accès::

    // définit la date de modification avec la date courante
    $fs->touch('file.txt');
    // définit la date de modification avec la date courante + 10 secondes
    $fs->touch('file.txt', time() + 10);
    // définit la date d'accès avec la date courante - 10 secondes
    $fs->touch('file.txt', time(), time() - 10);

.. note::

    Vous pouvez passer un tableau ou n'importe quel objet :phpclass:`Traversable`
    en tant que premier argument.

Chown
~~~~~

« Chown » est utilisée pour changer le propriétaire d'un fichier. Le troisième
argument est une option récursive booléenne::

    // définit le propriétaire de la vidéo lolcat comme étant www-data
    $fs->chown('lolcat.mp4', 'www-data');
    // change le propriétaire du répertoire « video » de manière récursive
    $fs->chown('/video', 'www-data', true);

.. note::

    Vous pouvez passer un tableau ou n'importe quel objet :phpclass:`Traversable`
    en tant que premier argument.

Chgrp
~~~~~

« Chgrp » est utilisée pour changer le groupe d'un fichier. Le troisième
argument est une option récursive booléenne::

    // définit le groupe de la vidéo lolcat comme étant nginx
    $fs->chgrp('lolcat.mp4', 'nginx');
    // change le groupe du répertoire « video » de manière récursive
    $fs->chgrp('/video', 'nginx', true);


.. note::

    Vous pouvez passer un tableau ou n'importe quel objet :phpclass:`Traversable`
    en tant que premier argument.

Chmod
~~~~~

« Chmod » est utilisée pour changer le mode d'un fichier. Le troisième
argument est une option récursive booléenne::

    // définit le mode de la vidéo comme étant 0600
    $fs->chmod('video.ogg', 0600);
    // change le mode du répertoire « src » de manière récursive
    $fs->chmod('src', 0700, true);

.. note::

    Vous pouvez passer un tableau ou n'importe quel objet :phpclass:`Traversable`
    en tant que premier argument.

Remove
~~~~~~

« Remove » vous permet de supprimer des fichiers, des liens symboliques et
des répertoires très facilement::

    $fs->remove(array('symlink', '/path/to/directory', 'activity.log'));

.. note::

    Vous pouvez passer un tableau ou n'importe quel objet :phpclass:`Traversable`
    en tant que premier argument.

Rename
~~~~~~

« Rename » est utilisée pour renommer des fichiers et des répertoires::

    // renomme un fichier
    $fs->rename('/tmp/processed_video.ogg', '/path/to/store/video_647.ogg');
    // renomme un répertoire
    $fs->rename('/tmp/files', '/path/to/store/files');

symlink
~~~~~~~

Crée un lien symbolique depuis une cible vers la destination. Si le système de
fichiers ne supporte pas les liens symboliques, un troisième argument booléen
est disponible::

    // crée un lien symbolique
    $fs->symlink('/path/to/source', '/path/to/destination');
    // duplique le répertoire source si le système de fichiers ne supporte pas les
    // liens symboliques
    $fs->symlink('/path/to/source', '/path/to/destination', true);

makePathRelative
~~~~~~~~~~~~~~~~

Retourne le chemin relatif d'un répertoire par rapport à un autre::

    // retourne '../'
    $fs->makePathRelative('/var/lib/symfony/src/Symfony/', '/var/lib/symfony/src/Symfony/Component');
    // retourne 'videos'
    $fs->makePathRelative('/tmp', '/tmp/videos');

mirror
~~~~~~

« Reflète » un répertoire::

    $fs->mirror('/path/to/source', '/path/to/target');

isAbsolutePath
~~~~~~~~~~~~~~

isAbsolutePath retourne « true » si le chemin donné est absolu, « false » sinon::

    // retourne « true »
    $fs->isAbsolutePath('/tmp');
    // retourne « true »
    $fs->isAbsolutePath('c:\\Windows');
    // retourne « false »
    $fs->isAbsolutePath('tmp');
    // retourne « false »
    $fs->isAbsolutePath('../dir');

Gestion des erreurs
-------------------

Chaque fois que quelque chose de faux/mal intervient, une exception implémentant
:class:`Symfony\\Component\\Filesystem\\Exception\\ExceptionInterface` est
lancée.

.. note::

    Avant la version 2.1, :method:`Symfony\\Component\\Filesystem\\Filesystem::mkdir`
    retournait un booléen et ne lançait pas d'exception. Depuis la version 2.1, une
    :class:`Symfony\\Component\\Filesystem\\Exception\\IOException` est lancée
    si la création d'un répertoire échoue.
