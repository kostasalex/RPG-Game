#include <cstring>
#include <iostream>

class Spell{

    private:
        std::string name;
        int price;
        int requiredLevel;
        
    protected:
        struct dmg{ int lb, ub; }dmg;   //Damage range 
        int mpNeeded;                   //Magical power needed

    public:
        Spell(std::string name, int defence, int price, int requiredLevel);

        int virtual useSpell(int &mp, int &stat, int &hp) = 0;


};



class IceSpell : public Spell{

    public:
        IceSpell(std::string name, int defence, int price, int requiredLevel);

        int useSpell(int &mp, int &stat, int &hp);


};



class FireSpell : public Spell{

    public:
        FireSpell(std::string name, int defence, int price, int requiredLevel);

        int useSpell(int &mp, int &stat, int &hp);


};



class LightingSpell : public Spell{

    public:
        LightingSpell(std::string name, int defence, int price, int requiredLevel);

        int useSpell(int &mp, int &stat, int &hp);


};
