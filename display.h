/**
 * @brief Show the entire grid
 * @param contentGrid Grid with the content of each cell
 * @param displayGrid Grid with the state of each cell
 * @param width Width of the grid
 * @param height Height of the grid
 * @param cursorX X position of the cursor
 * @param cursorY Y position of the cursor
 * @return 0 if success, -1 if error
 */
int showGameGrid(int *contentGrid, const int *displayGrid, int width, int height, int cursorX, int cursorY);

/**
 * @brief Wait for user input
 * @param contentGrid Grid with the content of each cell
 * @param displayGrid Grid with the state of each cell
 * @param width Width of the grid
 * @param height Height of the grid
 * @param coodX Pointer to the X position of the cursor
 * @param coordY Pointer to the Y position of the cursor
 * @param action Pointer to the action realized by the user
 * @return 0 if success, -1 if error
 */
int waitForInput(int *contentGrid, int *displayGrid, int width, int height, int *coordX, int *coordY, char *action);