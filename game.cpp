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

  int input;
  int options[5] = {0,1,2,3,4};
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


      while(inputHandler(input, options, 2) == false);

      //*debug*
      //select = 0;
      if(input == 0)break;
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
    
    cout << "[2] Sorcerer   || A Sorcerer class has additional dexterity and agility"
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

    while(inputHandler(input, &options[1], 3) == false);

    //*debug
    //select = 1;
    
    cout << "Enter name: ";
    
    cin >> name;
    cout << endl;
    //*Debug
    //name = "asapis";

    switch(input){
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
  

  int input, heroIndex, options[5] = {0,1,2,3,4};

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
         << "[0] Exit game " << endl
         << ">";


    while(inputHandler(input, options, 3) == false);
    
    switch(input){
      
      case 0: //Exit game
        return;
      
      case 1: //Move through grid

        grid->displayMap();
        if(move() == false)continue;
        grid->displayMap();
      
        setGameState(grid->getBlockType());
        switch(this->gameState){
          case trading:
            while(1){
              system("clear");
              if(this->heroesNum == 1)heroIndex = 0;
              else{
                cout << "[1] Select a hero to enter market " << endl
                    << "[0] Go back " << endl
                    << ">";
                
                while(inputHandler(input, options, 2) == false);

                if(input == 1){
                  for(int i = 0; i < this->heroesNum; i++)
                    cout << "[" << i << "] : " << heroes[i]->getName() << endl;

                  cout << "Hero: ";
                  while(inputHandler(heroIndex, options, this->heroesNum) == false);
                }
                else break;
              }
              this->shop(heroes[heroIndex]);
              system("clear");
              break;
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
      
      break;

      case 2:
        for(int i = 0; i < this->heroesNum; i++)
          cout << "[" << i << "] : " << heroes[i]->getName() << endl;
        
        cout << "Hero:";
        while(inputHandler(heroIndex, options, this->heroesNum) == false);
   
        while(1){
          cout << "[1] Check inventory " << endl
              << "[2] Check stats "<< endl
              << "[0] Go back " << endl
              << ">";

          while(inputHandler(input, options, 3) == false);

          if(input == 0)break;

          system("clear");

          if(input == 2)
          {
            cout << heroes[heroIndex]->getName() << endl;
            heroes[heroIndex]->checkStats();
            cout << endl;
          }

          else if(input == 1)
          {
            cout << heroes[heroIndex]->getName() << endl;
            heroes[heroIndex]->checkInventory();
            cout << endl;
            cout << "[1] Equip item " << endl
                  << "[0] Go back " << endl
                  << ">";

            while(inputHandler(input, options, 2) == false);

            if(input == 1){
              cout << "Enter inventory slot: ";

              int inventorySize = heroes[heroIndex]->getInventorySize();
              int inventorySlots[inventorySize];
              for(int i = 0; i < inventorySize; i++)inventorySlots[i] = i;

              while(inputHandler(input, inventorySlots, inventorySize) == false);

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
      << endl << "Direction: ";
  
  while(inputHandler(input, direction, 4) == false);

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
  
  int options[5] = {0,1,2,3,4};
  int money, select;

  cout << "Welcome to my store!!" << endl
       << "What are you looking for?" << endl;
       

      Spell *spellTemp = nullptr;
      Item *itemTemp = nullptr;


      money = hero->getMoney();
      bool goBack;

       while(1){
        
        cout << "1. Buy " << endl
             << "2. Sell" << endl
             << "3. Exit" << endl
             <<">";

        while(inputHandler(select,&options[1], 3) == false);

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
                  << "4. Exit" << endl
                  << ">" ;

              while(inputHandler(select, options, 5) == false);
              
              int productId;
              
              if(select == 3 )break; // Go back
              
              else if(select == 4)return; // Exit
              
              else{ // 0 - 2: Select a product from the selected category 
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
              }
              if(money != hero->getMoney()){
                hero->addMoney(money - hero->getMoney());
              }
            }
            break;
          
          /* Sell a product to market the */
          case 2:
            if(hero->getInventorySize() == 0){
              cout << "Your inventory is empty!" << endl;
              continue;
            }
            while(1){
              cout << "Select item to sell" << endl;
              hero->checkInventory();

              int inventorySize = hero->getInventorySize();
              int inventorySlots[inventorySize];
              for(int i = 0; i < inventorySize; i++)inventorySlots[i] = i;

              while(inputHandler(select, inventorySlots, inventorySize) == false){
                cout << "1. Select another item " << endl;
                cout << "0. Go back " << endl;
                cout <<" >"<< endl;

                while(inputHandler(select, options, 2) == false);
                if(select == 0)break;
                  
              }

              itemTemp = hero->dropItem(select);
              if(itemTemp != nullptr){
                market->buy(itemTemp, money);
                hero->addMoney(money - hero->getMoney());
                hero->checkInventory();
              }
              else cout << "Invalid inventory slot!! " << endl;

              cout << "1. Sell another item " << endl
                   << "0. Go back " << endl
                   << ">";
              while(inputHandler(select, options, 2) == false);
              if(select == 0)break; 
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

  int options[2] = {0,1};

  this->showItems(type); 

  int productSlots[productCounter[type]];
  for(int i = 0; i < productCounter[type]; i++)productSlots[i] = i;
  
  while(inputHandler(select, productSlots, productCounter[type]) == false){
    cout << "1. Select another item " << endl
         << "0. Go back " << endl
         <<">";
    while(inputHandler(select, options, 2) == false);
    if(select == 0){
      select = -1;
      break;
    }
  }

  return select;
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

  enum availability{ available, notAvailable};
  
  int availableHeroesCount, availableMobsCount, availableHeroes[this->heroesNum] = {};

  /* Keep arrays with valid options */
  int options[5] = {0, 1, 2 ,3 ,4};

  int selectHero[heroesNum];
  for(int i = 0; i < heroesNum; i++)selectHero[i] = i;
  
  int selectMonster[monstersNum];
  for(int i = 0; i < monstersNum; i++)selectMonster[i] = i;

  int spellsNum;
  int select;
  int heroIndex, monsterIndex, spellIndex;


  while(1){ //Menu 1

    availableMobsCount = availableHeroesCount = 0;

    /* Check if monsters are still alive */
    for(int i = 0; i < this->monstersNum; i++){
      if(monsters[i]->getHp() > 0){
        monsterIndex = i;
        availableMobsCount++;
      }
    }


    if(availableMobsCount == 0)return;

    /* Check if heroes are still available and alive*/
    for(int i = 0; i < this->heroesNum; i++){
      if(heroes[i]->getHp() != 0){
        if(availableHeroes[i] == available){
          availableHeroesCount++;
          heroIndex = i; //In case of one hero keep index
        }
      } 
    }


    if(availableHeroesCount == 0)return;

    /* If available heroes are more than 1 , select a hero.*/
    else if(availableHeroesCount > 1){
    
      cout << "Select a hero " << endl;

      for(int i = 0; i < this->heroesNum; i++){
        if(availableHeroes[i])continue;
        cout << i << " : " << heroes[i]->getName() << endl;
      }
      cout << "Hero:";

      while(inputHandler(heroIndex, selectHero, heroesNum) == false);

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
    else cout << "Preselected Hero: " << heroes[heroIndex]->getName() << endl;


    while(1){ //>Menu 2
      if(availableHeroes[heroIndex])break; //Go back

      cout << "0. Check inventory " << endl
          << "1. Select a monster to attack " << endl
          << "2. Display " << heroes[heroIndex]->getName() << endl
          << "3. Select another hero" << endl
          << ">";

      while(inputHandler(select, options, 4) == false);

      if(select == 3)break;


      if(select == 0){
        heroes[heroIndex]->checkInventory();
        cout << endl;
      }
      else if(select == 1){
        
        while(1){ //>>Menu 3
          if(availableHeroes[heroIndex])break;//Go back

          else if(availableMobsCount > 1){
          
            cout << "Select a monster " << endl;

            for(int i = 0; i < this->monstersNum; i++){
              if(monsters[i]->getHp() == 0)continue;
              cout << i << " : " << monsters[i]->getName() << endl;
            }


            cout << "Monter:";

            while(inputHandler(monsterIndex, selectMonster, monstersNum) == false);

            if(monsters[monsterIndex]->getHp() == 0){
              cout << monsters[monsterIndex]->getName() 
                  << " cant fight back .. is Unconscious ..!" << endl;
                  continue;
            } 
          }
          else cout << "Preselected Monster: " 
                << monsters[monsterIndex]->getName() << endl;

          while(1){
            if(availableHeroes[heroIndex])break; //Go back

            cout << "1. Attack" << endl
                 << "2. Cast Spell" << endl
                 << "3. Go back" << endl
                 << ">";

            while(inputHandler(select, options, 4) == false);

            if(select == 3)break;
            if(select == 1)heroes[heroIndex]->attack(monsters[monsterIndex]);
            else if(select == 2){
              
              spellsNum = heroes[heroIndex]->getSpellNum();
              if(spellsNum == 0){
                cout << "There aren't any spells learned yet!" << endl;
                continue;
              }
              
              cout << "Select a spell" << endl;
              heroes[heroIndex]->checkSpells();

              int selectSpell[spellsNum];
              for(int i = 0; i < spellsNum; i++)selectSpell[i] = i;
              cout << "Spell: ";

              while(inputHandler(spellIndex, selectSpell, spellsNum) == false){
                cout << "1. Select another spell " << endl;
                cout << "0. Go back " << endl;
                cout <<" >"<< endl;

                while(inputHandler(select, options, 2) == false);
                if(select == 0)break;
              }
              if(select == 0)continue; //Go back

              if(heroes[heroIndex]->castSpell(spellIndex, monsters[monsterIndex])\
               != Hero::succeed)continue;
            }
            //If hero use spell or attack , 
            //his turn is ended for the current round
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

template<typename T>
bool inputHandler(T &input, T options[], int size){

  cin >> input;

  while(1)
  {
    if(cin.fail())
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(),'\n');
      cout << "Invalid option.." <<endl;
      cin >> input;
    }
    if(!cin.fail()){
      for(int i = 0; i < size; i++){
        if(options[i] == input)return true;
      }
      cout << "Wrong option.." <<endl;
      break;
    }
  }
  return false;
}


