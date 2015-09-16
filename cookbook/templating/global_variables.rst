.. index::
   single: Templating; Global variables

Comment injecter des variables dans tous les templates (i.e. Variables Globales)
================================================================================

Parfois vous voulez qu'une variable soit accessible dans tous les templates
que vous utilisez. Il est possible de le configurer à l'intérieur du fichier
``app/config/config.yml``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            # ...
            globals:
                ga_tracking: UA-xxxxx-x

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <twig:config>
            <!-- ... -->
            <twig:global key="ga_tracking">UA-xxxxx-x</twig:global>
        </twig:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array(
             // ...
             'globals' => array(
                 'ga_tracking' => 'UA-xxxxx-x',
             ),
        ));

Maintenant, la variable ``ga_tracking`` est disponible dans tous les modèles Twig :

.. code-block:: html+jinja

    <p>Our google tracking code is: {{ ga_tracking }} </p>

C'est aussi simple que cela !

Utiliser les paramètres du contenaire de services
-------------------------------------------------

Vous pouvez aussi tirer parti du système intégré
:ref:`book-service-container-parameters`, qui vous permet d'isoler ou de réutiliser
une valeur :


.. code-block:: yaml

    # app/config/parameters.yml
    parameters:
        ga_tracking: UA-xxxxx-x

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            globals:
                ga_tracking: "%ga_tracking%"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <twig:config>
            <twig:global key="ga_tracking">%ga_tracking%</twig:global>
        </twig:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array(
             'globals' => array(
                 'ga_tracking' => '%ga_tracking%',
             ),
        ));

La même variable est disponible exactement comme précédemment.


Utiliser des services
---------------------

Au lieu d'utiliser des valeurs statiques, vous pouvez également assigner un
service à une variable twig. Lorsque la variable globale est appellée dans le
template, le service va être récupéré depuis le conteneur de services et vous
récupérerez un accès à cet objet.

.. note::

    Le service n'est pas chargé paresseusement (lazy loaded). En d'autres
    mots, dès que twig est chargé, votre service est instancié, même si
    vous n'utilisez jamais la variable globale que vous avez défini.

Pour définir un service en tant que variable globale Twig, préfixez la
chaîne de caractère que vous assignez avec  ``@``.
Cela devrait vous sembler familier car c'est la même syntaxe que pour la
configuration des services.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            # ...
            globals:
                user_management: "@acme_user.user_management"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <twig:config>
            <!-- ... -->
            <twig:global key="user_management">@acme_user.user_management</twig:global>
        </twig:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array(
             // ...
             'globals' => array(
                 'user_management' => '@acme_user.user_management',
             ),
        ));

Using a Twig Extension
----------------------

If the global variable you want to set is more complicated - say an object -
then you won't be able to use the above method. Instead, you'll need to create
a :ref:`Twig Extension <reference-dic-tags-twig-extension>` and return the
global variable as one of the entries in the ``getGlobals`` method.



Utiliser une extension Twig
---------------------------

Si vous voulez utiliser une variable globale plus complexe, comme un objet, alors
vous devez utiliser une autre méthode. Ainsi vous aurez besoin de créer une
:ref:`Extension Twig<reference-dic-tags-twig-extension>` et de retourner la variable
globale comme une des valeurs du tableau retourné par la méthode ``getGlobals``.
