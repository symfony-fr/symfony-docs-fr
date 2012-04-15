Standards syntaxiques
=====================

Quand vous contribuez au code source de Symfony2, vous devez suivre une syntaxe
standard, celle-ci est défini dans ce document.

Pour simplifier, la règle d'or est : **Imiter le code existant de Symfony2**.
De nombreux Bundles open-source et bibliothèques utilisés au sein de Symfony2 
suivent les mêmes lignes de conduites, vous devez donc les suivre vous aussi.

Rappelez vous que l'avantage principal d'un standard est que toutes les parties
du code source résultants se ressemblent et semblent ainsi familières, ce n'est
pas que telle technique soit plus lisible qu'une autre.

Comme une image - ou un peu de code - est plus efficace que des milliers de
mots, voici un court example contenant les plus courantes tournures décrites
plus haut:

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

* Ne jamais utiliser de tag court (`<?`);

* Ne jamais clotûrer un fichier comprenant une classe par le tag de fin php
  `?>`;

* L'indentation doit être réaliser par pallier de 4 espaces (les tabulations ne
  sont pas permises);

* Utiliser le caractère (linefeed :`0x0A`) pour terminer vos lignes;

* Ajouter un espace après chaque virgule séparatrice;

* N'utiliser pas d'espace après une parenthèse ouvrante ou avant d'en fermer
  une;

* Ajouter un espace autour des opérateurs (`==`, `&&`, ...);

* Ajouter un espace avant d'ouvrir une parenthèse suivant une mot-clef de
  contrôle (`if`, `else`, `for`, `while`, ...);

* Ajouter une ligne vide avant l'expression de retour `return`, à moins que le
  retour soit seul à l'intérieur du groupe parent (comme une expression `if`);

* N'ajouter pas d'espace à la fin de vos lignes;

* Utiliser les accolades pour indiquer les structures de contrôle quelque soit
  le nombre d'expressions qu'elles entourent.

* Ajouter les accolades sur des lignes séparées pour les classes, méthodes et
  déclarations de fonctions;

* Séparer les expressions conditionnelles (`if`, `else`, ...) et les accolades
  ouvrantes avec un seul espace, sans ligne vide.

* Déclarer la visibilité de chacunes des méthodes et des propriétés intégrées
  dans vos classes (l'usage de `var` est interdit);

* Utiliser le bas-de-casse pour les constantes native: `false`, `true`, et
  `null`. De même pour `array()`;

* Utiliser les majuscules pour les constantes en séparant les mots avec des 
  sous-tirets;

* Définissez une classe par fichier - cela ne s'applique pas à vos classes
  privées qui ne sont pas appelé à être instanciée en dehors du fichier dans 
  lequel elles sont présentes. Elles ne sont pas concernés par le standard
  PSR-0;

* Déclarer les propriétés de vos classes avant les méthodes;

* Déclarer d'abord les méthodes publiques puis les protegées, et finalement les
  privées.

Conventions de dénomination
---------------------------

* Utiliser le camelCase, pas de sous-tiret, pour les variables, les fonctions,
  les noms méthodes et les arguments;

* Utilser les sous-tirets pour les options, les noms des paramètres;

* Utiliser les espaces de nom pour toutes vos classes;

* Suffixez les interfaces avec `Interface`;

* Utiliser des caractères alphanumeriques et des sous-tirets pour les noms de
  fichier;

* Pensez à consulter la documentation :doc:`conventions` pour obtenir des
  suggestions de nom.

Documentation
-------------

* Ajouter les blocs PHPDoc pour toutes les classes, méthodes, et fonctions;

* Omettre le tag `@return` si la méthode ne retourne rien;

* Les annotations `@package` et `@subpackage` ne sont pas utilisées.

License
-------

* Symfony est réalisé sous licence MIT, un bloc licence doit être présent
  au début de chaque fichier PHP, avant l'espace de nom.
