#include "display.h"
#include "generation.h"
#include "core.h"
#include "constants.h"
#include <malloc.h>
#include <stdlib.h>

int main()
{
    if(initializeDisplay() == -1) {
        // Initialization failed
        exit(EXIT_FAILURE);
    }
    initializeGeneration();
    int width, height, nbBombs;
    getGameGridSize(&width, &height, &nbBombs);

    int cursorX = 0;
    int cursorY = 0;
    char action = 0;
    int gameState = 0;

    // Pas de bombe au premier coup
    int *contentGrid = creeTabAvecBombes(width, height, 0, 0, 0);
    int *displayGrid = generateTable(width, height);
    // On affiche une fois la grille pour récupérer le point de départ du joueur, cela permet de ne pas jouer sur une bombe au premier coup
    waitForInput(contentGrid, displayGrid, width, height, &cursorX, &cursorY, &action);
    // On régénère la grille de bombe en évitant le départ
    free(contentGrid);
    contentGrid = creeTabAvecBombes(width, height, nbBombs, cursorX, cursorY);
    // On joue le premier coup
    userInput(action, contentGrid, displayGrid, height, width, nbBombs, &gameState, cursorX, cursorY);

    // On peut maintenant rentrer dans la boucle de jeu
    while (gameState == PLAYING)
    {
        waitForInput(contentGrid, displayGrid, width, height, &cursorX, &cursorY, &action);
        userInput(action, contentGrid, displayGrid, height, width, nbBombs, &gameState, cursorX, cursorY);
    }
    if (gameState == WON)
    {
        displayWin(contentGrid, displayGrid, width, height);
    }
    else if (gameState == LOST)
    {
        displayLoose(contentGrid, displayGrid, width, height);
    }

    free(contentGrid);
    free(displayGrid);

    restoreDisplay();
}
