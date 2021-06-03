#include "classes.h"
#include <stdio.h>
#include <string.h>

#define M_PI 3.14159265358979323846

Ellipse::Ellipse()
{
    a=1;
    b=1;
}

void Ellipse::EnterParam()
{
    int er;

    do {
        printf("Введите длину первой полуоси: ");
        er=scanf("%d",&a);

        if (a <= 0 || er == 0) {
            printf("Ошибка!!! Повторите ввод! \n");
        }
        fflush(stdin);
    } while(a <= 0 || er == 0);

    do {
        printf("Введите длину второй полуоси: ");
        er=scanf("%d",&b);

        if(b <= 0 || er == 0) {
            printf("Ошибка!!! Повторите ввод! \n");
        }
        fflush(stdin);
    } while(b<=0 || er==0);
}

void Ellipse::DisplayParam()
{
    printf("Длины полуосей: a=%d b=%d \n", a,b);
}

double Ellipse::Area()
{
    return M_PI*a*b;
}

ColorEllipse::ColorEllipse()
{
    strcpy(color,"Красный");
}

void ColorEllipse::EnterParam()
{
    Ellipse::EnterParam();
    printf("Введите цвет эллипса: ");
    scanf("%s",color);
}

void ColorEllipse::DisplayParam()
{
    Ellipse::DisplayParam();
    printf("Цвет эллипса: %s\n",color);
}

