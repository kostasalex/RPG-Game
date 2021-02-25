#include "inputHandler.h"

int getFileLines(std::ifstream& file){

  int counter = 0;
  std::string temp;

  while(getline(file, temp))
      counter++;

  file.clear();
  file.seekg(0);

  return counter;
}

void pressEnterToContinue(bool flag){
    std::cout << "\nPress Enter to Continue";
    std::cin.ignore();
    if(flag == true)
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
}