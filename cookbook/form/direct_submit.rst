.. index::
   single: Form; Form::submit()

Comment utiliser la fonction submit() pour gérer les soumissions de formulaires
===============================================================================

.. versionadded:: 2.3
    La méthode :method:`Symfony\\Component\\Form\\FormInterface::handleRequest`
    a été ajoutée dans Symfony 2.3.

Avec la méthode ``handleRequest()`` il est très facile de gérer les soumissions
de formulaires::

    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function newAction(Request $request)
    {
        $form = $this->createFormBuilder()
            // ...
            ->getForm();

        $form->handleRequest($request);

        if ($form->isValid()) {
            // perform some action...

            return $this->redirect($this->generateUrl('task_success'));
        }

        return $this->render('AcmeTaskBundle:Default:new.html.twig', array(
            'form' => $form->createView(),
        ));
    }

.. tip::

    Pour en savoir plus sur cette méthode, lisez :ref:`book-form-handling-form-submissions`.

Appeler Form::submit() manuellement
-----------------------------------

.. versionadded:: 2.3
    Avant Symfony 2.3, la méthode ``submit()`` était connue sous le nom de ``bind()``.

Dans certains cas, vous voudrez avoir plus de contrôle sur le moment où
votre formulaire est soumis et sur les données qui y sont passées. Au lieu
d'utiliser la méthode :method:`Symfony\\Component\\Form\\FormInterface::handleRequest`,
passez directement les données soumises à :method:`Symfony\\Component\\Form\\FormInterface::submit`::

    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function newAction(Request $request)
    {
        $form = $this->createFormBuilder()
            // ...
            ->getForm();

        if ($request->isMethod('POST')) {
            $form->submit($request->request->get($form->getName()));

            if ($form->isValid()) {
                // perform some action...

                return $this->redirect($this->generateUrl('task_success'));
            }
        }

        return $this->render('AcmeTaskBundle:Default:new.html.twig', array(
            'form' => $form->createView(),
        ));
    }

.. tip::

    Les formulaires qui contiennent des champs imbriqués attendent un tableau
    en argument de :method:`Symfony\\Component\\Form\\FormInterface::submit`.
    Vous pouvez également soumettre des champs individuels en appelant
    :method:`Symfony\\Component\\Form\\FormInterface::submit` directement
    sur un champ::

        $form->get('firstName')->submit('Fabien');

.. _cookbook-form-submit-request:

Passer une requête à Form::submit() (deprécié)
----------------------------------------------

.. versionadded:: 2.3
    Avant Symfony 2.3, la méthode ``submit`` était connue sous le nom de ``bind``.

Avant Symfony 2.3, la méthode :method:`Symfony\\Component\\Form\\FormInterface::submit`
acceptait un objet :class:`Symfony\\Component\\HttpFoundation\\Request`. Un raccourci
pratique de l'exemple précédent::

    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function newAction(Request $request)
    {
        $form = $this->createFormBuilder()
            // ...
            ->getForm();

        if ($request->isMethod('POST')) {
            $form->submit($request);

            if ($form->isValid()) {
                // perform some action...

                return $this->redirect($this->generateUrl('task_success'));
            }
        }

        return $this->render('AcmeTaskBundle:Default:new.html.twig', array(
            'form' => $form->createView(),
        ));
    }

Passer directement la :class:`Symfony\\Component\\HttpFoundation\\Request` à
:method:`Symfony\\Component\\Form\\FormInterface::submit` fonctionne toujours,
mais c'est déprécié et sera supprimé dans Symfony 3.0. Vous devriez plutôt
utiliser :method:`Symfony\\Component\\Form\\FormInterface::handleRequest`.
