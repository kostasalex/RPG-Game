#include <algorithm> //upper_bound
#include <time.h>    
#include "living.h"
#include "spell.h"
#include "utils.h"
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

Living::~Living(){
}


/* Subclass of Living -> *Hero* implementation */
Hero::Hero(string name) : Living(name){

    /* Init strength, dexterity, agility */
    this->current = 
    this->base = {startingStr, startingDex, startingAgi};

    buffCounter = 0;

    maxMp = mp = startingMp;
    experience = startingExperience;
    
    int damage = 1000, price = 10, requiredLvl = 1;
    Weapon *weapon = 
    new Weapon("Wooden Sword", damage, price, requiredLvl, Weapon::oneHanded);

    int defence = 15; price = 20;
    Armor *armor = new Armor("Wooden Armor", defence, price, requiredLvl);
    
    int money = startingMoney;
    this->inventory = new Inventory(weapon, armor, money);
}


Hero::~Hero(){
    for(int i = 0; i < buffCounter; i++)
        delete buffs[i];
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


bool Hero::castSpell(int spellId, Living *living) {


    if(spellId < (int)this->spells.size()){
        Spell* spell = spells[spellId];

        if(spell->getMp() > this->mp){
            cout << "Not enough mp!" << endl;
            return false;
        }
        else{
            cout << this->getName() << " casting "
                 << spell->getName() << " to " 
                 << living->getName() << endl;

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


int Hero::attack(Living *living) const{

    int totalDmg;

    Weapon *weapon = this->inventory->getWeapon();
    if(weapon == nullptr)totalDmg = 0;
    else totalDmg = weapon->getDamage();

    totalDmg += (this->current.str / 100.0) * totalDmg; 
    cout << this->getName() << " attacks " <<
        living->getName() << endl;
    
    living->receiveDamage(totalDmg);
    return totalDmg;

}



int Hero::receiveDamage(int damage){

    int damageDealt = damage;

    if(damageDealt > 0){

        if(rand()%100 > agility){
            Armor *armor = this->inventory->getArmor();
            if(armor != nullptr){
                damageDealt -= (armor->getDefence() / 100.0) * damageDealt;
                cout << this->getName() << " received " 
                     << damageDealt << " damage!" <<  endl;
                subHp(damageDealt);
            }
        }
        else{
            damageDealt = 0;
            cout << this->getName() << " Evade attack!! " << endl;
        } 
    }

    if(this->getHp() == 0)goUnconscious();

    return damageDealt;

}


void Hero::receiveExperience(int exp){

    int result = getExp() + exp;

    cout << getName() << " received "
         << exp << " experience!" << endl;

    if(result >= maxExperience){
        if(getLevel() == maxLevel){
            result = maxExperience;
        }
        else{
            result = (getExp() + exp) - maxExperience;
            setExp(0);
            levelUp();
        }

    }

    cout << getExp() << " --> " << result << endl;
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



void Hero::goUnconscious(void){
    cout << "Fell into a state  of unconsciousness!!" << endl;
    int counter = buffCounter;
    for(int i = 0; i < counter; i++){
            removeBuff(i);
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
    << this->mp + mpPerLevel << "/" << this->maxMp + mpPerLevel << endl << endl;

    addStats(statsPerLevel, statsPerLevel, statsPerLevel);
    addBaseStats(statsPerLevel, statsPerLevel, statsPerLevel);
    addMaxHp(hpPerLevel);addHp(hpPerLevel);
    addMaxMp(mpPerLevel);addMp(mpPerLevel);

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
            else cout << item->getName() << " requires level "
                      << item->getLevel() << endl;
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


bool Hero::findSpell(Spell *spell){
    for(int i = 0; i < (int)spells.size(); i++){
        if(spell == spells[i])return true;
    }
    return false;
}   


bool Hero::checkInventory(bool equip, bool drinkPotion){
    
    inventory->print();
    int size;

    if(inventory->getSize() != 0 ){

        if(equip == true){
            cout << "[1] Equip Item" << endl;
            size = 2;
        }
        if(drinkPotion == true){
            cout << "[2] Drink a potion "<< endl;
            size = 3;
        }
        if(drinkPotion == true || equip == true){
            
            int selection;

            cout << "[0] Go back" << endl
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
                if(isPotion == true){
                    cout << "Can't equip " << item->getName() << " !!\n";
                    return false;
                }
                isItemUsed = this->equip(item);
            }
            else if(selection == 2){
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
    }

    return false;

} 


void Hero::checkStats(void) const{

    cout << "==================================" << endl
    << "Level: "<< getLevel() << endl
    << "Experience: " << this->experience << "/" << "100" << endl
    << "Life: " << getHp() << "/" << getMaxHp() << endl
    << "Mana: " << this->mp << "/" << this->maxMp << endl
    << endl << "----------------------------------" << endl
    << ">> Strength: " << this->current.str << endl
    << ">> Dexterity: " << this->current.dex  << endl
    << ">> Agility: "  << this->current.agi  << endl 
    << "==================================" << endl << endl;

    printBuffs();


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


void Hero::print(void){
    cout << "||Hero <" << getName() << ">||" << endl
    << "Level: "<< getLevel() << endl
    << "Experience: " << this->experience << "/" << "100" << endl
    << "Life: " << getHp() << "/" << getMaxHp() << endl
    << "Mana: " << this->mp << "/" << this->maxMp << endl
    << endl << "* Stats *" << endl
    << ">> Strength: " << this->current.str << endl
    << ">> Dexterity: " << this->current.dex  << endl
    << ">> Agility: "  << this->current.agi  << endl << endl;
    this->checkInventory();
    this->checkSpells();
    this->printBuffs();
}


/* Subclass of Hero -> *Warrior* implementation */

Warrior::Warrior(string name) : Hero(name){
    //*Debug
    cout << "Warrior " << name << " successfully created!!\n";
    //Inscrease strength and agility
    addBaseStats(additionalStr,0,additionalAgi);
    addStats(additionalStr,0,additionalAgi);

}

Warrior::~Warrior(){
}


/* Subclass of Hero -> *Sorcerer* implementation */

Sorcerer::Sorcerer(string name) : Hero(name){
    //*Debug
    cout << "Sorcerer " << name << " successfully created!!\n";
    //Inscrease dexterity and agility
    addBaseStats(0,additionalDex,additionalAgi);
    addStats(0,additionalDex,additionalAgi);

}

Sorcerer::~Sorcerer(){
}


/* Subclass of Hero -> *Paladin* implementation */

Paladin::Paladin(string name) : Hero(name){
    //*Debug
    cout << "Paladin " << name << " successfully created!!\n";
    //Inscrease strength and dexterity
    addBaseStats(additionalStr,additionalDex,0);
    addStats(additionalStr,additionalDex,0);

}

Paladin::~Paladin(){    
}


/* Subclass of Living -> *Monster* implementation */

Monster::Monster(string name) : Living(name){
    //*Debug
    cout << "Becoming a monster..!" << endl;

    //Init damage
    baseDmg = currentDmg = 
    {startingDmg, startingDmg + damageRange};

    //Init stats: defence dodge
    base = current = {startingDef, startingDodge};

    deBuffCounter = 0;

}

Monster::~Monster(){
    for(int i = 0; i < deBuffCounter; i++)
        delete deBuffs[i];
}


int Monster::attack(Living *living) const{

    int dmgRange = this->currentDmg.ub - this->currentDmg.lb;
    int totalDmg = this->currentDmg.lb + rand() % dmgRange;


    cout << this->getName() << " attacks " <<
        living->getName() << endl;
    
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
      cout << getHp() << endl;
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

        cout << "successfully descreased " << points
             << " points of " << Spell::statMsg[deBuff->getStat()] << endl;

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


void Monster::print(void) {
    cout << "((Monster <" << getName() << ">))" << endl
    << "Level: "<< getLevel() << endl
    << "Life: " << getHp() << "/" << getMaxHp() << endl
    << endl << "* Stats *" << endl

    << "Damage: " << this->currentDmg.lb << " - " 
    <<  this->currentDmg.ub << endl
    << "Defence: " << this->current.defence << endl
    << "Dodge: "  << this->current.dodge << endl;

    printDeBuffs();
}


/* Subclass of Monster -> *Dragon* implementation */

Dragon::Dragon(string name) : Monster(name){
    //*Debug
    cout << "Becoming a Dragon..!" << endl;

    //Inscrease the damage
    addBaseDmg(additionalDmg);
    addDmg(additionalDmg);

}

Dragon::~Dragon(){
    //*Debug
    cout << "A dragon to be destroyed " << endl;
}


/* Subclass of Monster -> *Exoskeleton* implementation */

Exoskeleton::Exoskeleton(string name) : Monster(name){
    //*Debug
    cout << "Becoming a Dragon..!" << endl;

    //Inscrease defence
    addBaseStats(additionalDef,0);
    addStats(additionalDef,0);
}

Exoskeleton::~Exoskeleton(){
    //*Debug
    cout << "An exoskeleton to be destroyed " << endl;
}


/* Subclass of Monster -> *Spirit* implementation */

Spirit::Spirit(string name) : Monster(name){
    //*Debug
    cout << "Becoming a Dragon..!" << endl;

    //Inscrease dodge
    addBaseStats(0,additionalDodge);
    addStats(0,additionalDodge);

}

Spirit::~Spirit(){
    //*Debug
    cout << "A spirit to be destroyed " << endl;
}
