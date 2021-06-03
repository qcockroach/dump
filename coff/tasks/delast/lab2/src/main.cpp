#include <clocale>
#include "aircon.h"
#include "deps.h"



int main()
{
    Aircon Myaircon;

    setlocale(0, "RUS");
    Myaircon.Run();

    return 0;
}
