Comment travailler avec les Passes de Compilation dans les Bundles
==================================================================

Les passes de compilation vous donnent l'opportunité de manipuler d'autres
définitions de service qui ont été définies via le conteneur de service.
Pour savoir comment les créer, vous pouvez lire la section des composants
« :doc:`/components/dependency_injection/compilation` ». Pour définir une
passe de compilation depuis un bundle, vous devez l'ajouter à la méthode
build se situant dans la classe du bundle::

    namespace Acme\MailerBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    use Acme\MailerBundle\DependencyInjection\Compiler\CustomCompilerPass;

    class AcmeMailerBundle extends Bundle
    {
        public function build(ContainerBuilder $container)
        {
            parent::build($container);

            $container->addCompilerPass(new CustomCompilerPass());
        }
    }

L'un des cas d'utilisation les plus fréquents des passes de compilation est lorsque
vous travaillez avec des services taggés (apprenez-en plus à propos des tags en lisant
la section sur les composants « :doc:`/components/dependency_injection/tags` »).
Si vous utilisez des tags personnalisés dans un bundle, alors par convention, les noms
de tag se constituent du nom du bundle (en minuscules, avec des tirets du bas en tant
que séparateurs), suivi par un point, et finalement le nom « réel ». Par exemple, si
vous voulez introduire un tag « transport » dans votre AcmeMailerBundle, vous devriez
l'appeler ``acme_mailer.transport``.
