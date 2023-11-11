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
#define SHOW_FLAG() "\e[1;31m\u2691 "            // ⚑
#define SHOW_BOMB() "\e[1m\e[38;2;0;0;0m\u2b24 " // ⬤
#else
#define SHOW_FLAG() "\e[1;31mD "            // D
#define SHOW_BOMB() "\e[1m\e[38;2;0;0;0mB " // B
#endif

// Colors for texts
char *foregroundColors[] = {
    "\e[0m", // RESET
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
 * @return 0 if success, -1 if failure
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
 */
void getTerminalSize(int *width, int *height)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    // Get infos on the console size
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        // Function fail
        fprintf(stderr, "Error: Unable to get terminal size\n");
        exit(EXIT_FAILURE);
    }
    // Pointer get the value-2, to be sure to fit in the size of the console
    *width = csbi.srWindow.Right - csbi.srWindow.Left - 2;
    *height = csbi.srWindow.Bottom - csbi.srWindow.Top - 2;
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
 */
void getTerminalSize(int *width, int *height)
{
    struct winsize w;
    // Get infos on the console size
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
    {
        // Function fail
        fprintf(stderr, "Error: Unable to get terminal size\n");
        exit(EXIT_FAILURE);
    }
    // Pointer get the value-2, to be sure to fit in the size of the console
    *width = w.ws_col - 2;
    *height = w.ws_row - 2;
}
#endif

/**
 * @brief Initialize display
 * @return 0 if success, -1 if failure
 */
int initializeDisplay()
{
    // On windows, we try to initialize the console
#ifdef _WIN32
    if (initializeWindowsConsole() < 0)
    {
        fprintf(stderr, "Error: Unable to initialize windows console\n");
        return -1;
    }
#endif
    printf("\e[?25l\e[1;1H\e[2J"); // hide cursor and clear screen
    return 0;
}

/**
 * @brief Restore display to default
 */
void restoreDisplay()
{
    // Only restore ascii things like cursor (sometimes it's useful)
    printf("\e[?25h");
}

/**
 * @brief Allocate memory, and exit if error
 * @param size Size of the memory to allocate
 * @return Pointer to the allocated memory
 */
void *allocateMemory(int size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Error: Unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

/**
 * @brief Get the size of the grid
 * @param gridWidth Pointer to the width of the grid
 * @param gridHeight Pointer to the height of the grid
 * @param nbBombs Pointer to the number of bombs
 */
void getGameGridSize(int *gridWidth, int *gridHeight, int *nbBombs)
{
    printf("\e[?25h"); // Restore cursor for inputs
    // Get width
    printf("Enter grid width (must be >= 5, or 0 to take the width of the console): ");
    while (scanf("%d", gridWidth) != 1 || (*gridWidth != 0 && *gridWidth < 5))
    {
        printf("Error: Invalid input\n");
        // Clear input buffer
        while (getchar() != '\n')
            ;
        printf("Enter grid width (must be >= 5, or 0 to take the width of the console): ");
    }
    if (*gridWidth == 0)
    {
        // If input is 0, get the width of the console
        int _temp;
        getTerminalSize(gridWidth, &_temp);
        // Divide by 2, because one cell is represented by 2 char
        *gridWidth = *gridWidth / 2;
    }
    // Get height
    printf("Enter grid height (must be >= 5, or 0 to take the width of the console): ");
    while (scanf("%d", gridHeight) != 1 || (*gridHeight != 0 && *gridHeight < 5))
    {
        printf("Error: Invalid input\n");
        // Clear input buffer
        while (getchar() != '\n')
            ;
        printf("Enter grid height (must be >= 5, or 0 to take the width of the console): ");
    }
    if (*gridHeight == 0)
    {
        // If input is 0, get the height of the console
        int _temp;
        getTerminalSize(&_temp, gridHeight);
        // Remove 1 to be sure to fit in the size of the console
        *gridHeight = *gridHeight - 1;
    }
    // Get difficulty
    int difficulty;
    printf("Enter difficulty (Easy: 1, Normal: 2, Difficult: 3): ");
    while (scanf("%d", &difficulty) != 1 || difficulty < 1 || difficulty > 3)
    {
        printf("Error: Invalid input\n");
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
    printf("\e[?25l\e[1;1H\e[2J"); // hide cursor and clear screen for game
}

/**
 * @brief Print a single cell
 * @param type Type of cell ()
 * @param isCursor 1 if cell is the cursor, 0 otherwise
 * @return String to print
 */
char *printCell(int type, int isCursor)
{
    // Allocate memory for result
    char *result = (char *)allocateMemory(100 * sizeof(char));
    char *content;
    if (type == -2)
    {
        content = SHOW_FLAG();
    }
    else if (type == BOMB)
    {
        content = SHOW_BOMB();
    }
    else if (type == 0)
    {
        // Show an empty cell
        content = "  ";
    }
    else
    {
        // Show the color associated with the number of bombs around
        content = foregroundColors[type];
    }

    // Choose background color depending on isCursor value, and then print the content
    int position = sprintf(result, "%s%s", backgroundColors[isCursor ? 3 : 1], content);
    if (type > 0)
    {
        // If the case must show the number of bombs around, add the number
        position += sprintf(result + position, "%d ", type);
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
 */
void showGameGrid(int *contentGrid, const int *displayGrid, int width, int height, int cursorX, int cursorY)
{
    // Create a 2D array of strings
    char **content = (char **)allocateMemory(height * sizeof(char *));
    for (int i = 0; i < height; i++)
    {
        // Allocate memory for each line
        content[i] = (char *)allocateMemory(width * 100 * sizeof(char));
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

            if (displayGrid[i * width + j] == FLAG_CELL)
            {
                // Add a flag
                char *cell = printCell(-2, isCursor);
                position += sprintf(content[i] + position, "%s", cell);
                free(cell);
            }
            else if (displayGrid[i * width + j] == VISIBLE_CELL)
            {
                // Add the content of the cell
                char *cell = printCell(contentGrid[i * width + j], isCursor);
                position += sprintf(content[i] + position, "%s", cell);
                free(cell);
            }
            else
            {
                // Add an empty hidden cell
                // Choose background color depending on isCursor value
                char *c = backgroundColors[isCursor ? 3 : 2];
                position += sprintf(content[i] + position, "%s  %s", c, foregroundColors[0]);
            }
        }
    }
    // Flatten content, to be able to show the entire grid in one call
    char *result = (char *)allocateMemory(height * width * 100 * sizeof(char));
    int position = sprintf(result, "%s", "\e[1;1H"); // Clear screen, hide cursor
    for (int i = 0; i < height; i++)
    {
        // Add each line
        position += sprintf(result + position, "%s", content[i]);
    }
    position += sprintf(result + position, "\n");
    // Finally, show the entire grid in one call
    fwrite(result, position, 1, stdout); // Equivalent to printf, but without format

    // Free memory
    for (int i = 0; i < height; i++)
    {
        free(content[i]);
    }
    free(content);
    free(result);
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
 */
void updateGameGrid(int *contentGrid, const int *displayGrid, int width, int cursorX, int cursorY, int oldCursorX, int oldCursorY)
{
    // Write on console only where changes occurred
    char *content = (char *)allocateMemory(200 * sizeof(char));
    int position = 0;

    // Edit old cursor
    char *cell;
    bool needFree = true;
    if (displayGrid[oldCursorY * width + oldCursorX] == FLAG_CELL)
    {
        // Add a flag
        cell = printCell(-2, 0);
    }
    else if (displayGrid[oldCursorY * width + oldCursorX] == VISIBLE_CELL)
    {
        // Add the content of the cell
        cell = printCell(contentGrid[oldCursorY * width + oldCursorX], 0);
    }
    else
    {
        cell = backgroundColors[2];
        needFree = false; // backgrounds colors doesn't come from malloc
    }
    position += sprintf(content, "\e[%d;%dH%s", oldCursorY + 1, oldCursorX * 2 + 1, cell);
    if (needFree)
        free(cell);
    if (displayGrid[oldCursorY * width + oldCursorX] == HIDDEN_CELL)
    {
        // Add an empty hidden cell
        position += sprintf(content + position, "  %s", foregroundColors[0]);
    }

    // Edit new cursor
    needFree = true;
    if (displayGrid[cursorY * width + cursorX] == FLAG_CELL)
    {
        // Add a flag
        cell = printCell(-2, 1);
    }
    else if (displayGrid[cursorY * width + cursorX] == VISIBLE_CELL)
    {
        // Add the content of the cell
        cell = printCell(contentGrid[cursorY * width + cursorX], 1);
    }
    else
    {
        cell = backgroundColors[3];
        needFree = false; // backgrounds colors doesn't come from malloc
    }
    position += sprintf(content + position, "\e[%d;%dH%s", cursorY + 1, cursorX * 2 + 1, cell);
    if (needFree)
        free(cell);
    if (displayGrid[cursorY * width + cursorX] == HIDDEN_CELL)
    {
        // Add an empty hidden cell
        position += sprintf(content + position, "  %s", foregroundColors[0]);
    }
    // Show changes
    fwrite(content, position, 1, stdout); // Equivalent to printf, but without format
    // Free memory
    free(content);
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
 */
void waitForInput(int *contentGrid, int *displayGrid, int width, int height, int *coordX, int *coordY, char *action)
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
        int input = _getch();
        input = tolower(input);

        // Save old cursor position
        oldX = x;
        oldY = y;

        // Process input
        if (input == COMMAND_RIGHT)
        {
            x++;
            // Don't go outside the grid
            if (x >= width)
                x = width - 1;
        }
        else if (input == COMMAND_LEFT)
        {
            x--;
            // Don't go outside the grid
            if (x < 0)
                x = 0;
        }
        else if (input == COMMAND_UP)
        {
            y--;
            // Don't go outside the grid
            if (y < 0)
                y = 0;
        }
        else if (input == COMMAND_DOWN)
        {
            y++;
            // Don't go outside the grid
            if (y >= height)
                y = height - 1;
        }
        else if (input == 3)
        {
            // If ctrl+c exit
            // Exit directly, process is stopped instantly
            *action = ACTION_QUIT;
            // Exit loop
            flag = 0;
        }
        else if (input == COMMAND_PLACE_FLAG || input == COMMAND_DIG)
        {
            // Pass cursor position in pointers
            *coordX = x;
            *coordY = y;
            // Pass action in pointer, depending on input
            *action = input == COMMAND_PLACE_FLAG ? ACTION_PLACE_FLAG : ACTION_DIG;
            // Exit loop
            flag = 0;
        }
    }
}

/**
 * @brief Display win message
 * @param contentGrid Grid with the content of each cell
 * @param displayGrid Grid with the state of each cell
 * @param width Width of the grid
 * @param height Height of the grid
 */
void displayWin(int *contentGrid, int *displayGrid, int width, int height)
{
    showGameGrid(contentGrid, displayGrid, width, height, -1, -1);
    printf("You won !");
}
/**
 * @brief Display lose message
 * @param contentGrid Grid with the content of each cell
 * @param displayGrid Grid with the state of each cell
 * @param width Width of the grid
 * @param height Height of the grid
 */
void displayLoose(int *contentGrid, int *displayGrid, int width, int height)
{
    showGameGrid(contentGrid, displayGrid, width, height, -1, -1);
    printf("You lose !");
}

#ifdef TEST_DISPLAY
int test()
{
    if (initializeDisplay() < 0)
    {
        fprintf(stderr, "Error: Unable to initialize display\n");
        return -1;
    }

    // Create grids
    int width = 10;
    int height = 10;
    int *contentGrid = (int *)allocateMemory(width * height * sizeof(int *));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            contentGrid[i * width + j] = 0;
        }
    }
    int *displayGrid = (int *)allocateMemory(width * height * sizeof(int *));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            displayGrid[i * width + j] = HIDDEN_CELL;
        }
    }

    // add some content to test all possible numbers
    contentGrid[2 * width + 1] = 1;
    contentGrid[2 * width + 2] = 2;
    contentGrid[2 * width + 3] = 3;
    contentGrid[2 * width + 4] = 4;
    contentGrid[2 * width + 5] = 5;
    contentGrid[2 * width + 6] = 6;
    contentGrid[2 * width + 7] = 7;
    contentGrid[2 * width + 8] = 8;

    // add some bombs
    contentGrid[1 * width + 2] = BOMB;
    contentGrid[1 * width + 3] = BOMB;
    contentGrid[3 * width + 8] = BOMB;
    contentGrid[4 * width + 1] = BOMB;
    contentGrid[8 * width + 7] = BOMB;

    // add some flags
    displayGrid[3 * width + 2] = FLAG_CELL;
    displayGrid[3 * width + 3] = FLAG_CELL;
    displayGrid[3 * width + 8] = FLAG_CELL;
    displayGrid[4 * width + 1] = FLAG_CELL;
    displayGrid[8 * width + 7] = FLAG_CELL;

    // Show some cells
    displayGrid[1 * width + 2] = VISIBLE_CELL;
    displayGrid[1 * width + 3] = VISIBLE_CELL;
    displayGrid[6 * width + 4] = VISIBLE_CELL;
    displayGrid[2 * width + 1] = VISIBLE_CELL;
    displayGrid[2 * width + 2] = VISIBLE_CELL;
    displayGrid[2 * width + 3] = VISIBLE_CELL;
    displayGrid[2 * width + 4] = VISIBLE_CELL;
    displayGrid[2 * width + 5] = VISIBLE_CELL;
    displayGrid[2 * width + 6] = VISIBLE_CELL;
    displayGrid[2 * width + 7] = VISIBLE_CELL;
    displayGrid[2 * width + 8] = VISIBLE_CELL;

    int x, y;
    char action;
    waitForInput(contentGrid, displayGrid, width, height, &x, &y, &action);

    // Free grids
    free(contentGrid);
    free(displayGrid);

    restoreDisplay();
    return 0;
}
int main()
{
    return test();
}
#endif // TEST_DISPLAY
