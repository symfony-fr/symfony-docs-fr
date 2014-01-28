.. index::
    single: Bundle; Supression du AcmeDemoBundle

Comment supprimer le AcmeDemoBundle
====================================

La Standard Edition de Symfony2 est livré avec une démo complète 
qui vit à l'intérieur d'un bundle appelé AcmeDemoBundle. C'est un 
bon repère pour tout démarrage d'un projet, mais vous aurez 
probablement envie de finalement le retirer.

.. tip::
    
    Cet article présente AcmeDemoBundle comme exemple, mais vous 
    pouvez suivre les mêmes étapes pour supprimer n'importe quel bundle.

1. Désinscrire le bundle dans ``AppKernel``
-------------------------------------------

Pour déconnecter le bundle du framework, vous devez le supprimer de 
la méthode ``AppKernel::registerBundles()``. Le bundle se trouve normalement 
dans le tableau ``$bundles`` mais il n'est enregistré que pour l'environnement 
de développement. Vous pouvez le trouver à l'interieur de l'instruction if ci-dessous ::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(...);

            if (in_array($this->getEnvironment(), array('dev', 'test'))) {
                // commentez ou supprimez cette ligne
                // $bundles[] = new Acme\DemoBundle\AcmeDemoBundle();
                // ...
            }
        }
    }

2. Supprimer la configuration du bundle
---------------------------------------

Maintenant que Symfony ne connaît plus le bundle, vous devez supprimer 
toute configuration ou configuration de routing qui se réfère au bundle 
dans le répertoire ``app/config``.

2.1 Supprimer le routing du bundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez trouver le routing du AcmeDemoBundle dans le fichier 
``app/config/routing_dev.yml``.

Supprimez la notation ``_acme_demo`` en bas de ce fichier.


2.2 Supprimer la configuration du bundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Certains bundles contiennent des configurations dans l'un des fichiers 
``app/config/config*.yml``. Assurez-vous de supprimer la configuration associée
 à ces fichiers. Vous pouvez rapidement repérer la configuration d'un bundle 
 en cherchant la chaine ``acme_demo`` (ou n'importe quel autre nom de bundle, 
i.e ``fos_user`` pour le le FOSUserBundle).


Le AcmeDemoBundle n'a pas de configuration. Toutefois, le bundle est utilisé 
dans la configuration de sécurité dans le fichier ``app/config/security.yml``.
Vous pouvez l'utiliser pour votre configuration de sécurité mais vous pouvez 
aussi la supprimer.


3. Supprimer le bundle depuis le système de fichier
---------------------------------------------------
Maintenant que vous avez supprimé toutes les références du bundle dans votre 
application, vous pouvez supprimer le bundle depuis le système de fichier. 
Le bundle se trouve dans le répertoire ``src/Acme/DemoBundle``. Vous devez 
supprimer ce dossier et vous pouvez aussi supprimer le dossier ``Acme``.

.. tip::

    Si vous ne connaissez pas où se trouve le bundle, vous pouvez utiliser la méthode 
    :method:`Symfony\\Component\\HttpKernel\\Bundle\\BundleInterface::getPath` pour récupérer 
    le chemin vers le bundle::

        echo $this->container->get('kernel')->getBundle('AcmeDemoBundle')->getPath();

4. Supprimer l'integration dans d'autres bundles
-----------------------------------------------

.. note :: 
    Ceci ne concerne pas le bundle AcmeDemoBundle - aucun autre bundle ne dépend de lui,
    donc pouvez sauter cette étape.


Certains bundles s'appuient sur d'autres, si vous supprimez l'un d'eux, l'autre bundle
ne fonctionnera probablement plus. Assurez-vous donc avant de supprimer un bundle 
qu'aucun autre bundle, tiers ou votre propre bundle, ne dépend de ce bundle.

.. tip::
    
    Si un bundle s'appuie sur un autre, le plus souvent cela signifie que ce dernier 
    utilise certains services du bundle. 
    
    Rechercher la chaine de caractère de l'alias du bundle pourrait vous aider à 
    les repérer (i.e ``acme_demo`` pour les bundles dépendant de AcmeDemoBundle).

.. tip::

    Si un bundle tiers s'appuie sur un autre bundle, vous pouvez trouver ce bundle 
    mentionné dans le fichier ``composer.json`` se trouvant dans le dossier du bundle.