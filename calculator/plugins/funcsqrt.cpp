#define BUILDING_PLUGIN
#include "Function.h"
#include <cmath>
#include <string>
#include <stdexcept>

class Sqrt : public Function {
public:
    std::string getName() const override {
        return "sqrt";
    }

    double evaluate(double argument) const override {
        if (argument < 0) {
            throw std::runtime_error("Square root of negative number");
        }
        return std::sqrt(argument);
    }
};

extern "C" {
    PLUGIN_API Function* createFunction() {
        return new Sqrt();
    }

    PLUGIN_API void destroyFunction(Function* function) {
        delete function;
    }
}