#include "Plugin.h"
#include <iostream>
#include <stdexcept>
#include <direct.h>

Plugin::Plugin() {}

Plugin::~Plugin() {
    for (auto& plugin : loadedPlugins) {
        unloadSinglePlugin(plugin);
    }
}

bool Plugin::directoryExists(const std::string& path) {
    DWORD attrib = GetFileAttributesA(path.c_str());
    return (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY));
}

std::vector<std::string> Plugin::getFilesInDirectory(const std::string& directoryPath) {
    std::vector<std::string> files;

    WIN32_FIND_DATAA findData;
    std::string searchPath = directoryPath + "\\*.dll";
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return files;
    }

    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            files.push_back(findData.cFileName);
        }
    } while (FindNextFileA(hFind, &findData));

    FindClose(hFind);
    return files;
}

template<typename T>
T Plugin::getFunctionPointer(HMODULE libraryHandle, const std::string& functionName) {
    return reinterpret_cast<T>(GetProcAddress(libraryHandle, functionName.c_str()));
}

bool Plugin::validatePlugin(HMODULE libraryHandle) {
    auto createFunc = getFunctionPointer<Function * (*)()>(libraryHandle, "createFunction");
    auto destroyFunc = getFunctionPointer<void(*)(Function*)>(libraryHandle, "destroyFunction");

    if (!createFunc || !destroyFunc) {
        return false;
    }

    Function* testInstance = nullptr;
    try {
        testInstance = createFunc();
        if (!testInstance || testInstance->getName().empty()) {
            if (testInstance) destroyFunc(testInstance);
            return false;
        }
        destroyFunc(testInstance);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Plugin validation exception: " << e.what() << std::endl;
        if (testInstance) destroyFunc(testInstance);
        return false;
    }
    catch (...) {
        std::cerr << "Unknown plugin validation exception" << std::endl;
        if (testInstance) destroyFunc(testInstance);
        return false;
    }
}

void Plugin::loadPluginsFromDirectory(const std::string& directoryPath) {
    try {
        if (!directoryExists(directoryPath)) {
            std::cout << "Plugins directory not found: " << directoryPath << std::endl;
            return;
        }

        auto files = getFilesInDirectory(directoryPath);
        int pluginsLoaded = 0;

        for (const auto& file : files) {
            std::string fullPath = directoryPath + "\\" + file;
            loadSinglePlugin(fullPath);
            pluginsLoaded++;
        }

        if (pluginsLoaded > 0) {
            std::cout << "Loaded " << pluginsLoaded << " plugin(s)" << std::endl << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading plugins from directory: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown error loading plugins from directory" << std::endl;
    }
}

void Plugin::loadSinglePlugin(const std::string& pluginPath) {
    HMODULE libraryHandle = LoadLibraryA(pluginPath.c_str());

    if (!libraryHandle) {
        DWORD error = GetLastError();
        std::cerr << "Failed to load DLL " << pluginPath << " (error: " << error << ")" << std::endl;
        return;
    }

    if (!validatePlugin(libraryHandle)) {
        std::cerr << "Plugin validation failed: " << pluginPath << std::endl;
        FreeLibrary(libraryHandle);
        return;
    }

    auto createFunc = getFunctionPointer<Function * (*)()>(libraryHandle, "createFunction");
    auto destroyFunc = getFunctionPointer<void(*)(Function*)>(libraryHandle, "destroyFunction");

    Function* functionInstance = nullptr;
    try {
        functionInstance = createFunc();
        if (!functionInstance) {
            throw std::runtime_error("Factory function returned nullptr");
        }

        std::string functionName = functionInstance->getName();

        if (availableFunctions.count(functionName)) {
            std::cout << "Function '" << functionName << "' already loaded, skipping" << std::endl;
            destroyFunc(functionInstance);
            FreeLibrary(libraryHandle);
            return;
        }

        PluginHandle plugin;
        plugin.libraryHandle = libraryHandle;
        plugin.function = functionInstance;

        loadedPlugins.push_back(plugin);
        availableFunctions[functionName] = functionInstance;

        std::cout << "Loaded function: " << functionName << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading plugin '" << pluginPath << "': " << e.what() << std::endl;
        if (functionInstance && destroyFunc) {
            destroyFunc(functionInstance);
        }
        FreeLibrary(libraryHandle);
    }
    catch (...) {
        std::cerr << "Unknown error loading plugin '" << pluginPath << "'" << std::endl;
        if (functionInstance && destroyFunc) {
            destroyFunc(functionInstance);
        }
        FreeLibrary(libraryHandle);
    }
}

bool Plugin::isFunctionAvailable(const std::string& functionName) const {
    return availableFunctions.find(functionName) != availableFunctions.end();
}

double Plugin::executeFunction(const std::string& functionName, double argument) const {
    auto it = availableFunctions.find(functionName);
    if (it != availableFunctions.end()) {
        try {
            return it->second->evaluate(argument);
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Function '" + functionName + "' error: " + e.what());
        }
        catch (...) {
            throw std::runtime_error("Function '" + functionName + "' unknown error");
        }
    }
    throw std::runtime_error("Function not found: '" + functionName + "'");
}

std::vector<std::string> Plugin::getAvailableFunctions() const {
    std::vector<std::string> functions;
    for (const auto& pair : availableFunctions) {
        functions.push_back(pair.first);
    }
    return functions;
}

void Plugin::unloadSinglePlugin(PluginHandle& plugin) {
    if (plugin.libraryHandle) {
        auto destroyFunc = getFunctionPointer<void(*)(Function*)>(
            plugin.libraryHandle, "destroyFunction");
        if (destroyFunc && plugin.function) {
            destroyFunc(plugin.function);
        }
        FreeLibrary(plugin.libraryHandle);
        plugin.libraryHandle = nullptr;
        plugin.function = nullptr;
    }
}