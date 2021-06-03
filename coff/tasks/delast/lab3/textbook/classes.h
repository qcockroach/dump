class Ellipse {
protected: //a и b должны быть напрямую доступны для классапотомка
    int a, b;

public:
    Ellipse();
    virtual void EnterParam(); //ввод параметров
    virtual void DisplayParam(); //вывод параметров
    double Area(); //нахождение площади; невиртуальная, так как не переопределяется в классе-потомке
};

class ColorEllipse: public Ellipse {
    char color[40];
public:
    ColorEllipse();
    virtual void EnterParam();
    virtual void DisplayParam();
};

