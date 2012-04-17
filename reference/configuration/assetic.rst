.. index::
   pair: Assetic; Configuration Reference

Configuration de Référence d'AsseticBundle
==========================================

Configuration complète par défaut
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        assetic:
            debug:                true
            use_controller:       true
            read_from:            %kernel.root_dir%/../web
            write_to:             %assetic.read_from%
            java:                 /usr/bin/java
            node:                 /usr/bin/node
            sass:                 /usr/bin/sass
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

                # Prototype
                name:
                    inputs:               []
                    filters:              []
                    options:

                        # Prototype
                        name:                 []
            filters:

                # Prototype
                name:                 []
            twig:
                functions:

                    # Prototype
                    name:                 []
