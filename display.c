#define UNICODE_DISPLAY // To remove if we use cmd or powershell
#define TEST            // To remove if we are not testing

// TODO free malloc calls

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "constants.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>

// #define showElement(x) printf(x)
#ifdef UNICODE_DISPLAY
#define SHOW_FLAG() "\u2691 " // ⚑
#define SHOW_BOMB() "\u2b24 " // ⬤
#else
#define SHOW_FLAG() "D "
#define SHOW_BOMB() "B "
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

#ifdef _WIN32
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
#ifdef _WIN32
    if (initializeWindowsConsole() < 0)
    {
        fprintf(stderr, "Error: Unable to initialize windows console\n");
        return -1;
    }
#endif
    return 0;
}

char *print_cell(int type, int isCursor)
{
    char *result = (char *)malloc(100 * sizeof(char));
    int position;
    if (isCursor)
    {
        position = sprintf(result, "%s", backgroundColors[3]);
    }
    else
    {
        position = sprintf(result, "%s", backgroundColors[1]);
    }

    if (type == -2)
    {
        sprintf(result + position, "%s%s%s", foregroundColors[1], SHOW_FLAG(), foregroundColors[0]);
    }
    else if (type == BOMB)
    {
        sprintf(result + position, "%s%s%s", foregroundColors[2], SHOW_BOMB(), foregroundColors[0]);
    }
    else if (type == 0)
    {
        sprintf(result + position, "  %s", foregroundColors[0]);
    }
    else
    {
        sprintf(result + position, "%s%d %s", foregroundColors[type + 2], type, foregroundColors[0]);
    }
    return result;
}

int showGameGrid(int *contentGrid, int *displayGrid, int width, int height, int cursorX, int cursorY)
{
    char **content = (char **)malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++)
    {
        content[i] = (char *)malloc(width * 100 * sizeof(char));
        int position = 0;
        if (i != 0)
        {
            position = sprintf(content[i], "\n");
        }
        for (int j = 0; j < width; j++)
        {
            int isCursor = (i == cursorY && j == cursorX);
            if (displayGrid[i * width + j] == FLAG)
            {
                position += sprintf(content[i] + position, "%s", print_cell(-2, isCursor));
            }
            else if (displayGrid[i * width + j] == SHOWED_CELL)
            {
                position += sprintf(content[i] + position, "%s", print_cell(contentGrid[i * width + j], isCursor));
            }
            else
            {
                char *c;
                if (isCursor)
                    c = backgroundColors[3];
                else
                    c = backgroundColors[2];
                position += sprintf(content[i] + position, "%s  %s", c, foregroundColors[0]);
            }
        }
    }
    // Flatten content
    char *result = (char *)malloc(height * width * 100 * sizeof(char));
    int position = sprintf(result, "%s", "\e[?25l\e[1;1H\e[2J"); // Clear screen, hide cursor
    for (int i = 0; i < height; i++)
    {
        position += sprintf(result + position, "%s", content[i]);
    }
    position += sprintf(result + position, "\n"); // Show cursor
    // Finally, show the entire grid in one call
    fwrite(result, position, 1, stdout);
    return 0;
}

int updateGameGrid(int *contentGrid, int *displayGrid, int width, int height, int cursorX, int cursorY, int oldCursorX, int oldCursorY)
{
    // Write on console only where changes occured
    char *content = (char *)malloc(100 * sizeof(char));
    int position = 0;
    // Edit old cursor
    if (displayGrid[oldCursorY * width + oldCursorX] == FLAG)
    {
        position += sprintf(content, "\e[%d;%dH%s", oldCursorY + 1, oldCursorX * 2 + 1, print_cell(-2, 0));
    }
    else if (displayGrid[oldCursorY * width + oldCursorX] == SHOWED_CELL)
    {
        position += sprintf(content, "\e[%d;%dH%s", oldCursorY + 1, oldCursorX * 2 + 1, print_cell(contentGrid[oldCursorY * width + oldCursorX], 0));
    }
    else
    {
        position += sprintf(content, "\e[%d;%dH%s  %s", oldCursorY + 1, oldCursorX * 2 + 1, backgroundColors[2], foregroundColors[0]);
    }
    // // Edit new cursor
    if (displayGrid[cursorY * width + cursorX] == FLAG)
    {
        position += sprintf(content + position, "\e[%d;%dH%s", cursorY + 1, cursorX * 2 + 1, print_cell(-2, 1));
    }
    else if (displayGrid[cursorY * width + cursorX] == SHOWED_CELL)
    {
        position += sprintf(content + position, "\e[%d;%dH%s", cursorY + 1, cursorX * 2 + 1, print_cell(contentGrid[cursorY * width + cursorX], 1));
    }
    else
    {
        position += sprintf(content + position, "\e[%d;%dH%s  %s", cursorY + 1, cursorX * 2 + 1, backgroundColors[3], foregroundColors[0]);
    }
    // Show changes
    fwrite(content, position, 1, stdout);
    return 0;
}

int waitForInput(int *contentGrid, int *displayGrid, int width, int height, int *coordX, int *coordY, int *action)
{
    int x = 0;
    int y = 0;
    int oldX = 0;
    int oldY = 0;
    bool flag = 1;
    // Show grid
    showGameGrid(contentGrid, displayGrid, width, height, x, y);
    while (flag)
    {
        // Update grid
        updateGameGrid(contentGrid, displayGrid, width, height, x, y, oldX, oldY);
        // Get input
        char input;
        input = _getch();
        input = tolower(input);

        oldX = x;
        oldY = y;

        if (input == 'd')
        {
            x++;
            if (x >= width)
                x = width - 1;
        }
        else if (input == 'q')
        {
            x--;
            if (x < 0)
                x = 0;
        }
        else if (input == 'z')
        {
            y--;
            if (y < 0)
                y = 0;
        }
        else if (input == 's')
        {
            y++;
            if (y >= height)
                y = height - 1;
        }
        else if (input == 3)
        {
            // If ctrl+c exit
            // Exit directly, process is stopped instantly
            // TODO : free memory ?
            exit(1);
        }
        else if (input == 'f')
        {
            *coordX = x;
            *coordY = y;
            *action = FLAG;
            flag = 0;
        }
        else if (input == 'e')
        {
            *coordX = x;
            *coordY = y;
            *action = SHOW_CELL;
            flag = 0;
        }
    }
    return 0;
}

#ifdef TEST
int main()
{
    initializeDisplay();
    // print_cell(1);
    // printf("\e[1;31m \u2691 💣 ⬤ • ◮ ☐");

    // Create grids
    int width = 10;
    int height = 10;
    int *contentGrid = (int *)malloc(width * height * sizeof(int *));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // random content
            contentGrid[i * width + j] = rand() % 9;
        }
    }
    int *displayGrid = (int *)malloc(width * height * sizeof(int *));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            displayGrid[i * width + j] = HIDDEN_CELL;
        }
    }
    // add random bombs and flags
    for (int i = 0; i < 10; i++)
    {
        // Compute random coords
        int x = rand() % width;
        int y = rand() % height;
        // Add bomb
        contentGrid[y * width + x] = BOMB;
    }
    for (int i = 0; i < 10; i++)
    {
        // Compute random coords
        int x = rand() % width;
        int y = rand() % height;
        // Add flag
        displayGrid[y * width + x] = FLAG;
    }

    // Show random numbers
    for (int i = 0; i < 20; i++)
    {
        // Compute random coords
        int x = rand() % width;
        int y = rand() % height;
        // Add flag
        displayGrid[y * width + x] = SHOWED_CELL;
    }

    int x, y, action;
    waitForInput(contentGrid, displayGrid, width, height, &x, &y, &action);

    // Free grids
    free(contentGrid);
    free(displayGrid);

    // Clear screen
    printf("\e[?25h\e[1;1H\e[2J");
    return 0;
}
#endif