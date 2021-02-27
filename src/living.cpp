#include <cstdlib>   
#include <cstring>
#include <iostream>
#include <utility> 
#include <vector>
#include "living.h"
#include "inputHandler.h"
#include "buff.h"

using namespace std;

const string Hero::statsTypeMsg[3] = 
{"strength", "dexterity", "agility"};

/* Base class *Living* implementation*/
Living::Living(string name)
{ 
    this->name = name;
    this->level = startingLevel;
    this->hp = this->maxHp = startingHp; 
}

Living::~Living(){}


/* Subclass of Living -> *Hero* implementation */
Hero::Hero(string name) : Living(name){

    /* Init strength, dexterity, agility */
    this->current = 
    this->base = {startingStr, startingDex, startingAgi};

    addMaxHp(additionalHp);addHp(additionalHp);

    buffCounter = 0;

    maxMp = mp = startingMp;
    experience = startingExperience;
    
    int damage = 100, price = 10, requiredLvl = 1;
    starterWeapon = 
    new Weapon("Wooden Sword", damage, price, requiredLvl, Weapon::oneHanded);

    int defence = 15; price = 20;
    starterArmor = 
    new Armor("Wooden Armor", defence, price, requiredLvl);
    
    this->inventory = 
    new Inventory(starterWeapon, starterArmor, startingMoney);
}


Hero::~Hero(){
    for(int i = 0; i < buffCounter; i++)
        delete buffs[i];

    delete starterWeapon;
    delete starterArmor;
    delete inventory;
}


int Hero::attack(Living *living) const{

    int damage = getDamage();

    cout << this->getName() << " attacks " <<
        living->getName() << ".."<< endl;
    
    living->receiveDamage(damage);

    return damage;
}


int Hero::getDamage(void) const{
    int result = 0;
    Weapon *weapon = inventory->getWeapon();

    if(weapon != nullptr){
        result = weapon->getDamage();
        result += (this->current.str / 100.0) * result; 
    }
    return result;
}



int Hero::receiveDamage(int damage){

    int damageDealt = damage;

    if(damageDealt > 0){

        if(rand()%100 > agility){
            damageDealt -= (getDefence() / 100.0) * damageDealt;
            cout << this->getName() << " received " 
                << damageDealt << " damage!" <<  endl;
            subHp(damageDealt);
        }
        else{
            damageDealt = 0;
            cout << this->getName() << " Evade attack!! " << endl;
        } 
    }

    if(this->getHp() == 0)goUnconscious();

    return damageDealt;
}


int Hero::getDefence(void) const{
    int result = 0;
    Armor *armor = inventory->getArmor();

    if(armor != nullptr)
        result = armor->getDefence();
    
    return result;
}


void Hero::goUnconscious(void){
    cout << "Fell into a state  of unconsciousness!!" << endl;
    int counter = buffCounter;
    for(int i = 0; i < counter; i++){
            removeBuff(i);
    }
}



int Hero::selectSpell(void) const{
    
    int spellsNum = spells.size();
    int spellId;
    int select;

    if(spellsNum == 0){
        cout << "There aren't any spells learned yet!" << endl;
        spellId = -1;
    }
    else{
        cout << "Select a spell" << endl;
        checkSpells();

        int selectSpell[spellsNum];
        for(int i = 0; i < spellsNum; i++)selectSpell[i] = i;
        cout << "Spell: ";

        while(inputHandler(spellId, selectSpell, spellsNum) == false){
            cout << endl;
            cout << "1. Select another spell " << endl;
            cout << "0. Go back " << endl;
            cout <<" >"<< endl;

            while(inputHandler(select, options, 2) == false);
            if(select == 0)return -1;
            cout << "Spell: " << endl;
        }
    }
    return spellId;
}


void Hero::checkSpells(void) const{
    if(spells.size() > 0){
        cout << "** Learned Spells ** " << endl;
        for(int i = 0; i < (int)spells.size(); i++){
            cout << i << ": " << spells[i]->getName();
            cout << endl;
        }
    }
    else cout << "No spell learned yet " << endl;
}



bool Hero::castSpell(int spellId, Living *living) {

    if(spellId < (int)this->spells.size() && spellId >= 0){
        Spell* spell = spells[spellId];

        if(spell->getMp() > this->mp){
            cout << "Not enough mp!" << endl;
            return false;
        }
        else{
            cout << this->getName() << " casting "
                 << spell->getName() << " to " 
                 << living->getName() << "..\n";

            int damage;
            Buff *deBuff = spell->cast(this->current.dex, damage);

            this->subMp(spell->getMp());

            //If damage received and it's still alive cast debuff
            if(living->receiveDamage(damage) != 0 && living->getHp()!= 0 ){
                Monster *monster = (Monster*)living;
                monster->receiveDeBuff(deBuff);
            }
            else delete deBuff;
        }
    }
    else {
        cout << "Spell not found!" << endl;
        return false;
    }
    return true;
}



void Hero::receiveExperience(int exp){

    int result = getExp() + exp;

    cout << getName() << " received "
         << exp << " experience!" << endl;

    while(result >= maxExperience){

        if(getLevel() != maxLevel){
            setExp(0);
            levelUp();
        }
        else{
            result = maxExperience;
            break;
        } 
        result -=  maxExperience; 
    }

    cout << "Experience: " <<getExp() << " --> " << result  << endl;
    setExp(result);
}


void Hero::revive(bool getPenalty){

    //Can't revive a living one..
    if(getHp() != 0){
        cout << getName() << " isn't unconscious" << endl;
        return;
    }
    int restoreLife = getMaxHp() / 2;

    cout << getName() << " revived!!" << endl
         << "Restored hp: " << getHp() << " --> ";
    this->addHp(restoreLife);
    cout << getHp() << endl;

    if(getPenalty == true){
        cout << "Money lost: " << getMoney() << " --> ";
        subMoney(getMoney()/2);
        cout << getMoney() << endl;
    }

}


void Hero::regeneration(void){

    if(getHp() != 0){
      cout << getName() << " regeneration ++ " << endl
           << "hp regen: "
           << getHp() << "-->";

      addHp(getMaxHp()*regenRate);
      cout << getHp() << endl;

      cout << "mp regen: " << getMp() << "-->";

      addMp(getMaxMp()*regenRate);
      cout << getMp() << endl << endl;
    }
    else  cout << getName() << " is Unconscious!" << endl << endl;

}


void Hero::levelUp(void){

    cout << "Level up!!" << endl;
    cout << "Level: " << getLevel() << " --> ";
    addLevel(1);
    cout << getLevel() << endl;

    cout 
    << "str: " << current.str << " --> " << current.str + statsPerLevel << endl
    << "dex: " << current.dex << " --> " << current.dex + statsPerLevel << endl
    << "agi: " << current.agi << " --> " << current.agi + statsPerLevel << endl
    << "Life: " << getHp() << "/" << getMaxHp() << " --> "
    << getHp() + hpPerLevel << "/" << getMaxHp() + hpPerLevel << endl
    << "Mana: " << this->mp << "/" << this->maxMp << " --> "
    << this->mp + mpPerLevel << "/" << this->maxMp + mpPerLevel << endl;

    addStats(statsPerLevel, statsPerLevel, statsPerLevel);
    addBaseStats(statsPerLevel, statsPerLevel, statsPerLevel);
    addMaxHp(hpPerLevel);addHp(hpPerLevel);
    addMaxMp(mpPerLevel);addMp(mpPerLevel);

}



void Hero::pickUp(Item *item){
    if(item != nullptr){
        inventory->addItem(item);
        cout << getName() << " Picked up " 
        << item->getName() << "!!!" << endl;
    }
}


void Hero::pickUp(int money){
    if(money > 0){
        addMoney(money);
        cout << getName() << " Picked up " 
        << money << " gold!!!" <<  endl;
    }
}


void Hero::receiveMoney(int money){
    if(money > 0){
        addMoney(money);
        cout << getName() << " Received " 
        << money << " gold!!" <<  endl;
    }
}


Item* Hero::sell(int inventorySlot){

    Item* item = inventory->popItem(inventorySlot);

    if(item != nullptr){
        cout << "Selling " << item->getName() << " .." << endl;
    }
    else{
        cout << "Couldn't sell requested item, there is no item in slot "
             << inventorySlot << "!!" << endl;
    }

    return item;
}


void Hero::buy(Item *item){

    if(item != nullptr){
        if(getMoney() >= item->getPrice()){
            if(getLevel() >= item->getLevel()){
                subMoney(item->getPrice());
                cout << item->getName() << " successfully purchased!!" << endl; 
                inventoryAdd(item);
            }
            else cout << item->getName() << " requires "
                      << item->getLevel()-getLevel() 
                      << " more levels" << endl;
        }
    else cout << item->getPrice()- getMoney() << " gold is missing!" << endl;
    }

}


void Hero::buy(Spell *spell){

    if(spell != nullptr){
        if(getMoney() >= spell->getPrice()){
            if(getLevel() >= spell->getLevel()){
                if(findSpell(spell) == false){
                    subMoney(spell->getPrice());
                    cout << spell->getName()
                         << " successfully purchased!!" << endl; 
                    learnSpell(spell);
                }
                else  cout << spell->getName()
                           << " allready learned!!" << endl; 
            }
            else cout << spell->getName() << " requires level "
                      << spell->getLevel() << endl;
        }
    else cout << spell->getPrice()- getMoney() << " gold is missing!" << endl;
    }

}


bool Hero::learnSpell(Spell *spell){

    if(this->getLevel() >= spell->getLevel()){
        this->spells.push_back(spell);
        cout << this->getName() << " successfully learned spell "
        << spell->getName() << endl;
        return true;
    }
    return false;
}


bool Hero::findSpell(Spell *spell){
    for(int i = 0; i < (int)spells.size(); i++){
        if(spell == spells[i])return true;
    }
    return false;
}   


bool Hero::checkInventory(bool equip, bool drinkPotion){
    
    inventory->print();
    int size = 1;

    if(inventory->getSize() != 0 ){

        cout << "\n1. Check Item" << endl;
        size++;

        if(equip == true){
            cout << "2. Equip Item" << endl;
            size++;
        
        }
        if(drinkPotion == true){
            cout << "3. Drink a potion "<< endl;
            size++;
        }

            
        int selection;

        cout << "\n0. Go back\n\n"
                <<">";
        while(inputHandler(selection, options, size) == false);
        
        if(selection == 0)return false;

        cout << "Inventory slot: ";
        size = inventory->getSize();
        int inventorySlots[size];
        for(int i = 0; i < size; i++)inventorySlots[i] = i;
        
        int inventorySlot;
        while(inputHandler(inventorySlot, inventorySlots, size)==false);

        Item *item = inventory->getItem(inventorySlot);
        Potion *potion = dynamic_cast<Potion*>(item);
        
        bool isPotion = (potion != nullptr);
        bool isItemUsed;

        if(selection == 1){
            item->print();
        }
        else if(selection == 2){
            if(isPotion == true){
                cout << "Can't equip " << item->getName() << " !!\n";
                return false;
            }
            isItemUsed = this->equip(item);
        }
        else if(selection == 3){
            if(isPotion == false){
                cout << "Can't drink " << item->getName() << " !!\n";
                return false;
            }
            isItemUsed = this->usePotion(potion);
        }

        if(isItemUsed == true){
            inventory->popItem(inventorySlot);
            return true;
        }
        else return false;
    }
    

    return false;

} 



int Hero::inventoryAdd(Item *item){
    return this->inventory->addItem(item);
}



bool Hero::equip(Item *item){

    bool isEquipped = false;

    if(item->getLevel() > this->getLevel()){
        cout << "Couldn't equip " << item->getName()
                << ".. Missing" << item->getLevel() - getLevel()
                << " levels" << endl;
    }

    Weapon *weapon = dynamic_cast<Weapon*>(item);
    if(weapon != nullptr)isEquipped = inventory->equipWeapon(weapon);
    else isEquipped = inventory->equipArmor((Armor*)item);

    if(isEquipped == false){
        cout << "Error ! Couldnt equip item\n";
    }
    else cout << item->getName() << " equipped!" << endl;

    return isEquipped;
}



bool Hero::usePotion(Potion *potion){

    cout << this->getName() << " is drinking " << potion->getName() << endl;

    addBuff(potion->drink());
    
    return true;
}


void Hero::printBuffs(void) const{

    if(buffCounter > 0){
        cout << "*Active buffs: ";
        for(int i = 0; i < buffCounter; i++){
            cout << "# "  << buffs[i]->getName() << " || "
                    <<"Rounds remain: " << buffs[i]->getRounds() << " # ";
        }
        cout << endl;
    }
}


void Hero::roundPass(void){

    regeneration();
    int counter = buffCounter;
    for(int i = 0; i < counter; i++){
        if(buffs[i]->roundPass() == 0){
            removeBuff(i);
        }     
    }
}


int Hero::findBuff(Buff *buff){
    for(int i = 0; i < buffCounter; i++)
        if(buffs[i]->isTheSame(buff))return i;

    return -1;
}


void Hero::addBuff(Buff *buff){

    if(buff == nullptr){
        cout << "Couldn't add buff\n";
    }

    cout << "Buff " << buff->getName();
    int buffSlot = findBuff(buff);

    if(findBuff(buff) == -1){
        /*If buff slots are full, remove the 1st buff */
        if(buffCounter == maxBuffs){
            removeBuff(0);
        }

        cout << " activated!!" << endl;
        int points = buff->getPoints();
        switch(buff->getStat()){
            case strength:
                this->addStats(points, 0, 0);
                break;
            case dexterity:
                this->addStats(0, points, 0); 
                break;
            case agility:
                this->addStats(0, 0, points); 
                break;
        }
        cout << "successfully inscreased " << points
             << " points of " << statsTypeMsg[buff->getStat()] << endl;

        buffSlot = this->buffCounter;
        this->buffCounter++;
    }
    else{/*If buff allready exist , delete it and add the new one */
        cout << " renewed!!" << endl;
        delete buffs[buffSlot];
    }

    buffs[buffSlot] = buff;
}


void Hero::removeBuff(int buffSlot){

    cout << buffs[buffSlot]->getName() << " worn off!!" << endl;

    switch(buffs[buffSlot]->getStat()){
        case strength:
            cout << "strength: " << this->current.str
            << " --> " << this->base.str << endl;
            this->current.str =  this->base.str;
            break;
        case dexterity:
            cout << "dexterity: " << this->current.dex
            << " --> " << this->base.dex << endl;
            this->current.dex =  this->base.dex; 
            break;
        case agility:
            cout << "agility: " << this->current.agi
            << " --> " << this->base.agi << endl;
            this->current.agi =  this->base.agi;
            break;
    }

    delete buffs[buffSlot];

    //Move all buffs one slot left
    for(int i = buffSlot; i < buffCounter-1; i++){
        buffs[i] = buffs[i+1];
    }
    buffCounter--;
}

/* Subclass of Hero -> *Warrior* implementation */

Warrior::Warrior(string name) : Hero(name){
    //*Debug
    cout << "==}== Warior => " << name << " successfully created!!\n";
    //Inscrease strength and agility
    addBaseStats(additionalStr,0,additionalAgi);
    addStats(additionalStr,0,additionalAgi);

}

Warrior::~Warrior(){}


void Warrior::displayStats(void) const{

cout
    << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl
    << " *==}::::::> warrior <::::::{==* " << endl
    << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl
    << "           " << getName() << endl
    <<  "=================================" << endl
    << "||  Level: "<< getLevel() << endl
    << "||  Experience: " << getExp() << "/" << "100" << endl
    << "||  Life: " << getHp() << "/" << getMaxHp() << endl
    << "||  Mana: " << getMp() << "/" << getMaxMp() << endl
    <<  "=================================" << endl
    << "||  Damage: " <<  getDamage()<< endl
    << "||  Defence: " <<  getDefence()<< endl
    <<  "=================================" << endl
    << "||  Strength: " <<  getStr()<< endl
    << "||  Dexterity: " << getDex()  << endl
    << "||  Agility: "  << getAgi()  << endl 
    << "=================================" << endl << endl;

    checkSpells();
    printBuffs();
}


/* Subclass of Hero -> *Sorcerer* implementation */

Sorcerer::Sorcerer(string name) : Hero(name){
    //Inscrease dexterity and agility
    addBaseStats(0,additionalDex,additionalAgi);
    addStats(0,additionalDex,additionalAgi);

}

Sorcerer::~Sorcerer(){
}


void Sorcerer::displayStats(void) const{

    cout 
    << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl
    << " _ * ._/^\\_ Sorcerer _/^\\_. *_ " << endl
    << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl
    << "           " << getName() << endl
    <<  "=================================" << endl
    << "||  Level: "<< getLevel() << endl
    << "||  Experience: " << getExp() << "/" << "100" << endl
    << "||  Life: " << getHp() << "/" << getMaxHp() << endl
    << "||  Mana: " << getMp() << "/" << getMaxMp() << endl
    <<  "=================================" << endl
    << "||  Damage: " <<  getDamage()<< endl
    << "||  Defence: " <<  getDefence()<< endl
    <<  "=================================" << endl
    << "||  Strength: " <<  getStr()<< endl
    << "||  Dexterity: " << getDex()  << endl
    << "||  Agility: "  << getAgi()  << endl 
    << "=================================" << endl << endl;

    checkSpells();
    printBuffs();
}


/* Subclass of Hero -> *Paladin* implementation */

Paladin::Paladin(string name) : Hero(name){
    //Inscrease strength and dexterity
    addBaseStats(additionalStr,additionalDex,0);
    addStats(additionalStr,additionalDex,0);

}

Paladin::~Paladin(){    
}


void Paladin::displayStats(void) const{

    cout 
    << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl
    << "       (|+|) Paladin (|+|) " <<  endl
    << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl
    << "           " << getName() << endl
    <<  "=================================" << endl
    << "||  Level: "<< getLevel() << endl
    << "||  Experience: " << getExp() << "/" << "100" << endl
    << "||  Life: " << getHp() << "/" << getMaxHp() << endl
    << "||  Mana: " << getMp() << "/" << getMaxMp() << endl
    <<  "=================================" << endl
    << "||  Damage: " <<  getDamage()<< endl
    << "||  Defence: " <<  getDefence()<< endl
    <<  "=================================" << endl
    << "||  Strength: " <<  getStr()<< endl
    << "||  Dexterity: " << getDex()  << endl
    << "||  Agility: "  << getAgi()  << endl 
    << "=================================" << endl << endl;

    checkSpells();
    printBuffs();
}

/* Subclass of Living -> *Monster* implementation */

Monster::Monster(string name, int level) : Living(name){

    addLevel(level);
    
    addMaxHp(level * hpPerLevel);addHp(level * hpPerLevel);
    
    //Init damage
    int damage = startingDmg + (level * damagePerLevel);
    baseDmg = currentDmg = 
    {damage, damage + damageRange};

    //Init stats: defence dodge
    int def = startingDef + (level * statsPerLevel);
    int dodge = startingDodge + (level * statsPerLevel);
    base = current = {def, dodge};

    deBuffCounter = 0;

}

Monster::~Monster(){
    for(int i = 0; i < deBuffCounter; i++)
        delete deBuffs[i];
}


int Monster::selectTarget(Hero **heroes, int num){
    
    cout << getName() << " selecting a target..\n";
    int target;

    while(1){
        target = rand()%num;
        if(heroes[target]->getHp() != 0)break;
    }
    cout << "Preparing to attack " << heroes[target]->getName()
         << "..\n";
    return target;
}


int Monster::attack(Living *living) const{

    int dmgRange = this->currentDmg.ub - this->currentDmg.lb;
    int totalDmg = this->currentDmg.lb + rand() % dmgRange;


    cout << this->getName() << " attacks " 
         << living->getName()<< ".."<< endl;
    
    living->receiveDamage(totalDmg);

    return totalDmg;

}



int Monster::receiveDamage(int damage){

    int damageDealt = damage;

    if(damageDealt > 0){

        if(rand()%100 > current.dodge){
            damageDealt -= (this->current.defence / 100.0) * damageDealt;
            cout << this->getName() << " received " 
                    << damageDealt << " damage!" <<  endl;
            subHp(damageDealt);
            
        }
        else{
            damageDealt = 0;
            cout << this->getName() << " Evade attack!! " << endl;
        } 
    }

    if(this->getHp() == 0)goUnconscious();

    return damageDealt;
}


void Monster::printDeBuffs(void) const{

    if(deBuffCounter > 0){
        cout << "*Active debuffs: ";
        for(int i = 0; i < deBuffCounter; i++){
            cout << "# "  << deBuffs[i]->getName() << " || "
                    <<"Rounds remain: " << deBuffs[i]->getRounds() << " # ";

        }
        cout << endl;
            
    }
}


void Monster::goUnconscious(void){
    cout << getName() 
         << " Fell into a state of unconsciousness!!" << endl;

    int counter = deBuffCounter;
    for(int i = 0; i < counter; i++){
            removeDeBuff(i);
    }
}



void Monster::regeneration(void){

    if(getHp() != 0){
      cout << getName() << " regeneration ++ " << endl
           << "hp regen: "
           << getHp() << "-->";

      addHp(getMaxHp()*regenRate);
      cout << getHp() << endl << endl;
    }
    else  cout << getName() << " is Unconscious!" << endl << endl;

}


void Monster::roundPass(void){

    regeneration();
    int counter = deBuffCounter;
    for(int i = 0; i < counter; i++){
        if(deBuffs[i]->roundPass() == 0){
            removeDeBuff(i);
        }     
    }
}


int Monster::findDeBuff(Buff *deBuff){
    for(int i = 0; i < deBuffCounter; i++)
        if(deBuffs[i]->isTheSame(deBuff))return i;

    return -1;
}


void Monster::receiveDeBuff(Buff *deBuff){
    
    if(deBuff == nullptr){
        cout << "Couldn't add debuff\n";
    }


    cout << "DeBuff " << deBuff->getName();
    int deBuffSlot = findDeBuff(deBuff);

    if(findDeBuff(deBuff) == -1){
        /*If debuff slots are full, remove the 1st debuff */
        if(deBuffCounter == maxDeBuffs){
            removeDeBuff(0);
        }

        cout << " activated!!" << endl;

        int points = deBuff->getPoints();
        switch(deBuff->getStat()){
            case Spell::damage:
                subDmg(points);
                break;
            case Spell::defence:
                subDef(points);
                break;
            case Spell::dodge:
                subDodge(points);
                break;
        }

        cout << Spell::statMsg[deBuff->getStat()] 
        << " successfully descreased " << points << " points!\n";

        deBuffSlot = this->deBuffCounter;
        this->deBuffCounter++;
    }
    else{/*If debuff allready exist , don't add more points to stat affected*/
        cout << " renewed!!" << endl;
        delete deBuffs[deBuffSlot];
    }

    deBuffs[deBuffSlot] = deBuff;

}


void Monster::removeDeBuff(int deBuffSlot){


    cout << deBuffs[deBuffSlot]->getName() << " worn off!!" << endl;

    switch(deBuffs[deBuffSlot]->getStat()){
        case Spell::damage:
            cout << "damage: " << this->currentDmg.lb 
            << " - " << this->currentDmg.ub
            << " --> ";
            this->currentDmg = this->baseDmg;
            cout << this->currentDmg.lb 
            << " - " << this->currentDmg.ub << endl;
            break;
        case Spell::defence:
            cout << "defence: " << this->current.defence
            << " --> ";
            this->current.defence = this->base.defence;
            cout << this->current.defence << endl;
            break;
        case Spell::dodge:
            cout << "dodge: " << this->current.dodge
            << " --> ";
            this->current.dodge = this->base.dodge;
            cout << this->current.dodge << endl;
            break;
    }
    delete deBuffs[deBuffSlot];

    //Move all buffs one slot left
    for(int i = deBuffSlot; i < deBuffCounter-1; i++){
        deBuffs[i] = deBuffs[i+1];
    }
    deBuffCounter--;
    
}



/* Subclass of Monster -> *Dragon* implementation */

Dragon::Dragon(string name, int level) : Monster(name, level){

    cout << "A wild dragon appeared out of the sky..!!" << endl;

    //Inscrease the damage
    addBaseDmg(additionalDmg);
    addDmg(additionalDmg);

}

Dragon::~Dragon(){}


void Dragon::displayStats(void) const{

cout
    << "---------------------------------" << endl
    << " /^.^^.^\\_  Dragon  _//^.^^.^\\ "  << endl
    << "---------------------------------" << endl
    << "           " << getName() << endl
    <<  "=================================" << endl
    << "||  Level: "<< getLevel() << endl
    << "||  Life: " << getHp() << "/" << getMaxHp() << endl
    <<  "==================================" << endl
    << "||  Damage: " << getDmgLb() << " - " 
    <<  getDmgUb() << endl
    << "||  Defence: " << getDef() << endl
    << "||  Dodge: "  << getDodge() << endl
    << "==================================" << endl << endl;

    printDeBuffs();
}

/* Subclass of Monster -> *Exoskeleton* implementation */

Exoskeleton::Exoskeleton(string name, int level) : Monster(name, level){
    //*Debug
    cout << "A wild exoskeleton appeared out of shadows..!!" << endl;

    //Inscrease defence
    addBaseStats(additionalDef,0);
    addStats(additionalDef,0);
}

Exoskeleton::~Exoskeleton(){
    cout << "An exoskeleton dissapeared!! " << endl;
}

void Exoskeleton::displayStats(void) const{

cout
    << "---------------------------------" << endl
    << "   |====|= Exoskeleton =|====| "  << endl
    << "---------------------------------" << endl
    << "         " << getName() << endl
    << "==================================" << endl
    << "||  Level: "<< getLevel() << endl
    << "||  Life: " << getHp() << "/" << getMaxHp() << endl
    <<  "==================================" << endl
    << "||  Damage: " << getDmgLb() << " - " 
    <<  getDmgUb() << endl
    << "||  Defence: " << getDef() << endl
    << "||  Dodge: "  << getDodge() << endl
    << "==================================" << endl << endl;

    printDeBuffs();
}


/* Subclass of Monster -> *Spirit* implementation */

Spirit::Spirit(string name, int level) : Monster(name, level){

    cout << "A wild spirit came out of thin air..!!" << endl;

    //Inscrease dodge
    addBaseStats(0,additionalDodge);
    addStats(0,additionalDodge);

}

Spirit::~Spirit(){

    cout << "A spirit dissapeared!!" << endl;
}

void Spirit::displayStats(void) const{

    cout 
    << "---------------------------------" << endl
    << "   ~~~~~~~ Spirit ~~~~~~~  " << endl
    << "---------------------------------" << endl
    << "           " << getName() << endl
    << "==================================" << endl
    << "||  Level: "<< getLevel() << endl
    << "||  Life: " << getHp() << "/" << getMaxHp() << endl
    <<  "==================================" << endl
    << "||  Damage: " << getDmgLb() << " - " 
    <<  getDmgUb() << endl
    << "||  Defence: " << getDef() << endl
    << "||  Dodge: "  << getDodge() << endl
    << "==================================" << endl << endl;
    
    printDeBuffs();
}