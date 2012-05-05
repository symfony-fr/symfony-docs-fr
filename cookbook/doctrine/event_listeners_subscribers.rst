.. _doctrine-event-config:

Enregistrer des listeners (« écouteurs » en français) et des souscripteurs d'évènement
======================================================================================

Doctrine vient avec un riche système d'évènement qui lance des évènements à
chaque fois - ou presque - que quelque chose se passe dans le système. Pour vous,
cela signifie que vous pouvez créer arbitrairement des :doc:`services</book/service_container>`
et dire à Doctrine de notifier ces objets à chaque fois qu'une certaine
action (par exemple : ``prePersist``) a lieu dans Doctrine. Cela pourrait être
utile par exemple de créer un index de recherche indépendant à chaque fois
qu'un objet est sauvegardé dans votre base de données.

Doctrine définit deux types d'objets qui peuvent « écouter » des évènements
Doctrine : les listeners et les souscripteurs d'évènement. Les deux sont très
similaires, mais les listeners sont un peu plus simples. Pour plus d'informations,
voir `The Event System`_ sur le site de Doctrine.

Configurer le listener/souscripteur d'évènement
-----------------------------------------------

Pour spécifier à un service d'agir comme un listener d'évènements ou comme un
souscripteur, vous devez simplement le :ref:`tagger<book-service-container-tags>`
avec un nom approprié. Dépendant de votre cas, vous pouvez placer un listener
dans chaque connexion DBAL et gestionnaire d'entité ORM ou juste dans une connexion
DBAL spécifique et tous les gestionnaires d'entité qui utilise cette connexion.

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                default_connection: default
                connections:
                    default:
                        driver: pdo_sqlite
                        memory: true

        services:
            my.listener:
                class: Acme\SearchBundle\Listener\SearchIndexer
                tags:
                    - { name: doctrine.event_listener, event: postPersist }
            my.listener2:
                class: Acme\SearchBundle\Listener\SearchIndexer2
                tags:
                    - { name: doctrine.event_listener, event: postPersist, connection: default }
            my.subscriber:
                class: Acme\SearchBundle\Listener\SearchIndexerSubscriber
                tags:
                    - { name: doctrine.event_subscriber, connection: default }

    .. code-block:: xml

        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine">

            <doctrine:config>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection driver="pdo_sqlite" memory="true" />
                </doctrine:dbal>
            </doctrine:config>

            <services>
                <service id="my.listener" class="Acme\SearchBundle\Listener\SearchIndexer">
                    <tag name="doctrine.event_listener" event="postPersist" />
                </service>
                <service id="my.listener2" class="Acme\SearchBundle\Listener\SearchIndexer2">
                    <tag name="doctrine.event_listener" event="postPersist" connection="default" />
                </service>
                <service id="my.subscriber" class="Acme\SearchBundle\Listener\SearchIndexerSubscriber">
                    <tag name="doctrine.event_subscriber" connection="default" />
                </service>
            </services>
        </container>

Créer la Classe du Listener
---------------------------

Dans l'exemple précédent, un service ``my.listener`` a été configuré en tant que
listener Doctrine sur l'évènement ``postPersist``. Cette classe derrière ce
service doit avoir une méthode ``postPersist``, qui va être appelée lorsque
l'évènement est lancé::

    // src/Acme/SearchBundle/Listener/SearchIndexer.php
    namespace Acme\SearchBundle\Listener;
    
    use Doctrine\ORM\Event\LifecycleEventArgs;
    use Acme\StoreBundle\Entity\Product;
    
    class SearchIndexer
    {
        public function postPersist(LifecycleEventArgs $args)
        {
            $entity = $args->getEntity();
            $entityManager = $args->getManager();
            
            // peut-être vous voulez seulement agir sur une entité « Product »
            if ($entity instanceof Product) {
                // fait quelque chose avec le « Product »
            }
        }
    }

Dans chaque évènement, vous avez accès à un objet ``LifecycleEventArgs``,
qui vous donne accès à l'objet entité de l'évènement ainsi qu'au gestionnaire
d'entité lui-même.

Une chose importante à noter est qu'un listener va écouter *toutes* les entités
de votre application. Donc, si vous êtes intéressé de gérer uniquement un type
spécifique d'entité (par exemple : une entité ``Product`` mais pas une entité
``BlogPost``), vous devriez vérifier le nom de la classe de votre entité dans
votre méthode (comme montré ci-dessus).

.. _`The Event System`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/events.html