#include <stdlib.h>
#include <cstdio>
#include "array.h"

CArrayRemoveIfOdd::CArrayRemoveIfOdd() //вызов конструктора
{
    nCount=0; //начальные значения полей класса: массив пуст
    arr=NULL;
}

CArrayRemoveIfOdd::~CArrayRemoveIfOdd()
{
    delete [] arr; //освобождение памяти, занимаемой массивом
}

void CArrayRemoveIfOdd::CreateArray(int N)
{
    if (N>0) { //если задан корректно новый размер массива – создаём его
        if (arr!=NULL) //если массив уже существовал – уничтожаем его
        delete [] arr;
        arr=new int[N];
        nCount=N;
        printf("Массив создан\n\n");
    }
    else {
        printf("Ошибка! Размер массива должен быть больше 0\n\n");
    }
}

void CArrayRemoveIfOdd::SetValue(int k, int value)
{
    if (k >= 0 && k < nCount) { //если элемент с индексом k существует, меняем его
        arr[k] = value;
    }
    else {
        printf("Ошибка! Задан неверный индекс массива\n\n");
        return ;
    }
    printf ("Индекс %d массива изменен\n\n", k);
}

void CArrayRemoveIfOdd::FillArrayRandValues()
{
    if(nCount>0) { //если массив создан
        for(int i=0; i<nCount; i++) {
            arr[i]=rand()%100;
        }
    }
    else {
        printf("Ошибка! Массив не создан\n");
    }
    printf("\n");
}

void CArrayRemoveIfOdd::DisplayArray()
{
    if(nCount > 0) { //если массив создан
        for(int i = 0; i < nCount; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
    else {
        printf("Массив не создан\n");
    }
    printf("\n");
}

void CArrayRemoveIfOdd::RemoveIfOdd (void)
{
    int i = 0;
    int j = 0;
    int *tmparr, tmpsize;

    tmpsize = nCount / 2 + 1;
    tmparr = new int[tmpsize];

    for ( ; i < nCount; ++i) {
        if (i % 2 == 1) {
            /* copy odd index values into temp array. */
            tmparr[j] = arr[i];
            j++;
        }
    }

    /* Delete previously allocated memory for array.  */
    delete [] arr;

    /* Assign pointer to newly created array.  */
    arr = tmparr;

    nCount = j;
}
