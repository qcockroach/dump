#ifndef __LAB1_H
#define __LAB1_H

// объявление класса
class CDynamicArray {

    //объявление приватных членов класса
    //поля класса:
    int nCount; //реальная длина массива; если массив пуст – nCount=0
    int *arr; // указатель на массив

public: //объеялание общедоступных членов класса
    // методы класса::
    CDynamicArray(); //конструктор; вызвается, когда создаётся объект класса
    ~CDynamicArray();//деструктор; вызвается, когда уничтожается объект класса

    void CreateArray(int N); //создание массива
    void SetValue(int k, int value); //установка элемента массива
    void FillArrayRandValues(); //заполнение массива случайными числами
    void DisplayArray(); // вывод элементов массива на экран

    void CArrayRemoveIfOdd (void); // удалить четные элементы массива
};

#endif
