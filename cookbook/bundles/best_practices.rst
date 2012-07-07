.. index::
   single: Bundle; Bonnes pratiques

Structure de Bundle et Bonnes Pratiques
===========================================

Un bundle est un répertoire qui a une structure bien définie et qui peut
héberger à peu près tout : des classes aux contrôleurs en passant par les ressources
web. Même si les bundles sont très flexibles, vous devriez suivre quelques unes
des bonnes pratiques si vous voulez les distribuer.

.. index::
   pair: Bundles; Conventions de nommage

.. _bundles-naming-conventions:

Nom du Bundle
-------------

Un bundle est aussi un espace de noms PHP. Ce dernier doit suivre les
`standards`_ d'intéropérabilité technique pour les espaces de noms PHP 5.3
et les noms de classes : il commence par un segment « vendor », suivi par
zéro ou plusieurs segments catégories, et il se termine par le nom raccourci
de l'espace de noms, qui doit finir par un suffixe ``Bundle``.

Un espace de noms devient un bundle aussitôt que vous lui ajoutez une classe
bundle. Le nom de la classe bundle doit suivre ces règles simples :

* Utiliser uniquement des caractères alphanumériques et des tirets bas (« underscore »
  en anglais) ;
* Utiliser un nom en notation dite « CamelCase » ;
* Utiliser un nom court et descriptif (pas plus de 2 mots) ;
* Préfixer le nom avec la concaténation du « vendor » (et optionnellement
  l'espace de noms de la catégorie) ;
* Suffixer le nom avec ``Bundle``.

Vous trouverez ci-dessous des espaces de noms de bundle et des noms
de classes valides :

+-----------------------------------+--------------------------+
| Espace de noms                    | Nom de la Classe Bundle  |
+===================================+==========================+
| ``Acme\Bundle\BlogBundle``        | ``AcmeBlogBundle``       |
+-----------------------------------+--------------------------+
| ``Acme\Bundle\Social\BlogBundle`` | ``AcmeSocialBlogBundle`` |
+-----------------------------------+--------------------------+
| ``Acme\BlogBundle``               | ``AcmeBlogBundle``       |
+-----------------------------------+--------------------------+

Par convention, la méthode ``getName()`` de la classe bundle devrait retourner
le nom de la classe.

.. note::

    Si vous partagez publiquement votre bundle, vous devez utiliser le nom
    de la classe bundle comme nom de dépôt (``AcmeBlogBundle`` et non pas
    ``BlogBundle`` par exemple).

.. note::

    Les Bundles du coeur de Symfony2 ne préfixent pas la classe Bundle avec
    ``Symfony`` et ajoutent toujours un sous-espace de noms ``Bundle`` ;
    par exemple : :class:`Symfony\\Bundle\\FrameworkBundle\\FrameworkBundle`.

Chaque bundle possède un alias, qui est la version raccourcie en miniscules du
nom du bundle en utilisant des tirets bas (``acme_hello`` pour ``AcmeHelloBundle``,
ou ``acme_social_blog`` pour ``Acme\Social\BlogBundle`` par exemple). Cet alias
est utilisé pour renforcer l'unicité à l'intérieur d'un bundle (voir ci-dessous
pour des exemples d'utilisation).

Structure de Répertoires
------------------------

La structure basique du répertoire d'un bundle ``HelloBundle`` doit être
comme suit :

.. code-block:: text

    XXX/...
        HelloBundle/
            HelloBundle.php
            Controller/
            Resources/
                meta/
                    LICENSE
                config/
                doc/
                    index.rst
                translations/
                views/
                public/
            Tests/

Le(s) répertoire(s) ``XXX`` reflète(nt) la structure de l'espace de noms
du bundle.

Les fichiers suivants sont obligatoires :

* ``HelloBundle.php`` ;
* ``Resources/meta/LICENSE``: La licence complète pour le code ;
* ``Resources/doc/index.rst``: Le fichier racine pour la documentation du bundle.

.. note::

    Ces conventions assurent que les outils automatisés puissent compter
    sur cette structure par défaut pour travailler.

La profondeur des sous-répertoires devrait être réduite au minimum pour les
classes et fichiers les plus utilisés (2 niveaux au maximum). Plus de niveaux
peuvent être définis pour les fichiers non-stratégiques et moins utilisés.

Le répertoire du bundle est en lecture seule. Si vous avez besoin d'écrire des
fichiers temporaires, stockez-les dans le dossier ``cache/` ou ``log/`` de
l'application hébergeant votre bundle. Des outils peuvent générer des fichiers
dans la structure du répertoire du bundle, mais uniquement si les fichiers
générés vont faire partie du répertoire.

Les classes et fichiers suivants ont des emplacements spécifiques :

+-------------------------------------+-----------------------------+
| Type                                | Répertoire                  |
+=====================================+=============================+
| Commandes                           | ``Command/``                |
+-------------------------------------+-----------------------------+
| Contrôleurs                         | ``Controller/``             |
+-------------------------------------+-----------------------------+
| Extensions du Conteneur de Services | ``DependencyInjection/``    |
+-------------------------------------+-----------------------------+
| Listeners d'Evènements              | ``EventListener/``          |
+-------------------------------------+-----------------------------+
| Configuration                       | ``Resources/config/``       |
+-------------------------------------+-----------------------------+
| Ressources Web                      | ``Resources/public/``       |
+-------------------------------------+-----------------------------+
| Fichiers de traduction              | ``Resources/translations/`` |
+-------------------------------------+-----------------------------+
| Templates                           | ``Resources/views/``        |
+-------------------------------------+-----------------------------+
| Tests Unitaires et Fonctionnels     | ``Tests/``                  |
+-------------------------------------+-----------------------------+

Classes
-------

La structure du répertoire du bundle est utilisée en tant que hiérarchie
d'espace de noms. Par exemple, un contrôleur ``HelloController`` est stocké
dans ``Bundle/HelloBundle/Controller/HelloController.php`` et le nom complet
qualifié de la classe est ``Bundle\HelloBundle\Controller\HelloController``.

Tous les fichiers et classes doivent suivre les :doc:`standards
</contributing/code/standards>` de codage de Symfony2 (« coding standards »
en anglais).

Certaines classes devraient être vues comme des façades et donc être aussi
courtes que possible, comme les « Commands », « Helpers », « Listeners » et
« Controllers ».

Les classes se connectant au dispatcher (« répartiteur » en français)
d'évènements devraient être suffixées avec ``Listener``.

Les classes d'exceptions devraient être stockées dans un sous-espace
de noms ``Exception``.

Vendors
-------

Un bundle ne doit pas embarquer de bibliothèques PHP tierces. Il devrait
compter sur le chargement automatique (« autoloading » en anglais) standard
de Symfony2 à la place.

Un bundle ne devrait pas embarquer de bibliothèques tierces écrites en JavaScript,
CSS, ou quelconque autre langage.

Tests
-----

Un bundle devrait venir avec un ensemble de tests écrits avec PHPUnit et
stockés dans le répertoire ``Tests/``. Les tests devraient suivre les principes
suivants :

* La suite de tests doit être exécutable avec une simple commande ``phpunit``
  lancée depuis une application ;
* Les tests fonctionnels devraient être utilisés uniquement pour tester la
  sortie de la réponse et quelques informations de profilage si vous en avez ;
* La couverture du code devrait couvrir au moins 95% de tout votre code.

.. note::

   Une suite de test ne doit pas contenir de script ``AllTests.php``, mais doit
   reposer sur l'existence d'un fichier ``phpunit.xml.dist``.

Documentation
-------------

Toutes les classes et fonctions doivent contenir une PHPDoc complète.

Une documentation complète devrait aussi être fournie dans le format
:doc:`reStructuredText</contributing/documentation/format>`, dans le
répertoire ``Resources/doc/`` ; le fichier ``Resources/doc/index.rst``
est l'unique fichier obligatoire et doit être le point d'entrée de la
documentation.

Contrôleurs
-----------

En tant que bonne pratique, les contrôleurs dans un bundle prévu pour être
distribué à d'autres ne doivent pas étendre la classe de base
:class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`.
Ils peuvent implémenter
:class:`Symfony\\Component\\DependencyInjection\\ContainerAwareInterface` ou
étendre :class:`Symfony\\Component\\DependencyInjection\\ContainerAware` à
la place.

.. note::

    Si vous jetez un oeil aux méthodes de la classe
    :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`,
    vous verrez qu'elles ne sont que des raccourcis pratiques pour faciliter
    la courbe d'apprentissage.

Routage
-------

Si le bundle fournit des routes, elles doivent être préfixées avec l'alias
du bundle. Par exemple, pour un « AcmeBlogBundle », toutes les routes doivent
être préfixées avec ``acme_blog_``.

Templates
---------

Si un bundle fournit des templates, ils doivent utiliser Twig. Un bundle ne
doit pas fournir de « layout » principal, excepté s'il fournit une application
entièrement fonctionnelle.

Fichiers de Traduction
----------------------

Si un bundle fournit des traductions de messages, ces dernières doivent être
définies au format XLIFF ; le domaine devrait être nommé après le nom du
bundle (``bundle.hello``).

Un bundle ne doit pas « écraser » les messages existants venant d'un autre bundle.

Configuration
-------------

Pour fournir plus de flexibilité, un bundle peut procurer des paramètres
configurables en utilisant les mécanismes intégrés de Symfony2.

Pour des paramètres de configuration simples, comptez sur les entrées par défaut
de ``parameters`` de la configuration de Symfony2. Les paramètres Symfony2 sont
de simples paires clé/valeur ; une valeur étant n'importe quelle valeur PHP valide.
Chaque nom de paramètre devrait commencer avec l'alias du bundle, bien que ceci
ne soit qu'une suggestion de bonne pratique. Le reste du nom du paramètre va
utiliser un point (``.``) pour séparer les différentes parties (par exemple :
``acme_hello.email.from``).

L'utilisateur final peut fournir des valeurs dans différents types de fichier de
configuration :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            acme_hello.email.from: fabien@example.com

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="acme_hello.email.from">fabien@example.com</parameter>
        </parameters>

    .. code-block:: php

        // app/config/config.php
        $container->setParameter('acme_hello.email.from', 'fabien@example.com');

    .. code-block:: ini

        [parameters]
        acme_hello.email.from = fabien@example.com

Récupérez les paramètres de configuration dans votre code depuis le
conteneur::

    $container->getParameter('acme_hello.email.from');

Même si ce mécanisme est assez simple, vous êtes grandement encouragé à utiliser
la configuration sémantique décrite dans le cookbook.

.. note::

    Si vous définissez des services, ils devraient aussi être préfixés avec
    l'alias du bundle.

En savoir plus grâce au Cookbook
--------------------------------

* :doc:`/cookbook/bundles/extension`

.. _standards: http://symfony.com/PSR0
