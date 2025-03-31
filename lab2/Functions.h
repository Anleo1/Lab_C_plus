#pragma once
#include <iostream>
#include <fstream>
#include <locale.h>
#include <vector>
#include <string>

struct QuadraticEquation {
    double a, b, c;

    QuadraticEquation(double a = 0, double b = 0, double c = 0) : a(a), b(b), c(c) {}

    std::string toString() const {
        return std::to_string(a) + "x^2 + " + std::to_string(b) + "x + " + std::to_string(c) + " = 0";
    }

    void getCoefficients(double k[3]) const {
        k[0] = a;
        k[1] = b;
        k[2] = c;
    }
};

struct EquationSolution {
    int num_roots;
    double root1, root2;
    std::string student_name;

    EquationSolution(int nr = 0, double r1 = 0, double r2 = 0, std::string name = "")
        : num_roots(nr), root1(r1), root2(r2), student_name(name) {
    }
};

std::vector<QuadraticEquation> readEquationsFromFile(const std::string& filename);
double discriminant(double[]);
double* solve(double[], double[]);
