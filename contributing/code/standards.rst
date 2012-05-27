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

Symfony suit les standards définis par les documents PSR-0, PSR-1 et PSR-2

Comme une image - ou un peu de code - est plus efficace que des milliers de
mots, voici un court exemple contenant les plus courantes conventions décrites
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

    class Foo
    {
        const SOME_CONST = 42;

        private $foo;

        /**
         * @param string $dummy Some argument description
         */
        public function __construct($dummy)
        {
            $this->foo = $this->transform($dummy);
        }

        /**
         * @param string $dummy Some argument description
         * @return string|null Transformed input
         */
        private function transform($dummy)
        {
            if (true === $dummy) {
                return;
            }
            if ('string' === $dummy) {
                $dummy = substr($dummy, 0, 5);
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

* Déclarez la visibilité de chacunes de vos classes, méthodes et propriétés
  (l'usage de `var` est interdit);

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

* Utilsez les underscores pour les options, les noms des paramètres;

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

License
-------

* Symfony est réalisé sous licence MIT, un bloc licence doit être présent
  au début de chaque fichier PHP, avant l'espace de nom.