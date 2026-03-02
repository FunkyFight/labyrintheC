# Système GenerationStep

Le système GenerationStep permet d'enregistrer et de rejouer visuellement les étapes de génération d'un labyrinthe. Chaque action effectuée pendant la génération peut être capturée comme une "step" et affichée plus tard pour visualiser l'algorithme en action.

## Vue d'ensemble

Le système fonctionne en trois phases :

1. **Enregistrement** : Pendant la génération, chaque action importante est enregistrée comme une `GenerationStep`
2. **Stockage** : Les steps sont stockées dans un tableau dans `struct Game->generationSteps`
3. **Lecture** : Les steps peuvent être rejouées dans un thread séparé avec des délais pour visualiser la génération

## Structures de données

### `enum StepType`

Définit les types d'étapes possibles :

```c
enum StepType {
    HighlightExistingNode,  // Colorer un noeud existant
    SetNodeVisibility,      // Changer la visibilité d'un noeud
    End                     // Marque la fin du tableau (comme \0 pour une chaîne)
}
```

**IMPORTANT** : Le type `End` doit TOUJOURS être présent à la fin du tableau de steps, sinon la lecture ne s'arrêtera jamais.

### `struct GenerationStep`

Structure principale contenant une étape :

```c
struct GenerationStep {
    void* step;              // Pointeur vers les données spécifiques (HighlightExistingStepType ou SetNodeVisibilityStepType)
    enum StepType stepType;  // Type de l'étape
    int iteration;           // Numéro d'itération de la génération
}
```

Le champ `iteration` permet de grouper plusieurs actions dans la même itération. Lors de la lecture, un délai de 250ms est appliqué entre chaque itération.

### `struct HighlightExistingStepType`

Données pour colorer un noeud :

```c
struct HighlightExistingStepType {
    int x, y;           // Coordonnées du noeud
    SDL_Color color;    // Couleur à appliquer
}
```

### `struct SetNodeVisibilityStepType`

Données pour changer la visibilité d'un noeud :

```c
struct SetNodeVisibilityStepType {
    int x, y;        // Coordonnées du noeud
    bool visible;    // true = visible, false = invisible
}
```

## Fonctions disponibles

### Création et libération

#### `GenerationSteps_Create()`

```c
struct GenerationStep* GenerationSteps_Create()
```

Alloue la mémoire pour un tableau de 9999 steps. Retourne un pointeur vers le tableau.

**Note** : La taille est fixe à 9999 steps. Si votre génération nécessite plus d'étapes, vous devrez augmenter cette valeur.

#### `GenerationSteps_Free(struct GenerationStep** strct)`

```c
void GenerationSteps_Free(struct GenerationStep** strct)
```

Libère la mémoire allouée pour le tableau de steps.

**Attention** : Cette fonction ne libère que le tableau principal, pas les steps individuelles ni leurs données.

### Création de steps

#### `GenerationSteps_CreateSetNodeVisibilityStep()`

```c
struct GenerationStep* GenerationSteps_CreateSetNodeVisibilityStep(
    int x,
    int y,
    bool visible,
    int iteration
)
```

Crée une step pour changer la visibilité d'un noeud.

**Paramètres** :
- `x, y` : Coordonnées du noeud
- `visible` : true pour rendre visible, false pour cacher
- `iteration` : Numéro d'itération de la génération

**Retour** : Pointeur vers la `GenerationStep` créée

**Exemple** :
```c
// Rendre visible le noeud (5, 3) à l'itération 10
struct GenerationStep* step = GenerationSteps_CreateSetNodeVisibilityStep(5, 3, true, 10);
```

#### `GenerationSteps_CreateHighlightExistingStepStep()`

```c
struct GenerationStep* GenerationSteps_CreateHighlightExistingStepStep(
    int x,
    int y,
    SDL_Color color,
    int iteration
)
```

Crée une step pour colorer un noeud existant.

**Paramètres** :
- `x, y` : Coordonnées du noeud
- `color` : Couleur SDL à appliquer
- `iteration` : Numéro d'itération de la génération

**Retour** : Pointeur vers la `GenerationStep` créée

**Exemple** :
```c
// Colorer le noeud (2, 4) en rouge à l'itération 5
SDL_Color red = {255, 0, 0, 255};
struct GenerationStep* step = GenerationSteps_CreateHighlightExistingStepStep(2, 4, red, 5);
```

### Lecture et affichage

#### `GenerationSteps_Read()`

```c
void GenerationSteps_Read(
    struct GenerationStep** steps,
    struct LabyrintheNode* rootNode
)
```

Lance la lecture et l'affichage des steps dans un thread séparé.

**Paramètres** :
- `steps` : Tableau de GenerationStep à afficher
- `rootNode` : Noeud racine du labyrinthe

**Comportement** :
- Vérifie qu'aucun affichage n'est déjà en cours (`is_generation_steps_displayer_running`)
- Convertit le labyrinthe en tableau de noeuds
- Lance un thread SDL pour afficher les steps
- Applique un délai de 250ms entre chaque itération

**Important** : Cette fonction ne bloque pas. L'affichage se fait en arrière-plan.

#### `displayGenerationSteps()` (fonction interne)

Fonction exécutée dans le thread d'affichage. Parcourt le tableau de steps et applique chaque action :

- Pour `HighlightExistingNode` : Change la couleur du GameObject associé au noeud
- Pour `SetNodeVisibility` : Change la visibilité du GameObject associé au noeud
- S'arrête quand le type `End` est rencontré

## Utilisation pratique

### Exemple complet

```c
// 1. Créer le tableau de steps
struct GenerationStep** steps = GenerationSteps_Create();
int stepIndex = 0;

// 2. Pendant la génération, enregistrer les steps
for(int iteration = 0; iteration < 10; iteration++) {
    // Rendre visible un noeud
    steps[stepIndex++] = GenerationSteps_CreateSetNodeVisibilityStep(
        iteration, 0, true, iteration
    );

    // Le colorer en vert
    SDL_Color green = {0, 255, 0, 255};
    steps[stepIndex++] = GenerationSteps_CreateHighlightExistingStepStep(
        iteration, 0, green, iteration
    );
}

// 3. IMPORTANT : Ajouter la step End
struct GenerationStep* endStep = malloc(sizeof(struct GenerationStep));
endStep->stepType = End;
steps[stepIndex] = endStep;

// 4. Stocker dans le Game
currentGame->generationSteps = steps;

// 5. Afficher les steps
GenerationSteps_Read(steps, labyrinthe->firstNode);
```

### Intégration avec un algorithme de génération

Dans votre fonction de génération (ex: RDFS), ajoutez l'enregistrement des steps :

```c
struct GenerationStep** steps = GenerationSteps_Create();
int stepIndex = 0;
int iteration = 0;

while(/* condition de génération */) {
    // Votre logique de génération...

    // Enregistrer l'action
    SDL_Color visitColor = {100, 100, 255, 255};
    steps[stepIndex++] = GenerationSteps_CreateHighlightExistingStepStep(
        currentNode->x, currentNode->y, visitColor, iteration
    );

    iteration++;
}

// Terminer avec End
struct GenerationStep* endStep = malloc(sizeof(struct GenerationStep));
endStep->stepType = End;
steps[stepIndex] = endStep;

return steps;
```

## Points d'attention

### Gestion de la mémoire

⚠️ **Attention aux fuites mémoire** :
- Chaque step alloue de la mémoire pour ses données spécifiques
- `GenerationSteps_Free()` ne libère que le tableau principal
- Vous devez libérer manuellement chaque step et ses données si nécessaire

### Limite de taille

Le tableau est limité à 9999 steps. Pour des labyrinthes très grands ou des algorithmes verbeux, cette limite peut être atteinte. Surveillez `stepIndex` pendant l'enregistrement.

### Thread safety

L'affichage se fait dans un thread séparé. Le flag `is_generation_steps_displayer_running` empêche de lancer plusieurs affichages simultanés.

### Step End obligatoire

Sans la step `End` à la fin du tableau, la fonction `displayGenerationSteps()` continuera de lire la mémoire indéfiniment, causant un crash.

## Bugs connus

Aucun

## Voir aussi

- [rdfs.md](rdfs.md) - Documentation de l'algorithme RDFS qui utilise ce système
- [afficher_labyrinthe.md](afficher_labyrinthe.md) - Documentation sur l'affichage du labyrinthe
