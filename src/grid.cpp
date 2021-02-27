#include <cstdlib>
#include <limits.h>
#include <fstream>
#include <cstring>
#include "grid.h"
#include "inputHandler.h"

using namespace std;


/* Grid implementation */
const char Grid::direction[5] = {'w', 's', 'a', 'd', 'b'};

const char Grid::defaultMap[defaultHeight][defaultWidth] = 
                                      {{' ', ' ', ' ', '#'},
                                      {' ', '#','M', '#'},
                                      {' ', ' ', ' ', '#'},
                                      {' ', ' ', '#', '#'}};

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
    printLogo();
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

            cout << "\n1. Create another hero\n"
                << "0. Exit creation\n\n"
                << ">";


            while(inputHandler(input, options, 2) == false);

            if(input == 0)break;
        }
        system("clear");


        printLogo();

        cout << "\nSelect a class :" << endl << endl;
        cout << "1. Warrior    || A warrior class has additional strength and agility"
            << endl 
            << "               || Strength : " 
            << Warrior::startingStr + Warrior::additionalStr << endl
            << "               || Dexterity : "
            << Warrior::startingDex << endl
            << "               || Agility : "
            << Warrior::startingAgi + Warrior::additionalAgi << endl << endl;
        
        cout << "2. Sorcerer   || A Sorcerer class has additional dexterity and agility"
            << endl 
            << "               || Strength : " 
            << Sorcerer::startingStr << endl
            << "               || Dexterity : "
            << Sorcerer::startingDex + Sorcerer::additionalDex << endl
            << "               || Agility : "
            << Sorcerer::startingAgi + Sorcerer::additionalAgi << endl << endl;
        
        cout << "3. Paladin    || A Paladin class has additional strength and dexterity"
            << endl 
            << "               || Strength : " 
            << Paladin::startingStr + Paladin::additionalStr << endl
            << "               || Dexterity : "
            << Paladin::startingDex + Paladin::additionalDex << endl
            << "               || Agility : "
            << Paladin::startingAgi << endl << endl;

        if(heroesNum > 0){
            cout << "0. Exit hero creation" << endl << endl
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

    cout << "\nHeroes created: \n\n";
    
    for(int i = 0; i < heroesNum; i++){
        heroes[i]->displayStats();
        cout << endl;
    }
    pressEnterToContinue();
}


Grid::~Grid(){

    system("clear");
    cout << "<<< Game over!! >>>\n\n";
    Combat::statistics();

    cout << "\nPress enter to exit ";
    std::cin.ignore();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    for(int i = 0; i < marketsNum; i++){
        delete markets[i];
    }
    
    delete[] markets;
    delete nonAccessive;
    delete common;

    for(int i = 0; i < heroesNum; i++)
        delete heroes[i];

    for(int i = 0; i < height; i++)
        delete[] blocks[i];
    
    delete[] blocks;
}


void Grid::play(void){

    int input = -1, heroIndex;

    while(1){
        system("clear");
        display();
        cout << "1. Move " << endl
            << "2. Select A hero "<< endl
            << "0. Exit game " << endl << endl
            << ">";


        while(inputHandler(input, options, 3) == false);

        switch(input){
            case 0: //Exit game
                return;

            case 1:
                //is valid direction
                system("clear");
                if(move() == true){
                    if(blocks[heroesLoc.x][heroesLoc.y]->\
                    access()){
                        blocks[heroesLoc.x][heroesLoc.y]->interactWith(heroes, heroesNum);
                        blocks[heroesLoc.x][heroesLoc.y]->leave();            
                    }
                }
                break;

            case 2: //Select a hero to check inventory / stats
                if(this->heroesNum > 1){
                    system("clear");
                    cout << "Hero selection\n\n";
                    for(int i = 0; i < this->heroesNum; i++){
                        cout << "[" << i+1 << "]  " << heroes[i]->getName()
                             << endl;
                    }

                    cout << "\nHero:";
                    while(inputHandler(heroIndex, &options[1], this->heroesNum)
                     == false);
                    heroIndex -= 1;
                }
                else{
                    heroIndex = 0;
                }
                while(1){
                    system("clear");

                    cout << "Selected hero: "<< heroes[heroIndex]->getName() << endl << endl;

                    cout << "1. Check inventory " << endl
                        << "2. Display stats "<< endl
                        << "0. Go back " << endl << endl
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

                    pressEnterToContinue();
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
        system("clear");
        display();
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
            if(blocks[x][y]->access() == true){
                heroesLoc.setLoc(x,y);
                return true;
            }
        }
        cout << "Oops couldn't move there.." << endl;
        pressEnterToContinue();
    }//End of while(1)


    return false;
}


void Grid::display(void) const{
                                      
    string block[5] = {"   |"," [M|"," ##|", "H  |", "H[M|"};
    cout << endl <<  "-----------------" << endl;

    for(int i = 0; i < this->height; i++){
        for(int j = 0; j < this->width; j++){
            if(j == 0)cout <<"|";
            switch(defaultMap[i][j]){
                case ' ':
                    if(heroesLoc.isSame(i,j) == true){
                        cout <<block[3];
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
        cout << endl <<  "-----------------" << endl;
    }                     
    //cout << "================" << endl;
    cout << endl;
}

void Grid::printLogo(void)
{
    for(int i = 0; i < logoSize; i++)
        cout << gameLogo[i] << endl;
}

/****************************************************************************/


Block::~Block(){}



/* Block Market implementation */
string Market::productType[4] = {"weapon", "armor", "spell", "potion"};

Market::Market(){
   

    
    this->name = "Default Market";

    string name, nameA, nameB, temp;
    int price, requiredLevel;

    /*  Create weapons  */
    int damage, handsRequired;

    ifstream myFile("./files/weapons");
    productCounter[weapon] = getFileLines(myFile);

    this->weapons = new Weapon *[productCounter[weapon]];

    if (myFile.is_open())
	{   
        int i = 0;
		while (myFile >> nameA >> nameB >> 
        damage >> price >> requiredLevel >> handsRequired)
		{		
            name = nameA + " "+ nameB;
            this->weapons[i++] = 
            new Weapon(name, damage, price, requiredLevel, handsRequired);
		}
	}
	else{
        cout << "Unable to read weapons file!" << endl;
        exit(1);
    } 
    myFile.close();


    /*  Create armors  */
    int defence = 0;

    myFile.open("./files/armors");
    productCounter[armor] = getFileLines(myFile);

    this->armors = new Armor *[productCounter[armor]];

    if (myFile.is_open())
	{   
        int i = 0;
		while (myFile >> nameA >> nameB >> 
        defence >> price >> requiredLevel)
		{		
            name = nameA + " "+ nameB;
            this->armors[i++] = 
            new Armor(name, defence, price, requiredLevel);
		}
	}
	else{
        cout << "Unable to read armors file!" << endl;
        exit(1);
    } 
    myFile.close();
    

    /*  Create spells  */
    int spellType, points, mpCost;
    enum types{ice, fire, light};
    Spell *spell;

    myFile.open("./files/spells");
    productCounter[Market::spell] = getFileLines(myFile);

    this->spells = new Spell *[productCounter[Market::spell]];

    if (myFile.is_open())
	{   
        int i = 0;
		while (myFile >> spellType >> nameA
        >> nameB >> damage >> points >> price >>requiredLevel >> mpCost)
		{		
            name = nameA + " "+ nameB;
            switch(spellType){
                case ice:
                    spell = new IceSpell(name, damage, points,
                    price, requiredLevel, mpCost);
                    break;
                case fire:
                    spell = new FireSpell(name, damage, points,
                    price, requiredLevel, mpCost);
                    break;
                case light:
                    spell = new LightingSpell(name, damage, points,
                    price, requiredLevel, mpCost);
                    break;
            }

            this->spells[i++] = spell;
		}
	}
	else{
        cout << "Unable to read armors file!" << endl;
        exit(1);
    } 
    myFile.close();


    /*  Create potions  */
    int stat;

    myFile.open("./files/potions");
    productCounter[potion] = getFileLines(myFile);

    this->potions = new Potion *[productCounter[potion]];

    if (myFile.is_open())
	{   
        int i = 0;
		while (myFile >> nameA >> nameB 
        >> stat >>  price >> points >>requiredLevel)
		{		
            name = nameA + " "+ nameB;
            this->potions[i++] = 
            new Potion(name, stat, price, points, requiredLevel);
		}
	}
	else{
        cout << "Unable to read armors file!" << endl;
        exit(1);
    } 
    myFile.close();

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
}


void Market::printLogo(void) const
{
    for(int i = 0; i < logoSize; i++)
        cout << logo[i] << endl;
}


void Market::interactWith(Hero **heroes, int heroesNum){
    
    addHeroes(heroes, heroesNum);

    if(heroes == nullptr){
        cout << "There is no heroes in Market block\n";
        return;
    }

    int money, select, heroIndex, input;
    bool goBack;
    Hero *hero;

    while(1){

        system("clear");
        if(heroesNum == 1)heroIndex = 0;
        else{
        cout << "1. Select a hero to enter market " << endl
            << "0. Exit store " << endl << endl
            << ">";
        
        while(inputHandler(input, options, 2) == false);

        if(input == 1){
            system("clear");
            cout << "Entering Market..\n\n";
            for(int i = 0; i < heroesNum; i++)
            cout << "[" << i+1 << "] " << heroes[i]->getName() << endl;

            cout << "\nHero: ";
            while(inputHandler(heroIndex, &options[1], heroesNum) == false);
            heroIndex -= 1;
        }
        else break; //Go back
        }

            
        hero = heroes[heroIndex];
        while(1){
            system("clear");
            printLogo();
            cout << "Selected hero: "<< heroes[heroIndex]->getName() << endl << endl;
            cout << "Welcome to my store!!" << endl
            << "What are you looking for?" << endl;
            cout << "1. Buy " << endl
                << "2. Sell" << endl;
            int size;
            if(heroesNum == 1){
                cout
                <<"0. Exit store" << endl << endl
                <<">";
                size = 3;
            }
            else{
                cout
                << "3. Enter store with another hero\n"
                <<"0. Exit store\n\n"
                <<">";
                size = 4;
            }                

            while(inputHandler(select, options, size) == false);


            if(select == 0)return;
            if(select == 3)break;
            goBack = false;

            switch(select){
                
            /* Buy a product from market */
            case 1:
                while(1){

                    if(goBack == true)break;
                    system("clear");
                    printLogo();
                    cout << "Selected hero: "<< heroes[heroIndex]->getName() << endl << endl;
                    cout << "What do you want to buy?" << endl;
                    /* Product categories */
                    cout << "1. Weapon" << endl
                        << "2. Armor" << endl
                        << "3. Spell" << endl
                        << "4. Potion" << endl
                        << "0. Go back" << endl << endl
                        << ">" ;

                    while(inputHandler(select, options, 5) == false);
                    
                    int productId;
                    int productType = select-1;

                    if(select == 0)break; // Go back
                    
                    else{ // 1 - 4: Select a product from the selected category 
                        system("clear");
                        productId = selectProduct(productType);
                        if(productId == -1)continue;
                    }

                    /* If product exists try to buy */
                    switch(productType){
                        case Market::weapon:
                        case Market::armor:
                        case Market::potion: //Any item:
                            hero->buy(\
                                sell(productType, productId, money, hero->getLevel()));
                            pressEnterToContinue();
                            break;

                        case Market::spell:
                            hero->buy(\
                                sell(productId, money, hero->getLevel()));
                            pressEnterToContinue();
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
                    pressEnterToContinue();
                continue;
                }
                while(1){
                    if(hero->getInventorySize() == 0){
                        cout << "Your inventory is empty!" << endl;
                        break; //Go back
                    }
                    system("clear");
                    printLogo();
                    cout << "Selected hero: "<< heroes[heroIndex]->getName() << endl << endl;
                    hero->checkInventory();
                    cout << 
                    "Select id of item to sell or any other number to go back\n\n"
                    << "Sell item: ";

                    int inventorySize = hero->getInventorySize();
                    int inventorySlots[inventorySize];
                    for(int i = 0; i < inventorySize; i++)inventorySlots[i] = i;
                    int inventorySlot;
                    select = -1;
                    while(inputHandler(inventorySlot, inventorySlots, inventorySize)
                     == false){
                        if(hero->getInventorySize() == 0)break;
                        cout << "1. Select another item " << endl
                             << "0. Go back " << endl << endl
                             << ">";

                        while(inputHandler(select, options, 2) == false);
                        break; //Go back
                        
                    }
                    if(select == 0)break; //Go back
                    else if(select == 1)continue; //Select another item

                    /* Inventory slot given is valid, sell item */
                    hero->receiveMoney(\
                        buy(\
                            hero->sell(inventorySlot)));
                }
                pressEnterToContinue();
                break;
                //End of case 2: sell item to market
            }
        }
    }
}


void Market::showItems(int type){
  
    switch(type){
      case weapon:
        for(int i = 0; i < this->productCounter[weapon]; i++){
            cout << "_[" << i+1 << "]_" << endl; 
            weapons[i]->print(); 
            cout << "======================" << endl << endl;
        }
        break;

      case armor:
        for(int i = 0; i < this->productCounter[armor]; i++){
            cout << "_[" << i+1 << "]_" << endl; 
            armors[i]->print(); 
            cout << "===================" << endl << endl;
        }
        break;

      case spell:
        for(int i = 0; i < this->productCounter[spell]; i++){
            cout << "_[" << i+1 << "]_" << endl; 
            spells[i]->print(); 
            cout << "=====================" << endl << endl;
        }
        break;

      case potion:
        for(int i = 0; i < this->productCounter[potion]; i++){
            cout << "_[" << i+1 << "]_" << endl; 
            potions[i]->print(); 
            cout << "===================" << endl << endl;
        }
        break;

    }
}


int Market::selectProduct(int type){
    int select = -1, input;
    
    int productSlots[productCounter[type]];
    for(int i = 1; i <= productCounter[type]; i++)productSlots[i] = i;

    while(1){
        input = -1;

        system("clear");
        cout << "Select " << this->productType[type] << " :\n\n"; 
        
        showItems(type); 
        
        cout << "0. Go back\n\n";
        cout << this->productType[type] << ":"; 
        
        while(inputHandler(select, productSlots, productCounter[type]+1) == false){
            cout << "\n1. Select another " << this->productType[type] << endl
                << "0. Go back \n" 
                <<">";
            while(inputHandler(input, options, 2) == false);
            if(input == 0)
                return -1;
            else break;
        }
        if(input != 1)break;
    }


  return select-1;
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
Common::Common(void){
    combat = nullptr;
}


Common::~Common(void){

    for(int i = 0; i < (int)itemsDropped.size(); i++)
        delete itemsDropped[i];

    itemsDropped.clear();
}


void Common::interactWith(Hero **heroes, int heroesNum){

    if(heroes == nullptr){
        cout << "There is no heroes in Common block\n";
        return;
    }
    
    if(isPeacefull() == true)return;

    combat = new Combat(heroes, heroesNum);
    combat->start();

    delete combat;
}


vector<string> Combat::monsterNames[3];

int Combat::monsterKilled, Combat::goldGained,
Combat::itemsGained,Combat::xpGained,Combat::heroesRevives;

bool Combat::isStaticInit = false;

void Combat::initStatics(){
    
    if(isStaticInit == true)return;

    ifstream myFile("./files/dragon_names");

    string name;

    if (myFile.is_open())
        while (myFile >> name){
            monsterNames[dragon].push_back(name);
        } 
    else{
        cout << "Unable to read dragon_names file!" << endl;
        exit(1);
    } 
    myFile.close();

    myFile.open("./files/exoskeleton_names");
    if (myFile.is_open())
        while (myFile >> name){
            monsterNames[exoskeleton].push_back(name);
        } 
    else{
        cout << "Unable to read exoskeleton_names file!" << endl;
        exit(1);
    } 
    myFile.close();

    myFile.open("./files/spirit_names");
    if (myFile.is_open())
        while (myFile >> name){
            monsterNames[spirit].push_back(name);
        } 
    else{
        cout << "Unable to read spirit_names file!" << endl;
        exit(1);
    } 
    myFile.close();

    isStaticInit = true;

    return;
}


Combat::Combat(Hero **heroes,int heroesNum){
 
    system("clear");
    cout << "You have entered a dangerous zone ..\n\n";

    addHeroes(heroes, heroesNum);

    if(isStaticInit == false){
        initStatics();
    }

    this->round = 1;

    /*Create same number of monsters*/
    this->monstersNum = heroesNum;
    this->monsters = new Monster*[heroesNum];

    int additionalLvl, j;

    for(int i = 0; i < this->monstersNum; i++){
      //*Temporary just one type of monster
        additionalLvl = heroes[i]->getLevel() - 1;
        switch(rand()%3){
            case dragon:
                j = rand() % monsterNames[dragon].size();
                this->monsters[i] = 
                new Dragon(monsterNames[dragon][j] , additionalLvl);
                break;

            case exoskeleton:
                j = rand() % monsterNames[exoskeleton].size();
                this->monsters[i] = 
                new Exoskeleton(monsterNames[exoskeleton][j], additionalLvl);
                break;     

            case spirit:
                j = rand() % monsterNames[spirit].size();
                this->monsters[i] = 
                new Spirit(monsterNames[spirit][j], additionalLvl);
                break; 

        }
    }
    
    cout << "\nA new combat started ..!!\n";

    pressEnterToContinue();
}


Combat::~Combat(void){

    for(int i = 0; i < monstersNum; i++)
        delete monsters[i];

    delete[] monsters;
}


void Combat::start(void){
    

    while(result() == stillFighting){

        heroesTurn();

        monstersTurn();

        endOfRound();

    }

    for(int i = 0; i < monstersNum; i++)
        if(monsters[i]->getHp() == 0)monsterKilled++;

    int results = result();
    bool receivePenalty;

    system("clear");
    cout
    << "===============================================================" 
    <<  endl << endl <<"                Combat ended\n\n"
    << "==============================================================="
    << endl << endl;

    if(results == heroesWon){
        cout << "Congratulations you won the fight !!" << endl;

        receivePenalty = false;
        reviveHeroes(receivePenalty);
        pressEnterToContinue(false);

        receiveRewards();
    }
    else if(results == monstersWon){
        cout << "You lost the fight!Better luck next time.. " << endl
        << endl;
        receivePenalty = true;
        reviveHeroes(receivePenalty);
        pressEnterToContinue(false);
    }


        pressEnterToContinue(false);
}





void Combat::heroesTurn(void){

    enum availability{ available, notAvailable};
    
    int availableHeroesCount, availableMobsCount, 
    heroTurn[this->heroesNum] = {};

    int select;
    int heroIndex, monsterIndex;


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
                if(heroTurn[i] == available){
                    availableHeroesCount++;
                    heroIndex = i; //In cacse of one hero keep index
                }
            } 
        }

        if(availableHeroesCount == 0)return;

        /* If available heroes are more than 1 , select a hero.*/
        if(availableHeroesCount > 1){
        
            print(-1,-1);
            cout << "Select a hero " << endl << endl;

            for(int i = 0; i < this->heroesNum; i++){
                if(heroTurn[i] != available)continue;
                cout << "["<< i+1 << "] " << heroes[i]->getName() << endl;
            }
            cout << "\nHero:";

            while(inputHandler(heroIndex, &options[1], heroesNum) == false);

            heroIndex -= 1;
            if(heroes[heroIndex]->getHp() == 0){
                cout << heroes[heroIndex]->getName() 
                    << " can't fight .. is Unconscious ..!" << endl;
                pressEnterToContinue();
                continue;   //Select another hero
            }
            else{
                if(heroTurn[heroIndex] != available){
                    cout << heroes[heroIndex]->getName() 
                    <<" allready fought" << endl;
                    pressEnterToContinue();
                    continue;
                }
            }
        }

        while(1){ //>Menu 2
            if(heroTurn[heroIndex] != available)break; //Go back
            
            print(heroIndex,-1);

            if(availableHeroesCount <= 1)
            cout << "\nPreselected Hero: " << 
            heroes[heroIndex]->getName() << endl << endl;

            cout << "1. Select a monster" << endl
                << "2. Check inventory  " << endl
                << "3. Display stats " << endl << endl
                << "0. Select another hero" << endl << endl
                << ">";

            while(inputHandler(select, options, 4) == false);

            /* Select another hero (GO back) */
            if(select == 0)break;

            /* Check inventory */
            if(select == 2){ 
                /*If hero equip an item or use a potion, loses his turn */
                if(heroes[heroIndex]->checkInventory(true, true) == true){
                    cout << heroes[heroIndex]->getName() << " lost his turn!" << endl;
                    heroTurn[heroIndex] = notAvailable;
                    pressEnterToContinue();
                    break;
                }
                pressEnterToContinue();
            }
            /* Display stats */
            else if(select == 3){
                heroes[heroIndex]->displayStats();
                cout << endl;
                pressEnterToContinue();
            }
            /* Select a monster */
            else if(select == 1){
                
                while(1){ //>>Menu 3
                    if(heroTurn[heroIndex] != available)break;//Go back
                    if(select == 0)break;

                    else if(availableMobsCount > 1){
                        print(heroIndex,-1);
                        cout << "Select a monster\n\n";

                        for(int i = 0; i < this->monstersNum; i++){
                            if(monsters[i]->getHp() == 0)continue;
                            cout << "["<< i+1 << "] "  << monsters[i]->getName() << endl;
                        }


                        cout << "\nMonter:";

                        while(inputHandler(monsterIndex, &options[1], monstersNum) == false);

                        monsterIndex -= 1;

                        if(monsters[monsterIndex]->getHp() == 0){
                        cout << monsters[monsterIndex]->getName() 
                            << " cant fight back .. is Unconscious ..!" << endl;
                            pressEnterToContinue();
                            continue;
                        } 
                        print(heroIndex,monsterIndex);
                    }
                    else {
                        print(heroIndex,monsterIndex);
                        cout << "Preselected Monster: " 
                            << monsters[monsterIndex]->getName() << endl << endl;

                    }
                    while(1){ //>>>menu 4
                        if(heroTurn[heroIndex] != available)break; //Go back

                        cout << "1. Attack" << endl
                            << "2. Cast Spell" << endl
                            << "3. Display stats" << endl << endl
                            << "0. Go back" << endl << endl
                            <<">";

                        while(inputHandler(select, options, 4) == false);

                        if(select == 0)break;//Go back
                        bool isSpellCasted = false;
                        int spellId;
                        switch(select){
                            case 1:/* Attack */
                                heroes[heroIndex]->attack(monsters[monsterIndex]);
                                heroTurn[heroIndex] = notAvailable;
                                break;
                            
                            case 2:/* Cast spell */
                                spellId = heroes[heroIndex]->selectSpell();
                                if(spellId != -1)
                                {
                                    isSpellCasted = heroes[heroIndex]->
                                    castSpell(spellId, monsters[monsterIndex]);
                                }
                                if(isSpellCasted == true)
                                    heroTurn[heroIndex] = notAvailable;   

                                cout << endl; 
                                break;

                            case 3:/* Display monster stats*/
                                monsters[monsterIndex]->displayStats();
                                break;
                        }

                        pressEnterToContinue();
                        print(heroIndex,monsterIndex);
                    }//<<Menu 4
                }//<<Menu 3
            }//Monster selection
        }//<Menu 2
    cout << endl << endl;
    }//<Menu 1

}


void Combat::monstersTurn(void){

  int target;
  bool stillAlive = false;

  for(int i = 0; i < this->monstersNum; i++){

    if(monsters[i]->getHp() == 0)continue;

    for(int i = 0; i < this->heroesNum; i++)
      if(heroes[i]->getHp() != 0)stillAlive = true;
   
    print(-1, -1, false);
    if(stillAlive == false) return;
    target = monsters[i]->selectTarget(heroes, heroesNum);
    
    pressEnterToContinue(false);
    print(target, i, false);

    this->monsters[i]->attack(heroes[target]);
    cout << endl;
    pressEnterToContinue(false);

  }
}



void Combat::endOfRound(){
    system("clear");
    cout
    << "===============================================================" 
    <<  endl << endl <<"               *** End of Round "
    << this->round << "   ***" << endl << endl
    << "==============================================================="
    << endl << endl;

    this->round++;

    for(int i = 0; i < this->heroesNum; i++){
        heroes[i]->roundPass();
    }

    for(int i = 0; i < this->monstersNum; i++){
        monsters[i]->roundPass();
    }
    cout << endl;
    pressEnterToContinue(false);
}


int Combat::result(){

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


void Combat::print(int heroSelect, int monsterSelect, bool heroTurn){

    system("clear");
    cout << endl 
    << "===============================================================" 
    <<  endl << endl <<"                  ***    Round "
    << this->round << "   ***" << endl << endl;
    if(heroTurn == true){
        cout
        << "===================== Heroes turn =============================" 
        << endl;
    }
    else{
        cout
        << "===================== Monsters turn ===========================" 
        << endl;
    }
   // cout << endl;
    for(int i = 0; i < 63; i++)cout << "/";
    cout << endl << endl;
    
    string monsterName[monstersNum];
    string heroName[heroesNum];

    for(int i = 0; i < heroesNum; i++){
        heroName[i] =  heroes[i]->getName() +" [" + 
        to_string(heroes[i]->getHp()) + 
        "/" + to_string(heroes[i]->getMaxHp()) + "]";
    }

    for(int i = 0; i < monstersNum; i++){
        monsterName[i] =  monsters[i]->getName() + " [" +
         to_string(monsters[i]->getHp()) + 
        "/" + to_string(monsters[i]->getMaxHp()) + "]";
    }

    if(heroSelect != -1)
        heroName[heroSelect] = "->" + heroName[heroSelect];
    if(monsterSelect != -1)
        monsterName[monsterSelect] = "->" + monsterName[monsterSelect];

    int n = (heroesNum > monstersNum)? heroesNum : monstersNum;

    int s, spacesNum = 30;
    for(int i = 0; i < n; i++){
        string hero;
        if(i >= heroesNum)s = spacesNum;
        else
        {
            cout << heroName[i];
            hero = heroName[i];
            s = spacesNum - hero.length();
        }
        
        for(int j = 0; j < s; j++)cout << " ";

        if(i < monstersNum)cout << monsterName[i];

        cout << endl;
    }
    cout << endl;
    for(int i = 0; i < 63; i++)cout << "\\";
    cout << endl << endl;

}


void Combat::receiveRewards(void){

  Item *item;
    system("clear");
    cout
    << "===============================================================" 
    <<  endl << endl <<"             +++++ Combat Rewards +++++\n\n"
    << "==============================================================="
    << endl << endl;

  for(int i = 0; i < this->heroesNum; i++){

    heroes[i]->receiveExperience(gainExp(heroes[i]->getLevel()));

    heroes[i]->pickUp(gainMoney(heroes[i]->getLevel()));

    if((item = gainItem())!= nullptr){
      heroes[i]->pickUp(item);
    }
    cout << "------------------------------------------\n";
  }
}


int Combat::gainMoney(int heroLvl)
{
    int result = goldRate;
    
    result += (result * (0.20 * heroLvl));
    result += rand()%4;
    result = result * (double)(monstersNum/heroesNum);
        
    goldGained += result;
    
    return result;
}


Item* Combat::gainItem(){

    Item *item = nullptr;

    if(rand() % 50 == 1){
        item = new Weapon("Power Sword", 220, 300, 1, Weapon::oneHanded);
        itemsGained++;
    }


        
    return item;

}


int Combat::gainExp(int heroLvl)
{
    int result = xpRate;
    
    result -= (result * (0.02 * heroLvl));
    result += rand()%4;
    result = result * (double)(monstersNum/heroesNum);

    this->xpGained += result;

    return result;
}


void Combat::reviveHeroes(bool receivePenalty){

  for(int i = 0; i < this->heroesNum; i++){
    if(heroes[i]->getHp() == 0){
        heroes[i]->revive(receivePenalty);
        heroesRevives++;
    }
  }

}


void Combat::statistics(void){
    cout
    << "         /~\n"
    << "*(=####{<>==================-\n"
    << "         \\_ \n"
    << "\n* Combat statistics *\n"
    << "\nMonsters killed: " << monsterKilled
    << "\nHeroes revived: " << heroesRevives
    << "\nXp gained: " << xpGained
    << "\nGold picked up: " << goldGained
    << "\nItems picked up: " << itemsGained
    << endl;
}