# DemineurC
Projet de démineur en C pour le cours d'informatique de 1ʳᵉ année de prépa MP2I au lycée Champollion.
Réalisé par [Di Gallo Clément](https://github.com/Merlode11), [Thillard Julien](https://github.com/supersurviveur) et [Truc Benjamin](https://github.com/TRUCBenjamin)

## Commandes de jeu
- `z`: monter d'une case
- `q`: aller à gauche d'une case
- `s`: descendre d'une case
- `d`: aller à droite d'une case
- `e`: creuser la case
- `f`: poser un drapeau sur la case

# Compilation
### Programme principal
Pour compiler le programme principal, vous pouvez utiliser la commande suivante: \
`gcc main.c display.c core.c generation.c constants.c -o main.exe -Wall -Wextra`
puis exécutez le programme avec `main.exe`

Le drapeau `UNICODE_DISPLAY` peut être ajouté pour un affichage UTF-8 plus agréable (nécessite un terminal supportant l'UTF-8, l'invite de commande windows 10 n'est pas compatible, mais un terminal dans un éditeur tel que vscode fonctionne). \
__Exemple :__ `gcc main.c display.c core.c generation.c constants.c -o main.exe -Wall -Wextra -DUNICODE_DISPLAY`

### Tests
Chaque partie du code peut être testée indépendamment, ou presque, via des assertions. Les compilations de tests possibles sont les suivantes : \
__display__ : `gcc display.c constants.c -o display.exe -Wall -Wextra -DTEST_DISPLAY` (le drapeau `UNICODE_DISPLAY` peut également être ajouté) \
puis `display.exe`

__core__ : `gcc core.c constants.c -o core.exe -Wall -Wextra -DTEST_CORE` \
puis `core.exe`

__generation__ : `gcc generation.c core.c constants.c -o generation.exe -Wall -Wextra -DTEST_GENERATION` \
puis `generation.exe`