#define WINDOWS         // To remove if we are not on windows
#define UNICODE_DISPLAY // To remove if we use cmd or powershell

#ifdef WINDOWS
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

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
    "\e[1;31m",            // FLAG
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
    "\e[48;2;100;100;100m", // CURRENT_CELL

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
#else
int _getch(void)
{
    int ch;
    struct termios oldt;
    struct termios newt;

    tcgetattr(STDIN_FILENO, &oldt);   /*store old settings */
    newt = oldt;                      /* copy old settings to new settings */
    newt.c_lflag &= ~(ICANON | ECHO); /* make one change to old settings in new settings */

    tcsetattr(STDIN_FILENO, TCSANOW, &newt); /*apply the new settings immediatly */

    ch = getchar(); /* standard getchar call */

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /*reapply the old settings */

    return ch; /*return received char */
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

void print_cell(int type, int isCursor)
{
    if (isCursor)
    {
        printf("%s", backgroundColors[3]);
    }
    else
    {
        printf("%s", backgroundColors[1]);
    }

    if (type == -2)
    {
        printf("%s", foregroundColors[1]);
        SHOW_FLAG();
        printf("%s", foregroundColors[0]);
    }
    else if (type == BOMB)
    {
        printf("%s", foregroundColors[2]);
        SHOW_BOMB();
        printf("%s", foregroundColors[0]);
    }
    else if (type == 0)
    {
        printf("  ");
        printf("%s", foregroundColors[0]);
    }
    else
    {
        printf("%s", foregroundColors[type + 2]);
        printf("%d ", type);
        printf("%s", foregroundColors[0]);
    }
}

int showGameGrid(int **contentGrid, int **displayGrid, int width, int height, int cursorX, int cursorY)
{
    // char **content = (char **)malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++)
    {
        if (i != 0)
        {
            printf("\n");
        }
        for (int j = 0; j < width; j++)
        {
            int isCursor = (i == cursorY && j == cursorX);
            if (displayGrid[i][j] == FLAG)
            {
                print_cell(-2, isCursor);
            }
            else if (displayGrid[i][j] == SHOWED_CELL)
            {
                print_cell(contentGrid[i][j], isCursor);
            }
            else
            {
                char *c;
                if (isCursor)
                    c = backgroundColors[3];
                else
                    c = backgroundColors[2];
                printf("%s  %s", c, foregroundColors[0]);
            }
        }
    }
    return 0;
}

int waitForInput(int **contentGrid, int **displayGrid, int width, int height, int *coordX, int *coordY, int *action)
{
    int x = 0;
    int y = 0;
    char flag = 1;
    while (flag)
    {
        // Clear screen
        printf("\e[1;1H\e[2J");
        // Show grid
        showGameGrid(contentGrid, displayGrid, width, height, x, y);
        // Get input
        char input;
        input = _getch();
        input = tolower(input);

        if (input == 'd')
        {
            x++;
        }
        else if (input == 'q')
        {
            x--;
        }
        else if (input == 'z')
        {
            y--;
        }
        else if (input == 's')
        {
            y++;
        }
        else if (input == 3)
        {
            // If ctrl+c exit
            // Exit directly, process is stopped instantly
            // TODO : free memory ?
            exit(1);
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
    for (int i = 0; i < 10; i++)
    {
        // Compute random coords
        int x = rand() % width;
        int y = rand() % height;
        // Add bomb
        contentGrid[y][x] = BOMB;
    }
    for (int i = 0; i < 10; i++)
    {
        // Compute random coords
        int x = rand() % width;
        int y = rand() % height;
        // Add flag
        displayGrid[y][x] = FLAG;
    }

    // Show random numbers
    for (int i = 0; i < 20; i++)
    {
        // Compute random coords
        int x = rand() % width;
        int y = rand() % height;
        // Add flag
        displayGrid[y][x] = SHOWED_CELL;
    }

    int x, y, action;
    waitForInput(contentGrid, displayGrid, width, height, &x, &y, &action);

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