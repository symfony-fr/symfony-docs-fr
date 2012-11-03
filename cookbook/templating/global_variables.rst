.. index::
   single: Templating; Global variables

Comment injecter des variables dans tous les modèles (i.e. Variables Globales)
==============================================================================

Parfois vous voulez qu'une variable soit accessible dans tous les modèles
que vous utilisez. C'est possible à l'intérieur du fichier
``app/config/config.yml``:

.. code-block:: yaml

    # app/config/config.yml
    twig:
        # ...
        globals:
            ga_tracking: UA-xxxxx-x

Maintenant, la variable ``ga_tracking`` est disponible dans tous les modèles Twig :

.. code-block:: html+jinja

    <p>Our google tracking code is: {{ ga_tracking }} </p>

C'est aussi simple que cela! Vous pouvez aussi tirer parti du système intégré
:ref:`book-service-container-parameters`, qui vous permet d'isoler ou de réutiliser
une valeur :

.. code-block:: ini

    ; app/config/parameters.yml
    [parameters]
        ga_tracking: UA-xxxxx-x

.. code-block:: yaml

    # app/config/config.yml
    twig:
        globals:
            ga_tracking: "%ga_tracking%"

La même variable est disponible exactement comme précédemment.

Des variables globales complexes
--------------------------------

Si vous voulez utiliser une variable globale plus complexe, comme un objet, alors
vous devez utiliser une autre méthode. Ainsi vous aurez besoin de créer une
:ref:`Extension Twig<reference-dic-tags-twig-extension>` et de retourner la variable
globale comme une des valeurs du tableau retourné par la méthode ``getGlobals``.
