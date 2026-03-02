# Utiliser Labyrinthe_SaveJSON et Labyrinthe_LoadJSON

## Sauvegarder un labyrinthe : `Labyrinthe_SaveJSON`

### Utilisation

```c
Labyrinthe_SaveJSON(struct Labyrinthe *labyrinthe);
```
La fonction ne retourne rien. Le fichier est toujours sauvegardé au chemin fixe :
```
../data/labyrinthe.json
```

### Fonctionnement

La fonction effectue un **parcours en largeur (BFS)** depuis `firstNode` pour collecter tous les noeuds accessibles dans une grille temporaire `grid[x][y]`.
Une fois le BFS terminé, la grille est écrite case par case de `x=0,y=0` à `x=width-1,y=height-1`. Si une case n'a pas été atteinte par le BFS (graphe non connexe), un noeud **placeholder WALL** avec `cost=9999` est créé temporairement pour garantir un JSON complet, puis libéré après l'écriture.

### Format produit

```json
{
  "width": 50,
  "height": 50,
  "nodes": [
    { "x": 0, "y": 0, "type": "START", "cost": 1 },
    { "x": 1, "y": 0, "type": "CELL",  "cost": 2 },
    { "x": 2, "y": 0, "type": "WALL",  "cost": 9999 },
    ...
  ]
}
```


# Charger un labyrinthe : `Labyrinthe_LoadJSON`

### Utilisation

```c
struct Labyrinthe *lab = Labyrinthe_LoadJSON("../data/labyrinthe.json");
```
Notez qu'il faut allouer la mémoire pour la labyrinthe avant d'appeler Labyrinthe_LoadJSON.


### Valeurs de retour

| Retour | Signification |
|---|---|
| `struct Labyrinthe*` | Chargement réussi |
| `NULL` | Echec — voir détail ci-dessous |

### Causes de retour NULL

Chaque cause affiche un message sur `stderr` avant de retourner `NULL` et de libérer toute la mémoire allouée.

| Cause                                      | Message stderr |
|--------------------------------------------|---|
| Fichier introuvable ou illisible           | `Erreur ouverture fichier` |
| `width` ou `height` absents ou <= 0        | `ERREUR: Dimensions invalides: WxH` |
| Echec d'allocation mémoire                 | *(silencieux, NULL retourné)* |
| Au moins une case manquante dans la grille | `ERREUR: N case(s) manquante(s), chargement annulé` |
| Le FirstNode est un noeud de type WALL     | `ERREUR: le START est un mur` |
| Nombre de START différent de 1             | `ERREUR: N START trouvé(s), exactement 1 requis` |
| Nombre de END différent de 1               | `ERREUR: N END trouvé(s), exactement 1 requis` |

### Avertissements non bloquants

Ces cas sont corrigés automatiquement, le chargement continue.

| Cas détecté | Correction                     | Message stderr |
|---|--------------------------------|---|
| Node avec coordonnées hors bornes | Ignoré                         | `AVERTISSEMENT: node hors bornes ignoré x=N y=N` |
| Deux noeuds pour la même case | Le second est ignoré           | `AVERTISSEMENT: doublon ignoré x=N y=N` |
| Type de noeud inconnu | Traité comme `CELL`            | `AVERTISSEMENT: type inconnu "..." en x=N y=N` |
| `travel_cost <= 0` | Corrigé selon le type du noeud | `AVERTISSEMENT: travel_cost invalide (N) en x=N y=N, corrigé` |
| WALL avec `cost < 9999` | Forcé à `9999`                 | `AVERTISSEMENT: WALL avec cost=N en x=N y=N, forcé à 9999` |
| CELL/START/END avec `cost >= 9999` | Forcé à `1`                    | `AVERTISSEMENT: TYPE avec cost=N en x=N y=N, forcé à 1` |

### Fonctionnement

Une fois le fichier parsé et les noeuds placés dans une grille temporaire `grid[x][y]`, les vérifications bloquantes sont effectuées dans l'ordre suivant :

1. Absence de cases manquantes
2. FirstNode n'est pas un WALL
3. Exactement 1 START
4. Exactement 1 END

Si toutes les vérifications passent, les **liaisons entre voisins** sont établies :

```
north → grid[x][y-1]
south → grid[x][y+1]
west  → grid[x-1][y]
east  → grid[x+1][y]
```

La grille temporaire est ensuite libérée. Les noeuds eux restent en mémoire et sont accessibles via `labyrinthe->firstNode`.

Il est recommandé de valider le labyrinthe chargé avant de l'afficher ou de lancer une résolution. 