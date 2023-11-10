# DemineurC
Projet pour le cours d'info

# Compilation
### Tests
`clang display.c -DTEST -o display.exe --target=x86_64-windows-gnu`

### Exécutable
`clang main.c display.c core.c generation.c -o main.exe --target=x86_64-windows-gnu`
