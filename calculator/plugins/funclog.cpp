#define BUILDING_PLUGIN
#include "Function.h"
#include <cmath>
#include <string>
#include <stdexcept>

class Log : public Function {
public:
    std::string getName() const override {
        return "log";
    }

    double evaluate(double argument) const override {
        if (argument <= 0) {
            throw std::runtime_error("Logarithm of non-positive number");
        }
        return std::log10(argument);
    }
};

extern "C" {
    PLUGIN_API Function* createFunction() {
        return new Log();
    }

    PLUGIN_API void destroyFunction(Function* function) {
        delete function;
    }
}