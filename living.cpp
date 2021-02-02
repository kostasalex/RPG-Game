#include "living.h"

using namespace std;

/* Constructors */
Hero::Hero(string name) : Living(name){
    
    /* Init stats */
    strength = dexterity = agility = 10;
    mp = 500;
    money = 1000;
    experience = 0;
}

void Hero::print(){
    cout << "Hero " << name << " has:" << endl
    << "Strength: " << strength << endl
    << "Dexterity: " << dexterity << endl
    << "Agility: "  << agility << endl
    << "Money: "  << money << endl;
}


Warrior::Warrior(string name) : Hero(name){

    strength += 5;
    agility += 5;

}

Sorcerer::Sorcerer(string name) : Hero(name){

    dexterity += 5;
    agility += 5;

}

Paladin::Paladin(string name) : Hero(name){

    strength += 5;
    dexterity += 5;

}


/* Monsters and their types */
Monster::Monster(string name) : Living(name){

    dmg.lb = 30;
    dmg.ub = 100;
    defence = 10;
    dodge = 10;

}

void Monster::print(){
    cout << "Monster " << name << " has:" << endl
    << "Damage: " << dmg.lb << " - " <<  dmg.ub << endl
    << "Defence: " << defence << endl
    << "Dodge: "  << dodge << endl;
}

Dragon::Dragon(string name) : Monster(name){

    dmg.lb += dmg.lb;
    dmg.ub += dmg.ub;

}

Exoskeleton::Exoskeleton(string name) : Monster(name){

    defence += defence;

}

Spirit::Spirit(string name) : Monster(name){

    dodge += dodge;

}