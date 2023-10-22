#define WINDOWS // To remove if we are not on windows
#define UNICODE_DISPLAY // To remove if we use cmd or powershell

#ifdef WINDOWS
#include <windows.h>
#endif

#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

#define showElement(x) printf(x)
#ifdef UNICODE_DISPLAY
#define SHOW_FLAG() showElement("\u2691 ") // ⚑
#define SHOW_BOMB() showElement("\u2b24 ") // ⬤
#else
#define SHOW_FLAG() showElement("D ")
#define SHOW_BOMB() showElement("B ")
#endif

char *foregroundColors[] = {
    "\e[0m", // RESET

    // FOREGROUND
    "\e[1;31m", // FLAG
    "\e[1m\e[38;2;0;0;0m", // BOMB
    // NUMBERS
    // 1 #0101FF
    "\e[1m\e[38;2;1;1;255m",
    // 2 #007B00
    "\e[1m\e[38;2;0;123;0m",
    // 3 #FF0101
    "\e[1m\e[38;2;255;1;1m",
    // 4 #00007B
    "\e[1m\e[38;2;0;0;123m",
    // 5 #7C0303
    "\e[1m\e[38;2;124;3;3m",
    // 6 #048076
    "\e[1m\e[38;2;4;128;118m",
    // 7 #040303
    "\e[1m\e[38;2;4;3;3m",
    // 8 #818181
    "\e[1m\e[38;2;129;129;129m",


};
char *backgroundColors[] = {
    "\e[0m", // RESET
    // BACKGROUND
    "\e[48;2;240;240;240m", // SHOWED_CELL
    "\e[48;2;180;180;180m", // HIDDEN_CELL

};

#ifdef WINDOWS
int initializeWindowsConsole()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE)
    {
        return -1;
    }
    DWORD mode;
    if (!GetConsoleMode(hConsole, &mode))
    {
        return -1;
    }
    if (!SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
    {
        return -1;
    }
    if (!SetConsoleOutputCP(CP_UTF8))
    {
        return -1;
    }
    return 0;
}
#endif

int initializeDisplay()
{
#ifdef WINDOWS
    if (initializeWindowsConsole() < 0)
    {
        fprintf(stderr, "Error: Unable to initialize Windows console\n");
        return -1;
    }
#endif
    return 0;
}

void print_cell(int type)
{
    if (type == -2)
    {
        printf("%s", backgroundColors[1]);
        printf("%s", foregroundColors[1]);
        SHOW_FLAG();
        printf("%s", foregroundColors[0]);
    } else if (type == BOMB) {
        printf("%s", backgroundColors[1]);
        printf("%s", foregroundColors[2]);
        SHOW_BOMB();
        printf("%s", foregroundColors[0]);
    } else if (type == 0) {
        printf("%s", backgroundColors[1]);
        printf("  ");
        printf("%s", foregroundColors[0]);
    } else {
        printf("%s", backgroundColors[1]);
        printf("%s", foregroundColors[type + 2]);
        printf("%d ", type);
        printf("%s", foregroundColors[0]);
    }
}

int showGameGrid(int **contentGrid, int **displayGrid, int width, int height)
{
    // Clear screen
    printf("\e[1;1H\e[2J");
    for (int i = 0; i < height; i++)
    {
        printf("\n");
        for (int j = 0; j < width; j++)
        {
            if (displayGrid[i][j] == FLAG) {
                print_cell(-2);
            } else if (displayGrid[i][j] == SHOWED_CELL) {
                print_cell(contentGrid[i][j]);
            } else {
                printf("%s", backgroundColors[2]);
                printf("  ");
                printf("%s", foregroundColors[0]);
            }
        }
    }
    return 0;
}

int main()
{
    initializeDisplay();
    // print_cell(1);
    // printf("\e[1;31m \u2691 💣 ⬤ • ◮ ☐");

    // Create grids
    int width = 10;
    int height = 10;
    int **contentGrid = (int **)malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++)
    {
        contentGrid[i] = (int *)malloc(width * sizeof(int));
        for (int j = 0; j < width; j++)
        {
            // random content
            contentGrid[i][j] = rand() % 9;
        }
    }
    int **displayGrid = (int **)malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++)
    {
        displayGrid[i] = (int *)malloc(width * sizeof(int));
        for (int j = 0; j < width; j++)
        {
            displayGrid[i][j] = HIDDEN_CELL;
        }
    }
    // add random bombs and flags
    for(int i = 0; i < 10; i++) {
        // Compute random coords
        int x = rand() % width;
        int y = rand() % height;
        // Add bomb
        contentGrid[y][x] = BOMB;
    }
    for(int i = 0; i < 10; i++) {
        // Compute random coords
        int x = rand() % width;
        int y = rand() % height;
        // Add flag
        displayGrid[y][x] = FLAG;
    }

    // Show random numbers
    for(int i = 0; i < 20; i++) {
        // Compute random coords
        int x = rand() % width;
        int y = rand() % height;
        // Add flag
        displayGrid[y][x] = SHOWED_CELL;
    }


    showGameGrid(contentGrid, displayGrid, width, height);

    // Free grids
    for (int i = 0; i < height; i++)
    {
        free(contentGrid[i]);
        free(displayGrid[i]);
    }
    free(contentGrid);
    free(displayGrid);
    return 0;
}