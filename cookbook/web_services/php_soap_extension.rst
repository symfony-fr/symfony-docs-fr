.. index::
    single: Web Services; SOAP

Comment créer des web services SOAP à l'intérieur d'un contrôleur Symfony2
==========================================================================

Configurer un contrôleur afin qu'il agisse comme un serveur est réalisé simplement
avec quelques outils. Vous devez, bien sûr, avoir installé l'extension `PHP SOAP`_.
Comme l'extension PHP SOAP ne peut actuellement pas générer un WSDL, vous devez soit
en créer un, soit utiliser un générateur provenant d'une librairie tierce.

.. note::

    Il existe de nombreuses implémentations de serveur SOAP disponibles compatibles
    avec PHP. `Zend SOAP`_ et `NuSOAP`_ en sont deux exemples. Bien que nous
    utilisions l'extension PHP SOAP dans nos exemples, l'idée générale devrait
    être applicable aux autres implementations.

SOAP fonctionne en exposant les méthodes d'un objet PHP à une entité externe (c'est-à-dire
le programme utilisant le service SOAP). Pour commencer, créez une classe - ``HelloService`` -
qui représente les fonctionnalités que vous mettrez à disposition dans votre service SOAP.
Dans ce cas, le service SOAP permettra à un client d'appeler la méthode nommée ``hello``, 
qui engendrera l'envoi d'un courriel::

    // src/Acme/SoapBundle/HelloService.php
    namespace Acme\SoapBundle;

    class HelloService
    {
        private $mailer;

        public function __construct(\Swift_Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function hello($name)
        {
            
            $message = \Swift_Message::newInstance()
                                    ->setTo('me@example.com')
                                    ->setSubject('Hello Service')
                                    ->setBody($name . ' dit bonjour !');

            $this->mailer->send($message);


            return 'Bonjour, '.$name;
        }
    }

Ensuite, vous devrez permettre à Symfony de créer une instance de cette classe.
Comme la classe envoie un courriel, le service prendra comme argument une instance
``Swift_Mailer``. En utilisant le conteneur de service, nous pouvons configurer 
Symfony et lui permettre de construire l'objet ``HelloService`` adéquat :

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml    
        services:
            hello_service:
                class: Acme\DemoBundle\Services\HelloService
                arguments: [@mailer]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
         <service id="hello_service" class="Acme\DemoBundle\Services\HelloService">
          <argument type="service" id="mailer"/>
         </service>
        </services>

Voici un exemple de contrôleur capable de gérer une requête SOAP.
Si ``indexAction()`` est accessible via la route ``/soap``, alors le document 
WSDL peut être atteint via ``/soap?wsdl``.

.. code-block:: php

    namespace Acme\SoapBundle\Controller;
    
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloServiceController extends Controller 
    {
        public function indexAction()
        {
            $server = new \SoapServer('/path/to/hello.wsdl');
            $server->setObject($this->get('hello_service'));
            
            $response = new Response();
            $response->headers->set('Content-Type', 'text/xml; charset=ISO-8859-1');
            
            ob_start();
            $server->handle();
            $response->setContent(ob_get_clean());
            
            return $response;
        }
    }

Notez les appels à ``ob_start()`` et ``ob_get_clean()``. Ces méthodes contrôlent
`le tampon de sortie`_ qui vous permettent «  d'intercepter » les flux de sortie
de la méthode ``$server->handle()``. Cela est nécessaire car Symfony attend de votre
contrôleur un objet ``Response`` contenant ce flux. Rappelez vous aussi de définir l'entête
HTTP « Content-Type » comme « text/xml » puisque c'est ce à quoi le client s'attendra.
Vous utilisez donc ``ob_start()`` pour commencer la mise en tampon de STDOUT et utilisez
``ob_get_clean()`` pour mettre la sortie dans le contenu de la Réponse et vider le tampon
de sortie. Finalement, vous êtes prêt à retourner l'objet ``Response``.

Voici un exemple qui appelle un service en utilisant le client `NuSOAP`_. Cet exemple
suppose que le ``indexAction`` présent dans le contrôleur ci-dessus est accessible via la
route ``/soap``::

    $client = new \Soapclient('http://example.com/app.php/soap?wsdl', true);
    
    $result = $client->call('hello', array('name' => 'Scott'));

Un exemple d'un flux WSDL :

.. code-block:: xml

    <?xml version="1.0" encoding="ISO-8859-1"?>
     <definitions xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/" 
         xmlns:xsd="http://www.w3.org/2001/XMLSchema" 
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
         xmlns:SOAP-ENC="http://schemas.xmlsoap.org/soap/encoding/" 
         xmlns:tns="urn:arnleadservicewsdl" 
         xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap/" 
         xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/" 
         xmlns="http://schemas.xmlsoap.org/wsdl/" 
         targetNamespace="urn:helloservicewsdl">
      <types>
       <xsd:schema targetNamespace="urn:hellowsdl">
        <xsd:import namespace="http://schemas.xmlsoap.org/soap/encoding/" />
        <xsd:import namespace="http://schemas.xmlsoap.org/wsdl/" />
       </xsd:schema>
      </types>
      <message name="helloRequest">
       <part name="name" type="xsd:string" />
      </message>
      <message name="helloResponse">
       <part name="return" type="xsd:string" />
      </message>
      <portType name="hellowsdlPortType">
       <operation name="hello">
        <documentation>Hello World</documentation>
        <input message="tns:helloRequest"/>
        <output message="tns:helloResponse"/>
       </operation>
      </portType>
      <binding name="hellowsdlBinding" type="tns:hellowsdlPortType">
      <soap:binding style="rpc" transport="http://schemas.xmlsoap.org/soap/http"/>
      <operation name="hello">
       <soap:operation soapAction="urn:arnleadservicewsdl#hello" style="rpc"/>
       <input>
        <soap:body use="encoded" namespace="urn:hellowsdl" 
            encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"/>
       </input>
       <output>
        <soap:body use="encoded" namespace="urn:hellowsdl" 
            encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"/>
       </output>
      </operation>
     </binding>
     <service name="hellowsdl">
      <port name="hellowsdlPort" binding="tns:hellowsdlBinding">
       <soap:address location="http://example.com/app.php/soap" />
      </port>
     </service>
    </definitions>


.. _`PHP SOAP`:          http://php.net/manual/fr/book.soap.php
.. _`NuSOAP`:            http://sourceforge.net/projects/nusoap
.. _`le tampon de sortie`:  http://php.net/manual/fr/book.outcontrol.php
.. _`Zend SOAP`:         http://framework.zend.com/manual/fr/zend.soap.server.html