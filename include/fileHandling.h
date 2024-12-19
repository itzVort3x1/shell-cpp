#pragma once

#include <filesystem>
#include <fstream>
#include <sys/stat.h>
#include "stringManipulation.h"

//////////////////////////////////////////////////
// Function to search for a file in a directory //
//////////////////////////////////////////////////
bool searchFileInDirectory(const std::string &directory, const std::string &filename, std::string &foundPath) {
    struct stat sb;
    std::string fullPath = directory + "/" + filename;

    // Check if the file exists and is a regular file
    if (stat(fullPath.c_str(), &sb) == 0 && S_ISREG(sb.st_mode)) {
        foundPath = fullPath;
        return true; // File found
    }
    return false; // File not found
}

/////////////////////////////////////
// Function to print file contents //
/////////////////////////////////////
void printFileContents(const std::vector<std::string> &fileNames) {
    for (const std::string& fileName : fileNames) {
        std::ifstream file(fileName);

        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file '" << fileName << "'\n";
            continue;
        }

        std::string line;
        while (getline(file, line)) {
            std::cout << line;
        }
        file.close();
    }
    std::cout << std::endl;
}

//////////////////////////////////////////////////////////
// Function to check if a file exists and is executable //
//////////////////////////////////////////////////////////
bool isExecutable(const std::string &filePath) {
    return std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath);
}