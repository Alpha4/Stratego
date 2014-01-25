Stratego
========

Pour compiler:

$ make

Cela compilera l'interface, et également la librairie qui se trouve dans lib/


IA:

Basée sur des coups aléatoires, elle regardent quelles pièces peuvent jouer et prennent le premier coup possible.
Le placement des pièces est semi-aléatoire, une pièce forte est placée à coté d'une pièce faible. Le drapeau est toujours situé dans la même zone.


Interface:

Affichage d'un plateau classique et d'un espace à droite du plateau qui permet d'afficher toutes les informations nécessaires aux joueurs.
Tirage aléatoire de la couleur des joueurs.
Arbitre qui vérifie les coups avant qu'ils ne soient exécutés.
