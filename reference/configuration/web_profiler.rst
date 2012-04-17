.. index::
   single: Configuration Reference; WebProfiler

Configuration du WebProfilerBundle 
==================================

Configuration complète par défaut
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        web_profiler:
            
            # affiche les informations secondaire pour raccourcir la barre de debug
            verbose:             true

            # affiche la barre de debug en bas des pages avec un sommaire des informations du profiler
            toolbar:             false

            # vous donne une occasion de regarder les données avant de suivre une redirection
            intercept_redirects: false