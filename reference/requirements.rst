.. index::
   single: Requirements
   
Pré-requis au bon fonctionnement de Symfony2
============================================

Afin d'exécuter Symfony2, votre système a besoin de valider une liste de pré-requis.
Vous pouvez facilement voir si votre système remplit tous les critères en affichant
la page ``web/config.php`` de votre distribution Symfony. Puisque l'outil de ligne
de commandes utilise souvent un ``php.ini`` différent, c'est une bonne idée de
vérifier aussi les pré-requis en ligne de commandes via :


.. code-block:: bash

    php app/check.php

Voici la liste des pré-requis obligatoires et optionnels.

Obligatoires
------------

* PHP doit être au minimum à la version PHP 5.3.3
* JSON doit être activé
* ctype doit être activé
* Votre PHP.ini doit avoir le paramètre date.timezone défini

.. caution::

    Symfony2 est sujet à des bugs lorsqu'il est utilisé avec une version de PHP
    inférieure à 5.3.8 ou égale à 5.3.16. Pour plus d'informations, veuillez consulter
    `la section "Requirements" du README`_.
    
Facultatives
------------

* Vous devez avoir le module PHP-XML installé
* Vous devez avoir au moins la version 2.6.21 de libxml
* PHP tokenizer doit être activé
* Les fonctions mbstring doivent être activées
* iconv doit être activé
* POSIX doit être activé (seulement sur \*nix)
* Intl doit être installé avec ICU 4+
* APC 3.0.17+ (ou un autre cache d'opcode doit être installé)
* Configuration du PHP.ini recommandée

  * ``short_open_tag = Off``
  * ``magic_quotes_gpc = Off``
  * ``register_globals = Off``
  * ``session.autostart = Off``

Doctrine
--------

Si vous voulez utiliser Doctrine, vous devrez installer PDO. De plus, vous devrez
installer le driver PDO pour le serveur de base de données que vous voulez utiliser.

.. _`la section "Requirements" du README`: https://github.com/symfony/symfony#requirements
