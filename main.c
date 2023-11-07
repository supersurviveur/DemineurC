#include "display.h"
#include "generation.h"
#include "core.h"
#include "constants.h"

int main()
{
    initializeDisplay();
    initializeGeneration();
    int width = 10;
    int height = 10;
    int nbBombs = 33;
    int *contentGrid = creetableauaveclesbombes(width, height, nbBombs);
    int *displayGrid = creetableauvide(width, height);

    int cursorX = 0;
    int cursorY = 0;
    char action = 0;
    int gameState = 0;
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