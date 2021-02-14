#include <algorithm> //upper_bound
#include <time.h>    
#include "living.h"
#include "spell.h"
#include "item.h"

using namespace std;

/* Base class *Living* implementation*/

string Hero::statsMsg[3] = {"strength", "dexterity", "agility"};
string Hero::statusMsg[4] = {"succeed", "notFound", "higherLevel", "wrongType"};

Living::Living(string name)
{ 
    //*Debug
    cout << "Creating a Living..!" << endl;
    this->name = name;
    this->level = 1;
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
    this->current = this->base = {10,10,10};

    maxMp = mp = 500;
    experience = 0;
    
    int damage = 120, price = 10, requiredLvl = 1;
    Weapon *weapon = new Weapon("Wooden Sword", damage, price, requiredLvl);

    int defence = 15; price = 20;
    Armor *armor = new Armor("Wooden Armor", defence, price, requiredLvl);

    int money = 100;
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
    return damageDealt;

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


void Hero::checkInventory(void) const{
    this->inventory->print();
} 


int Hero::inventoryAdd(Item *item){
    return this->inventory->addItem(item);
}



int Hero::equip(int inventorySlot){

    int result;
    
    Item *item = inventory->getItem(inventorySlot);
    if(item == nullptr) return notFound;


    if(item->getType() == Item::weapon){
        if(item->getLevel() > this->getLevel())result = higherLevel;
        else result = this->inventory->equipWeapon((Weapon*)item);
    }
    else if(item->getType() == Item::armor){
        if(item->getLevel() > this->getLevel())result = higherLevel;
        else result = this->inventory->equipArmor((Armor*)item);
    }
    else return wrongType;
    
    if(result == succeed)inventory->popItem(inventorySlot);

    return result;
}



int Hero::usePotion(int inventorySlot){
    int result = -1;
    
    Item *item = inventory->getItem(inventorySlot);
    if(item == nullptr) return notFound;

    /* Check if item is weapon */

    if(item->getType() == Item::potion){
        if(item->getLevel() > this->getLevel())result = higherLevel;
        else result = succeed;
    }
    else return wrongType;
    
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
        cout << "successfully restored " << potion->getPoints()
             << " points of " << statsMsg[potion->getStat()] << endl;
        //delete inventory->popItem(inventorySlot);
    }

    return result;

}


void Hero::print(void) const{
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
    addBaseStats(5,0,5);
    addStats(5,0,5);

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
    addBaseStats(0,5,5);
    addStats(0,5,5);


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
    addBaseStats(5,5,0);
    addStats(5,5,0);

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
    baseDmg = currentDmg = {30, 80};

    //Init stats: defence dodge
    base = current = {10, 10};

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
    return damageDealt;

}

void Monster::print(void) const{
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
    addBaseDmg(20);
    addDmg(20);

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
    addBaseStats(10,0);
    addStats(10,0);
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
    addBaseStats(0,10);
    addStats(0,10);

}

Spirit::~Spirit(){
    //*Debug
    cout << "A spirit to be destroyed " << endl;
}
