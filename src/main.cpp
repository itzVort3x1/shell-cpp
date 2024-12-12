#include <iostream>
#include <unordered_set>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string>
#include <dirent.h>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <filesystem>
#include <iomanip>

using namespace std;

// Set of predefined shell built-in commands
unordered_set<string> SHELLBUILTINS = {"exit", "echo", "type", "abcd", "ls", "pwd"};

// Function to split a string by a specified delimiter
vector<string> split_string(string &s, char delimiter) {
    stringstream ss(s);
    vector<string> return_vect;
    string token;

    while (getline(ss, token, delimiter)) {
        return_vect.push_back(token); // Add each token to the vector
    }

    return return_vect;
}

// Function to search for a file in a directory
bool searchFileInDirectory(const string &directory, const string &filename, string &foundPath) {
    struct stat sb;
    string fullPath = directory + "/" + filename;

    // Check if the file exists and is a regular file
    if (stat(fullPath.c_str(), &sb) == 0 && S_ISREG(sb.st_mode)) {
        foundPath = fullPath;
        return true; // File found
    }
    return false; // File not found
}

// Function to handle "type" command, checking if a command is a shell built-in or executable
void searchBuiltIn(const string &filename) {
    if (SHELLBUILTINS.find(filename) != SHELLBUILTINS.end()) {
        cout << filename << " is a shell builtin" << endl; // Command is a built-in
        return;
    }

    string foundPath;
    const char *pathEnv = getenv("PATH"); // Get the PATH environment variable
    vector<string> searchPaths;

    if (pathEnv) {
        string pathStr = pathEnv;
        size_t pos = 0;
        while ((pos = pathStr.find(':')) != string::npos) {
            searchPaths.push_back(pathStr.substr(0, pos)); // Add each directory in PATH
            pathStr.erase(0, pos + 1);
        }
        searchPaths.push_back(pathStr);
    }

    searchPaths.insert(searchPaths.begin(), {"/usr/bin", "/bin"}); // Common directories

    // Search for the command in the directories
    for (const string &dir : searchPaths) {
        if (searchFileInDirectory(dir, filename, foundPath)) {
            cout << filename << " is " << foundPath << endl; // Command found
            return;
        }
    }

    cerr << filename << ": not found" << endl; // Command not found
}

// Main function
int main() {
    std::cout << std::unitbuf; // Enable automatic flushing for cout
    std::cerr << std::unitbuf; // Enable automatic flushing for cerr

    string path_string = getenv("PATH"); // Get PATH environment variable
    vector<string> path = split_string(path_string, ':'); // Split PATH into directories

    while (true) {
        cout << "$ "; // Display shell prompt
        string input;
        getline(cin, input); // Get user input

        if (input.empty()) {
            continue; // Skip empty input
        }

        size_t spacePos = input.find(' ');
        string command;
        string arguments;

        if (spacePos != string::npos) {
            command = input.substr(0, spacePos); // Extract command
            arguments = input.substr(spacePos + 1); // Extract arguments
        } else {
            command = input;
        }

        if (command == "exit") {
            int exitCode = 0;
            if (!arguments.empty()) {
                try {
                    exitCode = stoi(arguments); // Convert argument to exit code
                } catch (std::invalid_argument &e) {
                    cerr << "Invalid exit code. Exiting with default code 0." << endl;
                } catch (std::out_of_range &e) {
                    cerr << "Exit code out of range. Exiting with default code 0." << endl;
                }
            }
            return exitCode; // Exit with the specified code
        } else if (command == "echo") {
            cout << arguments << endl; // Print arguments to stdout
        } else if (command == "type") {
            if (arguments.empty()) {
                cerr << "type: usage: type [command]" << endl; // Error if no arguments provided
                continue;
            }

            searchBuiltIn(arguments); // Check if the command is a built-in or executable
        } else if(command == "pwd") {
          cout << absolute(filesystem::current_path()).string() << endl;
        } else if (command == "cd") {
            try {
                if (arguments != "~") {
                    filesystem::current_path(arguments);
                }else {
                    const char *homeEnv = getenv("HOME");
                    filesystem::current_path(homeEnv);
                }
            }catch (exception &e) {
                cerr << "cd: " << arguments << ": No such file or directory" << endl;
            }
        }else {
            string foundPath;
            bool found = false;
            
            // Search for the command in the PATH directories
            for (const string &dir : path) {
                if (searchFileInDirectory(dir, command, foundPath)) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                cerr << command << ": not found" << endl; // Command not found
                continue;
            }

            pid_t pid = fork(); // Fork a new process
            if (pid == 0) { // Child process
                vector<string> args = split_string(input, ' '); // Split input into arguments
                vector<char *> c_args;

                for (auto &arg : args) {
                    c_args.push_back(&arg[0]); // Convert arguments to C-style strings
                }
                c_args.push_back(nullptr);

                execvp(foundPath.c_str(), c_args.data()); // Execute the command
                perror("execvp"); // Print error if execvp fails
                exit(EXIT_FAILURE); // Exit child process on failure
            } else if (pid > 0) { // Parent process
                int status;
                waitpid(pid, &status, 0); // Wait for the child process to complete
            } else {
                perror("fork"); // Print error if fork fails
            }
        }
    }

    return 0;
}
