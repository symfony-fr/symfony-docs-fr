Gérer les bibliothèques vendors avec composer.json
--------------------------------------------------

Comment ça marche ?
~~~~~~~~~~~~~~~~~~~

Chaque projet Symfony utilise des bibliothèques tierces regroupées sous l'appellation
« vendors ». D'une façon ou d'une autre, le but est de vous permettre de télécharger
ces bibliothèques dans le répertoire  ``vendor/`` et de pouvoir obtenir pour chacune la
version désirée.

Par défaut, ces bibliothèques sont téléchargées en exécutant ``php composer.phar install``.
Le fichier binaire ``composer.phar`` est une bibliothèque appellé
`Composer`_. Vous pouvez en savoir plus sur son installation dans le chapitre
:ref:`Installation<installation-updating-vendors>`.

Le fichier ``composer.phar`` utilise un fichier ``composer.json`` situé à la racine
de votre projet. Ce fichier au format JSON contient la liste des bibliothèques nécessaires
à votre projet, ainsi que leurs versions et bien d'autres informations. Le fichier
``composer.phar`` se base également sur un fichier ``composer.lock``, lequel vous permet
d'arrêter la version à une révision précise. En faite, si un ``composer.lock`` existe,
Les versions de ce fichier remplacerons celle dans ``composer.json``. Pour mettre à jour
vos bibliothèques, vous devez exécuter ``php composer.phar update``.

Pour en savoir plus sur Composer, visitez `GetComposer.org`_ (en anglais).

Nota bene : ces bibliothèques « vendor » *ne sont pas* gérées dans votre *dépôt*;
ce sont de simples fichiers installés (et ignorés du dépôt git) dans le répertoire
``vendor/``. Or, puisque toute l'information nécessaire pour télécharger ces fichiers
est disponible dans ``composer.json`` et ``composer.lock`` (qui, eux, *sont* gérés par
votre dépôt), tout autre développeur peut utiliser votre projet, lancer
``php composer.phar install``, et obtenir les mêmes bibliothèques (et version).
Vous contrôlez donc exactement le contenu de chaque bibliothèque, sans avoir besoin
de le versionner dans votre dépôt.

Ainsi, lorsqu'un développeur veut travailler sur votre projet, il lui suffit de
lancer le script ``php composer.phar install`` pour s'assurer que toutes les bibliothèques
nécessaires soient téléchargées.

.. sidebar:: Mettre à jour Symfony

    Puisque Symfony est une bibliothèque tierce de votre projet et qu'elle est
    donc gérée entièrement via ``composer.json`` et``composer.lock``,
    mettre à jour Symfony signifie simplement mettre à jour
    ces deux fichiers afin qu'ils correspondent à la dernière version
    de l'édition standard de Symfony.

    Bien sûr, si vous avez ajouté des entrées aux fichiers ``composer.json`` ou
    ``composer.lock``, veillez à ne remplacer que les parties originales afin de ne pas
    supprimer les entrées supplémentaires.

.. _Composer: http://getcomposer.org/
.. _GetComposer.org: http://getcomposer.org/
