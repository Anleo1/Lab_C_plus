#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>

class Function {
public:
    virtual ~Function() = default;
    virtual std::string getName() const = 0;
    virtual double evaluate(double argument) const = 0;
};

extern "C" {
#ifdef BUILDING_PLUGIN
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API __declspec(dllimport)
#endif

    PLUGIN_API Function* createFunction();
    PLUGIN_API void destroyFunction(Function* function);
}

#endif