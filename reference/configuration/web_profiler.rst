.. index::
   single: Configuration Reference; WebProfiler

Configuration du WebProfilerBundle 
==================================

Configuration complète par défaut
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        web_profiler:
            
            # DEPRECIE, n'est plus utile et peut être retiré de votre configuration sans danger.
            verbose:             true

            # affiche la barre de debug en bas des pages avec un sommaire des informations du profiler
            toolbar:             false
            position:             bottom
            intercept_redirects: false