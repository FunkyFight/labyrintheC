# Utiliser rdfsGeneration

PS: Tout les retours sont évidemment des pointeurs

## Génération :

Pour obtenir une liste de noeuds générées, il suffit d'appeler la fonction
`fullFillLabyrintheGeneration(int height,int width,int isPerfect,LabyrintheNode* starter)` ou il suffit de rentrer la taille du labyrinthe souhaité ainsi que si le labyrinthe doit être un labyrinthe parfait.
Starter est un `LabyrintheNode` qui est le node de départ de la génération, si celui ci est `NULL`, un `LabyrintheNode` auto-générer en 0,0 sera choisit comme starter.

Cette fonction renvoie un objet `ListNode` contenant toutes les `LabyrintheNode` necéssaires pour chacunes des cases du labyrinthe
de la forme : [chemins,murs]

## Utiliser les `ListNode` :

Les objets `ListNode` contiennent trois variables:
 
`size`,`maxSize` et `nodeTab`

### Variables `size` et `maxSize` :

Ces variables servent pour obtenir la taille de la liste pour la première et la seconde aide pour l'alocation de mémoire , `size` étant tout simplement la taille actuelle de la liste et `maxSize` étant la taille maximum de la liste.

Elles sont gérés automatiquement par les fonctions suivantes:

`addToListNode(ListNode* l,LabyrintheNode* n)` : Ajoute n à la liste et gère la mémoire en conséquence.

`LabyrintheNode* removeLastToListNode(ListNode* l)` : Retire et renvoie le dernier élément de la liste

`void freeListNode(ListNode* l)` : Libère l'espace mémoire utilisé par la liste


### Variable `nodeTab` : 

Cette variable est la liste à proprement parlé, elle contient un ensemble de pointeur de `LabyrintheNode` de la forme `LabyrintheNode**`

Pour récuperer un `LabyrintheNode` 'i' dans un `ListNode` il suffit donc de faire :

`VariableListNode->nodeTab[i]`

## Aller plus loin :

### Fonction `randomTravelCost()`

Cette fonction renvoie un nombre aléatoire entre 1 et 3 définissant la difficulté à parcourir la case.

Probabilité de 1 : 50%

Probabilité de 2 : 33,3%

Probabilité de 3 : 16,7%

### Fonction `rdfsGeneration(LabyrintheNode* nodeStart, int height, int width)`

Fonction renvoyant un `ListNode` ne contenant que les chemins 'parcourables' du labyrinthe généré ainsi que leurs coûts.

Ne requière qu'un `LabyrintheNode` de départ ainsi qu'une taille

### Fonction `FillWithWalls(ListNode* chemins, int height, int width,int isPerfect)`

Considère que la liste 'chemins' est une la liste des chemins choisit comme étant 'parcourables' du labyrinthe, soit la liste générer par `rdfsGeneration`.

Renvoie la liste pour toutes les coordonnées dans l'intervale [(O,width),(0,height)] n'etant pas parmis 'chemins' des `LabyrintheNode` ayant pour coût "9999"
Si le labyrinthe souhaité est imparfait, chaque mur à 11% de chance de devenir un chemin à la place.


### Fonction `containLabyrintheNode(ListNode* l,LabyrintheNode* n)`

Renvoie 1 si 'n' est contenu dans la liste 'l'

Sinon Renvoie 0

### Fonction `isInLaby(LabyrintheNode* node,int height, int width)`

Verifie que le `LabyrintheNode` possède bien des coordonnées dans l'intervale du labyrinthe.

Renvoie 1 si c'est le cas

Sinon renvoie 0


# Attention

## Erreur DX DY

Github peut afficher une erreur par rapport à un DX et un DY , cette erreur est a ignorer, le systeme se basant sur les coordonnées géométriques : donc x etant les lignes et y etant les colonnes.