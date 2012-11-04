@ParamConverter
===============

Utilisation
-----------

L'annotation ``@ParamConverter`` appelle des *convertisseurs*, pour convertir des paramètres
de requêtes en objets. Ces objets sont stockés comme attributs de requête de telle sorte
qu'ils puissent être injectés comme arguments de méthodes de contrôleur::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;

    /**
     * @Route("/blog/{id}")
     * @ParamConverter("post", class="SensioBlogBundle:Post")
     */
    public function showAction(Post $post)
    {
    }

Plusieurs choses se passent dans l'envers du décor :

* Le convertisseur esssaie de récupérer un objet ``SensioBlogBundle:Post`` depuis les
  attributs de la requête (les attributs de la requête proviennent des paramètres
  de substitution de la route, ici ``id``) ;

* Si aucun objet ``Post`` n'est trouvé, une réponse ``404`` est générée ;

* Si un objet ``Post`` est trouvé, un nouvel attribut ``post`` de requête est défini
  (accessible via ``$request->attributes->get('post')``) ;

* Comme pour tout autre attribut de requête, il est injecté dans le contrôleur lorsqu'il
  est présent dans la signature de la méthode.

Si vous utilisez le « type hinting », comme dans notre exemple ci-dessus, vous pouvez
même complètement omettre l'annotation ``@ParamConverter``::

    // automatique, avec la signature de la méthode
    public function showAction(Post $post)
    {
    }

Convertisseurs préconstruits
----------------------------

Le bundle possède deux convertisseurs préconstruits : celui de Doctrine, et
un convertisseur DateTime.

Convertisseur Doctrine
~~~~~~~~~~~~~~~~~~~~~~

Nom du convertisseur : ``doctrine.orm``

Le convertisseur Doctrine tente de convertir des attributs de requête en entités
Doctrine récupérées depuis la base de données. Deux approches sont possibles :

- Récupérer l'objet par sa clé primaire ;
- Récupérer l'objet par un ou plusieurs champ(s) qui contien(nen)t une valeur unique en
  base de données.

L'algorithme suivant détermine quelle opération sera exécutée.

- Si un paramètre ``{id}`` est présent dans la route, retrouve l'objet par sa clé
  primaire ;
- Si une option ``'id'`` est configurée et correspond aux paramètres de la route, l'objet
  est récupéré par sa clé primaire ;
- Si les règles précédentes ne s'appliquent pas, essaye de trouver une entité en faisant
  correspondre les paramètres de la route aux champs. Vous pouvez contrôler ce processus
  en configurant des paramètres ``exclude`` ou un attribut pour le nom de champ ``mapping``.

Par défaut, le convertisseur Doctrine utilise le gestionnaire d'entité *default*. Cela
peut être configuré avec l'option ``entity_manager``::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;

    /**
     * @Route("/blog/{id}")
     * @ParamConverter("post", class="SensioBlogBundle:Post", options={"entity_manager" = "foo"})
     */
    public function showAction(Post $post)
    {
    }

Si le paramètre de substitution n'a pas le même nom que la clé primaire, passez
l'option ``id``::

    /**
     * @Route("/blog/{post_id}")
     * @ParamConverter("post", class="SensioBlogBundle:Post", options={"id" = "post_id"})
     */
    public function showAction(Post $post)
    {
    }

Cela vous permet également d'avoir plusieurs convertisseurs pour une action::

    /**
     * @Route("/blog/{id}/comments/{comment_id}")
     * @ParamConverter("comment", class="SensioBlogBundle:Comment", options={"id" = "comment_id"})
     */
    public function showAction(Post $post, Comment $comment)
    {
    }

Dans l'exemple ci-dessus, le paramètre ``post`` est géré automatiquement, mais le
paramètre ``comment`` est configuré avec l'annotation car ils ne peuvent pas tous
les deux suivre la convention par défaut.

Si vous voulez faire correspondre une entité en utilisant plusieurs champs,
utilisez ``mapping``::

    /**
     * @Route("/blog/{date}/{slug}/comments/{comment_slug}")
     * @ParamConverter("post", options={"mapping": {"date": "date", "slug": "slug"})
     * @ParamConverter("comment", options={"mapping": {"comment_slug": "slug"})
     */
    public function showAction(Post $post, Comment $comment)
    {
    }

Si vous faites correspondre une entité en utilisant plusieurs champs, et si vous
voulez exclure l'un des paramètres de la route des critères de sélection de l'entité,
vous pouvez procéder comme suit::

    /**
     * @Route("/blog/{date}/{slug}")
     * @ParamConverter("post", options={"exclude": ["date"]})
     */
    public function showAction(Post $post, \DateTime $date)
    {
    }

Convertisseur DateTime
~~~~~~~~~~~~~~~~~~~~~~

Nom du convertisseur: ``datetime``

Le convertisseur DateTime convertit une route ou un attribut de requête en
une instance de DateTime::

    /**
     * @Route("/blog/archive/{start}/{end}")
     */
    public function archiveAction(\DateTime $start, DateTime $end)
    {
    }

Par défaut, tout format de date qui peut être analysé par le constructeur de
``DateTime`` est accepté. Vous pouvez cependant être plus restrictif en passant
le format en option::

    /**
     * @Route("/blog/archive/{start}/{end}")
     * @ParamConverter("start", options={"format": "Y-m-d"})
     * @ParamConverter("end", options={"format": "Y-m-d"})
     */
    public function archiveAction(\DateTime $start, DateTime $end)
    {
    }

Créer un convertisseur
----------------------

Tout les convertisseurs doivent implémenter
:class:`Sensio\\Bundle\\FrameworkExtraBundle\\Request\\ParamConverter\\ParamConverterInterface`::

    namespace Sensio\Bundle\FrameworkExtraBundle\Request\ParamConverter;

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\ConfigurationInterface;
    use Symfony\Component\HttpFoundation\Request;

    interface ParamConverterInterface
    {
        function apply(Request $request, ConfigurationInterface $configuration);

        function supports(ConfigurationInterface $configuration);
    }

La méthode ``supports()`` doit retourner ``true`` quand elle est capable de convertir
la configuration donnée (une instance de ``ParamConverter``).

L'instance de ``ParamConverter`` possède trois informations pour l'annotation :

* ``name``: Le nom de l'attribut ;
* ``class``: Le nom de la classe de l'attribut (toute chaîne de caractères qui représente
  un nom de classe) ;
* ``options``: Un tableau d'options.

La méthode ``apply()`` est appelée chaque fois que la configuration est supportée.
Basée sur les attributs de la requête, elle définit un attribut nommé
``$configuration->getName()``, qui stocke un objet de la classe ``$configuration->getClass()``.

Pour enregistrer votre service convertisseur, vous devez y ajouter un tag :

.. configuration-block::

    .. code-block:: xml

        <service id="my_converter" class="MyBundle/Request/ParamConverter/MyConverter">
            <tag name="request.param_converter" priority="-2" name="my_converter" />
        </service>

Vous pouvez enregistrer un convertisseur par priorité, par nom ou les deux. Si
vous ne spécifiez pas de priorité ou de nom, le convertisseur sera ajouté à la pile
avec une priorité `0`. Pour explicitement désactiver l'enregistrement par priorité,
vous devez définir `priority="false"` dans votre définition de tag.

.. tip::

    Utilisez la classe ``DoctrineParamConverter`` comme modèle pour vos propres
    convertisseurs.
