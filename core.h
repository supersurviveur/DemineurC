/**
 * @file core.h
 * @brief Provides functions to manage the core of the game (grid management, check if the game is won or lost, ...)
 * @authors Julien THILLARD, Clement DI-GALLO, Benjamin TRUC
 */

/**
 * @brief Play the user input
 * @param input The user input
 * @param backTable The table with the bombs
 * @param frontTable The table with the state of the cells
 * @param rows The number of rows of the tables
 * @param columns The number of columns of the tables
 * @param bombNumbers The number of bombs
 * @param gameState The game state
 * @param x The index of the column of the cell to do the action
 * @param y The index of the row of the cell to do the action
 */
void userInput(char input, int *backTable, int *frontTable, int rows, int columns, int bombNumbers, int *gameState, int x, int y);

/**
 * @brief Generate a table 2D with the given size
 * @param rows Number of rows
 * @param columns Number of columns
 * @return Pointer to the table if success, NULL if error
 */
int *generateTable(int rows, int columns);