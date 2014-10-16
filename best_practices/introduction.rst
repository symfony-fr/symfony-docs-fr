.. index::
   single: Les bonnes pratiques du framework Symfony

Les bonnes pratiques du framework Symfony
=========================================

Le framework Symfony est bien connu pour être *réellement* flexible et utilisé
pour construire des micro-site, des applications d'entreprise permettant de tenir
des milliard de connexions et être la base d'autres frameworks. Depuis qu'il est
sorti en juillet 2011, la communauté a appris énormément sur ce qui est possible
et comment faire les choses *de la meilleure manière*.

Les ressources de la communauté - comme les blogs ou les présentations - ont créées
un ensemble de recommandations non-officielles pour développer des applications 
Symfony. Malheureusement, un certain nombre de ces recommandations ne sont pas 
nécessaires pour les applications web. La plupart du temps, elles compliquent
inutilement les choses et ne suivent pas la philosophie pragmatique de Symfony.

Qu'apporte ce guide ?
---------------------

Ce guide entends définir ceci en décrivant les **bonnes pratiques officielles
permettant de développer des applications web avec le framework Symfony**. Elles
sont les bonnes pratiques qui collent à la philosophie du framework telle 
qu'imaginée par le créateur original `Fabien Potencier`_.

.. note::

    **Bonnes pratiques** est une expression désignant *"un ensemble de procédures 
    définies qui permettent de produire des résultats optimums"*. Et c'est exactement
    ce que ce guide entend procurer. Sauf si vous n'êtes pas d'accord avec 
    l'ensemble des recommandations, nous pensons qu'elles peuvent vous aider 
    à construire de grosses application avec moins de complexité.

Ce guide est *particulièrement adapté* pour :

* Les sites internet et les applications web développés avec le framework Symfony.

Pour les autres cas, ce guide devrait être un bon **point de départ** que vous 
pouvez ensuite **amender et adapter à vos besoins**:

* Bundles partagés avec la communauté Symfony;
* Développeurs avancés ou équipe créant leurs propres standards;
* Quelques applications complexe ayant des pré-requis fortement personnalisés;
* Bundles devant être partagés en interne dans une entreprise.

Nous savons que les vieilles habitudes ont la vie dure et que certains d'entre
vous seront choqués par certaines de ces bonnes pratiques. Mais en suivant 
celles-ci vous serez capable de développer des applications rapides, en 
retirant de la complexité et avec le même niveau ou plus de qualité. C'est
également un objet mouvant qui continuera à s'améliorer.

Gardez en tête que ce sont des **recommandations facultatives** que vous
et votre équipe pouvait ou ne pouvait pas suivre pour développer des 
applications Symfony. Si vous souhaitez continuer à suivre vos propres
bonnes pratiques et méthodologies, vous pouvez bien entendu faire cela.
Symfony est assez flexible pour s'adapter à vos besoins. Cela ne changera
jamais.

À qui s'adresse ce livre (Indice : ce n'est pas un tutoriel)
------------------------------------------------------------

Chaque développeur Symfony, que vous soyez expert ou novice, peut lire ce
guide. Mais, comme ce n'est pas un tutoriel, quelques connaissances de base
sur Symfony sont requises pour pouvoir tout comprendre. Si vous êtes totalement
novice avec Symfony, bienvenu ! Commencez par le premier tutoriel `The Quick Tour`_.

Nous avons volontairement garder ce guide court. Nous ne voulons par répéter des
explications que vous pouvez trouver dans la vaste documentation de Symfony, 
comme les discussions autour de l'injection de dépendance ou des ///front controllers///.
Nous allons uniquement mettre l'accent sur l'explication de la façon de faire de
ce que vous connaissez déjà.

L'application
-------------

En complément de ce guide, vous trouverez un exemple d'application développée 
avec à l'esprit l'ensemble des bonnes pratiques. **L'application est un simple
moteur de blog**, car cela permet de ne se concentrer que sur les concepts et 
fonctionnalités de Symfony sans s'embarrasser de détails complexes.

Au lieu de développer l'application étape par étape dans ce guide, vous trouverez
une sélection d'extrait de code à travers les chapitres. Veuillez vous référer
au dernier chapitre de ce guide pour trouver plus de détails au sujet de 
l'application et des instructions pour l'installer.

Ne mettez pas à jour vos applications existantes
------------------------------------------------

Après avoir lu ce manuel, certains d'entre vous pourrait vouloir refactoriser
leurs applications Symfony existantes. Notre recommandation est simple et 
claire : **vous ne devriez pas refactoriser vos applications existante pour 
suivre ces bonnes pratiques**. Les raisons de ne pas le faire sont nombreuses :

* Vos applications existantes ne sont pas mauvaises, elles suivent simplement 
  un ensemble d'autres lignes directrices;
* Une refactorisation complète d'une base de code présente un risque d'introduire
  des erreurs dans vos applications;
* La somme de travail nécessaire à cela pourrait être mieux dépenser à 
  améliorer vos tests ou à ajouter des fonctionnalités procurant une réelle
  plus-value à vos utilisateurs finaux.

.. _`Fabien Potencier`: https://connect.sensiolabs.com/profile/fabpot
.. _`The Quick Tour`: http://symfony.com/doc/current/quick_tour/the_big_picture.html
.. _`The Official Symfony Book`: http://symfony.com/doc/current/book/index.html
.. _`The Symfony Cookbook`: http://symfony.com/doc/current/cookbook/index.html
.. _`github.com/.../...`: http://github.com/.../...
