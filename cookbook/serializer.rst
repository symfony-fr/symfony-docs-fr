.. index::
   single: Serializer

Comment utiliser le Serializer
==============================

Sérialiser et désérialiser vers et depuis des objets et différents formats
(ex JSON ou XML) est un sujet très complexe. Symfony est fourni avec un
:doc:`Composant Serializer</components/serializer>`, qui vous donne certains
outils pour résoudre vos problèmes.

En fait, avant de commencer, familiarisez vous avec le sérialiseur, les
normaliseurs et les encodeurs en lisant la
:doc:`documentation du Serializer</components/serializer>`. Vous pouvez
également vous renseigner sur le `JMSSerializerBundle`_ qui étend certaines
des fonctionnalités offertes par le Serializer de Symfony.

Activer le Serializer
---------------------

.. versionadded:: 2.3
    Le Serializer a toujours existé dans Symfony mais, avant la version 2.3, vous
    deviez construire le service ``serializer`` vous même.

Le service ``serializer`` n'est pas disponible par défaut. Pour le rendre disponible,
vous devez l'activer dans votre configuration :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            serializer:
                enabled: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <!-- ... -->
            <framework:serializer enabled="true" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'serializer' => array(
                'enabled' => true
            ),
        ));

Ajouter des Normaliseurs et des Encodeurs
-----------------------------------------

Une fois activé, le service ``serializer`` sera disponible dans le conteneur
et sera chargé avec deux :ref:`encodeurs<component-serializer-encoders>`
(:class:`Symfony\\Component\\Serializer\\Encoder\\JsonEncoder` et
:class:`Symfony\\Component\\Serializer\\Encoder\\XmlEncoder`) mais aucun
:ref:`normaliseurs<component-serializer-normalizers>`, ce qui veut dire
que vous devrez charger le vôtre.

Vous pouvez charger des normaliseurs et/ou des encodeurs en les taggant
comme :ref:`serializer.normalizer<reference-dic-tags-serializer-normalizer>` et
:ref:`serializer.encoder<reference-dic-tags-serializer-encoder>`. Il est également
possible de définir la priorité du tag pour influencer l'ordre dans lequel il seront
pris.

Voici un exemple de chargement du
:class:`Symfony\\Component\\Serializer\\Normalizer\\GetSetMethodNormalizer`:

.. configuration-block:: 

    .. code-block:: yaml

       # app/config/config.yml
       services:
          get_set_method_normalizer:
             class: Symfony\Component\Serializer\Normalizer\GetSetMethodNormalizer
             tags:
                - { name: serializer.normalizer }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
            <service id="get_set_method_normalizer" class="Symfony\Component\Serializer\Normalizer\GetSetMethodNormalizer">
                <tag name="serializer.normalizer" />
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition(
            'Symfony\Component\Serializer\Normalizer\GetSetMethodNormalizer'
        ));
        $definition->addTag('serializer.normalizer');
        $container->setDefinition('get_set_method_normalizer', $definition);

.. note::

    La classe :class:`Symfony\\Component\\Serializer\\Normalizer\\GetSetMethodNormalizer`
    est inefficace de par sa conception. Dès que vous aurez un modèle de données circulaire, une
    boucle infinie sera créée à l'appel des getters. Vous êtes donc invité à ajouter vos
    propres normaliseurs pour répondre à ce besoin.

.. _JMSSerializerBundle: http://jmsyst.com/bundles/JMSSerializerBundle