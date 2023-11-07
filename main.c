#include "display.h"
#include "generation.h"
#include "core.h"
#include "constants.h"
#include <malloc.h>

int main()
{
    if(initializeDisplay() == -1) {
        // Initialization failed
        return 2;
    }
    initializeGeneration();
    int width, height, nbBombs;
    getGameGridSize(&width, &height, &nbBombs);

    int cursorX = 0;
    int cursorY = 0;
    char action = 0;
    int gameState = 0;

    // Pas de bombe au premier coup
    int *contentGrid = creetableauaveclesbombes(width, height, 0, 0, 0);
    int *displayGrid = creetableauvide(width, height);
    // On affiche une fois la grille pour récuperer le point de départ du joueur, cela permet de ne pas jouer sur une bombe au premier coup
    waitForInput(contentGrid, displayGrid, width, height, &cursorX, &cursorY, &action);
    // On regenere la grille de bombe en evitant le départ
    free(contentGrid);
    contentGrid = creetableauaveclesbombes(width, height, nbBombs, cursorX, cursorY);
    // On joue le premer coup
    userInput(action, contentGrid, displayGrid, width, height, nbBombs, &gameState, cursorX, cursorY);

    // On peut maintenant rentrer dans la boucle de jeu
    while (gameState == 0)
    {
        waitForInput(contentGrid, displayGrid, width, height, &cursorX, &cursorY, &action);
        userInput(action, contentGrid, displayGrid, width, height, nbBombs, &gameState, cursorX, cursorY);
    }
    if (gameState == WON)
    {
        displayWin(contentGrid, displayGrid, width, height);
    }
    else if (gameState == LOST)
    {
        displayLoose(contentGrid, displayGrid, width, height);
    }
}
