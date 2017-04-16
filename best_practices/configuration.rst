Configuration
=============

La configuration implique généralement différentes parties de l'application (comme
l'infrastructure et la sécurité) et différents environnements (développement, production).
C'est pourquoi Symfony recommande de diviser la configuration de l'application en trois
parties.

Configuration liée à l'infrastructure
-------------------------------------

.. best-practice::

    Définissez les options de configuration liée à l'infrastructure dans
    le fichier ``app/config/parameters.yml``.

Le fichier par défaut ``parameters.yml`` suit cette recommandation et défini les
options relatives à la base de données et au serveur de mail :

.. code-block:: yaml

    # app/config/parameters.yml
    parameters:
        database_driver:   pdo_mysql
        database_host:     127.0.0.1
        database_port:     ~
        database_name:     symfony
        database_user:     root
        database_password: ~

        mailer_transport:  smtp
        mailer_host:       127.0.0.1
        mailer_user:       ~
        mailer_password:   ~

        # ...

Ces options ne sont pas définies dans le fichier ``app/config/config.yml`` car
elles n'ont pas de rapport avec le comportement de l'application. En d'autres termes,
votre application ne se soucie pas de l'emplacement de la base de données ou des
droits permettant d'y avoir accès, tant que la base de données est bien configurée.

Paramètres standards
~~~~~~~~~~~~~~~~~~~~

.. best-practice::

    Définissez tous les paramètres de votre application dans le fichier
    ``app/config/parameters.yml.dist``.

Depuis la version 2.3, Symfony inclus un fichier de configuration appelé
``parameters.yml.dist``, qui stocke la liste des paramètres de configuration
standard de votre application.

Chaque fois qu'un nouveau paramètre de configuration est défini pour votre application,
vous devriez également l'ajouter à ce fichier et envoyer cette modification à votre
gestionnaire de source. Ensuite, à chaque fois qu'un développeur met à jour le projet
ou le déploie sur un serveur, Symfony vérifiera s'il y a des différences entre le fichier 
standard ``parameters.yml.dist`` et votre fichier local ``parameters.yml``. S'il existe 
une différence, Symfony vous demandera d'indiquer une valeur pour le nouveau paramètre
et l'ajoutera à votre fichier local ``parameters.yml``.

Configuration liée à l'application
----------------------------------

.. best-practice::

    Définissez les options de configuration liées au comportement de 
    l'application dans le fichier ``app/config/config.yml``.

Le fichier ``config.yml`` contient les options utilisées par l'application pour
modifier son comportement, comme l'expéditeur des notifications par email ou 
l'activation de `fonctionnalitées conditionnelles`_. Définir ces valeurs
dans le fichier ``parameters.yml`` serait ajouter une couche supplémentaire de
configuration qui ne serait pas nécessaire car vous ne voulez pas ou n'avez pas
besoin de modifier ces valeurs de configuration sur chaque serveur.

Les options de configuration définies dans le fichier ``config.yml`` varient 
généralement d'un `environnement d'exécution`_ à l'autre. C'est pourquoi Symfony
inclut déjà les fichiers ``app/config/config_dev.yml`` et ``app/config/config_prod.yml``
de sorte que vous puissiez indiquer des valeurs spécifiques à chaque environnement.

Constantes vs Options de Configuration 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'une des erreurs les plus commune lors de la définition de la configuration
de l'application est de créer de nouvelles options pour les valeurs qui ne 
change jamais, comme le nombre d'éléments pour des résultats paginés.

.. best-practice::

    Utilisez des constantes pour définir les options de configuration ne changeant que rarement.

L'approche traditionnelle pour définir les options de configuration a impliqué
que beaucoup d'application Symfony ont inclut une option comme ce qui suit, qui
serait utilisée pour gérer le nombre de messages à afficher sur la page d'accueil
d'un blog :

.. code-block:: yaml

    # app/config/config.yml
    parameters:
        homepage.num_items: 10

Si vous vous demandez quand est-ce la dernière fois que vous avez changé la valeur
d'une option de ce type, il y a de fortes chances pour que ce soit *jamais*. Créer une
option de configuration pour une valeur que vous ne configurerez jamais n'est pas 
nécessaire. Notre recommandation est de définir ces valeurs en tant que constantes
dans votre application. Vous pourriez, par exemple, définir une constante ``NUM_ITEMS``
dans l'entité ``Post`` :

.. code-block:: php

    // src/AppBundle/Entity/Post.php
    namespace AppBundle\Entity;

    class Post
    {
        const NUM_ITEMS = 10;

        // ...
    }

Le principal avantage à définir des constantes est que vous pouvez utiliser leur
valeur partout dans votre application. Lorsque vous utilisez des paramètres, ils
ne sont disponibles que lorsque vous avez accès au container Symfony.

Les constantes peuvent être utilisées par exemple dans vos templates Twig grâce
à la fonction ``constant()`` :

.. code-block:: html+jinja

    <p>
        Displaying the {{ constant('NUM_ITEMS', post) }} most recent results.
    </p>

Et les entités et repositories Doctrine peuvent accéder facilement à ces valeurs,
alors qu'elles n'ont pas accès aux paramètres du container :

.. code-block:: php

    namespace AppBundle\Repository;

    use Doctrine\ORM\EntityRepository;
    use AppBundle\Entity\Post;

    class PostRepository extends EntityRepository
    {
        public function findLatest($limit = Post::NUM_ITEMS)
        {
            // ...
        }
    }

Le seul inconvénient notable de l'utilisation des constantes pour ce type de 
configuration est que vous ne pouvez pas les redéfinir facilement dans vos tests.

Configuration Sémantique: Ne le faites pas
------------------------------------------

.. best-practice::

    Ne définissez pas une configuration sémantique d'injection de dépendance pour vos bundle.

Comme expliqué dans l'article `Comment exposer une configuration sémantique pour un Bundle`_,
les bundles Symfony ont deux possibilités concernant la gestion de la configuration : la
configuration normale des serveur via le fichier ``services.yml`` et la configuration
sémantique via une classe spécifique ``*Extension``.

Bien que la configuration sémantique soit beaucoup plus puissante et fournisse des
fonctionnalités intéressante comme la validation, la charge de travail nécessaire
pour définir cette configution n'est pas valable pour vos bundles qui ne sont pas 
destinés à être partagés en tant que bundle réutilisable.

Déplacez les options sensibles entièrement en dehors de Symfony
---------------------------------------------------------------

Lorsque vous manipulez des options sensibles, comme des accès à une base de données, nous
vous recommendons de les stocker en dehors du projet Symfony et de les rendre disponible
au travers des variables d'environnement. Apprenez comme faire en suivant cet article :
`Comment configurer les paramètres externes dans le conteneur de services`_

.. _`fonctionnalitées conditionnelles`: http://en.wikipedia.org/wiki/Feature_toggle
.. _`environnement d'exécution`: http://symfony.com/doc/current/cookbook/configuration/environments.html
.. _`constant() function`: http://twig.sensiolabs.org/doc/functions/constant.html
.. _`Comment exposer une configuration sémantique pour un Bundle`: http://symfony.com/fr/doc/current/cookbook/bundles/extension.html
.. _`Comment configurer les paramètres externes dans le conteneur de services`: http://symfony.com/fr/doc/current/cookbook/configuration/external_parameters.html
