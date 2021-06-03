#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deps.h"
using namespace std;



class CDoubleString {
    char *DoubleStr;//указатель на строку, которая хранит число
    int count;

public:
    CDoubleString(int n=100) //конструктор
    {
        count=0;
        DoubleStr=new char[n]; //выделение памяти под строку
    }

    CDoubleString(const CDoubleString& c1) //конструктор копирования
    {
        DoubleStr=new char[c1.count+1]; //выделение памяти под строку
        strcpy(DoubleStr,c1.DoubleStr);
        count=c1.count;
    }

    ~CDoubleString()
    {
        if (DoubleStr) delete DoubleStr;
    }

    friend istream & operator>>(istream &, CDoubleString &);//дружественная функция для ввода строки
    friend ostream & operator<<(ostream &, CDoubleString &);//дружественная функция для ввода строки

    void operator *(const CDoubleString& c1) //перегруженный оператор умножить
    {
        double d1,d2;
        d1=atof(DoubleStr); //перевели строку из текущего в вещественное число
        d2=atof(c1.DoubleStr); //перевели строку из аргумента функции в вещественное число
        d1*=d2;
        sprintf(DoubleStr,"%f",d1);//перевели число в строку текущего объекта
        count=strlen(DoubleStr)+1; //получили новое значение count
    }

    void operator *=(const CDoubleString& c1)//перегруженный оператор *=
    {
        double d1,d2;
        d1=atof(DoubleStr);
        d2=atof(c1.DoubleStr);
        d1*=d2;
        sprintf(DoubleStr,"%f",d1);
        count=strlen(DoubleStr)+1;
    }

    CDoubleString& operator =(const CDoubleString& c1) //перегруженный оператор присваивания
    {
        if (this!=&c1)
        {
            strcpy(DoubleStr,c1.DoubleStr); //копируем строку из аргумента функции в текущий объект
            count=c1.count;
        }
        return *this;
    }

    operator double()
    {
        return atof(DoubleStr);
    }
};


istream & operator>>(istream & input, CDoubleString &c1) //определение дружественной функции ввода строки
{
    char key;
    int flag=0; //пользователь не нажал еще десятичную точку
    printf("Enter the number: ");

    do {
        // printf (">> \n");
        key = getch(); //считываем символ с клавиатуры без вывода на акран
        if (c1.count==0 && (key=='-' || (key>='0' && key<='9')))
        //проверяем, является ли перевый введенный символ цифрой или минусом
        {
            c1.DoubleStr[c1.count++]=key; //сохраняем введенный символ в строке и увеличиваем счетчик числа введенных символов на 1
            putch(key); //выводим символ на экран
        }
        else
            if (flag==0 && key=='.') //проверяем, первый ли раз нажал пользолватель десятичную точку
            {
                flag=1;//устанавливаем признак введенной десятичной точки
                c1.DoubleStr[c1.count++]=key;
                putch(key);//выводим символ на экран
            }
            else
                if (key>='0' && key<='9') //нажата ли цифра
                {
                    c1.DoubleStr[c1.count++]=key;
                    putch(key);//выводим символ на экран
                }
    } while (c1.count == 0 || key != TERMINATOR);
    //} while (c1.count==0 || key!=13 || key != 10); //цикл работает, пока пользователь не нажмет ENTER

    c1.DoubleStr[c1.count++]='\x0';
    printf("\n");
    return input;
}

ostream & operator<<(ostream & output, CDoubleString &c1)
{
    int i;
    i=0;
    cout<<"Number: ";
    while(i<c1.count)
    {
        putch(c1.DoubleStr[i++]);
    }

    printf("\n");
    return output;
}

int func (void)
{
   int ch;
   printf("Press x to exit.\n\n");
   for (;;) {
      ch = getch();
      printf("ch = %c (%d)\n", ch, ch);
      if(ch == 'x')
         break;
   }
    return 0;
}


int driver(void)
{
    CDoubleString d; //создали объект
    cin>>d; //ввели строку

    printf ("----<\n");
    cout<<d; //вывели строку
    printf ("---->\n\n");

    CDoubleString d1;//создали второй объект
    d1=d; //скоировали данные из первого объекта во второй

    printf ("----<\n");
    cout<<d1; //вывели второй
    printf ("---->\n\n");

    d1*d; //умножили второй на первый, результат во втором

    printf ("----<\n");
    cout<<d1; //вывели второй
    printf ("---->\n");
    printf ("--------------------\n\n\n");




    CDoubleString d2; //создали третий объект
    cin>>d2; //ввели данные в третий объект
    
    printf ("----<\n");
    cout<<d2; //выведи третий объект
    printf ("---->\n\n");

    d2*=d; //умножили третий на первый, результат в третьем

    printf ("----<\n");
    cout<<d2; //вывели третий
    printf ("---->\n\n");

    double x;
    x=double(d);
    cout<<x << endl;
    return 0;
}

int main (void)
{

    driver ();

    return 0;
}

