post_max_size_message
~~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``The uploaded file was too large. Please try to upload a smaller file.``

C'est le message d'erreur de validation qui est utilisé si la taille des
données soumises en POST excède la directive ``post_max_size`` du ``php.ini``.
La variable ``{{ max }}``peut être utilisée pour afficher la taille autorisée.

.. note:
    
    Valider la ``post_max_size`` ne peut se faire que sur le formulaire base.