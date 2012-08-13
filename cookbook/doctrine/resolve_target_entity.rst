Définir des Relations avec des Classes Abstraites et des Interfaces
===================================================================

.. versionadded: 2.1
    Le ResolveTargetEntityListener est une nouveauté de Doctrine 2.2, qui a été
    « packagé » pour la première fois avec Symfony 2.1.

L'un des buts des bundles est de créer des ensembles distincts de fonctionnalités
qui n'ont pas beaucoup (ou pas du tout) de dépendances, vous permettant
d'utiliser cette fonctionnalité dans d'autres applications sans inclure
d'éléments superflus.

Doctrine 2.2 inclut un nouvel utilitaire appelé le ``ResolveTargetEntityListener``,
qui fonctionne en interceptant certains appels dans Doctrine et en ré-écrivant
des paramètres ``targetEntity`` dans vos méta-données de correspondance durant
l'exécution. Cela signifie que depuis votre bundle, vous êtes capable d'utiliser
une interface ou une classe abstraite dans vos correspondances et que vous pouvez
vous attendre à une correspondance correcte avec une entité concrète au moment
de l'exécution.

Cette fonctionnalité vous permet de définir des relations entre différentes
entités sans en faire des dépendances « écrites en dur ».

Contexte/décor
--------------

Supposons que vous ayez un `InvoiceBundle` qui fournit une fonctionnalité de
facturation (« invoicing » en anglais) et un `CustomerBundle` qui contient
les outils de gestion de client. Vous souhaitez garder ces deux entités
séparées, car elles peuvent être utilisées dans d'autres systèmes l'une
sans l'autre ; mais pour votre application, vous voulez les utiliser ensemble.

Dans ce cas, vous avez une entité ``Invoice`` ayant une relation avec un
objet qui n'existe pas, une ``InvoiceSubjectInterface``. Le but est de
récupérer le ``ResolveTargetEntityListener`` pour remplacer toute mention de
l'interface par un objet réel qui implémente cette interface.

Mise en place
-------------

Utilisons les entités basiques suivantes (qui sont incomplètes pour plus de
brièveté) pour expliquer comment mettre en place et utiliser le RTEL.

Une entité « Customer »::

    // src/Acme/AppBundle/Entity/Customer.php

    namespace Acme\AppBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Acme\CustomerBundle\Entity\Customer as BaseCustomer;
    use Acme\InvoiceBundle\Model\InvoiceSubjectInterface;

    /**
     * @ORM\Entity
     * @ORM\Table(name="customer")
     */
    class Customer extends BaseCustomer implements InvoiceSubjectInterface
    {
        // Dans notre exemple, toutes les méthodes définies dans
        // l'« InvoiceSubjectInterface » sont déjà implémentées dans le « BaseCustomer »
    }

Une entité « Invoice »::

    // src/Acme/InvoiceBundle/Entity/Invoice.php

    namespace Acme\InvoiceBundle\Entity;

    use Doctrine\ORM\Mapping AS ORM;
    use Acme\InvoiceBundle\Model\InvoiceSubjectInterface;

    /**
     * Représente une « Invoice ».
     *
     * @ORM\Entity
     * @ORM\Table(name="invoice")
     */
    class Invoice
    {
        /**
         * @ORM\ManyToOne(targetEntity="Acme\InvoiceBundle\Model\InvoiceSubjectInterface")
         * @var InvoiceSubjectInterface
         */
        protected $subject;
    }

Une « InvoiceSubjectInterface »::

    // src/Acme/InvoiceBundle/Model/InvoiceSubjectInterface.php

    namespace Acme\InvoiceBundle\Model;

    /**
     * Une interface que le sujet de la facture devrait implémenter.
     * Dans la plupart des circonstances, il ne devrait y avoir qu'un unique objet
     * qui implémente cette interface puisque le ResolveTargetEntityListener peut
     * changer seulement la cible d'un objet unique.
     */
    interface InvoiceSubjectInterface
    {
        // Liste toutes les méthodes additionnelles dont votre
        // InvoiceBundle aura besoin pour accéder au sujet afin
        // que vous soyez sûr que vous avez accès à ces méthodes.

        /**
         * @return string
         */
        public function getName();
    }

Ensuite, vous devez configurer le « listener », qui informe le DoctrineBundle
de votre remplacement :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            # ....
            orm:
                # ....
                resolve_target_entities:
                    Acme\InvoiceBundle\Model\InvoiceSubjectInterface: Acme\AppBundle\Entity\Customer

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:orm>
                    <!-- ... -->
                    <doctrine:resolve-target-entity interface="Acme\InvoiceBundle\Model\InvoiceSubjectInterface">Acme\AppBundle\Entity\Customer</resolve-target-entity>
                </doctrine:orm>
            </doctrine:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'orm' => array(
                // ...
                'resolve_target_entities' => array(
                    'Acme\InvoiceBundle\Model\InvoiceSubjectInterface' => 'Acme\AppBundle\Entity\Customer',
                ),
            ),
        ));

Réflexions finales
------------------

Avec le ``ResolveTargetEntityListener``, vous êtes capable de découpler
vos bundles, en les gardant utilisables par eux-mêmes, mais en étant
toujours capable de définir des relations entre différents objets. En
utilisant cette méthode, vos bundles vont finir par être plus faciles
à maintenir indépendamment.
