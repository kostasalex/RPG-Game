#include <algorithm> //upper_bound
#include <time.h>    
#include "living.h"
#include "spell.h"
#include "utils.h"

using namespace std;

/* Base class *Living* implementation*/

string Hero::statsMsg[3] = {"strength", "dexterity", "agility"};
string Hero::statusMsg[4] = {"succeed", "notFound", "higherLevel", "wrongType"};

Living::Living(string name)
{ 
    //*Debug
    cout << "Creating a Living..!" << endl;
    this->name = name;
    this->level = 3;
    this->hp = this->maxHp = 1000; 
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

    maxMp = mp = 500;
    experience = 0;
    
    int damage = 1000, price = 10, requiredLvl = 1;
    Weapon *weapon = new Weapon("Wooden Sword", damage, price, requiredLvl);

    int defence = 15; price = 20;
    Armor *armor = new Armor("Wooden Armor", defence, price, requiredLvl);

    int money = 200;
    this->inventory = new Inventory(weapon, armor, money);


}

Hero::~Hero(){
    //*Debug
    cout << "A Hero to be destroyed" << endl; 
}


bool Hero::learnSpell(Spell *spell){

    if(this->getLevel() >= spell->getLvl()){
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

            //If damage received cast debuff
            if(living->receiveDamage(damage) != 0){
                Monster *monster = (Monster*)living;
                switch(targetStat){
                    case Spell::damage:
                        monster->subDmg(points);
                        break;
                    case Spell::defence:
                        monster->subDef(points);
                        break;
                    case Spell::dodge:
                        monster->subDodge(points);
                        break;

                }
                cout << Spell::statMsg[targetStat] 
                    << " descreased by " << points << endl;
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

    if(this->getHp() == 0) cout << " Fell into a state of unconsciousness!!" << endl;

    return damageDealt;

}


void Hero::receiveExperience(int exp){

    int result;

    cout << getName() << " received "
         << exp << " experience!" << endl;

    if((getExp() + exp) >= 100){

        levelUp();

        int result = (getExp() + exp) - 100;

        cout << 0 << " --> " << result << endl;

    }
    else{
        result = getExp() + exp;
        cout << getExp() << " --> " << result << endl;
    }

    setExp(result);

}


void Hero::levelUp(void){

    cout << "Level up!!" << endl;
    cout << getLevel() << " --> ";
    addLevel(1);
    cout << getLevel() << endl;
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
            cout << "Can't equip " << Item::types[item->getType()] << endl;
        
        
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
        cout << "Can't drink " << Item::types[item->getType()] << ".." << endl;
        return wrongType;
    } 
    
    if(result == succeed){
        inventory->popItem(inventorySlot);
        Potion* potion = (Potion*)item;
        int points = potion->getPoints();

        switch(potion->getStat()){
            case strength:
                this->addStats(points, 0, 0);
                break;
            case dexterity:
                this->addStats(0, points, 0); 
                break;
            case agility:
                this->addStats(0, 0, agility); 
                break;
        }
        cout << "successfully inscreased " << potion->getPoints()
             << " points of " << statsMsg[potion->getStat()] << endl;
        //delete inventory->popItem(inventorySlot);
    }

    return result;

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

    if(this->getHp() == 0) cout << "Fell into a state of unconsciousness!!"
                    << endl << endl;

    return damageDealt;
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
