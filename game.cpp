#include <iostream>
#include <cstdlib>
#include "game.h"

using namespace std;

string Market::type[3] = {"weapon", "armor", "spell"};

Market::Market(){
  /* Create different type of weapons */
  this->name = "Default Market";

  this->weaponsNum = 6;
  this->weapons = new Weapon *[weaponsNum];

  this->weapons[0] = new Weapon("Iron Slasher", 250, 200, 3);
  this->weapons[1] = new Weapon("Iron Sword", 200, 180, 3);
  this->weapons[2] = new Weapon("Iron Staff", 180, 150, 3);
  this->weapons[3] = new Weapon("Silver Slasher", 350, 500, 6);
  this->weapons[4] = new Weapon("Silver Sword", 300, 450, 6);
  this->weapons[5] = new Weapon("Silver Staff", 280, 420, 6);

  this->armorsNum = 2;
  this->armors = new Armor *[armorsNum];

  this->armors[0] = new Armor("Iron armor", 25, 300, 3);
  this->armors[1] = new Armor("silver armor", 35, 700, 6);

  this->spellsNum = 3;
  this->spells = new Spell *[spellsNum];
  int spellDmg = 100;
  int mp = 50;
  spells[0] = new IceSpell("Frost attack", spellDmg, 20, 100, 1, mp);
  spells[1] = new FireSpell("Inferno blast", spellDmg, 5, 100, 1, mp);
  spells[2] = new LightingSpell("Lighting bold", spellDmg, 5, 100, 1, mp);


  cout << "A new market " << this->name << " Constructed!" << endl;

}


Market::~Market(){

  for(int i = 0; i < armorsNum; i++)delete armors[i];
  for(int i = 0; i < weaponsNum; i++)delete weapons[i];
  for(int i = 0; i < spellsNum; i++)delete spells[i];

  delete[] armors;
  delete[] weapons;
  delete[] spells;

  cout << "Market " << name << " to be destructed" << endl;
  
}


bool Market::buy(Item *item, int &money){

  if(item == nullptr)return false;

  money += (item->getPrice() / 2);

  cout << item->getName() << " successfully sold!!" << endl;

  return true;
  
}


Item* Market::sell(int type, int id, int &money, int heroLvl){

  Item *item = nullptr;

  switch(type){
    case this->weapon:
      if(id < this->weaponsNum)item = weapons[id];
      break;
    case this->armor:
      if(id < this->armorsNum)item = armors[id];
      break;
  }

  if(item != nullptr){
    if(money > item->getPrice()){
      if(heroLvl >= item->getLevel()){
        money -= item->getPrice();
        cout << this->type[type] << " " << 
        item->getName() << " successfully purchased!!" << endl;
      }
      else cout << item->getName() << " requires level "
          << item->getLevel() << endl;
    }
    else cout << item->getPrice()-money << " gold is missing!" << endl;
  }
  
  return item;

}


Spell* Market::sell(int id, int &money, int heroLvl){
  
  Spell *spell = nullptr;

  if(id < this->spellsNum)spell = spells[id];

  if(spell != nullptr){
    if(money > spell->getPrice()){
      if(heroLvl >= spell->getLvl()){
        money -= spell->getPrice();
        cout << "Spell " << 
        spell->getName() << " successfully purchased!!" << endl;
      }
      else cout << spell->getName() << " requires level "
          << spell->getLvl() << endl;
    }
    else cout << spell->getPrice()-money << " gold is missing!" << endl;
  }
  
  return spell;
}
