Standards syntaxiques
=====================

Quand vous contribuez au code source de Symfony2, vous devez suivre ses
conventions de codage. Pour simplifier, la règle d'or est : **Imiter le code
existant de Symfony2**. De nombreux bundles open-source et bibliothèques utilisés
au sein de Symfony2 suivent les mêmes lignes de conduites, vous devez donc
les suivre vous aussi.

Rappelez vous que l'avantage principal d'un standard est que toutes les parties
du code source résultant se ressemblent et semblent ainsi familières, ce n'est
pas que telle technique soit plus lisible qu'une autre.

Symfony suit les standards définis par les documents `PSR-0`_, `PSR-1`_ et `PSR-2`_.

Comme une image - ou un peu de code - est plus efficace qu'un long discours,
voici un court exemple contenant les conventions les plus courantes décrites
ci-dessous :

.. code-block:: php

    <?php

    /*
     * This file is part of the Symfony package.
     *
     * (c) Fabien Potencier <fabien@symfony.com>
     *
     * For the full copyright and license information, please view the LICENSE
     * file that was distributed with this source code.
     */

    namespace Acme;

    class FooBar
    {
        const SOME_CONST = 42;

        private $fooBar;

        /**
         * @param string $dummy Some argument description
         */
        public function __construct($dummy)
        {
            $this->fooBar = $this->transformText($dummy);
        }

        /**
         * @param string $dummy Some argument description  
         * @param array  $options
         *
         * @return string|null Transformed input
         */
        private function transformText($dummy, array $options = array())
        {
            $mergedOptions = array_merge($options, array(
                'some_default' => 'values',
            ));

            if (true === $dummy) {
                return;
            }

            if ('string' === $dummy) {
                if ('values' === $mergedOptions['some_default']) {
                    $dummy = substr($dummy, 0, 5);
                } else {
                    $dummy = ucwords($dummy);
                }
            } 

            return $dummy;
        }
    }

Structure
---------

* Ajoutez un espace après chaque virgule séparatrice;

* Ajoutez un espace autour des opérateurs (`==`, `&&`, ...);

* Ajoutez une ligne vide avant l'expression de retour `return`, à moins que le
  retour soit seul à l'intérieur du groupe parent (comme une expression `if`);

* Utilisez les accolades pour indiquer les structures de contrôle quel que soit
  le nombre d'expressions qu'elles entourent.

* Définissez une classe par fichier - cela ne s'applique pas à vos classes
  de helper privées qui ne sont pas appelées à être instanciée de l'extérieur.
  Elles ne sont pas concernées par le standard PSR-0;

* Déclarez les propriétés de vos classes avant les méthodes;

* Déclarez d'abord les méthodes publiques puis les protegées, et finalement les
  privées.

Conventions de nommage
----------------------

* Utilisez le camelCase, pas de underscore, pour les variables,
  les noms de méthodes et de fonctions, et les arguments;

* Utilisez les underscores pour les noms d'options et de paramètres;

* Utilisez les espaces de nom pour toutes vos classes;

* Suffixez les interfaces avec `Interface`;

* Utilisez des caractères alphanumériques et des underscores pour les noms de
  fichier;

* Pensez à consulter la documentation :doc:`conventions` pour obtenir des
  suggestions de nom.

Documentation
-------------

* Ajoutez les blocs PHPDoc pour toutes les classes, méthodes, et fonctions;

* Omettez le tag `@return` si la méthode ne retourne rien;

* Les annotations `@package` et `@subpackage` ne sont pas utilisées.

Licence
-------

* Symfony est réalisé sous licence MIT, un bloc licence doit être présent
  au début de chaque fichier PHP, avant l'espace de nom.

.. _`PSR-0`: https://github.com/php-fig/fig-standards/blob/master/accepted/PSR-0.md 
.. _`PSR-1`: https://github.com/php-fig/fig-standards/blob/master/accepted/PSR-1-basic-coding-standard.md 
.. _`PSR-2`: https://github.com/php-fig/fig-standards/blob/master/accepted/PSR-2-coding-style-guide.md
