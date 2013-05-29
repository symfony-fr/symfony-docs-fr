.. index::
     single: reference de configuration : la classe de noyau
Configurer dans le noyau (par ex: AppKernel)
============================================

Quelques configuartions seront possibles sur le noyau lui-même (appellés souvent ``app/AppKernel.php``) . ceci s'effectue par
surcharge de méthodes spécifiques au sein de la classe mère :class:`Symfony\\Component\\HttpKernel\\Kernel`

Configuration
-------------

* `Charset`_
* `Kernel Name`_
* `Root Directory`_
* `Cache Directory`_
* `Log Directory`_

.. versionadded:: 2.1
    La méthode :method:`Symfony\\Component\\HttpKernel\\Kernel::getCharset` est nouvelle dans Symfony 2.1

Charset
~~~~~~~

**type**: ``string`` **default**: ``UTF-8``
ceci retourne le charset utilisé pour cette application. pour le changer il faut surcharger  `Symfony\\Component\\HttpKernel\\Kernel::getCharset`
pour retourner un autre Charset; 

par exemple:

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function getCharset()
        {
            return 'ISO-8859-1';
        }
    }
  


Kernel Name
~~~~~~~~~~~

**type**: ``string`` **default**: ``app`` (i.e. le nom de la repertoire contenant la classe du noyau)
pour changer ce paramètre, surcharger la méthode :method:`Symfony\\Component\\HttpKernel\\Kernel::getName`
alternativement, deplacer  le noyau vers une differente repertoire.
par exemple ,si vous deplacez le noyau vers une repertoire ``foo`` (au lieu de ``app``) , le nom de la repertoire serait ``foo``
généralement, le nonm du noyau n'est pas trés important. Il est utilisé pour générer les fichiers cache.
si l'application se présente avec plusieurs noyaux,la méthode la plus facile pour donner un nom different a chacun est
de dupliquer  la répertoire ``app`` et lui donner un different nom   (exemple : ``foo``).

repertoire racine
~~~~~~~~~~~~~~~~~
**type**: ``string`` **default**: la repertoire ``AppKernel``
ceci retourne la répertoire racine du noyau. si on utilise l'édition standard de Symphony,la répertoire racine refère a
lq répertoire ``app``
pour changer ce paramètre , surcharger la methode 

:method:`Symfony\\Component\\HttpKernel\\Kernel::getRootDir` ::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function getRootDir()
        {
            return realpath(parent::getRootDir().'/../');
        }
    }
  

repertoire cache:
~~~~~~~~~~~~~~~~
**type**: ``string`` **default**: ``$this->rootDir/cache/$this->environment``
ceci retourne le chemin vers la repertoire cache directement.pour la changer , surcharger la méthode
:method:`Symfony\\Component\\HttpKernel\\Kernel::getCacheDir`
pour plus d information , lire ":ref:`override-cache-dir`"
	
repertoire log :
~~~~~~~~~~~~~~~~	
**type**: ``string`` **default**: ``$this->rootDir/logs``
ceci retourne le chemin vers la repertoire log.
pour la changer , surcharger la methode `Symfony\\Component\\HttpKernel\\Kernel::getLogDir`
pour plus d information , lire ":ref:`override-logs-dir`"
