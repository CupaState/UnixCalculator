#pragma once

#include <iostream>
//

class Calculator
{
public:

    Calculator(std::string e_string);

    ~Calculator();

    double ReturnResult();

private:

    bool IsInvalidChars();

    void ChangeCommaToDot();

    // Reverse Polish Notation
    void RPN();

    bool CheckRemainStringForNumbers(std::string str, int iter);

    double Calculate();

    std::string entry_string;
    std::string srpn;
};