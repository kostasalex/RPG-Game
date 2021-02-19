#include <algorithm> //upper_bound
#include <time.h>    
#include "living.h"
#include "spell.h"
#include "utils.h"

using namespace std;

/* Base class *Living* implementation*/

string Hero::statusMsg[4] = {"succeed", "notFound", "higherLevel", "wrongType"};
const string Hero::buffTypeMsg[Potion::potionTypes] =
    {"Power boost", "Magic boost", "Dodge boost"};

const string Monster::deBuffTypeMsg[Spell::spellTypes] = 
    {"Power curse", "Defence curse", "Dodge curse"};

Living::Living(string name)
{ 
    //*Debug
    cout << "Creating a Living..!" << endl;
    this->name = name;
    this->level = startingLevel;
    this->hp = this->maxHp = startingHp; 
}

Living::~Living(){
    //*Debug
    cout << "A living to be destroyed" << endl;
}


/* Subclass of Living -> *Hero* implementation */

Hero::Hero(string name) : Living(name){
    //*Debug
    cout << "Becoming a Hero..!" << endl;

    /* Init strength, dexterity, agility */
    this->current = 
    this->base = {startingStr, startingDex, startingAgi};

    this->buffs[Potion::potionTypes] = {};

    maxMp = mp = startingMp;
    experience = startingExperience;
    
    int damage = 1000, price = 10, requiredLvl = 1;
    Weapon *weapon = new Weapon("Wooden Sword", damage, price, requiredLvl);

    int defence = 15; price = 20;
    Armor *armor = new Armor("Wooden Armor", defence, price, requiredLvl);

    int money = startingMoney;
    this->inventory = new Inventory(weapon, armor, money);


}

Hero::~Hero(){
    //*Debug
    cout << "A Hero to be destroyed" << endl; 
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


int Hero::castSpell(int spellId, Living *living) {


    if(spellId < (int)this->spells.size()){
        Spell* spell = spells[spellId];

        if(spell->getMp() > this->mp){
            cout << "Not enough mp!" << endl;
            return -1;
        }
        else{
            cout << this->getName() << " casting "
                 << spell->getName() << " to " 
                 << living->getName() << endl;

            int targetStat, points, damage;
            spell->cast(this->dexterity, targetStat, points, damage);

            this->subMp(spell->getMp());

            //If damage received and it's still alive cast debuff
            if(living->receiveDamage(damage) != 0 && living->getHp()!= 0 ){
                Monster *monster = (Monster*)living;
                monster->receiveDeBuff(targetStat, points);
            }
        }
    }
    else {
        cout << "Spell not found!" << endl;
        return notFound;
    }
    return succeed;
}


int Hero::attack(Living *living) const{

    int totalDmg;

    Weapon *weapon = this->inventory->getWeapon();
    if(weapon == nullptr)totalDmg = 0;
    else totalDmg = weapon->getDamage();

    totalDmg +=(this->strength / 100.0) * totalDmg; 
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

    for(int i = 0; i < Potion::potionTypes; i++){
        if(buffs[i] != 0){
            buffs[i] = 0;
            removeBuff(i);
        }
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

            if(selection == 1){
                if(this->equip(inventorySlot) == succeed)return true;
            }
            else if(selection == 2){
                if(this->usePotion(inventorySlot) == succeed)return true;
            }
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



int Hero::equip(int inventorySlot){

    int result;
    
    Item *item = inventory->getItem(inventorySlot);
    if(item == nullptr)result = notFound;
    else{

        if(item->getType() == Item::weapon){
            if(item->getLevel() > this->getLevel())result = higherLevel;
            else result = this->inventory->equipWeapon((Weapon*)item);
        }
        else if(item->getType() == Item::armor){
            if(item->getLevel() > this->getLevel())result = higherLevel;
            else result = this->inventory->equipArmor((Armor*)item);
        }
        else result = wrongType;
        
    }

    switch(result){

        case succeed:
            cout << item->getName() << " equipped!" << endl;
            inventory->popItem(inventorySlot);
            break;

        case notFound:
            cout << "There is no item in slot "<< inventorySlot << endl;
            break;

        case higherLevel:
            cout << "Couldn't equip " << item->getName()
                 << ".. Missing" << item->getLevel() - getLevel()
                 << " levels" << endl;
            break;

        case wrongType:
            cout << "Can't equip " << Item::itemTypeMsg[item->getType()] << endl;
        
        
    }

    return result;
}



int Hero::usePotion(int inventorySlot){
    int result = -1;
    
    Item *item = inventory->getItem(inventorySlot);
    if(item == nullptr) return notFound;

    /* Check if item is potion */

    if(item->getType() == Item::potion){
        if(item->getLevel() > this->getLevel())result = higherLevel;
        else result = succeed;
    }
    else{
        cout << "Can't drink " << Item::itemTypeMsg[item->getType()] << ".." << endl;
        return wrongType;
    } 
    
    if(result == succeed){
        inventory->popItem(inventorySlot);
        Potion* potion = (Potion*)item;
        addBuff(potion->getStat(), potion->getPoints());
        //delete inventory->popItem(inventorySlot);
    }

    return result;

}


void Hero::printBuffs(void) const{
    bool buffExist = false;

    for(int i = 0; i < Potion::potionTypes; i++)
        if(buffs[i] != 0)buffExist = true;

    if(buffExist == true){
        cout << "*Active buffs: ";
        for(int i = 0; i < Potion::potionTypes; i++){
            if(buffs[i] != 0){
                cout << "# "  << buffTypeMsg[i] << " || "
                     <<"Rounds remain: " << buffs[i] << " # ";
            } 
        }
        cout << endl;
            
    }
}


void Hero::roundPass(void){

    regeneration();

    for(int i = 0; i < Potion::potionTypes; i++){
        if(buffs[i] > 0){
            buffs[i]--;
            if(buffs[i] == 0){
                removeBuff(i);
            }
        }
    }
}



void Hero::addBuff(int buffType, int points){

    if(buffType < 0 || buffType > Potion::potionTypes){
        cout << "Couldn't add buff.. Invalid buffType" << endl;
        return;
    }

    cout << "Buff " << buffTypeMsg[buffType];
    if(this->buffs[buffType] == 0){
        cout << " activated!!" << endl;
        switch(buffType){
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
             << " points of " << Potion::statsTypeMsg[buffType] << endl;
    }
    else{/*If buff allready exist , don't add more points to stat affected*/
        cout << " renewed!!" << endl;
    }

    this->buffs[buffType] = buffRounds;

}


void Hero::removeBuff(int buffType){

    if(buffType < 0 || buffType > Potion::potionTypes){
        cout << "Couldn't remove buff.. Invalid buffType" << endl;
        return;
    }

    cout << buffTypeMsg[buffType] << " worn off!!" << endl;

    switch(buffType){
        case strength:
            cout << "strength: " << this->current.str
            << " --> " << this->base.str << endl;
            this->current.str =  this->base.str;
            break;
        case dexterity:
            cout << "strength: " << this->current.str
            << " --> " << this->base.str << endl;
            this->current.dex =  this->base.dex; 
            break;
        case agility:
            cout << "strength: " << this->current.str
            << " --> " << this->base.str << endl;
            this->current.agi =  this->base.agi;
            break;
    }
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
}


/* Subclass of Hero -> *Warrior* implementation */

Warrior::Warrior(string name) : Hero(name){
    //*Debug
    cout << "Becoming a warrior..!" << endl;
    //Inscrease strength and agility
    addBaseStats(additionalStr,0,additionalAgi);
    addStats(additionalStr,0,additionalAgi);

}

Warrior::~Warrior(){
    //*Debug
    cout << "A warrior to be destroyed " << endl;
}


/* Subclass of Hero -> *Sorcerer* implementation */

Sorcerer::Sorcerer(string name) : Hero(name){
    //*Debug
    cout << "Becoming a sorcerer..!" << endl;
    //Inscrease dexterity and agility
    addBaseStats(0,additionalDex,additionalAgi);
    addStats(0,additionalDex,additionalAgi);

}

Sorcerer::~Sorcerer(){
    //*Debug
    cout << "A sorcerer to be destroyed " << endl;
}


/* Subclass of Hero -> *Paladin* implementation */

Paladin::Paladin(string name) : Hero(name){
    //*Debug
    cout << "Becoming a paladin..!" << endl;
    //Inscrease strength and dexterity
    addBaseStats(additionalStr,additionalDex,0);
    addStats(additionalStr,additionalDex,0);

}

Paladin::~Paladin(){    
    //*Debug
    cout << "A paladin to be destroyed " << endl;
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

}

Monster::~Monster(){
    //*Debug
    cout << "A monster to be destroyed " << endl;
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
    bool buffExist = false;

    for(int i = 0; i < Spell::spellTypes; i++)
        if(deBuffs[i] != 0)buffExist = true;

    if(buffExist == true){
        cout << "*Active debuffs: ";
        for(int i = 0; i < Potion::potionTypes; i++){
            if(deBuffs[i] != 0){
                cout << "# "  << deBuffTypeMsg[i] << " || "
                     <<"Rounds remain: " << deBuffs[i] << " # ";
            } 
        }
        cout << endl;
            
    }
}


void Monster::goUnconscious(void){
    cout << getName() 
         << " Fell into a state of unconsciousness!!" << endl;

    for(int i = 0; i < Spell::spellTypes; i++){
        if(deBuffs[i] != 0){
            deBuffs[i] = 0;
            removeDeBuff(i);
        }
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

    for(int i = 0; i < Spell::spellTypes; i++){
        if(deBuffs[i] > 0){
            deBuffs[i]--;
            if(deBuffs[i] == 0){
                removeDeBuff(i);
            }
        }
    }

}


void Monster::receiveDeBuff(int deBuffType, int points){

    if(deBuffType < 0 || deBuffType > Spell::spellTypes){
        cout << "Couldn't add deBuff.. Invalid buffType" << endl;
        return;
    }

    cout << "DeBuff " << deBuffTypeMsg[deBuffType];
    if(this->deBuffs[deBuffType] == 0){
        cout << " activated!!" << endl;
        switch(deBuffType){
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
             << " points of " << Spell::statMsg[deBuffType] << endl;
    }
    else{/*If buff allready exist , don't add more points to stat affected*/
        cout << " renewed!!" << endl;
    }

    this->deBuffs[deBuffType] = deBuffRounds;

}


void Monster::removeDeBuff(int deBuffType){

    if(deBuffType < 0 || deBuffType > Spell::spellTypes){
        cout << "Couldn't remove debuff.. Invalid debuff type" << endl;
        return;
    }

    cout << deBuffTypeMsg[deBuffType] << " worn off!!" << endl;

    switch(deBuffType){
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
