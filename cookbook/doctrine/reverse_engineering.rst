.. index::
   single: Doctrine; Générer des entités à partir d'une base de données existante

Comment générer des Entités à partir d'une base de données existante
====================================================================

Lorsque vous commencez à travailler sur un tout nouveau projet qui utilise
une base de données, deux situations différentes peuvent arriver. Dans la
plupart des cas, le modèle de base de données est conçu et construit depuis
zéro. Dans d'autres cas, cependant, vous commencerez avec un modèle de base
de données existant et probablement inchangeable. Heureusement, Doctrine
vient avec une série d'outils vous aidant à générer les classes de modèle
à partir de votre base de données existante.

.. note::

    Comme la `documentation des outils Doctrine`_ le dit, faire du « reverse
    engineering » est un processus qu'on n'effectue qu'une seule fois lorsqu'on
    démarre un projet. Doctrine est capable de convertir environ 70-80% des
    informations de correspondance nécessaires basé sur les champs, les index
    et les contraintes de clés étrangères. En revanche, Doctrine ne peut pas
    identifier les associations inverses, les types d'inhéritance, les entités
    avec clés étrangères en tant que clés primaires ou les opérations sémantiques
    sur des associations telles que la « cascade » ou les évènements de cycle de
    vie. Ainsi, du travail additionnel sur les entités générées sera nécessaire
    par la suite pour finir la conception de chacune afin de satisfaire les
    spécificités du modèle de votre domaine.

Ce tutoriel assume que vous utilisez une application de blog simple avec les
deux tables suivantes : ``blog_post`` et ``blog_comment``. Une entrée
« comment » est liée à une entrée « post » grâce à une contrainte de clé
étrangère.

.. code-block:: sql

    CREATE TABLE `blog_post` (
      `id` bigint(20) NOT NULL AUTO_INCREMENT,
      `title` varchar(100) COLLATE utf8_unicode_ci NOT NULL,
      `content` longtext COLLATE utf8_unicode_ci NOT NULL,
      `created_at` datetime NOT NULL,
      PRIMARY KEY (`id`),
    ) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

    CREATE TABLE `blog_comment` (
      `id` bigint(20) NOT NULL AUTO_INCREMENT,
      `post_id` bigint(20) NOT NULL,
      `author` varchar(20) COLLATE utf8_unicode_ci NOT NULL,
      `content` longtext COLLATE utf8_unicode_ci NOT NULL,
      `created_at` datetime NOT NULL,
      PRIMARY KEY (`id`),
      KEY `blog_comment_post_id_idx` (`post_id`),
      CONSTRAINT `blog_post_id` FOREIGN KEY (`post_id`) REFERENCES `blog_post` (`id`) ON DELETE CASCADE
    ) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

Avant d'aller plus loin, assurez-vous que vos paramètres de connexion à la base
de données sont correctement définis dans le fichier ``app/config/parameters.yml``
(ou à quelconque endroit que ce soit où votre configuration de base de données est
conservée) et que vous avez initialisé un bundle qui va contenir votre future classe
entité. Dans ce tutorial, nous assumerons qu'un ``AcmeBlogBundle`` existe et
qu'il se situe dans le dossier ``src/Acme/BlogBundle``.

La première étape permettant de construire les classes entité depuis une
base de données existante est de demander à Doctrine d'introspecter cette
dernière et de générer les fichiers de méta-données correspondants. Les
fichiers de méta-données décrivent la classe entité à générer basé sur
les champs des tables.

.. code-block:: bash

    php app/console doctrine:mapping:convert xml ./src/Acme/BlogBundle/Resources/config/doctrine/metadata/orm --from-database --force

Cette outil de ligne de commande demande à Doctrine d'introspecter la base de données et de
générer les fichiers XML de méta-données dans le dossier
``src/Acme/BlogBundle/Resources/config/doctrine/metadata/orm`` de votre bundle.

.. tip::

    Il est aussi possible de générer les méta-données au format YAML en changeant
    le premier argument pour `yml`.

Le fichier de méta-données ``BlogPost.dcm.xml`` généré ressemble à ce qui suit :

.. code-block:: xml

    <?xml version="1.0" encoding="utf-8"?>
    <doctrine-mapping>
      <entity name="BlogPost" table="blog_post">
        <change-tracking-policy>DEFERRED_IMPLICIT</change-tracking-policy>
        <id name="id" type="bigint" column="id">
          <generator strategy="IDENTITY"/>
        </id>
        <field name="title" type="string" column="title" length="100"/>
        <field name="content" type="text" column="content"/>
        <field name="isPublished" type="boolean" column="is_published"/>
        <field name="createdAt" type="datetime" column="created_at"/>
        <field name="updatedAt" type="datetime" column="updated_at"/>
        <field name="slug" type="string" column="slug" length="255"/>
        <lifecycle-callbacks/>
      </entity>
    </doctrine-mapping>

Une fois que les fichiers de méta-données sont générés, vous pouvez demander
à Doctrine d'importer le schéma et de construire les classes entité qui lui
sont liées en exécutant les deux commandes suivantes.

.. code-block:: bash

    php app/console doctrine:mapping:import AcmeBlogBundle annotation
    php app/console doctrine:generate:entities AcmeBlogBundle

La première commande génère les classes entité avec des annotations de
correspondance, mais vous pouvez bien sûr changer l'argument ``annotation``
pour être ``xml`` ou ``yml``. La classe entité nouvellement créée ressemble
à ce qui suit :

.. code-block:: php

    <?php

    // src/Acme/BlogBundle/Entity/BlogComment.php
    namespace Acme\BlogBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;

    /**
     * Acme\BlogBundle\Entity\BlogComment
     *
     * @ORM\Table(name="blog_comment")
     * @ORM\Entity
     */
    class BlogComment
    {
        /**
         * @var bigint $id
         *
         * @ORM\Column(name="id", type="bigint", nullable=false)
         * @ORM\Id
         * @ORM\GeneratedValue(strategy="IDENTITY")
         */
        private $id;

        /**
         * @var string $author
         *
         * @ORM\Column(name="author", type="string", length=100, nullable=false)
         */
        private $author;

        /**
         * @var text $content
         *
         * @ORM\Column(name="content", type="text", nullable=false)
         */
        private $content;

        /**
         * @var datetime $createdAt
         *
         * @ORM\Column(name="created_at", type="datetime", nullable=false)
         */
        private $createdAt;

        /**
         * @var BlogPost
         *
         * @ORM\ManyToOne(targetEntity="BlogPost")
         * @ORM\JoinColumn(name="post_id", referencedColumnName="id")
         */
        private $post;
    }

Comme vous pouvez le voir, Doctrine convertit tous les champs de la table en propriétés
privées et annotées de la classe. La chose la plus impressionnante et qu'il
identifie aussi la relation avec la classe entité ``BlogPost`` basé sur la contrainte
de clé étrangère. De ce fait, vous pouvez trouver une propriété privée ``$post``
correspondant à une entité ``BlogPost`` dans la classe entité ``BlogComment``.

La dernière commande a généré tous les « getters » et « setters » pour les propriétés
de vos deux classes entité ``BlogPost`` et ``BlogComment``. Les entités générées
sont maintenant prêtes à être utilisées. Amusez-vous!

.. _`documentation des outils Doctrine`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/tools.html#reverse-engineering
