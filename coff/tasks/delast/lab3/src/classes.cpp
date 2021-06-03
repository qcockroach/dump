#include <iostream>
#include "classes.h"
using namespace std;


static void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
// Function to perform Selection Sort
static void selectionSort(int *arr, int n)
{
    int i, j, min_idx;
 
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) {
 
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
 
        // Swap the found minimum element
        // with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}

Table::Table (void)
{
    x = 0;
    y = 0;

    /* Fill table with with min values to
        indicate it's ready to use.  */
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            table[i][j] = 0;
        }
    }
}

Table::~Table (void) { }

void Table::add (int val)
{
    if (x == WIDTH) {
        if (y == HEIGHT - 1) {
            cout << "Ошибка: таблица заполнена\n\n" << endl;
            return ;
        }
        y++;
        x = 0;
    } 

    table[y][x++] = val; /* fill table cell.  */

    cout << "Элемент добавлен в массив\n\n";
}

void Table::del (int val)
{
    for (int i = 0; i <= y; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (table[i][j] == val) {
                table[i][j] = 0;
                cout << "Элемент удален из массива\n\n";
                return ;
            }
        }
    }
    cout << "Ошибка: нет значения " << val  << "\n" << endl;
}

void Table::show (void) {
    cout << "Таблица\n";
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            cout << table[i][j] << "\t";
        }
        cout << "\n";
    }
    cout << endl;
}

SortedTable::SortedTable (void) { }

SortedTable::~SortedTable (void) { }

void SortedTable::add (int val)
{
    if (x == WIDTH) {
        if (y == HEIGHT - 1) {
            cout << "Ошибка: Таблица заполнена\n" << endl;
            return ;
        }
        y++;
        x = 0;
    } 

    table[y][x++] = val; /* fill table cell.  */
    cout << "Элемент добавлен в массив\n\n";

    /* sort table's row ONLY if it's full.  */
    if (x != WIDTH) {
        return ;
    }

    selectionSort (table[y], HEIGHT);
}
