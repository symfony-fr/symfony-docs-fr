.. index::
   single: Forms; Fields; file

Le type de champ File
=====================

Le type ``file`` représente un input File dans votre formulaire.

+-------------+---------------------------------------------------------------------+
| Rendu comme | Champ ``input`` ``file``                                            |
+-------------+---------------------------------------------------------------------+
| Options     | - `required`_                                                       |
| héritées    | - `empty_data`_                                                     |
|             | - `label`_                                                          |
|             | - `label_attr`_                                                     |
|             | - `read_only`_                                                      |
|             | - `disabled`_                                                       |
|             | - `error_bubbling`_                                                 |
|             | - `error_mapping`_                                                  |
|             | - `mapped`_                                                         |
+-------------+---------------------------------------------------------------------+
| Type parent | :doc:`form</reference/forms/types/form>`                            |
+-------------+---------------------------------------------------------------------+
| Classe      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\FileType`  |
+-------------+---------------------------------------------------------------------+

Utilisation de base
-------------------

Imaginons que vous avez défini ce formulaire :

.. code-block:: php

    $builder->add('attachment', 'file');

.. caution::

    N'oubliez pas d'ajouter l'attribut ``enctype`` dans la balise form : 
    ``<form action="#" method="post" {{ form_enctype(form) }}>``.

Lorsque le formulaire est soumis, le champ ``attachment`` sera une instance de
:class:`Symfony\\Component\\HttpFoundation\\File\\UploadedFile`. Elle peut être
utilisée pour déplacer le fichier ``attachment`` vers son emplacement définitif :

.. code-block:: php

    use Symfony\Component\HttpFoundation\File\UploadedFile;

    public function uploadAction()
    {
        // ...

        if ($form->isValid()) {
            $someNewFilename = ...
        
            $form['attachment']->getData()->move($dir, $someNewFilename);

            // ...
        }

        // ...
    }

La méthode ``move()`` prend un répertoire et un nom de fichier comme arguments.
Vous pouvez calculer le nom de fichier grâce à l'une des méthodes suivantes::

    // utiliser le nom de fichier original
    $file->move($dir, $file->getClientOriginalName());

    // générer un nom aléatoire et essayer de deviner l'extension (plus sécurisé)
    $extension = $file->guessExtension();
    if (!$extension) {
        // l'extension n'a pas été trouvée
        $extension = 'bin';
    }
    $file->move($dir, rand(1, 99999).'.'.$extension);

Utiliser le nom original via la méthode ``getClientOriginalName()`` n'est pas sécurisé
car il a pu être manipulé par l'utilisateur. De plus, il peut contenir des caractères
qui ne sont pas tolérés dans les noms de fichiers. Il est recommandé de nettoyer
le nom avant de l'utiliser.

Lisez le chapitre :doc:`cookbook </cookbook/doctrine/file_uploads>` pour avoir un
exemple d'upload de fichier associé à une entité Doctrine.

Options héritées
----------------

Ces options sont héritées du type :doc:`form</reference/forms/types/form>` :

.. include:: /reference/forms/types/options/empty_data.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc