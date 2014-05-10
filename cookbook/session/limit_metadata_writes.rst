.. index::
    single: Limiter les Écritures de Metadata; Session

Limiter les Écritures de Metadonnées en Session
===============================================

.. versionadded:: 2.4
    La capacité de limiter les écritures de métadonnées en session a été
    introduite dans Symfony 2.4.

Le comportement par défaut de la session PHP est de persister la session
indépendamment du fait que les données de la session aient changé ou pas.
Dans Symfony, à chaque fois que l'on accède à la session, les métadonnées
sont enregistrées (session créée/utilisée pour la dernière fois) qui peuvent être
utilisées pour déterminer l'âge de la session et l'idle time (temps d'inactivité).

Si pour des raisons de performance vous souhaitez limiter la fréquence à laquelle
la session est persistée, cette fonctionnalité peut ajuster la granularité
à laquelle les métadonnées sont mises à jour et persister la session moins souvent
tout en gardant des métadonnées précises. Si d'autres données en session sont
changées, la session sera toujours persistée.

Il vous est possible de dire à Symfony de ne pas mettre à jour les métadonnées
de la "session dernièrement modifiée" jusqu'à ce qu'un certain temps
soit passé, en fixant ``framework.session.metadata_update_threshold`` à une
valeur en seconde plus grande que zéro :

.. configuration-block::

    .. code-block:: yaml

        framework:
            session:
                metadata_update_threshold: 120

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>
                <framework:session metadata-update-threshold="120" />
            </framework:config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('framework', array(
            'session' => array(
                'metadata_update_threshold' => 120,
            ),
        ));

.. note::

    Le comportement par défaut de PHP est de sauvegarder la session qu'elle ait
    été modifée ou pas. En utilisant ``framework.session.metadata_update_threshold``
    Symfony décorera le session handler (configuré avec ``framework.session.handler_id``)
    avec le WriteCheckSessionHandler. Cela préviendra toute écriture dans la session
    si cette dernière n'a pas été modifiée.

.. caution::

    Sachez que si la session n'est pas écrite à chaque requête, cela peut
    être collecté par le garbage collector plus tôt que d'habitude. Cela
    signifie que vos utilisateurs peuvent être déconnectés plus tôt que
    prévu.
