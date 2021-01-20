#include "LinuxCalculator.h"

#include <iomanip>
//1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 15)
//-1 + 5 - 3
//-10 + (8 * 2.5) - (3 / 1.5)

int main(int argc, char* argv[])
{
    std::string bash_args;
    for (int i = 1; i < argc; i++)
    {
        bash_args += argv[i];
    }

    for (int i = 0; i < bash_args.size(); i++)
    {
        if (bash_args[i] == ' ')
        {
            bash_args.erase(i, 1);
            i--;
        }
    }

    Calculator calc(bash_args);

    std::cout << std::fixed << std::setprecision(2) << calc.ReturnResult() << std::endl;

    return 0;
}