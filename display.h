/**
 * @file display.h
 * @brief Provides functions to display the game grid and manage user input
 * @authors Julien THILLARD, Clement DI-GALLO, Benjamin TRUC
 */

/**
 * @brief Initialize display
 * @return 0 if success, -1 if failure
 */
int initializeDisplay();

/**
 * @brief Restore display to default
*/
void restoreDisplay();

/**
 * @brief Get the size of the grid
 * @param gridWidth Pointer to the width of the grid
 * @param gridHeight Pointer to the height of the grid
 * @param nbBombs Pointer to the number of bombs
 * @return 0 if success, -1 if failure
 */
int getGameGridSize(int *gridWidth, int *gridHeight, int *nbBombs);

/**
 * @brief Show the entire grid
 * @param contentGrid Grid with the content of each cell
 * @param displayGrid Grid with the state of each cell
 * @param width Width of the grid
 * @param height Height of the grid
 * @param cursorX X position of the cursor
 * @param cursorY Y position of the cursor
 * @return 0 if success, -1 if failure
 */
int showGameGrid(int *contentGrid, const int *displayGrid, int width, int height, int cursorX, int cursorY);

/**
 * @brief Wait for user input
 * @param contentGrid Grid with the content of each cell
 * @param displayGrid Grid with the state of each cell
 * @param width Width of the grid
 * @param height Height of the grid
 * @param coordX Pointer to the X position of the cursor
 * @param coordY Pointer to the Y position of the cursor
 * @param action Pointer to the action realized by the user
 * @return 0 if success, -1 if failure
 */
int waitForInput(int *contentGrid, int *displayGrid, int width, int height, int *coordX, int *coordY, char *action);

/**
 * @brief Display win message
 * @param contentGrid Grid with the content of each cell
 * @param displayGrid Grid with the state of each cell
 * @param width Width of the grid
 * @param height Height of the grid
 */
int displayWin(int *contentGrid, int *displayGrid, int width, int height);

/**
 * @brief Display lose message
 * @param contentGrid Grid with the content of each cell
 * @param displayGrid Grid with the state of each cell
 * @param width Width of the grid
 * @param height Height of the grid
 */
int displayLoose(int *contentGrid, int *displayGrid, int width, int height);
