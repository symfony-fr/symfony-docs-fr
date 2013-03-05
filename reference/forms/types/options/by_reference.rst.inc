by_reference
~~~~~~~~~~~~

**type**: ``Boolean`` **default**: ``true``

Dans la plupart des cas, si vous avez un champ ``name``, alors vous vous attendez
à ce que l'appel à ``setName`` soit fait sur l'objet sous-jacent. Pourtant, dans
certains cas, ``setName`` ne doit *pas* être appelé. Définir ``by_reference`` vous
assure que le setter est appelé dans tous les cas.

Pour aller plus loin, voici un exemple simple::

    $builder = $this->createFormBuilder($article);
    $builder
        ->add('title', 'text')
        ->add(
            $builder->create('author', 'form', array('by_reference' => ?))
                ->add('name', 'text')
                ->add('email', 'email')
        )

Si ``by_reference`` est à true, voici ce qui se passe dans l'envers du décor lorsque
vous faites un ``bind`` sur le formulaire::

    $article->setTitle('...');
    $article->getAuthor()->setName('...');
    $article->getAuthor()->setEmail('...');

Notez que la méthode ``setAuthor`` n'est pas appelée. L'objet Author est modifié par référence.

Si vous définissez ``by_reference`` à false, l'association ressemblera à ceci::

    $article->setTitle('...');
    $author = $article->getAuthor();
    $author->setName('...');
    $author->setEmail('...');
    $article->setAuthor($author);

Donc, tout ce que ``by_reference=false`` fait réellement est de forcer le framework à faire
l'appel au setter de l'objet parent.

De même, si vous utilisez le type de formulaire :doc:`collection</reference/forms/types/collection>`
où la collection sous-jacente est un objet (comme un ``ArrayCollection`` Doctrine), alors
``by_reference`` doit être défini à ``false`` si vous avez besoin que
le setter (ex ``setAuthors``) soit appelé.