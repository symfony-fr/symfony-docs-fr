.. versionadded:: 2.1
    L'option ``error_mapping`` est nouvelle depuis Symfony 2.1.

error_mapping
~~~~~~~~~~~~~

**type**: ``array`` **default**: ``empty``

Cette option permet de modifier la cible de l'erreur de validation.

Imaginez une méthode personnalisée nommée ``matchingCityAndZipCode`` qui valide
si le nom d'une ville et d'un code postal correspondent. Malheureusement, il n'
existe pas de champs "matchingCityAndZipCode" dans le formulaire, donc Symfony
peux seulement afficher l'erreur en haut du formulaire.

Avec la personnalisation error mapping, vous pouvez faire mieux: mapper l'erreur
sur le champ city comme afficher ci-dessous::

    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'error_mapping' => array(
                'matchingCityAndZipCode' => 'city',
            ),
        ));
    }

Ci-dessous, vous trouverez les règles du coté droit et gauche du mapping:

* Le coté gauche contient le chemin des propriétés.
* Si une violation est généré sur une méthode ou une propriété d'une classe, son
  chemin est simplement "propertyName".
* Si une violation est généré sur l'entrée d'une ``array`` ou d'un objet
  ``ArrayAccess``, son chemin est ``[indexName]``.
* Vous pouvez construire des chemins de propriété imbriquées en les enchaînant,
  en séparant les propriétés par des points. Par exemple: ``addresses[work].matchingCityAndZipCode``
* Le coté gauche du mapping accepte également un point ``.``, qui se réfère au
  au domaine. Cela signifie que toute erreur ajouté au champ est ajouté au champ
  imbriqué donné à la place.
* La partie droite contient simplement le nom des champs du formulaire.
