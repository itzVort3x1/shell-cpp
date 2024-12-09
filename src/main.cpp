#include <iostream>
#include <unordered_set>
#include <sys/wait.h>
#include <sys/stat.h>
using namespace std;

string BIN_PATH="/usr/bin/";
string LOCAL_PATH="/usr/local/bin/";
unordered_set<string> SHELLBUILTINS = {"exit", "echo", "type", "abcd", "ls"};


void searchBuiltIn(string &args){
  string binFullPath = BIN_PATH + args;
  string localFullPath = LOCAL_PATH + args;
  const char *binSearchPath = binFullPath.c_str();
  const char *localSearchPath = localFullPath.c_str();
  struct stat sb;

  if(stat(binSearchPath, &sb) == 0){
    cout << args << " is " << binFullPath << endl;
  }else if(stat(localSearchPath, &sb) == 0){
    cout << args << " is " << localSearchPath << endl;
  }
}

int main() {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

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
      if (SHELLBUILTINS.find(arguments) != SHELLBUILTINS.end()) {
        searchBuiltIn(arguments);
      } else {
        cerr << arguments << ": not found" << endl;
      }
    } else {
      cerr << command << ": command not found" << endl;
    }
  }

  cout << endl;
}
