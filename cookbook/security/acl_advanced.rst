.. index::
   single: Sécurité; Concepts d'ACL avancés

Comment utiliser les concepts d'ACL avancés
===========================================

Le but de ce chapitre est de donner une vision plus profonde du système des
ACL (« Liste de Contrôle d'Accès » en français), et aussi d'expliquer certaines
décisions prises en ce qui concerne sa conception.

Concepts d'Architecture
-----------------------

Les capacités de sécurité de l'instance objet de Symfony2 sont basées sur le
concept d'une « Access Control List ». Chaque **instance** d'un objet domaine a sa
propre ACL. L'instance ACL contient une liste détaillée des « Access Control
Entries » (ACEs ou « Entrées de Contrôle d'Accès » en français) qui sont
utilisées pour prendre les décisions d'accès. Le système d'ACL de Symfony2 se
concentre sur deux objectifs :

- fournir un moyen de récupérer de manière efficace un grand nombre d'ACLs/ACEs
  pour vos objets domaine, et de les modifier ;
- fournir un moyen de prendre les décisions facilement quant à savoir si une
  personne est autorisée à effectuer une action sur un objet domaine ou non.

Comme spécifié dans le premier point, l'une des principales facultés du système
ACL de Symfony2 est de fournir une manière très performante de récupérer des
ACLs/ACEs. Ceci est extrêmement important sachant que chaque ACL pourrait avoir
plusieurs ACEs, et hériter d'une autre ACL à la manière d'une structure en arbre.
Par conséquent, aucun ORM n'est utilisé mais l'implémentation par défaut intéragit
avec votre connexion en utilisant directement le DBAL de Doctrine.

Identités d'Objet
~~~~~~~~~~~~~~~~~

Le système ACL est complètement découplé de vos objets domaine. Ils ne doivent
même pas être stockés dans la même base de données, ou sur le même serveur.
Pour pouvoir accomplir ce découplage, vos objets sont représentés dans le
système ACL par des objets d'identité d'objet. Chaque fois que vous voulez
récupérer une ACL pour un objet domaine, le système ACL va d'abord créer
une identité d'objet pour votre objet domaine, et va ensuite passer cette
identité d'objet au fournisseur d'ACL pour un traitement ultérieur.

Identités de Sécurité
~~~~~~~~~~~~~~~~~~~~~

Ceci est similaire à l'identité d'objet, mais représente un utilisateur, ou
un rôle dans votre application. Chaque rôle ou utilisateur possède sa
propre identité de sécurité.

Structure de Table dans la Base de Données
------------------------------------------

L'implémentation par défaut utilise cinq tables de base de données qui sont
listées ci-dessous. Les tables sont classées par nombre de lignes, de celle
contenant le moins de lignes à celle en contenant le plus dans une application 
classique :

- *acl_security_identities* : Cette table enregistre toutes les identités
  de sécurité (SID) qui détiennent les ACEs. L'implémentation par défaut
  est fournie avec deux identités de sécurité : ``RoleSecurityIdentity``, et
  ``UserSecurityIdentity`` ;
- *acl_classes* : Cette table fait correspondre les noms de classe avec
  un id unique qui peut être référencé depuis d'autres tables ;
- *acl_object_identities* : Chaque ligne dans cette table représente une
  unique instance d'objet domaine ;
- *acl_object_identity_ancestors* : Cette table nous autorise à déterminer
  tous les ancêtres d'une ACL de manière très efficace ;
- *acl_entries* : Cette table contient toutes les ACEs. C'est typiquement la
  table avec le plus de lignes. Elle peut en contenir des dizaines de millions
  sans impacter de façon significative les performances.

Portée des « Access Control Entries »
-------------------------------------

Les entrées de contrôle d'accès peuvent avoir différentes portées dans lesquelles
elles s'appliquent. Dans Symfony2, il existe principalement deux portées
différentes :

- Portée de la Classe : Ces entrées s'appliquent à tous les objets ayant la
  même classe.
- Portée de l'Objet : Ceci est la portée utilisée dans le chapitre
  précédent, et elle s'applique uniquement à un objet spécifique.

Parfois, vous aurez besoin d'appliquer une ACE uniquement sur le champ
spécifique d'un objet. Supposons que vous voulez que l'ID soit uniquement
visible par un administrateur mais pas par votre service client. Pour
solutionner ce problème commun, deux sous-portées supplémentaires ont
été ajoutées :

- Portée d'un Champ de Classe : Ces entrées s'appliquent à tous les objets
  ayant la même classe, mais uniquement à un champ spécifique de ces objets.
- Portée d'un Champ d'Objet : Ces entrées s'appliquent à un objet spécifique,
  et uniquement à un champ spécifique de cet objet.

Décisions de pré-autorisation
-----------------------------

Pour les décisions de pré-autorisation, que ce soit des décisions prises avant
quelconque méthode ou bien une action sécurisée qui est invoquée, le service 
éprouvé « AccessDecisionManager » est utilisé. l'« AccessDecisionManager » 
est aussi utilisé pour connaître les décisions d'autorisation basées sur des rôles.
Comme les rôles, le système d'ACL ajoute plusieurs nouveaux attributs qui
pourraient être utilisés pour vérifier différentes permissions.

Table de Permission Intégrée
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

+------------------+----------------------------+-----------------------------+
| Attribut         | Signification              | Masques Binaires            |
+==================+============================+=============================+
| VIEW             | Si quelqu'un est autorisé  | VIEW, EDIT, OPERATOR,       |
|                  | à voir l'objet domaine.    | MASTER, or OWNER            |
+------------------+----------------------------+-----------------------------+
| EDIT             | Si quelqu'un est autorisé  | EDIT, OPERATOR, MASTER,     |
|                  | à effectuer des changements| or OWNER                    |
|                  | sur l'objet domaine.       |                             |
+------------------+----------------------------+-----------------------------+
| CREATE           | Si quelqu'un est autorisé  | CREATE, OPERATOR, MASTER,   |
|                  | à créer l'objet domaine.   | or OWNER                    |
+------------------+----------------------------+-----------------------------+
| DELETE           | Si quelqu'un est autorisé  | DELETE, OPERATOR, MASTER,   |
|                  | à supprimer l'objet        | or OWNER                    |
|                  | domaine.                   |                             |
+------------------+----------------------------+-----------------------------+
| UNDELETE         | Si quelqu'un est autorisé  | UNDELETE, OPERATOR, MASTER, |
|                  | à restaurer un objet       | or OWNER                    |
|                  | domaine précédemment       |                             |
|                  | supprimé.                  |                             |
+------------------+----------------------------+-----------------------------+
| OPERATOR         | Si quelqu'un est autorisé  | OPERATOR, MASTER, or OWNER  |
|                  | à effectuer toutes les     |                             |
|                  | actions ci-dessus.         |                             |
+------------------+----------------------------+-----------------------------+
| MASTER           | Si quelqu'un est autorisé  | MASTER, or OWNER            |
|                  | à effectuer toutes les     |                             |
|                  | actions ci-dessus, et en   |                             |
|                  | plus a le droit d'affecter |                             |
|                  | n'importe laquelle d'entre |                             |
|                  | elles à quelqu'un d'autre. |                             |
+------------------+----------------------------+-----------------------------+
| OWNER            | Si quelqu'un possède       | OWNER                       |
|                  | l'objet domaine. Un        |                             |
|                  | propriétaire peut effectuer|                             |
|                  | n'importe laquelle des     |                             |
|                  | actions ci-dessus *et*     |                             |
|                  | affecter les permissions   |                             |
|                  | master et owner.           |                             |
+------------------+----------------------------+-----------------------------+

Attributs de Permission vs. Masques Binaires de Permission
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les attributs sont utilisés par l'« AccessDecisionManager », tout comme
les rôles. Souvent, ces attributs représentent en fait une agrégation de masques
binaires. D'un autre côté, les masques binaires sous forme d'entier sont utilisés
par le système d'ACL en interne pour stocker de manière efficace les permissions
de vos utilisateurs dans la base de données, et pour effectuer des
vérifications en utilisant des opérations sur les masques binaires extrêmement
rapides.

Extensibilité
~~~~~~~~~~~~~

La table de permissions ci-dessus n'est en rien statique, et pourrait
théoriquement être complètement remplacée. Cependant, elle devrait couvrir
la plupart des problèmes que vous pourriez rencontrer, et pour des raisons
d'intéropérabilité avec d'autres bundles, vous êtes encouragé à conserver
les significations initialement prévues pour ces permissions.

Décisions de post-autorisation
------------------------------

Les décisions de post-autorisation sont effectuées après qu'une méthode
sécurisée a été invoquée, et impliquent typiquement l'objet domaine qui
est retourné par une telle méthode. Après invocations, les fournisseurs
permettent aussi de modifier, ou de filtrer l'objet domaine avant qu'il
ne soit retourné.

A cause de limitations actuelles du langage PHP, il n'y a pas de
fonctionnalités de post-autorisation implémentées dans le composant
coeur « Security ». Néanmoins, il y a un bundle expérimental appelé
JMSSecurityExtraBundle_ qui ajoute ces fonctionnalités. Lisez sa
documentation pour avoir plus d'informations pour comprendre comment ceci
est réalisé.

Processus pour connaître les décisions d'autorisation
-----------------------------------------------------

La classe ACL fournit deux méthodes pour déterminer si une identité de
sécurité possède les masques binaires requis, ``isGranted`` et
``isFieldGranted``. Lorsque l'ACL reçoit une requête d'autorisation à
travers l'une de ces méthodes, elle délègue cette requête à une
implémentation de « PermissionGrantingStrategy ». Cela vous permet de remplacer
la manière dont les décisions d'accès sont atteintes sans modifier la
classe ACL elle-même.

La « PermissionGrantingStrategy » vérifie en premier toutes les ACEs de vos
portées d'objet ; si aucune n'est applicable, les ACEs de vos portées de classe
vont être vérifiées, et si aucune n'est applicable, alors le processus va être
répété avec les ACEs du parent de l'ACL. Si aucun parent de l'ACL n'existe, une
exception sera lancée.

.. _JMSSecurityExtraBundle: https://github.com/schmittjoh/JMSSecurityExtraBundle
