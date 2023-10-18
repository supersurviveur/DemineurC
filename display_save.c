#define WINDOWS // To remove if we are not on windows
#define UNICODE_DISPLAY // To remove if we use cmd or powershell

#ifdef WINDOWS
#include <windows.h>
#endif

#include <stdio.h>
#include <fcntl.h>

#define _showElement(x) printf(x)
#ifdef UNICODE_DISPLAY
#define SHOW_FLAG() _showElement("\u2691")
#else
#define SHOW_FLAG() _showElement("D")
#endif

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

#ifdef UNICODE_DISPLAY
#ifdef WINDOWS
void print_unicode(unsigned short *unicode_code)
{
    _setmode(_fileno(stdout), _O_U16TEXT);

    wprintf(unicode_code);

    _setmode(_fileno(stdout), 0x4000);
}
#else
void print_unicode(unsigned short *unicode_code)
{
    printf("%ls", unicode_code);
}
#endif
#endif

void print_cell(int type)
{
    if (type == 1)
    {
        SHOW_FLAG();
    }
}

int main()
{
    initializeDisplay();
    print_cell(1);
    printf(" \u2691");
    return 0;
}