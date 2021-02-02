#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "living.h"

using namespace std;

int main(void){

  cout << "Creating heroes.." << endl;

  Hero *heroes[3];

  heroes[0] = new Paladin("Kotsi");

  heroes[1] = new Warrior("Asapis");

  heroes[2] = new Sorcerer("Katsa");

  for(int i = 0; i < 3; i++){
    heroes[i]->print();    
    cout << endl;
  }
  cout << endl;

  cout << "Creating monsters.." << endl;

  Monster *monsters[3];

  monsters[0] = new Dragon("Antharas");

  monsters[1] = new Exoskeleton("Doom Blade Thanatos");

  monsters[2] = new Spirit("Fairy Queen");

  for(int i = 0; i < 3; i++){
    monsters[i]->print();    
    cout << endl;
  }
  cout << endl;

}