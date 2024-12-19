#pragma once

#include <string>
#include <sstream>
#include <vector>
#include "builtins.h"
/////////////////////////////////////////////////////////
// Function to split a string by a specified delimiter //
/////////////////////////////////////////////////////////
std::vector<std::string> split_string(std::string &s, char delimiter) {
    std::stringstream ss(s);
    std::vector<std::string> return_vect;
    std::string token;

    while (getline(ss, token, delimiter)) {
        return_vect.push_back(token); // Add each token to the vector
    }

    return return_vect;
}

////////////////////////////////////
// Function to parse string input //
////////////////////////////////////
std::string parseArgument(const std::string &input) {
    std::string result;
    std::string temp;
    bool isInsideQuotes = false;
    char currentQuoteChar = '\0';
    bool isEscaping = false;

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if (isEscaping) {
            // Handle escaped characters
            temp += c; // Add the next character literally
            isEscaping = false;
            continue;
        }

       if (c == '\\') {
            // Start escaping the next character
            if (isInsideQuotes && currentQuoteChar == '"') {
                // In double quotes, allow escaping specific characters
                if (i + 1 < input.size() &&
                    (input[i + 1] == '"' || input[i + 1] == '\\' || input[i + 1] == '$' || input[i + 1] == '`')) {
                    isEscaping = true;
                    continue;
                }
            } else if (!isInsideQuotes) {
                // Outside quotes, treat backslash as escape
                isEscaping = true;
                continue;
            }

            // Inside single quotes, treat backslash as literal
            temp += c;
            continue;
        }

        // Handle quotes
        if ((c == '"' || c == '\'') && !isEscaping) {
            if (isInsideQuotes && c == currentQuoteChar) {
                // End of quoted section
                isInsideQuotes = false;
                currentQuoteChar = '\0';
            } else if (!isInsideQuotes) {
                // Start of quoted section
                isInsideQuotes = true;
                currentQuoteChar = c;
            } else {
                // Inside quotes, treat as literal
                temp += c;
            }
            continue;
        }

        // Outside quotes, split arguments by spaces
        if (c == ' ' && !isInsideQuotes) {
            if (!temp.empty()) {
                if (!result.empty()) {
                    result += " ";
                }
                result += temp;
                temp.clear();
            }
        } else {
            temp += c;
        }
    }

    // Append the last word
    if (!temp.empty()) {
        if (!result.empty()) {
            result += " ";
        }
        result += temp;
    }

    return result;
}

////////////////////////////////////////////////////////////////
// Function to parse the command input and extract file names //
////////////////////////////////////////////////////////////////
std::vector<std::string> parseFileNames(const std::string &commandInput) {
    std::vector<std::string> fileNames;
    std::string currentFileName;
    bool isInsideQuotes = false;
    char currentQuoteChar = '\0';
    bool isEscaping = false;
    bool isExecutableParsed = false; // To track whether the executable part has been handled

    for (size_t i = 0; i < commandInput.size(); ++i) {
        char c = commandInput[i];

        if (isEscaping) {
            // Handle escaped characters
            currentFileName += c; // Add the next character literally
            isEscaping = false;
            continue;
        }

        if (c == '\\') {
            // Start escaping the next character
            isEscaping = true;
            continue;
        }

        // Handle quoting
        if ((c == '"' || c == '\'') && !isEscaping) {
            if (isInsideQuotes && c == currentQuoteChar) {
                // End of quoted section
                isInsideQuotes = false;
                currentQuoteChar = '\0';
            } else if (!isInsideQuotes) {
                // Start of quoted section
                isInsideQuotes = true;
                currentQuoteChar = c;
            } else {
                // Inside quotes, treat as literal
                currentFileName += c;
            }
            continue;
        }

        // Handle spaces outside quotes
        if (c == ' ' && !isInsideQuotes) {
            if (!currentFileName.empty()) {
                fileNames.push_back(currentFileName); // Add completed filename or executable
                currentFileName.clear();
                if (!isExecutableParsed) {
                    isExecutableParsed = true; // Mark the executable as parsed
                }
            }
        } else {
            currentFileName += c;
        }
    }

    // Add the last file name or executable
    if (!currentFileName.empty()) {
        fileNames.push_back(currentFileName);
    }

    return fileNames;
}