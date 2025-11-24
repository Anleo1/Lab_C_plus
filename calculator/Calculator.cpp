#include "Calculator.h"
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <cctype>

Calculator::Calculator() : pos(0) {
    plugin.loadPluginsFromDirectory("./plugins");
}

double Calculator::calculate(const std::string& expression) {
    if (expression.empty()) {
        throw std::runtime_error("Empty expression");
    }

    if (!checkParenthesesBalance(expression)) {
        throw std::runtime_error("Unbalanced parentheses");
    }

    expr = expression;
    pos = 0;

    try {
        double result = parseAdditionSubtraction();

        skipSpaces();
        if (pos < expr.length()) {
            throw std::runtime_error("Unexpected characters at end of expression");
        }
        if (std::isinf(result)) {
            throw std::runtime_error("Calculation result is infinite");
        }
        if (std::isnan(result)) {
            throw std::runtime_error("Calculation result is undefined");
        }

        return result;
    }
    catch (const std::exception& e) {
        std::string errorMsg = e.what();
        throw std::runtime_error(errorMsg);
    }
}

bool Calculator::checkParenthesesBalance(const std::string& expression) {
    int balance = 0;
    for (char c : expression) {
        if (c == '(') {
            balance++;
        }
        else if (c == ')') {
            if (--balance < 0) return false;
        }
    }
    return balance == 0;
}

std::vector<std::string> Calculator::getAvailableFunctions() const {
    return plugin.getAvailableFunctions();
}

void Calculator::skipSpaces() {
    while (pos < expr.length() && std::isspace(expr[pos])) {
        pos++;
    }
}

char Calculator::peekNextChar() {
    skipSpaces();
    return (pos < expr.length()) ? expr[pos] : '\0';
}

char Calculator::readNextChar() {
    skipSpaces();
    return (pos < expr.length()) ? expr[pos++] : '\0';
}

double Calculator::parseAdditionSubtraction() {
    double result = parseMultiplicationDivision();

    while (true) {
        char op = peekNextChar();
        if (op == '+') {
            readNextChar();
            result += parseMultiplicationDivision();
        }
        else if (op == '-') {
            readNextChar();
            result -= parseMultiplicationDivision();
        }
        else {
            break;
        }
    }
    return result;
}

double Calculator::parseMultiplicationDivision() {
    double result = parsePrimary();

    while (true) {
        char op = peekNextChar();
        if (op == '*') {
            readNextChar();
            result *= parsePrimary();
        }
        else if (op == '/') {
            readNextChar();
            double divisor = parsePrimary();
            if (divisor == 0.0) throw std::runtime_error("Division by zero");
            result /= divisor;
        }
        else if (op == '^') {
            readNextChar();
            double exponent = parsePrimary();
            if (result == 0.0 && exponent < 0) throw std::runtime_error("Zero to negative power");
            if (result < 0 && std::fmod(exponent, 1.0) != 0.0) throw std::runtime_error("Negative to fractional power");
            result = std::pow(result, exponent);
        }
        else break;
    }

    return result;
}

double Calculator::parsePrimary() {
    char nextChar = peekNextChar();

    if (nextChar == '(') {
        readNextChar();
        double result = parseAdditionSubtraction();
        if (readNextChar() != ')') throw std::runtime_error("Expected ')'");
        return result;
    }
    else if (std::isalpha(nextChar)) {
        return parseFunctionCall();
    }
    else {
        return parseNumericValue();
    }
}

double Calculator::parseFunctionCall() {
    std::string functionName;

    while (std::isalpha(peekNextChar())) {
        functionName += readNextChar();
    }

    if (readNextChar() != '(') throw std::runtime_error("Expected '(' after function name");

    double argument = parseAdditionSubtraction();

    if (readNextChar() != ')') throw std::runtime_error("Expected ')' for function call");

    return plugin.executeFunction(functionName, argument);
}

bool Calculator::isValidNumberFormat(const std::string& numberStr) const {
    if (numberStr.empty()) return false;
    if (numberStr == "." || numberStr == "-.") return false;

    int dotCount = 0;
    for (size_t i = 0; i < numberStr.length(); ++i) {
        char c = numberStr[i];
        if (c == '.') {
            dotCount++;
            if (dotCount > 1) return false;
        }
        else if (c == '-') {
            if (i != 0) return false;
        }
        else if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

double Calculator::parseNumericValue() {
    std::string numberStr;
    bool hasDecimal = false;

    if (peekNextChar() == '-') {
        numberStr += readNextChar();
    }

    while (std::isdigit(peekNextChar()) || (!hasDecimal && peekNextChar() == '.')) {
        char c = readNextChar();
        if (c == '.') {
            if (hasDecimal) throw std::runtime_error("Multiple decimal points");
            hasDecimal = true;
        }
        numberStr += c;
    }

    if (!isValidNumberFormat(numberStr)) {
        throw std::runtime_error("Invalid number format: '" + numberStr + "'");
    }

    try {
        double result = std::stod(numberStr);
        return result;
    }
    catch (const std::out_of_range&) {
        throw std::runtime_error("Number out of range: '" + numberStr + "'");
    }
    catch (const std::invalid_argument&) {
        throw std::runtime_error("Invalid number format: '" + numberStr + "'");
    }
}