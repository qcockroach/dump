#include <iostream>
#include <cstdio>
#include "aircon.h"
#include "deps.h"
using namespace std;


#if defined (__WIN32)
# define SLEEP(sec) Sleep ((sec) * 100) // потому что Windows Sleep примимает милисекунды

#else
# define SLEEP(sec) sleep ((sec))       // a Unix-like секунды
#endif

#define TM_PERIOD 60    /* промежуток работы кондиционера 60 сек */
#define TM_OFF    -1    /* счётчик сброшен */


/* Кондиционер имеет 3 кнопки:
1) кнопка режима (холодный/горячий воздух);
2) кнопка питания;
3) кнопка работы.
*/

void menu (void) //выводит на экран меню
{
    cout << "b - кнопка работы\n"; // запускает кондиционер на 1 минуту повторное нажатие добавляет еще минуту, если включен
    cout << "p - вкл/выкл кондиционер\n";       // питание
    cout << "m - холодный/горячий воздух\n";
    cout << "ESC - выход\n\n";
}

Aircon::Aircon()
{
    // начальное состояние:
    curr_button = off;  // кнопка не нажата
    curr_mode = m_cold; // кондиционер в холодном режиме
    curr_power = off;   // питание выключено
    time = TM_OFF;          // кондиционер не работет
}

void Aircon::turn_power(void)
{
    if (curr_power == off) {
        curr_power = on;
        cout << "Кондиционер включён\n";
    }
    else {
    /* если есть питание, то сбрасываем все и выкл кондиционер.  */
        curr_power = off;
        curr_button = off;
        time = TM_OFF;
        cout << "Кондиционер выключен\n";
    }
    cout << "\n";
}

void Aircon::change_mode(void)
{
    if (curr_power == on) {
        cout << "Выключи питание чтобы поменять режим!\n\n";
        return ;
    }

    if (curr_mode == m_cold) {
        curr_mode = m_hot;
        cout << "Режим изменён на горячий\n";
    }
    else if (curr_mode == m_hot) {
        curr_mode = m_cold;
        cout << "Режим изменён на холодный\n";
    }
    cout << "\n";
}

void Aircon::push_button(void)
{
    if (curr_power == off) {
        cout << "Включи питание перед началом работы!\n\n";
        return ;
    }

    if (curr_button == off) {
        curr_button = on;
        time = TM_PERIOD;
        cout << "Кондиционер работает\n";
    }
    else {
        time += TM_PERIOD;
        cout << "Вам добавлена минута\n";
    }
}

void Aircon::deactivate(void)
{
    curr_power = off;   // питание выключено
    curr_button = off;  // лампа выключена
    time = -1;          // сбрасываем счётчик времени
    cout << "Кондиционер выключен\n\n";
}

void Aircon::Run(void)
{
    int key;

    menu();

    do {
        while (!kbhit() && time > 0) //в этот цикл заходим, если еда готовится; условие выхода из цикла: нажата клавиша или истекло время приготовления еды
        //функция kbhit() возвращает истину, если пользователь нажал клавишу
        {
            time--;   // вычитаем секунду из времени, оставшегося до конца приготовления еды
            SLEEP(1); // задержка в 100 мс
            cout <<  "Осталось " << time << " секунд до выключения\n";
        }
        if (time==0) { // если кондиционер закончила работу, вызываем метод deactivate()
            deactivate();
        }

        key = getch();

        switch (key) {
            case 'b':
                push_button();
                break;
            case 'p':
                turn_power();
                break;
            case 'm':
                change_mode();
                break;
        }
        fflush(stdin);
    } while(key != KB_ESCAPE);
}
