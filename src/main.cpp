#include <iostream>
#include <unordered_set>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string>
#include <dirent.h>
#include <vector>
using namespace std;

string BIN_PATH="/usr/bin/";
string LOCAL_PATH="/usr/local/bin/";
unordered_set<string> SHELLBUILTINS = {"exit", "echo", "type", "abcd", "ls"};


bool searchFileInDirectory(const string &directory, const string &filename, string &foundPath) {
    struct stat sb;
    string fullPath = directory + "/" + filename;

    if (stat(fullPath.c_str(), &sb) == 0 && S_ISREG(sb.st_mode)) {
        foundPath = fullPath;
        return true;
    }
    return false;
}

void searchBuiltIn(const string &filename) {
    string foundPath;

    // Get directories from PATH environment variable
    const char *pathEnv = getenv("PATH");
    vector<string> searchPaths;

    if (pathEnv) {
        string pathStr = pathEnv;
        size_t pos = 0;
        while ((pos = pathStr.find(':')) != string::npos) {
            searchPaths.push_back(pathStr.substr(0, pos));
            pathStr.erase(0, pos + 1);
        }
        searchPaths.push_back(pathStr); // Add the last segment
    }

    // Prioritize specific directories (e.g., /usr/bin)
    searchPaths.insert(searchPaths.begin(), {"/usr/bin", "/bin"});

    // Search in prioritized directories
    for (const string &dir : searchPaths) {
        if (searchFileInDirectory(dir, filename, foundPath)) {
            cout << filename << " is " << foundPath << endl;
            return;
        }
    }

    // If not found in PATH, fallback to a full filesystem search
    cerr << filename << ": not found" << endl;
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

      searchBuiltIn(arguments);
    } else {
      cerr << command << ": command not found" << endl;
    }
  }

  cout << endl;
}
