# Comment afficher un labyrinthe

L'interface graphique expose des fonctions qui peuvent être utilisées pour intéragir avec.
Tout se passe dans GameFacade.

### Créer des noeuds
MERCI DE TOUJOURS PASSER PAR LES FONCTIONS `LabyrintheNode_Create()` et `LabyrintheNode_Destroy()` !

### Tester son code
Ne pas toucher main.c ! Si vous avez du code à tester, mettez le dans 
GameFacade#DebugAndTest(). Le code sera exécuté dès le lancement automatiquement

### Validation du labyrinthe
Pour éviter tout bug potentiel, vous devez d'abord voir si votre labyrinthe est valide
en le donnant à la fonction GameFacade#Labyrinthe_ValidateGrid(struct Labyrinthe* labyrinthe).

Cette fonction vérifie que :

1) Le premier noeud du labyrinthe existe `if(labyrinthe == NULL) return false;`
2) La largeur et la hauteur sont strictement positivies `int max = w * h;`

Si c'est bon, alors le code va parcourir le labyrinthe pour le vérifier complètement.
Le code alloue alors 2 tableaux.

1) nodes : tableau 1d qui sert juste à savoir quels noeuds ont été visités.
2) stack : pour le parcours en profondeur du labyrinthe.

Le code dépile alors un noeud et effectue plusieurs tests de validité dessus.

1) Le noeud ne doit pas être en dehors du labyrinthe. Les coordonnées doivent respecter la condition
$\forall (x, y) \in N | x \in [0 ; w - 1] ET y \in [0; h -1]$
2) L'emplacement des coordonnées ne doit pas être occupée ou doit être occupée par le même noeud si on l'a déjà rencontré (car plusieurs chemins peuvent mener au même noeud).
3) Le labyrinthe doit être complet. C'est à dire qu'il faut exactement $w * h$ noeuds **distincts**

Une fois ça terminé, une troisième série de vérification des frontières est faite.

1) Les frontières du labyrinthe sont fermées. Un noeud au bord ne doit pas avoir de noeud à l'extérieur.
2) Les noeuds non au bord doivent avoir un dans cette direction. (Si un noeud n'est pas au bord nord, bah il est censé avoir un voisin au nord du coup).
3) Les coordonnées du voisin sont mathématiquement logiques (situé à x + 1, x - 1, y + 1, y - 1, un offset de 1 quoi).
4) Les liens sont bidirectionnels. Si un noeud A est voisin de B alors pour B doit être le voisin de A.

Si toutes ces conditions sont respectées, alors vous ne devriez pas avoir de problème à afficher le labyrinthe.

### Afficher le labyrinthe
Dans GameFacade#DebugAndTest(), simplement utiliser `GameFacade_ShowInstantlyLabyrinthe(&labyrinthe);`
et lancer l'application.