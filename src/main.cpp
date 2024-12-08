#include <iostream>
#include <unordered_set>
#include <sys/wait.h>
using namespace std;

int main() {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Define the set of shell builtins
  unordered_set<string> shellBuiltins = {"exit", "echo", "type"};

  while (true) {
    cout << "$ ";
    string input;
    getline(cin, input);

    // Handling empty input
    if (input.empty()) {
      continue;
    }

    size_t spacePos = input.find(' ');
    string command;
    string arguments;

    if (spacePos != string::npos) {
      command = input.substr(0, spacePos);
      arguments = input.substr(spacePos + 1);
    } else {
      command = input;
    }

    if (command == "exit") {
      int exitCode = 0;
      if (!arguments.empty()) {
        try {
          exitCode = stoi(arguments);
        } catch (std::invalid_argument &e) {
          cerr << "Invalid exit code. Exiting with default code 0." << endl;
        } catch (std::out_of_range &e) {
          cerr << "Exit code out of range. Exiting with default code 0." << endl;
        }
      }
      return exitCode;
    } else if (command == "echo") {
      cout << arguments << endl;
    } else if (command == "type") {
      if (arguments.empty()) {
        cerr << "type: usage: type [command]" << endl;
        continue;
      }

      // Check if the argument is a shell builtin
      if (shellBuiltins.find(arguments) != shellBuiltins.end()) {
        cout << arguments << " is a shell builtin" << endl;
      } else {
        cout << "testing: " << arguments << endl;
        cerr << command << ": not found" << endl;
      }
    } else {
      cerr << command << ": command not found" << endl;
    }
  }

  cout << endl;
}
