.. index::
   single: Sécurité; Chemin Cible de Redirection

Comment changer le Comportement par Défaut du Chemin Cible
==========================================================

Par défaut, le composant de sécurité conserve l'information de l'URI de la dernière
requête dans une variable de session nommée ``_security.target_path``. Lors
d'une connexion réussie, l'utilisateur est redirigé vers ce chemin afin de l'aider
à continuer sa visite en le renvoyant vers la dernière page connue qu'il a parcourue.

Lors de certaines occasions, ceci est inattendu. Par exemple, quand l'URI de la
dernière requête est une méthode POST HTTP avec une route configurée pour
accepter seulement une méthode POST, l'utilisateur est redirigé vers cette
route et se retrouvera confronté inévitablement à une erreur 404.

Pour contourner ce comportement, vous auriez simplement besoin d'étendre la
classe ``ExceptionListener`` et d'outrepasser la méthode par défaut nommée
``setTargetPath()``.

Tout d'abord, outrepassez le paramètre ``security.exception_listener.class``
dans votre fichier de configuration. Cela peut être effectué depuis votre
fichier de configuration principal (dans `app/config`) ou depuis un fichier
de configuration étant importé depuis un bundle :

.. configuration-block::

        .. code-block:: yaml

            # src/Acme/HelloBundle/Resources/config/services.yml
            parameters:
                # ...
                security.exception_listener.class: Acme\HelloBundle\Security\Firewall\ExceptionListener

        .. code-block:: xml

            <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
            <parameters>
                <!-- ... -->
                <parameter key="security.exception_listener.class">Acme\HelloBundle\Security\Firewall\ExceptionListener</parameter>
            </parameters>

        .. code-block:: php

            // src/Acme/HelloBundle/Resources/config/services.php
            // ...
            $container->setParameter('security.exception_listener.class', 'Acme\HelloBundle\Security\Firewall\ExceptionListener');

Ensuite, créez votre propre ``ExceptionListener``::

    // src/Acme/HelloBundle/Security/Firewall/ExceptionListener.php
    namespace Acme\HelloBundle\Security\Firewall;

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\Security\Http\Firewall\ExceptionListener as BaseExceptionListener;

    class ExceptionListener extends BaseExceptionListener
    {
        protected function setTargetPath(Request $request)
        {
            // Ne conservez pas de chemin cible pour les requêtes XHR et non-GET
            // Vous pouvez ajouter n'importe quelle logique supplémentaire ici
            // si vous le voulez
            if ($request->isXmlHttpRequest() || 'GET' !== $request->getMethod()) {
                return;
            }

            $request->getSession()->set('_security.target_path', $request->getUri());
        }
    }

Ajoutez plus ou moins de logique ici comme votre scénario le requiert !