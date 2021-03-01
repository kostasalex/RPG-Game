#include "item.h"
#include "buff.h"

using namespace std;
/* Base class *Item* implementation */

Item::Item(string name, int price, int requiredLevel)
{
    this->name = name;
    this->price = price;
    this->requiredLevel = requiredLevel;
}

Item::~Item (){}


/* Subclass *Potion* implementation */
const string Potion::statTypeMsg[3] = {"strength", "dexterity", "agility"};

Potion::Potion(string name, int stat, \
int price, int points, int requiredLevel) : 
Item(name, price, requiredLevel)
{
    this->stat = stat;
    this->points = points;
    this->rounds = (requiredLevel/2) + buffRounds;
}

Potion::~Potion(){}


const string Potion::buffNames[3] =
    {"Power boost", "Magic boost", "Dodge boost"};
    
Buff *Potion::drink(void){
    return new Buff(buffNames[stat], rounds, stat, points);
}

void Potion::print() const{
    
    cout << "Potion: " << getName() << endl
         << "Effect: +" << points << " " 
         << statTypeMsg[stat] << endl 
         << "Required level: " << getLevel() << endl
         << "Price: " << getPrice() << endl;
}


/* Subclass *Weapon* implementation */

Weapon::Weapon(string name, int damage, \
int price, int requiredLevel, int handsRequired)
: Item(name, price, requiredLevel)
{
    this->damage = damage;
    this->handsRequired = handsRequired;
}

Weapon::~Weapon(){}


void Weapon::print()const {

    cout << "Weapon: " << getName() << endl
         << "Damage: " << this->damage << endl 
         << "Required level: " << getLevel() << endl
         << "Price: " << getPrice() << endl;

}


/* Subclass *Armor* implementation */

Armor::Armor(string name, int defence, int price, int requiredLevel)
: Item(name, price, requiredLevel)
{
    this->defence = defence;
}

Armor::~Armor(){}


void Armor::print()const {

    cout << "Armor: " << getName() << endl
         << "Defence: " << this->defence << endl 
         << "Required level: " << getLevel() << endl
         << "Price: " << getPrice() << endl;

}



Inventory::Inventory(Weapon *weaponToequip, Armor *armorToEquip, int money)
{   
    this->weapon = nullptr;
    this->handSlot[0] = this->handSlot[1] = nullptr;
    this->armor = nullptr;
    equipWeapon(weaponToequip);
    equipArmor(armorToEquip);
    this->money = money;

}


Inventory::~Inventory(){}


void Inventory::disarmWeapon(void){
    if(this->weapon != nullptr){
        items.push_back(this->weapon);
        if(handSlot[0] == weapon)handSlot[0] = nullptr;
        if(handSlot[1] == weapon)handSlot[1] = nullptr;
        this->weapon = nullptr;
    }
}


void Inventory::disarmArmor(void){
    if(this->armor == nullptr)return;
    items.push_back(this->armor);
    this->armor = nullptr;
}


bool Inventory::equipWeapon(Weapon *weapon){
    
    /* If can't equip weapon */
    if(weapon == nullptr)return false;
    
    /* If weapon can be equipped */
    disarmWeapon();
    this->weapon = weapon;

    handSlot[0] = weapon; 
    if(weapon->getHandsRequired() == Weapon::twoHanded)
        handSlot[1] = weapon; 


    return true;
}


bool Inventory::equipArmor(Armor *armor){
    
    /* If can't equip armor */
    if(armor == nullptr)return false;
    
    /* If armor can be equipped */
    disarmArmor();
    this->armor = armor;

    return true;
}


Item* Inventory::popItem(int inventorySlot){
    
    if((long unsigned int)inventorySlot >= items.size())
        return nullptr;

    Item* item = items[inventorySlot];

    items.erase(items.begin() + inventorySlot);

    return item;

}


Item* Inventory::getItem(int inventorySlot){
    
    if((long unsigned int)inventorySlot >= items.size())
        return nullptr;

    return items[inventorySlot];

}


int Inventory::addItem(Item *item){

    if(item == NULL)return false;

    items.push_back(item);

    return true;
}


void Inventory::print(void) const{

    cout << "** Inventory **" << endl
        << "-----------------------------------" << endl
        << "Equipped Weapon: \n";
    if(this->weapon != nullptr){
        if(handSlot[0] == this->weapon)
            cout << "   Right hand: " << handSlot[0]->getName() << endl;
        if(handSlot[1] == this->weapon)
            cout << "   Left  hand: " << handSlot[1]->getName() << endl;
    }
    else cout << " - " << endl;

    cout << endl;

    cout << "Equipped Armor: ";
    if(this->armor != nullptr)
        cout << this->armor->getName() << endl;
    else cout << " - " << endl ;

    cout << "-----------------------------------" << endl;
    
    int i = 0;
    
    for (auto item = items.begin(); item != items.end(); item++, i++) {
        cout << "inventory slot[" << i+1 << "]: "
             << (*item)->getName() << endl; 
    }

    if(items.size() > 0)
        cout << "-----------------------------------" << endl;

    cout << "money: " << money << endl;

    cout << "-----------------------------------" << endl;
}