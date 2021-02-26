#include <time.h>    
#include "spell.h"


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
}

Spell::~Spell (){}



IceSpell::IceSpell(string name, int lb_damage, int pointsAffect, int price, \
    int requiredLevel, int mpCost): 
    Spell(name, lb_damage, pointsAffect, price, requiredLevel, mpCost, damage)
{
}


IceSpell::~IceSpell(){}


void IceSpell::print(void) {

    cout << "*IceSpell " << this->getName() << "*" << endl
    << "Damage: " << this->getLbDmg() << " - " 
    <<  this->getUbDmg() << endl
    << "Level: "  << this->getLevel() << endl
    <<  "Effect: -" << this->getPoints() <<" damage" << endl
    << "Mp cost: " << this->getMp() << endl
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
}


FireSpell::~FireSpell(){}


void FireSpell::print(void) {

    cout << "*FireSpell " << this->getName() << "*" << endl
    << "Damage: " << this->getLbDmg() << " - " 
    <<  this->getUbDmg() << endl
    << "Level: "  << this->getLevel() << endl
    <<  "Effect: -" << this->getPoints() <<" defence" << endl
    << "Mp cost: " << this->getMp() << endl
    << "Price:" << this->getPrice() << endl;
}


Buff* FireSpell::cast(int dexterity, int &damage){

    cout << "FireSpell casted!" << endl;
    
    damage = this->getDamage(dexterity);

    return new Buff(deBuffNames[statAffect::defence],\
    getRounds(), getStat(), getPoints());  

}



LightingSpell::LightingSpell(string name, int lb_damage, int pointsAffect, int price, \
    int requiredLevel, int mpCost): 
    Spell(name, lb_damage, pointsAffect, price, requiredLevel, mpCost, dodge)
{
}


LightingSpell::~LightingSpell(){}


void LightingSpell::print(void) {

    cout << "*LightingSpell " << this->getName() << "*" << endl
    << "Damage: " << this->getLbDmg() << " - " 
    <<  this->getUbDmg() << endl
    << "Level: "  << this->getLevel() << endl
    <<  "Effect: -" << this->getPoints() <<" dodge" << endl
    << "Mp cost: " << this->getMp() << endl
    << "Price:" << this->getPrice() << endl;

}


Buff* LightingSpell::cast(int dexterity, int &damage){

    cout << "LightingSpell casted!" << endl;
    
    damage = this->getDamage(dexterity);

    return new Buff(deBuffNames[statAffect::dodge],\
    getRounds(), getStat(), getPoints());  
}