#include "item.h"

using namespace std;

/* Base class *Item* implementation */

Item::Item(string name, int price, int requiredLevel)
{
    this->name = name;
    this->price = price;
    this->requiredLevel = requiredLevel;

    //*Debug
    cout << "A new item constructed " << endl;
}

Item::~Item ()
{
    //*Debug
    cout << "Item " << name << " to be destructed!!" << endl;
}


/* Subclass *Potion* implementation */

Potion::Potion(string name, string stat, \
int price, int requiredLevel, int points) : Item(name, price, requiredLevel)
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
         << "Adding " << points << " points to " << this->stat << endl 
         << "Required level: " << getLevel() << endl
         << "Price: " << getPrice() << endl;

}


/* Subclass *Weapon* implementation */

Weapon::Weapon(string name, int damage, int price, int requiredLevel)
: Item(name, price, requiredLevel)
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
: Item(name, price, requiredLevel)
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