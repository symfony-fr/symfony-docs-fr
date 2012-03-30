.. index::
   single: Profiling; Collecteur de Données

Comment créer un Collecteur de Données personnalisé
===================================================

Le :ref:`Profiler <internals-profiler>` de Symfony2 délègue la collection de
données aux collecteurs de données. Symfony2 est livré avec quelques uns, mais
vous pouvez facilement créer le vôtre.

Créer un Collecteur de Données Personnalisé
-------------------------------------------

Créer un collecteur de données personnalisé est aussi simple que d'implémenter
:class:`Symfony\\Component\\HttpKernel\\DataCollector\\DataCollectorInterface`::

    interface DataCollectorInterface
    {
        /**
         * Collects data for the given Request and Response.
         *
         * @param Request    $request   A Request instance
         * @param Response   $response  A Response instance
         * @param \Exception $exception An Exception instance
         */
        function collect(Request $request, Response $response, \Exception $exception = null);

        /**
         * Returns the name of the collector.
         *
         * @return string The collector name
         */
        function getName();
    }

La méthode ``getName()`` doit retourner un nom unique. Ceci est utilisé pour
plus tard pour accéder à l'information (voir :doc:`/cookbook/testing/profiling`
par exemple).

La méthode ``collect()`` est responsable de stocker les données auxquelles
elle veut donner accès dans des propriétés locales.

.. caution::

    Comme le profiler sérialise les instances de collecteur de données, vous
    ne devriez pas stocker des objets ne pouvant pas être sérialisés (comme
    des objets PDO), ou vous devrez fournir votre propre méthode ``serialize()``.

La plupart du temps, il est pratique d'étendre la classe
:class:`Symfony\\Component\\HttpKernel\\DataCollector\\DataCollector` et
de remplir la propriété ``$this->data`` (elle se charge de sérialiser la
propriété ``$this->data``)::

    class MemoryDataCollector extends DataCollector
    {
        public function collect(Request $request, Response $response, \Exception $exception = null)
        {
            $this->data = array(
                'memory' => memory_get_peak_usage(true),
            );
        }

        public function getMemory()
        {
            return $this->data['memory'];
        }

        public function getName()
        {
            return 'memory';
        }
    }

.. _data_collector_tag:

Activer les Collecteurs de Données Personnalisés
------------------------------------------------

Pour activer un collecteur de données, ajoutez le comme un service ordinaire
dans l'une de vos configurations, et « taggez » le avec ``data_collector`` :

.. configuration-block::

    .. code-block:: yaml

        services:
            data_collector.your_collector_name:
                class: Fully\Qualified\Collector\Class\Name
                tags:
                    - { name: data_collector }

    .. code-block:: xml

        <service id="data_collector.your_collector_name" class="Fully\Qualified\Collector\Class\Name">
            <tag name="data_collector" />
        </service>

    .. code-block:: php

        $container
            ->register('data_collector.your_collector_name', 'Fully\Qualified\Collector\Class\Name')
            ->addTag('data_collector')
        ;

Ajouter des Templates de Profiler Web
-------------------------------------

Quand vous voulez afficher les données collectées par votre Collecteur de Données
dans la barre d'outils de débuggage ou dans le profiler web, créez un template Twig
en vous appuyant sur l'exemple suivant :

.. code-block:: jinja

    {% extends 'WebProfilerBundle:Profiler:layout.html.twig' %}

    {% block toolbar %}
        {# le contenu de la barre d'outils de débuggage web #}
    {% endblock %}

    {% block head %}
        {# si le « panel » du profiler web nécessite des fichiers JS ou CSS spécifiques #}
    {% endblock %}

    {% block menu %}
        {# le contenu du menu #}
    {% endblock %}

    {% block panel %}
        {# le contenu du « panel » #}
    {% endblock %}

Chaque bloc est optionnel. Le bloc ``toolbar`` est utilisé pour la barre
d'outils de débuggage web et les blocs ``menu`` et ``panel`` sont utilisés
pour ajouter un « panel » au profiler web.

Tous les blocs ont accès à l'objet ``collector``.

.. tip::

    Les templates intégrés utilisent une image encodée en base64 pour la
    barre d'outils (``<img src="src="data:image/png;base64,..."``). Vous
    pouvez facilement calculer la valeur en base64 d'une image avec ce petit
    script : ``echo base64_encode(file_get_contents($_SERVER['argv'][1]));``.

Pour activer le template, ajoutez un attribut ``template`` au tag
``data_collector`` dans votre configuration. Par exemple, en assumant que
votre template est dans un ``AcmeDebugBundle`` :

.. configuration-block::

    .. code-block:: yaml

        services:
            data_collector.your_collector_name:
                class: Acme\DebugBundle\Collector\Class\Name
                tags:
                    - { name: data_collector, template: "AcmeDebug:Collector:templatename", id: "your_collector_name" }

    .. code-block:: xml

        <service id="data_collector.your_collector_name" class="Acme\DebugBundle\Collector\Class\Name">
            <tag name="data_collector" template="AcmeDebug:Collector:templatename" id="your_collector_name" />
        </service>

    .. code-block:: php

        $container
            ->register('data_collector.your_collector_name', 'Acme\DebugBundle\Collector\Class\Name')
            ->addTag('data_collector', array('template' => 'AcmeDebugBundle:Collector:templatename', 'id' => 'your_collector_name'))
        ;
