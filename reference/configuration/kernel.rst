.. index::
    single: Configuration reference; Kernel class
    
Configurer dans le noyau (par ex: AppKernel)
============================================

Certaines configurations peuvent être faites dans la classe noyau elle-même (souvent appellée ``app/AppKernel.php``).
Cela passe par la surcharge de méthodes spécifiques au sein de la classe mère
:class:`Symfony\\Component\\HttpKernel\\Kernel`.

Configuration
-------------

* `Charset`_
* `Nom du noyau`_
* `Répertoire racine`_
* `Répertoire Cache`_
* `Répertoire Log`_

Charset
~~~~~~~

**type**: ``string`` **default**: ``UTF-8``

Ceci retourne le charset utilisé pour cette application. Pour le changer, surchargez
la méthode :method:`Symfony\\Component\\HttpKernel\\Kernel::getCharset`
pour retourner un autre Charset, par exemple::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        public function getCharset()
        {
            return 'ISO-8859-1';
        }
    }

Nom du noyau
~~~~~~~~~~~~

**type**: ``string`` **default**: ``app`` (c-a-d le nom du répertoire contenant la classe du noyau)

Pour changer ce paramètre, surchargez la méthode :method:`Symfony\\Component\\HttpKernel\\Kernel::getName`
Vous pouvez également deplacer le noyau vers un répertoire différent. Par exemple,
si vous avez déplacé le noyau dans un répertoire ``foo`` (au lieu de ``app``), le  nom du noyau
sera ``foo``.

Le nom du répertoire n'est pas directement important. Il est utilisé lors de
la génération des fichiers de cache. Si vous avez une application avec plusieurs
noyaux, la méthode la plus simple pour que chacun ait un nom unique est de
dupliquer le répertoire ``app`` et de le renommer autrement (ex ``foo``).

Répertoire racine
~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: le répertoire de ``AppKernel``

Ceci retourne le répertoire racine du noyau. Si vous utilisez l'Edition
Standard de Symfony, le répertoire racine est ``app``. Pour changer cette
valeur, surchargez la méthode :method:`Symfony\\Component\\HttpKernel\\Kernel::getRootDir`::

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
  

Répertoire Cache
~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``$this->rootDir/cache/$this->environment``

Cceci retourne le chemin du répertoire Cache. Pour changer cette valeur,
surchargez la méthode :method:`Symfony\\Component\\HttpKernel\\Kernel::getCacheDir`.
Pour plus d'informations, lisez ":ref:`override-cache-dir`".
	
Répertoire Log
~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``$this->rootDir/logs``

Ceci retourne le chemin du répertoire Log. Pour changer sa valeur,
surchargez la methode :method:`Symfony\\Component\\HttpKernel\\Kernel::getLogDir`.
Pour plus d'informations, lisez ":ref:`override-logs-dir`"
