#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "Plugin.h"
#include <string>
#include <vector>

class Calculator {
public:
    Calculator();
    double calculate(const std::string& expression);
    std::vector<std::string> getAvailableFunctions() const;

private:
    Plugin plugin;
    std::string expr;
    size_t pos;

    char peekNextChar();
    char readNextChar();
    void skipSpaces();
    double parseAdditionSubtraction();
    double parseMultiplicationDivision();
    double parsePrimary();
    double parseFunctionCall();
    double parseNumericValue();
    bool isValidNumberFormat(const std::string& numberStr) const;

    static bool checkParenthesesBalance(const std::string& expression);
};

#endif