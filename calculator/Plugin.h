#ifndef PLUGIN_H
#define PLUGIN_H

#include "Function.h"
#include <string>
#include <vector>
#include <map>
#include <windows.h>

class Plugin {
private:
    struct PluginHandle {
        HMODULE libraryHandle;
        Function* function;
    };

    std::vector<PluginHandle> loadedPlugins;
    std::map<std::string, Function*> availableFunctions;

public:
    Plugin();
    ~Plugin();

    void loadPluginsFromDirectory(const std::string& directoryPath);
    bool isFunctionAvailable(const std::string& functionName) const;
    double executeFunction(const std::string& functionName, double argument) const;
    std::vector<std::string> getAvailableFunctions() const;

private:
    void loadSinglePlugin(const std::string& pluginPath);
    void unloadSinglePlugin(PluginHandle& plugin);

    template<typename T>
    T getFunctionPointer(HMODULE libraryHandle, const std::string& functionName);
    bool validatePlugin(HMODULE libraryHandle);
    bool directoryExists(const std::string& path);
    std::vector<std::string> getFilesInDirectory(const std::string& directoryPath);
};

#endif