.. index::
    single: Sessions, enregistrer la locale

Faire que la Locale soit "persistente" durant la session de l'utilisateur
=========================================================================

Avant Symfony 2.1, la locale était enregistrée dans un attribut de session
appelé ``_locale``. Depuis 2.1, c'est stocké dans la Request, ce qui signifie
qu'elle n'est pas "persistante" durant la requête de l'utilisateur. Dans cet
article, vous allez apprendre comment faire que la locale de l'utilisateur soit
persistante pour que, une fois fixée, la même locale soit utilisée pour chaque
requête suivante.

Créer un LocaleListener
-----------------------

Pour simuler le fait que la locale soit stockée en session, vous devez créer et
enregistrer :doc:`un nouvel event listener </cookbook/service_container/event_listener>`
(écouteur d'évènement).
Le listener ressemblera à quelque chose comme le code qui suit. Typiquement,
``_locale`` est utilisé comme paramètre du routing pour indiquer la locale, ceci
dit la manière dont vous déterminez la locale désirée pour une requête n'est pas
important ::

    // src/Acme/LocaleBundle/EventListener/LocaleListener.php
    namespace Acme\LocaleBundle\EventListener;

    use Symfony\Component\HttpKernel\Event\GetResponseEvent;
    use Symfony\Component\HttpKernel\KernelEvents;
    use Symfony\Component\EventDispatcher\EventSubscriberInterface;

    class LocaleListener implements EventSubscriberInterface
    {
        private $defaultLocale;

        public function __construct($defaultLocale = 'en')
        {
            $this->defaultLocale = $defaultLocale;
        }

        public function onKernelRequest(GetResponseEvent $event)
        {
            $request = $event->getRequest();
            if (!$request->hasPreviousSession()) {
                return;
            }

            // on essaie de voir si la locale a été fixée dans le paramètre de routing _locale
            if ($locale = $request->attributes->get('_locale')) {
                $request->getSession()->set('_locale', $locale);
            } else {
                // si aucune locale n'a été fixée explicitement dans la requête, on utilise celle de la session
                $request->setLocale($request->getSession()->get('_locale', $this->defaultLocale));
            }
        }

        public static function getSubscribedEvents()
        {
            return array(
                // doit être enregistré avant le Locale listener par défaut
                KernelEvents::REQUEST => array(array('onKernelRequest', 17)),
            );
        }
    }

Puis enregistrez le listener :

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_locale.locale_listener:
                class: Acme\LocaleBundle\EventListener\LocaleListener
                arguments: ["%kernel.default_locale%"]
                tags:
                    - { name: kernel.event_subscriber }

    .. code-block:: xml

        <service id="acme_locale.locale_listener"
            class="Acme\LocaleBundle\EventListener\LocaleListener">
            <argument>%kernel.default_locale%</argument>

            <tag name="kernel.event_subscriber" />
        </service>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $container
            ->setDefinition('acme_locale.locale_listener', new Definition(
                'Acme\LocaleBundle\EventListener\LocaleListener',
                array('%kernel.default_locale%')
            ))
            ->addTag('kernel.event_subscriber')
        ;

Et voilà ! Maintenant célébrons en changeant la locale de l'utilisateur qui
est persisté au fil des requêtes. Souvenez-vous, pour récupérer la locale de
l'utilisateur, utilisez toujours la méthode
:method:`Request::getLocale <Symfony\\Component\\HttpFoundation\\Request::getLocale>` ::

    // depuis un controller...
    use Symfony\Component\HttpFoundation\Request;

    public function indexAction(Request $request)
    {
        $locale = $request->getLocale();
    }
