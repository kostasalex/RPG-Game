#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <iostream>
#include <limits>

const int options[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

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

#endif