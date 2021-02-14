#include "item.h"

using namespace std;
//!Temporary
string statsMsg[3] = {"strength", "dexterity", "agility"};

/* Base class *Item* implementation */

Item::Item(string name, int price, int requiredLevel, int type)
{
    this->name = name;
    this->price = price;
    this->requiredLevel = requiredLevel;
    this->type = type;

    //*Debug
    cout << "A new item constructed " << endl;
}

Item::~Item ()
{
    //*Debug
    cout << "Item " << this->name << " to be destructed!!" << endl;
}


/* Subclass *Potion* implementation */

Potion::Potion(string name, int stat, \
int price, int points, int requiredLevel) : 
Item(name, price, requiredLevel, potion)
{
    this->stat = stat;
    this->points = points;

    //*Debug
    print();

}

Potion::~Potion(){
    
    //*debug
    cout << "A potion to be destructed!!" << endl;
    print();

}

void Potion::print() const{
    
    cout << "Potion: " << getName() << endl
         << "Adding " << points << " points to " <<statsMsg[stat] << endl 
         << "Required level: " << getLevel() << endl
         << "Price: " << getPrice() << endl;

}


/* Subclass *Weapon* implementation */

Weapon::Weapon(string name, int damage, int price, int requiredLevel)
: Item(name, price, requiredLevel, weapon)
{
    this->damage = damage;
    print();
}

Weapon::~Weapon(){
    
    //*debug
    cout << "A weapon to be destructed!!" << endl;
    print();

}

void Weapon::print()const {

    cout << "Weapon: " << getName() << endl
         << "With damage " << this->damage << endl 
         << "Required level: " << getLevel() << endl
         << "Price: " << getPrice() << endl;

}


/* Subclass *Armor* implementation */

Armor::Armor(string name, int defence, int price, int requiredLevel)
: Item(name, price, requiredLevel, armor)
{
    this->defence = defence;
    print();
}

Armor::~Armor(){
    
    //*debug
    cout << "An armor to be destructed!!" << endl;
    print();

}

void Armor::print()const {

    cout << "Armor: " << getName() << endl
         << "With defence " << this->defence << endl 
         << "Required level: " << getLevel() << endl
         << "Price: " << getPrice() << endl;

}



Inventory::Inventory(Weapon *weaponToequip, Armor *armorToEquip, int money)
{
    this->weapon = weaponToequip;
    this->armor = armorToEquip;
    this->money = money;
    cout << "A new Inventory constructed" << endl;
    print();
}


Inventory::~Inventory(){
    //*debug
    cout << "An inventory to be destructed!!" << endl;
    print();
}


void Inventory::disarmWeapon(void){
    if(this->weapon != nullptr){
        items.push_back(this->weapon);
        this->weapon = nullptr;
    }
}


void Inventory::disarmArmor(void){
    items.push_back(this->armor);
    this->armor = nullptr;
}


int Inventory::equipWeapon(Weapon *weapon){
    
    /* If can't equip weapon */
    if(weapon == nullptr)return notFound;
    
    /* If weapon can be equipped */
    disarmWeapon();
    this->weapon = weapon;

    return succeed;
}


int Inventory::equipArmor(Armor *armor){
    
    /* If can't equip armor */
    if(armor == nullptr)return notFound;
    
    /* If armor can be equipped */
    disarmArmor();
    this->armor = armor;

    return succeed;
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

    if(item == NULL)return notFound;

    items.push_back(item);

    return succeed;
}


void Inventory::print(void) const{

    cout << "** Inventory **" << endl
        << "-----------------------------------" << endl
        << "Equipped Weapon: ";
    if(this->weapon != nullptr)
        cout << this->weapon->getName() << endl;
    else cout << " - " << endl;

    cout << "Equipped Armor: ";
    if(this->armor != nullptr)
        cout << this->armor->getName() << endl;
    else cout << " - " << endl ;

    cout << "-----------------------------------" << endl;

    int i = 0;
    //or auto (*item)->print();
    bool hasItems = false;
    for (auto item = items.begin(); item != items.end(); item++, i++) {
        cout << "inventory slot[" << i << "]: "
             << (*item)->getName() << endl; 
        hasItems = true;
        //?better print just name (*item)->print();
    }

    if(hasItems == true)
        cout << "-----------------------------------" << endl;

    cout << "money: " << money << endl;

    cout << "-----------------------------------" << endl;
}


