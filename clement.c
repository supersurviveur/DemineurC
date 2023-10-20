#include <malloc.h>
#include <stdio.h>

char const PLACE_FLAG = 'f';
char const SHOW_CELL = 's';
int const FLAG = 2;
int const BOMB = -1;
int const HIDDEN_CELL = 0;
int const SHOWED_CELL = 1;
int const PLAYING = 0;
int const WON = 1;
int const LOST = 2;

// Functions to manage a 2D table

int *generateTable(int rows, int columns) {
    int *table = (int *) malloc(rows * columns * sizeof(int));
    if (table == NULL) {
        return NULL;
    }
    for (int i = 0; i < rows * columns; i++) {
        table[i] = 0;
    }
    return table;
}

void printTable(int *table, int rows, int columns) {
    for (int i = 0; i < rows * columns; i++) {
        if (i % columns == 0) {
            printf("\n");
        }
        printf("%d ", table[i]);
    }
    printf("\n");
}

void freeTable(int *table) {
    free(table);
}

void editTable(int *table, int rows, int columns, int row, int column, int value) {
    table[row * columns + column] = value;
}

int getTableValue(int *table, int rows, int columns, int row, int column) {
    return table[row * columns + column];
}

int countTable(const int *table, int rows, int columns, int value) {
    int count = 0;
    for (int i = 0; i < rows * columns; i++) {
        if (table[i] == value) {
            count++;
        }
    }
    return count;
}

int *getAllTablesAroundCell(int *table, int rows, int columns, int row, int column) {
    int *values = generateTable(8, 1);
    values[0] = getTableValue(table, rows, columns, row - 1, column - 1);
    values[1] = getTableValue(table, rows, columns, row - 1, column);
    values[2] = getTableValue(table, rows, columns, row - 1, column + 1);
    values[3] = getTableValue(table, rows, columns, row, column - 1);
    values[4] = getTableValue(table, rows, columns, row, column + 1);
    values[5] = getTableValue(table, rows, columns, row + 1, column - 1);
    values[6] = getTableValue(table, rows, columns, row + 1, column);
    values[7] = getTableValue(table, rows, columns, row + 1, column + 1);
    return values;
}

// Fonction to play the bomb game

void showCell(int *backTable, int *frontTable, int rows, int columns, int x, int y) {
    // Get all the values around the cell
    int *values = getAllTablesAroundCell(backTable, rows, columns, x, y);
    // Show the next cells if the value is 0
    // TODO: Fonction à corriger car pas toutes les cases sont affichées
    for (int i = 0; i < 8; ++i) {
        editTable(frontTable, rows, columns, x + i / 3 - 1, y + i % 3 - 1, SHOWED_CELL);
        if (values[i] == 0 && getTableValue(frontTable, rows, columns, x + i / 3 - 1, y + i % 3 - 1) == HIDDEN_CELL) {
            showCell(backTable, frontTable, rows, columns, x + i / 3 - 1, y + i % 3 - 1);
        }
    }
    freeTable(values);
}

void loseGame(int *backTable, int *frontTable, int rows, int columns, int *gameState) {
    *gameState = LOST;
    for (int i = 0; i < rows * columns; i++) {
        if (getTableValue(backTable, rows, columns, i / columns, i % columns) == BOMB) {
            if (getTableValue(frontTable, rows, columns, i / columns, i % columns) != FLAG) {
                editTable(frontTable, rows, columns, i / columns, i % columns, SHOWED_CELL);
            }
        }
    }
    printf("You lost !\n");
}

void winGame(int *backTable, int *frontTable, int rows, int columns, int *gameState) {
    *gameState = WON;
    printf("You won !\n");
}

void
userInput(char input, int *backTable, int *frontTable, int rows, int columns, int bombNumbers, int *gameState, int x,
          int y) {
    if (input == PLACE_FLAG) {
        int frontValue = getTableValue(frontTable, rows, columns, x, y);
        if (frontValue == HIDDEN_CELL) {
            editTable(frontTable, rows, columns, x, y, FLAG);
            int nbFlags = countTable(frontTable, rows, columns, FLAG);
            if (nbFlags == bombNumbers) {
                winGame(backTable, frontTable, rows, columns, gameState);
            }
        } else if (frontValue == FLAG) {
            editTable(frontTable, rows, columns, x, y, HIDDEN_CELL);
        }
    } else if (input == SHOW_CELL) {
        int backValue = getTableValue(backTable, rows, columns, x, y);
        int frontValue = getTableValue(frontTable, rows, columns, x, y);
        if (frontValue == HIDDEN_CELL) {
            if (backValue == BOMB) {
                loseGame(backTable, frontTable, rows, columns, gameState);
            } else {
                editTable(frontTable, rows, columns, x, y, SHOWED_CELL);
                showCell(backTable, frontTable, rows, columns, x, y);
                int nbShowedCells = countTable(frontTable, rows, columns, SHOWED_CELL);
                if (nbShowedCells == rows * columns - bombNumbers) {
                    winGame(backTable, frontTable, rows, columns, gameState);
                }
            }
        }
    }
}

int main() {
    // Tester le jeu avec génération du tableau, mise en place des bombes à des endroits précis, différentes entrées utilisateur, la fin du jeu
    // Définition des variables
    int rows = 6;
    int columns = 6;
    int bombNumbers = 5;
    int gameState = PLAYING;
    int x = 0;
    int y = 0;
    char input = 's';

    // Génération du tableau avec les bombes et le nombre de bombes autour de chaque case
    // 0 = case vide
    // -1 = bombe
    // 1-8 = nombre de bombes autour de la case
    // Bombe en (0, 0), (1, 1), (2, 2), (3, 3), (4, 4)
    int *backTable = generateTable(rows, columns);
    editTable(backTable, rows, columns, 0, 0, BOMB);
    editTable(backTable, rows, columns, 1, 1, BOMB);
    editTable(backTable, rows, columns, 2, 2, BOMB);
    editTable(backTable, rows, columns, 3, 3, BOMB);
    editTable(backTable, rows, columns, 4, 4, BOMB);
    editTable(backTable, rows, columns, 0, 1, 2);
    editTable(backTable, rows, columns, 0, 2, 1);
    editTable(backTable, rows, columns, 1, 0, 2);
    editTable(backTable, rows, columns, 1, 2, 2);
    editTable(backTable, rows, columns, 1, 3, 1);
    editTable(backTable, rows, columns, 2, 0, 1);
    editTable(backTable, rows, columns, 2, 1, 2);
    editTable(backTable, rows, columns, 2, 3, 2);
    editTable(backTable, rows, columns, 2, 4, 1);
    editTable(backTable, rows, columns, 3, 1, 1);
    editTable(backTable, rows, columns, 3, 2, 2);
    editTable(backTable, rows, columns, 3, 4, 2);
    editTable(backTable, rows, columns, 3, 5, 1);
    editTable(backTable, rows, columns, 4, 2, 1);
    editTable(backTable, rows, columns, 4, 3, 2);
    editTable(backTable, rows, columns, 4, 5, 1);
    editTable(backTable, rows, columns, 5, 3, 1);
    editTable(backTable, rows, columns, 5, 4, 1);
    editTable(backTable, rows, columns, 5, 5, 1);

    // Génération du tableau qui sera affiché
    // 0 = case cachée
    // 1 = case affichée
    // 2 = case avec un drapeau
    int *frontTable = generateTable(rows, columns);

    // Affichage des tableaux
    printf("Initial tables:\n");
    printTable(frontTable, rows, columns);
    printTable(backTable, rows, columns);

    // Mise en place du jeu sans interaction utilisateur
    printf("Show the (0,4) character\n");
    x = 0;
    y = 4;
    userInput(input, backTable, frontTable, rows, columns, bombNumbers, &gameState, x, y);
    printTable(frontTable, rows, columns);
    printTable(backTable, rows, columns);

    printf("Flag the (0,0) character\n");
    x = 0;
    y = 0;
    input = 'f';
    userInput(input, backTable, frontTable, rows, columns, bombNumbers, &gameState, x, y);
    printTable(frontTable, rows, columns);
    printTable(backTable, rows, columns);

    printf("Show the (0,0) character\n");
    x = 0;
    y = 0;
    input = 's';
    userInput(input, backTable, frontTable, rows, columns, bombNumbers, &gameState, x, y);
    printTable(frontTable, rows, columns);
    printTable(backTable, rows, columns);

    printf("Show the (1,1) character\n");
    x = 1;
    y = 1;
    input = 's';
    userInput(input, backTable, frontTable, rows, columns, bombNumbers, &gameState, x, y);
    printTable(frontTable, rows, columns);
    printTable(backTable, rows, columns);

    printf("Game state: %d\n", gameState);
}