#include <time.h>    
#include "spell.h"

using namespace std;

int Spell::ub_damage = 50; 

string Spell::statMsg[3] = {"damage", "defence", "dodge"};

Spell::Spell(string name, int lb_damage, int pointsAffect, \
        int price, int requiredLevel, int mpCost){
    
    this->name = name;
    this->dmg.lb = lb_damage;
    this->dmg.ub = lb_damage + this->ub_damage;
    this->pointsAffect = pointsAffect;
    this->price = price;
    this->requiredLevel = requiredLevel;
    this->mpCost = mpCost;

    //*Debug
    cout << "A new spell constructed " << endl;

}

Spell::~Spell ()
{
    //*Debug
    cout << "Spell " << this->name << " to be destructed!!" << endl;
}



IceSpell::IceSpell(string name, int lb_damage, int pointsAffect, int price, \
    int requiredLevel, int mpCost): 
    Spell(name, lb_damage, pointsAffect, price, requiredLevel, mpCost)
{
    //*Debug
    cout << "IceSpell created !" << endl; 
    print();
}


IceSpell::~IceSpell(){
    //*debug
    cout << "An IceSpell to be destructed!!" << endl;
    print();
}


void IceSpell::print(void) {

    cout << "*IceSpell " << this->getName() << "*" << endl
    << "Damage: " << this->getLbDmg() << " - " 
    <<  this->getUbDmg() << endl
    <<  "Descrease enemy's damage by " << this->getPoints() << endl
    << "requires " << this->getMp() << "mp" << endl
    << "Price:" << this->getPrice() << endl;

}


void IceSpell::cast(int dexterity, int &targetStat, int &points,  int &damage){

    cout << "IceSpell casted!" << endl;
    targetStat = this->damage;
    points = this->getPoints();
    damage = this->getDamage(dexterity);

}



FireSpell::FireSpell(string name, int lb_damage, int pointsAffect, int price, \
    int requiredLevel, int mpCost):
     Spell(name, lb_damage, pointsAffect, price, requiredLevel, mpCost)
{
    //*Debug
    cout << "FireSpell created !" << endl; 
    print();
}


FireSpell::~FireSpell(){
    //*debug
    cout << "A FireSpell to be destructed!!" << endl;
    print();
}


void FireSpell::print(void) {

    cout << "*FireSpell " << this->getName() << "*" << endl
    << "Damage: " << this->getLbDmg() << " - " 
    <<  this->getUbDmg() << endl
    <<  "Descrease enemy's defence by " << this->getPoints() << endl
    << "requires " << this->getMp() << "mp" << endl
    << "Price:" << this->getPrice() << endl;

}


void FireSpell::cast(int dexterity, int &targetStat, int &points,  int &damage){

    cout << "FireSpell casted!" << endl;
    targetStat = this->defence;
    points = this->getPoints();
    damage = this->getDamage(dexterity);

}



LightingSpell::LightingSpell(string name, int lb_damage, int pointsAffect, int price, \
    int requiredLevel, int mpCost): 
    Spell(name, lb_damage, pointsAffect, price, requiredLevel, mpCost)
{
    //*Debug
    cout << "LightingSpell created !" << endl; 
    print();
}


LightingSpell::~LightingSpell(){
    //*debug
    cout << "A LightingSpell to be destructed!!" << endl;
    print();
}


void LightingSpell::print(void) {

    cout << "*LightingSpell " << this->getName() << "*" << endl
    << "Damage: " << this->getLbDmg() << " - " 
    <<  this->getUbDmg() << endl
    <<  "Descrease enemy's dodge by " << this->getPoints() << endl
    << "requires " << this->getMp() << " mp" << endl
    << "Level "  << this->getLevel() << endl
    << "Price:" << this->getPrice() << endl;

}


void LightingSpell::cast(int dexterity, int &targetStat, int &points,  int &damage){

    cout << "LightingSpell casted!" << endl;
    targetStat = this->dodge;
    points = this->getPoints();
    damage = this->getDamage(dexterity);

}