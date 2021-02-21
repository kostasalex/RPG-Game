#ifndef SPELL_H
#define SPELL_H

#include <cstring>
#include <iostream>

class Spell{

    private:
        std::string name;
        int requiredLevel;
        int mpCost;                   //Magical power needed
        int pointsAffect;
        int price;
        struct dmg{ int lb, ub; }dmg;   //Damage range 
        static const int ub_damage = 50;           //Added to lb_damage
        int rounds; //Debuff duration
        int targetStat;

    protected:
        static const std::string deBuffNames[3];
        static const int deBuffRounds = 5;
        inline int getRounds(void){return rounds;}
    public:
        static const int spellTypes = 3;
        enum statAffect{damage, defence, dodge};
        static std::string statMsg[3];

        Spell(std::string name, int lb_damage, int pointsAffect, \
        int price, int requiredLevel, int mpCost, int stat);

        virtual ~Spell() = 0;

        virtual struct Buff* cast(int dexterity, int &damage) = 0;

        void virtual print(void) = 0;


        inline int getMp(void) const {return this->mpCost;}

        inline int getLevel(void) const {return this->requiredLevel;}

        inline std::string getName(void) const {return this->name;}

        inline int getLbDmg(void){return this->dmg.lb;}

        inline int getUbDmg(void){return this->dmg.ub;}

        inline int getPoints(void){return this->pointsAffect;}

        inline int getPrice(void){return this->price;}
        
        inline int getDamage(int dexterity)
        {return ((dmg.lb + dexterity) > dmg.ub)? dmg.ub : (dmg.lb + dexterity);}

        inline int getStat(void){return targetStat;}

};



class IceSpell : public Spell{

    public:
        IceSpell(std::string name, int lb_damage, int pointsAffect, \
        int price, int requiredLevel, int mpCost);

        ~IceSpell();

        struct Buff* cast(int dexterity, int &damage)override;

        void print(void) override;

};



class FireSpell : public Spell{

    public:
        FireSpell(std::string name, int lb_damage, int pointsAffect, \
        int price, int requiredLevel, int mpCost);
        
        ~FireSpell();

        struct Buff* cast(int dexterity, int &damage)override;

        void print(void) override;


};



class LightingSpell : public Spell{

    public:
        LightingSpell(std::string name, int lb_damage, int pointsAffect, \
        int price, int requiredLevel, int mpCost);

        ~LightingSpell();

        struct Buff* cast(int dexterity, int &damage)override;

        void print(void) override;


};
#endif