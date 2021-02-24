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
