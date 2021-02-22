#include <time.h>    
#include "spell.h"
#include "buff.h"

using namespace std;


string Spell::statMsg[3] = {"damage", "defence", "dodge"};

const string Spell::deBuffNames[Spell::spellTypes] = 
    {"Power curse", "Defence curse", "Dodge curse"};

Spell::Spell(string name, int lb_damage, int pointsAffect, \
        int price, int requiredLevel, int mpCost, int stat){
    
    this->name = name;
    this->dmg.lb = lb_damage;
    this->dmg.ub = lb_damage + this->ub_damage;
    this->pointsAffect = pointsAffect;
    this->price = price;
    this->requiredLevel = requiredLevel;
    this->mpCost = mpCost;
    this->rounds = (requiredLevel/2) + deBuffRounds;
    this->targetStat = stat;
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
    Spell(name, lb_damage, pointsAffect, price, requiredLevel, mpCost, damage)
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


Buff* IceSpell::cast(int dexterity, int &damage){

    cout << "IceSpell casted!" << endl;
    
    damage = this->getDamage(dexterity);

    return new Buff(deBuffNames[statAffect::damage],\
    getRounds(), getStat(), getPoints());    
}



FireSpell::FireSpell(string name, int lb_damage, int pointsAffect, int price, \
    int requiredLevel, int mpCost):
     Spell(name, lb_damage, pointsAffect, price, requiredLevel, mpCost, defence)
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


struct Buff* FireSpell::cast(int dexterity, int &damage){

    cout << "FireSpell casted!" << endl;
    
    damage = this->getDamage(dexterity);

    return new Buff(deBuffNames[statAffect::defence],\
    getRounds(), getStat(), getPoints());  

}



LightingSpell::LightingSpell(string name, int lb_damage, int pointsAffect, int price, \
    int requiredLevel, int mpCost): 
    Spell(name, lb_damage, pointsAffect, price, requiredLevel, mpCost, dodge)
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


struct Buff* LightingSpell::cast(int dexterity, int &damage){

    cout << "LightingSpell casted!" << endl;
    
    damage = this->getDamage(dexterity);

    return new Buff(deBuffNames[statAffect::dodge],\
    getRounds(), getStat(), getPoints());  
}