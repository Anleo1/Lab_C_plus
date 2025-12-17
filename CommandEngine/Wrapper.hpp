#pragma once
#ifndef WRAPPER_HPP
#define WRAPPER_HPP

#include "InterfaceCommand.hpp"
#include <tuple>
#include <type_traits>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <functional>
#include <utility>


namespace detail {
    template<typename T>
    T parseValue(const std::string& str) {
        std::stringstream ss(str);
        T value;
        ss >> value;
        if (ss.fail()) {
            throw std::runtime_error("Cannot parse value: " + str);
        }
        return value;
    }

    // Специализации для разных типов
    template<>
    inline int parseValue<int>(const std::string& str) {
        try {
            return std::stoi(str);
        }
        catch (...) {
            throw std::runtime_error("Cannot parse int from: " + str);
        }
    }

    template<>
inline double parseValue<double>(const std::string& str) {
    try {
        std::string localStr = str;
        
        std::locale current_locale("");
        char decimal_point = std::use_facet<std::numpunct<char>>(current_locale).decimal_point();
        
        // Если разделитель - запятая, заменяем точку на запятую
        if (decimal_point == ',') {
            std::replace(localStr.begin(), localStr.end(), '.', ',');
        }
        
        return std::stod(localStr);
    }
    catch (...) {
        throw std::runtime_error("Cannot parse double from: " + str);
    }
}

    template<>
    inline std::string parseValue<std::string>(const std::string& str) {
        return str;
    }

    template<>
    inline bool parseValue<bool>(const std::string& str) {
        std::string lower = str;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        return (lower == "true" || lower == "1" || lower == "yes" || lower == "on");
    }
}

template<typename ClassType, typename ReturnType, typename... ParamTypes>
class Wrapper : public InterfaceCommand {
public:
    using MethodPtr = ReturnType(ClassType::*)(ParamTypes...);

    Wrapper(ClassType* obj, MethodPtr method,
        const std::vector<std::pair<std::string, ArgValue>>& defaults = {})
        : obj(obj), method(method), defaults(defaults) {
    }

    CommandResult execute(const ArgList& args = {}) override {
        std::vector<ArgValue> finalValues;

        if (!defaults.empty()) {
            for (const auto& def : defaults) {
                finalValues.push_back(def.second);
            }
        }
        else {
            finalValues = initializeDefaults();
        }

        for (const auto& arg : args) {
            const auto& name = arg.first;
            const auto& strValue = arg.second;

            int index = -1;
            for (size_t i = 0; i < defaults.size(); ++i) {
                if (defaults[i].first == name) {
                    index = static_cast<int>(i);
                    break;
                }
            }

            if (index >= 0 && index < static_cast<int>(finalValues.size())) {
                finalValues[index] = convertStringToType(strValue, defaults[index].second);
            }
        }

        constexpr std::size_t expected = sizeof...(ParamTypes);
        if (finalValues.size() < expected) {
            throw std::runtime_error("Not enough arguments for command");
        }

        return callMethod(finalValues);
    }

private:
    ClassType* obj;
    MethodPtr method;
    std::vector<std::pair<std::string, ArgValue>> defaults;

    std::vector<ArgValue> initializeDefaults() const {
        std::vector<ArgValue> result;
        (result.push_back(getDefaultValue<ParamTypes>()), ...);
        return result;
    }

    template<typename T>
    ArgValue getDefaultValue() const {
        if constexpr (std::is_same_v<T, int>) {
            return 0;
        }
        else if constexpr (std::is_same_v<T, double>) {
            return 0.0;
        }
        else if constexpr (std::is_same_v<T, bool>) {
            return false;
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            return std::string("");
        }
        else {
            throw std::runtime_error("Unsupported type");
        }
    }

    ArgValue convertStringToType(const std::string& str, const ArgValue& target) const {
        if (std::holds_alternative<int>(target)) {
            return detail::parseValue<int>(str);
        }
        else if (std::holds_alternative<double>(target)) {
            return detail::parseValue<double>(str);
        }
        else if (std::holds_alternative<bool>(target)) {
            return detail::parseValue<bool>(str);
        }
        else if (std::holds_alternative<std::string>(target)) {
            return str;
        }
        else {
            throw std::runtime_error("Unsupported argument type");
        }
    }

    template<std::size_t... I>
    CommandResult callImpl(const std::vector<ArgValue>& finalValues,
        std::index_sequence<I...>) {
        if constexpr (std::is_same_v<ReturnType, void>) {
            // Для void 
            (obj->*method)(std::get<ParamTypes>(finalValues[I])...);
            return CommandResult{ VoidResult{} };
        }
        else {
            ReturnType result = (obj->*method)(std::get<ParamTypes>(finalValues[I])...);
            return CommandResult{ ArgValue{result} };
        }
    }

    CommandResult callMethod(const std::vector<ArgValue>& finalValues) {
        return callImpl(finalValues, std::index_sequence_for<ParamTypes...>{});
    }
};

// Для методов без параметров
template<typename ClassType, typename ReturnType>
class Wrapper<ClassType, ReturnType> : public InterfaceCommand {
public:
    using MethodPtr = ReturnType(ClassType::*)();

    Wrapper(ClassType* obj, MethodPtr method)
        : obj(obj), method(method) {
    }

    CommandResult execute(const ArgList& args = {}) override {
        if (!args.empty()) {
            throw std::runtime_error("Method takes no arguments");
        }

        if constexpr (std::is_same_v<ReturnType, void>) {
            (obj->*method)();
            return CommandResult{ VoidResult{} };
        }
        else {
            ReturnType result = (obj->*method)();
            return CommandResult{ ArgValue{result} };
        }
    }

private:
    ClassType* obj;
    MethodPtr method;
};

#endif