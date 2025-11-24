#include "Calculator.h"
#include <iostream>
#include <iomanip>

int main() {
    try {
        Calculator calculator;

        std::cout << "Calculator ready." << std::endl;
        auto functions = calculator.getAvailableFunctions();
        if (!functions.empty()) {
            std::cout << "Available functions: ";
            for (size_t i = 0; i < functions.size(); ++i) {
                std::cout << functions[i] << (i < functions.size() - 1 ? ", " : "");
            }
            std::cout << std::endl;
        }
        std::cout << "Enter 'exit' to out" << std::endl << std::endl;

        while (true) {
            std::cout << "Enter expression :" << std::endl <<"> ";
            std::string input;
            std::getline(std::cin, input);

            if (input == "exit") break;
            if (input.empty()) continue;

            try {
                double result = calculator.calculate(input);
                std::cout << "= " << std::setprecision(15) << result << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }

        std::cout << "The calculator is finished" << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Initialization error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown initialization error" << std::endl;
        return 1;
    }

    return 0;
}