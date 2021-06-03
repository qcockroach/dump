#include <iostream>
#include <stdio.h>
// #include <conio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include "array.h"
#include "main.h"


int main (void)
{
    CDynamicArray ar; //создаётся объект класса, при этом автоматически вызывается конструктор
    int key,n,k,val;
    srand(time(NULL)); // включаем генератор случайных чисел
    setlocale(0,"RUS"); // обеспечиваем вывод в консольное окно символов кириллицы

    do
    {
        printf("\n\n1 - создание массива\n");
        printf("2 - изменение элементов массива\n");
        printf("3 - заполнение массива случайными числами\n");
        printf("4 - вывод на экран элементов массива\n");
        printf("5 - удаление элементов массива на четных местах\n");
        printf("ESC - выход\n");
        printf("Ваш выбор: \n");

        key = getchar();
        switch(key)
        {
        case '1':
            printf("Введите количество элементов массива: ");
            scanf("%d",&n);
            ar.CreateArray(n);
            break;
        case '2':
            printf("Введите номер элемента массива: ");
            scanf("%d",&k);
            printf("Введите значение элемента массива: ");
            scanf("%d",&val);
            ar.SetValue(k,val);
            break;
        case '3':
            ar.FillArrayRandValues();
            break;
        case '4':
            ar.DisplayArray();
            break;
        case '5':
            ar.CArrayRemoveIfOdd ();
            break;
        }
    } while (key != 27); //код клавиши ESC

    return 0; //объект класса уничтожается, автоматически вызывается деструктор
}
