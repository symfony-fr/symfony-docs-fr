.. index::
   single: Logging; Emailling errors

Comment configurer Monolog pour envoyer les erreurs par Email
=============================================================

Monolog_ peut être configuré pour envoyer un email lorsqu'une erreur se
produit dans une application. Pour ce faire, la configuration nécessite quelques
gestionnaires « imbriqués » afin d'éviter de recevoir trop d'emails. Cette
configuration paraît compliquée en premier lieu mais chaque gestionnaire
est facilement compréhensible lorsqu'on les analyse un par un.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        monolog:
            handlers:
                mail:
                    type:         fingers_crossed
                    action_level: critical
                    handler:      buffered
                buffered:
                    type:    buffer
                    handler: swift
                swift:
                    type:       swift_mailer
                    from_email: error@example.com
                    to_email:   error@example.com
                    subject:    An Error Occurred!
                    level:      debug

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/monolog http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <monolog:config>
                <monolog:handler
                    name="mail"
                    type="fingers_crossed"
                    action-level="critical"
                    handler="buffered"
                />
                <monolog:handler
                    name="buffered"
                    type="buffer"
                    handler="swift"
                />
                <monolog:handler
                    name="swift"
                    from-email="error@example.com"
                    to-email="error@example.com"
                    subject="An Error Occurred!"
                    level="debug"
                />
            </monolog:config>
        </container>

Le gestionnaire ``mail`` est un gestionnaire ``fingers_crossed``, ce qui signifie
qu'il est déclenché uniquement lorsque le niveau d'action, dans notre cas ``critical``
est atteint. Il écrit alors des logs pour tout, incluant les messages en dessous
du niveau d'action. Le niveau ``critical`` est déclenché seulement pour les erreurs
HTTP de code 5xx. Le paramètre « handler » signifie que la « sortie » (« output »
en anglais) est alors passée au gestionnaire ``buffered``.

.. tip::
    Si vous souhaitez que les erreurs de niveau 400 et 500 déclenchent un email,
    définissez ``action_level`` avec la valeur ``error`` à la place de ``critical``.

Le gestionnaire ``buffered`` garde simplement tous les messages pour une requête
et les passe ensuite au gestionnaire « imbriqué » en une seule fois. Si vous
n'utilisez pas ce gestionnaire alors chaque message sera envoyé par email
séparément. C'est donc le gestionnaire ``swift`` qui prend le relais. C'est ce
dernier qui se charge de vous envoyer l'erreur par email. Ses paramètres
sont simples : « to » (« à » en français), « from » (« de » en français) et le
sujet.

Vous pouvez combiner ces gestionnaires avec d'autres afin que les erreurs continuent
d'être loguées sur le serveur en même temps qu'elles sont envoyées par email :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        monolog:
            handlers:
                main:
                    type:         fingers_crossed
                    action_level: critical
                    handler:      grouped
                grouped:
                    type:    group
                    members: [streamed, buffered]
                streamed:
                    type:  stream
                    path:  "%kernel.logs_dir%/%kernel.environment%.log"
                    level: debug
                buffered:
                    type:    buffer
                    handler: swift
                swift:
                    type:       swift_mailer
                    from_email: error@example.com
                    to_email:   error@example.com
                    subject:    An Error Occurred!
                    level:      debug

    .. code-block:: xml

         <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/monolog http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <monolog:config>
                <monolog:handler
                    name="main"
                    type="fingers_crossed"
                    action_level="critical"
                    handler="grouped"
                />                
                <monolog:handler
                    name="grouped"
                    type="group"
                >
                    <member type="stream"/>
                    <member type="buffered"/>
                </monolog:handler>
                <monolog:handler
                    name="stream"
                    path="%kernel.logs_dir%/%kernel.environment%.log"
                    level="debug"
                />
                <monolog:handler
                    name="buffered"
                    type="buffer"
                    handler="swift"
                />
                <monolog:handler
                    name="swift"
                    from-email="error@example.com"
                    to-email="error@example.com"
                    subject="An Error Occurred!"
                    level="debug"
                />
            </monolog:config>
        </container>

Cette configuration utilise le gestionnaire ``group`` pour envoyer les messages
aux deux membres du groupe, les gestionnaires ``buffered`` et ``stream``. Les
messages vont donc maintenant être écrits dans le fichier log et envoyés par email.

.. _Monolog: https://github.com/Seldaek/monolog