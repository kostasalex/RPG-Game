#include <cstdlib>
#include <limits.h>
#include "grid.h"

using namespace std;


/* Grid implementation */
const char Grid::direction[5] = {'w', 's', 'a', 'd', 'b'};

const char Grid::defaultMap[defaultHeight][defaultWidth] = 
                                      {{' ', ' ', ' ', '#'},
                                      {' ', ' ',' ', '#'},
                                      {' ', ' ', 'M', '#'},
                                      {' ', ' ', ' ', '#'}};

Grid::Grid(){

    /* Create the different type of blocks */
    marketsNum = 1;

    markets = new Market*[marketsNum];

    markets[0] = new Market();

    nonAccessive = new NonAccessive();

    common = new Common();


    /* Load default map */
    this->height = defaultHeight;
    this->width = defaultWidth;


    /* Create grid with the required block types */
    blocks = new Block**[height];
    for(int i = 0; i < height; i++){

        blocks[i] = new Block*[width];

        for(int j = 0; j < width; j++){

            if(defaultMap[i][j] == blockTypes[0])
                blocks[i][j] = common;

            else if(defaultMap[i][j] == blockTypes[1])
                blocks[i][j] = markets[0];
                
            else blocks[i][j] = nonAccessive;
    
        }
    }

    string gameLogo[7] = {
        "=================================",
        "||     Rpg game started!!      ||",
        "||                             ||",
        "||       /|________________    ||",
        "|| O|===|* >________________>  ||",
        "||       \\|                    ||",
        "================================="
    };
    for(int i = 0; i < 7; i++){
        cout << gameLogo[i] << endl;
    }
    /* Create heroes */

    cout << "You can create " << maxHeroes << " heroes maximum.." << endl << endl;

    int input;
    string name;
    Hero *hero;

    heroesLoc = {};
    heroesNum = 0;

    while(heroesNum < maxHeroes){

        if(heroesNum > 0){
            system("clear");

            /* Print logo */
            for(int i = 0; i < 7; i++){
                cout << gameLogo[i] << endl;
            }

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
            << Paladin::startingAgi << endl << endl;

        if(heroesNum > 0){
            cout << "[0] Exit hero creation" << endl
                 << ">";
            while(inputHandler(input, options, 4) == false);
        }
        else{
            cout << ">";
            while(inputHandler(input, &options[1], 3) == false);
        } 



        if(input != 0){
            cout << "Enter name: ";
            
            cin >> name;
            cout << endl;

            switch(input){
            case 1:
                hero = new Warrior(name);
                break;
            
            case 2:
                hero = new Sorcerer(name);
                break;

            case 3:
                hero = new Paladin(name);
                break;
            }
            heroes[heroesNum] = hero;
            heroesNum++;
        }

    }

    cout << heroesNum <<" Heroes created: " << endl << endl;
    for(int i = 0; i < heroesNum; i++){
        heroes[i]->displayStats();
        cout << endl;
    }
}


Grid::~Grid(){



}


void Grid::play(void){

    int input, heroIndex;

    while(1){
        system("clear");
        displayMap();
        cout << "[1] Move " << endl
            << "[2] Select A hero "<< endl
            << "[0] Exit game " << endl
            << ">";


        while(inputHandler(input, options, 3) == false);

        switch(input){
            case 0: //Exit game
                return;

            case 1:
                //is valid direction
                if(move() == true){
                    if(blocks[heroesLoc.x][heroesLoc.y]->\
                    access(heroes, heroesNum)){
                        blocks[heroesLoc.x][heroesLoc.y]->interactWith();
                        blocks[heroesLoc.x][heroesLoc.y]->leaveBlock();            
                    }
                }
                break;

            case 2: //Select a hero to check inventory / stats
                if(this->heroesNum > 1){
                    for(int i = 0; i < this->heroesNum; i++)
                        cout << "[" << i << "] : " << heroes[i]->getName()
                             << endl;
                    
                    cout << "Hero:";
                    while(inputHandler(heroIndex, options, this->heroesNum)
                     == false);
                }
                else{
                    heroIndex = 0;
                    cout << "Preselected hero " << heroes[heroIndex]->getName() 
                        << endl;
                }
                while(1){
                    cout << "[1] Check inventory " << endl
                        << "[2] Display stats "<< endl
                        << "[0] Go back " << endl
                        << ">";

                    while(inputHandler(input, options, 3) == false);

                    if(input == 0){
                        break;
                    }

                    system("clear");

                    if(input == 2)
                    {
                        heroes[heroIndex]->displayStats();
                        cout << endl;
                    }
                    else if(input == 1)
                    {
                        heroes[heroIndex]->checkInventory(true);
                    }
                }
                break;
            //End of case 2 select hero.
        }   

    }

}


bool Grid::move(void){

    bool inGrid = false;
    int x, y;
    char input;

    while(1){

        cout << "Where do you want to go? [w] = up, [s] = down, [a] = left, [d] = right"
            << endl << "Or 'b' to go back" << endl
            << "Direction: ";

        while(inputHandler(input, this->direction, 5) == false);
        cout << endl;

        /* Check if the requested location is inside the grid */
        switch(input){
            case 'w':
                if((heroesLoc.x - 1) >= 0){
                    x = heroesLoc.x - 1; y = heroesLoc.y;
                    inGrid = true;
                }
                break;

            case 's':
                if((heroesLoc.x + 1) < height ){
                    x = heroesLoc.x + 1; y = heroesLoc.y;
                    inGrid = true;
                }
                break;

            case 'd':
                if((heroesLoc.y + 1) < width ){
                    x = heroesLoc.x; y = heroesLoc.y + 1;
                    inGrid = true;
                }
                break;

            case 'a':
                if((heroesLoc.y - 1) >= 0){
                    x = heroesLoc.x; y = heroesLoc.y - 1;
                    inGrid = true;
                }
                break;
            case 'b':
                return false;
        }

        /* Check if new block is accessible */ 
        if(inGrid == true){  //If yes change heroes location
            if(blocks[x][y]->access(heroes, heroesNum) == true){
                heroesLoc.setLoc(x,y);
                return true;
            }
        }
        cout << "Oops couldn't move there.." << endl;
    }//End of while(1)


    return false;
}


void Grid::displayMap(void) const{

    string block[5] = {"[__]","[_M]","[xx]", "[H_]", "[HM]"};

    for(int i = 0; i < this->height; i++){
        for(int j = 0; j < this->width; j++){
            switch(defaultMap[i][j]){
                case ' ':
                    if(heroesLoc.isSame(i,j) == true){
                        cout << block[3];
                    }
                    else cout << block[0];
                    break;
                case 'M':
                    if(heroesLoc.isSame(i,j) == true){
                        cout << block[4];
                    }
                    else cout << block[1];
                    break;
                case '#':
                    cout << block[2];
                    break;
            }
        }
        cout << endl;
    }

    cout << endl;
}
/****************************************************************************/


Block::~Block(){}



/* Block Market implementation */
string Market::productType[4] = {"weapon", "armor", "spell", "potion"};

Market::Market(){
  /* Create different type of weapons */
  this->name = "Default Market";

  this->productCounter[weapon] = 6;
  this->weapons = new Weapon *[productCounter[weapon]];

  this->weapons[0] = new Weapon("Iron Slasher", 250, 200, 3, Weapon::twoHanded);
  this->weapons[1] = new Weapon("Iron Sword", 200, 180, 3, Weapon::oneHanded);
  this->weapons[2] = new Weapon("Iron Staff", 180, 150, 3, Weapon::oneHanded);
  this->weapons[3] = new Weapon("Silver Slasher", 350, 500, 6, Weapon::twoHanded);
  this->weapons[4] = new Weapon("Silver Sword", 300, 450, 6, Weapon::oneHanded);
  this->weapons[5] = new Weapon("Silver Staff", 280, 420, 6, Weapon::oneHanded);

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

  this->productCounter[potion] = 3;

  this->potions = new Potion *[productCounter[potion]];
  potions[0] = new Potion("Power potion", Potion::strength, 20, 5, 1);
  potions[1] = new Potion("Magic potion", Potion::dexterity, 20, 5, 1);
  potions[2] = new Potion("Stealth potion", Potion::agility, 20, 5, 1);

  cout << "A new market " << this->name << " Constructed!" << endl;

}


Market::~Market(){

  for(int i = 0; i < this->productCounter[weapon]; i++)delete weapons[i];
  for(int i = 0; i < this->productCounter[armor]; i++)delete armors[i];
  for(int i = 0; i < this->productCounter[spell]; i++)delete spells[i];
  for(int i = 0; i < this->productCounter[potion]; i++)delete potions[i];

  delete[] armors;
  delete[] weapons;
  delete[] spells;
  delete[] potions;

  cout << "Market " << name << " to be destructed" << endl;
  
}


void Market::interactWith(void){

    if(heroes == nullptr){
        cout << "There is no heroes in Market block\n";
        return;
    }

    int money, select, heroIndex, input;
    bool goBack;
    Hero *hero;

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
        else break; //Go back
        }
        system("clear");
        
        cout << "Welcome to my store!!" << endl
            << "What are you looking for?" << endl;
            
        hero = heroes[heroIndex];
        while(1){
                
            cout << "1. Buy " << endl
                << "2. Sell" << endl
                << "3. Exit store" << endl
                <<"0. Go back" << endl
                <<">";

            while(inputHandler(select,options, 3) == false);

            if(select == 0)break;
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
                        << "3. Potion" << endl
                        << "4. Go back" << endl
                        << "5. Exit store" << endl
                        << ">" ;

                    while(inputHandler(select, options, 6) == false);
                    
                    int productId;
                    
                    if(select == 4 )break; // Go back
                    
                    else if(select == 5)return; // Exit
                    
                    else{ // 0 - 2: Select a product from the selected category 
                        productId = selectProduct(select);
                        if(productId == -1)continue;
                    }

                    /* If product exists try to buy */
                    int productType = select;

                    switch(select){
                        case Market::weapon:
                        case Market::armor:
                        case Market::potion: //Any item:
                        hero->buy(\
                            sell(productType, productId, money, hero->getLevel()));
                        break;

                        case Market::spell:
                        
                        hero->buy(\
                            sell(productId, money, hero->getLevel()));
                        break;

                        case 4: //Go back
                        goBack = true;
                        break;
                    }
                }
                break;
                //End of case 1: buy from market

                /* Sell a product to market the */
            case 2:
                if(hero->getInventorySize() == 0){
                cout << "Your inventory is empty!" << endl;
                continue;
                }
                while(1){
                if(hero->getInventorySize() == 0){
                    cout << "Your inventory is empty!" << endl;
                    break; //Go back
                }

                cout << "Select item to sell" << endl;
                hero->checkInventory();

                int inventorySize = hero->getInventorySize();
                int inventorySlots[inventorySize];
                for(int i = 0; i < inventorySize; i++)inventorySlots[i] = i;

                while(inputHandler(select, inventorySlots, inventorySize) == false){
                    if(hero->getInventorySize() == 0)break;
                    cout << "1. Select another item " << endl;
                    cout << "0. Go back " << endl;

                    while(inputHandler(select, options, 2) == false);
                    break; //Go back
                    
                }
                if(select == 0)break; //Go back
                else if(select == 1)continue; //Select another item

                /* Inventory slot given is valid, sell item */
                int inventorySlot = select;
                hero->pickUp(\
                    buy(\
                        hero->sell(inventorySlot)));
                }
                break;
                //End of case 2: sell item to market

            case 3://Exit store;
                return;
            }
        }
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

      case potion:
        for(int i = 0; i < this->productCounter[potion]; i++){
          cout << "______" << i << "______" << endl;
          potions[i]->print(); 
          cout << endl;
        }
        break;

    }
}


int Market::selectProduct(int type){
  
  cout << "Select " << this->productType[type] << " :" << endl; 
 
  int select; 

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


int Market::buy(Item *item){

  if(item == nullptr)return -1;

  int money = (item->getPrice() / 2);

  cout << item->getName() << " successfully sold!!" << endl;

  return money;
  
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
    case this->potion:
      if(id < this->productCounter[this->potion])item = potions[id];
      break;
  }

  
  return item;

}


Spell* Market::sell(int id, int &money, int heroLvl){
  
  Spell *spell = nullptr;

  if(id < this->productCounter[this->spell])spell = spells[id];

  return spell;

}
/****************************************************************************/


/* Block common implementation */
void Common::interactWith(void){

    if(heroes == nullptr){
        cout << "There is no heroes in Common block\n";
        return;
    }
    
    if(isPeacefull() == true)return;

    initCombat();

    combat();

}


void Common::combat(void){

    while(combatResult() == stillFighting){
        
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
        << "____________________End of round " 
        << this->round <<                      "___________________________"
        << endl;

        endOfRound();

    }
    cout << "Fight ended! " << endl;

    int results = combatResult();
    bool receivePenalty;

    if(results == heroesWon){
        cout << "Heroes won !" << endl
        << "+++++++++++++++++++ Combat Rewards ++++++++++++++++++++++++++++"
        << endl;
        
        receivePenalty = false;
        reviveHeroes(receivePenalty);
        receiveRewards();
    }
    else if(results == monstersWon){
        cout << "Heroes lost !" << endl
        << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
        << endl;
        receivePenalty = true;
        reviveHeroes(receivePenalty);
    }

    for(int i = 0; i < heroesNum; i++){
        heroes[i]->displayStats();
        cout << endl;
    }
    for(int i = 0; i < monstersNum; i++){
        monsters[i]->displayStats();
        cout << endl;
    }
}


void Common::initCombat(void){

    this->round = 0;

    /*Create same number of monsters*/
    this->monstersNum = heroesNum;
    this->monsters = new Monster*[heroesNum];

    int additionalLvl;

    for(int i = 0; i < this->monstersNum; i++){
      //*Temporary just one type of monster
        additionalLvl = heroes[i]->getLevel() - 1;
        switch(rand()%3){
            case 0:
                this->monsters[i] = 
                new Dragon("Dragon" + to_string(i), additionalLvl);
                break;

            case 1:
                this->monsters[i] = 
                new Exoskeleton("Exoskeleton" + to_string(i), additionalLvl);
                break;     

            case 2:
                this->monsters[i] = 
                new Spirit("Spirit" + to_string(i), additionalLvl);
                break; 

        }
    }
    
    cout << "A new combat is about to start ..!!" << endl;

}


void Common::heroesTurn(void){

    enum availability{ available, notAvailable};
    
    int availableHeroesCount, availableMobsCount, availableHeroes[this->heroesNum] = {};

    /* Keep arrays with valid options */
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
            << "1. Select a monster " << endl
            << "2. Display stats " << heroes[heroIndex]->getName() << endl
            << "3. Select another hero" << endl
            << ">";

        while(inputHandler(select, options, 4) == false);

        if(select == 3)break;


        if(select == 0){
            /*If hero equip an item or use a potion, loses his turn */
            if(heroes[heroIndex]->checkInventory(true, true) == true){
            cout << heroes[heroIndex]->getName() << " lost his turn!" << endl;
            availableHeroes[heroIndex]++;
            break;
            }
        }
        else if(select == 1){
            
            while(1){ //>>Menu 3
                if(availableHeroes[heroIndex])break;//Go back
                if(select == 0)break;

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
                        << "3. Display stats" << endl
                        << "0. Go back" << endl
                        <<">";

                    while(inputHandler(select, options, 4) == false);

                    if(select == 0)break;
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
                            cout << endl;
                            cout << "1. Select another spell " << endl;
                            cout << "0. Go back " << endl;
                            cout <<" >"<< endl;

                            while(inputHandler(select, options, 2) == false);
                            if(select == 0)break;
                        }
                        if(select == 0)continue; //Go back
                        
                        cout << endl;
                        if(heroes[heroIndex]->castSpell(spellIndex, monsters[monsterIndex])\
                        == false)continue; //Go back
                    }
                    else if(select == 3){
                        monsters[monsterIndex]->displayStats();
                        continue;
                    }
                    //If hero use spell, attack , equip weapon/armor, drink potion 
                    //his turn is ended for the current round
                    availableHeroes[heroIndex]++;
                }//>>Menu 3

            }

        }
        else if(select == 2){
            heroes[heroIndex]->displayStats();
            cout << endl;
        }

        }//>Menu 2
    cout << endl << endl;
    }//>Menu 1

}


void Common::monstersTurn(void){

  int target;
  bool stillAlive = false;

  for(int i = 0; i < this->monstersNum; i++){

    if(monsters[i]->getHp() == 0)continue;

    for(int i = 0; i < this->heroesNum; i++)
      if(heroes[i]->getHp() != 0)stillAlive = true;

    if(stillAlive == false) return;
    target = getRandTarget();
    
    cout << this->monsters[i]->getName()
         << " targeted " << this->heroes[target]->getName() << endl << endl;
   
    /*cout << "Press Enter to Continue";
    cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
    cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
    cout << endl;*/

    this->monsters[i]->attack(heroes[target]);
    cout << endl;

    /*cout << "Press Enter to Continue";
    cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
    cout << endl;*/
  }
}


int Common::getRandTarget(void){
  int target;
  //*debug
  cout << "Monster selecting a target.." << endl;
  while(1){
    target = rand()%this->heroesNum;
    if(this->heroes[target]->getHp() != 0)break;
  }
  return target;
}


void Common::endOfRound(){

  this->round++;

  for(int i = 0; i < this->heroesNum; i++){
    heroes[i]->roundPass();
  }

  for(int i = 0; i < this->monstersNum; i++){
    monsters[i]->roundPass();
  }
}


int Common::combatResult(){

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


void Common::receiveRewards(void){

  Item *item;

  for(int i = 0; i < this->heroesNum; i++){

    heroes[i]->receiveExperience(gainExp(heroes[i]->getLevel()));

    heroes[i]->pickUp(gainMoney(heroes[i]->getLevel()));

    if((item = gainItem())!= nullptr){
      heroes[i]->pickUp(item);
    }
  }
}


int Common::gainMoney(int heroLvl)
{
  int result = 15;
  
  result += (result * (0.20 * heroLvl));
  result += rand()%4;
  result *= this->monstersNum;

  return result;
}


Item* Common::gainItem(){

  Item *item = nullptr;

  if(rand() % 50 == 1){
    item = new Weapon("Power Sword", 220, 400, 1, Weapon::oneHanded);
  }

  return item;

}


int Common::gainExp(int heroLvl)
{
  int result = 50;
  
  result -= (result * (0.02 * heroLvl));
  result += rand()%4;
  result *= this->monstersNum;

  return result;
}


void Common::reviveHeroes(bool receivePenalty){

  for(int i = 0; i < this->heroesNum; i++){
    if(heroes[i]->getHp() == 0)heroes[i]->revive(receivePenalty);
  }

}