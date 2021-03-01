#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <iostream>
#include <limits>
#include <fstream>


template<typename T>
bool inputHandler(T &input, const T options[], int size){

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
      for(int i = 0; i < size; i++){
        if(options[i] == input)return true;
      }
      std::cout << "Wrong option.." <<std::endl;
      break;
    }
  }
  return false;
}

bool inputHandler(int &input, int min, int max);

int getFileLines(std::ifstream& file);

void pressEnterToContinue(bool flag = true);

#endif