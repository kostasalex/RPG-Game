#include "inputHandler.h"

bool inputHandler(int &input, int min, int max){
  std::cin >> input;

  while(1)
  {
    if(std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
      std::cout << "Invalid option.." <<std::endl;
      std::cin >> input;
    }
    if(!std::cin.fail()){
      if(input >= min && input <= max)return true;
      std::cout << "Wrong option.." <<std::endl;
      break;
    }
  }
  return false;
}

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