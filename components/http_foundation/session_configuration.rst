.. index::
   single: HTTP
   single: HttpFoundation, Sessions

Configurer les Sessions et les Gestionnaires de Sauvegarde
==========================================================

Cette section explique comment configurer la gestion des sessions et comment
l'adapter à vos besoins spécifiques. Cette documentation parle aussi des
gestionnaires de sauvegarde, qui stockent et récupèrent les données de
session, et configurent le comportement des sessions.

Gestionnaires de sauvegarde
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le fonctionnement d'une session PHP possède six opérations possibles qui
peuvent arriver. La session normale suit le schéma suivant : `open` (« ouvrir » en
français, `read` (« lire » en français), `write` (« écrire » en français) et `close`
(« fermer » en français), avec la possibilité d'avoir aussi `destroy` (« détruire »
en français) et `gc` (ramasse-miettes qui va rendre n'importe quelles anciennes sessions
expirées : `gc` est appelée de manière aléatoire selon la documentation de PHP et
si elle est appelée, elle est invoquée après l'opération `open`). Vous pouvez en
lire plus à propos de ce sujet : `php.net/session.customhandler`_.

Gestionnaires de Sauvegarde PHP natifs
--------------------------------------

Les gestionnaires dits « natifs » sont des gestionnaires de sauvegarde qui sont
soit compilés en PHP, soit fournis en tant qu'extensions PHP, comme PHP-Sqlite,
PHP-Memcached, etc.

Tous les gestionnaires de sauvegarde sont internes à PHP, et en tant que tel, n'ont
pas d'API publique. Ils doivent être configurés via des directives PHP ini, généralement
``session.save_path`` et potentiellement d'autres directives spécifiques de « driver ».
Des détails plus précis peuvent être trouvés dans le « docblock » de la méthode
``setOptions()`` de chaque classe.

Bien que les gestionnaires de sauvegarde puissent être activés en utilisant directement
``ini_set('session.save_handler', $name);``, Symfony2 fournit un moyen pratique d'activer
ces derniers de la même manière que pour les gestionnaires personnalisés.

Symfony2 fournit des « drivers » pour le gestionnaire de sauvegarde natif suivant en
tant qu'exemple :

  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeFileSessionHandler`

Exemple d'utilisation::

    use Symfony\Component\HttpFoundation\Session\Session;
    use Symfony\Component\HttpFoundation\Session\Storage\NativeSessionStorage;
    use Symfony\Component\HttpFoundation\Session\Storage\Handler\NativeFileSessionHandler;

    $storage = new NativeSessionStorage(array(), new NativeFileSessionHandler());
    $session = new Session($storage);

.. note::

    Excepté le gestionnaire des ``fichiers`` (« files » en anglais) qui est intégré dans PHP
    et toujours à disposition, la disponibilité des autres gestionnaires dépend du fait que
    ces extensions PHP soient activées ou non lors de l'exécution.

.. note::

    Les gestionnaires de sauvegarde natifs fournissent une solution rapide pour le stockage de session,
    cependant, dans des systèmes complexes où vous avez besoin de plus de contrôle, des gestionnaires
    de sauvegarde personnalisés peuvent fournir plus de liberté et de flexibilité. Symfony2 fournit
    plusieurs implémentations que vous pourriez personnaliser plus tard si nécessaire.

Gestionnaires de Sauvegarde Personnalisés
-----------------------------------------

Les gestionnaires personnalisés sont ceux qui remplacent complètement les gestionnaires de
sauvegarde intégrés dans PHP en fournissant six fonctions de « callback » que PHP appelle
en interne à différents points durant le flux d'exécution de la session.

Le composant « HttpFoundation » de Symfony2 en fournit quelques uns par défaut et ces derniers
peuvent vous servir d'exemples si vous souhaitez écrire le(s) vôtre(s).

  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\PdoSessionHandler`
  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\MemcacheSessionHandler`
  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\MemcachedSessionHandler`
  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NullSessionHandler`

Exemple d'utilisation::

    use Symfony\Component\HttpFoundation\Session\Session;
    use Symfony\Component\HttpFoundation\Session\Storage\SessionStorage;
    use Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler;

    $storage = new NativeSessionStorage(array(), new PdoSessionHandler());
    $session = new Session($storage);


Configurer les Sessions PHP
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le classe :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage`
peut configurer la plupart des directives de configuration PHP ini qui sont documentées ici :
`php.net/session.configuration`_.

Pour configurer ces paramètres, passez les clés (en omettant la partie initiale ``session.``
de la clé) en tant qu'un tableau clés-valeurs à l'argument ``$options`` du constructeur.
Ou définissez-les via la méthode
:method:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage::setOptions`.

Pour des raisons de clarté, certaines options clés sont expliquées dans cette documentation.

Durée de vie du Cookie de Session
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour des raisons de sécurité, il est généralement recommandé que les jetons de sécurité soient
envoyés comme des cookies de session. Vous pouvez configurer la durée de vie de ces derniers
en spécifiant (en secondes) la clé ``cookie_lifetime`` dans l'argument ``$options``
du constructeur de :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage`.

Définir ``cookie_lifetime`` à ``0`` va avoir pour effet que le cookie perdurera
uniquement jusqu'à ce que le navigateur reste ouvert. Généralement, ``cookie_lifetime``
devrait être défini avec un grand nombre de jours, semaines ou mois. Il n'est pas
rare de définir des cookies pour une année ou plus dépendant de l'application.

Comme les cookies de session sont juste des jetons côté client, ils sont moins
importants pour le contrôle des détails de vos paramètres de sécurité qui peuvent
en fin de compte être contrôlés de manière sécuritaire seulement côté serveur.

.. note::

    Le paramètre ``cookie_lifetime`` est le nombre de secondes durant lequel le
    cookie devrait perdurer, ce n'est pas un « timestamp Unix ». Le cookie de
    session résultant va être estampillé avec une date d'expiration correspondant
    à ``time()``+``cookie_lifetime`` où la date « time » est prise depuis le
    serveur.

Configurer le Ramasse-Miettes (« Garbage Collector » en anglais)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Lorsqu'une session débute, PHP va appeler le gestionnaire ``gc`` de manière
aléatoire selon la probabilité définie par ``session.gc_probability`` /
``session.gc_divisor``. Par exemple, si ces dernières étaient définies
respectivement avec ``5 / 100``, cela signifierait une probabilité de 5%.
De même, ``3 / 4`` signifierait 3 chances sur 4 d'être appelé, i.e. 75%.

Si le gestionnaire de ramasse-miettes est invoqué, PHP va passer la valeur stockée
dans la directive PHP ini ``session.gc_maxlifetime`. La signification dans ce
contexte est que n'importe quelle session stockée qui a été sauvegardée il y a
plus longtemps que ``maxlifetime`` devrait être supprimée. Cela permet d'expirer
des enregistrements selon leur temps d'inactivité.

Vous pouvez configurer ces paramètres en passant un tableau contenant
``gc_probability``, ``gc_divisor`` et ``gc_maxlifetime`` au constructeur
de :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage`
ou à la méthode
:method:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage::setOptions`.

Durée de vie de la Session
~~~~~~~~~~~~~~~~~~~~~~~~~~

Quand une nouvelle session est créée, signifiant que Symfony2 envoie un
nouveau cookie de session au client, le cookie va être estampillé avec une
date d'expiration. Cette dernière est calculée en ajoutant la valeur de
configuration PHP de ``session.cookie_lifetime`` à la date courante du serveur.

.. note::

    PHP va générer un cookie une fois seulement. On attend du client qu'il stocke
    ce cookie pour toute la durée de vie spécifiée. Un nouveau cookie sera généré
    uniquement lorsque la session est détruite, le cookie du navigateur est
    supprimé, ou que l'ID de la session est regénéré en utilisant les méthodes
    ``migrate()`` ou ``invalidate()`` de la classe ``Session``.

    La durée de vie initiale du cookie peut être définie en configurant
    ``NativeSessionStorage`` grâce à la méthode
    ``setOptions(array('cookie_lifetime' => 1234))``.

.. note::

    Une durée de vie de cookie égale à ``0`` signifie que le cookie expire
    lorsque le navigateur est fermé.

Durée d'Inactivité de la Session/Maintenir Actif
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il existe souvent des circonstances durant lesquelles vous souhaitez protéger
ou minimiser l'utilisation d'une session quand un utilisateur étant connecté
s'éloigne de son terminal en détruisant sa session après une certaine période
d'inactivité. Par exemple, cela est commun pour les applications bancaires
de déconnecter (« log out » en anglais) l'utilisateur après 5 ou 10 minutes
d'inactivité. Définir la durée de vie du cookie n'est pas approprié ici
car cela peut être manipulé par le client, donc nous devons rendre effectif
l'expiration du côté du serveur. La manière la plus facile est d'implémenter
ce mécanisme via le ramasse-miettes qui s'exécute assez fréquemment. Le
cookie ``lifetime`` serait défini avec une valeur relativement grande,
et la propriété ``maxlifetime`` du ramasse-miettes serait définie afin
de détruire les sessions au bout de quelconque période d'inactivité souhaitée.

L'autre option est de vérifier spécifiquement si une session a expiré après
qu'elle ait été démarrée. La session peut être détruite comme nous le souhaitons.
Ce procédé permet d'intégrer l'expiration des sessions au sein de l'expérience
utilisateur, par exemple, en affichant un message.

Symfony2 enregistre quelques métadonnées basiques à propos de chaque session
afin de vous offrir une liberté entière en ce qui concerne ce sujet.

Métadonnées de Session
~~~~~~~~~~~~~~~~~~~~~~

Les sessions possèdent quelques métadonnées permettant d'effectuer des
réglages fins sur les paramètres de sécurité. L'objet session possède un
accesseur pour les métadonnées, :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getMetadataBag`
qui expose une instance de :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\MetadataBag`::

    $session->getMetadataBag()->getCreated();
    $session->getMetadataBag()->getLastUsed();

Les deux méthodes retournent un « timestamp Unix » (relatif au serveur).

Ces métadonnées peuvent être utilisées pour rendre explicitement une session
expirée lors d'un accès au site, par exemple::

    $session->start();
    if (time() - $session->getMetadataBag()->getLastUpdate() > $maxIdleTime) {
        $session->invalidate();
        throw new SessionExpired(); // rediriger vers la page d'expiration de session
    }

Il est aussi possible de dire comment le ``cookie_lifetime`` a été défini
pour un cookie particulier en observant le retour de la méthode ``getLifetime()``::

    $session->getMetadataBag()->getLifetime();

La date d'expiration du cookie peut être déterminée en ajoutant le « timestamp »
créé et la durée de vie.

Compatibilité avec PHP 5.4
~~~~~~~~~~~~~~~~~~~~~~~~~~

Depuis PHP 5.4.0, :phpclass:`SessionHandler` et :phpclass:`SessionHandlerInterface`
sont disponibles. Symfony 2.1 fournit une compatibilité ascendante pour
l'interface :phpclass:`SessionHandlerInterface` afin qu'elle puisse être
utilisée avec PHP 5.3. Cela améliore grandement l'interopérabilité avec
d'autres bibliothèques.

:phpclass:`SessionHandler` est une classe interne à PHP spéciale qui expose
les gestionnaires de sauvegarde natifs au développeur PHP.

Afin de fournir une solution à ceux qui utilisent PHP 5.4, Symfony2 possède
une classe spéciale appelée :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeSessionHandler`
qui sous PHP 5.4, étend `\SessionHandler` et sous PHP 5.3 est juste une
classe de base vide. Cela fournit quelques opportunités intéressantes pour
tirer parti de la fonctionnalité de PHP 5.4 si elle est disponible.

Gestionnaire de Sauvegarde par procuration (« proxy » en anglais)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il existe deux types de gestionnaires de sauvegarde par procuration qui
héritent de :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\AbstractProxy` :
ce sont :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeProxy`
et :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\SessionHandlerProxy`.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage`
injecte automatiquement les gestionnaires de stockage dans un gestionnaire
de sauvegarde par procuration à moins qu'ils soient déjà gérés par un autre.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeProxy`
est utilisée automatiquement sous PHP 5.3 quand des gestionnaires de sauvegarde
PHP natifs sont spécifiés en utilisant les classes `Native*SessionHandler`,
pendant que :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\SessionHandlerProxy`
sera utilisé pour gérer quelconques gestionnaires de sauvegarde personnalisés,
qui implémentent :phpclass:`SessionHandlerInterface`.

Sous PHP 5.4 et supérieur, tous les gestionnaires de session implémentent
:phpclass:`SessionHandlerInterface` incluant les classes `Native*SessionHandler`
qui héritent de :phpclass:`SessionHandler`.

Le mécanisme de procuration (« proxy » en anglais) vous permet d'être impliqué
plus profondément dans les classes de gestionnaire de sauvegarde de session.
Un « proxy » pourrait par exemple être utilisé pour encrypter toute transaction
de session sans avoir aucune connaissance du gestionnaire de sauvegarde
spécifique qui est utilisé.

.. _`php.net/session.customhandler`: http://php.net/session.customhandler
.. _`php.net/session.configuration`: http://php.net/session.configuration
