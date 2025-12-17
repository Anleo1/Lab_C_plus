#pragma once
#ifndef INTERFACECOMMAND_HPP
#define INTERFACECOMMAND_HPP

#include <string>
#include <vector>
#include <variant>
#include <iostream>
#include <map>

struct VoidResult {};
using ArgValue = std::variant<int, double, std::string, bool>;
using ArgList = std::vector<std::pair<std::string, std::string>>;
using CommandResult = std::variant<VoidResult, ArgValue>;

class InterfaceCommand {
public:
    virtual ~InterfaceCommand() = default;
    virtual CommandResult execute(const ArgList& args = {}) = 0;
};

inline std::ostream& operator<<(std::ostream& os, const CommandResult& result) {
    if (std::holds_alternative<VoidResult>(result)) {
        os << "void";
    }
    else {
        const auto& value = std::get<ArgValue>(result);
        std::visit([&os](auto&& arg) {
            os << arg;
            }, value);
    }
    return os;
}

#endif 
