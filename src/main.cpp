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

    size_t spacePos = input.find(' ');
    string command;
    string arguments;


    if(spacePos != string::npos){
      command = input.substr(0, spacePos);
      arguments = input.substr(spacePos + 1);
    }else {
      command = input;
    }

    if(command == "exit"){
      return stoi(arguments);
    }

    cout << input + ":" << " command not found";
    
    cout << endl;
  }

  cout << endl;
}
