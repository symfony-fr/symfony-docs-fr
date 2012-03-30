.. index::
   single: Emails

Comment envoyer un Email
========================

Envoyer des emails est une tâche classique pour n'importe quelle application web
et une qui possède des complications et pièges potentiels. A la place de réinventer
la roue, une solution pour envoyer des emails est d'utiliser le ``SwiftmailerBundle``,
qui tire partie de la puissance de la bibliothèque `Swiftmailer`_.

.. note::

    N'oubliez pas d'activer le bundle dans votre Kernel avant de l'utiliser::

        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
            );

            // ...
        }

.. _swift-mailer-configuration:

Configuration
-------------

Avant d'utiliser Swiftmailer, soyez sûr d'inclure sa configuration. Le
seul paramètre obligatoire de la configuration est ``transport`` :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer:
            transport:  smtp
            encryption: ssl
            auth_mode:  login
            host:       smtp.gmail.com
            username:   your_username
            password:   your_password

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            transport="smtp"
            encryption="ssl"
            auth-mode="login"
            host="smtp.gmail.com"
            username="your_username"
            password="your_password" />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('swiftmailer', array(
            'transport'  => "smtp",
            'encryption' => "ssl",
            'auth_mode'  => "login",
            'host'       => "smtp.gmail.com",
            'username'   => "your_username",
            'password'   => "your_password",
        ));

La plus grande partie de la configuration de Swiftmailer traite de comment
les messages eux-mêmes devraient être délivrés.

Les attributs de configuration suivants sont disponibles :

* ``transport``         (``smtp``, ``mail``, ``sendmail``, ou ``gmail``)
* ``username``
* ``password``
* ``host``
* ``port``
* ``encryption``        (``tls``, or ``ssl``)
* ``auth_mode``         (``plain``, ``login``, or ``cram-md5``)
* ``spool``

  * ``type`` (comment mettre les messages dans une queue, seulement ``file`` est supporté pour le moment)
  * ``path`` (où stocker les messages)
* ``delivery_address``  (une adresse email où envoyer TOUS les emails)
* ``disable_delivery``  (définir à true pour désactiver entièrement la livraison des emails)

Envoyer des Emails
------------------

La bibliothèque Swiftmailer fonctionne grâce à la création, la configuration
et enfin à l'envoi d'objets ``Swift_Message``. Le « mailer » est responsable
de l'actuelle livraison du message et est accessible via le service ``mailer``.
Globalement, envoyer un email est assez simple::

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

Pour garder les choses découplées, le corps de l'email a été stocké dans un
template et est rendu grâce à la méthode ``renderView()``.

L'objet ``$message`` supporte beaucoup plus d'options, comme inclure des pièces
jointes, ajouter du contenu HTML, et bien plus encore. Heureusement, Swiftmailer
couvre le thème de la `Création de Messages`_ de manière très détaillée dans sa
documentation.

.. tip::

    Plusieurs articles du cookbook liés à l'envoi d'emails dans Symfony2 sont
    disponibles :

    * :doc:`gmail`
    * :doc:`dev_environment`
    * :doc:`spool`

.. _`Swiftmailer`: http://www.swiftmailer.org/
.. _`Création de Messages`: http://swiftmailer.org/docs/messages