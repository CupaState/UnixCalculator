﻿#include "LinuxCalculator.h"

#include <regex>
#include <sstream>
#include <string>
#include <map>
#include <stack>

Calculator::Calculator(std::string e_string) : entry_string(e_string)
{

}

Calculator::~Calculator()
{

}

double Calculator::ReturnResult()
{
    return this->Calculate();
}

bool Calculator::IsInvalidChars()
{
    if (std::regex_search(entry_string,
        std::regex("[a-zA-Z_\\$&\\^#№\\@\\!\\{\\}\\[\\]\\?`;:=]", std::regex::ECMAScript)))
    {
        std::cout << "Unexpected chars\n";
        return true;
    }
    return false;
}

void Calculator::ChangeCommaToDot()
{
    for (unsigned int i = 0; i < entry_string.length(); i++)
    {
        if (entry_string[i] == ',')
        {
            entry_string[i] = '.';
        }
    }
}

void Calculator::RPN()
{
    for (int i = 0; i < entry_string.size(); ++i) // Accounting for negative numbers
    {
        if ((entry_string[i] == '+' || entry_string[i] == '-') && (0 == i || (!isalnum(entry_string[i - 1]) && entry_string[i - 1] != '.' && entry_string[i - 1] != ')')))
        {
            auto iterator = std::find_if(entry_string.begin() + i + 1, entry_string.end(), [](char const character) {return !isalnum(character); });
            entry_string.insert(iterator, ')');
            entry_string.insert(i, "(0");
        }
    }

    std::map<char, size_t> map; // priorities
    map.insert(std::make_pair('*', 3));
    map.insert(std::make_pair('/', 3));
    map.insert(std::make_pair('+', 2));
    map.insert(std::make_pair('-', 2));
    map.insert(std::make_pair('(', 1));

    std::stack<char> stack;
    srpn += ' ';
    for (auto ch : entry_string) // forming result string in RPN
    {
        if (!isalnum(ch) && (ch != '.'))
        {
            srpn += ' ';
            if (ch == ')')
            {
                while (stack.top() != '(')
                {
                    srpn += stack.top();
                    stack.pop();
                    srpn += ' ';
                }
                stack.pop();
            }
            else if (ch == '(')
            {
                stack.push(ch);
            }
            else if (stack.empty() || (map[stack.top()] < map[ch]))
            {
                stack.push(ch);
            }
            else
            {
                do
                {
                    srpn += stack.top();
                    srpn += ' ';
                    stack.pop();
                } while (!(stack.empty() || (map[stack.top()] < map[ch])));
                stack.push(ch);
            }
        }
        else
        {
            srpn += ch;
        }
    }

    while (!stack.empty())// remain items from stack to result string
    {
        srpn += stack.top();
        srpn += ' ';
        stack.pop();
    }
}

bool Calculator::CheckRemainStringForNumbers(std::string str, int iter)
{
    for (int i = iter; i < str.length(); i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            return true;
        }
    }
    return false;
}

double Calculator::Calculate()
{
    if (!this->IsInvalidChars())
    {
        this->ChangeCommaToDot();
        this->RPN();
        std::stack<double> temp;
        std::string double_num;
        double first_operand = 0.0;
        double second_operand = 0.0;

        for (unsigned int i = 0; i < srpn.length(); i++)
        {
            if (i == 0)
            {
                continue;
            }

            if (isdigit(srpn[i]) && (srpn[i + 1] == ' ' || srpn[i + 1] == '-' || srpn[i + 1] == '/' || srpn[i + 1] == '*' || srpn[i + 1] == '+') && srpn[i + 1] != '.' && srpn[i - 1] != '.' && !isdigit(srpn[i - 1]))
            {
                temp.push(srpn[i] - 48.0);
            }
            else if ((isdigit(srpn[i]) && srpn[i + 1] == '.') || (isdigit(srpn[i]) && srpn[i - 1] == '.') ||
                (srpn[i] == '.' && isdigit(srpn[i + 1])) || (isdigit(srpn[i]) && isdigit(srpn[i + 1])) ||
                (isdigit(srpn[i]) && isdigit(srpn[i - 1]))
                )
            {
                double_num.push_back(srpn[i]);
            }

            if (srpn[i] == ' ')
            {
                std::stringstream ss(double_num);
                double t = 0.0;
                ss >> t;
                if (t != 0.0)
                    temp.push(t);
                double_num = "";
            }

            if (!CheckRemainStringForNumbers(srpn, i))
            {
                double_num = "";
            }

            //Processing operation 
            if (srpn[i] == '+' || srpn[i] == '-' || srpn[i] == '*' || srpn[i] == '/')
            {
                second_operand = temp.top();
                temp.pop();
                first_operand = temp.top();
                temp.pop();

                if (srpn[i] == '+')
                {
                    first_operand += second_operand;
                }
                else if (srpn[i] == '-')
                {
                    first_operand -= second_operand;
                }
                else if (srpn[i] == '*')
                {
                    first_operand *= second_operand;
                }
                else if (srpn[i] == '/')
                {
                    first_operand /= second_operand;
                }
                temp.push(first_operand);
            }
        }
        return temp.top();
    }
    else
    {
        return 0.0;
    }
}