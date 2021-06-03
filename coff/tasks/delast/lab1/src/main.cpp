#include <iostream>
#include <locale.h>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include "array.h"
#include "deps.h"
using namespace std;

void menu (void)
{
    cout << "1 - создание массива\n";
    cout << "2 - изменение элементов массива\n";
    cout << "3 - заполнение массива случайными числами\n";
    cout << "4 - вывод на экран элементов массива\n";
    cout << "5 - удаление элементов массива с нечетным индексом\n";
    cout << "ESC - выход\n\n";
}

int main (void)
{
    int key, n, k, val;
    CArrayRemoveIfOdd ar;   // создаётся объект класса, при этом автоматически вызывается конструктор

    srand(time(NULL));      // включаем генератор случайных чисел
    setlocale(0, "RUS");    // обеспечиваем вывод в консольное окно символов кириллицы

    menu ();
    do {
        key = getch();
        switch(key)
        {
        case '1':
            cout << "Введите количество элементов массива: ";
            scanf("%d", &n);
            ar.CreateArray(n);
            break;
        case '2':
            cout << "Введите номер элемента массива: ";
            scanf("%d", &k);
            cout << "Введите значение элемента массива: ";
            scanf("%d",&val);
            ar.SetValue(k, val);
            break;
        case '3':
            cout << "Массив заполнен случайными числами\n";
            ar.FillArrayRandValues();
            break;
        case '4':
            cout << "Вывод на экран элементов массива\n";
            ar.DisplayArray();
            break;
        case '5':
            cout << "Элементов массива с нечетным индексом удалены\n\n";
            ar.RemoveIfOdd ();
            break;
        }
    } while (key != KB_ESCAPE); // код клавиши ESC

    return 0; // объект класса уничтожается, автоматически вызывается деструктор
}
