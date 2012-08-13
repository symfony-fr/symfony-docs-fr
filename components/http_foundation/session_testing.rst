.. index::
   single: HTTP
   single: HttpFoundation, Sessions

Tester avec les Sessions
========================

Symfony2 est architecturé depuis ses propres fondations avec l'objectif
en tête que le code soit testable. Afin de rendre votre code utilisant les
sessions facilement testable, nous fournissons deux mécanismes de simulation
de stockage pour les tests unitaires et fonctionnels.

Tester du code en utilisant des sessions réelles est délicat car le flux
des états dans PHP est global et il n'est pas possible d'avoir plusieurs
sessions concurrentes dans le même processus PHP.

Les moteurs de simulation de stockage simulent le flux d'une session PHP
sans en démarrer une en fait ; ce qui vous permet de tester votre code sans
avoir de complications. Vous pourriez aussi exécuter plusieurs instances
dans le même processus PHP.

Les « drivers » de simulation de stockage ne lisent ni écrivent les variables
globales du système `session_id()` ou `session_name()`. Des méthodes sont
fournies pour simuler cela si nécessaire :

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionStorageInterface::getId`:
  Récupère l'ID de la session.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionStorageInterface::setId`:
  Définit l'ID de la session.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionStorageInterface::getName`:
  Récupère le nom de la session.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionStorageInterface::setName`:
  Définit le nom de la session.

Tester de manière Unitaire
--------------------------

Pour tester unitairement où il n'est pas nécessaire de persister la session,
vous devriez simplement permuter le moteur de stockage par défaut avec
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\MockArraySessionStorage`::

    use Symfony\Component\HttpFoundation\Session\Storage\MockArraySessionStorage;
    use Symfony\Component\HttpFoundation\Session\Session;

    $session = new Session(new MockArraySessionStorage());

Tester de manière Fonctionnelle
-------------------------------

Pour tester de manière fonctionnelle où vous pourriez avoir besoin de persister
les données de session à travers des processus PHP séparés, changer simplement
le moteur de stockage pour être
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\MockFileSessionStorage`::

    use Symfony\Component\HttpFoundation\Session\Session;
    use Symfony\Component\HttpFoundation\Session\Storage\MockFileSessionStorage;

    $session = new Session(new MockFileSessionStorage());
