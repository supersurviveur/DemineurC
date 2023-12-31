# DémineurC
Projet de démineur en C pour le cours d'informatique de 1ʳᵉ année de prépa MP2I au lycée Champollion.
Réalisé par [Di Gallo Clément](https://github.com/Merlode11), [Thillard Julien](https://github.com/supersurviveur) et [Truc Benjamin](https://github.com/TRUCBenjamin)

## Jouer au démineur 

### But du jeu
Le but du jeu est de trouver toutes les bombes présentes sur la grille. Pour cela, vous pouvez creuser les cases une par une. Si vous creusez une case contenant une bombe, vous perdez la partie. Si vous creusez une case ne contenant pas de bombe, un nombre apparaît sur la case. Ce nombre indique le nombre de bombes présentes sur les cases adjacentes à la case creusée. Si vous creusez toutes les cases ne contenant pas de bombe, vous gagnez la partie.
Vous avez la possibilité de placer des drapeaux sur les cases que vous pensez contenir une bombe. Ces cases ne pourront pas être creusées tant que vous n'aurez pas retiré le drapeau. 

### Initialisation
Une fois le programme lancé, le jeu va vous demander la **longueur** de la grille. Vous pouvez mettre minimum 5 cases. Vous avez aussi la possibilité de mettre 0 : le programme va alors déterminer automatiquement la taille de votre terminal pour pouvoir le remplir.
Le jeu vous demandera de la même manière la **hauteur** de la grille, avec un minimum de 5 cases et la possibilité de remplir tout l'écran en renseignant 0.
Le jeu vous demandera ensuite le niveau de difficulté que vous souhaitez pour jouer. Plus le niveau de difficulté est haut, plus vous aurez de bombes à trouver.
- **1**: 10% de bombes
- **2**: 15% de bombes
- **3**: 20% de bombes

Vous pouvez maintenant jouer avec les commandes suivantes !

### Commandes de jeu
- `z`: monter d'une case
- `q`: aller à gauche d'une case
- `s`: descendre d'une case
- `d`: aller à droite d'une case
- `e`: creuser la case
- `f`: poser (_ou retirer_) un drapeau sur la case

Le jeu est complètement fonctionnel. Vous devrez pouvoir y jouer sans difficultés. Bonne chance !

## Compilation
### Récupérer les fichiers
Le projet est sous forme de fichier zip, vous pouvez le télécharger. \
Pour décompresser le fichier, vous pouvez utiliser la commande suivante dans le terminal :
```bash
unzip DemineurC.zip
```

Une fois le fichier décompressé, vous pouvez vous déplacer dans le dossier, et compiler les fichiers.
### Programme principal
Pour compiler le programme principal, vous pouvez utiliser la commande suivante dans le terminal :
```bash
gcc main.c display.c core.c generation.c constants.c -o demineur -O3
```
Puis exécutez le programme avec `demineur` ou `./demineur` selon votre système d'exploitation.

Sur windows, la librairie `windows.h` est nécessaire (Déjà installé dans de nombreux cas), et vous pouvez ajouter l'extension `.exe` au fichier compilé.

Le drapeau `-DUNICODE_DISPLAY` peut être ajouté pour un affichage UTF-8 plus agréable (nécessite un terminal supportant l'UTF-8, l'invite de commande windows 10 n'est pas compatible, mais un terminal dans un éditeur tel que vscode fonctionne). \
__Exemple :__ 
```bash
gcc main.c display.c core.c generation.c constants.c -o demineur -O3 -DUNICODE_DISPLAY
```

### Tests
Chaque partie du code peut être testée indépendamment _(ou presque pour le fichier `generation.c` qui nécessite une fonction provenant de `core.c`)_ via des assertions. Les compilations de tests possibles sont les suivantes : \
__display__ : 
```bash
gcc display.c constants.c -o display -Wall -Wextra -Wvla -fsanitize=address,undefined -DTEST_DISPLAY
```
(le drapeau `-DUNICODE_DISPLAY` peut également être ajouté)

__core__ : 
```bash
gcc core.c constants.c -o core -Wall -Wextra -Wvla -fsanitize=address,undefined -DTEST_CORE
```

__generation__ : 
```bash
gcc generation.c core.c constants.c -o generation -Wall -Wextra -Wvla -fsanitize=address,undefined -DTEST_GENERATION
```