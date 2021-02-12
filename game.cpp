#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "living.h"
#include "item.h"
#include "grid.h"

using namespace std;

int main(void){

  //*Debug: Hero
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


  //*Debug: Monster
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


  //*Debug: Item
  cout << "Creating Items.." << endl;

  Item *items[3];

  items[0] = new Potion("Power", "Strength", 100, 2, 2);
  cout << endl;
  items[1] = new Armor("Draconic Leather", 100, 1000, 76);
  cout << endl;
  items[2] = new Weapon("Draconic Bow", 1000, 500, 76);
  cout << endl;

  cout << "Creating a Grid.." << endl;


  //*Debug: Grid
  Grid *grid = new Grid();

  cout << endl;
  grid->displayMap();

  for(int i = 0; i < 3; i++){
    items[i]->print();
    cout << endl;
  }

  char input = ' ';
  char direction[4] = {'w', 's', 'a', 'd'};
  bool validLoc;
  while(input != 'x'){
    validLoc = false;
    cout << "Where do you want to go? w = up, s = down, a = left, d = right" << endl;
    cin >> input; // Get user input from the keyboard
    for(int i = 0; i < 4; i++){
      if(input == direction[i]){
        validLoc = grid->move(direction[i]);
        grid->displayMap();
      }
    }
    if(validLoc == false){
      cout << "Oops couldn't move there.." << endl;
    }
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