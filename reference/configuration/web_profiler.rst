.. index::
   single: Configuration reference; WebProfiler

Configuration du WebProfilerBundle 
==================================

Configuration complète par défaut
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        web_profiler:
            
            # DÉPRÉCIÉ, n'est plus utile et peut être retiré de votre configuration sans danger
            verbose:             true

            # affiche la barre de débuggage en bas des pages avec un résumé des informations du profiler
            toolbar:             false
            position:             bottom
            intercept_redirects: false