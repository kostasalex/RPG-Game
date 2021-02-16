#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include<limits>
#include "game.h"

using namespace std;

string Market::productType[3] = {"weapon", "armor", "spell"};

Game::Game(){
  
  system("clear");

  this->gameState = peace;

  cout <<R"(
           =================================
           ||     Rpg game started!!      || 
           ||                             ||
           ||       /|________________    ||
           || O|===|* >________________>  ||
           ||       \|                    ||
           =================================)" 
  << endl << endl;

  cout << "You can create " << maxHeroes << " heroes maximum.." << endl << endl;

  int select;
  string name;
  Hero *hero;

  this->heroesNum = 0;

  while(heroesNum < maxHeroes){

    if(heroesNum > 0){
      system("clear");
      cout <<R"(
                =================================
                ||     Rpg game started!!      || 
                ||                             ||
                ||       /|________________    ||
                || O|===|* >________________>  ||
                ||       \|                    ||
                =================================)" 
      << endl << endl;
      cout << "[1] Create another hero" << endl
           << "[0] Exit creation." << endl
           << ">";
      cin >> select;


      //*debug*
      //select = 0;
      if(select == 0)break;
    }

    cout << "Select a class :" << endl << endl;
    cout << "[1] Warrior    || A warrior class has additional strength and agility"
         << endl 
         << "               || Strength : " 
         << Warrior::startingStr + Warrior::additionalStr << endl
         << "               || Dexterity : "
         << Warrior::startingDex << endl
         << "               || Agility : "
         << Warrior::startingAgi + Warrior::additionalAgi << endl << endl;
    
    cout << "[2] Sorcerer    || A Sorcerer class has additional dexterity and agility"
         << endl 
         << "               || Strength : " 
         << Sorcerer::startingStr << endl
         << "               || Dexterity : "
         << Sorcerer::startingDex + Sorcerer::additionalDex << endl
         << "               || Agility : "
         << Sorcerer::startingAgi + Sorcerer::additionalAgi << endl << endl;
    
    cout << "[3] Paladin    || A Paladin class has additional strength and dexterity"
         << endl 
         << "               || Strength : " 
         << Paladin::startingStr + Paladin::additionalStr << endl
         << "               || Dexterity : "
         << Paladin::startingDex + Paladin::additionalDex << endl
         << "               || Agility : "
         << Paladin::startingAgi << endl << endl
         << ">";

    cin >> select;
    /*while(1)
    {
      if(cin.fail())
      {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<< "You have entered wrong input" <<endl;
        cin>>select;
      }
      if(!cin.fail())
      break;
    }*/
    //*debug
    //select = 1;
    if(select < 1 || select > 3){
      cout << "Invalid number!"<< endl;
      continue;
    }
    
    cout << "Enter name: ";
    
    cin >> name;
    cout << endl;
    //*Debug
    //name = "asapis";

    switch(select){
      case 1:
        hero = new Warrior(name);

        //*debug
        //this->heroes.push_back(hero);
        //this->heroesNum++;
        //hero = new Warrior("dvp");
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
  

  int input, heroIndex;

  system("clear");
  
    //*debug
  Hero *heroes[this->heroesNum];
  for(int i = 0; i < this->heroesNum; i++){
    heroes[i] = this->heroes[i];
  }

  while(1){

    //system("clear");

    cout << "[1] Move " << endl
         << "[2] Select A hero "<< endl
         << "[0] Exit game " << endl;

    cin >> input;

    if(input == 0)return;

    if(input == 1){
      grid->displayMap();
      if(move() == false)continue;
      grid->displayMap();
      setGameState(grid->getBlockType());
      cout << "** " << this->gameState <<" **" << endl;
      switch(this->gameState){
        case trading:
          while(1){
            system("clear");
            cout << "[1] Select a hero to enter market " << endl
                << "[0] Go back " << endl;
            
            cin >> input;

            if(input == 1){
              for(int i = 0; i < this->heroesNum; i++)
                cout << "[" << i << "] : " << heroes[i]->getName() << endl;

              cin >> heroIndex;

              if(heroIndex < 0 && heroIndex > this->heroesNum){
                cout << "Invalid selection.." << endl;
                continue;
              }
              this->shop(heroes[heroIndex]);
              system("clear");
            }

          }

          break;
        case fighting:
          system("clear");
          cout << "Monsters appeared! " << endl;
          this->combat = new Combat(heroes, heroesNum);
          combat->fight();
          sleep(2);
          cout <<"Combat ended! " << endl;
      }
      //system("clear");
      cout << endl
      << "===============================================================" 
      << endl << endl;
    }

    else if(input == 2){
      for(int i = 0; i < this->heroesNum; i++)
        cout << "[" << i << "] : " << heroes[i]->getName() << endl;

      cin >> heroIndex;
      
      if(heroIndex < 0 && heroIndex > this->heroesNum){
        cout << "Invalid selection.." << endl;
        continue;
      }

      while(1){
        cout << "[1] Check inventory " << endl
             << "[2] Check stats "<< endl
             << "[0] Go back " << endl;

        cin >> input;

        if(input < 0 && input > 2){
          cout << "Invalid selection.." << endl;
          continue;
        }
        if(input == 0)break;

        system("clear");

        if(input == 2){

          cout << heroes[heroIndex]->getName() << endl;
          heroes[heroIndex]->checkStats();
          cout << endl;
        }
        else if(input == 1){
          cout << heroes[heroIndex]->getName() << endl;
          heroes[heroIndex]->checkInventory();
          cout << endl;
          cout << "[1] Equip item " << endl
                << "Any other number to Go back " << endl;

          cin >> input;

          if(input == 1){
            cout << "Enter inventory slot: ";
            cin >> input;
            heroes[heroIndex]->equip(input);
          }
        }
      }
    }
  }
}


bool Game::move(void){
  bool validLoc = false;
  char input;

  cout << "Where do you want to go? [w] = up, [s] = down, [a] = left, [d] = right"
      << endl << ">";

  cin >> input; 

  for(int i = 0; i < 4; i++){
    if(input == this->direction[i]){
      validLoc = grid->move(this->direction[i]);
      break;
    }
  }
  if(validLoc == false)
    cout << "Oops couldn't move there.." << endl;

  return validLoc;
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
                productId = market->selectProduct(select);
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


/*void Game::combat(){
  cout << "enter combat mode" << endl;
}*/


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

  this->productCounter[weapon] = 6;
  this->weapons = new Weapon *[productCounter[weapon]];

  this->weapons[0] = new Weapon("Iron Slasher", 250, 200, 3);
  this->weapons[1] = new Weapon("Iron Sword", 200, 180, 3);
  this->weapons[2] = new Weapon("Iron Staff", 180, 150, 3);
  this->weapons[3] = new Weapon("Silver Slasher", 350, 500, 6);
  this->weapons[4] = new Weapon("Silver Sword", 300, 450, 6);
  this->weapons[5] = new Weapon("Silver Staff", 280, 420, 6);

  this->productCounter[armor] = 2;
  this->armors = new Armor *[productCounter[armor]];

  this->armors[0] = new Armor("Iron armor", 25, 300, 3);
  this->armors[1] = new Armor("silver armor", 35, 700, 6);

  this->productCounter[spell] = 3;
  this->spells = new Spell *[productCounter[spell]];
  int spellDmg = 100;
  int mp = 50;
  spells[0] = new IceSpell("Frost attack", spellDmg, 20, 100, 1, mp);
  spells[1] = new FireSpell("Inferno blast", spellDmg, 5, 100, 1, mp);
  spells[2] = new LightingSpell("Lighting bold", spellDmg, 5, 100, 1, mp);


  cout << "A new market " << this->name << " Constructed!" << endl;

}


Market::~Market(){

  for(int i = 0; i < this->productCounter[weapon]; i++)delete weapons[i];
  for(int i = 0; i < this->productCounter[armor]; i++)delete armors[i];
  for(int i = 0; i < this->productCounter[spell]; i++)delete spells[i];

  delete[] armors;
  delete[] weapons;
  delete[] spells;

  cout << "Market " << name << " to be destructed" << endl;
  
}


int Market::selectProduct(int type){
  
  cout << "Select " << this->productType[type] << " :" << endl; 
 
  int select; 

  while(1){

    this->showItems(type); 

    cin >> select;

    if(select >= productCounter[type] || select < 0){
      cout << this->productType[type] << " " << select << " doesn't exist!"<< endl;
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
        for(int i = 0; i < this->productCounter[weapon]; i++){
          cout << "______" << i << "______" << endl;
          weapons[i]->print(); 
          cout << endl;
        }
        break;

      case armor:
        for(int i = 0; i < this->productCounter[armor]; i++){
          cout << "__" << i << "__" << endl;
          armors[i]->print(); 
          cout << endl;
        }
        break;

      case spell:
        for(int i = 0; i < this->productCounter[spell]; i++){
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
      if(id < this->productCounter[this->weapon])item = weapons[id];
      break;
    case this->armor:
      if(id < this->productCounter[this->armor])item = armors[id];
      break;
  }

  if(item != nullptr){
    if(money >= item->getPrice()){
      if(heroLvl >= item->getLevel()){
        money -= item->getPrice();
        cout << this->productType[type] << " " << 
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

  if(id < this->productCounter[this->spell])spell = spells[id];

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



Combat::Combat(Hero **heroes, int heroesNum){

    this->round = 0;

    this->heroes = heroes;
    this->heroesNum = heroesNum;

    /*Create same number of monsters*/
    this->monstersNum = heroesNum;
    this->monsters = new Monster*[heroesNum];

    for(int i = 0; i < this->monstersNum; i++){
      //*Temporary just one type of monster
      this->monsters[i] = new Dragon("Dragon" + to_string(i));
    }
    
    cout << "A new combat is about to start ..!!" << endl;

}


Combat::~Combat(){

    for(int i = 0; i < this->monstersNum; i++){
      delete this->monsters[i];
    }

    delete[] this->monsters;
}


void Combat::fight(void){

  while(fightResult() == stillFighting){
     
    cout << endl 
    << "===============================================================" 
    <<  endl << endl <<"                  ***    Round "
    << this->round << "   ***" << endl << endl;
    cout
    << "===================== Heroes turn =============================" 
    << endl;
    heroesTurn();

    cout << endl;
    cout
    << "==================== Monsters turn ============================" 
    << endl;
    monstersTurn();
    cout << endl;

    cout 
    << "++++++++++++++++++++ Regeneration +++++++++++++++++++++++++++++"
    << endl;

    regeneration();

  }
  cout << "Fight ended! " << endl;

  reviveHeroes();

  if(fightResult() == heroesWon){
    cout << "Heroes won !" << endl
    << "+++++++++++++++++++ Combat Rewards ++++++++++++++++++++++++++++"
    << endl;

    receiveRewards();
  }
  else if(fightResult() == monstersWon){
    cout << "Heroes lost !" << endl
    << "++++++++++++++++++++ Regeneration +++++++++++++++++++++++++++++"
    << endl;
    receivePenalty();
  }

  for(int i = 0; i < heroesNum; i++){
    heroes[i]->print();
    cout << endl;
  }
  for(int i = 0; i < monstersNum; i++){
    monsters[i]->print();
    cout << endl;
  }

}


void Combat::heroesTurn(void){
  
  int availableHeroes[this->heroesNum] = {};
  
  int select, heroIndex, monsterIndex, spellIndex; 
  bool isAvailable, isAlive, isMonsterAlive;

  while(1){ //Menu 1



    /* Check if monsters are still alive */
    isMonsterAlive = false;

    for(int i = 0; i < this->monstersNum; i++)
      if(monsters[i]->getHp() > 0)isMonsterAlive = true;

    if(isMonsterAlive == false)return;

    /* Check if heroes are still available and alive*/
    isAvailable = false;
    isAlive = false;
    for(int i = 0; i < this->heroesNum; i++){
      if(heroes[i]->getHp() != 0) isAlive = true;
  
      if(availableHeroes[i] == 0) isAvailable = true;
    }


    if(isAvailable == false || isAlive == false)return;

    cout << "Select a hero " << endl;

    for(int i = 0; i < this->heroesNum; i++){
      if(availableHeroes[i])continue;
      cout << i << " : " << heroes[i]->getName() << endl;
    }

    cin >> heroIndex;

    if(heroIndex >= 0 && heroIndex < this->heroesNum){
      if(heroes[heroIndex]->getHp() == 0){
        cout << heroes[heroIndex]->getName() 
             << " cant fight .. is Unconscious ..!" << endl;
        continue;   //Select another hero
      }
      else{
        if(availableHeroes[heroIndex]){
          cout << heroes[heroIndex]->getName() <<" allready fought" << endl;
          continue;
        }
      }
    }
    else continue; //Invalid input



    while(1){ //>Menu 2
      if(availableHeroes[heroIndex])break;
      cout << "0. Check inventory " << endl
          << "1. Select a monster to attack " << endl
          << "2. Display " << heroes[heroIndex]->getName() << endl
          << "3. Select another hero" << endl;

      cin >> select;
      if(select == 3)break;
      if(select > 3 && select < 0){
        cout << "Invalid selection!!" << endl;
        continue;
      }

      if(select == 0){
        heroes[heroIndex]->checkInventory();
        cout << endl;
      }
      else if(select == 1){
        
        while(1){ //>>Menu 3
          if(availableHeroes[heroIndex])break;
          cout << "Select a monster " << endl;

          for(int i = 0; i < this->monstersNum; i++){
            if(monsters[i]->getHp() == 0)continue;
            cout << i << " : " << monsters[i]->getName() << endl;
          }


          cin >> monsterIndex;

          if(monsterIndex < 0 || monsterIndex >= this->monstersNum){
            cout << "Invalid selection!!" << endl;
            continue;
          }

          if(monsters[monsterIndex]->getHp() == 0){
            cout << monsters[monsterIndex]->getName() 
                << " cant fight back .. is Unconscious ..!" << endl;
                continue;
          } 
          while(1){
            if(availableHeroes[heroIndex])break;

            cout << "1. Attack" << endl
                 << "2. Cast Spell" << endl
                 << "3. Go back" << endl;

            cin >> select;

            if(select == 3)break;

            if(select > 2 && select < 0){
              cout << "Invalid selection!!" << endl;
              continue;
            }
            if(select == 1)heroes[heroIndex]->attack(monsters[monsterIndex]);
            else if(select == 2){
              
              cout << "Select spell" << endl;
              heroes[heroIndex]->checkSpells();
              cin >> spellIndex;

              if(heroes[heroIndex]->castSpell(spellIndex, monsters[monsterIndex])\
               != Hero::succeed)continue;
            }

            availableHeroes[heroIndex]++;
          }//>>Menu 3

        }

      }
      else if(select == 2){
        heroes[heroIndex]->print();
        cout << endl;
      }

    }//>Menu 2
  cout << endl << endl;
  }//>Menu 1

}


void Combat::monstersTurn(void){

  int target;

  for(int i = 0; i < this->monstersNum; i++){

    if(monsters[i]->getHp() == 0)continue;

    target = getRandTarget();
    
    cout << this->monsters[i]->getName()
         << " targeted " << this->heroes[target]->getName() << endl << endl;
    sleep(1);
    this->monsters[i]->attack(heroes[target]);
    cout << endl;
    sleep(1);
  }

  this->round++;
}


int Combat::getRandTarget(void){
  int target;
  //*debug
  cout << "Monster selecting a target.." << endl;
  while(1){
    target = rand()%this->heroesNum;
    if(this->heroes[target]->getHp() != 0)break;
  }
  return target;
}


void Combat::regeneration(){

  double regenRate = 0.05;

  for(int i = 0; i < this->heroesNum; i++){
    if(this->heroes[i]->getHp() != 0){
      cout << heroes[i]->getName() << " hp regen: "
           << this->heroes[i]->getHp() << "-->";

      heroes[i]->addHp(this->heroes[i]->getMaxHp()*regenRate);
      cout << this->heroes[i]->getHp() << endl;

      cout << "mp regen: " << this->heroes[i]->getMp() << "-->";

      heroes[i]->addMp(this->heroes[i]->getMaxMp()*regenRate);
      cout << this->heroes[i]->getMp() << endl << endl;
    }
    else  cout << heroes[i]->getName() << " hp: Unconscious!" << endl << endl;
  }

  for(int i = 0; i < this->monstersNum; i++){
    if(this->monsters[i]->getHp() != 0){
      cout << monsters[i]->getName() << " hp regen: "
           << this->monsters[i]->getHp() << "-->";

      monsters[i]->addHp(this->monsters[i]->getMaxHp()*regenRate);
      cout << this->monsters[i]->getHp() << endl << endl;
    }
    else  cout << monsters[i]->getName() << " hp: Unconscious!" << endl << endl;
  }

}


int Combat::fightResult(){

  bool monstersDead = true;
  bool heroesDead = true;

  for(int i = 0; i < this->heroesNum; i++){
    if(this->heroes[i]->getHp() != 0){
      heroesDead = false;
      break;
    }
  }

  for(int i = 0; i < this->monstersNum; i++){
    if(this->monsters[i]->getHp() != 0){
      monstersDead = false;
      break;
    }
  }

  if(heroesDead == true)return monstersWon;
  else if(monstersDead == true)return heroesWon;
  else return stillFighting;

}


void Combat::receiveRewards(void){

  int money;
  Item *item;

  for(int i = 0; i < this->heroesNum; i++){

    heroes[i]->receiveExperience(gainExp(heroes[i]->getLevel()));

    money = gainMoney(heroes[i]->getLevel());
    cout << heroes[i]->getName() << " picked up " 
         << money << " gold!!" << endl << endl;

    heroes[i]->addMoney(money);

    if((item = gainItem())!= nullptr){
      cout << heroes[i]->getName() << " picked up " 
            << item->getName() << "  !!!" << endl << endl;
      heroes[i]->inventoryAdd(item);
    }
  }

}


int Combat::gainMoney(int heroLvl)
{
  int result = 15;
  
  result += (result * (0.20 * heroLvl));
  result += rand()%4;
  result *= this->monstersNum;

  return result;
}


Item* Combat::gainItem(){

  Item *item = nullptr;

  if(rand() % 50 == 1){
    item = new Weapon("Power Sword", 220, 400, 1);
  }

  return item;

}


int Combat::gainExp(int heroLvl)
{
  int result = 10;
  
  result -= (result * (0.02 * heroLvl));
  result += rand()%4;
  result *= this->monstersNum;

  return result;
}


void Combat::reviveHeroes(void){

  for(int i = 0; i < this->heroesNum; i++){

    cout << heroes[i]->getName() << " hp: "
          << this->heroes[i]->getHp() << "-->";
    heroes[i]->addHp(heroes[i]->getMaxHp()/2);
    cout << this->heroes[i]->getHp() << endl << endl;


  }

}


void Combat::receivePenalty(void){
  
  int moneyLoss;

  for(int i = 0; i < heroesNum; i++){

    moneyLoss = heroes[i]->getMoney() / 2;

    cout << heroes[i]->getName() << " lost " 
         << moneyLoss << " gold.." << endl << endl;

    heroes[i]->subMoney(moneyLoss);
  }

}