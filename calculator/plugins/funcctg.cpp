#define BUILDING_PLUGIN
#include "Function.h"
#include <cmath>
#include <string>
#include <stdexcept>

class Ctg : public Function {
public:
    std::string getName() const override {
        return "ctg";
    }

    double evaluate(double argument) const override {
        double radians = argument * 3.14159265358979323846 / 180.0;
        double sin = std::sin(radians);
        if (std::abs(sin) < 1e-12) {
            throw std::runtime_error("Cotangent undefined (k*180°)");
        }
        return std::cos(radians) / sin;
    }
};

extern "C" {
    PLUGIN_API Function* createFunction() {
        return new Ctg();
    }

    PLUGIN_API void destroyFunction(Function* function) {
        delete function;
    }
}