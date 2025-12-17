#pragma once
#ifndef TESTFUNCTIONS_HPP
#define TESTFUNCTIONS_HPP

#include "Engine.hpp"
#include "Wrapper.hpp"
#include <stdexcept>
#include <cassert>
#include <cmath>

class TestClass {
public:
    int f3(int arg1, int arg2) {
        return arg1 + arg2;
    }

    std::string repeat(std::string s, int n) {
        std::string result;
        for (int i = 0; i < n; ++i) result += s;
        return result;
    }

    int getSquare(int x) {
        return x * x;
    }

    double average(double a, double b) {
        return (a + b) / 2.0;
    }

    double multiply(double a, double b) {
        return a * b;
    }

    bool yes_no(bool flag) { return flag; }

    void print_greeting(std::string name, int times) {
        for (int i = 0; i < times; ++i) {
            std::cout << "Hello, " << name << "!" << std::endl;
        }
    }
    double multiply5(double a, double b, double c, double d, double e) {
        return a * b * c * d * e;
    }

   
};

void runAssignmentTest();
void runAllTypeTests();
void testDefaultArguments();

#endif