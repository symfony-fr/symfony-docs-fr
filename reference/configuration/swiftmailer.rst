.. index::
   single: Configuration de référence; Swiftmailer

Configuration du SwiftmailerBundle ("swiftmailer")
==================================================

Ce document de référence est un travail en cours. Il devrait être exact, mais
toutes les options ne sont pas encore totalement décrites. Pour une liste
complète des options de configuration par défaut, voir
`Toutes les Options de Configuration par Défaut`_.

La clé ``swiftmailer`` configure l'intégration de Symfony avec Swiftmailer,
qui est responsable de créer et d'envoyer les emails.

Configuration
-------------

* `transport`_
* `username`_
* `password`_
* `host`_
* `port`_
* `encryption`_
* `auth_mode`_
* `spool`_
    * `type`_
    * `path`_
* `sender_address`_
* `antiflood`_
    * `threshold`_
    * `sleep`_
* `delivery_address`_
* `disable_delivery`_
* `logging`_

transport
~~~~~~~~~

**type**: ``string`` **par défaut**: ``smtp``

La méthode de transport exacte à utiliser pour envoyer des emails. Les
valeurs valides sont :

* smtp
* gmail (voir :doc:`/cookbook/email/gmail`)
* mail
* sendmail
* null (revient au même que de définir `disable_delivery`_ à ``true``)

username
~~~~~~~~

**type**: ``string``

Le nom d'utilisateur lors de l'utilisation de ``smtp`` en tant que mode de
transport.

password
~~~~~~~~

**type**: ``string``

Le mot de passe lors de l'utilisation de ``smtp`` en tant que mode de
transport.

host
~~~~

**type**: ``string`` **par défaut**: ``localhost``

Le serveur auquel se connecter lors de l'utilisation de ``smtp`` en tant
que mode de transport.

port
~~~~

**type**: ``string`` **par défaut**: 25 or 465 (dépend de la valeur de `encryption`_)

Le numéro du port lors de l'utilisation de ``smtp`` en tant que mode de
transport. Par défaut, la valeur est 465 si le mode d'encryption est
``ssl``, 25 sinon.

encryption
~~~~~~~~~~

**type**: ``string``

Le mode d'encryption lors de l'utilisation de ``smtp`` en tant que mode de
transport. Les valeurs valides sont ``tls``, ``ssl``, ou ``null``
(indiquant aucune encryption).

auth_mode
~~~~~~~~~

**type**: ``string``

Le mode d'authentification lors de l'utilisation de ``smtp`` en tant que
mode de transport. Les valeurs valides sont ``plain``, ``login``,
``cram-md5``, ou ``null``.

spool
~~~~~

Pour des détails sur l'envoi d'emails en mode « spool », voir
:doc:`/cookbook/email/spool`.

type
....

**type**: ``string`` **par défaut**: ``file``

La méthode utilisée pour stocker les messages du « spool ». Pour le moment,
seulement ``file``est supporté. Cependant, un « spool » personnalisé devrait
être possible en créant un service appelé ``swiftmailer.spool.myspool`` et
en définissant cette valeur à ``myspool``.

path
....

**type**: ``string`` **par défaut**: ``%kernel.cache_dir%/swiftmailer/spool``

Lors de l'utilisation du mode de « spool » ``file``, ceci est le chemin où
les messages du « spool » seront stockés.

sender_address
~~~~~~~~~~~~~~

**type**: ``string``

Si défini, tous les messages seront délivrés avec cette adresse en tant
qu'adresse « à qui répondre », qui indique où les messages en retour
devraient être envoyés. Cela est géré en interne par la classe
``Swift_Plugins_ImpersonatePlugin`` de Swiftmailer.

antiflood
~~~~~~~~~

threshold
.........

**type**: ``string`` **par défaut**: ``99``

Utilisé avec ``Swift_Plugins_AntiFloodPlugin``. Ceci est le nombre d'emails
à envoyer avant de redémarrer le transport.

sleep
.....

**type**: ``string`` **par défaut**: ``0``

Utilisé avec ``Swift_Plugins_AntiFloodPlugin``. Ceci est le nombre d'emails
à mettre en attente lors du redémarrage du transport.

delivery_address
~~~~~~~~~~~~~~~~

**type**: ``string``

Si défini, tous les emails seront envoyés à cette adresse à la place d'être
envoyés à leurs destinaires définis. Cela est souvent pratique lorsque vous
développez. Par exemple, en définissant cela dans le fichier ``config_dev.yml``,
vous pouvez garantir que tous les emails envoyés pendant la phase de
développement seront envoyés vers un compte unique.

Ceci utilise le ``Swift_Plugins_RedirectingPlugin``. Les destinaires d'origine
sont disponibles dans les en-têtes ``X-Swift-To``, ``X-Swift-Cc`` et ``X-Swift-Bcc``.

disable_delivery
~~~~~~~~~~~~~~~~

**type**: ``Boolean`` **par défaut**: ``false``

Si « true », la valeur de ``transport`` va automatiquement être définie
à ``null``, et aucun email ne sera envoyé.

logging
~~~~~~~

**type**: ``Boolean`` **par défaut**: ``%kernel.debug%``

Si « true », le collecteur de données de Symfony sera activé pour Swiftmailer
et les informations seront disponibles dans le profiler.

Toutes les Options de Configuration par Défaut
----------------------------------------------

.. configuration-block::

    .. code-block:: yaml

        swiftmailer:
            transport:            smtp
            username:             ~
            password:             ~
            host:                 localhost
            port:                 false
            encryption:           ~
            auth_mode:            ~
            spool:
                type:                 file
                path:                 "%kernel.cache_dir%/swiftmailer/spool"
            sender_address:       ~
            antiflood:
                threshold:            99
                sleep:                0
            delivery_address:     ~
            disable_delivery:     ~
            logging:              "%kernel.debug%"