.. index::
   single: Security

Le composant sécurité
=====================

Introduction
------------

Le composant sécurité fournit un système de sécurité complet pour vos
applications web. Il est livré avec des méthodes d'authentification
utilisant HTTP basic ou l'authentification digest, ou encore un formulaire
interactif de login ou via le certificat X.509, mais permet également
d'implémenter vos propres stratégies d'authentification.
De plus, le composant offre la possibilité d'authoriser les utilisateurs
en se basant sur leurs rôles, et il contient un système avancé d'ACL.


Installation
------------

Vous pouvez installer le composant de deux manières différentes :

* :doc:`L'installation via Composer </components/using_components>` (``symfony/security`` sur Packagist_);
* Utilisez le repository Git officiel (https://github.com/symfony/Security).

Sections
--------

* :doc:`/components/security/firewall`
* :doc:`/components/security/authentication`
* :doc:`/components/security/authorization`

.. _Packagist: https://packagist.org/packages/symfony/security
