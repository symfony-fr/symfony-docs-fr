.. index::
   single: Logging

Comment utiliser Monolog pour écrire des Logs
=============================================

Monolog_ est une bibliothèque pour PHP 5.3 servant à écrire des logs
et utilisée par Symfony2. Elle est inspirée de la bibliothèque
Python LogBook.

Utilisation
-----------

Dans Monolog, chaque « logger » définit un canal de « logging ». Chaque
canal possède une pile de gestionnaires pour écrire les logs (les
gestionnaires peuvent être partagés).

.. tip::

    Lorsque vous injectez le « logger » dans un service, vous pouvez
    :ref:`utiliser un canal personnalisé<dic_tags-monolog>` pour voir
    facilement quelle partie de l'application a loggé le message.

Le gestionnaire par défaut est le ``StreamHandler`` qui écrit les logs
dans un « stream » (par défaut dans le fichier ``app/logs/prod.log`` dans
l'environnement de production et dans ``app/logs/dev.log`` dans l'environnment
de développement).

Monolog est aussi livré avec un puissant gestionnaire intégré pour le « logging »
en environnement de production : le ``FingersCrossedHandler``. Il vous permet
de stocker les messages dans un « buffer » (« mémoire tampon » en français)
et de les écrire dans le log que si un message atteint le niveau d'action
(ERROR dans la configuration fournie dans l'édition standard) en transmettant
les messages à un autre gestionnaire.

Pour « logger » un message, utilisez tout simplement le service logger depuis
le conteneur dans un contrôleur::

    $logger = $this->get('logger');
    $logger->info('Nous avons récupéré le logger');
    $logger->err('Une erreur est survenue');

.. tip::

    Utiliser uniquement les méthodes de l'interface
    :class:`Symfony\\Component\\HttpKernel\\Log\\LoggerInterface` permet
    de changer l'implémentation du logger sans changer votre code.

Utiliser plusieurs gestionnaires
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le logger utilise une pile de gestionnaires qui sont appelés successivement.
Ceci vous permet de « logger » facilement les messages de plusieurs manières.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config*.yml
        monolog:
            handlers:
                syslog:
                    type: stream
                    path: /var/log/symfony.log
                    level: error
                main:
                    type: fingers_crossed
                    action_level: warning
                    handler: file
                file:
                    type: stream
                    level: debug

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/monolog http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <monolog:config>
                <monolog:handler
                    name="syslog"
                    type="stream"
                    path="/var/log/symfony.log"
                    level="error"
                />
                <monolog:handler
                    name="main"
                    type="fingers_crossed"
                    action-level="warning"
                    handler="file"
                />
                <monolog:handler
                    name="file"
                    type="stream"
                    level="debug"
                />
            </monolog:config>
        </container>

La configuration ci-dessus définit une pile de gestionnaires qui vont être
appelés dans l'ordre où ils sont définis.

.. tip::

    Le gestionnaire nommé « file » ne va pas être inclu dans la pile elle-même
    car il est utilisé comme un gestionnaire « imbriqué » du gestionnaire
    ``fingers_crossed``.

.. note::

    Si vous voulez changer la configuration de MonologBundle dans un autre
    fichier de configuration, vous avez besoin de redéfinir tout le bloc.
    Il ne peut pas être fusionné car l'ordre importe et une fusion ne
    permet pas de contrôler ce dernier.

Changer la mise en forme
~~~~~~~~~~~~~~~~~~~~~~~~

Le gestionnaire utilise un ``Formatter`` pour mettre en forme une entrée
avant de la « logger ». Tous les gestionnaires Monolog utilisent une instance
de ``Monolog\Formatter\LineFormatter`` par défaut mais vous pouvez la
remplacer facilement. Votre outil de mise en forme doit implémenter
``Monolog\Formatter\FormatterInterface``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            my_formatter:
                class: Monolog\Formatter\JsonFormatter
        monolog:
            handlers:
                file:
                    type: stream
                    level: debug
                    formatter: my_formatter

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/monolog http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <services>
                <service id="my_formatter" class="Monolog\Formatter\JsonFormatter" />
            </services>
            <monolog:config>
                <monolog:handler
                    name="file"
                    type="stream"
                    level="debug"
                    formatter="my_formatter"
                />
            </monolog:config>
        </container>

Ajouter des données supplémentaires dans les messages de log
------------------------------------------------------------

Monolog permet de traiter l'entrée avant de la « logger » afin d'y
ajouter des données supplémentaires. Un processeur peut être appliqué
pour la pile entière des gestionnaires ou uniquement pour un gestionnaire
spécifique.

Un processeur est simplement un « callable » recevant l'entrée log en tant que
son premier argument.

Les processeurs sont configurés en utilisant la balise DIC ``monolog.processor``.
Voir la :ref:`référence à propos de celle-ci<dic_tags-monolog-processor>`.

Ajouter un jeton de Session/Requête
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Parfois il est difficile de dire quelles entrées dans le log appartiennent à
quelle session et/ou requête. L'exemple suivant va ajouter un jeton unique pour
chaque requête en utilisant un processeur.

.. code-block:: php

    namespace Acme\MyBundle;

    use Symfony\Component\HttpFoundation\Session\Session;

    class SessionRequestProcessor
    {
        private $session;
        private $token;

        public function __construct(Session $session)
        {
            $this->session = $session;
        }

        public function processRecord(array $record)
        {
            if (null === $this->token) {
                try {
                    $this->token = substr($this->session->getId(), 0, 8);
                } catch (\RuntimeException $e) {
                    $this->token = '????????';
                }
                $this->token .= '-' . substr(uniqid(), -8);
            }
            $record['extra']['token'] = $this->token;

            return $record;
        }
    }

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml        
        services:
            monolog.formatter.session_request:
                class: Monolog\Formatter\LineFormatter
                arguments:
                    - "[%%datetime%%] [%%extra.token%%] %%channel%%.%%level_name%%: %%message%%\n"

            monolog.processor.session_request:
                class: Acme\MyBundle\SessionRequestProcessor
                arguments:  [ @session ]
                tags:
                    - { name: monolog.processor, method: processRecord }

        monolog:
            handlers:
                main:
                    type: stream
                    path: "%kernel.logs_dir%/%kernel.environment%.log"
                    level: debug
                    formatter: monolog.formatter.session_request

.. note::

    Si vous utilisez plusieurs gestionnaires, vous pouvez aussi déclarer le
    processeur au niveau du gestionnaire au lieu de le faire globalement.

.. _Monolog: https://github.com/Seldaek/monolog
