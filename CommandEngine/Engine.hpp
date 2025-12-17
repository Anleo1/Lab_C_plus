#pragma once
#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "InterfaceCommand.hpp"
#include <unordered_map>
#include <mutex>
#include <stdexcept>
#include <string>

class Engine {
public:
    Engine() = default;
    void register_command(InterfaceCommand* cmd, const std::string& name) {

        if (name.empty()) {
            throw std::invalid_argument("Command name cannot be empty");
        }

        if (commands.find(name) != commands.end()) {
            throw std::runtime_error("Command already registered: " + name);
        }

        commands[name] = cmd;
    }

    CommandResult execute(const std::string& name, const ArgList& args = {}) {

        auto it = commands.find(name);
        if (it == commands.end()) {
            throw std::runtime_error("Command not found: " + name);
        }

        try {
            return it->second->execute(args);
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Error executing command '" + name + "': " + e.what());
        }
    }

    bool has_command(const std::string& name) const {
        return commands.find(name) != commands.end();
    }

    void clear() {
        commands.clear();
    }

private:
    std::unordered_map<std::string, InterfaceCommand*> commands;
};

#endif 