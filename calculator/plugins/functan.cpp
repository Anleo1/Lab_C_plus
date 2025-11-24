#define BUILDING_PLUGIN
#include "Function.h"
#include <cmath>
#include <string>
#include <stdexcept>

class Tan : public Function {
public:
    std::string getName() const override {
        return "tan";
    }

    double evaluate(double argument) const override {
        double radians = argument * 3.14159265358979323846 / 180.0;
        double cos = std::cos(radians);
        if (std::abs(cos) < 1e-12) {
            throw std::runtime_error("Tangent undefined (90° + k*180°)");
        }
        return std::sin(radians) / cos;
    }
};

extern "C" {
    PLUGIN_API Function* createFunction() {
        return new Tan();
    }

    PLUGIN_API void destroyFunction(Function* function) {
        delete function;
    }
}