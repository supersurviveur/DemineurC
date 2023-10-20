#include <malloc.h>
#include <stdio.h>

// Functions to manage a 2D table

int* generateTable(int rows, int columns)
{
    int *table = (int*)malloc(rows * columns * sizeof(int));
    if (table == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < rows * columns; i++)
    {
        table[i] = 0;
    }
    return table;
}

void printTable(int *table, int rows, int columns)
{
    for (int i = 0; i < rows * columns; i++)
    {
        if (i % columns == 0)
        {
            printf("\n");
        }
        printf("%d ", table[i]);
    }
    printf("\n");
}

void freeTable(int *table)
{
    free(table);
}

void editTable(int *table, int rows, int columns, int row, int column, int value)
{
    table[row * columns + column] = value;
}

int getTableValue(int *table, int rows, int columns, int row, int column)
{
    return table[row * columns + column];
}

int main()
{
    // Test editTable and getTableValue functions
    int* table = generateTable(3, 3);
    printTable(table, 3, 3);
    editTable(table, 3, 3, 1, 1, 1);
    printTable(table, 3, 3);
    printf("%d\n", getTableValue(table, 3, 3, 1, 1));
    freeTable(table);
}