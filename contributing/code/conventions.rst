Conventions
===========

Le document :doc:`standards` décrit les coventions syntaxiques définis pour les
projets Symfony2 et les bundles internes ou de tierce partie. ce document décrit
les standards et conventions utilisés dans le coeur du framework afin de lui 
apporter consistence et prédictibilité. Vous êtes encouragés à suivre ceux-ci
dans votre propre code, mais n'y êtes pas tenu.

Nom des Méthodes
----------------

Quand un objet a une relation conventionnelle avec d'autres conteneurs (objets,
paramètres, ...), les noms des méthodes utilisent un schéma pré-établi:

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

* un ``CookieJar`` possède de nombreux objects ``Cookie``;

* un service ``Container`` a de nombreux services et de nombreux paramètres
  (comme le service est sa relation principale, nous utilisons cette convention
  pour cette relation);

* un Console ``Input`` a de nombreux arguments et de nombreuses options. Il
  n'existe pas de relation principale, la convention de nom ne s'applique donc
  pas.

Pour de nombreuses relations dans lesquelles la convention de nom ne s'applique
pas, les méthodes suivantes doivent être utilisées en remplacement 
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

   Notez que si "setXXX" et "replaceXXX" sont très similairs,  il y'a tout de
   même une différence notable: "setXXX" peut remplacer, ou ajouter de nouveaux 
   éléments à une relation. "replaceXXX" à l'interdiction d'ajouter des éléments,
   et devrait émettre une exception si l'élément identifié n'existe pas.
