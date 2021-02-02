#include <cstring>
#include <iostream>

/* Base class for all livings in game,
   heroes and monsters               */
class Living{

    protected: 
        std::string name;
        int level;
        int hp;      //Health Power
    
    public:
        Living(std::string name)
        { this->name = name; level = 1; hp = 1000; }

};


/* Heroes and their classes */
class Hero : public Living{
    
    protected: //?Maybe protected
        int mp;        //Magic Power
        int strength;  //Adds when attack.
        int dexterity; //Adds damage to a spell.
        int agility;   //Change to dodge an attack.
        int money;
        int experience;

    public:
        Hero(std::string name);
        
        void print();
        //* A hero should:
        // have inventory
        // Wear armor 
        // Hold weapon

        void levelUp(void);
        //todo:
        //attack
        //use spell
        //defence

};

class Warrior : public Hero{
    //Has:
    // + strength
    // + agility
    public:
        Warrior(std::string name);
};

class Sorcerer : public Hero{
    //Has:
    // + dexterity
    // + agility
    public:
        Sorcerer(std::string name);
};

class Paladin : public Hero{
    //Has:
    // + strength
    // + dexterity
    public:
        Paladin(std::string name);
};


/* Monsters and their types */
class Monster : public Living{

    protected:
    //?Temporary type.
        struct dmg{ int lb, ub; }dmg;   //Damage range 
        int defence;              
        int dodge;
    
    public:
        Monster(std::string name);
        void print();

};

class Dragon : public Monster{
    //Has:
    // + dmg
    public:
        Dragon(std::string name);
};

class Exoskeleton : public Monster{
    //Has:
    // + armor
    public:
        Exoskeleton(std::string name);
};

class Spirit : public Monster{
    //Has:
    // + dodge
    public:
        Spirit(std::string name);
};