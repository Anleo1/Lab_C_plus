#define BUILDING_PLUGIN
#include "Function.h"
#include <cmath>
#include <string>

class Abs : public Function {
public:
    std::string getName() const override {
        return "abs";
    }

    double evaluate(double argument) const override {
        return std::abs(argument);
    }
};

extern "C" {
    PLUGIN_API Function* createFunction() {
        return new Abs();
    }

    PLUGIN_API void destroyFunction(Function* function) {
        delete function;
    }
}