Comment gérer les uploads de fichier avec Doctrine
==================================================

Gérer les uploads de fichier avec les entités Doctrine n'est en rien différent
que de gérer n'importe quel autre upload. En d'autres termes, vous êtes libre
de déplacer le fichier via votre contrôleur après avoir géré la soumission du
formulaire. Pour voir des exemples, référez-vous à la page de
:doc:`référence du type fichier</reference/forms/types/file>`.

Si vous le choisissez, vous pouvez aussi intégrer l'upload de fichier dans le
cycle de vie de votre entité (i.e. création, mise à jour et suppression). Dans
ce cas, lorsque votre entité est créée, mise à jour et supprimée de Doctrine,
les processus d'upload et de suppression du fichier se feront de manière
automatique (sans avoir besoin de faire quoi que ce soit dans votre contrôleur).

Pour que cela fonctionne, vous allez avoir besoin de prendre en compte un
certain nombre de détails qui vont être couverts dans cet article du cookbook.

Installation Basique
--------------------

Tout d'abord, créez une classe Entité Doctrine simple avec laquelle nous
allons travailler::

    // src/Acme/DemoBundle/Entity/Document.php
    namespace Acme\DemoBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Symfony\Component\Validator\Constraints as Assert;

    /**
     * @ORM\Entity
     */
    class Document
    {
        /**
         * @ORM\Id
         * @ORM\Column(type="integer")
         * @ORM\GeneratedValue(strategy="AUTO")
         */
        public $id;

        /**
         * @ORM\Column(type="string", length=255)
         * @Assert\NotBlank
         */
        public $name;

        /**
         * @ORM\Column(type="string", length=255, nullable=true)
         */
        public $path;

        public function getAbsolutePath()
        {
            return null === $this->path ? null : $this->getUploadRootDir().'/'.$this->path;
        }

        public function getWebPath()
        {
            return null === $this->path ? null : $this->getUploadDir().'/'.$this->path;
        }

        protected function getUploadRootDir()
        {
            // le chemin absolu du répertoire où les documents uploadés doivent être sauvegardés
            return __DIR__.'/../../../../web/'.$this->getUploadDir();
        }

        protected function getUploadDir()
        {
            // on se débarrasse de « __DIR__ » afin de ne pas avoir de problème lorsqu'on affiche
            // le document/image dans la vue.
            return 'uploads/documents';
        }
    }

L'entité ``Document`` a un nom et est associée à un fichier. La propriété ``path``
stocke le chemin relatif du fichier et est persistée dans la base de données.
La méthode ``getAbsolutePath()`` est un moyen pratique de retourner le chemin absolu
du fichier alors que la méthode ``getWebPath()`` permet de retourner le chemin web, qui
lui peut être utilisé dans un template pour ajouter un lien vers le fichier uploadé.

.. tip::

    Si vous ne l'avez pas déjà fait, vous devriez probablement lire la
    documentation du type :doc:`fichier</reference/forms/types/file>` en
    premier afin de comprendre comment le processus basique d'upload
    fonctionne.

.. note::

    Si vous utilisez les annotations pour spécifier vos régles de
    validation (comme montré dans cet exemple), assurez-vous d'avoir
    activé la validation via les annotations (voir
    :ref:`configuration de la validation<book-validation-configuration>`).

Pour gérer l'upload de fichier dans le formulaire, utilisez un champ « virtuel »
``file``. Par exemple, si vous construisez votre formulaire directement dans un
contrôleur, cela ressemblerait à quelque chose comme ça::

    public function uploadAction()
    {
        // ...

        $form = $this->createFormBuilder($document)
            ->add('name')
            ->add('file')
            ->getForm()
        ;

        // ...
    }

Ensuite, créez cette propriété dans votre classe ``Document`` et ajoutez quelques
règles de validation::

    // src/Acme/DemoBundle/Entity/Document.php

    // ...
    class Document
    {
        /**
         * @Assert\File(maxSize="6000000")
         */
        public $file;

        // ...
    }

.. note::

    Comme vous utilisez la contrainte ``File``, Symfony2 va automatiquement
    deviner que le champ du formulaire est un champ d'upload de fichier.
    C'est pourquoi vous n'avez pas eu à le spécifier explicitement lors
    de la création du formulaire ci-dessus (``->add('file')``).

Le contrôleur suivant vous montre comment gérer le processus en entier::

    use Acme\DemoBundle\Entity\Document;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    // ...

    /**
     * @Template()
     */
    public function uploadAction()
    {
        $document = new Document();
        $form = $this->createFormBuilder($document)
            ->add('name')
            ->add('file')
            ->getForm()
        ;

        if ($this->getRequest()->getMethod() === 'POST') {
            $form->bindRequest($this->getRequest());
            if ($form->isValid()) {
                $em = $this->getDoctrine()->getManager();

                $em->persist($document);
                $em->flush();

                $this->redirect($this->generateUrl('...'));
            }
        }

        return array('form' => $form->createView());
    }

.. note::

    Lorsque vous écrivez le template, n'oubliez pas de spécifier
    l'attribut ``enctype`` :

    .. code-block:: html+php

        <h1>Upload File</h1>

        <form action="#" method="post" {{ form_enctype(form) }}>
            {{ form_widget(form) }}

            <input type="submit" value="Upload Document" />
        </form>

Le contrôleur précédent va automatiquement persister l'entité ``Document``
avec le nom soumis, mais elle ne va rien faire à propos du fichier et la
propriété ``path`` sera vide.

Une manière facile de gérer l'upload de fichier est de le déplacer juste avant
que l'entité soit persistée et ainsi spécifier la propriété ``path`` en
conséquence. Commencez par appeler une nouvelle méthode ``upload()`` sur la
classe ``Document`` que vous allez créer dans un moment pour gérer l'upload
de fichier::

    if ($form->isValid()) {
        $em = $this->getDoctrine()->getManager();

        $document->upload();

        $em->persist($document);
        $em->flush();

        $this->redirect('...');
    }

La méthode ``upload()`` va tirer parti de l'objet :class:`Symfony\\Component\\HttpFoundation\\File\\UploadedFile`,
qui correspond à ce qui est retourné après qu'un champ ``file`` ait été soumis::

    public function upload()
    {
        // la propriété « file » peut être vide si le champ n'est pas requis
        if (null === $this->file) {
            return;
        }

        // nous utilisons le nom de fichier original ici mais
        // vous devriez « l'assainir » pour au moins éviter
        // quelconques problèmes de sécurité
        
        // la méthode « move » prend comme arguments le répertoire cible et
        // le nom de fichier cible où le fichier doit être déplacé
        $this->file->move($this->getUploadRootDir(), $this->file->getClientOriginalName());

        // définit la propriété « path » comme étant le nom de fichier où vous
        // avez stocké le fichier
        $this->path = $this->file->getClientOriginalName();

        // « nettoie » la propriété « file » comme vous n'en aurez plus besoin
        $this->file = null;
    }

Utiliser les callbacks du « cycle de vie » (lifecycle)
------------------------------------------------------

Même si cette implémentation fonctionne, elle souffre d'un défaut majeur : que
se passe-t-il s'il y a un problème lorsque l'entité est persistée ? Le fichier
serait déjà déplacé vers son emplacement final même si la propriété ``path``
de l'entité n'a pas été persistée correctement.

Pour éviter ces problèmes, vous devriez changer l'implémentation afin que les
opérations sur la base de données et le déplacement du fichier deviennent
atomiques : s'il y a un problème en persistant l'entité ou si le fichier ne
peut pas être déplacé, alors *rien* ne devrait se passer.

Pour faire cela, vous devez déplacer le fichier aussitôt que Doctrine persiste
l'entité dans la base de donnés. Ceci peut être accompli en s'interférant dans
le cycle de vie de l'entité via un callback::

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
    }

Ensuite, réfactorez la classe ``Document`` pour tirer parti de ces callbacks::

    use Symfony\Component\HttpFoundation\File\UploadedFile;

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
        /**
         * @ORM\PrePersist()
         * @ORM\PreUpdate()
         */
        public function preUpload()
        {
            if (null !== $this->file) {
                // faites ce que vous voulez pour générer un nom unique
                $this->path = uniqid().'.'.$this->file->guessExtension();
            }
        }

        /**
         * @ORM\PostPersist()
         * @ORM\PostUpdate()
         */
        public function upload()
        {
            if (null === $this->file) {
                return;
            }

            // s'il y a une erreur lors du déplacement du fichier, une exception
            // va automatiquement être lancée par la méthode move(). Cela va empêcher
            // proprement l'entité d'être persistée dans la base de données si
            // erreur il y a
            $this->file->move($this->getUploadRootDir(), $this->path);

            unset($this->file);
        }

        /**
         * @ORM\PostRemove()
         */
        public function removeUpload()
        {
            if ($file = $this->getAbsolutePath()) {
                unlink($file);
            }
        }
    }

La classe effectue maintenant tout ce dont vous avez besoin : elle génère un nom
de fichier unique avant de le persister, déplace le fichier après avoir persisté
l'entité, et efface le fichier si l'entité est supprimée.

.. note::

    Les évènements de callback ``@ORM\PrePersist()`` et ``@ORM\PostPersist()``
    sont déclenchés avant et après que l'entité soit persistée dans la base
    de données. D'autre part, les évènements de callback ``@ORM\PreUpdate()``
    et ``@ORM\PostUpdate()`` sont appelés lorsque l'entité est mise à jour.

.. caution::

    Les callbacks ``PreUpdate`` et ``PostUpdate`` sont déclenchés seulement s'il
    y a un changement dans l'un des champs de l'entité étant persistée. Cela
    signifie que, par défaut, si vous modifiez uniquement la propriété ``$file``,
    ces évènements ne seront pas déclenchés, comme la propriété elle-même n'est pas
    directement persistée via Doctrine. Une solution pourrait être d'utiliser un
    champ ``updated`` qui soit persisté dans Doctrine, et de le modifier manuellement
    lorsque le fichier est changé.

Utiliser l'``id`` en tant que nom de fichier
--------------------------------------------

Si vous voulez utiliser l'``id`` comme nom de fichier, l'implémentation est
légèrement différente car vous devez sauvegarder l'extension dans la propriété
``path``, à la place du nom de fichier actuel::

    use Symfony\Component\HttpFoundation\File\UploadedFile;

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
        /**
         * @ORM\PrePersist()
         * @ORM\PreUpdate()
         */
        public function preUpload()
        {
            if (null !== $this->file) {
                $this->path = $this->file->guessExtension();
            }
        }

        /**
         * @ORM\PostPersist()
         * @ORM\PostUpdate()
         */
        public function upload()
        {
            if (null === $this->file) {
                return;
            }

            // vous devez lancer une exception ici si le fichier ne peut pas
            // être déplacé afin que l'entité ne soit pas persistée dans la
            // base de données comme le fait la méthode move() de UploadedFile
            $this->file->move($this->getUploadRootDir(), $this->id.'.'.$this->file->guessExtension());

            unset($this->file);
        }

        /**
         * @ORM\PostRemove()
         */
        public function removeUpload()
        {
            if ($file = $this->getAbsolutePath()) {
                unlink($file);
            }
        }

        public function getAbsolutePath()
        {
            return null === $this->path ? null : $this->getUploadRootDir().'/'.$this->id.'.'.$this->path;
        }
    }
