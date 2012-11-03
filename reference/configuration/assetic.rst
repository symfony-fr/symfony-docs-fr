.. index::
   pair: Assetic; Configuration reference

Configuration de Référence d'AsseticBundle
==========================================

Configuration complète par défaut
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        assetic:
            debug:                "%kernel.debug%"
            use_controller:
                enabled:              "%kernel.debug%"
                profiler:             false
            read_from:            "%kernel.root_dir%/../web"
            write_to:             "%assetic.read_from%"
            java:                 /usr/bin/java
            node:                 /usr/bin/node
            ruby:                 /usr/bin/ruby
            sass:                 /usr/bin/sass
            # Une paire clé-valeur de noms d'éléments
            variables:
                some_name:                 []
            bundles:

                # Par défaut (tous les bundles enregistrés):
                - FrameworkBundle
                - SecurityBundle
                - TwigBundle
                - MonologBundle
                - SwiftmailerBundle
                - DoctrineBundle
                - AsseticBundle
                - ...
            assets:
                # Un tableau de noms de ressources (par exemple : some_asset, some_other_asset)
                some_asset:
                    inputs:               []
                    filters:              []
                    options:
                        # Un tableau clé-valeur d'options et de valeurs
                        some_option_name: []
            filters:

                # Un tableau de noms de filtres (par exemple : some_filter, some_other_filter)
                some_filter:                 []
            twig:
                functions:
                    # Un tableau de noms de fonctions (par exemple : some_function, some_other_function)
                    some_function:                 []
