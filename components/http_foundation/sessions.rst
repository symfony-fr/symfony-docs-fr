.. index::
   single: HTTP
   single: HttpFoundation, Sessions

Gestion de Session
==================

Le Composant HttpFoundation de Symfony2 possède un sous-système de session
très flexible et puissant qui est implémenté de façon à fournir une gestion
de session à travers une interface orientée-objet simple et utilisant une
variété de « drivers » de stockage de session.

.. versionadded:: 2.1
    L'interface :class:`Symfony\\Component\\HttpFoundation\\Session\\SessionInterface`,
    de même que de nombreux autres changements, sont nouveaux depuis Symfony 2.1.

Les sessions sont utilisées via l'implémentation simple de
:class:`Symfony\\Component\\HttpFoundation\\Session\\Session` de l'interface
:class:`Symfony\\Component\\HttpFoundation\\Session\\SessionInterface`.

Rapide exemple::

    use Symfony\Component\HttpFoundation\Session\Session;

    $session = new Session();
    $session->start();

    // définit et récupère des attributs de session
    $session->set('name', 'Drak');
    $session->get('name');

    // définit des messages dits « flash »
    $session->getFlashBag()->add('notice', 'Profile updated');

    // récupère des messages
    foreach ($session->getFlashBag()->get('notice', array()) as $message) {
        echo "<div class='flash-notice'>$message</div>";
    }

.. note::

    Les sessions de Symfony sont implémentées pour remplacer plusieurs
    fonctions PHP natives. Les applications devraient éviter d'utiliser
    ``session_start()``, ``session_regenerate_id()``, ``session_id()``,
    ``session_name()``, et ``session_destroy()`` et utiliser à la place
    les APIs de la section suivante.

.. note::

    Bien qu'il soit recommandé de démarrer une session explicitement, une
    session va en fait être démarrée sur demande, ce qui veut dire, si
    toute requête d'une session est faite pour lire/écrire des données de
    session.

.. warning::

    Les sessions de Symfony sont incompatibles avec la directive PHP ini
    ``session.auto_start = 1``. Cette directive devrait être désactivée
    dans le fichier ``php.ini``, dans les directives du serveur web ou
    dans un fichier ``.htaccess``.

API de Session
~~~~~~~~~~~~~~

La classe :class:`Symfony\\Component\\HttpFoundation\\Session\\Session`
implémente :class:`Symfony\\Component\\HttpFoundation\\Session\\SessionInterface`.

La classe :class:`Symfony\\Component\\HttpFoundation\\Session\\Session`
possède une API simple comme suit (divisée en différents groupes).

Flux de fonctionnement d'une session

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::start`:
  Démarre la session - ne pas utiliser ``session_start()`` ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::migrate`:
  Regénère l'ID de la session - ne pas utiliser ``session_regenerate_id()``.
  Cette méthode peut optionnellement changer la durée de vie du nouveau
  cookie qui va être émis lors de l'appel de cette méthode ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::invalidate`:
  Supprime toutes les données de la session et regénère l'ID de la session - ne pas
  utiliser ``session_destroy()``.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getId`:
  Récupère l'ID de la session - ne pas utiliser ``session_id()`` ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::setId`:
  Définit l'ID de la session - ne pas utiliser ``session_id()`` ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getName`:
  Récupère le nom de la session - ne pas utiliser ``session_name()`` ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::setName`:
  Définit le nom de la session - ne pas utiliser ``session_name()`` ;

Attributs de session

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::set`:
  Définit un attribut par clé ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::get`:
  Récupère un attribut par clé ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::all`:
  Récupère tous les attributs sous forme de tableau de paires clé => valeur ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::has`:
  Retourne « true » si l'attribut existe ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::keys`:
  Retourne un tableau contenant les clés des attributs ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::replace`:
  Définit plusieurs attributs en une fois : prend un tableau de clés et
  définit chaque paire de clé => valeur ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::remove`:
  Supprime un attribut par clé ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::clear`:
  Supprime tous les attributs.

Les attributs sont stockés en interne dans un « Bag » (« sac » en français),
un objet PHP qui agit comme un tableau. Quelques méthodes existent pour
gérer ce « Bag » :

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::registerBag`:
  Enregistre une :class:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface` ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getBag`:
  Récupère une :class:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface`
  par nom de « bag » ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getFlashBag`:
  Récupère la :class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface`.
  Ceci est juste un raccourci pour plus de commodité.

Métadonnées de session

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getMetadataBag`:
  Récupère le :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\MetadataBag`
  qui contient des informations à propos de la session.

Gestion de données de session
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La gestion de session de PHP requiert l'utilisation de la variable super-globale
``$_SESSION``, cependant, cela interfère d'une certaine manière avec la
testabilité du code et l'encapsulation dans un paradigme POO. Pour aider
à résoudre ce soucis, Symfony2 utilise des « bags de session » (« sacs
de session » en français) liés à la session pour encapsuler un ensemble
de données spécifique d'« attributs » ou de « messages flash ».

Cette approche diminue aussi la « pollution » de l'espace de noms dans
la variable super-globale ``$_SESSION`` car chaque sac stocke toutes ses
données sous un espace de noms unique. Cela permet à Symfony2 de co-exister
en toute quiétude avec d'autres applications ou bibliothèques qui pourrait
utiliser la varible super-globale ``$_SESSION`` et toutes ses données restent
entièrement compatibles avec la gestion de session de Symfony2.

Symfony2 fournit deux sortes de sacs de stockage, avec deux implémentations
séparées. Tout est écrit à l'aide d'interfaces donc vous pourriez étendre
ou créer votre propre sac si nécessaire.

:class:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface`
possède l'API suivante qui est destinée principalement à des fins internes :

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface::getStorageKey`:
  Retourne la clé sous laquelle le sac va stocker au final son tableau
  dans la variable super-globale ``$_SESSION`` ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface::initialize`:
  Cette méthode est appelée en interne par les classes de stockage de session
  de Symfony2 pour lier les données du sac à la session ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface::getName`:
  Retourne le nom du sac de session.

Attributs
~~~~~~~~~

Le but des « sacs » implémentant l'interface
:class:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface`
est de gérer le stockage des attributs de session. Cela peut inclure des
choses comme l'ID utilisateur, les paramètres concernant le login tel
« Se souvenir de moi » ou d'autres informations à propos de l'état de l'utilisateur.

* :class:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBag`
  Cela est l'implémentation standard par défaut ;

* :class:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\NamespacedAttributeBag`
  Cette implémentation permet aux attributs d'être stockés dans un espace
  de noms structuré.

Tout système de stockage `clé => valeur` est limité quant aux données complexes
qui peuvent être stockées puisque chaque clé doit être unique. Vous pouvez
néanmoins utiliser les espaces de noms en introduisant une convention de
nommage pour les clés afin que les différentes parties de votre application
puissent fonctionner sans soucis. Par exemple, `module1.foo` et `module2.foo`.
Cependant, parfois, cela n'est pas très pratique quand les données des attributs
sont sous la forme d'un tableau, par exemple pour un ensemble de jetons.
Dans ce cas, gérer le tableau devient un fardeau car vous devez récupérer
le tableau puis le traiter et le stocker de nouveau::

    $tokens = array('tokens' => array('a' => 'a6c1e0b6',
                                      'b' => 'f4a7b1f3'));

Du coup, n'importe quel traitement similaire à ce que vous avez ci-dessus pourrait
vite devenir moche, même en ajoutant simplement un jeton au tableau::

    $tokens = $session->get('tokens');
    $tokens['c'] = $value;
    $session->set('tokens', $tokens);

Avec un espace de noms structuré, la clé peut être traduite en une structure
en tableau comme ceci en utilisant un caractère d'espace de noms (par défaut
`/`)::

    $session->set('tokens/c', $value);

De cette manière, vous pouvez facilement accéder à une clé du tableau stocké.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface`
possède une API simple :

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::set`:
  Définit un attribut par clé ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::get`:
  Récupère un attribut par clé ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::all`:
  Récupère tous les attributs en tant que tableau « clé => valeur » ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::has`:
  Retourne « true » si l'attribut existe ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::keys`:
  Retourne un tableau des clés d'attributs stockées ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::replace`:
  Définit plusieurs attributs en une fois : prend un tableau de clés et
  définit chaque paire de clé => valeur ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::remove`:
  Supprime un attribut par sa clé ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::clear`:
  Supprime le sac.

Messages flash
~~~~~~~~~~~~~~

Le but de l'interface :class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface`
est de fournir une manière de définir et récupérer des messages par session.
Le flux standard pour les messages flash serait la définition du message
dans une requête, et l'affichage après une redirection vers une autre page.
Par exemple, un utilisateur soumet un formulaire qui passe par un contrôleur
de mise à jour, et ensuite, à la fin de ce contrôleur, ce dernier redirige
la page vers celle mise à jour ou vers une page d'erreur. Le message flash
défini dans la requête de la page précédente serait donc affiché immédiatement
dans la page suivante de cette session. Cela n'est qu'un exemple d'utilisation
des messages flash.

* :class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\AutoExpireFlashBag`
  Cette implémentation de la définition des messages flash ne sera disponible
  que durant l'affichage de la prochaine page. Ces messages vont expirer
  de manière automatique suivant s'ils ont été recupérés ou non ;

* :class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBag`
  Dans cette implémentation, les messages vont rester dans la session jusqu'à
  ce qu'ils soient explicitement récupérés ou supprimés. Cela rend donc
  possible l'utilisation du mécanisme de cache ESI.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface`
possède une API simple :

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::add`:
  Ajoute un message flash à la pile du type spécifié ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::set`:
  Définit des flashs par type ; cette méthode prend soit un message « unique »
  en tant que ``chaîne de caractères`` ou plusieurs messages dans un ``tableau`` ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::get`:
  Récupère les flashs par type et supprime ces derniers du sac ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::setAll`:
  Définit tous les flashs ; accepte un tableau à clé de tableaux ``type => array(messages)`` ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::all`:
  Récupère tous les flashs (en tant que tableau à clé de tableaux) et supprime
  les flashs du sac ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::peek`:
  Récupère tous les flashs par type (lecture seulement) ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::peekAll`:
  Récupère tous les flashs (lecture seulement) en tant que tableau à clé
  de tableaux ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::has`:
  Retourne « true » si le type existe, « false » sinon ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::keys`:
  Retourne un tableau des types de flash stockés ;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::clear`:
  Supprime le sac.

Pour des applications simples, il est généralement suffisant d'avoir un
message flash par type, par exemple une notification de confirmation après
qu'un formulaire ait été soumis. Cependant, les messages flash sont stockés
dans un tableau à clé par ``$type`` de flash qui signifie que votre application
peut émettre plusieurs messages pour un type donné. Cela permet à l'API
d'être utilisée pour un système de messages plus complexe dans votre application.

Exemples de définition de plusieurs flashs::

    use Symfony\Component\HttpFoundation\Session\Session;

    $session = new Session();
    $session->start();

    // ajoute des messages flash
    $session->getFlashBag()->add('warning', 'Your config file is writable, it should be set read-only');
    $session->getFlashBag()->add('error', 'Failed to update name');
    $session->getFlashBag()->add('error', 'Another error');

Afficher les messages flash pourrait ressembler à quelque chose comme ca :

Affiche simplement un type de message::

    // affiche les avertissements
    foreach ($session->getFlashBag()->get('warning', array()) as $message) {
        echo "<div class='flash-warning'>$message</div>";
    }

    // affiche les erreurs
    foreach ($session->getFlashBag()->get('error', array()) as $message) {
        echo "<div class='flash-error'>$message</div>";
    }

Méthode compacte de traitement de l'affichage de tous les flashs en une
seule fois::

    foreach ($session->getFlashBag()->all() as $type => $messages) {
        foreach ($messages as $message) {
            echo "<div class='flash-$type'>$message</div>\n";
        }
    }
