#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>         
#include <assert.h>

#include "constants.h"
#include "display.h"

// Functions to manage a 2D table
/**
 * @brief Generate a table 2D with the given size
 * @param rows Number of rows
 * @param columns Number of columns
 * @return Pointer to the table
 */
int *generateTable(int rows, int columns)
{
    int *table = (int *)malloc(rows * columns * sizeof(int)); // Allocate memory for the linear table
    if (table == NULL)
    {
        fprintf(stderr, "Error: Unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows * columns; i++)
    {
        table[i] = 0; // Define the value of each cell to 0
    }
    return table;
}

/**
 * @brief Edit value of a table
 * @param table The table to edit
 * @param rows The number of rows of the table
 * @param columns The number of columns of the table
 * @param row The index of the row of the value to edit
 * @param column The index of the column of the value to edit
 * @param value The new value
 */
void editTable(int *table, int columns, int row, int column, int value)
{
    table[row * columns + column] = value; // Edit the value of the cell in the linear table
}

/**
 * @brief Get the value of cell in a table
 * @param table The table to get the cell value
 * @param rows The number of rows of the table
 * @param columns The number of columns of the table
 * @param row The index of the row of the cell to get the value
 * @param column The index of the column of the cell to get the value
 * @return
 */
int getTableValue(int *table, int columns, int row, int column)
{
    return table[row * columns + column]; // Get the value of the cell in the linear table
}

/**
 * @brief Count the number of cells with the given value
 * @param table The table to count the cells
 * @param rows The number of rows of the table
 * @param columns The number of columns of the table
 * @param value The value to count
 * @return The number of cells with the given value
 */
int countTable(const int *table, int rows, int columns, int value)
{
    int count = 0;
    for (int i = 0; i < rows * columns; i++)
    {
        if (table[i] == value)
        {
            count++;
        }
    }
    return count;
}

/**
 * @brief Get all the values around a cell
 * @param table The table to get the values
 * @param rows The number of rows of the table
 * @param columns The number of columns of the table
 * @param row The index of the row of the cell
 * @param column The index of the column of the cell
 * @return A pointer to the table with the values around the cell
 */
int *getAllTablesAroundCell(int *table, int rows, int columns, int row, int column)
{
    int *values = generateTable(8, 1);
    int Xpos[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int Ypos[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    for (int i = 0; i < 8; i++)
    {
        if (row + Ypos[i] < 0 || row + Ypos[i] >= rows || column + Xpos[i] < 0 || column + Xpos[i] >= columns)
        {
            values[i] = -1;
        }
        else
        {
            values[i] = getTableValue(table, columns, row + Ypos[i], column + Xpos[i]);
        }
    }
    return values;
}

// Fonction to play the bomb game

/**
 * @brief Reveal a cell and the cells around if the value is 0 for the given position
 * @param backTable The table with the bombs
 * @param frontTable The table with the state of the cells
 * @param rows The number of rows of the tables
 * @param columns The number of columns of the tables
 * @param x The index of the column of the cell to show
 * @param y The index of the row of the cell to show
 */
void showCell(int *backTable, int *frontTable, int rows, int columns, int x, int y)
{
    editTable(frontTable, columns, y, x, VISIBLE_CELL); // Edit the value of the cell to set to show it
    if (getTableValue(backTable, columns, y, x) != 0)
        return; // If the value of the cell is not 0, stop the function
    // Get all the values around the cell
    int *values = getAllTablesAroundCell(backTable, rows, columns, y, x);

    // Get the position of the cells around the cell
    editTable(frontTable, columns, y, x, VISIBLE_CELL);
    if (getTableValue(backTable, columns, y, x) != 0)
        return;
    // Show the next cells if the value is 0
    int Xpos[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int Ypos[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    for (int i = 0; i < 8; i++)
    {
        if (!(x + Xpos[i] < 0 || x + Xpos[i] >= columns || y + Ypos[i] < 0 || y + Ypos[i] >= rows)) // If the cell is in the table
        {
            int temp_value = getTableValue(frontTable, columns, y + Ypos[i], x + Xpos[i]);
            if (values[i] != BOMB && (temp_value == HIDDEN_CELL || temp_value == FLAG_CELL))
                editTable(frontTable, columns, y + Ypos[i], x + Xpos[i], VISIBLE_CELL); // If the cell is not a bomb and is hidden or flagged, show the cell
            if (values[i] == 0 && temp_value == HIDDEN_CELL)
            {
                showCell(backTable, frontTable, rows, columns, x + Xpos[i], y + Ypos[i]); // Show the cell and the cells around
            }
        }
    }
    free(values);
}

/**
 * @brief Set the game to lost and show all the bombs
 * @param table The table to print
 * @param rows The number of rows of the table
 * @param columns The number of columns of the table
 */
void loseGame(int *backTable, int *frontTable, int rows, int columns, int *gameState)
{
    *gameState = LOST; // Set the game to lost
    for (int i = 0; i < rows * columns; i++)
    {
        if (getTableValue(backTable, columns, i / columns, i % columns) == BOMB)
        {
            if (getTableValue(frontTable, columns, i / columns, i % columns) != FLAG_CELL)
            {
                editTable(frontTable, columns, i / columns, i % columns, VISIBLE_CELL);
            }
        }
    }
}

/**
 * @brief Set the game to won
 * @param gameState Pointer to the game state
 */
void winGame(int *gameState)
{
    *gameState = WON; // Set the game to won
}

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
void userInput(char input, int *backTable, int *frontTable, int rows, int columns, int bombNumbers, int *gameState, int x,
               int y)
{
    // Place or remove a flag
    if (input == ACTION_PLACE_FLAG)
    {
        int frontValue = getTableValue(frontTable, columns, y, x);
        if (frontValue == HIDDEN_CELL)
        {
            editTable(frontTable, columns, y, x, FLAG_CELL);
        }
        else if (frontValue == FLAG_CELL)
        {
            editTable(frontTable, columns, y, x, HIDDEN_CELL);
        }
    }
    else if (input == ACTION_DIG)
    {
        int backValue = getTableValue(backTable, columns, y, x);
        int frontValue = getTableValue(frontTable, columns, y, x);
        if (frontValue == HIDDEN_CELL)
        {
            if (backValue == BOMB)
            {
                loseGame(backTable, frontTable, rows, columns, gameState);
            }
            else
            {
                showCell(backTable, frontTable, rows, columns, x, y);
                int nbShowedCells = countTable(frontTable, rows, columns, VISIBLE_CELL);
                if (nbShowedCells == rows * columns - bombNumbers)
                {
                    *gameState = WON;
                }
            }
        }
    }
    else if (input == ACTION_QUIT)
    {
        free(backTable);
        free(frontTable);
        exit(1);
    }
}

#ifdef TEST_CORE
int test()
{
    // Tester le jeu avec génération du tableau, mise en place des bombes à des endroits précis, différentes entrées utilisateur, la fin du jeu
    // Définition des variables
    int rows = 6;
    int columns = 6;
    int bombNumbers = 5;
    int gameState = 0;

    // Génération du tableau avec les bombes et le nombre de bombes autour de chaque case
    // 0 = case vide
    // -1 = bombe
    // 1-8 = nombre de bombes autour de la case
    // Bombe en (0, 0), (1, 1)
    int *backTable = generateTable(rows, columns);
    assert(getTableValue(backTable, columns, 0, 0) == 0);
    assert(getTableValue(backTable, columns, 1, 1) == 0);
    editTable(backTable, columns, 0, 0, BOMB);
    assert(getTableValue(backTable, columns, 0, 0) == BOMB);
    assert(getTableValue(backTable, columns, 1, 1) == 0);
    editTable(backTable, columns, 1, 1, BOMB);
    assert(getTableValue(backTable, columns, 1, 1) == BOMB);
    assert(getTableValue(backTable, columns, 0, 1) == 0);

    editTable(backTable, columns, 0, 1, 2);
    assert(getTableValue(backTable, columns, 0, 1) == 2);
    editTable(backTable, columns, 0, 2, 1);
    assert(getTableValue(backTable, columns, 0, 2) == 1);
    editTable(backTable, columns, 0, 2, 5);
    assert(getTableValue(backTable, columns, 0, 2) == 5);

    // Génération du tableau qui sera affiché
    // 0 = case cachée
    // 1 = case affichée
    // 2 = case avec un drapeau
    int *frontTable = generateTable(rows, columns);

    // Mise en place du jeu sans interaction utilisateur
    char input = COMMAND_DOWN;
    userInput(input, backTable, frontTable, rows, columns, bombNumbers, &gameState, 0, 4);
    assert(getTableValue(frontTable, columns, 0, 4) == VISIBLE_CELL);

    input = COMMAND_PLACE_FLAG;
    userInput(input, backTable, frontTable, rows, columns, bombNumbers, &gameState, 0, 0);
    assert(getTableValue(frontTable, columns, 0, 0) == FLAG_CELL);

    input = COMMAND_DOWN;
    userInput(input, backTable, frontTable, rows, columns, bombNumbers, &gameState, 0, 0);
    assert(getTableValue(frontTable, columns, 0, 0) == FLAG_CELL);

    input = COMMAND_DOWN;
    userInput(input, backTable, frontTable, rows, columns, bombNumbers, &gameState, 1, 1);
    assert(getTableValue(frontTable, columns, 1, 1) == VISIBLE_CELL);

    free(backTable);
    free(frontTable);
    printf("Tests passed successfully\n");
    return 0;
}
int main()
{
    return test();
}
#endif // TEST_CORE
