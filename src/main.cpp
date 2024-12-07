#include <iostream>
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

    cout << input + ":" << " command not found";
    
    cout << endl;
  }

  cout << endl;
}
