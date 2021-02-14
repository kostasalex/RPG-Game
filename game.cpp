#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <time.h>    
#include "living.h"
#include "item.h"
#include "grid.h"
#include "spell.h"

using namespace std;

int main(void){

  srand (time(NULL));

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

  items[0] = new Potion("Power", Hero::strength, 100, 2, 1);
  cout << endl;
  items[1] = new Armor("Draconic Leather", 100, 1000, 1);
  cout << endl;
  items[2] = new Weapon("Draconic Bow", 1000, 500, 1);
  cout << endl;


  //*Debug: Spell
  cout << "creating spells.. " << endl;
  int spellDmg = 100;
  int mp = 50;
  Spell *spells[3];
  spells[0] = new IceSpell("Frost attack", spellDmg, 20, 100, 1, mp);
  cout << endl;
  spells[1] = new FireSpell("Inferno blast", spellDmg, 5, 100, 1, mp);
  cout << endl;
  spells[2] = new LightingSpell("Lighting bold", spellDmg, 5, 100, 1, mp);
  cout << endl;

  cout << "Printing spells " << endl;

  for(int i = 0; i < 3; i++){
    spells[i]->print();
    cout << endl;
  }

  for(int i = 0; i < 3; i++){
    heroes[i]->learnSpell(spells[i]);
    cout << endl;
  }


  for(int i = 0; i < 3; i++){
    heroes[i]->checkSpells();
    cout << endl;
  }
  
  cout << "Attacking with spells: "<< endl;

  for(int i = 0; i < 3; i++){
    heroes[i]->castSpell(0, monsters[i]);
    
    cout << endl;
  }


  //*Debug :Attacks

  cout << "Simple attacks: "<< endl;

  for(int i = 0; i < 3; i++){
    heroes[i]->attack(monsters[i]);
    cout << endl;
  }
  cout << endl;


  cout << "Stats changed after compat: " << endl;
  for(int i = 0; i < 3; i++){
    monsters[i]->print();    
    heroes[i]->print();  
    cout << endl;
  }
  cout << endl;



  //*Debug: Inventory

  for(int i = 0; i < 3; i++){
    cout << "Round " << i << endl;

    if(heroes[0]->inventoryAdd(items[i]) == Hero::succeed)
      cout << "Item added succesfuly in inventory!" << endl;
    else cout << "Error adding item : "<< items[i]->getName() << endl;

    heroes[0]->checkInventory();
    int result;

    cout << endl;
    cout << "Trying to equip item: " << endl;
    cout << "result is " << Hero::statusMsg[heroes[0]->equip(1)] << endl;
    cout << endl;
    
    cout << "Trying to equip item: " << endl;
    cout << "result is " << Hero::statusMsg[heroes[0]->equip(0)] << endl;
    cout << endl;
    
    cout << "Trying to use potion: " << endl;
    result = heroes[0]->usePotion(0);

    cout << "result is " << Hero::statusMsg[result] << endl;
    cout << endl;
    cout << endl;

  }

  heroes[0]->print();

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
  while(1){
    validLoc = false;
    cout << "Where do you want to go? w = up, s = down, a = left, d = right" << endl;
    cout << "Or type 'x' for exit." << endl;
    cin >> input; // Get user input from the keyboard
    if(input == 'x')break;
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
    delete spells[i];
    cout << endl;

  }


}