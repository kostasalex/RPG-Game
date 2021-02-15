#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "game.h"

using namespace std;

string Market::type[3] = {"weapon", "armor", "spell"};

Game::Game(){
  
  this->gameState = peace;

  cout << "Rpg game started!!" << endl;
  
  int heroesNo = 3;
  cout << "You can create " << heroesNo << " heroes maximum" << endl;
  
  int select;
  string name;
  Hero *hero;

  this->heroesNum = 0;

  while(heroesNum < 3){

    if(heroesNum > 0){
      cout << "1. Create another hero" << endl
           << "0. Exit creation." << endl;
      //cin >> select;
      //*debug*
      select = 0;
      if(select == 0)break;
    }

    cout << "Select a class : " << endl
         << "1.Warrior"  << endl
         << "2.Paladin"  << endl
         << "3.Sorcerer" << endl;

    //cin >> select;
    //*debug
    select = 1;
    if(select < 1 || select > 3){
      cout << "Invalid number!"<< endl;
      continue;
    }
    
    cout << "Enter name: " << endl;
    
    //cin >> name;
    //*Debug
    name = "asapis";

    switch(select){
      case 1:
        hero = new Warrior(name);
        break;
      
      case 2:
        hero = new Paladin(name);
        break;

      case 3:
        hero = new Sorcerer(name);
        break;
    }
    this->heroes.push_back(hero);
    this->heroesNum++;
  }

  cout << this->heroesNum <<" Heroes created: " << endl << endl;
  for(int i = 0; i < heroesNum; i++){
    this->heroes[i]->print();
    cout << endl;
  }

  this->market = new Market();

  this->grid = new Grid();

}


void Game::play(){
  
  system("clear");

  char input = ' ';
  char direction[4] = {'w', 's', 'a', 'd'};
  bool validLoc;

  grid->displayMap();

  int state;
  while(1){


    validLoc = false;

    cout << "Where do you want to go? w = up, s = down, a = left, d = right" << endl;
    cout << "Or type 'x' for exit." << endl;
    cin >> input; // Get user input from the keyboard

    if(input == 'x')break;

    for(int i = 0; i < 4; i++){
      if(input == direction[i]){
        validLoc = grid->move(direction[i]);
        break;
      }
    }
    if(validLoc == false){
      cout << "Oops couldn't move there.." << endl;
      sleep(1);
    }
    else{
      
      grid->displayMap();
      setGameState(grid->getBlockType());
      state = getGameState();
      cout << "** " << state <<" **" << endl;
      switch(state){
        case trading:
          this->shop(heroes[0]);
          break;
        case fighting:
          this->combat();
          break;
      }
    }
    //system("clear");
  }

}


void Game::shop(Hero *hero){
  
  cout << "Welcome to my store!!" << endl
       << "What item are you looking for?" << endl;
       

      Spell *spellTemp = nullptr;
      Item *itemTemp = nullptr;

      int money, select;
      money = hero->getMoney();
      bool goBack;

       while(1){
        
        cout << "1. Buy " << endl
             << "2. Sell" << endl
             << "3. Exit" << endl;

        cin >> select;

        goBack = false;

        switch(select){
          
          /* Buy a product from market */
          case 1:
            cout << "What do you want to buy?" << endl;
            while(1){

              if(goBack == true)break;
                
              /* Product categories */
              cout << "0. Weapon" << endl
                  << "1. Armor" << endl
                  << "2. Spell" << endl
                  << "3. Go back" << endl
                  << "4. Exit" << endl;

              cin >> select;
              
              int productId;
              
              if(select < 3 && select >= 0){
                /* Select a product from the selected category */
                productId = market->selectItem(select);
                if(productId == -1)continue;
              }

              /* If product exists try to buy */
              cout << "money :" << money<< endl;

              switch(select){
                case Market::weapon:
                case Market::armor:
                  itemTemp = market->sell(select, productId, money, hero->getLevel());
                  if(itemTemp != nullptr)hero->inventoryAdd(itemTemp);
                  break;

                case Market::spell:
                  spellTemp = market->sell(productId, money, hero->getLevel());
                  if(spellTemp != nullptr)hero->learnSpell(spellTemp);
                  break;

                case 3: //Go back
                  goBack = true;
                  break;

                case 4: //Exit
                  return;
                  break;

                default:
                  cout << "Invalid selection !" << endl;
              }
              if(money != hero->getMoney()){
                hero->addMoney(money - hero->getMoney());
              }
              hero->print();
              /*cout << "0. Buy another " << Market::type[select]<< endl
                    << "Any key to Go back" << endl;
              cin >> select;
              if(goBack == true)break;
                cout << "money :" << money<< endl;*/
            }
            break;
          
          /* Sell a product to market the */
          case 2:
            while(1){
              cout << "Select item to sell" << endl;
              hero->checkInventory();
              cin >> select;
              itemTemp = hero->dropItem(select);
              if(itemTemp != nullptr){
                market->buy(itemTemp, money);
                hero->addMoney(money - hero->getMoney());
                hero->checkInventory();
              }
              else cout << "Invalid inventory slot!! " << endl;

              cout << "0. Sell another item " << endl
                   << "Any key to Go back " << endl;

              cin >> select;
              if(select != 0)break; 
            }


            break;

          case 3:
            return;
        }

      }
}


void Game::combat(){
  cout << "enter combat mode" << endl;
}


void Game::setGameState(int blockType){
  cout << "Blocktype: " << blockType << endl;
  switch(blockType){

    case Grid::common:
      if(rand()%3 == 0)this->gameState = this->fighting;
      else this->gameState = this->peace;
      break;

    case Grid::market:
      this->gameState = this->trading;
      break;
  }
}



Market::Market(){
  /* Create different type of weapons */
  this->name = "Default Market";

  this->itemsCounter[weapon] = 6;
  this->weapons = new Weapon *[itemsCounter[weapon]];

  this->weapons[0] = new Weapon("Iron Slasher", 250, 200, 3);
  this->weapons[1] = new Weapon("Iron Sword", 200, 180, 3);
  this->weapons[2] = new Weapon("Iron Staff", 180, 150, 3);
  this->weapons[3] = new Weapon("Silver Slasher", 350, 500, 6);
  this->weapons[4] = new Weapon("Silver Sword", 300, 450, 6);
  this->weapons[5] = new Weapon("Silver Staff", 280, 420, 6);

  this->itemsCounter[armor] = 2;
  this->armors = new Armor *[itemsCounter[armor]];

  this->armors[0] = new Armor("Iron armor", 25, 300, 3);
  this->armors[1] = new Armor("silver armor", 35, 700, 6);

  this->itemsCounter[spell] = 3;
  this->spells = new Spell *[itemsCounter[spell]];
  int spellDmg = 100;
  int mp = 50;
  spells[0] = new IceSpell("Frost attack", spellDmg, 20, 100, 1, mp);
  spells[1] = new FireSpell("Inferno blast", spellDmg, 5, 100, 1, mp);
  spells[2] = new LightingSpell("Lighting bold", spellDmg, 5, 100, 1, mp);


  cout << "A new market " << this->name << " Constructed!" << endl;

}


Market::~Market(){

  for(int i = 0; i < this->itemsCounter[weapon]; i++)delete weapons[i];
  for(int i = 0; i < this->itemsCounter[armor]; i++)delete armors[i];
  for(int i = 0; i < this->itemsCounter[spell]; i++)delete spells[i];

  delete[] armors;
  delete[] weapons;
  delete[] spells;

  cout << "Market " << name << " to be destructed" << endl;
  
}


int Market::selectItem(int type){
  
  cout << "Select " << this->type[type] << " :" << endl; 
 
  int select; 

  while(1){

    this->showItems(type); 

    cin >> select;

    if(select >= itemsCounter[type] || select < 0){
      cout << this->type[type] << " " << select << " doesn't exist!"<< endl;
      cout << "1. Select again" << endl
           << "Any key to Go back "     << endl;
      cin >> select;
      if(select != 1)return -1;
    }
    else return select;
  }
} 


void Market::showItems(int type){
  
    switch(type){
      case weapon:
        for(int i = 0; i < this->itemsCounter[weapon]; i++){
          cout << "______" << i << "______" << endl;
          weapons[i]->print(); 
          cout << endl;
        }
        break;

      case armor:
        for(int i = 0; i < this->itemsCounter[armor]; i++){
          cout << "__" << i << "__" << endl;
          armors[i]->print(); 
          cout << endl;
        }
        break;

      case spell:
        for(int i = 0; i < this->itemsCounter[spell]; i++){
          cout << "__" << i << "__" << endl;
          spells[i]->print(); 
          cout << endl;
        }
        break;
    }


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
      if(id < this->itemsCounter[this->weapon])item = weapons[id];
      break;
    case this->armor:
      if(id < this->itemsCounter[this->armor])item = armors[id];
      break;
  }

  if(item != nullptr){
    if(money >= item->getPrice()){
      if(heroLvl >= item->getLevel()){
        money -= item->getPrice();
        cout << this->type[type] << " " << 
        item->getName() << " successfully purchased!!" << endl;
        return item;
      }
      else cout << item->getName() << " requires level "
          << item->getLevel() << endl;
    }
    else cout << item->getPrice()-money << " gold is missing!" << endl;
  }
  
  return nullptr;

}


Spell* Market::sell(int id, int &money, int heroLvl){
  
  Spell *spell = nullptr;

  if(id < this->itemsCounter[this->spell])spell = spells[id];

  if(spell != nullptr){
    if(money >= spell->getPrice()){
      if(heroLvl >= spell->getLvl()){
        money -= spell->getPrice();
        cout << "Spell " << 
        spell->getName() << " successfully purchased!!" << endl;
        return spell;
      }
      else cout << spell->getName() << " requires level "
          << spell->getLvl() << endl;
    }
    else cout << spell->getPrice()-money << " gold is missing!" << endl;
  }
  
  return nullptr;
}
