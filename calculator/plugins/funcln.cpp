#define BUILDING_PLUGIN
#include "Function.h"
#include <cmath>
#include <string>
#include <stdexcept>

class Ln : public Function {
public:
    std::string getName() const override {
        return "ln";
    }

    double evaluate(double argument) const override {
        if (argument <= 0) {
            throw std::runtime_error("Natural logarithm of non-positive number");
        }
        return std::log(argument);
    }
};

extern "C" {
    PLUGIN_API Function* createFunction() {
        return new Ln();
    }

    PLUGIN_API void destroyFunction(Function* function) {
        delete function;
    }
}