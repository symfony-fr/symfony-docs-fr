.. index::
   single: Formulaire; Intégrer une collection de formulaires

Comment intégrer une Collection de Formulaires
==============================================

Dans cet article, vous allez apprendre à créer un formulaire qui intègre une
collection de plusieurs autres formulaires. Cela pourrait être utile, par
exemple, si vous aviez une classe ``Task`` et que vous voudriez éditer/créer/supprimer
différents objets ``Tag`` liés à cette « Task », tout cela dans le même
formulaire.

.. note::

    Dans cet article, nous assumerons que vous utilisez Doctrine en tant
    qu'outil de stockage pour votre base de données. Mais si vous n'utilisez
    pas Doctrine (par exemple : Propel ou une simple connexion à la base
    de données), tout reste très similaire. Il n'y a que quelques parties
    de ce tutoriel qui s'occupent de la « persistence ».

    Si vous *utilisez* Doctrine, vous aurez besoin d'ajouter les méta-données
    de Doctrine, incluant la ``ManyToMany`` sur la propriété ``tags`` de Task.

Commençons ici : supposons que chaque ``Task`` appartienne à plusieurs objets
``Tags``. Créons tout d'abord une simple classe ``Task``::

    // src/Acme/TaskBundle/Entity/Task.php
    namespace Acme\TaskBundle\Entity;
    
    use Doctrine\Common\Collections\ArrayCollection;

    class Task
    {
        protected $description;

        protected $tags;

        public function __construct()
        {
            $this->tags = new ArrayCollection();
        }
        
        public function getDescription()
        {
            return $this->description;
        }

        public function setDescription($description)
        {
            $this->description = $description;
        }

        public function getTags()
        {
            return $this->tags;
        }

        public function setTags(ArrayCollection $tags)
        {
            $this->tags = $tags;
        }
    }

.. note::

    ``ArrayCollection`` est spécifique à Doctrine et revient au même que
    d'utiliser un ``array`` (mais cela doit être une ``ArrayCollection``) si vous
    utilisez Doctrine.

Maintenant, créez une classe ``Tag``. Comme vous l'avez vu ci-dessus, une ``Task``
peut avoir plusieurs objets ``Tag``::

    // src/Acme/TaskBundle/Entity/Tag.php
    namespace Acme\TaskBundle\Entity;

    class Tag
    {
        public $name;
    }

.. tip::

    La propriété ``name`` est « public » içi, mais elle pourrait tout aussi bien
    être « protected » ou « private » (ce qui impliquerait d'avoir des méthodes
    ``getName`` et ``setName``).

Venons-en maintenant aux formulaires. Créez une classe formulaire afin
qu'un objet ``Tag`` puisse être modifié par l'utilisateur::

    // src/Acme/TaskBundle/Form/Type/TagType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class TagType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('name');
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'Acme\TaskBundle\Entity\Tag',
            ));
        }

        public function getName()
        {
            return 'tag';
        }
    }

Avec cela, nous avons tout ce qu'il faut pour afficher un formulaire pour le
tag lui-même. Mais comme le but final étant de permettre la modification des
tags d'une ``Task`` directement depuis le formulaire de la « task » lui-même,
créez un formulaire pour la classe ``Task``.

Notez que nous intégrons une collection de formulaires ``TagType``
en utilisant le type de champ
:doc:`collection</reference/forms/types/collection>`::

    // src/Acme/TaskBundle/Form/Type/TaskType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class TaskType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('description');

            $builder->add('tags', 'collection', array('type' => new TagType()));
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'Acme\TaskBundle\Entity\Task',
            ));
        }

        public function getName()
        {
            return 'task';
        }
    }

Dans votre contrôleur, vous allez maintenant initialiser une nouvelle instance
de ``TaskType``::

    // src/Acme/TaskBundle/Controller/TaskController.php
    namespace Acme\TaskBundle\Controller;
    
    use Acme\TaskBundle\Entity\Task;
    use Acme\TaskBundle\Entity\Tag;
    use Acme\TaskBundle\Form\Type\TaskType;
    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    
    class TaskController extends Controller
    {
        public function newAction(Request $request)
        {
            $task = new Task();

            // code "test" - le code ci-dessous est simplement là pour que la
            // Task ait quelques tags, sinon, l'exemple ne serait pas intéressant
            $tag1 = new Tag();
            $tag1->name = 'tag1';
            $task->getTags()->add($tag1);
            $tag2 = new Tag();
            $tag2->name = 'tag2';
            $task->getTags()->add($tag2);
            // fin du code "test"
            
            $form = $this->createForm(new TaskType(), $task);
            
            // analyse le formulaire quand on reçoit une requête POST
            if ('POST' === $request->getMethod()) {
                $form->bindRequest($request);
                if ($form->isValid()) {
                    // ici vous pouvez par exemple sauvegarder la Task et ses objets Tag
                }
            }
            
            return $this->render('AcmeTaskBundle:Task:new.html.twig', array(
                'form' => $form->createView(),
            ));
        }
    }

Le template correspondant est maintenant capable d'afficher le champ
``description`` pour le formulaire de la tâche ainsi que les formulaires
``TagType`` pour n'importes quels tags qui sont liés à cette objet ``Task``.
Dans le contrôleur ci-dessus, j'ai ajouté du code test afin que vous puissiez
voir cela en action (puisqu'un objet ``Task`` possède zéro tag lorsqu'il est
créé pour la première fois).

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Task/new.html.twig #}
        {# ... #}

        <form action="..." method="POST" {{ form_enctype(form) }}>
            {# affiche l'unique champ de la tâche : description #}
            {{ form_row(form.description) }}

            <h3>Tags</h3>
            <ul class="tags">
                {# itère sur chaque tag existant et affiche son unique champ : name #}
                {% for tag in form.tags %}
                    <li>{{ form_row(tag.name) }}</li>
                {% endfor %}
            </ul>

            {{ form_rest(form) }}
            {# ... #}
        </form>

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Task/new.html.php -->
        <!-- ... -->

        <form action="..." method="POST" ...>
            <h3>Tags</h3>
            <ul class="tags">
                <?php foreach($form['tags'] as $tag): ?>
                    <li><?php echo $view['form']->row($tag['name']) ?></li>
                <?php endforeach; ?>
            </ul>

            <?php echo $view['form']->rest($form) ?>
        </form>
        
        <!-- ... -->

Lorsque l'utilisateur soumet le formulaire, les données soumises pour les
champs ``Tags`` sont utilisées pour construire une collection « ArrayCollection »
d'objets ``Tag``, qui est ensuite affectée au champ ``tag`` de l'instance ``Task``.

La collection ``Tags`` est naturellement accessible via ``$task->getTags()``
et peut être persistée dans la base de données ou utilisée de quelconque manière
dont vous en avez besoin.

Jusqu'ici, tout cela fonctionne bien, mais cela ne vous permet pas d'ajouter
de nouveaux tags ou de supprimer des tags existants de manière dynamique. Donc,
bien qu'éditer des tags existants fonctionnera parfaitement, votre utilisateur
ne pourra pour le moment pas en ajouter de nouveaux.

.. caution::

    Dans cet exemple, nous intégrons une seule collection, mais vous n'êtes
    pas limité à cela. Vous pouvez aussi intégrer des collections « imbriquées »
    avec autant de sous-niveaux que vous souhaitez. Mais si vous utilisez Xdebug
    dans votre environnement de développement, vous pourriez recevoir une erreur
    telle ``Maximum function nesting level of '100' reached, aborting!``.
    Cela est dû au paramètre PHP ``xdebug.max_nesting_level``, qui est défini
    avec une valeur de ``100`` par défaut.

    Cette directive limite la récursion à 100 appels, ce qui ne pourrait pas
    être assez pour afficher le formulaire dans le template si vous rendez le
    formulaire en entier en une seule fois (par exemple : ``form_widget(form)``).
    Pour parer à cela, vous pouvez définir cette directive avec une valeur plus
    haute (soit dans le fichier ini PHP ou à l'aide de :phpfunction:`ini_set`,
    par exemple dans ``app/autoload.php``) ou bien afficher chaque champ du
    formulaire « manuellement » en utilisant ``form_row``.

.. _cookbook-form-collections-new-prototype:

Autoriser de « nouveaux » tags avec le « prototypage »
------------------------------------------------------

Autoriser l'utilisateur à ajouter de nouveaux tags signifie que nous allons
avoir besoin d'utiliser un peu de Javascript. Plus tôt, nous avons ajouter deux
tags à notre formulaire dans le contrôleur. Maintenant, nous devons permettre à
l'utilisateur d'ajouter autant de tags qu'il souhaite directement depuis le navigateur.
Cela va être effectué grâce à quelques lignes de Javascript.

La première chose que nous devons faire est de laisser savoir à la collection
du formulaire qu'elle va recevoir un nombre inconnu de tags. Jusqu'ici, nous
avons ajouté deux tags et le formulaire s'attend à en recevoir exactement deux,
sinon une erreur sera lancé : ``This form should not contain extra fields``.
Pour rendre cela flexible, nous ajoutons l'option ``allow_add`` à notre champ
collection::

    // src/Acme/TaskBundle/Form/Type/TaskType.php
    // ...
    
    use Symfony\Component\Form\FormBuilderInterface;

    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $builder->add('description');

        $builder->add('tags', 'collection', array(
            'type' => new TagType(),
            'allow_add' => true,
            'by_reference' => false,
        ));
    }

Notez que nous avons aussi ajouté ``'by_reference' => false``. Normalement, le
framework de formulaire modifierait les tags d'un objet `Task` *sans* ne jamais
appeler `setTags`. En définissant :ref:`by_reference<reference-form-types-by-reference>`
à `false`, `setTags` sera appelée. Vous comprendrez plus tard pourquoi cela est important.

En plus de dire au champ d'accepter n'importe quel nombre d'objets soumis, l'option
``allow_add`` met une variable « prototype » à votre disposition. Ce « prototype »
est un petit « template » qui contient tout le code HTML nécessaire pour afficher
tout nouveau formulaire « tag ». Pour l'utiliser, faites le changement suivant
dans votre formulaire :

.. configuration-block::

    .. code-block:: html+jinja
    
        <ul class="tags" data-prototype="{{ form_widget(form.tags.get('prototype')) | e }}">
            ...
        </ul>
    
    .. code-block:: html+php
    
        <ul class="tags" data-prototype="<?php echo $view->escape($view['form']->row($form['tags']->get('prototype'))) ?>">
            ...
        </ul>

.. note::

    Si vous affichez votre sous-formulaire « tags » en entier et en une seule fois
    (par exemple : ``form_row(form.tags)``), alors le prototype est automatiquement
    disponible dans le ``div`` extérieur avec l'attribut ``data-prototype``,
    similaire à ce que vous voyez ci-dessus.

.. tip::

    ``form.tags.get('prototype')`` est un élément de formulaire qui ressemble à
    l'élément individuel ``form_widget(tag)`` à l'intérieur de notre boucle ``for``.
    Cela signifie que vous pouvez appeler ``form_widget``, ``form_row``, ou
    ``form_label`` sur ce prototype. Vous pourriez même choisir de ne rendre qu'un
    seul de ses champs (par exemple : le champ ``name``) :
    
    .. code-block:: html+jinja
    
        {{ form_widget(form.tags.get('prototype').name) | e }}

Sur la page délivrée, le résultat ressemblera à quelque chose comme ca :

.. code-block:: html

    <ul class="tags" data-prototype="&lt;div&gt;&lt;label class=&quot; required&quot;&gt;__name__&lt;/label&gt;&lt;div id=&quot;task_tags___name__&quot;&gt;&lt;div&gt;&lt;label for=&quot;task_tags___name___name&quot; class=&quot; required&quot;&gt;Name&lt;/label&gt;&lt;input type=&quot;text&quot; id=&quot;task_tags___name___name&quot; name=&quot;task[tags][__name__][name]&quot; required=&quot;required&quot; maxlength=&quot;255&quot; /&gt;&lt;/div&gt;&lt;/div&gt;&lt;/div&gt;">

Le but de cette section sera d'utiliser JavaScript pour lire cet attribut et
ajouter dynamiquement un nouveau tag lorsque l'utilisateur clique sur un
lien « Ajouter un tag ». Pour garder les choses simples, nous allons utiliser
jQuery et assumer que vous l'avez déjà inclus quelque part dans votre page.

Ajoutez une balise ``script`` quelque part dans votre page afin que nous puissions
commencer à écrire un peu de JavaScript.

Tout d'abord, ajoutez un lien en bas de votre liste de « tags » via JavaScript.
Ensuite, liez l'événement « click » de ce lien afin que nous puissions ajouter
un formulaire tag (``addTagForm`` sera montré plus tard) :

.. code-block:: javascript

    // Récupère le div qui contient la collection de tags
    var collectionHolder = $('ul.tags');

    // ajoute un lien « add a tag »
    var $addTagLink = $('<a href="#" class="add_tag_link">Add a tag</a>');
    var $newLinkLi = $('<li></li>').append($addTagLink);

    jQuery(document).ready(function() {
        // ajoute l'ancre « add a tag » et li à la balise ul
        collectionHolder.append($newLinkLi);

        $addTagLink.on('click', function(e) {
            // empêche le lien de créer un « # » dans l'URL
            e.preventDefault();

            // ajoute un nouveau formulaire tag (voir le prochain bloc de code)
            addTagForm(collectionHolder, $newLinkLi);
        });
    });

Le travail de la fonction ``addTagForm`` sera d'utiliser l'attribut ``data-prototype``
pour dynamiquement ajouter un nouveau formulaire lorsque ce lien est cliqué. Le code
HTML de ``data-prototype`` contient la balise ``texte`` avec un nom tel
``task[tags][__name__][name]`` et un id tel ``task_tags___name___name``. La chaîne de
caractères ``__name__`` est une variable de substitution (« placeholder » en anglais) que
nous remplacerons avec un nombre unique et incrémental (par exemple : ``task[tags][3][name]``).

.. versionadded:: 2.1
    La variable de substitution a été changée dans Symfony 2.1. Au lieu de ``$$name$$``,
    elle se nomme dorénavant ``__name__``.

Le code nécessaire pour faire fonctionner tout cela peut varier, mais en voici un
exemple :

.. code-block:: javascript

    function addTagForm(collectionHolder, $newLinkLi) {
        // Récupère l'élément ayant l'attribut data-prototype comme expliqué plus tôt
        var prototype = collectionHolder.attr('data-prototype');

        // Remplace '__name__' dans le HTML du prototype par un nombre basé sur
        // la longueur de la collection courante
        var newForm = prototype.replace(/__name__/g, collectionHolder.children().length);

        // Affiche le formulaire dans la page dans un li, avant le lien "Add a tag"
        var $newFormLi = $('<li></li>').append(newForm);
        $newLinkLi.before($newFormLi);
    }

.. note:

    Il est préférable de séparer votre javascript dans des fichiers JavaScript
    réels plutôt que de l'écrire directement en plein milieu de votre code HTML
    comme nous le faisons ici.

Maintenant, chaque fois qu'un utilisateur clique sur le lien ``Add a tag``, un
nouveau sous-formulaire apparaîtra sur la page. Lors de la soumission, quelconques
formulaires de tag seront convertis en de nouveaux objets ``Tag`` et ajoutés à la
propriété ``tags`` de l'objet ``Task``.

.. sidebar:: Doctrine: Relations de Cascade et sauvegarde du côté « Inverse »

    Afin que les nouveaux tags soient sauvegardés dans Doctrine, vous devez
    considérer certaines choses en plus. Tout d'abord, à moins que vous
    itériez sur tous les nouveaux objets ``Tag`` et appelez ``$em->persist($tag)``
    sur chacun d'entre eux, vous allez recevoir une erreur de la part de Doctrine :

        A new entity was found through the relationship 'Acme\TaskBundle\Entity\Task#tags' that was not configured to cascade persist operations for entity...

    Pour réparer cela, vous pourriez choisir d'effectuer automatiquement l'opération
    de persistence en mode « cascade » de l'objet ``Task`` avec quelconques tags liés.
    Pour faire ceci, ajoutez l'option ``cascade`` à votre méta-donnée ``ManyToMany`` :
    
    .. configuration-block::
    
        .. code-block:: php-annotations

            /**
             * @ORM\ManyToMany(targetEntity="Tag", cascade={"persist"})
             */
            protected $tags;

        .. code-block:: yaml

            # src/Acme/TaskBundle/Resources/config/doctrine/Task.orm.yml
            Acme\TaskBundle\Entity\Task:
                type: entity
                # ...
                oneToMany:
                    tags:
                        targetEntity: Tag
                        cascade:      [persist]

    Un second problème potentiel peut toucher les relations de Doctrine
    en ce qui concerne `Le côté Propriétaire et le côté Inverse`_. Dans
    cet exemple, si le côté « propriétaire » dans la relation est « Task »,
    alors la persistence va fonctionner sans problème comme les tags sont
    ajoutés correctement à la « Task ». Cependant, si le côté « propriétaire »
    est « Tag », alors vous aurez besoin de coder un peu plus afin de
    vous assurer que le bon côté de la relation soit correctement modifié.

    L'astuce est de s'assurer qu'une unique « Task » est définie pour chaque
    « Tag ». Une manière facile de faire cela est d'ajouter un bout de logique
    supplémentaire à la méthode ``setTags()``, qui est appelée par le framework
    formulaire puisque la valeur de :ref:`by_reference<reference-form-types-by-reference>`
    est définie comme ``false``::
    
        // src/Acme/TaskBundle/Entity/Task.php
        // ...

        public function setTags(ArrayCollection $tags)
        {
            foreach ($tags as $tag) {
                $tag->addTask($this);
            }

            $this->tags = $tags;
        }

    Dans le ``Tag``, assurez-vous simplement d'avoir une méthode ``addTask``::

        // src/Acme/TaskBundle/Entity/Tag.php
        // ...

        public function addTask(Task $task)
        {
            if (!$this->tasks->contains($task)) {
                $this->tasks->add($task);
            }
        }

    Si vous avez une relation ``OneToMany``, alors l'astuce reste similaire,
    excepté que vous pouvez simplement appeler la méthode ``setTask`` depuis
    la méthode ``setTags``.

.. _cookbook-form-collections-remove:

Autoriser des tags à être supprimés
-----------------------------------

La prochaine étape est d'autoriser la suppression d'un élément particulier de
la collection. La solution est similaire à celle qui permet d'autoriser l'ajout
de tags.

Commencez par ajouter l'option ``allow_delete`` dans le Type de formulaire::

    // src/Acme/TaskBundle/Form/Type/TaskType.php
    // ...
    
    use Symfony\Component\Form\FormBuilderInterface;
    
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $builder->add('description');

        $builder->add('tags', 'collection', array(
            'type' => new TagType(),
            'allow_add' => true,
            'allow_delete' => true,
            'by_reference' => false,
        ));
    }

Modifications des Templates
~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'option ``allow_delete`` a une conséquence : si un élément d'une collection
n'est pas envoyé lors de la soumission, les données liées à cet élément sont
supprimées de la collection sur le serveur. La solution est donc de supprimer
l'élément formulaire du DOM.

Premièrement, ajoutez un lien « Supprimer ce tag » dans chaque formulaire de tag :

.. code-block:: javascript

    jQuery(document).ready(function() {
        // ajoute un lien de suppression à tous les éléments li de
        // formulaires de tag existants
        collectionHolder.find('li').each(function() {
            addTagFormDeleteLink($(this));
        });
    
        // ... le reste du bloc vu plus haut
    });
    
    function addTagForm() {
        // ...
        
        // ajoute un lien de suppression au nouveau formulaire
        addTagFormDeleteLink($newFormLi);
    }

La fonction ``addTagFormDeleteLink`` va ressembler à quelque chose comme ça :

.. code-block:: javascript

    function addTagFormDeleteLink($tagFormLi) {
        var $removeFormA = $('<a href="#">delete this tag</a>');
        $tagFormLi.append($removeFormA);

        $removeFormA.on('click', function(e) {
            // empêche le lien de créer un « # » dans l'URL
            e.preventDefault();

            // supprime l'élément li pour le formulaire de tag
            $tagFormLi.remove();
        });
    }

Lorsqu'un formulaire de tag est supprimé du DOM et soumis, l'objet ``Tag`` supprimé
ne sera pas inclus dans la collection passée à ``setTags``. Dépendant de votre couche
de persistence, cela pourrait ou ne pourrait pas être suffisant pour effectivement
supprimer la relation entre le ``Tag`` supprimé et l'objet ``Task``.

.. sidebar:: Doctrine: Assurer la persistence dans la base de données

    Quand vous supprimez des objets de cette manière, vous pourriez avoir à
    travailler un peu plus afin de vous assurer que la relation entre la
    « Task » et le « Tag » supprimé soit correctement enlevée.

    Dans Doctrine, vous avez deux « côtés » dans une relation : le côté propriétaire
    et le côté inverse. Normalement, dans ce cas, vous aurez une relation ``ManyToMany``
    et les tags supprimés disparaîtront et seront persistés correctement (ajouter
    des tags fonctionne aussi sans efforts supplémentaires).

    Mais si vous avez une relation ``OneToMany`` ou une ``ManyToMany`` avec
    un ``mappedBy`` sur l'entité « Task » (signifiant qu'une « Task » est le
    côté « inverse »), vous devrez effectuer plus de choses afin que les tags
    supprimés soient persistés correctement.

    Dans ce cas, vous pouvez modifier le contrôleur afin qu'il efface la
    relation pour les tags supprimés. Ceci assume que vous ayez une ``editAction``
    qui gére la « mise à jour » de votre « Task »::

        // src/Acme/TaskBundle/Controller/TaskController.php
        // ...

        public function editAction($id, Request $request)
        {
            $em = $this->getDoctrine()->getManager();
            $task = $em->getRepository('AcmeTaskBundle:Task')->find($id);
    
            if (!$task) {
                throw $this->createNotFoundException('No task found for is '.$id);
            }

            // Crée un tableau contenant les objets Tag courants de la
            // base de données
            foreach ($task->getTags() as $tag) $originalTags[] = $tag;
          
            $editForm = $this->createForm(new TaskType(), $task);

               if ('POST' === $request->getMethod()) {
                $editForm->bindRequest($this->getRequest());

                if ($editForm->isValid()) {
        
                    // filtre $originalTags pour ne contenir que les tags
                    // n'étant plus présents
                    foreach ($task->getTags() as $tag) {
                        foreach ($originalTags as $key => $toDel) {
                            if ($toDel->getId() === $tag->getId()) {
                                unset($originalTags[$key]);
                            }
                        }
                    }

                    // supprime la relation entre le tag et la « Task »
                    foreach ($originalTags as $tag) {
                        // supprime la « Task » du Tag
                        $tag->getTasks()->removeElement($task);
    
                        // si c'était une relation ManyToOne, vous pourriez supprimer la
                        // relation comme ceci
                        // $tag->setTask(null);
                        
                        $em->persist($tag);

                        // si vous souhaitiez supprimer totalement le Tag, vous pourriez
                        // aussi faire comme cela
                        // $em->remove($tag);
                    }

                    $em->persist($task);
                    $em->flush();

                    // redirige vers quelconque page d'édition
                    return $this->redirect($this->generateUrl('task_edit', array('id' => $id)));
                }
            }
            
            // affiche quelconque template de formulaire
        }

    Comme vous pouvez le voir, ajouter et supprimer les éléments correctement
    peut ne pas être trivial. A moins que vous ayez une relation ``ManyToMany``
    où « Task » est le côté « propriétaire », vous devrez ajouter du code
    supplémentaire pour vous assurer que la relation soit correctement mise à
    jour (que ce soit pour l'ajout de nouveaux tags ou pour la suppression de
    tags existants) pour chacun des objets Tag.


.. _`Le côté Propriétaire et le côté Inverse`: http://docs.doctrine-project.org/en/latest/reference/unitofwork-associations.html
