#define UNICODE_DISPLAY // To remove if we use cmd or powershell

// TODO add comments
// TODO better error handling

// Includes different libraries depending on the OS
// These libs are used for getting input without pressing enter, and for ascii colors on windows
// _WIN32 is defined by the compiler
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#include "constants.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

// Define characters to display for each type of cell, in unicode or letters, depending on UNICODE_DISPLAY flag
#ifdef UNICODE_DISPLAY
#define SHOW_FLAG() "\u2691 " // ⚑
#define SHOW_BOMB() "\u2b24 " // ⬤
#else
#define SHOW_FLAG() "D "
#define SHOW_BOMB() "B "
#endif

// Colors for texts
char *foregroundColors[] = {
    "\e[0m", // RESET

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
// Colors for backgrounds
char *backgroundColors[] = {
    "\e[0m", // RESET

    "\e[48;2;240;240;240m", // SHOWED_CELL
    "\e[48;2;180;180;180m", // HIDDEN_CELL
    "\e[48;2;100;100;100m", // CURRENT_CELL

};

// On windows, we need to initialize the console to be able to use unicode characters and ascii colors
#ifdef _WIN32
/**
 * @brief Initialize windows console to be able to use unicode characters and ascii colors
 * @return 0 if success, -1 if error
 */
int initializeWindowsConsole()
{
    // Get current console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE)
    {
        return -1;
    }
    // Get current console mode
    DWORD mode;
    if (!GetConsoleMode(hConsole, &mode))
    {
        return -1;
    }
    // Enable ascii colors mode
    if (!SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
    {
        return -1;
    }
    // Enable unicode characters
    if (!SetConsoleOutputCP(CP_UTF8))
    {
        return -1;
    }
    return 0;
}

// Function to get terminal size is also different on windows/linux
/**
 * @brief Get terminal size
 * @param width Pointer to the width of the terminal
 * @param height Pointer to the height of the terminal
 * @return 0 if success, -1 if error
 */
int getTerminalSize(int *width, int *height)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        return -1;
    }
    *width = csbi.dwSize.X-2;
    *height = csbi.dwSize.Y-2;
    return 0;
}
#else
// On linux, we need to use a custom function to get input without pressing enter
/**
 * @brief Get input without pressing enter
 * @return input char
 */
int _getch()
{
    int ch;
    struct termios oldt;
    struct termios newt;

    tcgetattr(STDIN_FILENO, &oldt);   // Get current settings of stdin
    newt = oldt;                      // Copy them
    newt.c_lflag &= ~(ICANON | ECHO); // Add flags to get input without pressing enter

    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new settings

    ch = getchar(); // Get input

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old settings

    return ch;
}

/**
 * @brief Get terminal size
 * @param width Pointer to the width of the terminal
 * @param height Pointer to the height of the terminal
 * @return 0 if success, -1 if error
 */
int getTerminalSize(int *width, int *height)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *width = w.ws_col;
    *height = w.ws_row;
    return 0;
}
#endif

/**
 * @brief Initialize display
 * @return 0 if success, -1 if error
 */
int initializeDisplay(void)
{
    // On windows, we try to initialize the console
#ifdef _WIN32
    if (initializeWindowsConsole() < 0)
    {
        fprintf(stderr, "Error: Unable to initialize windows console\n");
        return -1;
    }
#endif
    return 0;
}

/**
 * @brief Restore display to default
 */
void restoreDisplay(void)
{
    // Only restore ascii things like cursor (sometimes it's useful)
    printf("\e[?25h");
}

/**
 * @brief Get the size of the grid
 * @param gridWidth Pointer to the width of the grid
 * @param gridHeight Pointer to the height of the grid
 * @param nbBombs Pointer to the number of bombs
 * @return 0 if success, -1 if error
 */
int getGameGridSize(int *gridWidth, int *gridHeight, int *nbBombs)
{
    // Get width
    printf("Enter grid width (must be >= 5): ");
    while (scanf("%d", gridWidth) != 1 || (*gridWidth != 0 && *gridWidth < 5))
    {
        fprintf(stderr, "Error: Invalid input\n");
        // Clear input buffer
        while (getchar() != '\n')
            ;
        printf("Enter grid width (must be >= 5): ");
    }
    if (*gridWidth == 0)
    {
        int _temp;
        getTerminalSize(gridWidth, &_temp);
        *gridWidth = *gridWidth / 2;
    }
    // Get height
    printf("Enter grid height (must be >= 5): ");
    while (scanf("%d", gridHeight) != 1 || (*gridHeight != 0 && *gridHeight < 5))
    {
        fprintf(stderr, "Error: Invalid input\n");
        // Clear input buffer
        while (getchar() != '\n')
            ;
        printf("Enter grid height (must be >= 5): ");
    }
    if (*gridHeight == 0)
    {
        int _temp;
        getTerminalSize(&_temp, gridHeight);
    }
    // Get difficulty
    int difficulty;
    printf("Enter difficulty (Easy: 1, Normal: 2, Difficult: 3): ");
    while (scanf("%d", &difficulty) != 1 || difficulty < 1 || difficulty > 3)
    {
        fprintf(stderr, "Error: Invalid input\n");
        // Clear input buffer
        while (getchar() != '\n')
            ;
        printf("Enter difficulty (Easy: 1, Normal: 2, Difficult: 3): ");
    }

    // Compute number of bombs
    if (difficulty == 1) // 10% of the grid
    {
        *nbBombs = (*gridWidth * *gridHeight) * 0.10;
    }
    else if (difficulty == 2) // 15% of the grid
    {
        *nbBombs = (*gridWidth * *gridHeight) * 0.15;
    }
    else if (difficulty == 3) // 20% of the grid
    {
        *nbBombs = (*gridWidth * *gridHeight) * 0.20;
    }

    return 0;
}

/**
 * @brief Print a single cell
 * @param type Type of cell
 * @param isCursor 1 if cell is the cursor, 0 otherwise
 * @return String to print
 */
char *print_cell(int type, int isCursor)
{
    // Allocate memory for result
    char *result = (char *)malloc(100 * sizeof(char));
    // Choose background color depending on isCursor value
    int position = sprintf(result, "%s", backgroundColors[isCursor ? 3 : 1]);
    if (type == -2)
    {
        // Show a flag
        position += sprintf(result + position, "%s%s", foregroundColors[1], SHOW_FLAG());
    }
    else if (type == BOMB)
    {
        // Show a bomb
        position += sprintf(result + position, "%s%s", foregroundColors[2], SHOW_BOMB());
    }
    else if (type == 0)
    {
        // Show an empty cell
        position += sprintf(result + position, "  ");
    }
    else
    {
        // Show the number of bombs around
        position += sprintf(result + position, "%s%d ", foregroundColors[type + 2], type);
    }
    // Reset colors
    sprintf(result + position, "%s", foregroundColors[0]);
    return result;
}

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
int showGameGrid(int *contentGrid, const int *displayGrid, int width, int height, int cursorX, int cursorY)
{
    // Create a 2D array of strings
    char **content = (char **)malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++)
    {
        // Allocate memory for each line
        content[i] = (char *)malloc(width * 100 * sizeof(char));
        int position = 0;
        if (i != 0)
        {
            // If we are not on the first line, add a newline
            position = sprintf(content[i], "\n");
        }
        for (int j = 0; j < width; j++)
        {
            // Check if current cell is the cursor
            int isCursor = (i == cursorY && j == cursorX);

            if (displayGrid[i * width + j] == FLAG)
            {
                // Add a flag
                char *cell = print_cell(-2, isCursor);
                position += sprintf(content[i] + position, "%s", cell);
                free(cell);
            }
            else if (displayGrid[i * width + j] == VISIBLE_CELL)
            {
                // Add the content of the cell
                char *cell = print_cell(contentGrid[i * width + j], isCursor);
                position += sprintf(content[i] + position, "%s", cell);
                free(cell);
            }
            else
            {
                // Add an empty hidden cell
                // Choose background color depending on isCursor value
                char *c = backgroundColors[isCursor ? 3 : 2];
                // Add an empty cell
                position += sprintf(content[i] + position, "%s  %s", c, foregroundColors[0]);
            }
        }
    }
    // Flatten content, to be able to show the entire grid in one call
    char *result = (char *)malloc(height * width * 100 * sizeof(char));
    int position = sprintf(result, "%s", "\e[?25l\e[1;1H\e[2J"); // Clear screen, hide cursor
    for (int i = 0; i < height; i++)
    {
        // Add each line
        position += sprintf(result + position, "%s", content[i]);
    }
    position += sprintf(result + position, "\n");
    // Finally, show the entire grid in one call
    fwrite(result, position, 1, stdout);

    // Free memory
    for (int i = 0; i < height; i++)
    {
        free(content[i]);
    }
    free(content);
    free(result);
    return 0;
}

/**
 * @brief Update the grid, only where changes occurred
 * @param contentGrid Grid with the content of each cell
 * @param displayGrid Grid with the state of each cell
 * @param width Width of the grid
 * @param cursorX X position of the cursor
 * @param cursorY Y position of the cursor
 * @param oldCursorX X position of the cursor before the update
 * @param oldCursorY Y position of the cursor before the update
 * @return 0 if success, -1 if error
 */
int updateGameGrid(int *contentGrid, const int *displayGrid, int width, int cursorX, int cursorY, int oldCursorX, int oldCursorY)
{
    // Write on console only where changes occurred
    char *content = (char *)malloc(200 * sizeof(char));
    int position = 0;
    // Edit old cursor
    if (displayGrid[oldCursorY * width + oldCursorX] == FLAG)
    {
        char *cell = print_cell(-2, 0);
        position += sprintf(content, "\e[%d;%dH%s", oldCursorY + 1, oldCursorX * 2 + 1, cell);
        free(cell);
    }
    else if (displayGrid[oldCursorY * width + oldCursorX] == VISIBLE_CELL)
    {
        char *cell = print_cell(contentGrid[oldCursorY * width + oldCursorX], 0);
        position += sprintf(content, "\e[%d;%dH%s", oldCursorY + 1, oldCursorX * 2 + 1, cell);
        free(cell);
    }
    else
    {
        position += sprintf(content, "\e[%d;%dH%s  %s", oldCursorY + 1, oldCursorX * 2 + 1, backgroundColors[2], foregroundColors[0]);
    }
    // // Edit new cursor
    if (displayGrid[cursorY * width + cursorX] == FLAG)
    {
        char *cell = print_cell(-2, 1);
        position += sprintf(content + position, "\e[%d;%dH%s", cursorY + 1, cursorX * 2 + 1, cell);
        free(cell);
    }
    else if (displayGrid[cursorY * width + cursorX] == VISIBLE_CELL)
    {
        char *cell = print_cell(contentGrid[cursorY * width + cursorX], 1);
        position += sprintf(content + position, "\e[%d;%dH%s", cursorY + 1, cursorX * 2 + 1, cell);
        free(cell);
    }
    else
    {
        position += sprintf(content + position, "\e[%d;%dH%s  %s", cursorY + 1, cursorX * 2 + 1, backgroundColors[3], foregroundColors[0]);
    }
    // Show changes
    fwrite(content, position, 1, stdout);
    // Free memory
    free(content);
    return 0;
}

/**
 * @brief Wait for user input
 * @param contentGrid Grid with the content of each cell
 * @param displayGrid Grid with the state of each cell
 * @param width Width of the grid
 * @param height Height of the grid
 * @param coordX Pointer to the X position of the cursor
 * @param coordY Pointer to the Y position of the cursor
 * @param action Pointer to the action realized by the user
 * @return 0 if success, -1 if error
 */
int waitForInput(int *contentGrid, int *displayGrid, int width, int height, int *coordX, int *coordY, char *action)
{
    // Initialize cursor position
    int x = *coordX;
    int y = *coordY;
    int oldX = x;
    int oldY = y;
    bool flag = 1;
    // Show grid
    showGameGrid(contentGrid, displayGrid, width, height, x, y);
    while (flag)
    {
        // Update grid
        updateGameGrid(contentGrid, displayGrid, width, x, y, oldX, oldY);
        // Get input
        int input;
        input = _getch();
        input = tolower(input);

        // Save old cursor position
        oldX = x;
        oldY = y;

        // Process input
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
        else if (input == 'f' || input == 'e')
        {
            // Pass cursor position in pointers
            *coordX = x;
            *coordY = y;
            // Pass action in pointer, depending on input
            *action = input == 'f' ? ACTION_PLACE_FLAG : ACTION_DIG;
            // Exit loop
            flag = 0;
        }
    }
    return 0;
}

/**
 * @brief Display win message
 * @param contentGrid Grid with the content of each cell
 * @param displayGrid Grid with the state of each cell
 * @param width Width of the grid
 * @param height Height of the grid
 */
int displayWin(int *contentGrid, int *displayGrid, int width, int height)
{
    showGameGrid(contentGrid, displayGrid, width, height, -1, -1);
    printf("You won !");
    return 0;
}
/**
 * @brief Display lose message
 * @param contentGrid Grid with the content of each cell
 * @param displayGrid Grid with the state of each cell
 * @param width Width of the grid
 * @param height Height of the grid
 */
int displayLoose(int *contentGrid, int *displayGrid, int width, int height)
{
    showGameGrid(contentGrid, displayGrid, width, height, -1, -1);
    printf("You lose !");
    return 0;
}

// If TEST is defined, we create a random grid and show it
#ifdef TEST
int test()
{
    initializeDisplay();

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

    int x, y;
    char action;
    waitForInput(contentGrid, displayGrid, width, height, &x, &y, &action);

    // Free grids
    free(contentGrid);
    free(displayGrid);

    // Clear screen
    printf("\e[?25h\e[1;1H\e[2J");
    return 0;
}
int main()
{
    return test();
}
#endif
