#pragma once

#include <unordered_set>
#include <iostream>
#include "fileHandling.h"

///////////////////////////////////////////////
// Set of predefined shell built-in commands //
///////////////////////////////////////////////
std::unordered_set<std::string> SHELLBUILTINS = {"exit", "echo", "type", "abcd", "ls", "pwd"};

////////////////////////////////////////////////////////////////////////////////////////////////
// Function to handle "type" command, checking if a command is a shell built-in or executable //
////////////////////////////////////////////////////////////////////////////////////////////////
void searchBuiltIn(const std::string &filename) {
    if (SHELLBUILTINS.find(filename) != SHELLBUILTINS.end()) {
        std::cout << filename << " is a shell builtin" << std::endl; // Command is a built-in
        return;
    }

    std::string foundPath;
    const char *pathEnv = getenv("PATH"); // Get the PATH environment variable
    std::vector<std::string> searchPaths;

    if (pathEnv) {
        std::string pathStr = pathEnv;
        size_t pos = 0;
        while ((pos = pathStr.find(':')) != std::string::npos) {
            searchPaths.push_back(pathStr.substr(0, pos)); // Add each directory in PATH
            pathStr.erase(0, pos + 1);
        }
        searchPaths.push_back(pathStr);
    }

    searchPaths.insert(searchPaths.begin(), {"/usr/bin", "/bin"}); // Common directories

    // Search for the command in the directories
    for (const std::string &dir : searchPaths) {
        if (searchFileInDirectory(dir, filename, foundPath)) {
            std::cout << filename << " is " << foundPath << std::endl; // Command found
            return;
        }
    }

    std::cerr << filename << ": not found" << std::endl; // Command not found
}