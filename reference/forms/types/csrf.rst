.. index::
   single: Forms; Fields; csrf

Type de champ Csrf
==================

Le type ``csrf`` est un champ input hidden qui contient le CSRF token (ou jeton).

+-------------+--------------------------------------------------------------------+
| Rendu comme | Champ ``input`` ``hidden``                                         |
+-------------+--------------------------------------------------------------------+
| Options     | - ``csrf_provider``                                                |
|             | - ``intention``                                                    |
|             | - ``property_path``                                                |
+-------------+--------------------------------------------------------------------+
| Type parent | ``hidden``                                                         |
+-------------+--------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Csrf\\Type\\CsrfType` |
+-------------+--------------------------------------------------------------------+

Options du champ
----------------

csrf_provider
~~~~~~~~~~~~~

**type**: ``Symfony\Component\Form\CsrfProvider\CsrfProviderInterface``

L'objet ``CsrfProviderInterface`` qui doit générer le CSRF token.
S'il n'est pas défini, la valeur par défaut est le provider par défaut.

intention
~~~~~~~~~

**type**: ``string``

Un identifiant unique facultatif pour générer le CSRF token.

.. include:: /reference/forms/types/options/property_path.rst.inc