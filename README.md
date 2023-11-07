# DemineurC
Projet de démineur en C pour le cours d'informatique de 1ère année de prépa MP2I au lycée Champollion.
Réalisé par [Di Gallo Clément](https://github.com/Merlode11), [Thillard Julien](https://github.com/supersurviveur) et [Truc Benjamin](https://github.com/TRUCBenjamin)

## Commandes de jeu
- `z`: monter d'une case
- `q`: aller à gauche d'une case
- `s`: descendre d'une case
- `d`: aller à droite d'une case
- `e`: creuser la case
- `f`: poser un drapeau sur la case

# Compilation
### Tests
`clang display.c -DTEST -o display.exe --target=x86_64-windows-gnu`

### Exécutable
`clang main.c display.c core.c generation.c -o main.exe --target=x86_64-windows-gnu`
