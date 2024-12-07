#include <iostream>
#include <sys/wait.h>
using namespace std;

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage

  while(true){

    cout << "$ ";
    string input;
    getline(cin, input);

    // handling empty input
    if(input.empty()){
      continue;
    } 

    size_t spacePos = input.find(' ');
    string command;
    string arguments;

    if(spacePos != string::npos){
      command = input.substr(0, spacePos);
      arguments = input.substr(spacePos+1);
    }else {
      command = input;
    }

    if(command == "exit"){
      int exitCode = 0;
      if(!arguments.empty()){
        try {
          exitCode = stoi(arguments);
        }catch (std::invalid_argument &e){
          cerr << "Invalid exit code. Exiting with default code 0." << endl;
        }catch (std::out_of_range &e){
          cerr << "Exit code out of range. Exiting with default code 0." << endl;
        }
      }
      return exitCode;
    }

    cerr << command + ":" << " command not found" << endl;

  }

  cout << endl;
}
