.. index::
   single: Sécurité; Access Control Lists (ACLs)

Comment utiliser les Access Control Lists (ACLs) (« liste de contrôle d'accès » en français)
============================================================================================

Dans les applications complexes, vous allez souvent rencontrer le problème
que les décisions d'accès ne peuvent pas uniquement se baser sur la personne
(``Token``) qui demande l'accès, mais qu'elles impliquent aussi un objet domaine
auquel l'accès est demandé. C'est là où le système des ACL intervient.

Imaginez que vous êtes en train de créer un système de blog dans lequel vos
utilisateurs peuvent commenter vos posts. Maintenant, vous voulez qu'un
utilisateur puisse éditer ses propres commentaires, mais pas ceux d'autres
utilisateurs ; en outre, vous voulez vous-même être capable d'éditer tous
les commentaires. Dans ce scénario, ``Comment`` (commentaire) serait l'objet
auquel vous souhaitez restreindre l'accès. Vous pouvez envisager plusieurs
approches pour accomplir cela en utilisant Symfony2 ; les deux approches basiques
sont (liste non-exhaustive) :

- *Forcer la sécurité dans vos méthodes métier* : cela signifie garder une
  référence dans chaque ``Comment`` de tous les utilisateurs qui ont accès, et
  alors de comparer ces utilisateurs avec le ``Token`` fourni.
- *Forcer la sécurité avec des rôles* : avec cette approche, vous ajouteriez
  un rôle pour chaque objet ``Comment``, i.e. ``ROLE_COMMENT_1``,
  ``ROLE_COMMENT_2``, etc.

Les deux approches sont parfaitement valides. Cependant, elles associent votre
logique d'autorisation à votre code métier, ce qui rend le tout moins
réutilisable ailleurs, et qui augmente aussi la difficulté d'effectuer des tests
unitaires. En outre, vous pourriez rencontrer des problèmes de performance
si beaucoup d'utilisateurs accédaient à un même et unique objet domaine.

Heureusement, il y a une meilleure façon de faire, que vous allez découvrir
maintenant.

Initialisation (« Bootstrapping » en anglais)
---------------------------------------------

Maintenant, avant que vous puissiez finalement passer à l'action, vous avez
besoin d'effectuer certaines initialisations. Premièrement, vous devez
configurer la connexion que le système d'ACL est supposé utiliser :

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            acl:
                connection: default

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <acl>
            <connection>default</connection>
        </acl>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', 'acl', array(
            'connection' => 'default',
        ));


.. note::

    Le système ACL requiert qu'une connexion DBAL Doctrine (utilisable par défaut)
    ou qu'une connexion ODM Doctrine (utilisable avec `MongoDBAclBundle`_) soit
    configurée. Cependant, cela ne veut pas dire que vous devez utiliser
    l'ORM ou l'ODM Doctrine pour faire correspondre vos objets. Vous
    pouvez utiliser l'outil de correspondance de votre choix pour
    vos objets, que ce soit l'ORM Doctrine, l'ODM Mongo, Propel, ou du SQL brut,
    le choix reste le vôtre.

Une fois la connexion configurée, vous devez importer la structure de
la base de données. Heureusement, il existe une tâche pour cela. Exécutez
simplement la commande suivante :

.. code-block:: bash

    $ php app/console init:acl

Démarrage
---------

Revenez au petit exemple du début et implémentez ses ACLs.

Créer un ACL, et ajouter un ACE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: php

    // src/Acme/DemoBundle/Controller/BlogController.php
    namespace Acme\DemoBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\Security\Core\Exception\AccessDeniedException;
    use Symfony\Component\Security\Acl\Domain\ObjectIdentity;
    use Symfony\Component\Security\Acl\Domain\UserSecurityIdentity;
    use Symfony\Component\Security\Acl\Permission\MaskBuilder;

    class BlogController
    {
        // ...

        public function addCommentAction(Post $post)
        {
            $comment = new Comment();

            // préparation du $form et liaison (bind) des données

            if ($form->isValid()) {
                $entityManager = $this->get('doctrine.orm.default_entity_manager');
                $entityManager->persist($comment);
                $entityManager->flush();

                // création de l'ACL
                $aclProvider = $this->get('security.acl.provider');
                $objectIdentity = ObjectIdentity::fromDomainObject($comment);
                $acl = $aclProvider->createAcl($objectIdentity);

                // retrouve l'identifiant de sécurité de l'utilisateur actuellement connecté
                $securityContext = $this->get('security.context');
                $user = $securityContext->getToken()->getUser();
                $securityIdentity = UserSecurityIdentity::fromAccount($user);

                // donne accès au propriétaire
                $acl->insertObjectAce($securityIdentity, MaskBuilder::MASK_OWNER);
                $aclProvider->updateAcl($acl);
            }
        }
    }

Il y a plusieurs décisions d'implémentation importantes dans ce petit bout de
code. Pour le moment, je veux mettre en évidence seulement deux d'entre elles :

Tout d'abord, vous avez peut-être remarqué que ``->createAcl()`` n'accepte
pas d'objets de domaine directement, mais uniquement des implémentations de
``ObjectIdentityInterface``. Cette étape additionnelle d'indirection vous
permet de travailler avec les ACLs même si vous n'avez pas d'instance d'objet
domaine sous la main. Cela va être extrêmement utile si vous voulez vérifier
les permissions pour un grand nombre d'objets sans avoir à les désérialiser.

L'autre partie intéressante est l'appel à ``->insertObjectAce()``. Dans
l'exemple, vous accordez à l'utilisateur connecté un accès propriétaire
au Comment. Le ``MaskBuilder::MASK_OWNER`` est un masque binaire prédéfini ;
ne vous inquiétez pas, le constructeur de masque va abstraire la plupart des
détails techniques, mais en utilisant cette technique vous pouvez stocker
plein de  permissions différentesdans une même ligne de base de données ;
ce qui vous offre un avanatage considérable au niveau performance.

.. tip::

    L'ordre dans lequel les ACEs sont vérifiées est important. En tant que règle
    générale, vous devriez placer les entrées les plus spécifiques au début.

Vérification des Accès
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: php

    // src/Acme/DemoBundle/Controller/BlogController.php

    // ...

    class BlogController
    {
        // ...

        public function editCommentAction(Comment $comment)
        {
            $securityContext = $this->get('security.context');

            // check for edit access
            if (false === $securityContext->isGranted('EDIT', $comment))
            {
                throw new AccessDeniedException();
            }

            // ... récupérez le bon objet « comment », et éditez-le ici
        }
    }

Dans cet exemple, vous vérifiez que l'utilisateur possède la permission
``EDIT``. En interne, Symfony2 fait correspondre la permission avec
plusieurs masques binaires, et vérifie si l'utilisateur possède l'un
d'entre eux.

.. note::

    Vous pouvez définir jusqu'à 32 permissions de base (dépendant du PHP
    de votre OS, cela pourrait varier entre 30 et 32). De plus, vous pouvez
    aussi définir des permissions cumulées.

Permissions Cumulées
--------------------

Dans le premier exemple ci-dessus, vous avez accordé uniquement la
permission basique ``OWNER`` à l'utilisateur. Bien que cela permette aussi
à l'utilisateur d'effectuer n'importe quelle opération telle que la lecture,
l'édition, etc. sur l'objet domaine, il y a des cas où vous voudrez accorder
ces permissions explicitement.

Le ``MaskBuilder`` peut être utilisé pour créer des masques binaires facilement
en combinant plusieurs permissions de base :

.. code-block:: php

    $builder = new MaskBuilder();
    $builder
        ->add('view')
        ->add('edit')
        ->add('delete')
        ->add('undelete')
    ;
    $mask = $builder->get(); // int(29)

Ce masque binaire représenté par un entier peut ainsi être utilisé pour accorder
à un utilisateur les permissions de base que vous avez ajoutées ci-dessus :

.. code-block:: php

    $identity = new UserSecurityIdentity('johannes', 'Acme\UserBundle\Entity\User');
    $acl->insertObjectAce($identity, $mask);

L'utilisateur a désormais le droit de lire, éditer, supprimer, et annuler
une suppression sur des objets.

.. _`MongoDBAclBundle`: https://github.com/IamPersistent/MongoDBAclBundle