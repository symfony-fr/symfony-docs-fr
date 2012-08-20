.. index::
   single: Emails; In development

Comment travailler avec les Emails pendant le Développement
===========================================================

Lorsque vous développez une application qui envoie des emails, vous
voudrez souvent que cette dernière n'en envoie pas au destinaire
spécifié pendant le développement. Si vous utilisez le ``SwiftmailerBundle``
avec Symfony2, vous pouvez facilement réaliser cela à travers les paramètres
de configuration sans avoir à faire de quelconques changements dans votre code.
Il y a deux choix principaux possibles lorsqu'on parle de gestion d'emails
durant le développement : (a) désactiver complètement l'envoi d'emails ou
(b) envoyer tous les emails à une adresse spécifique.

Désactiver l'Envoi
------------------

Vous pouvez désactiver l'envoi d'emails en définissant l'option
``disable_delivery`` à ``true``. C'est ce qui est fait par défaut
dans l'environnement ``test`` dans la distribution Standard. Si vous
faites cela dans la configuration spécifique ``test``, alors les emails ne seront
pas envoyés lorsque vous exécuterez les tests, mais continueront d'être
envoyés dans les environnements ``prod`` et ``dev`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        swiftmailer:
            disable_delivery:  true

    .. code-block:: xml

        <!-- app/config/config_test.xml -->

        <!--
            xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
            http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            disable-delivery="true" />

    .. code-block:: php

        // app/config/config_test.php
        $container->loadFromExtension('swiftmailer', array(
            'disable_delivery'  => "true",
        ));

Si vous voulez aussi désactiver l'envoi dans l'environnement ``dev``,
ajoutez tout simplement la même configuration dans le fichier ``config_dev.yml``.

Envoyer à une Adresse Spécifique
--------------------------------

Vous pouvez aussi choisir d'avoir tous les emails envoyés à une adresse
spécifique à la place de l'adresse réellement spécifiée lors de l'envoi
du message. Ceci peut être accompli via l'option ``delivery_address`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        swiftmailer:
            delivery_address:  dev@example.com

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->

        <!--
            xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
            http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            delivery-address="dev@example.com" />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('swiftmailer', array(
            'delivery_address'  => "dev@example.com",
        ));

Maintenant, supposons que vous envoyiez un email à ``recipient@example.com``.

.. code-block:: php

    public function indexAction($name)
    {
        $message = \Swift_Message::newInstance()
            ->setSubject('Hello Email')
            ->setFrom('send@example.com')
            ->setTo('recipient@example.com')
            ->setBody($this->renderView('HelloBundle:Hello:email.txt.twig', array('name' => $name)))
        ;
        $this->get('mailer')->send($message);

        return $this->render(...);
    }

Dans l'environnement ``dev``, l'email sera envoyé à ``dev@example.com``.
Swiftmailer ajoutera un en-tête supplémentaire à l'email, ``X-Swift-To``, contenant
l'adresse remplacée, afin que vous puissiez toujours voir à qui il aurait été
envoyé.

.. note::

    En plus des adresses ``to``, cela va aussi stopper les emails envoyés
    à n'importe quelle adresse des champs ``CC`` et ``BCC``. Swiftmailer
    ajoutera des en-têtes additionnels à l'email contenant les adresses
    surchargées. Ces en-têtes sont respectivement ``X-Swift-Cc`` et
    ``X-Swift-Bcc`` pour les adresses de ``CC`` et ``BCC``.

Voir les informations depuis la Barre d'Outils de Débuggage Web
---------------------------------------------------------------

Vous pouvez voir tout email envoyé durant une unique réponse lorsque vous
êtes dans l'environnement ``dev`` via la Barre d'Outils de Débuggage.
L'icône d'email dans la barre d'outils montrera combien d'emails ont été
envoyés. Si vous cliquez dessus, un rapport s'ouvrira montrant les détails
des emails envoyés.

Si vous envoyez un email et puis redirigez immédiatement vers une autre page,
la barre d'outils de débuggage n'affichera pas d'icône d'email ni de rapport
sur la page d'après.

A la place, vous pouvez définir l'option ``intercept_redirects`` comme étant
``true`` dans le fichier ``config_dev.yml``, ce qui va forcer les redirections
à s'arrêter et à vous permettre d'ouvrir le rapport avec les détails des emails
envoyés.

.. tip::

    Sinon, vous pouvez ouvrir le profiler après la redirection et rechercher
    par l'URL soumise et utilisée lors de la requête précédente (par exemple :
    ``/contact/handle``). La fonctionnalité de recherche du profiler vous
    permet de charger les informations du profiler pour toutes les requêtes
    passées.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        web_profiler:
            intercept_redirects: true

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->

        <!--
            xmlns:webprofiler="http://symfony.com/schema/dic/webprofiler"
            xsi:schemaLocation="http://symfony.com/schema/dic/webprofiler http://symfony.com/schema/dic/webprofiler/webprofiler-1.0.xsd"
        -->

        <webprofiler:config
            intercept-redirects="true"
        />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('web_profiler', array(
            'intercept_redirects' => 'true',
        ));
