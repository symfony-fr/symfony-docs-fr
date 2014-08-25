.. index::
   single: Request; Trusted Proxies

Proxies de confiance
====================

Si vous vous trouvez derrière un proxy, par exemple un load balancer, alors
certains entêtes vous sont peut être envoyés via l'entête spécial ``X-Forwarded-*``.
Par exemple, l'entête HTTP ``Host`` est habituellement utilisé pour retourner
l'hôte demandé. Mais lorsque vous êtes derrière un proxy, le véritable hôte peut
être stocké dans l'entête ``X-Forwarded-Host``.

Comme les entêtes HTTP peuvent être trafiqués, Symfony2 ne fait *pas confiance*
à ces entêtes de proxy par défaut. Si vous êtes derrière un proxy, vous devez
déclarer manuellement votre proxy comme étant de confiance.

.. versionadded:: 2.3
    Le support de la notation CIDR a été introduit dans Symfony 2.3, donc
    vous pouvez déclarer tout un sous-réseau dans la liste blanche
    (ex ``10.0.0.0/8``, ``fc00::/7``).

.. code-block:: php

    use Symfony\Component\HttpFoundation\Request;

    // ne fait confiance qu'aux entêtes de proxy qui viennent de ces adresses IP
    Request::setTrustedProxies(array('192.0.0.1', '10.0.0.0/8'));

.. note::

   Lorsque vous utilisez le reverse proxy interne de Symfony (``AppCache.php``), assurez vous
   d'avoir ajouté ``127.0.0.1`` à la liste des proxies de confiance.


Configurer les noms d'entêtes
-----------------------------

Par défaut, les entêtes de proxy suivant sont déclarés fiables:

* ``X-Forwarded-For`` Utilisé dans :method:`Symfony\\Component\\HttpFoundation\\Request::getClientIp`;
* ``X-Forwarded-Host`` Utilisé dans :method:`Symfony\\Component\\HttpFoundation\\Request::getHost`;
* ``X-Forwarded-Port`` Utilisé dans :method:`Symfony\\Component\\HttpFoundation\\Request::getPort`;
* ``X-Forwarded-Proto`` Utilisé dans :method:`Symfony\\Component\\HttpFoundation\\Request::getScheme` et :method:`Symfony\\Component\\HttpFoundation\\Request::isSecure`;

Si votre reverse proxy utilise un nom d'entête différent de ceux-ci, vous pouvez
configurer ce nom d'entête via :method:`Symfony\\Component\\HttpFoundation\\Request::setTrustedHeaderName`::

    Request::setTrustedHeaderName(Request::HEADER_CLIENT_IP, 'X-Proxy-For');
    Request::setTrustedHeaderName(Request::HEADER_CLIENT_HOST, 'X-Proxy-Host');
    Request::setTrustedHeaderName(Request::HEADER_CLIENT_PORT, 'X-Proxy-Port');
    Request::setTrustedHeaderName(Request::HEADER_CLIENT_PROTO, 'X-Proxy-Proto');

Déclarer des entêtes comme non fiables
--------------------------------------

Par défaut, si vous déclarez l'adresse IP de votre proxy dans la liste blanche, alors
les 4 entêtes listés ci-dessus seront considérés comme fiables. Si vous avez besoin de
déclarer certains de ces entêtes comme fiables, mais pas d'autre, vous pouvez également
le faire::

    // l'entête ``X-Forwarded-Proto`` est déclaré non fiable, l'entête par défaut est utilisé
    Request::setTrustedHeaderName(Request::HEADER_CLIENT_PROTO, '');
