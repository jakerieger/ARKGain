// Author: Jake Rieger
// Created: 7/8/2024.
//

#pragma once

#include <string>
#include <filesystem>

#if defined(_WIN32) || defined(_WIN64)
    #pragma comment(lib, "shlwapi.lib")

    #include <Windows.h>
    #include <shlwapi.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
    #include <libgen.h>
#elif defined(__linux__)
    #include <unistd.h>
    #include <libgen.h>
    #include <dlfcn.h>
#endif

inline bool GetModulePath(std::string& path) {
    char modulePath[FILENAME_MAX];
#if defined(_WIN32) || defined(_WIN64)
    HMODULE hModule = nullptr;
    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                           reinterpret_cast<LPCSTR>(GetModulePath),
                           &hModule)) {
        GetModuleFileNameA(hModule, modulePath, sizeof(modulePath));
        PathRemoveFileSpecA(modulePath);
        path = std::string(modulePath);
        return true;
    }
#elif defined(__APPLE__)
    uint32_t size = sizeof(modulePath);
    if (_NSGetExecutablePath(modulePath, &size) == 0) {
        char* dir = dirname(modulePath);  // Get the directory part of the path
        path      = dir;
        return true;
    }
#elif defined(__linux__)
    Dl_info info;
    if (dladdr(reinterpret_cast<void*>(&getModulePath), &info)) {
        strncpy(modulePath, info.dli_fname, sizeof(modulePath));
        char* dir = dirname(modulePath);  // Get the directory part of the path
        path      = dir;
        return true;
    }
#endif

    return false;
}

inline std::string GetResourcePath(const std::string& name) {
    std::string pathStr;
    if (!GetModulePath(pathStr)) {
        return "";
    }

    auto path = std::filesystem::path(pathStr).parent_path();
    path.append("Resources");
    path.append(name);

    return path.string();
}