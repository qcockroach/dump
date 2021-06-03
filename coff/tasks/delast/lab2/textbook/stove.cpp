#include <stdio.h>
#include <ncurses.h>
#include "stove.h"

#if defined (__WIN32)
# define SLEEP(sec) Sleep ((sec))

#else
# define SLEEP(sec) sleep ((sec))
#endif


void menu (void) //выводит на экран меню
{
    printf("\nb - нажать кнопку для приготовления еды\n");
    printf("d - открыть/закрыть дверь\n");
#ifdef __WIN32
    printf("ESC - выход\n\n");
#else
    printf("c - выход\n\n");
#endif
}


stove::stove()
{
    //начальноге состояние:
    curr_lamp=off; //лампа выключена
    curr_door=d_close; //дверь закрыта
    curr_power=off; //питание выключено
    time=-1; //еда не готовится
}

void stove::take_door(void)
{
    if (curr_door==d_open) {//если дверь открыта
        curr_door=d_close; //закрываем дверь
        curr_lamp=off; //выключаем лампу
        printf("Дверь закрыта\n");
    }
    else {//если дверь закрыта
        curr_door=d_open; //открываем дверь
        curr_lamp=on; //включаем лампу
        printf("Дверь открыта\n");

        if (curr_power==on) { //если в момент открытия двери готовилась еда
            printf("Процесс прерван\n");
            time=-1; //сбрасываем счётчик времени
            curr_power=off; //выключаем питание
        }
    }
    return ;
}

void stove::push_button(void)
{
    if (curr_door==d_open) {//если дверь открыта, еду готовить нельзя
        printf("Закройте дверь!\n");
    }
    else {//если дверь закрыта
        if (curr_power==on) {//если еда уже готовится
            time+=60; //добавляем минуту
            printf("Вам добавлена минута\n");
        }
        else {//если еда не готовится
            curr_power=on; //включаем питание
            curr_lamp=on; //включаем лампу
            time=60; //устанавливаем счётчик времени
            printf("Еда готовится\n");
        }
    }
    return ;
}

void stove::ready(void)
{
    //еда пригтовлена
    curr_power=off; //выключаем питание
    curr_lamp=off; //выключаем лампу
    time=-1; //сбрасываем счётчик времени
    printf("\aЕда готова\n");
}

void stove::Run(void)
{
    int key;

    do {
        while (!kbhit() && time>0) //в этот цикл заходим, если еда готовится; условие выхода из цикла: нажата клавиша или истекло время приготовления еды
        //функция kbhit() возвращает истину, если пользователь нажал клавишу
        {
            time--; //вычитаем секунду из времени, оставшегося до конца приготовления еды
            Sleep(100); //задержка в 100 мс
            printf("Осталось %d секунд\n",time);
        }
        if (time==0) { //если еда закончила готовиться, вызываем метод ready()
            ready();
        }

        menu();
        key = getch();

        switch (key) {
            case 'b':
                push_button();
                break;
            case 'd':
                take_door();
                break;
        }
        fflush(stdin);
    } while(key != ESCAPE);

    return ;
}
