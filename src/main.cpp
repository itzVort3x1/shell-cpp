#include <sys/wait.h>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>
#include "../include/builtins.h"
#include "../include/redirection.h"

using namespace std;

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
            string parsedArgument = parseArgument(arguments);// parsing the arguments
            cout << parsedArgument << endl;
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
        } else if (command == "cat") {
            vector<string> files = parseFileNames(arguments);
            printFileContents(files);
        }else {
            vector<string> parsedCommand = parseFileNames(command);

            // for (auto it: parsedCommand) {
            //     cout << it << endl;
            // }

            if (!parsedCommand.empty()) {
                string executable = parsedCommand[0];

                // Validate if the first token is an executable
                if (isExecutable(executable)) {
                    // Combine the command back for execution
                    string fullCommand = executable;
                    for (size_t i = 1; i < parsedCommand.size(); ++i) {
                        fullCommand += " \"" + parsedCommand[i] + "\"";
                    }

                    // Execute the command
                    // cout << "Executing: " << fullCommand << endl;
                    int retCode = system(fullCommand.c_str()); // Use system() to execute the command

                    if (retCode != 0) {
                        cerr << "Error: Command failed with return code " << retCode << endl;
                    }
                }
            }

            // Checking if there is any redirection
            for(auto str:parsedCommand){
                cout << "testing" << str << endl;
            }

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
