.. index::
     single: reference de configuration : la classe de noyau
Configurer dans le noyau (par ex: AppKernel)
============================================

Quelques configuartions seront possibles sur le noyau lui-même (appelles souvent ``app/AppKernel.php``) . ceci s'effectue par
surcharger des méthodes spécifiques au sein de la classe mère :class:`Symfony\\Component\\HttpKernel\\Kernel`

Configuration
-------------

* `Charset`_
* `Kernel Name`_
* `Root Directory`_
* `Cache Directory`_
* `Log Directory`_

.. versionadded:: 2.1
    The :method:`Symfony\\Component\\HttpKernel\\Kernel::getCharset` method is new
    in Symfony 2.1

Charset
~~~~~~~

**type**: ``string`` **default**: ``UTF-8``
ceci retourne le charset utilise pour cette application. pour le (la) changer il faut surcharger  `Symfony\\Component\\HttpKernel\\Kernel::getCharset`
et retourne une autre methode; par exemple:

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
pour changer ce parametre, surcharger la methode :method:`Symfony\\Component\\HttpKernel\\Kernel::getName`
alternativement, deplacer  le noyau vers une diffetrente repertoire. par exemple ,si vous deplacez le noyau vers une repertoire
``foo`` (au lieu de ``app``) , le nom de la repertoire serait ``foo``
generalement, le nonm du noayu n est pas tres important.il est utilise pour generer le cahce.
si l application se presente avec plusieurs noyaux,la methode la plus facile pour donner un nom different a chacun est
de dupliquer  la repertoire ``app`` et lui donner un different nom   (exemple : ``foo``).

repertoire racine
~~~~~~~~~~~~~~~~~
**type**: ``string`` **default**: la repertoire ``AppKernel``
ceci retourne la repertoire racine du noyau. si on utilise l edition standard de Symphony,la repertoire racine refere a
repertoire ``app``
pour changer ce parametre , surcharger la methode 

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
ceci retourne le chemin vers la repertoire cache directement.pour la changer , (surcharger) la methode
:method:`Symfony\\Component\\HttpKernel\\Kernel::getCacheDir`
pour plus d information , lire ":ref:`override-cache-dir`"
	
repertoire log :
~~~~~~~~~~~~~~~~	
**type**: ``string`` **default**: ``$this->rootDir/logs``
ceci retourne le chemin vers la repertoire log.
pour la changer , surcharger la methode `Symfony\\Component\\HttpKernel\\Kernel::getLogDir`
pour plus d information , lire ":ref:`override-logs-dir`"
