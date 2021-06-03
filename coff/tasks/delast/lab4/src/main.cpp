#include <iostream>
#include <string.h>
#include <cstdio>
#include <clocale>
#include "deps.h"
using namespace std;


/* Bin2Dec
    binary to (2) decimal - бинарное в деситичное
 */
class Bin2Dec {
    char *binstr; //указатель на строку, которая хранит число
    int count;

    /* convert binary string to decimal values.  */
    int bin2dec(char *n)
    {
        char *num = n;
        int size = strlen (n);
        int decval = 0;
        int sign = 1;
        int base = 1; /* Initializing base value to 1, i.e 2^0  .*/

        if (n[0] == '1') {
            sign = -1;
        }

        /* binary convertion with 2's complement */
        for (int i = size - 1; i > 0; --i) {
            int last_digit = (num[i]) - 48; /* convert ascii to digit.  */
            decval += last_digit * base;
            base = base * 2;
        }

        return sign * decval;
    }

    char *dec2bin (int num)
    {
        int size = 20;
        int arr[size];
        char *str = new char[size];
        int i = 0, k = 0;

        if (num < 0) {
            num = -num;
            str[k++] = '1';
        }
        else {
            str[k++] = '0';
        }

        while (num > 0) {
            arr[i] = num % 2;
            num = num / 2;
            i++;
        }

        for (int j = i - 1; j >= 0; j--) {
            str[k++] = arr[j] + 48;
        }

        return str;
    }

public:

    Bin2Dec(int n = 100) //конструктор
    {
        count = 0;
        binstr = new char[n]; //выделение памяти под строку
    }

    Bin2Dec(const Bin2Dec &c1) //конструктор копирования
    {
        binstr= new char[c1.count+1]; //выделение памяти под строку
        strcpy (binstr,c1.binstr);
        count = c1.count;
    }

    ~Bin2Dec()
    {
        if (binstr) {
            delete binstr;
        }
    }

    friend istream & operator>>(istream &, Bin2Dec &);//дружественная функция для ввода строки
    friend ostream & operator<<(ostream &, Bin2Dec &);//дружественная функция для ввода строки

    int convert (void)
    {
        return bin2dec (binstr);
    }

    Bin2Dec& operator =(const Bin2Dec& c1) //перегруженный оператор присваивания
    {
        if (this!=&c1)
        {
            strcpy(binstr,c1.binstr); //копируем строку из аргумента функции в текущий объект
            count=c1.count;
        }
        return *this;
    }

    void operator -(const Bin2Dec& c1) //перегруженный оператор вычитания
    {
        int d1, d2;
        d1 = bin2dec (binstr);                  // перевели строку из текущего в вещественное число
        d2 = bin2dec (c1.binstr);               // перевели строку из аргумента функции в вещественное число
        d1 -= d2;
        binstr = dec2bin (d1);                  // перевели число обратно в бинарную строку
        count = strlen (binstr);                // получили новое значение count
    }

    void operator -=(const Bin2Dec& c1) //перегруженный оператор вычитания
    {
        int d1, d2;
        d1 = bin2dec (binstr); //перевели строку из текущего в вещественное число
        d2 = bin2dec (c1.binstr); //перевели строку из аргумента функции в вещественное число
        d1 -= d2;
        binstr = dec2bin (d1);                     // перевели число обратно в бинарную строку
        count = strlen (binstr); //получили новое значение count
    }

    void operator --(int a) //перегруженный оператор декремента
    {
        for (int i = count - 1; i >= 0; --i) {
            if (binstr[i] == '1') {
                binstr[i] = '0';
                return ;
            }
        }

        /* flip all bits.  */
        for (int i = 0; i < count; ++i) {
            binstr[i] = '1';
        }
    }

    operator int (void)
    {
        return bin2dec(binstr);
    }
};


istream & operator >>(istream & input, Bin2Dec &c1) //определение дружественной функции ввода строки
{
    char key;
    cout << "Введите бинарноe число (2's comp): ";

    do {
        key = getch(); //считываем символ с клавиатуры без вывода на акран
        if (key == '0' || key == '1')
        //проверяем, является ли перевый введенный символ цифрой или минусом
        {
            c1.binstr[c1.count++]=key; //сохраняем введенный символ в строке и увеличиваем счетчик числа введенных символов на 1
            putch(key); //выводим символ на экран
        }
    } while (key != KB_ENTER);

    c1.binstr[c1.count]='\x0';
    cout << "\n";
    return input;
}

ostream & operator <<(ostream & output, Bin2Dec &c1)
{
    int i = 0;

    cout << "Ваше число (bin): ";
    while (i<c1.count) {
        putch (c1.binstr[i++]);
    }

    return output;
}

int main (void)
{
    int dec;
    Bin2Dec bin1, bin2;

    /* ввети и вывести 1-ое число.  */
    cin >> bin1;
    cout << bin1 << endl;
    cout << "Ваше число (dec): " << bin1.convert() << endl << endl;

    /* вычесть из 2-го 1-ое число.  */
    cout << "Декремент первого числа\n";
    bin1--;
    cout << bin1 << endl;
    cout << "Ваше число (dec): " << bin1.convert() << endl << endl;
    cout << endl << endl;


    /* ввети и вывести 3-е число.  */
    cin >> bin2;
    cout << bin2 << endl;
    cout << "Ваше число (dec): " << bin2.convert() << endl << endl;


    /* вычесть из 1-го 2-ое число.  */
    cout << "Вычитаем 1-ое число из 2-его\n";
    bin2 -= bin1;
    cout << bin2 << endl;
    cout << "Ваше число (dec): " << bin2.convert() << endl << endl;


    /* превратить бинарные строки в целое число.  */
    cout << "Превращаем строку в деситичное число\n";
    dec = int (bin1);
    cout << "Ваше первое число (dec): " << dec << endl;

    return 0;
}
