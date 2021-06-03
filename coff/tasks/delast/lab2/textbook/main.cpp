// #include <conio.h>
#include <locale.h>
#include "stove.h"


Кондиционер имеет 3 кнопки: 
кнопка режима (холодный/горячий воздух);
кнопка питания; 
кнопка работы.

Необходимые функции (сообщения):
1 нажать кнопку;
2 изменить положение двери;
3 пища готова

int main()
{
    setlocale(0,"RUS");
    stove MyStove;
    MyStove.Run();
    return 0;
}
