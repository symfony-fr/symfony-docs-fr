SensioGeneratorBundle
=====================

Le ``SensioGeneratorBundle`` �tend l'interface de ligne de commande par d�faut
de Symfony2 en proposant de nouvelles commandes interactives et intuitives pour
g�n�rer des squelettes de code pour des bundles, des classes de formulaire, ou des
contr�leurs CRUD bas�s sur un sch�ma Doctrine 2.

Installation
------------

`T�l�chargez`_ le bundle et placez le sous l'espace de nom ``Sensio\\Bundle\\``.
Ensuite, comme pour tout autre bundle, incluez dans votre classe Kernel::

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
peuvent �tre ex�cut�es en mode interactif ou non. Le mode interactif vous pose
quelques questions pour configurer les param�tres servant � g�n�rer le code d�finitif.
La liste des commandes est list�e ci-dessous :

.. toctree::
   :maxdepth: 1

   commands/generate_bundle
   commands/generate_doctrine_crud
   commands/generate_doctrine_entity
   commands/generate_doctrine_form

.. _T�l�chargez: http://github.com/sensio/SensioGeneratorBundle