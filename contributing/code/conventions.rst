Conventions
===========

Le document :doc:`standards` décrit les conventions syntaxiques définies pour les
projets Symfony2 et les bundles internes ou tiers. Ce document décrit
les standards et conventions utilisés dans le coeur du framework afin de lui 
apporter consistence et prédictibilité. Vous êtes encouragés à les suivre
dans votre propre code, mais n'êtes pas obligé.

Nom des Méthodes
----------------

Quand un objet a une relation many « principale » avec d'autres « choses » (objets,
paramètres, ...), les noms des méthodes sont normalisés :

  * ``get()``
  * ``set()``
  * ``has()``
  * ``all()``
  * ``replace()``
  * ``remove()``
  * ``clear()``
  * ``isEmpty()``
  * ``add()``
  * ``register()``
  * ``count()``
  * ``keys()``

L'utilisation de ces méthodes est seulement autorisé quand il est certain que 
la relation décrite est la relation principale:

* un ``CookieJar`` possède de nombreux objets ``Cookie``;

* un service ``Container`` a de nombreux services et de nombreux paramètres
  (comme le service est sa relation principale, la convention de nommage est 
  utilisé pour cette relation);

* une ``commande`` de Console a de nombreux arguments et de nombreuses options. Il
  n'existe pas de relation principale, la convention de nommage ne s'applique donc
  pas.

Pour les relations où la convention de nommage ne s'applique
pas, les méthodes suivantes doivent être utilisées 
(``XXX`` est le nom de la chose en relation):

+----------------------+-------------------+
| Relation principale  | Autre Relations   |
+======================+===================+
| ``get()``            | ``getXXX()``      |
+----------------------+-------------------+
| ``set()``            | ``setXXX()``      |
+----------------------+-------------------+
| n/a                  | ``replaceXXX()``  |
+----------------------+-------------------+
| ``has()``            | ``hasXXX()``      |
+----------------------+-------------------+
| ``all()``            | ``getXXXs()``     |
+----------------------+-------------------+
| ``replace()``        | ``setXXXs()``     |
+----------------------+-------------------+
| ``remove()``         | ``removeXXX()``   |
+----------------------+-------------------+
| ``clear()``          | ``clearXXX()``    |
+----------------------+-------------------+
| ``isEmpty()``        | ``isEmptyXXX()``  |
+----------------------+-------------------+
| ``add()``            | ``addXXX()``      |
+----------------------+-------------------+
| ``register()``       | ``registerXXX()`` |
+----------------------+-------------------+
| ``count()``          | ``countXXX()``    |
+----------------------+-------------------+
| ``keys()``           | n/a               |
+----------------------+-------------------+

.. note::

   Notez que si « setXXX » et « replaceXXX » sont très similaires,  il y a tout de
   même une différence notable : « setXXX » peut remplacer, ou ajouter de nouveaux 
   éléments à une relation. Avec « replaceXXX », il est interdit d'ajouter des éléments,
   et une exception est lancée si l'élément identifié n'existe pas.