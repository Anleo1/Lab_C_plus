#include "TestFunctions.hpp"
#include "locale.h"

// Главная функция
int main() {
    setlocale(LC_ALL, "Rus");
    try {
        runAssignmentTest();
        runAllTypeTests();
        testDefaultArguments();

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "\nОшибка: " << e.what() << std::endl;
        return 1;
    }
}