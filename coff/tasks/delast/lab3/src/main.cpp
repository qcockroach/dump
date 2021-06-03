#include <iostream>
#include <climits>
#include <cstdio>
#include "classes.h"
#include "deps.h"
using namespace std;


/*
 Создать базовый класс «таблица» из N*N полей, определив в нем
1) функции добавления элемента в таблицу, 
2) удаление элемента из таблицы по значению, 
3) вывод таблицы на экран. 

Породить от него класс «упорядоченная
таблица», переопределив функцию добавления элементов.
*/

static void menu1 (void)
{
    cout << "1 - обычная таблица\n";
    cout << "2 - сортированная таблица\n";
    cout << "ESC - выход\n\n";
}

static void menu2 (void)
{
    cout << "1 - добавить элемент\n";
    cout << "2 - удалить элемент\n";
    cout << "3 - вывод на экран\n\n";
}

static int getnum (void)
{
    int idx;
    int c;
    int i = 0;
    char buff[20];

    while ((c = getch ()) && c != '\n') {
        if (c >= '0' && c <= '9') {
            buff[i++] = c;
            putch (c);
        }
    }
    buff[i] = 0;

    putch ('\n');
    idx = atoi (buff);
    return idx;
}


int main (void)
{
    Table otable;        /* ordinary table.  */
    SortedTable stable;  /* sorted table.  */
    Table *table; //указатель нужен для реализации полиморфизма
    int key1, key2;
    int val; /* where insert value to.  */

    setlocale(0, "RUS"); // обеспесиваем вывод в консольное окно символов кириллицы

    menu1 ();
    do {
        key1 = getch();

        switch(key1) {
        case '1':
            table=&otable;
            break;
        case '2':
            table=&stable;
            break;
        }
        if (key1 != KB_ESCAPE) {//выбираем действие для выбранного типа таблицы
            menu2 ();
            do {
                key2 = getch();


                switch (key2) {
                case '1':
                    cout << "Введите число (add): ";
                    val = getnum ();
                    table->add(val);
                    break;
                case '2':
                    cout << "Введите число (del): ";
                    val = getnum ();
                    table->del(val);
                    break;
                case '3':
                    table->show();
                    break;
                }
            } while (key2 != KB_ESCAPE);
        }
        cout << "Выход из таблицы\n\n";
        menu1 ();
    } while (key1 != KB_ESCAPE);

    return 0;
}
