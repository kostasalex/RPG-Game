#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "living.h"
#include "item.h"

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



  cout << "Creating Items.." << endl;

  Item *items[3];

  items[0] = new Potion("Power", "Strength", 100, 2, 2);
  cout << endl;
  items[1] = new Armor("Draconic Leather", 100, 1000, 76);
  cout << endl;
  items[2] = new Weapon("Draconic Bow", 1000, 500, 76);
  cout << endl;

  for(int i = 0; i < 3; i++){
    items[i]->print();
    cout << endl;
  }

  for(int i = 0; i < 3; i++){

    delete heroes[i];
    cout << endl;
    delete monsters[i];
    cout << endl;
    delete items[i];
    cout << endl;

  }

}