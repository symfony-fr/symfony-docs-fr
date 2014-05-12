.. index::
   single: Sessions, Session Proxy, Proxy

Exemple de Session Proxy
========================

Le mécanisme de proxy de session possède une variété d'utilisations et cet exemple
présente deux utilisations communes. Plutôt que d'injecter un session handler
comme d'habitude, un handler est injecté dans le proxy et est enregistré avec
un pilote de stockage de session ::

    use Symfony\Component\HttpFoundation\Session\Session;
    use Symfony\Component\HttpFoundation\Session\Storage\NativeSessionStorage;
    use Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler;

    $proxy = new YourProxy(new PdoSessionHandler());
    $session = new Session(new NativeSessionStorage(array(), $proxy));

Plus bas, vous apprendrez deux réels exemples qui peuvent être utilisés pour la
classe ``YourProxy`` : le chiffrement des données de session et les sessions
invitées en lecture seules.

Chiffrement des Données en Session
----------------------------------

Si vous vouliez chiffrer les données en session, vous pouvez utiliser le proxy
pour chiffrer ou déchiffrer la session comme suit ::

    use Symfony\Component\HttpFoundation\Session\Storage\Proxy\SessionHandlerProxy;

    class EncryptedSessionProxy extends SessionHandlerProxy
    {
        private $key;

        public function __construct(\SessionHandlerInterface $handler, $key)
        {
            $this->key = $key;

            parent::__construct($handler);
        }

        public function read($id)
        {
            $data = parent::read($id);

            return mcrypt_decrypt(\MCRYPT_3DES, $this->key, $data);
        }

        public function write($id, $data)
        {
            $data = mcrypt_encrypt(\MCRYPT_3DES, $this->key, $data);

            return parent::write($id, $data);
        }
    }

Les sessions invitées en lecture seule
--------------------------------------

Il y a quelques applications où une session est requise pour les utilisateurs
invités, mais il n'y a pas spécialement besoin de persister cette session. Dans
ce cas vous pouvez intercepter la session avant qu'elle ne soit écrite ::

    use Foo\User;
    use Symfony\Component\HttpFoundation\Session\Storage\Proxy\SessionHandlerProxy;

    class ReadOnlyGuestSessionProxy extends SessionHandlerProxy
    {
        private $user;

        public function __construct(\SessionHandlerInterface $handler, User $user)
        {
            $this->user = $user;

            parent::__construct($handler);
        }

        public function write($id, $data)
        {
            if ($this->user->isGuest()) {
                return;
            }

            return parent::write($id, $data);
        }
    }
