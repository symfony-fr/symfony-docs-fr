.. index::
   single: Logging

Comment Logger des Messages dans différents Fichiers
====================================================

.. versionadded:: 2.1
    La possibilité de spécifier des canaux pour un gestionnaire spécifique
    a été ajouté au MonologBundle dans Symfony 2.1.

L'Edition Standard de Symfony contient plusieurs canaux pour le logging :
``doctrine``, ``event``, ``security`` et ``request``. Chaque canal correspond
à un service de « logger » (``monolog.logger.XXX``) dans le conteneur et est
injecté dans le service concerné. Le but des canaux est d'être capable d'organiser
différents types de messages de logging.

Par défaut, Symfony2 loggue tous les messages dans un fichier unique (peu
importe le canal).

Transférer un Canal vers un Gestionnaire différent
--------------------------------------------------

Maintenant, supposons que vous vouliez logguer le canal ``doctrine`` dans
un fichier différent.

Pour faire cela, créez simplement un nouveau gestionnaire et configurez-le
comme ceci :

.. configuration-block::

    .. code-block:: yaml

        monolog:
            handlers:
                main:
                    type: stream
                    path: /var/log/symfony.log
                    channels: !doctrine
                doctrine:
                    type: stream
                    path: /var/log/doctrine.log
                    channels: doctrine

    .. code-block:: xml

        <monolog:config>
            <monolog:handlers>
                <monolog:handler name="main" type="stream" path="/var/log/symfony.log">
                    <monolog:channels>
                        <type>exclusive</type>
                        <channel>doctrine</channel>
                    </monolog:channels>
                </monolog:handler>

                <monolog:handler name="doctrine" type="stream" path="/var/log/doctrine.log" />
                    <monolog:channels>
                        <type>inclusive</type>
                        <channel>doctrine</channel>
                    </monolog:channels>
                </monolog:handler>
            </monolog:handlers>
        </monolog:config>

Spécification Yaml
------------------

Vous pouvez spécifier la configuration de différentes façons :

.. code-block:: yaml

    channels: ~    # Inclus tous les canaux

    channels: foo  # Inclus seulement le canal "foo"
    channels: !foo # Inclus tous les canaux, excepté "foo"

    channels: [foo, bar]   # Inclus seulement les canaux "foo" et "bar"
    channels: [!foo, !bar] # Inclus tous les canaux, excepté "foo" et "bar"

    channels:
        type:     inclusive # Inclus seulement ceux listés ci-dessous
        elements: [ foo, bar ]
    channels:
        type:     exclusive # Inclus tous, excepté ceux listés ci-dessous
        elements: [ foo, bar ]


En savoir plus grâce au Cookbook
--------------------------------

* :doc:`/cookbook/logging/monolog`
