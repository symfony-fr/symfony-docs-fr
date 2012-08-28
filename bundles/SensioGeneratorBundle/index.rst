SensioGeneratorBundle
=====================

Le ``SensioGeneratorBundle`` étend l'interface de ligne de commande par défaut
de Symfony2 en proposant de nouvelles commandes interactives et intuitives pour
générer des squelettes de code pour des bundles, des classes de formulaire, ou des
contrôleurs CRUD basés sur un schéma Doctrine 2.

Installation
------------

`Téléchargez`_ le bundle et placez-le sous l'espace de nom ``Sensio\\Bundle\\``.
Ensuite, comme pour tout autre bundle, incluez-le dans votre classe Kernel::

    public function registerBundles()
    {
        $bundles = array(
            ...

            new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle(),
        );

        ...
    }

Liste des commandes disponibles
-------------------------------

Le ``SensioGeneratorBundle`` est fourni avec quatre nouvelles commandes qui
peuvent être exécutées en mode interactif ou non. Le mode interactif vous pose
quelques questions pour configurer les paramètres servant à générer le code définitif.
La liste des commandes est listée ci-dessous :

.. toctree::
   :maxdepth: 1

   commands/generate_bundle
   commands/generate_doctrine_crud
   commands/generate_doctrine_entity
   commands/generate_doctrine_form

.. _Téléchargez: http://github.com/sensio/SensioGeneratorBundle