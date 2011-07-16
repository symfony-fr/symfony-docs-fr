L'API stable de Symfony2
========================

L'API stable de Symfony2 is un sous-ensemble des toutes les méthodes publiques
de Symfony2 (composants et bundles core) qui partagent les propriétés suivantes:

* le namespace et le nom de la classe ne changera pas,
* le nom de la méthode ne changera pas,
* la signatude de la méthode (arguments et valeur de retour) ne changera pas,
* le rôle et la responsbilité de ce que la méthode fait ne changera pas ('''NOTE: pas satisfait de la traduction''')

L'implémentation elle-même peut changer. La seule raison valable pour un changement 
de l'API stable de Symfony2 serait pour corriger un problème de sécurité.

..note::

    L'API stable est basé sur un principe de liste blanche ("whitelist"). Donc tout ce qui n'est pas 
    listé dans ce document ne fait pas parti de l'API stable.

..note::

    Cette liste est en cours de définition et la liste définitive sera finalisée lorsque
    la version finale de Symfony2 sera publiée. Si vous considérez que des méthodes
    devraient être incluses à cette liste, vous pouvez démarrer une discussion sur 
    la mailing-list Symfony developer.

..note::

    Chaque méthode faisant partie de l'API stable est signalée sur le site de 'API
    de Symony2 (au moyen de l'annotation ``@stable``).

.. tip::

    Chaque bundle tiers devrait aussi publier sa propre API stable.
    
HttpKernel Component
--------------------

* HttpKernelInterface:::method:`Symfony\\Component\\HttpKernel\\HttpKernelInterface::handle`
